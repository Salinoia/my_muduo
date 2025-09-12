#pragma once

#include <functional>
#include <memory>
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

#include "http/HttpRequest.h"
#include "http/HttpResponse.h"

// Base class for HTTP request interceptors.
class Interceptor {
public:
    using Next = std::function<void()>;
    virtual ~Interceptor() = default;
    virtual void Handle(HttpRequest& req, HttpResponse& res, Next next) = 0;
};

// Maintains a list of interceptors and executes them sequentially.
class InterceptorChain {
public:
    void addInterceptor(const std::shared_ptr<Interceptor>& interceptor) {
        interceptors_.push_back(interceptor);
    }

    void handle(HttpRequest& req, HttpResponse& res,
                const std::function<void(HttpRequest&, HttpResponse&)>& finalHandler) {
        std::function<void(size_t)> dispatch = [&](size_t index) {
            if (index < interceptors_.size()) {
                interceptors_[index]->Handle(req, res, [&, index]() { dispatch(index + 1); });
            } else {
                finalHandler(req, res);
            }
        };
        dispatch(0);
    }

private:
    std::vector<std::shared_ptr<Interceptor>> interceptors_;
};

// Simple path based router with support for interceptors.
class Router {
public:
    using Handler = std::function<void(HttpRequest&, HttpResponse&)>;

    void addRoute(const std::string& method, const std::string& path, const Handler& handler) {
        Route r;
        r.method = method;
        r.pattern = std::regex(pathToRegex(path));
        r.handler = handler;
        routes_.push_back(std::move(r));
    }

    void addInterceptor(const std::shared_ptr<Interceptor>& interceptor) {
        chain_.addInterceptor(interceptor);
    }

    void handle(HttpRequest& req, HttpResponse& res) {
        Handler handler = [](HttpRequest& /*unused*/, HttpResponse& response) {
            response.setStatusCode(HttpResponse::k404NotFound);
            response.setStatusMessage("Not Found");
        };
        for (const auto& route : routes_) {
            if (route.method == req.method() && std::regex_match(req.path(), route.pattern)) {
                handler = route.handler;
                break;
            }
        }
        chain_.handle(req, res, handler);
    }

private:
    struct Route {
        std::string method;
        std::regex pattern;
        Handler handler;
    };

    static std::string pathToRegex(const std::string& path) {
        std::string regexStr = "^";
        const std::string special = ".^$|()[]{}*+?\\";
        for (size_t i = 0; i < path.size(); ++i) {
            char c = path[i];
            if (c == ':') {
                regexStr += "([^/]+)";
                while (i + 1 < path.size() && path[i + 1] != '/') {
                    ++i;
                }
            } else {
                if (special.find(c) != std::string::npos)
                    regexStr += '\\';
                regexStr += c;
            }
        }
        regexStr += '$';
        return regexStr;
    }

    std::vector<Route> routes_;
    InterceptorChain chain_;
};

