#pragma once

#include <string>
#include <sys/time.h>
#include <hiredis/hiredis.h>

// Simple Redis client managing a single connection.
// Provides basic Get/Set/Del operations with optional authentication
// and timeout configuration.
class RedisClient {
public:
    RedisClient(const std::string& host, int port,
                const std::string& password = "",
                const struct timeval& timeout = {1, 500000});
    ~RedisClient();

    // Connect to redis server. Returns true on success.
    bool Connect();

    bool Get(const std::string& key, std::string& value);
    bool Set(const std::string& key, const std::string& value);
    bool Del(const std::string& key);

    bool IsConnected() const { return context_ != nullptr; }

private:
    bool EnsureConnected();

    redisContext* context_;
    std::string host_;
    int port_;
    std::string password_;
    struct timeval timeout_;
};

