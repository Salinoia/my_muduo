#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "TcpServer.h"
#include "http/HttpContext.h"
#include "http/HttpResponse.h"
#include "router/Router.h"

class HttpServer {
public:
    using HttpCallback = std::function<void(HttpRequest&, HttpResponse&)>;

    HttpServer(EventLoop* loop, const InetAddress& listenAddr, const std::string& name, TcpServer::Option option = TcpServer::kNoReusePort);

    Router& router() { return router_; }

    void setThreadNum(int numThreads) { server_.setThreadNum(numThreads); }
    void start();

    void setHttpCallback(const HttpCallback& cb) { httpCallback_ = cb; }

    // Expose request handling for tests
    void handleRequestForTest(const TcpConnectionPtr& conn, HttpRequest& req) { onRequest(conn, req); }

private:
    void onConnection(const TcpConnectionPtr& conn);
    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, TimeStamp receiveTime);
    void onRequest(const TcpConnectionPtr& conn, HttpRequest& req);

    TcpServer server_;
    Router router_;
    std::unordered_map<TcpConnection*, HttpContext> contexts_;
    HttpCallback httpCallback_;
};
