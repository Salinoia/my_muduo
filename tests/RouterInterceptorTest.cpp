#include <cassert>
#include <vector>
#include <string>
#include <iostream>

#include "router/Router.h"

class RecordingInterceptor : public Interceptor {
public:
    RecordingInterceptor(const std::string& name, std::vector<std::string>& log)
        : name_(name), log_(log) {}

    void Handle(HttpRequest& req, HttpResponse& res, Next next) override {
        log_.push_back(name_);
        next();
    }
private:
    std::string name_;
    std::vector<std::string>& log_;
};

class StopInterceptor : public Interceptor {
public:
    StopInterceptor(std::vector<std::string>& log) : log_(log) {}
    void Handle(HttpRequest& req, HttpResponse& res, Next next) override {
        log_.push_back("stop");
        // Intentionally do not call next() to stop the chain
    }
private:
    std::vector<std::string>& log_;
};

void TestOrder() {
    Router router;
    std::vector<std::string> log;
    router.addInterceptor(std::make_shared<RecordingInterceptor>("A", log));
    router.addInterceptor(std::make_shared<RecordingInterceptor>("B", log));
    router.addRoute("/test", [&](HttpRequest& req, HttpResponse& res) {
        log.push_back("handler");
        res.setStatusCode(HttpResponse::k200Ok);
    });

    HttpRequest req;
    const char* path = "/test";
    req.setPath(path, path + 5);
    HttpResponse res(false);
    router.handle(req, res);

    assert((log == std::vector<std::string>{"A", "B", "handler"}));
}

void TestShortCircuit() {
    Router router;
    std::vector<std::string> log;
    router.addInterceptor(std::make_shared<RecordingInterceptor>("A", log));
    router.addInterceptor(std::make_shared<StopInterceptor>(log));
    router.addInterceptor(std::make_shared<RecordingInterceptor>("B", log));
    router.addRoute("/test", [&](HttpRequest& req, HttpResponse& res) {
        log.push_back("handler");
        res.setStatusCode(HttpResponse::k200Ok);
    });

    HttpRequest req;
    const char* path = "/test";
    req.setPath(path, path + 5);
    HttpResponse res(false);
    router.handle(req, res);

    assert((log == std::vector<std::string>{"A", "stop"}));
}

int main() {
    TestOrder();
    TestShortCircuit();
    std::cout << "All router tests passed!" << std::endl;
    return 0;
}

