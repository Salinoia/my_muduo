#pragma once
#include <cppconn/resultset.h>

#include <future>
#include <memory>
#include <string>

namespace sql {
    class ResultSet;
}
class MySQLConn;

class SQLOperation {
public:
    explicit SQLOperation(const std::string& sql) : sql_(sql) {}
    void Execute(MySQLConn* conn);

    std::future<std::unique_ptr<sql::ResultSet>> GetFuture() {  // 通过 GetFuture 获取私有 promise 的 future
        return promise_.get_future();
    }

private:
    std::string sql_;
    std::promise<std::unique_ptr<sql::ResultSet>> promise_;
};