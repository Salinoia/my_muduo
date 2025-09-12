#pragma once
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "QueryCallback.h"

namespace sql {
    class ResultSet;
}
template <typename T>
class BlockingQueuePro;
class MySQLConn;
class SQLOperation;

class MySQLConnPool {
public:
    static MySQLConnPool* GetInstance(const std::string& db);
    void InitPool(const std::string& url, int pool_size);
    QueryCallback Query(const std::string& sql, std::function<void(std::unique_ptr<sql::ResultSet>)>&& cb);

private:
    MySQLConnPool(const std::string& db) : database_(db) {}
    ~MySQLConnPool();
    std::string database_;
    std::vector<MySQLConn*> pool_;
    static std::unordered_map<std::string, MySQLConnPool*> instances_;
    BlockingQueuePro<SQLOperation*> *task_queue_;
};
