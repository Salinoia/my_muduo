enable_testing()

add_executable(spsc_test SpscRingBufferTest.cpp)
target_include_directories(spsc_test PRIVATE ${CMAKE_SOURCE_DIR}/framework/utils)
add_test(NAME spsc_test COMMAND spsc_test)

add_executable(router_interceptor_test RouterInterceptorTest.cpp)
target_include_directories(router_interceptor_test PRIVATE ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(router_interceptor_test muduo_http)
add_test(NAME router_interceptor_test COMMAND router_interceptor_test)

add_executable(http_server_test HttpServerTest.cpp)
target_include_directories(http_server_test PRIVATE ${CMAKE_SOURCE_DIR}/tests/mocks ${CMAKE_SOURCE_DIR}/modules ${CMAKE_SOURCE_DIR}/core/include)
add_test(NAME http_server_test COMMAND http_server_test)

add_executable(router_test RouterTest.cpp)
target_include_directories(router_test PRIVATE ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(router_test muduo_http)
add_test(NAME router_test COMMAND router_test)

add_executable(config_manager_test ConfigManagerTest.cpp)
target_include_directories(config_manager_test PRIVATE ${CMAKE_SOURCE_DIR}/framework/utils)
add_test(NAME config_manager_test COMMAND config_manager_test)

add_executable(auth_role_interceptor_test AuthRoleInterceptorTest.cpp)
target_include_directories(auth_role_interceptor_test PRIVATE ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(auth_role_interceptor_test muduo_http)

add_executable(connection_pool_test ConnectionPoolTest.cpp)
target_include_directories(connection_pool_test PRIVATE ${CMAKE_SOURCE_DIR}/tests/mocks ${CMAKE_SOURCE_DIR}/storage/db/ConnectionPool)
add_test(NAME connection_pool_test COMMAND connection_pool_test)


add_executable(container_test ContainerTest.cpp ../framework/ioc/Container.cpp)
target_include_directories(container_test PRIVATE ${CMAKE_SOURCE_DIR}/framework/ioc)

add_executable(router_route_test RouterRouteTest.cpp)
target_include_directories(router_route_test PRIVATE ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/modules ${CMAKE_SOURCE_DIR}/core/include)
target_link_libraries(router_route_test muduo_http)


add_executable(http_json_test HttpJsonTest.cpp)
target_include_directories(http_json_test PRIVATE ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(http_json_test muduo_http)

add_executable(session_test SessionTest.cpp ../framework/session/Session.cpp)
target_include_directories(session_test PRIVATE ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(session_test muduo_http)

add_executable(config_reload_test ConfigReloadTest.cpp ../framework/utils/ConfigManager.cpp)
target_include_directories(config_reload_test PRIVATE ${CMAKE_SOURCE_DIR}/framework/utils)
target_link_libraries(config_reload_test yaml-cpp)

