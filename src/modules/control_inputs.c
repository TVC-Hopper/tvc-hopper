#include "modules/control_inputs.h"

#include <string.h>
#include <math.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

#include "hw/lidar.h"
#include "hw/imu.h"

SemaphoreHandle_t start_capture_sem;

SemaphoreHandle_t imu_data_raw_mx;
SemaphoreHandle_t imu_data_proc_mx;
SemaphoreHandle_t lidar_data_mx;

static uint32_t lidar_distance = 0;


// linear accel <- x, y, z (q=8)
// gyro <- x, y, z (q=9)
// yaw, pitch, roll (quaternions) <- i, j, k, real (q=14)
static float imu_data_raw[10]= {0};
static float imu_data_proc[9] = {0};

static float Qi = 0;
static float Qj = 0;
static float Qk = 0;
static float Qreal = 0;
static float norm = 0;

static void ComputeQuaternions();
static void ProcessIMU();

extern void ControlsInputs_Init() {
    imu_data_raw_mx = xSemaphoreCreateMutex();
    imu_data_proc_mx = xSemaphoreCreateMutex();
    lidar_data_mx = xSemaphoreCreateMutex();
    start_capture_sem = xSemaphoreCreateBinary();
}

extern void ControlsInputs_Task(void* task_args) {
    while(1) {
        xSemaphoreTake(start_capture_sem, 0xFFFF);
       
        uint32_t temp_distance = HwLidar_GetDistance();
        
        xSemaphoreTake(lidar_data_mx, 0xFFFF);
        lidar_distance = temp_distance;
        xSemaphoreGive(lidar_data_mx);

        xSemaphoreTake(imu_data_raw_mx, 0xFFFF);
        HwImu_GetReadings(imu_data_raw);
        xSemaphoreGive(imu_data_raw_mx); // FIXME

        ComputeQuaternions();

        ProcessIMU();
    }
}

extern void ControlsInputs_NotifyStart() {
    xSemaphoreGive(start_capture_sem);
}

extern void ControlsInputs_GetLidar(float *distance) {
    xSemaphoreTake(lidar_data_mx, 0xFFFF);
    *distance = (float)lidar_distance;
    xSemaphoreGive(lidar_data_mx);
}

extern void ControlsInputs_GetIMURaw(float *data) {
    xSemaphoreTake(imu_data_raw_mx, 0xFFFF);
    memcpy(data, imu_data_raw, sizeof(imu_data_raw));
    xSemaphoreGive(imu_data_raw_mx);
}

extern void ControlsInputs_GetIMUProcessed(float *data) {
    xSemaphoreTake(imu_data_proc_mx, 0xFFFF);
    memcpy(data, imu_data_proc, sizeof(imu_data_proc));
    xSemaphoreGive(imu_data_proc_mx);
}

static void ComputeQuaternions() {
    xSemaphoreTake(imu_data_raw_mx, 0xFFFF);

	Qreal = imu_data_raw[IMU_RAW_IDX_QUATREAL];
    Qi = imu_data_raw[IMU_RAW_IDX_QUATI];
	Qj = imu_data_raw[IMU_RAW_IDX_QUATJ];
	Qk = imu_data_raw[IMU_RAW_IDX_QUATK];

    // give as soon as possible
    xSemaphoreGive(imu_data_raw_mx);

    norm = sqrt(Qreal*Qreal + Qi*Qi + Qj*Qj + Qk*Qk);

    Qreal /= norm;
    Qi /= norm;
    Qj /= norm;
    Qk /= norm;

}

static void ProcessIMU() {

	// x-axis rotation
	float t0 = +2.0 * (Qreal * Qi + Qj * Qk);
	float t1 = +1.0 - 2.0 * (Qi * Qi + Qj * Qj);
	float roll = atan2(t0, t1);


	// y-axis rotation
	float t2 = +2.0 * (Qreal * Qj - Qk * Qi);
	t2 = t2 > 1.0 ? 1.0 : t2;
	t2 = t2 < -1.0 ? -1.0 : t2;
	float pitch = asin(t2);

	// z-axis rotation
	float t3 = +2.0 * (Qreal * Qk + Qi * Qj);
	float t4 = +1.0 - 2.0 * (Qj * Qj + Qk * Qk);
	float yaw = atan2(t3, t4);

    xSemaphoreTake(imu_data_raw_mx, 0xFFFF);

    xSemaphoreTake(imu_data_proc_mx, 0xFFFF);
	imu_data_proc[IMU_PROC_IDX_ROLL] = roll;
	imu_data_proc[IMU_PROC_IDX_PITCH] = pitch;
    imu_data_proc[IMU_PROC_IDX_YAW] = yaw;

    xSemaphoreTake(imu_data_raw_mx, 0xFFFF);
    memcpy(&imu_data_proc[IMU_PROC_IDX_GYRO], &imu_data_raw[IMU_RAW_IDX_GYRO], 3 * sizeof(float));
    xSemaphoreGive(imu_data_raw_mx);

    xSemaphoreGive(imu_data_proc_mx);

}
