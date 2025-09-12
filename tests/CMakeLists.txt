add_executable(spsc_test SpscRingBufferTest.cpp)
target_include_directories(spsc_test PRIVATE ${CMAKE_SOURCE_DIR}/framework/utils)

add_executable(router_interceptor_test RouterInterceptorTest.cpp)
target_include_directories(router_interceptor_test PRIVATE ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(router_interceptor_test muduo_http)

add_executable(http_json_test HttpJsonTest.cpp)
target_include_directories(http_json_test PRIVATE ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(http_json_test muduo_http)

