
add_library(fslhal
    src/fsl_adc.c
    src/fsl_assert.c
    src/fsl_clock.c
    src/fsl_debug_console.c
    src/fsl_flexspi.c
    src/fsl_flexspi_nor_boot.c
    src/fsl_gpio.c
    src/fsl_lpi2c.c
    src/fsl_lpspi.c
    src/fsl_lpuart.c
    src/fsl_memcpy.S
    src/fsl_pwm.c
    src/fsl_sbrk.c
    src/fsl_str.c
    src/fsl_xbara.c
    src/fsl_lpuart_freertos.c
    src/fsl_lpi2c_freertos.c
    src/fsl_lpspi_freertos.c

    component/fsl_adapter_lpuart.c
    component/fsl_component_generic_list.c
    component/fsl_component_serial_manager.c
    component/fsl_component_serial_port_uart.c
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
        include/component

        ../freertos/freertos_kernel/portable/GCC/ARM_CM4F
        ../freertos/freertos_kernel/include/freertos
        ../src
    INTERFACE
        include
)

if(BUILD_MIMXRT1010EVK_BSP)
    target_include_directories(fslhal
        PRIVATE
            ../bsp/mimxrt1010-evk/include/bsp
    )
endif()
