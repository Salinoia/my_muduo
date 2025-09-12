add_executable(echo EchoServer.cpp) target_link_libraries(echo muduo_core ${LIBS})

    add_executable(client Client.cpp) target_link_libraries(client muduo_core ${LIBS})

        add_executable(rabbitmq_example RabbitMQExample.cpp ${CMAKE_SOURCE_DIR} / storage / mq / RabbitMQClient.cpp)
            target_include_directories(rabbitmq_example PUBLIC ${CMAKE_SOURCE_DIR} / storage / mq) target_link_libraries(rabbitmq_example muduo_core ${LIBS})
