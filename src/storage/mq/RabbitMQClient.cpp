#include "RabbitMQClient.h"
#include "ConfigManager.h"

#include <chrono>
#include <iostream>

RabbitMQClient::RabbitMQClient(const std::string& host, int port)
    : host_(host), port_(port), connected_(false), reconnect_(true) {}

RabbitMQClient::~RabbitMQClient() { disconnect(); }

std::shared_ptr<RabbitMQClient> RabbitMQClient::Instance() {
    static std::shared_ptr<RabbitMQClient> instance;
    static std::once_flag flag;
    std::call_once(flag, []() {
        RabbitMQConfig cfg = ConfigManager::LoadTyped<RabbitMQConfig>("config/mq.json");
        instance = std::make_shared<RabbitMQClient>(cfg.host, cfg.port);
        instance->connect();
    });
    return instance;
}

bool RabbitMQClient::connect() {
    // Simulate a successful connection.
    connected_ = true;
    return connected_;
}

void RabbitMQClient::disconnect() {
    if (!connected_)
        return;
    connected_ = false;
    cond_.notify_all();
    for (auto& t : consumerThreads_) {
        if (t.joinable())
            t.join();
    }
    consumerThreads_.clear();
}

bool RabbitMQClient::isConnected() const {
    return connected_;
}

void RabbitMQClient::enableReconnect(bool enable) {
    reconnect_ = enable;
}

void RabbitMQClient::setErrorCallback(ErrorCallback cb) {
    errorCb_ = std::move(cb);
}

bool RabbitMQClient::ensureConnection() {
    if (connected_)
        return true;
    if (reconnect_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (connect())
            return true;
    }
    if (errorCb_)
        errorCb_("failed to connect");
    return false;
}

bool RabbitMQClient::publish(const std::string& queue, const std::string& message) {
    if (!ensureConnection())
        return false;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        queues_[queue].push(message);
    }
    cond_.notify_all();
    return true;
}

void RabbitMQClient::consume(const std::string& queue, MessageCallback cb) {
    consumerThreads_.emplace_back([this, queue, cb]() {
        while (true) {
            std::unique_lock<std::mutex> lock(mutex_);
            cond_.wait(lock, [this, &queue]() { return !queues_[queue].empty() || !connected_; });
            if (!connected_)
                break;
            auto msg = std::move(queues_[queue].front());
            queues_[queue].pop();
            lock.unlock();
            cb(msg);
        }
    });
}

Producer::Producer(const std::shared_ptr<RabbitMQClient>& client) : client_(client) {}

bool Producer::publish(const std::string& queue, const std::string& message) {
    return client_ && client_->publish(queue, message);
}

Consumer::Consumer(const std::shared_ptr<RabbitMQClient>& client) : client_(client) {}

void Consumer::subscribe(const std::string& queue, MessageCallback cb) {
    if (client_)
        client_->consume(queue, std::move(cb));
}
