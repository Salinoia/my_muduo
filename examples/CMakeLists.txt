add_executable(echo EchoServer.cpp)
target_link_libraries(echo muduo_core ${LIBS})

add_executable(client Client.cpp)
target_link_libraries(client muduo_core ${LIBS})

add_executable(rabbitmq_example RabbitMQExample.cpp ../src/storage/mq/RabbitMQClient.cpp ../src/framework/utils/ConfigManager.cpp)
target_include_directories(rabbitmq_example PUBLIC ${CMAKE_SOURCE_DIR}/src/storage/mq ${CMAKE_SOURCE_DIR}/src/framework/utils)
target_link_libraries(rabbitmq_example muduo_core yaml-cpp ${LIBS})

add_executable(web_server WebServer.cpp ../src/services/UserService.cpp ../src/storage/UserRepository.cpp ../src/framework/ioc/Container.cpp)
target_include_directories(web_server PRIVATE ${CMAKE_SOURCE_DIR}/src/framework/ioc ${CMAKE_SOURCE_DIR}/src/services ${CMAKE_SOURCE_DIR}/src/storage)
target_link_libraries(web_server muduo_core ${LIBS})

add_executable(redis_example RedisExample.cpp ../src/storage/cache/RedisClient.cpp ../src/storage/cache/RedisPool.cpp ../src/framework/utils/ConfigManager.cpp)
target_include_directories(redis_example PRIVATE ${CMAKE_SOURCE_DIR}/src/storage/cache ${CMAKE_SOURCE_DIR}/src/framework/utils)

target_link_libraries(redis_example hiredis muduo_core yaml-cpp ${LIBS})

add_executable(config_hot_reload ConfigHotReload.cpp ../src/framework/utils/ConfigManager.cpp)
target_include_directories(config_hot_reload PRIVATE ${CMAKE_SOURCE_DIR}/src/framework/utils)
target_link_libraries(config_hot_reload yaml-cpp)

target_link_libraries(redis_example hiredis muduo_core ${LIBS})


add_executable(session_example SessionExample.cpp ../src/framework/session/Session.cpp)
target_include_directories(session_example PRIVATE ${CMAKE_SOURCE_DIR}/src/framework ${CMAKE_SOURCE_DIR}/src/modules)
target_link_libraries(session_example muduo_http)

add_executable(web_server WebServer.cpp)
target_include_directories(web_server PRIVATE ${CMAKE_SOURCE_DIR}/src/core/include ${CMAKE_SOURCE_DIR}/src/modules ${CMAKE_SOURCE_DIR}/src/framework ${CMAKE_SOURCE_DIR}/src/services)
target_link_libraries(web_server muduo_http muduo_core ${LIBS})

add_executable(logger_stress LoggerStress.cpp ../src/framework/utils/ConfigManager.cpp)
target_include_directories(logger_stress PRIVATE ${CMAKE_SOURCE_DIR}/src/framework/utils)
target_link_libraries(logger_stress muduo_core ${LIBS})

add_executable(router_example RouterExample.cpp)
target_include_directories(router_example PRIVATE ${CMAKE_SOURCE_DIR}/src/framework ${CMAKE_SOURCE_DIR}/src/modules)
target_link_libraries(router_example muduo_http)


