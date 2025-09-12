#include "ConfigManager.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <chrono>

namespace {
std::string Trim(const std::string& s) {
    size_t start = 0;
    while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) ++start;
    size_t end = s.size();
    while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) --end;
    return s.substr(start, end - start);
}
}

ConfigManager& ConfigManager::Instance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::~ConfigManager() { StopWatch(); }

bool ConfigManager::Load(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs.is_open()) return false;
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

    bool ok = false;
    if (filename.size() >= 5) {
        auto ext = filename.substr(filename.find_last_of('.') + 1);
        if (ext == "json") ok = ParseJson(content);
        else if (ext == "yml" || ext == "yaml") ok = ParseYaml(content);
    }
    if (!ok) ok = ParseJson(content) || ParseYaml(content);
    if (ok) {
        filename_ = filename;
        lastWriteTime_ = std::filesystem::last_write_time(filename_);
    }
    return ok;
}

void ConfigManager::StartWatch(int intervalMs) {
    StopWatch();
    if (filename_.empty()) return;
    watching_ = true;
    watcher_ = std::thread(&ConfigManager::WatchLoop, this, intervalMs);
}

void ConfigManager::StopWatch() {
    watching_ = false;
    if (watcher_.joinable()) watcher_.join();
}

void ConfigManager::WatchLoop(int intervalMs) {
    while (watching_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(intervalMs));
        auto cur = std::filesystem::last_write_time(filename_);
        if (cur != lastWriteTime_) {
            Load(filename_);
        }
    }
}

namespace {
nlohmann::json YamlToJson(const YAML::Node& node) {
    if (node.IsScalar()) {
        const std::string s = node.as<std::string>();
        if (s == "true" || s == "false") return node.as<bool>();
        try {
            if (s.find('.') != std::string::npos) return std::stod(s);
            return std::stoll(s);
        } catch (...) {
            return s;
        }
    } else if (node.IsSequence()) {
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& it : node) arr.push_back(YamlToJson(it));
        return arr;
    } else if (node.IsMap()) {
        nlohmann::json obj = nlohmann::json::object();
        for (const auto& it : node) obj[it.first.as<std::string>()] = YamlToJson(it.second);
        return obj;
    }
    return {};
}

nlohmann::json::json_pointer MakePointer(const std::string& key) {
    std::string ptr = "/";
    for (char c : key) ptr += (c == '.' ? '/' : c);
    return nlohmann::json::json_pointer(ptr);
}
}

bool ConfigManager::ParseJson(const std::string& content) {
    std::lock_guard<std::mutex> lock(mutex_);
    try {
        root_ = nlohmann::json::parse(content);
    } catch (...) {
        return false;
    }
    return !root_.is_null();
}

bool ConfigManager::ParseYaml(const std::string& content) {
    std::lock_guard<std::mutex> lock(mutex_);
    try {
        YAML::Node node = YAML::Load(content);
        root_ = YamlToJson(node);
    } catch (...) {
        return false;
    }
    return !root_.is_null();
}

std::string ConfigManager::GetString(const std::string& key, const std::string& defaultValue) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto ptr = MakePointer(key);
    if (!root_.contains(ptr)) return defaultValue;
    const auto& v = root_.at(ptr);
    if (v.is_string()) return v.get<std::string>();
    return v.dump();
}

int ConfigManager::GetInt(const std::string& key, int defaultValue) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto ptr = MakePointer(key);
    if (!root_.contains(ptr)) return defaultValue;
    try { return root_.at(ptr).get<int>(); } catch (...) { return defaultValue; }
}

double ConfigManager::GetDouble(const std::string& key, double defaultValue) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto ptr = MakePointer(key);
    if (!root_.contains(ptr)) return defaultValue;
    try { return root_.at(ptr).get<double>(); } catch (...) { return defaultValue; }
}

bool ConfigManager::GetBool(const std::string& key, bool defaultValue) const {
    std::lock_guard<std::mutex> lock(mutex_);
    auto ptr = MakePointer(key);
    if (!root_.contains(ptr)) return defaultValue;
    try { return root_.at(ptr).get<bool>(); } catch (...) { return defaultValue; }
}

