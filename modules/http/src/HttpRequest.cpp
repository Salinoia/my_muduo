#include "http/HttpRequest.h"

#include <algorithm>
#include <cctype>

void HttpRequest::swap(HttpRequest& that) {
    std::swap(method_, that.method_);
    std::swap(version_, that.version_);
    path_.swap(that.path_);
    query_.swap(that.query_);
    headers_.swap(that.headers_);
    body_.swap(that.body_);
    json_.swap(that.json_);
}

bool HttpRequest::setMethod(const char* start, const char* end) {
    std::string m(start, end);
    if (m == "GET")
        method_ = kGet;
    else if (m == "POST")
        method_ = kPost;
    else if (m == "HEAD")
        method_ = kHead;
    else if (m == "PUT")
        method_ = kPut;
    else if (m == "DELETE")
        method_ = kDelete;
    else
        method_ = kInvalid;
    return method_ != kInvalid;
}

const char* HttpRequest::methodString() const {
    switch (method_) {
    case kGet:
        return "GET";
    case kPost:
        return "POST";
    case kHead:
        return "HEAD";
    case kPut:
        return "PUT";
    case kDelete:
        return "DELETE";
    default:
        return "UNKNOWN";
    }
}

void HttpRequest::addHeader(const char* start, const char* colon, const char* end) {
    std::string field(start, colon);
    ++colon;
    while (colon < end && isspace(*colon))
        ++colon;
    std::string value(colon, end);
    while (!value.empty() && isspace(value.back()))
        value.pop_back();
    headers_[field] = value;
}

std::string HttpRequest::getHeader(const std::string& field) const {
    auto it = headers_.find(field);
    if (it != headers_.end())
        return it->second;
    return "";
}

void HttpRequest::setBody(const std::string& body) {
    body_ = body;
    json_ = nlohmann::json();
    std::string contentType = getHeader("Content-Type");
    if (contentType.find("application/json") != std::string::npos) {
        json_ = nlohmann::json::parse(body_, nullptr, false);
    }
}
