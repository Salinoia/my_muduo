#include <iostream>

#include "RedisClient.h"
#include "RedisPool.h"

int main() {
    auto& pool = RedisPool::Instance();
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
