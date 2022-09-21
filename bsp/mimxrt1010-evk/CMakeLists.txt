
add_library(bsp STATIC
    src/board.c
    src/clock_config.c
    src/peripherals.c
    src/pin_mux.c
    src/evkmimxrt1010_flexspi_nor_config.c
)

target_include_directories(bsp
    PRIVATE
        include/bsp
        ../../fslhal/include/fslhal
        ../../fslhal/include/component
        ../../device
        ../../CMSIS

        ../../freertos/freertos_kernel/portable/GCC/ARM_CM4F
        ../../freertos/freertos_kernel/include/freertos
        ../../src
    INTERFACE
        include
)


