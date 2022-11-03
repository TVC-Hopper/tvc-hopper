#include "modules/controls_inputs.h"

#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "hw/lidar.h"
#include "hw/imu.h"


SemaphoreHandle_t start_capture_sem;
SemaphoreHandle_t imu_data_mx;
SemaphoreHandle_t lidar_data_mx;

static uint32_t lidar_distance = 0;
static float imu_placeholder[4] = {0};

extern void ControlsInputs_Init() {
    imu_data_mx = xSemaphoreCreateMutex();
    lidar_data_mx = xSemaphoreCreateMutex();
    start_capture_sem = xSemaphoreCreateBinary();
}

extern void ControlsInputs_Task() {
    while(1) {
        xSemaphoreTake(start_capture_sem, 0xFFFF);
       
        uint32_t temp_distance;
        HwLidar_GetDistance(&temp_distance);
        
        xSemaphoreTake(lidar_data_mx, 0xFFFF);
        lidar_distance = temp_distance;
        xSemaphoreGive(lidar_data_mx);

        xSemaphoreTake(imu_data_mx, 0xFFFF);
        xSemaphoreGive(imu_data_mx);
    }
}

extern void ControlsInputs_NotifyStart() {
    xSemaphoreGive(start_capture_sem);
}


extern void ControlsInputs_GetLidar(uint32_t *distance) {
    xSemaphoreTake(lidar_data_mx, 0xFFFF);
    *distance = lidar_distance;
    xSemaphoreGive(lidar_data_mx);
}

extern void ControlsInputs_GetIMU(float *data) {
    xSemaphoreTake(imu_data_mx, 0xFFFF);
    memcpy(data, imu_placeholder, sizeof(imu_placeholder));
    xSemaphoreGive(imu_data_mx);
}



