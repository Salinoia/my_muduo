#pragma once

#include <random>
#include <string>

#include "router/Router.h"
#include "Logger.h"

// Interceptor that attaches a unique trace identifier to each request.
class TraceInterceptor : public Interceptor {
public:
    void Handle(HttpRequest& req, HttpResponse& res, Next next) override {
        std::string trace = req.getHeader("TraceId");
        if (trace.empty()) {
            trace = generateId();
        }
        Logger::instance().setTraceId(trace);
        next();
        Logger::instance().clearTraceId();
    }

private:
    static std::string generateId() {
        static std::mt19937_64 rng{std::random_device{}()};
        static const char chars[] = "0123456789abcdef";
        std::string id(16, '0');
        for (char& c : id) {
            c = chars[rng() % (sizeof(chars) - 1)];
        }
        return id;
    }
};
