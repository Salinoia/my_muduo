#pragma once

#include <map>
#include <memory>
#include <string>
#include <nlohmann/json.hpp>

class Session;  // forward declaration

class HttpRequest {
public:
    enum Method {
        kInvalid,
        kGet,
        kPost,
        kHead,
        kPut,
        kDelete,
    };

    enum Version {
        kUnknown,
        kHttp10,
        kHttp11,
    };

    HttpRequest() : method_(kInvalid), version_(kUnknown) {}

    void setVersion(Version v) { version_ = v; }
    Version version() const { return version_; }

    bool setMethod(const char* start, const char* end);
    // Convenience accessors for routing
    std::string method() const { return methodString(); }
    const char* methodString() const;
    const std::string& path() const { return path_; }

    Method methodEnum() const { return method_; }
    void setPath(const char* start, const char* end) { path_.assign(start, end); }

    void setQuery(const char* start, const char* end) { query_.assign(start, end); }
    const std::string& query() const { return query_; }

    void addHeader(const char* start, const char* colon, const char* end);
    std::string getHeader(const std::string& field) const;

    void setBody(const std::string& body);
    const std::string& body() const { return body_; }
    const nlohmann::json& getJson() const { return json_; }

    const std::map<std::string, std::string>& headers() const { return headers_; }
    void setHeader(const std::string& field, const std::string& value) { headers_[field] = value; }

    void setSession(const std::shared_ptr<Session>& session) { session_ = session; }
    std::shared_ptr<Session> getSession() const { return session_; }

    void swap(HttpRequest& that);

private:
    Method method_;
    Version version_;
    std::string path_;
    std::string query_;
    std::map<std::string, std::string> headers_;
    std::string body_;
    nlohmann::json json_;
    std::shared_ptr<Session> session_;
};
