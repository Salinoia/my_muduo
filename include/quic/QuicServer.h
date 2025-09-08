#pragma once

#include <memory>
#include <unordered_map>

#include "Channel.h"
#include "EventLoop.h"
#include "InetAddress.h"
#include "NonCopyable.h"
#include "TimeStamp.h"

class QuicSession;

// Simplified QUIC server built on UDP; acts as placeholder for real QUIC
class QuicServer : NonCopyable {
public:
    using SessionPtr = std::shared_ptr<QuicSession>;

    QuicServer(EventLoop* loop, const InetAddress& listenAddr);
    ~QuicServer();

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
