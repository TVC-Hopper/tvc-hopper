cmake_minimum_required(VERSION 3.17)

project(hopper VERSION 0.0 LANGUAGES C ASM)

option(BUILD_MIMXRT1010EVK_BSP "Build MIMRX1010-EVK BSP" ON)
option(BUILD_FLIGHTCONTROLLER_BSP "Build flight controller BSP" OFF)

add_compile_definitions(CPU_MIMXRT1011DAE5A)

include(cmake/flags.cmake)

add_subdirectory(circbuf)
add_subdirectory(spp)
add_subdirectory(stcp)
add_subdirectory(freertos)
add_subdirectory(drivers)
add_subdirectory(bsp)
add_subdirectory(fslhal)

add_executable(${PROJECT_NAME}.elf
    #    device/startup_mimxrt1011.c
    device/startup_MIMXRT1011.S
    device/system_MIMXRT1011.c

    src/main.c
    src/app_hal_xconnect.c

    src/modules/command_control_comms.c
    src/modules/spp_property_list.c
)

target_include_directories(${PROJECT_NAME}.elf
    PRIVATE
        src

        fslhal/include/fslhal
        device
        CMSIS 
        stcp/include
        spp/include

        freertos/freertos_kernel/include
        freertos/freertos_kernel/include/freertos
        freertos/freertos_kernel/portable/GCC/ARM_CM4F
)

if(BUILD_MIMXRT1010EVK_BSP)
    target_include_directories(${PROJECT_NAME}.elf
        PRIVATE
            ../bsp/mimxrt1010-evk/include/bsp
    )
endif()

target_link_libraries(${PROJECT_NAME}.elf PUBLIC circbuf spp_client stcp bsp fslhal drivers freertos)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE -Wl,--start-group)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE m)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE c)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE gcc)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE nosys)
target_link_libraries(${PROJECT_NAME}.elf PRIVATE -Wl,--end-group)
