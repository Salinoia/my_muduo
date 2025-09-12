#pragma once

#include <string>
namespace sql {
    class Driver;
    class Connection;
    class SQLException;
    class ResultSet;
}  // namespace sql
template <typename T>
class BlockingQueuePro;

class MySQLWorker;
class SQLOperation;

struct MySQLConnInfo {
    explicit MySQLConnInfo(const std::string& info, const std::string db);
    std::string user;
    std::string password;
    std::string database;
    std::string url;
};

class MySQLConn {
public:
    MySQLConn(const std::string& info, const std::string& db, BlockingQueuePro<SQLOperation*>& task_queue);
    ~MySQLConn();

    int Open();
    void Close();

    sql::ResultSet* Query(const std::string& sql);

private:
    void HandleExecption(sql::SQLException& e);
    sql::Driver* driver_;
    sql::Connection* conn_;
    MySQLWorker* worker_;
    MySQLConnInfo info_;
};