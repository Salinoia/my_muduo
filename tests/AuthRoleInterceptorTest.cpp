#include <cassert>
#include <iostream>
#include <string>
#include <cstring>
#include <utility>
#include <vector>

#include "router/AuthInterceptor.h"
#include "router/RoleInterceptor.h"
#include "router/Router.h"

struct RouterConfig {
    bool enableAuth{true};
    bool enableRole{true};
};

static void InitRouter(Router& router, const RouterConfig& cfg) {
    if (cfg.enableAuth) {
        router.addInterceptor(std::make_shared<AuthInterceptor>("token123", "session123"));
    }
    if (cfg.enableRole) {
        router.addInterceptor(std::make_shared<RoleInterceptor>(std::unordered_set<std::string>{"admin"}));
    }
    router.addRoute("/secure", [](HttpRequest& req, HttpResponse& res) {
        res.setStatusCode(HttpResponse::k200Ok);
    });
}

static HttpRequest BuildRequest(const std::vector<std::pair<std::string, std::string>>& headers) {
    HttpRequest req;
    const char* path = "/secure";
    req.setPath(path, path + strlen(path));
    for (const auto& h : headers) {
        std::string line = h.first + ": " + h.second;
        req.addHeader(line.c_str(), line.c_str() + h.first.size(), line.c_str() + line.size());
    }
    return req;
}

static void TestAuthSuccess() {
    Router router;
    RouterConfig cfg;
    cfg.enableRole = false;
    InitRouter(router, cfg);
    auto req = BuildRequest({{"Authorization", "token123"}});
    HttpResponse res(false);
    router.handle(req, res);
    assert(res.statusCode() == HttpResponse::k200Ok);
}

static void TestAuthFailure() {
    Router router;
    RouterConfig cfg;
    cfg.enableRole = false;
    InitRouter(router, cfg);
    auto req = BuildRequest({});
    HttpResponse res(false);
    router.handle(req, res);
    assert(res.statusCode() == HttpResponse::k401Unauthorized);
}

static void TestRoleFailure() {
    Router router;
    RouterConfig cfg;
    InitRouter(router, cfg);
    auto req = BuildRequest({{"Authorization", "token123"}, {"Role", "user"}});
    HttpResponse res(false);
    router.handle(req, res);
    assert(res.statusCode() == HttpResponse::k403Forbidden);
}

static void TestRoleSuccess() {
    Router router;
    RouterConfig cfg;
    InitRouter(router, cfg);
    auto req = BuildRequest({{"Authorization", "token123"}, {"Role", "admin"}});
    HttpResponse res(false);
    router.handle(req, res);
    assert(res.statusCode() == HttpResponse::k200Ok);
}

int main() {
    TestAuthSuccess();
    TestAuthFailure();
    TestRoleFailure();
    TestRoleSuccess();
    std::cout << "All auth/role interceptor tests passed!" << std::endl;
    return 0;
}

