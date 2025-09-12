add_executable(echo EchoServer.cpp)
target_link_libraries(echo muduo_core ${LIBS})

add_executable(client Client.cpp)
target_link_libraries(client muduo_core ${LIBS})

add_executable(rabbitmq_example RabbitMQExample.cpp ../storage/mq/RabbitMQClient.cpp ../framework/utils/ConfigManager.cpp)
target_include_directories(rabbitmq_example PUBLIC ${CMAKE_SOURCE_DIR}/storage/mq ${CMAKE_SOURCE_DIR}/framework/utils)
target_link_libraries(rabbitmq_example muduo_core ${LIBS})


add_executable(web_server WebServer.cpp ../services/UserService.cpp ../storage/UserRepository.cpp ../framework/ioc/Container.cpp)
target_include_directories(web_server PRIVATE ${CMAKE_SOURCE_DIR}/framework/ioc ${CMAKE_SOURCE_DIR}/services ${CMAKE_SOURCE_DIR}/storage)
target_link_libraries(web_server muduo_core ${LIBS})

