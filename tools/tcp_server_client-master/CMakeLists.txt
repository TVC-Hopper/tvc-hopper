cmake_minimum_required(VERSION 3.8.1)
project(tcp_client_server VERSION 0.0 LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(${PROJECT_NAME}
        src/tcp_client.cpp
        src/tcp_server.cpp
        src/client.cpp
        src/pipe_ret_t.cpp
        src/common.cpp)

option(SERVER_EXAMPLE "Build SERVER" OFF)

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

if(SERVER_EXAMPLE)

    add_definitions(
            -DSERVER_EXAMPLE
    )

    add_executable(tcp_server examples/server_example.cpp)

    target_link_libraries (tcp_server ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

endif()

option(CLIENT_EXAMPLE "Build CLIENT" OFF)

if(CLIENT_EXAMPLE)

    add_definitions(
            -DCLIENT_EXAMPLE
    )

    add_executable(tcp_client examples/client_example.cpp)

    target_link_libraries (tcp_client ${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

endif()
