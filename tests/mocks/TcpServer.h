#pragma once
#include <functional>
#include <memory>
#include <string>
#include "Buffer.h"
#include "TimeStamp.h"

class EventLoop {};
class InetAddress {
public:
    InetAddress(const std::string& ip, uint16_t port) {}
};

class TcpConnection {
public:
    std::string sent;
    bool shutdownCalled{false};
    bool connected() const { return true; }
    void send(const std::string& data) { sent += data; }
    void shutdown() { shutdownCalled = true; }
};

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;
using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;
using MessageCallback = std::function<void(const TcpConnectionPtr&, Buffer*, TimeStamp)>;

class TcpServer {
public:
    enum Option { kNoReusePort };
    TcpServer(EventLoop*, const InetAddress&, const std::string&, Option option = kNoReusePort) {}
    void setConnectionCallback(const ConnectionCallback& cb) { connectionCallback_ = cb; }
    void setMessageCallback(const MessageCallback& cb) { messageCallback_ = cb; }
    void setThreadNum(int) {}
    void start() {}
private:
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
};
