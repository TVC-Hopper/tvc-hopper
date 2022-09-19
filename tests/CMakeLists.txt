cmake_minimum_required(VERSION 3.17)

project(hopper_tests VERSION 0.0 LANGUAGES C CXX)

find_package(Catch2 3 REQUIRED)
add_subdirectory(../drivers drivers)


set(DRIVER_TESTS
    drivers/test_esc.cpp
    drivers/test_imu.cpp
    drivers/test_servo.cpp
)

add_executable(tests
    ${DRIVER_TESTS}
)

target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PUBLIC drivers)

