#pragma once

#include <functional>
#include <string>
#include <unordered_map>

#include "TcpServer.h"
#include "TcpConnection.h"
#include "Buffer.h"
#include "InetAddress.h"
#include "EventLoop.h"
#include "TimeStamp.h"

#include "WebSocketFrame.h"
#include "WebSocketContent.h"

// Simple WebSocket server that performs handshake and dispatches text frames.
class WebSocketServer {
public:
    WebSocketServer(EventLoop* loop, const InetAddress& addr)
        : server_(loop, addr, "WebSocketServer") {
        server_.setConnectionCallback([this](const TcpConnectionPtr& conn) { onConnection(conn); });
        server_.setMessageCallback([this](const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts) {
            onMessage(conn, buf, ts);
        });
    }

    void start() { server_.start(); }

    void setMessageCallback(std::function<void(const TcpConnectionPtr&, const std::string&)> cb) {
        messageCallback_ = std::move(cb);
    }

private:
    TcpServer server_;
    std::unordered_map<std::string, bool> handshaked_;
    std::function<void(const TcpConnectionPtr&, const std::string&)> messageCallback_;

    void onConnection(const TcpConnectionPtr& conn) {
        if (!conn->connected()) {
            handshaked_.erase(conn->name());
        }
    }

    void onMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp) {
        if (handshaked_.find(conn->name()) == handshaked_.end()) {
            std::string req = buf->retrieveAllAsString();
            std::string keyHeader = "Sec-WebSocket-Key: ";
            auto pos = req.find(keyHeader);
            if (pos == std::string::npos) {
                conn->shutdown();
                return;
            }
            pos += keyHeader.size();
            auto end = req.find("\r\n", pos);
            std::string key = req.substr(pos, end - pos);
            std::string accept = WebSocketAcceptKey(key);
            std::string resp = "HTTP/1.1 101 Switching Protocols\r\n";
            resp += "Upgrade: websocket\r\n";
            resp += "Connection: Upgrade\r\n";
            resp += "Sec-WebSocket-Accept: " + accept + "\r\n\r\n";
            conn->send(resp);
            handshaked_[conn->name()] = true;
        } else {
            std::string data = buf->retrieveAllAsString();
            WebSocketFrame frame;
            if (WebSocketFrame::Decode(data, frame)) {
                if (messageCallback_) {
                    messageCallback_(conn, frame.payload);
                }
            }
        }
    }
};
