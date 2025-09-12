#pragma once
#include <string>
#include <mutex>
#include <thread>
#include <filesystem>
#include <nlohmann/json.hpp>
#include <yaml-cpp/yaml.h>

// Configuration manager supporting hierarchical JSON/YAML files. The loaded
// configuration is stored as a JSON object which allows easy mapping to
// strongly typed structures. It can optionally watch files for changes and
// reload automatically.
class ConfigManager {
public:
    static ConfigManager& Instance();

    // Load configuration from a JSON or YAML file.
    bool Load(const std::string& filename);

    // Start/stop watching the loaded file for modifications.
    void StartWatch(int intervalMs = 1000);
    void StopWatch();

    std::string GetString(const std::string& key, const std::string& defaultValue = "") const;
    int GetInt(const std::string& key, int defaultValue = 0) const;
    double GetDouble(const std::string& key, double defaultValue = 0.0) const;
    bool GetBool(const std::string& key, bool defaultValue = false) const;

    template <typename T>
    static T LoadTyped(const std::string& filename) {
        ConfigManager mgr;
        if (!mgr.Load(filename)) return T{};
        std::lock_guard<std::mutex> lock(mgr.mutex_);
        return mgr.root_.get<T>();
    }

    ~ConfigManager();

private:
    ConfigManager() = default;
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    bool ParseJson(const std::string& content);
    bool ParseYaml(const std::string& content);
    void WatchLoop(int intervalMs);

    mutable std::mutex mutex_;
    nlohmann::json root_;
    std::string filename_;
    std::filesystem::file_time_type lastWriteTime_{};

    bool watching_ = false;
    std::thread watcher_;
};

