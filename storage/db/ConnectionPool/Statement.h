#pragma once
#include <memory>
#include <string>

#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

class Statement {
public:
    explicit Statement(sql::Connection* conn) : conn_(conn) {}

    std::unique_ptr<sql::ResultSet> Select(const std::string& query) {
        std::unique_ptr<sql::PreparedStatement> stmt(conn_->prepareStatement(query));
        return std::unique_ptr<sql::ResultSet>(stmt->executeQuery());
    }

    int Insert(const std::string& query) { return ExecuteUpdate(query); }
    int Update(const std::string& query) { return ExecuteUpdate(query); }
    int Delete(const std::string& query) { return ExecuteUpdate(query); }

private:
    int ExecuteUpdate(const std::string& query) {
        std::unique_ptr<sql::PreparedStatement> stmt(conn_->prepareStatement(query));
        return stmt->executeUpdate();
    }
    sql::Connection* conn_;
};
