#pragma once

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>

#include "RedisClient.h"

// Simple thread-safe pool for Redis connections.
class RedisPool {
public:
    RedisPool(const std::string& host, int port, size_t pool_size,
              const std::string& password = "", int timeout_ms = 1000);
    ~RedisPool();

    // Acquire a client from pool. Returned shared_ptr will automatically
    // return client back to pool when destroyed.
    std::shared_ptr<RedisClient> GetClient();

private:
    void Release(RedisClient* client);

    std::string host_;
    int port_;
    std::string password_;
    struct timeval timeout_;

    std::mutex mutex_;
    std::condition_variable cond_;
    std::queue<RedisClient*> clients_;
};

