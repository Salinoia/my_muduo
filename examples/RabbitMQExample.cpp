#include <chrono>
#include <iostream>
#include <thread>

#include "RabbitMQClient.h"
#include "ConfigManager.h"

int main() {
    ConfigManager::Instance().Load("config/mq.json");
    std::string host = ConfigManager::Instance().GetString("host", "localhost");
    int port = ConfigManager::Instance().GetInt("port", 5672);

    auto client = std::make_shared<RabbitMQClient>(host, port);
    client->setErrorCallback([](const std::string& err) { std::cerr << "RabbitMQ error: " << err << std::endl; });
    client->enableReconnect(true);
    client->connect();

    Producer producer(client);
    Consumer consumer(client);

    consumer.subscribe("demo", [](const std::string& msg) { std::cout << "Received: " << msg << std::endl; });

    producer.publish("demo", "hello");
    producer.publish("demo", "world");

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return 0;
}
