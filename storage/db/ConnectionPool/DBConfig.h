#pragma once
#include <cstdint>
#include <string>

struct DBConfig {
    std::string host{"127.0.0.1"};
    uint16_t port{3306};
    std::string user{"root"};
    std::string password{};
    std::string database{};
    std::size_t pool_size{1};

    std::string url() const {
        return "tcp://" + host + ":" + std::to_string(port);
    }
};
