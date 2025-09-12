#include "ConnectionPool.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>

ConnectionPool& ConnectionPool::Instance() {
    static ConnectionPool instance;
    return instance;
}

void ConnectionPool::Init(const DBConfig& config) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (initialized_) return;
    config_ = config;
    sql::Driver* driver = get_driver_instance();
    for (std::size_t i = 0; i < config_.pool_size; ++i) {
        sql::Connection* conn = driver->connect(config_.url(), config_.user, config_.password);
        conn->setSchema(config_.database);
        pool_.push(conn);
    }
    initialized_ = true;
}

sql::Connection* ConnectionPool::CreateConnection() {
    sql::Driver* driver = get_driver_instance();
    sql::Connection* conn = driver->connect(config_.url(), config_.user, config_.password);
    conn->setSchema(config_.database);
    return conn;
}

std::shared_ptr<sql::Connection> ConnectionPool::Acquire() {
    std::unique_lock<std::mutex> lock(mutex_);
    if (!initialized_) return nullptr;
    cond_.wait(lock, [this] { return !pool_.empty(); });
    sql::Connection* conn = pool_.front();
    pool_.pop();
    auto deleter = [this](sql::Connection* c) { this->Release(c); };
    return std::shared_ptr<sql::Connection>(conn, deleter);
}

void ConnectionPool::Release(sql::Connection* conn) {
    std::lock_guard<std::mutex> lock(mutex_);
    pool_.push(conn);
    cond_.notify_one();
}
