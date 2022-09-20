
add_library(fslhal
    src/fsl_clock.c
    src/fsl_lpuart.c
    src/fsl_gpio.c
    src/fsl_flexspi_nor_boot.c
)

target_sources(fslhal
    PRIVATE
        src/fsl_common_arm.c
        src/fsl_common.c
)

target_include_directories(fslhal
    PRIVATE
        ../device
        ../CMSIS
        include/fslhal
    INTERFACE
        include
)

if(BUILD_MIMXRT1010EVK_BSP)
    target_include_directories(fslhal
        PRIVATE
            ../bsp/mimxrt1010-evk/include/bsp
    )
endif()
