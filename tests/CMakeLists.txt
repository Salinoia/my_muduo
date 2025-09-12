add_executable(spsc_test SpscRingBufferTest.cpp)
target_include_directories(spsc_test PRIVATE ${CMAKE_SOURCE_DIR}/framework/utils)

add_executable(router_interceptor_test RouterInterceptorTest.cpp)
target_include_directories(router_interceptor_test PRIVATE ${CMAKE_SOURCE_DIR}/framework ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(router_interceptor_test muduo_http)

add_executable(config_reload_test ConfigReloadTest.cpp ../framework/utils/ConfigManager.cpp)
target_include_directories(config_reload_test PRIVATE ${CMAKE_SOURCE_DIR}/framework/utils)
target_link_libraries(config_reload_test yaml-cpp)
