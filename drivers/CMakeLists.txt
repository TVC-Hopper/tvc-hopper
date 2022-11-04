add_library(drivers STATIC
    src/bno085_i2c.c
    src/tfl_i2c.c
)

target_include_directories(drivers
    PRIVATE
        include/drivers
    INTERFACE
        include/
)
        

