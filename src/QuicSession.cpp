#include "quic/QuicSession.h"

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

QuicSession::QuicSession(int sockfd, const InetAddress& peer) : sockfd_(sockfd), peer_(peer) {}

QuicSession::~QuicSession() = default;

void QuicSession::send(const char* data, size_t len) {
    ::sendto(sockfd_, data, len, 0, reinterpret_cast<const sockaddr*>(peer_.getSockAddr()), sizeof(sockaddr_in));
}
