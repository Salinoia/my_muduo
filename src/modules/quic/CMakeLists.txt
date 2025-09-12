file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
 add_library(muduo_quic SHARED ${SRC_FILES})
 target_include_directories(muduo_quic PUBLIC ${CMAKE_SOURCE_DIR}/src/core/include ${CMAKE_SOURCE_DIR}/src/modules)
 target_link_libraries(muduo_quic PUBLIC muduo_core)

