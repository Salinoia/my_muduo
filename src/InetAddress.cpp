#include "InetAddress.h"

#include <string.h>
InetAddress::InetAddress(std::string ip, uint16_t port) {
    ::memset(&addr_, 0, sizeof(addr_));
    addr_.sin_family = AF_INET;  // IPV4
    addr_.sin_port = ::htons(port);  // 本地字节序转化为网络字节序
    addr_.sin_addr.s_addr = ::inet_addr(ip.c_str());
}

std::string InetAddress::toIp() const {
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    return buf;
}

std::string InetAddress::toIpPort() const {
    // ip:port
    char buf[64] = {0};
    ::inet_ntop(AF_INET, &addr_.sin_addr, buf, sizeof buf);
    size_t end = ::strlen(buf);
    uint16_t port = ::ntohs(addr_.sin_port);
    sprintf(buf + end, ":%u", port);
    return buf;
}

uint16_t InetAddress::toPort() const {
    return ::ntohs(addr_.sin_port);
}
