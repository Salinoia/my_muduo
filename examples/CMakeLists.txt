add_executable(echo EchoServer.cpp)
target_link_libraries(echo muduo_core ${LIBS})

add_executable(client Client.cpp)
target_link_libraries(client muduo_core ${LIBS})

add_executable(rabbitmq_example RabbitMQExample.cpp ../storage/mq/RabbitMQClient.cpp ../framework/utils/ConfigManager.cpp)
target_include_directories(rabbitmq_example PUBLIC ${CMAKE_SOURCE_DIR}/storage/mq ${CMAKE_SOURCE_DIR}/framework/utils)
target_link_libraries(rabbitmq_example muduo_core ${LIBS})

add_executable(redis_example RedisExample.cpp ../storage/cache/RedisClient.cpp ../storage/cache/RedisPool.cpp ../framework/utils/ConfigManager.cpp)
target_include_directories(redis_example PRIVATE ${CMAKE_SOURCE_DIR}/storage/cache ${CMAKE_SOURCE_DIR}/framework/utils)
target_link_libraries(redis_example hiredis muduo_core ${LIBS})

add_executable(web_server WebServer.cpp)
target_include_directories(web_server PRIVATE ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/modules ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/services)
target_link_libraries(web_server muduo_http muduo_core ${LIBS})
