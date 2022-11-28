#include "bno085_i2c.h"

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <sh2/shtp.h>
#include <sh2/sh2.h>
#include <sh2/sh2_SensorValue.h>
#include <sh2/sh2_err.h>

static int Open(sh2_Hal_t *self);
static void Close(sh2_Hal_t *self);
static int Read(sh2_Hal_t *self, uint8_t *buffer, unsigned length, uint32_t *t_us);
static int Write(sh2_Hal_t *self, uint8_t *buffer, unsigned length);
static uint32_t GetTimeUs(sh2_Hal_t *self);
static void HalCallback(void* cookie, sh2_AsyncEvent_t *pEvent);
static void SensorHandler(void* cookie, sh2_SensorEvent_t *event);

// don't include all of math.h for this function
static uint32_t min(uint32_t a, uint32_t b) {
    if (a < b) {
        return a;
    } else {
        return b;
    }
}

extern bool Bno085_Init(Bno085_t *b, Bno085InitParams_t *bip) {
    b->buffer_size = bip->buffer_size;
    b->getTime_us = bip->getTime_us;
    b->onWrite = bip->onWrite;
    b->onRead = bip->onRead;
    b->onDelay = bip->onDelay;

    sh2_SensorId_t id_map[3] = {SH2_LINEAR_ACCELERATION, SH2_GYROSCOPE_CALIBRATED, SH2_ROTATION_VECTOR};
    memcpy(b->sensor_map, id_map, sizeof(id_map));

    b->sh2hal.open = Open;
    b->sh2hal.close = Close;
    b->sh2hal.read = Read;
    b->sh2hal.write = Write;
    b->sh2hal.getTimeUs = GetTimeUs;
    b->sh2hal.instance_data = b;

    return true;
}

extern bool Bno085_InitSensorHub(Bno085_t* b) {
    int status = sh2_open(&b->sh2hal, HalCallback, b);
    if (status != SH2_OK) {
        return false;
    }

    memset(&b->product_ids, 0, sizeof(b->product_ids));
    status = sh2_getProdIds(&b->product_ids);

    if (status != SH2_OK) {
        return false;
    }

    sh2_setSensorCallback(SensorHandler, b);
    return true;
}

extern bool Bno085_GetSensorEvents(Bno085_t *b) {
    b->sensor_value_count = 0;

    // this needs to be called periodically.
    // polls sensor hub for new reports
    sh2_service();

    return true;
}

extern bool Bno085_GetSensorValueFloat(Bno085_t *b, sh2_SensorId_t sensor, uint8_t count, float* floating) {
    uint8_t s_idx;

    // find reading index
    for (s_idx = 0; s_idx < BNO085_READING_COUNT; ++s_idx) {
        if (b->sensor_values[s_idx].sensorId == sensor) {
            break;
        }
    }

    if (sensor == SH2_LINEAR_ACCELERATION) {
        memcpy(floating, &b->sensor_values[s_idx].un.linearAcceleration, count * sizeof(float));
    } else if (sensor == SH2_GYROSCOPE_CALIBRATED) {
        memcpy(floating, &b->sensor_values[s_idx].un.gyroscope, count * sizeof(float));
    } else if (sensor == SH2_ROTATION_VECTOR) {
        memcpy(floating, &b->sensor_values[s_idx].un.rotationVector, count * sizeof(float));
    }

    // TODO: add other report IDs

    return true;
}

extern bool Bno085_EnableReport(Bno085_t *b, sh2_SensorId_t sensor, uint32_t interval_us) {
    static sh2_SensorConfig_t config;

    config.changeSensitivityEnabled = false;
    config.wakeupEnabled = false;
    config.changeSensitivityRelative = false;
    config.alwaysOnEnabled = false;
    config.changeSensitivity = 0;
    config.batchInterval_us = 0;
    config.sensorSpecific = 0;

    config.reportInterval_us = interval_us;
    int status = sh2_setSensorConfig(sensor, &config);

    if (status != SH2_OK) {
        return false;
    }

    return true; 
}


static int Open(sh2_Hal_t *self) {
    Bno085_t* imu = (Bno085_t*)self->instance_data;
    // LSB length 5
    // MSB length 0
    // Channel 1
    // Seq 0
    // Data = 0x01
    uint8_t soft_reset[5] = {5, 0, 1, 0, 1};
   
    // attempt soft reset 5 times
    for (uint8_t i = 0; i < 5; ++i) {
        imu->onWrite(BNO085_I2C_ADDR, soft_reset, 5);
        imu->onDelay(30);
    }

    imu->onDelay(300);

    return 0;
}

static void Close(sh2_Hal_t *self) {
    // do nothing
}

static int Read(sh2_Hal_t *self, uint8_t *buffer, unsigned length, uint32_t *t_us) {
    uint8_t header[4];
    
    Bno085_t* imu = (Bno085_t*)self->instance_data;

    // read header
    imu->onRead(BNO085_I2C_ADDR, header, 4);

    // length is 2 bytes
    uint16_t packet_size = (uint16_t)header[0] | (uint16_t)header[1] << 8;

    // clear continuation byte
    packet_size &= ~0x8000;

    uint16_t cargo_remaining = packet_size;
    uint8_t i2c_buffer[imu->buffer_size];
    uint16_t read_size;
    uint16_t cargo_read_amount = 0;
    bool first_read = true;

    // read until no more bytes are needed
    while (cargo_remaining > 0) {
        if (first_read) {
            read_size = min(imu->buffer_size, (size_t)cargo_remaining);
        } else {
            read_size = min(imu->buffer_size, (size_t)cargo_remaining + 4);
        }

        // header is included in every I2C read
        imu->onRead(BNO085_I2C_ADDR, i2c_buffer, read_size);

        if (first_read) {
            // first read will contain header
            // keep it
            cargo_read_amount = read_size;
            memcpy(buffer, i2c_buffer, cargo_read_amount);
            first_read = false;
        } else {
            // don't include 4 byte header in following reads
            // SH2 will not be able to parse the received packet
            cargo_read_amount = read_size - 4;
            memcpy(buffer, i2c_buffer + 4, cargo_read_amount);
        }

        buffer += cargo_read_amount;
        cargo_remaining -= cargo_read_amount;
    }


    // return bytes read
    return cargo_remaining == 0 ? packet_size : 0;
}

static int Write(sh2_Hal_t *self, uint8_t *buffer, unsigned length) {
    Bno085_t* imu = (Bno085_t*)self->instance_data;

    uint32_t write_size = min(length, imu->buffer_size);
    imu->onWrite(BNO085_I2C_ADDR, buffer, write_size);

    return write_size;
}


static uint32_t GetTimeUs(sh2_Hal_t *self) {
    Bno085_t* imu = (Bno085_t*)self->instance_data;
    // get time in microseconds
    // TODO: fix precision
    return imu->getTime_us() * 1000;
}

static void HalCallback(void *cookie, sh2_AsyncEvent_t *pEvent) {
    // If we see a reset, set a flag so that sensors will be reconfigured.
    if (pEvent->eventId == SH2_RESET) {
    }
}

static void SensorHandler(void *cookie, sh2_SensorEvent_t *event) {
    Bno085_t* imu = (Bno085_t*)cookie;
    int rc;

    if (event->reportId == SH2_LINEAR_ACCELERATION) {
        rc = sh2_decodeSensorEvent(&imu->sensor_values[0], event);
    } else if (event->reportId == SH2_GYROSCOPE_CALIBRATED) {
        rc = sh2_decodeSensorEvent(&imu->sensor_values[1], event);
    } else if (event->reportId == SH2_ROTATION_VECTOR) {
        rc = sh2_decodeSensorEvent(&imu->sensor_values[2], event);
    }

    // TODO: add other report IDs
}



