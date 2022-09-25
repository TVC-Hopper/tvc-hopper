
project(spp VERSION 0.00 LANGUAGES C CXX)

option(BUILD_SPP_TESTS "Build SPP tests" ON)

add_library(spp_host STATIC
    src-host/spp.c
)

target_include_directories(spp_host
    PRIVATE
        include/spphost
        include/common
    INTERFACE
        include
)

add_library(spp_client STATIC
    src-client/spp.c
)

target_include_directories(spp_client
    PRIVATE
        include/sppclient
        include/common
    INTERFACE
        include
)
