#include "lidar.h"

#include <drivers/tfl_i2c.h>

#include "app_hal_xconnect.h"

TfLidar_t lidar;

extern void HwLidar_Init() {
    TfLidarInitParams_t ip;
    ip.address = TFL_DEFAULT_I2C;
    ip.onWrite = XCCb_I2CWrite;
    ip.onRead = XCCb_I2CRead;

    Tfl_Init(&lidar, &ip);

}

extern uint32_t HwLidar_GetDistance() {
    uint16_t distance;
    Tfl_GetDistance(&lidar, &distance);
    return distance;
}
