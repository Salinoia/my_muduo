#include <cassert>
#include <fstream>
#include <iostream>
#include "ConfigManager.h"
#include "ConfigManager.cpp"

int main() {
    const std::string filename = "test_config.json";
    std::ofstream ofs(filename);
    ofs << "{\n  \"port\": \"8080\",\n  \"debug\": \"true\",\n  \"pi\": \"3.14\"\n}";
    ofs.close();

    auto& cm = ConfigManager::Instance();
    assert(cm.Load(filename));
    assert(cm.GetInt("port") == 8080);
    assert(cm.GetBool("debug"));
    assert(cm.GetDouble("pi") == 3.14);
    assert(cm.GetString("missing", "default") == "default");

    std::remove(filename.c_str());
    std::cout << "ConfigManagerTest passed" << std::endl;
    return 0;
}
