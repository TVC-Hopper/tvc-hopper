
if (BUILD_MIMXRT1010EVK_BSP)
    add_subdirectory(mimxrt1010-evk)
elseif (BUILD_FLIGHTCONTROLLER_BSP)
    add_subdirectory(flight-controller)
endif()

target_link_libraries(bsp PUBLIC fslhal)

