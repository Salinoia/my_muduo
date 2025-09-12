#include "RedisPool.h"
#include "ConfigManager.h"

#include <iostream>
#include <mutex>

RedisPool& RedisPool::Instance() {
    static RedisPool instance;
    static std::once_flag flag;
    std::call_once(flag, [&]() {
        RedisConfig cfg = ConfigManager::LoadTyped<RedisConfig>("config/redis.yaml");
        instance.Init(cfg);
    });
    return instance;
}

void RedisPool::Init(const RedisConfig& config) {
    if (initialized_) return;
    host_ = config.host;
    port_ = config.port;
    password_ = config.password;
    timeout_.tv_sec = config.timeout_ms / 1000;
    timeout_.tv_usec = (config.timeout_ms % 1000) * 1000;
    for (size_t i = 0; i < config.poolSize; ++i) {
        auto* client = new RedisClient(host_, port_, password_, timeout_);
        if (client->Connect()) {
            clients_.push(client);
        } else {
            delete client;
            std::cerr << "Failed to create redis connection" << std::endl;
        }
    }
    initialized_ = true;
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

