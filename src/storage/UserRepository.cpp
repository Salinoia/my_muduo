#include "UserRepository.h"

#include "cache/RedisPool.h"
#include "db/ConnectionPool/ConnectionPool.h"

std::string UserRepository::getUserName(int id) {
    auto redis = RedisPool::Instance().GetClient();
    std::string key = "user:" + std::to_string(id);
    std::string name;
    if (redis && redis->Get(key, name)) {
        return name;
    }

    auto conn = ConnectionPool::Instance().Acquire();
    if (conn) {
        // Placeholder for real database query.
        name = "db-user";
        if (redis) {
            redis->Set(key, name);
            redis->Expire(key, 3600);
        }
    }
    return name;
}

bool UserRepository::updateUserName(int id, const std::string& name) {
    auto conn = ConnectionPool::Instance().Acquire();
    if (!conn) return false;
    // Placeholder for update logic.
    (void)name;
    auto redis = RedisPool::Instance().GetClient();
    if (redis) {
        redis->Del("user:" + std::to_string(id));
    }
    return true;
}

