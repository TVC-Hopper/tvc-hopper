
cmake_minimum_required(VERSION 3.17)

project(telemetry VERSION 0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(../../circbuf circbuf)
add_subdirectory(../../stcp stcp)
add_subdirectory(../../spp spp)
add_subdirectory(tcp_server_client-master)

add_library(telemetry 
    src/telemetry_comms.cpp
    src/rs232.c
)

target_include_directories(telemetry
    PRIVATE
        include/telemetry
        tcp_server_client-master/include
        ../../spp/include
        ../../stcp/include
    INTERFACE
        include
)

target_link_libraries(telemetry PUBLIC circbuf tcp_client_server stcp spp_host)


add_executable(telemetry_server
    main.cpp
)

target_include_directories(telemetry_server
    PRIVATE
        include/telemetry
        tcp_server_client-master/include
)

target_link_libraries(telemetry_server PUBLIC telemetry)
