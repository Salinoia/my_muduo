#include "Logger.h"
#include "ConfigManager.h"
#include <chrono>
#include <filesystem>
#include <iostream>

int main() {
    ConfigManager::Instance().Load("config/logger.json");
    bool async = ConfigManager::Instance().GetBool("async", true);
    int sizeMb = ConfigManager::Instance().GetInt("roll_size_mb", 5);
    Logger::instance().setRollSize(sizeMb * 1024 * 1024);
    if (async) {
        Logger::instance().enableAsync();
    }
    std::filesystem::create_directories("logs");
    Logger::instance().setOutputToFile("logs/stress.log");

    const int count = 100000;
    auto start = std::chrono::steady_clock::now();
    for (int i = 0; i < count; ++i) {
        LOG_INFO("stress log line %d", i);
    }
    auto end = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << (async ? "async" : "sync") << " mode: " << count << " logs in " << ms << " ms" << std::endl;
    return 0;
}
