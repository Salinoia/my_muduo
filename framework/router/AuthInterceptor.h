#pragma once

#include <string>
#include <utility>

#include "router/Router.h"

// Interceptor that validates Authorization token or Session header.
class AuthInterceptor : public Interceptor {
public:
    AuthInterceptor(std::string validToken, std::string validSession)
        : token_(std::move(validToken)), session_(std::move(validSession)) {}

    void Handle(HttpRequest& req, HttpResponse& res, Next next) override {
        std::string token = req.getHeader("Authorization");
        std::string sess = req.getHeader("Session");
        if ((!token_.empty() && token == token_) ||
            (!session_.empty() && sess == session_)) {
            next();
        } else {
            res.setStatusCode(HttpResponse::k401Unauthorized);
            res.setStatusMessage("Unauthorized");
        }
    }

private:
    std::string token_;
    std::string session_;
};

