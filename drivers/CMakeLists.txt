add_library(drivers STATIC
    src/bno085_i2c.c
    src/tfl_i2c.c
)

target_link_libraries(drivers
    PRIVATE
        sh2
)

target_include_directories(drivers
    PRIVATE
        include/drivers
        ../sensor-hub/include
    INTERFACE
        include/
)
        

