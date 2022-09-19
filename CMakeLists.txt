cmake_minimum_required(VERSION 3.17)

project(hopper VERSION 0.0 LANGUAGES C CXX ASM)


add_executable(${PROJECT_NAME} src/main.c)

add_subdirectory(drivers)
target_link_libraries(${PROJECT_NAME} PUBLIC drivers)
