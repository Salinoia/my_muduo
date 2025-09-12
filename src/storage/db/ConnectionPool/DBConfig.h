#pragma once
#include <cstdint>
#include <string>
#include <nlohmann/json.hpp>

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

inline void from_json(const nlohmann::json& j, DBConfig& c) {
    j.at("host").get_to(c.host);
    j.at("port").get_to(c.port);
    j.at("user").get_to(c.user);
    j.at("password").get_to(c.password);
    j.at("database").get_to(c.database);
    if (j.contains("pool_size")) j.at("pool_size").get_to(c.pool_size);
}
