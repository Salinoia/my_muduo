#include <cassert>
#include <iostream>
#include <string>

#include "Buffer.h"
#include "router/Router.h"

void TestMethodRouting() {
    Router router;
    int getHit = 0;
    int postHit = 0;
    router.addRoute("GET", "/get", [&](HttpRequest& req, HttpResponse& res) {
        getHit = 1;
        res.setStatusCode(HttpResponse::k200Ok);
    });
    router.addRoute("POST", "/post", [&](HttpRequest& req, HttpResponse& res) {
        postHit = 1;
        res.setStatusCode(HttpResponse::k200Ok);
    });

    HttpRequest req1;
    const char* m1 = "GET";
    req1.setMethod(m1, m1 + 3);
    const char* p1 = "/get";
    req1.setPath(p1, p1 + 4);
    HttpResponse res1(false);
    router.handle(req1, res1);
    assert(getHit == 1 && postHit == 0);

    HttpRequest req2;
    const char* m2 = "POST";
    req2.setMethod(m2, m2 + 4);
    const char* p2 = "/post";
    req2.setPath(p2, p2 + 5);
    HttpResponse res2(false);
    router.handle(req2, res2);
    assert(getHit == 1 && postHit == 1);
}

void TestDynamicParam() {
    Router router;
    int hit = 0;
    router.addRoute("GET", "/users/:id", [&](HttpRequest& req, HttpResponse& res) {
        hit = 1;
        res.setStatusCode(HttpResponse::k200Ok);
    });
    HttpRequest req;
    const char* m = "GET";
    req.setMethod(m, m + 3);
    const char* p = "/users/123";
    req.setPath(p, p + 10);
    HttpResponse res(false);
    router.handle(req, res);
    assert(hit == 1);
}

void TestNotFound() {
    Router router;
    HttpRequest req;
    const char* m = "GET";
    req.setMethod(m, m + 3);
    const char* p = "/unknown";
    req.setPath(p, p + 8);
    HttpResponse res(false);
    router.handle(req, res);
    Buffer buf;
    res.appendToBuffer(&buf);
    std::string output = buf.retrieveAllAsString();
    assert(output.find("404") != std::string::npos);
}

int main() {
    TestMethodRouting();
    TestDynamicParam();
    TestNotFound();
    std::cout << "All router route tests passed!" << std::endl;
    return 0;
}
