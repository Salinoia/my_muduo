#include <cassert>
#include <iostream>
#include <vector>

#include "RedisClient.h"

int main() {
    RedisClient client("127.0.0.1", 6379);
    if (!client.Connect()) {
        std::cout << "Redis server not available, skipping RedisClientTest" << std::endl;
        return 0;
    }

    // Clean up any existing data
    client.Del("rank:test");

    double newScore = 0.0;
    assert(client.ZIncrBy("rank:test", 1.0, "alice", newScore));
    assert(client.ZIncrBy("rank:test", 2.0, "bob", newScore));

    std::vector<std::pair<std::string, double>> top;
    assert(client.ZRevRangeWithScores("rank:test", 0, -1, top));
    assert(top.size() == 2);
    assert(top[0].first == "bob");
    assert(top[1].first == "alice");

    std::cout << "RedisClientTest passed" << std::endl;
    return 0;
}
