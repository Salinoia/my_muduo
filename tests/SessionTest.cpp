#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>

#include "router/Router.h"
#include "session/Session.h"

void TestSessionLifecycle() {
    Router router;
    auto store = std::make_shared<MemorySessionStore>();
    router.addInterceptor(std::make_shared<SessionInterceptor>(store, 1));
    router.addRoute("/test", [](HttpRequest& req, HttpResponse& res) {
        auto session = req.getSession();
        std::string count = session->get("count");
        if (count.empty())
            count = "0";
        int c = std::stoi(count);
        ++c;
        session->set("count", std::to_string(c));
        res.setStatusCode(HttpResponse::k200Ok);
    });

    HttpRequest req;
    const char* path = "/test";
    req.setPath(path, path + 5);
    HttpResponse res(false);
    router.handle(req, res);
    auto sid = req.getSession()->id();
    assert(req.getSession()->get("count") == "1");

    HttpRequest req2;
    req2.setPath(path, path + 5);
    req2.setHeader("Cookie", "SESSIONID=" + sid);
    HttpResponse res2(false);
    router.handle(req2, res2);
    assert(req2.getSession()->get("count") == "2");

    std::this_thread::sleep_for(std::chrono::seconds(2));

    HttpRequest req3;
    req3.setPath(path, path + 5);
    req3.setHeader("Cookie", "SESSIONID=" + sid);
    HttpResponse res3(false);
    router.handle(req3, res3);
    // Session should have expired and restarted from 1
    assert(req3.getSession()->get("count") == "1");
}

int main() {
    TestSessionLifecycle();
    std::cout << "Session tests passed!" << std::endl;
    return 0;
}
