#include <cassert>
#include <iostream>

#include "router/Router.h"

int main() {
    Router router;
    router.addRoute("GET", "/hi", [](HttpRequest& req, HttpResponse& res) {
        res.setStatusCode(HttpResponse::k200Ok);
        res.setStatusMessage("OK");
    });

    HttpRequest req1; const char* p1 = "/hi"; req1.setPath(p1, p1 + 3);
    const char* m = "GET"; req1.setMethod(m, m+3);
    req1.setVersion(HttpRequest::kHttp10);
    HttpResponse res1(false);
    router.handle(req1, res1);
    assert(res1.statusCode() == HttpResponse::k200Ok);

    HttpRequest req2; const char* p2 = "/none"; req2.setPath(p2, p2 + 5);
    req2.setMethod(m, m+3);
    req2.setVersion(HttpRequest::kHttp10);
    HttpResponse res2(false);
    router.handle(req2, res2);
    assert(res2.statusCode() == HttpResponse::k404NotFound);

    std::cout << "RouterTest passed" << std::endl;
    return 0;
}
