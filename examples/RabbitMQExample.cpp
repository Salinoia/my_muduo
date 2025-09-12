#include <chrono>
#include <iostream>
#include <thread>

#include "RabbitMQClient.h"

int main() {
    auto client = RabbitMQClient::Instance();
    client->setErrorCallback([](const std::string& err) { std::cerr << "RabbitMQ error: " << err << std::endl; });
    client->enableReconnect(true);

    Producer producer(client);
    Consumer consumer(client);

    consumer.subscribe("demo", [](const std::string& msg) { std::cout << "Received: " << msg << std::endl; });

    producer.publish("demo", "hello");
    producer.publish("demo", "world");

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return 0;
}
