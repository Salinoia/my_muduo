#include "session/Session.h"

#include <random>

Session::Session(const std::string& id, std::shared_ptr<SessionStore> store, int ttlSeconds) : id_(id), store_(std::move(store)), ttlSeconds_(ttlSeconds), dirty_(false) {
    data_ = store_->get(id_);
}

thread_local std::shared_ptr<Session> Session::tlsSession_{};

void Session::SetCurrent(std::shared_ptr<Session> session) {
    tlsSession_ = std::move(session);
}

std::shared_ptr<Session> Session::Current() {
    return tlsSession_;
}

std::string Session::get(const std::string& key) const {
    auto it = data_.find(key);
    if (it != data_.end()) {
        return it->second;
    }
    return "";
}

void Session::set(const std::string& key, const std::string& value) {
    data_[key] = value;
    dirty_ = true;
}

void Session::save() {
    if (dirty_) {
        store_->set(id_, data_, ttlSeconds_);
        dirty_ = false;
    }
}

std::unordered_map<std::string, std::string> MemorySessionStore::get(const std::string& id) {
    auto now = std::chrono::steady_clock::now();
    auto it = sessions_.find(id);
    if (it != sessions_.end()) {
        if (it->second.expire > now) {
            return it->second.data;
        }
        sessions_.erase(it);
    }
    return {};
}

void MemorySessionStore::set(const std::string& id, const std::unordered_map<std::string, std::string>& data, int ttlSeconds) {
    sessions_[id] = {data, std::chrono::steady_clock::now() + std::chrono::seconds(ttlSeconds)};
}

void MemorySessionStore::destroy(const std::string& id) {
    sessions_.erase(id);
}

namespace {
std::string parseCookie(const std::string& header, const std::string& key) {
    std::string pattern = key + "=";
    auto pos = header.find(pattern);
    if (pos == std::string::npos)
        return "";
    pos += pattern.size();
    auto end = header.find(';', pos);
    return header.substr(pos, end - pos);
}

std::string generateId() {
    static std::mt19937_64 rng{std::random_device{}()};
    static const char chars[] = "0123456789abcdef";
    std::string id(16, '0');
    for (char& c : id) {
        c = chars[rng() % (sizeof(chars) - 1)];
    }
    return id;
}
}  // namespace

SessionInterceptor::SessionInterceptor(std::shared_ptr<SessionStore> store, int ttlSeconds) : store_(std::move(store)), ttlSeconds_(ttlSeconds) {}

void SessionInterceptor::Handle(HttpRequest& req, HttpResponse& res, Next next) {
    std::string cookie = req.getHeader("Cookie");
    std::string id = parseCookie(cookie, "SESSIONID");
    if (id.empty()) {
        id = generateId();
    }
    auto session = std::make_shared<Session>(id, store_, ttlSeconds_);
    Session::SetCurrent(session);
    req.setSession(session);
    next();
    session->save();
    res.addHeader("Set-Cookie", "SESSIONID=" + session->id() + "; Path=/; HttpOnly");
    Session::SetCurrent(nullptr);
}
