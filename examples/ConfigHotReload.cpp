#include <iostream>
#include <thread>
#include "ConfigManager.h"

int main() {
    ConfigManager& mgr = ConfigManager::Instance();
    if (!mgr.Load("config/hotreload.json")) {
        std::cerr << "Failed to load config" << std::endl;
        return -1;
    }
    mgr.StartWatch(200);
    for (int i = 0; i < 5; ++i) {
        std::cout << "value=" << mgr.GetInt("value", -1) << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    mgr.StopWatch();
    return 0;
}
