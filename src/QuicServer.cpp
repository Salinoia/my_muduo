#include "quic/QuicServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Logger.h"
#include "quic/QuicSession.h"

QuicServer::QuicServer(EventLoop* loop, const InetAddress& listenAddr) : loop_(loop), sockfd_(::socket(AF_INET, SOCK_DGRAM, 0)), listenAddr_(listenAddr), channel_(loop, sockfd_) {
    ::bind(sockfd_, reinterpret_cast<const sockaddr*>(listenAddr_.getSockAddr()), sizeof(sockaddr_in));
    channel_.setReadCallback(std::bind(&QuicServer::handleRead, this, std::placeholders::_1));
}

QuicServer::~QuicServer() {
    ::close(sockfd_);
}

void QuicServer::start() {
    channel_.enableReading();
}

void QuicServer::handleRead(TimeStamp /*receiveTime*/) {
    char buf[4096];
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    ssize_t n = ::recvfrom(sockfd_, buf, sizeof(buf), 0, reinterpret_cast<sockaddr*>(&peeraddr), &len);
    if (n <= 0) {
        return;
    }
    InetAddress peer(peeraddr);
    SessionPtr session = getSession(peer);
    // Placeholder for QUIC handling; echo back for now.
    session->send(buf, static_cast<size_t>(n));
}

QuicServer::SessionPtr QuicServer::getSession(const InetAddress& peer) {
    std::string key = peer.toIpPort();
    auto it = sessions_.find(key);
    if (it != sessions_.end()) {
        return it->second;
    }
    SessionPtr session = std::make_shared<QuicSession>(sockfd_, peer);
    sessions_[key] = session;
    return session;
}
