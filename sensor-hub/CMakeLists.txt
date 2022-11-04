add_library(sh2
    src/sh2.c
    src/sh2_util.c
    src/sh2_SensorValue.c
    src/shtp.c
)

target_include_directories(sh2
    PRIVATE
        include/sh2

    INERFACE
        include
)
        


