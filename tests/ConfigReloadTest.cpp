#include <cassert>
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include "ConfigManager.h"

void TestHotReload() {
    const std::string file = "config_reload_test.json";
    {
        std::ofstream ofs(file);
        ofs << "{\n  \"value\": 1\n}\n";
    }
    ConfigManager& mgr = ConfigManager::Instance();
    assert(mgr.Load(file));
    mgr.StartWatch(200);
    assert(mgr.GetInt("value") == 1);

    std::this_thread::sleep_for(std::chrono::seconds(1));
    {
        std::ofstream ofs(file);
        ofs << "{\n  \"value\": 2\n}\n";
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));
    assert(mgr.GetInt("value") == 2);
    mgr.StopWatch();
    std::remove(file.c_str());
    std::cout << "Hot reload test passed" << std::endl;
}

int main() {
    TestHotReload();
    return 0;
}
