project(circbuf VERSION 0.00 LANGUAGES C CXX)

option(BUILD_CBUF_TESTS "Build circbuf tests" ON)

add_library(circbuf STATIC
    src/cbuf.c
)

target_include_directories(circbuf
    PRIVATE
        include/circbuf
    INTERFACE
        include
)
