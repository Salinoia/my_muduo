#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

#include <cppconn/connection.h>

#include "DBConfig.h"

class ConnectionPool {
public:
    static ConnectionPool& Instance();

    void Init(const DBConfig& config);

    std::shared_ptr<sql::Connection> Acquire();
    void Release(sql::Connection* conn);

private:
    ConnectionPool() = default;
    ConnectionPool(const ConnectionPool&) = delete;
    ConnectionPool& operator=(const ConnectionPool&) = delete;

    sql::Connection* CreateConnection();

    std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<sql::Connection*> pool_;
    DBConfig config_;
    bool initialized_{false};
};
