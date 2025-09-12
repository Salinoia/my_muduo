#include <iostream>

#include "router/Router.h"
#include "session/Session.h"

int main() {
    Router router;
    auto store = std::make_shared<MemorySessionStore>();
    router.addInterceptor(std::make_shared<SessionInterceptor>(store));

    router.addRoute("/", [](HttpRequest& req, HttpResponse& res) {
        auto session = req.getSession();
        std::string count = session->get("count");
        if (count.empty())
            count = "0";
        int c = std::stoi(count);
        ++c;
        session->set("count", std::to_string(c));
        res.setStatusCode(HttpResponse::k200Ok);
        res.setBody("visit count: " + std::to_string(c));
    });

    HttpRequest req;
    const char* path = "/";
    req.setPath(path, path + 1);
    HttpResponse res(false);
    router.handle(req, res);
    std::cout << "First response body: " << req.getSession()->get("count") << std::endl;

    HttpRequest req2;
    req2.setPath(path, path + 1);
    req2.setHeader("Cookie", "SESSIONID=" + req.getSession()->id());
    HttpResponse res2(false);
    router.handle(req2, res2);
    std::cout << "Second response body: " << req2.getSession()->get("count") << std::endl;

    return 0;
}
