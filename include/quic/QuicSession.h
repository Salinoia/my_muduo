#pragma once

#include <memory>

#include "InetAddress.h"
#include "NonCopyable.h"

// Simplified QUIC session placeholder
class QuicSession : NonCopyable, public std::enable_shared_from_this<QuicSession> {
public:
    QuicSession(int sockfd, const InetAddress& peer);
    ~QuicSession();

    void send(const char* data, size_t len);

    const InetAddress& peerAddr() const { return peer_; }

private:
    int sockfd_;
    InetAddress peer_;
};
