#pragma once

#include <openssl/sha.h>
#include <string>

inline std::string Base64Encode(const unsigned char* data, size_t len) {
    static const char tbl[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    size_t i = 0;
    for (; i + 2 < len; i += 3) {
        out.push_back(tbl[(data[i] >> 2) & 0x3F]);
        out.push_back(tbl[((data[i] & 0x3) << 4) | ((data[i + 1] >> 4) & 0xF)]);
        out.push_back(tbl[((data[i + 1] & 0xF) << 2) | ((data[i + 2] >> 6) & 0x3)]);
        out.push_back(tbl[data[i + 2] & 0x3F]);
    }
    if (i < len) {
        out.push_back(tbl[(data[i] >> 2) & 0x3F]);
        if (i + 1 < len) {
            out.push_back(tbl[((data[i] & 0x3) << 4) | ((data[i + 1] >> 4) & 0xF)]);
            out.push_back(tbl[(data[i + 1] & 0xF) << 2]);
            out.push_back('=');
        } else {
            out.push_back(tbl[(data[i] & 0x3) << 4]);
            out.push_back('=');
            out.push_back('=');
        }
    }
    return out;
}

inline std::string WebSocketAcceptKey(const std::string& key) {
    std::string data = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char*>(data.data()), data.size(), digest);
    return Base64Encode(digest, SHA_DIGEST_LENGTH);
}
