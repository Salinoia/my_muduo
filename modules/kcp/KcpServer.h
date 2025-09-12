#pragma once

#include <memory>
#include <unordered_map>

#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "NonCopyable.h"
#include "TimeStamp.h"

class KcpSession;

// Basic KCP server built on UDP and EventLoop
class KcpServer : NonCopyable {
public:
    using SessionPtr = std::shared_ptr<KcpSession>;

    KcpServer(EventLoop* loop, const InetAddress& listenAddr);
    ~KcpServer();

    // Start receiving datagrams
    void start();

private:
    void handleRead(TimeStamp receiveTime);
    SessionPtr getSession(const InetAddress& peer);

    EventLoop* loop_;
    int sockfd_;
    InetAddress listenAddr_;
    Channel channel_;
    std::unordered_map<std::string, SessionPtr> sessions_;
};
