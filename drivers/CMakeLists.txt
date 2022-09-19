add_library(drivers STATIC
    src/imu.c
    src/esc.c
    src/servo.c
)

target_include_directories(drivers
    PRIVATE
        include/drivers
    INTERFACE
        include/
)
        

