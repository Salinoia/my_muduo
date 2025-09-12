file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(muduo_http SHARED ${SRC_FILES})

target_include_directories(muduo_http PUBLIC ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/modules ${CMAKE_SOURCE_DIR}/framework)

target_link_libraries(muduo_http PUBLIC muduo_core)

