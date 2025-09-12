#pragma once

#include <memory>

#include "InetAddress.h"
#include "NonCopyable.h"

// Forward declaration for KCP control block
struct IKCPCB;

// Basic session for KCP over UDP
class KcpSession : NonCopyable, public std::enable_shared_from_this<KcpSession> {
public:
    KcpSession(int sockfd, const InetAddress& peer);
    ~KcpSession();

    // send data to peer using underlying UDP socket
    void send(const char* data, size_t len);

    const InetAddress& peerAddr() const { return peer_; }

private:
    int sockfd_;
    InetAddress peer_;
    IKCPCB* kcp_;  // placeholder for future KCP control block
};
