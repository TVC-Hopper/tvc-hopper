#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>

#include <sh2/shtp.h>
#include <sh2/sh2.h>
#include <sh2/sh2_SensorValue.h>
#include <sh2/sh2_err.h>

/**
 *  BNO085 I2C address
 */
#define BNO085_I2C_ADDR     0x4A

/**
 *  Number of distinct sensor reports received
 */
#define BNO085_READING_COUNT 3

/**
 *  Initialization parameters for BNO085
 */
typedef struct Bno085InitParams_s {
    uint16_t buffer_size;
    uint8_t (*onWrite)(uint8_t address, uint8_t *buffer, uint16_t len);
    uint8_t (*onRead)(uint8_t address, uint8_t *buffer, uint16_t len);
    uint32_t (*getTime_us)();
    void (*onDelay)(uint32_t ms);
} Bno085InitParams_t;

/**
 *  BNO085 object
 */
typedef struct Bno085_s {
    sh2_ProductIds_t product_ids; 
    uint16_t buffer_size;
    sh2_Hal_t sh2hal;
    uint8_t sensor_value_count;
    sh2_SensorValue_t sensor_values[BNO085_READING_COUNT];
    sh2_SensorId_t sensor_map[BNO085_READING_COUNT];
    uint32_t (*getTime_us)();
    uint8_t (*onWrite)(uint8_t address, uint8_t *buffer, uint16_t len);
    uint8_t (*onRead)(uint8_t address, uint8_t *buffer, uint16_t len);
    void (*onDelay)(uint32_t ms);
} Bno085_t;

/**
 *  Initialize BNO085 driver object
 *
 *  @param b object
 *  @param bip Initialization parameters
 *  @return true on success, false on failure
 */
extern bool Bno085_Init(Bno085_t *b, Bno085InitParams_t *bip);

/**
 *  Starts and configures sensor hub. I2C must be configured and ready for use
 *  before calling this function. If using any RTOS compatible HAL, ensure
 *  RTOS has started before calling this function. This must also be called
 *  before any requests are made of the sensor hub.
 *
 *  @param b object
 *  @return true on success, false on failure
 */
extern bool Bno085_InitSensorHub(Bno085_t* b);

/**
 *  Configure a sensor report.
 *
 *  @param b object
 *  @param sensor sensor from which to generate reports
 *  @param interval_us microsecond interval between report generation
 *  @return true on success, false on failure
 */
extern bool Bno085_EnableReport(Bno085_t *b, sh2_SensorId_t sensor, uint32_t interval_us);

/**
 *  Service sensor hub and receive generated reports. Call this function to match report
 *  interval set in BNo085_EnableReport
 *
 *  @param b object
 *  @return true on success, false on failure
 */
extern bool Bno085_GetSensorEvents(Bno085_t *b);

/**
 *  Extract certain number of floats from the received sensor reports
 *
 *  @param b object
 *  @param sensor sensor from which to get measurements
 *  @param count number of floats to read
 *  @param floating buffer to fill with count number of floats
 *  @return true on success, false on failure
 */
extern bool Bno085_GetSensorValueFloat(Bno085_t *b, sh2_SensorId_t sensor, uint8_t count, float* floating);

#endif
