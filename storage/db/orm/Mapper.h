#pragma once

#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

#include "storage/db/ConnectionPool/ConnectionPool.h"
#include "Entity.h"

template <typename T>
class Mapper {
public:
    bool insert(const T& obj);
    bool update(const T& obj);
    std::unique_ptr<T> selectById(const decltype(std::declval<T>().id)& id);
    bool deleteById(const decltype(std::declval<T>().id)& id);

private:
    template <typename Tuple, typename Fn, std::size_t... I>
    static void for_each(Tuple&& tup, Fn&& fn, std::index_sequence<I...>) {
        (fn(std::get<I>(tup), std::integral_constant<std::size_t, I>{}), ...);
    }

    template <typename Tuple, typename Fn>
    static void for_each(Tuple&& tup, Fn&& fn) {
        for_each(std::forward<Tuple>(tup), std::forward<Fn>(fn),
                 std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<Tuple>>>{});
    }

    static void appendInsert(std::stringstream& cols, std::stringstream& vals,
                             const std::string& name, bool& first);

    template <typename FieldType>
    static void bind(sql::PreparedStatement* stmt, int index, const FieldType& value);

    template <typename FieldType>
    static FieldType fetch(sql::ResultSet* rs, const std::string& col);
};

template <typename T>
void Mapper<T>::appendInsert(std::stringstream& cols, std::stringstream& vals,
                             const std::string& name, bool& first) {
    if (!first) {
        cols << ",";
        vals << ",";
    }
    first = false;
    cols << name;
    vals << "?";
}

template <typename T>
template <typename FieldType>
void Mapper<T>::bind(sql::PreparedStatement* stmt, int index, const FieldType& value) {
    if constexpr (std::is_integral_v<FieldType>)
        stmt->setInt(index, static_cast<int>(value));
    else if constexpr (std::is_floating_point_v<FieldType>)
        stmt->setDouble(index, static_cast<double>(value));
    else if constexpr (std::is_same_v<FieldType, std::string>)
        stmt->setString(index, value);
    else
        stmt->setString(index, std::to_string(value));
}

template <typename T>
template <typename FieldType>
FieldType Mapper<T>::fetch(sql::ResultSet* rs, const std::string& col) {
    if constexpr (std::is_integral_v<FieldType>)
        return rs->getInt(col);
    else if constexpr (std::is_floating_point_v<FieldType>)
        return rs->getDouble(col);
    else if constexpr (std::is_same_v<FieldType, std::string>)
        return rs->getString(col);
    else
        return FieldType{};
}

template <typename T>
bool Mapper<T>::insert(const T& obj) {
    auto conn = ConnectionPool::Instance().Acquire();
    if (!conn) return false;
    auto fields = T::Fields();
    std::stringstream cols, vals;
    cols << "INSERT INTO " << T::TableName() << "(";
    vals << "VALUES(";
    bool first = true;
    for_each(fields, [&](auto& field, auto) {
        appendInsert(cols, vals, field.first, first);
    });
    cols << ") " << vals.str() << ")";
    std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(cols.str()));
    int idx = 1;
    for_each(fields, [&](auto& field, auto) {
        bind(stmt.get(), idx++, obj.*(field.second));
    });
    stmt->execute();
    return true;
}

template <typename T>
bool Mapper<T>::update(const T& obj) {
    auto conn = ConnectionPool::Instance().Acquire();
    if (!conn) return false;
    auto fields = T::Fields();
    std::stringstream sql;
    sql << "UPDATE " << T::TableName() << " SET ";
    bool first = true;
    for_each(fields, [&](auto& field, auto index) {
        if (index == 0) return; // skip id
        if (!first) sql << ",";
        first = false;
        sql << field.first << "=?";
    });
    sql << " WHERE " << std::get<0>(fields).first << "=?";
    std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(sql.str()));
    int idx = 1;
    for_each(fields, [&](auto& field, auto index) {
        if (index == 0) return;
        bind(stmt.get(), idx++, obj.*(field.second));
    });
    bind(stmt.get(), idx, obj.*(std::get<0>(fields).second));
    stmt->executeUpdate();
    return true;
}

template <typename T>
std::unique_ptr<T> Mapper<T>::selectById(const decltype(std::declval<T>().id)& id) {
    auto conn = ConnectionPool::Instance().Acquire();
    if (!conn) return nullptr;
    auto fields = T::Fields();
    std::stringstream sql;
    sql << "SELECT ";
    bool first = true;
    for_each(fields, [&](auto& field, auto) {
        if (!first) sql << ",";
        first = false;
        sql << field.first;
    });
    sql << " FROM " << T::TableName() << " WHERE " << std::get<0>(fields).first << "=?";
    std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(sql.str()));
    bind(stmt.get(), 1, id);
    std::unique_ptr<sql::ResultSet> rs(stmt->executeQuery());
    if (!rs->next()) return nullptr;
    auto res = std::make_unique<T>();
    for_each(fields, [&](auto& field, auto) {
        using FieldType = std::remove_reference_t<decltype(res->*(field.second))>;
        res->*(field.second) = fetch<FieldType>(rs.get(), field.first);
    });
    return res;
}

template <typename T>
bool Mapper<T>::deleteById(const decltype(std::declval<T>().id)& id) {
    auto conn = ConnectionPool::Instance().Acquire();
    if (!conn) return false;
    auto fields = T::Fields();
    std::stringstream sql;
    sql << "DELETE FROM " << T::TableName() << " WHERE " << std::get<0>(fields).first << "=?";
    std::unique_ptr<sql::PreparedStatement> stmt(conn->prepareStatement(sql.str()));
    bind(stmt.get(), 1, id);
    stmt->executeUpdate();
    return true;
}

