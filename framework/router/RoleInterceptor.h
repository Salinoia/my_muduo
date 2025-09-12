#pragma once

#include <string>
#include <unordered_set>
#include <utility>

#include "router/Router.h"

// Interceptor that restricts access based on user roles.
class RoleInterceptor : public Interceptor {
public:
    explicit RoleInterceptor(std::unordered_set<std::string> allowed)
        : allowed_(std::move(allowed)) {}

    void Handle(HttpRequest& req, HttpResponse& res, Next next) override {
        std::string role = req.getHeader("Role");
        if (allowed_.find(role) != allowed_.end()) {
            next();
        } else {
            res.setStatusCode(HttpResponse::k403Forbidden);
            res.setStatusMessage("Forbidden");
        }
    }

private:
    std::unordered_set<std::string> allowed_;
};

