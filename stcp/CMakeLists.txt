
project(stcp VERSION 0.0 LANGUAGES C CXX)

add_library(stcp STATIC
    src/stcp.c
)

target_include_directories(stcp
    PRIVATE
        include/stcp
    INTERFACE
        include
)
