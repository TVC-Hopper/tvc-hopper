
cmake_minimum_required(VERSION 3.17)

project(telemetry VERSION 0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(../../circbuf circbuf)
add_subdirectory(../../stcp stcp)
add_subdirectory(../../spp spp)
add_subdirectory(../tcp_server_client-master tcp_server_client-master)

add_executable(telemetry_emulator
    src/spp_property_list.c
    src/main.cpp
)

target_link_libraries(telemetry_emulator PUBLIC circbuf tcp_client_server stcp spp_client)

target_include_directories(telemetry_emulator
    PRIVATE
        src
        ../tcp_server_client-master/include
        ../../spp/include
        ../../stcp/include
)
