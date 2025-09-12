#pragma once

#include <cstdint>
#include <string>
#include <vector>

// Minimal WebSocket frame representation supporting text frames only.
struct WebSocketFrame {
    bool fin{true};
    uint8_t opcode{1}; // 1 = text frame
    std::string payload;

    static std::string Encode(const WebSocketFrame& frame);
    static bool Decode(const std::string& data, WebSocketFrame& frame);
};

inline std::string WebSocketFrame::Encode(const WebSocketFrame& frame) {
    std::string out;
    uint8_t first = (frame.fin ? 0x80 : 0x00) | (frame.opcode & 0x0F);
    out.push_back(static_cast<char>(first));
    size_t len = frame.payload.size();
    if (len < 126) {
        out.push_back(static_cast<char>(len));
    } else {
        // For simplicity we only handle payload < 126 bytes
        return std::string();
    }
    out.append(frame.payload);
    return out;
}

inline bool WebSocketFrame::Decode(const std::string& data, WebSocketFrame& frame) {
    if (data.size() < 2) return false;
    uint8_t b1 = static_cast<uint8_t>(data[0]);
    uint8_t b2 = static_cast<uint8_t>(data[1]);
    frame.fin = (b1 & 0x80) != 0;
    frame.opcode = b1 & 0x0F;
    bool masked = (b2 & 0x80) != 0;
    size_t len = b2 & 0x7F;
    size_t pos = 2;
    uint8_t mask[4] = {0};
    if (masked) {
        if (data.size() < pos + 4) return false;
        for (int i = 0; i < 4; ++i) mask[i] = static_cast<uint8_t>(data[pos++]);
    }
    if (data.size() < pos + len) return false;
    frame.payload.resize(len);
    for (size_t i = 0; i < len; ++i) {
        uint8_t c = static_cast<uint8_t>(data[pos + i]);
        frame.payload[i] = masked ? static_cast<char>(c ^ mask[i % 4]) : static_cast<char>(c);
    }
    return true;
}
