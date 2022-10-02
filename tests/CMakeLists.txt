cmake_minimum_required(VERSION 3.17)

project(hopper_tests VERSION 0.0 LANGUAGES C CXX)

find_package(Catch2 3 REQUIRED)
add_subdirectory(../circbuf circbuf)
add_subdirectory(../spp spp)
add_subdirectory(../drivers drivers)
add_subdirectory(../stcp stcp)


set(DRIVER_TESTS
    drivers/test_esc.cpp
    drivers/test_imu.cpp
    drivers/test_servo.cpp
)

set(SPP_TESTS
    spp/test_spp.cpp
    spp/test_tvc.cpp
    spp/test_spp_client.cpp
)

set(CBUF_TESTS
    circbuf/test_cbuf.cpp
)

set(STCP_TESTS
    stcp/test_stcp.cpp
)


add_executable(tests
    ${DRIVER_TESTS}
    ${SPP_TESTS}
    ${STCP_TESTS}
    ${CBUF_TESTS}
)

target_include_directories(tests PUBLIC ../circbuf/include)
target_include_directories(tests PUBLIC ../stcp/include)
target_include_directories(tests PUBLIC ../spp/include)
target_include_directories(tests PUBLIC ../drivers/include)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PUBLIC circbuf)
target_link_libraries(tests PUBLIC stcp)
target_link_libraries(tests PUBLIC spp_client)
target_link_libraries(tests PUBLIC spp_host)
target_link_libraries(tests PUBLIC drivers)

