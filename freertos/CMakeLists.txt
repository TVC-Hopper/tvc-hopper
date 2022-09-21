

add_library(freertos STATIC
    freertos_kernel/croutine.c
    freertos_kernel/event_groups.c
    freertos_kernel/list.c
    freertos_kernel/queue.c
    freertos_kernel/stream_buffer.c
    freertos_kernel/tasks.c
    freertos_kernel/timers.c

    freertos_kernel/portable/MemMang/heap_4.c
    freertos_kernel/portable/GCC/ARM_CM4F/port.c
)

target_include_directories(freertos
    PRIVATE
        freertos_kernel/include/freertos
        freertos_kernel/portable/GCC/ARM_CM4F
        ../src # needs FreeRTOSConfig.h
    INTERFACE
        include
)
