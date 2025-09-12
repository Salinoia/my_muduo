#pragma once

#include <chrono>
#include <memory>
#include <string>
#include <unordered_map>

#include "router/Router.h"

class SessionStore {
public:
    virtual ~SessionStore() = default;
    virtual std::unordered_map<std::string, std::string> get(const std::string& id) = 0;
    virtual void set(const std::string& id, const std::unordered_map<std::string, std::string>& data, int ttlSeconds) = 0;
    virtual void destroy(const std::string& id) = 0;
};

class Session {
public:
    Session(const std::string& id, std::shared_ptr<SessionStore> store, int ttlSeconds);

    const std::string& id() const { return id_; }
    std::string get(const std::string& key) const;
    void set(const std::string& key, const std::string& value);
    void save();

private:
    std::string id_;
    std::unordered_map<std::string, std::string> data_;
    std::shared_ptr<SessionStore> store_;
    int ttlSeconds_;
    bool dirty_;
};

class MemorySessionStore : public SessionStore {
public:
    std::unordered_map<std::string, std::string> get(const std::string& id) override;
    void set(const std::string& id, const std::unordered_map<std::string, std::string>& data, int ttlSeconds) override;
    void destroy(const std::string& id) override;

private:
    struct Entry {
        std::unordered_map<std::string, std::string> data;
        std::chrono::steady_clock::time_point expire;
    };
    std::unordered_map<std::string, Entry> sessions_;
};

class SessionInterceptor : public Interceptor {
public:
    SessionInterceptor(std::shared_ptr<SessionStore> store, int ttlSeconds = 1800);
    void Handle(HttpRequest& req, HttpResponse& res, Next next) override;

private:
    std::shared_ptr<SessionStore> store_;
    int ttlSeconds_;
};
