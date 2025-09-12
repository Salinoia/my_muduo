#include <cassert>
#include <iostream>
#include "Buffer.h"
#include "TimeStamp.h"
#include "TcpServer.h"
#include "http/HttpContext.h"
#include "http/HttpResponse.h"

// expose private methods of HttpServer only
#define private public
#include "http/HttpServer.h"
#undef private
#include "http/src/HttpRequest.cpp"
#include "http/src/HttpResponse.cpp"
#include "http/src/HttpContext.cpp"
#include "http/src/HttpServer.cpp"

int main() {
    EventLoop loop;
    InetAddress addr("127.0.0.1", 8080);

    // Test default 404 when no callback is set
    HttpServer server(&loop, addr, "test");
    TcpConnectionPtr conn1 = std::make_shared<TcpConnection>();
    HttpRequest req1;
    const char* path1 = "/";
    req1.setPath(path1, path1 + 1);
    server.onRequest(conn1, req1);
    assert(conn1->sent.find("404 Not Found") != std::string::npos);
    assert(conn1->shutdownCalled);

    // Test custom callback returning 200
    HttpServer server2(&loop, addr, "test2");
    server2.setHttpCallback([](HttpRequest& req, HttpResponse& res) {
        res.setStatusCode(HttpResponse::k200Ok);
        res.setStatusMessage("OK");
    });
    TcpConnectionPtr conn2 = std::make_shared<TcpConnection>();
    HttpRequest req2;
    const char* path2 = "/hello";
    req2.setPath(path2, path2 + 6);
    server2.onRequest(conn2, req2);
    assert(conn2->sent.find("200 OK") != std::string::npos);
    assert(!conn2->shutdownCalled);

    std::cout << "HttpServerTest passed" << std::endl;
    return 0;
}
