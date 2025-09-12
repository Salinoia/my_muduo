#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <nlohmann/json.hpp>

#include "RedisClient.h"

struct RedisConfig {
    std::string host{"127.0.0.1"};
    int port{6379};
    size_t poolSize{1};
    std::string password{};
    int timeout_ms{1000};
};

inline void from_json(const nlohmann::json& j, RedisConfig& c) {
    j.at("host").get_to(c.host);
    j.at("port").get_to(c.port);
    if (j.contains("poolSize")) j.at("poolSize").get_to(c.poolSize);
    if (j.contains("password")) j.at("password").get_to(c.password);
    if (j.contains("timeout_ms")) j.at("timeout_ms").get_to(c.timeout_ms);
}

// Simple thread-safe pool for Redis connections.
class RedisPool {
public:
    static RedisPool& Instance();

    void Init(const RedisConfig& config);
    ~RedisPool();

    // Acquire a client from pool. Returned shared_ptr will automatically
    // return client back to pool when destroyed.
    std::shared_ptr<RedisClient> GetClient();

private:
    void Release(RedisClient* client);

    std::string host_;
    int port_{};
    std::string password_;
    struct timeval timeout_{};

    std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<RedisClient*> clients_;
    bool initialized_{false};
    RedisPool() = default;
    RedisPool(const RedisPool&) = delete;
    RedisPool& operator=(const RedisPool&) = delete;
};

