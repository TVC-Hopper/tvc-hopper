#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>

#include <sh2/shtp.h>
#include <sh2/sh2.h>
#include <sh2/sh2_SensorValue.h>
#include <sh2/sh2_err.h>


#define BNO085_I2C_ADDR     0x4A

typedef struct Bno085InitParams_s {
    uint16_t buffer_size;
    uint8_t (*onWrite)(uint8_t address, uint8_t *buffer, uint16_t len);
    uint8_t (*onRead)(uint8_t address, uint8_t *buffer, uint16_t len);
    uint32_t (*getTime_us)();
} Bno085InitParams_t;


typedef struct Bno085_s {
    sh2_ProductIds_t product_ids; 
    uint16_t buffer_size;
    sh2_Hal_t sh2hal;
    sh2_SensorValue_t *sensor_value;
    uint32_t (*getTime_us)();
    uint8_t (*onWrite)(uint8_t address, uint8_t *buffer, uint16_t len);
    uint8_t (*onRead)(uint8_t address, uint8_t *buffer, uint16_t len);
} Bno085_t;

extern bool Bno085_Init(Bno085_t *b, Bno085InitParams_t *bip);

extern bool Bno085_InitSensorHub(Bno085_t* b);

extern bool Bno085_EnableReport(Bno085_t *b, sh2_SensorId_t sensor, uint32_t interval_us);

extern bool Bno085_GetSensorEvent(Bno085_t *b, sh2_SensorId_t sensor, sh2_SensorValue_t *value);

#endif
