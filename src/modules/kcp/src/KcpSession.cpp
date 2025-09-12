#include "kcp/KcpSession.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

KcpSession::KcpSession(int sockfd, const InetAddress& peer) : sockfd_(sockfd), peer_(peer), kcp_(nullptr) {}

KcpSession::~KcpSession() = default;

void KcpSession::send(const char* data, size_t len) {
    ::sendto(sockfd_, data, len, 0, reinterpret_cast<const sockaddr*>(peer_.getSockAddr()), sizeof(sockaddr_in));
}
