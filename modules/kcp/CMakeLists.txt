file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(muduo_kcp SHARED ${SRC_FILES})
target_include_directories(muduo_kcp PUBLIC ${CMAKE_SOURCE_DIR}/core/include ${CMAKE_SOURCE_DIR}/modules)
target_link_libraries(muduo_kcp PUBLIC muduo_core)

