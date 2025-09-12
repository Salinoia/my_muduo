add_executable(echo EchoServer.cpp)
target_link_libraries(echo muduo_core ${LIBS})

add_executable(client Client.cpp)
target_link_libraries(client muduo_core ${LIBS})


add_executable(redis_example RedisExample.cpp ../storage/cache/RedisClient.cpp ../storage/cache/RedisPool.cpp)
target_include_directories(redis_example PRIVATE ${CMAKE_SOURCE_DIR}/storage/cache)
target_link_libraries(redis_example hiredis ${LIBS})

