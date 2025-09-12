#include <iostream>

#include "RedisClient.h"
#include "RedisPool.h"
#include "ConfigManager.h"

int main() {
    // Load configuration
    ConfigManager::Instance().Load("config/redis.yaml");
    std::string host = ConfigManager::Instance().GetString("host", "127.0.0.1");
    int port = ConfigManager::Instance().GetInt("port", 6379);
    int poolSize = ConfigManager::Instance().GetInt("poolSize", 1);

    RedisPool pool(host, port, poolSize);
    auto client = pool.GetClient();
    if (!client) {
        std::cerr << "No redis client available" << std::endl;
        return -1;
    }
    client->Set("hello", "world");
    std::string value;
    if (client->Get("hello", value)) {
        std::cout << "hello => " << value << std::endl;
    }
    client->Del("hello");
    return 0;
}
