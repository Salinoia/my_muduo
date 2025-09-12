#include "router/Router.h"
#include "router/AuthInterceptor.h"
#include "router/RoleInterceptor.h"

#include <memory>
#include <unordered_set>

struct RouterConfig {
    bool enableAuth{true};
    bool enableRole{true};
};

int main() {
    Router router;
    RouterConfig config{}; // toggles can be set as needed
    if (config.enableAuth) {
        router.addInterceptor(std::make_shared<AuthInterceptor>("token123", "session123"));
    }
    if (config.enableRole) {
        router.addInterceptor(std::make_shared<RoleInterceptor>(std::unordered_set<std::string>{"admin"}));
    }
    router.addRoute("/secure", [](HttpRequest& req, HttpResponse& res) {
        res.setStatusCode(HttpResponse::k200Ok);
        res.setStatusMessage("OK");
    });
    return 0;
}

