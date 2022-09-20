
add_library(bsp STATIC
    src/clock_config.c
    src/peripherals.c
    src/pin_mux.c
    src/evkmimxrt1010_flexspi_nor_config.c
)

target_include_directories(bsp
    PRIVATE
        include/bsp
        ../../fslhal/include/fslhal
        ../../device
        ../../CMSIS
    INTERFACE
        include
)


