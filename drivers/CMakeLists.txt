add_library(drivers STATIC
    src/imu.c
    src/lidar.c
)

target_include_directories(drivers
    PRIVATE
        include/drivers
    INTERFACE
        include/
)
        

