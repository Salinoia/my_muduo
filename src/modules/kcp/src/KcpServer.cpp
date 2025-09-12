#include "kcp/KcpServer.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Logger.h"
#include "kcp/KcpSession.h"

KcpServer::KcpServer(EventLoop* loop, const InetAddress& listenAddr) : loop_(loop), sockfd_(::socket(AF_INET, SOCK_DGRAM, 0)), listenAddr_(listenAddr), channel_(loop, sockfd_) {
    ::bind(sockfd_, reinterpret_cast<const sockaddr*>(listenAddr_.getSockAddr()), sizeof(sockaddr_in));
    channel_.setReadCallback(std::bind(&KcpServer::handleRead, this, std::placeholders::_1));
}

KcpServer::~KcpServer() {
    ::close(sockfd_);
}

void KcpServer::start() {
    channel_.enableReading();
}

void KcpServer::handleRead(TimeStamp /*receiveTime*/) {
    char buf[4096];
    sockaddr_in peeraddr;
    socklen_t len = sizeof(peeraddr);
    ssize_t n = ::recvfrom(sockfd_, buf, sizeof(buf), 0, reinterpret_cast<sockaddr*>(&peeraddr), &len);
    if (n <= 0) {
        return;
    }
    InetAddress peer(peeraddr);
    SessionPtr session = getSession(peer);
    // In real KCP, input would parse data. Here we simply echo back.
    session->send(buf, static_cast<size_t>(n));
}

KcpServer::SessionPtr KcpServer::getSession(const InetAddress& peer) {
    std::string key = peer.toIpPort();
    auto it = sessions_.find(key);
    if (it != sessions_.end()) {
        return it->second;
    }
    SessionPtr session = std::make_shared<KcpSession>(sockfd_, peer);
    sessions_[key] = session;
    return session;
}
