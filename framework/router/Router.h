#pragma once

#include <functional>
#include <memory>
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

    void addRoute(const std::string& path, const Handler& handler) {
        routes_[path] = handler;
    }

    void addInterceptor(const std::shared_ptr<Interceptor>& interceptor) {
        chain_.addInterceptor(interceptor);
    }

    void handle(HttpRequest& req, HttpResponse& res) {
        auto it = routes_.find(req.path());
        Handler handler;
        if (it != routes_.end()) {
            handler = it->second;
        } else {
            handler = [](HttpRequest& /*unused*/, HttpResponse& response) {
                response.setStatusCode(HttpResponse::k404NotFound);
                response.setStatusMessage("Not Found");
            };
        }
        chain_.handle(req, res, handler);
    }

private:
    std::unordered_map<std::string, Handler> routes_;
    InterceptorChain chain_;
};

