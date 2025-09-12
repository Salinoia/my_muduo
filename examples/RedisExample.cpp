#include <iostream>

#include "RedisClient.h"
#include "RedisPool.h"

int main() {
    // Create pool with single connection for demonstration.
    RedisPool pool("127.0.0.1", 6379, 1);
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

