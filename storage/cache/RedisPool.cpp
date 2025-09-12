#include "RedisPool.h"

#include <iostream>

RedisPool::RedisPool(const std::string& host, int port, size_t pool_size,
                     const std::string& password, int timeout_ms)
    : host_(host), port_(port), password_(password) {
    timeout_.tv_sec = timeout_ms / 1000;
    timeout_.tv_usec = (timeout_ms % 1000) * 1000;
    for (size_t i = 0; i < pool_size; ++i) {
        auto* client = new RedisClient(host_, port_, password_, timeout_);
        if (client->Connect()) {
            clients_.push(client);
        } else {
            delete client;
            std::cerr << "Failed to create redis connection" << std::endl;
        }
    }
}

RedisPool::~RedisPool() {
    while (!clients_.empty()) {
        delete clients_.front();
        clients_.pop();
    }
}

std::shared_ptr<RedisClient> RedisPool::GetClient() {
    std::unique_lock<std::mutex> lock(mutex_);
    cond_.wait(lock, [this] { return !clients_.empty(); });
    RedisClient* client = clients_.front();
    clients_.pop();
    lock.unlock();
    // Ensure connection alive
    if (!client->IsConnected()) {
        client->Connect();
    }
    return std::shared_ptr<RedisClient>(client, [this](RedisClient* c) { this->Release(c); });
}

void RedisPool::Release(RedisClient* client) {
    std::lock_guard<std::mutex> lock(mutex_);
    clients_.push(client);
    cond_.notify_one();
}

