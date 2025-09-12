#pragma once

#include <condition_variable>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <nlohmann/json.hpp>

// A lightweight RabbitMQ client simulation providing basic
// publishing and consuming capabilities. The implementation does not
// depend on any external RabbitMQ libraries which keeps the example
// self contained. The goal is to demonstrate how connection management
// and producer/consumer helpers could be structured.
struct RabbitMQConfig {
    std::string host{"localhost"};
    int port{5672};
};

inline void from_json(const nlohmann::json& j, RabbitMQConfig& c) {
    j.at("host").get_to(c.host);
    j.at("port").get_to(c.port);
}

class RabbitMQClient : public std::enable_shared_from_this<RabbitMQClient> {
public:
    using ErrorCallback = std::function<void(const std::string&)>;
    using MessageCallback = std::function<void(const std::string&)>;

    RabbitMQClient(const std::string& host = "localhost", int port = 5672);
    ~RabbitMQClient() = default;

    static std::shared_ptr<RabbitMQClient> Instance();

    // Establish a connection to the server. In this simplified
    // implementation the connection always succeeds.
    bool connect();
    void disconnect();
    bool isConnected() const;

    void enableReconnect(bool enable);
    void setErrorCallback(ErrorCallback cb);

    // Publish a message to the given queue.
    bool publish(const std::string& queue, const std::string& message);

    // Consume messages from the given queue asynchronously. Each
    // received message is forwarded to the provided callback.
    void consume(const std::string& queue, MessageCallback cb);

private:
    bool ensureConnection();

    std::string host_;
    int port_;
    bool connected_;
    bool reconnect_;
    ErrorCallback errorCb_;

    std::map<std::string, std::queue<std::string>> queues_;
    std::mutex mutex_;
    std::condition_variable cond_;
};

// Simple helper for publishing messages.
class Producer {
public:
    explicit Producer(const std::shared_ptr<RabbitMQClient>& client);
    bool publish(const std::string& queue, const std::string& message);

private:
    std::shared_ptr<RabbitMQClient> client_;
};

// Helper for subscribing to a queue.
class Consumer {
public:
    using MessageCallback = RabbitMQClient::MessageCallback;
    explicit Consumer(const std::shared_ptr<RabbitMQClient>& client);
    void subscribe(const std::string& queue, MessageCallback cb);

private:
    std::shared_ptr<RabbitMQClient> client_;
};
