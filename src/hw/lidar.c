#include "lidar.h"

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include <drivers/tfl_i2c.h>

#include "app_hal_xconnect.h"

TfLidar_t lidar;

extern void HwLidar_Init() {

    TfLidarInitParams_t ip;
    ip.address = TFL_DEF_ADDR;
    ip.onWrite = XCCb_I2CRegisterWrite;
    ip.onRead = XCCb_I2CRegisterRead;

    Tfl_Init(&lidar, &ip);

}

extern uint32_t HwLidar_GetDistance() {
    uint16_t distance;
    Tfl_GetDistance(&lidar, &distance);
    return distance;
}
