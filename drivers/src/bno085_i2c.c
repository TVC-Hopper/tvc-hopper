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

    b->sh2hal.open = Open;
    b->sh2hal.close = Close;
    b->sh2hal.read = Read;
    b->sh2hal.write = Write;
    b->sh2hal.getTimeUs = GetTimeUs;

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

extern bool Bno085_GetSensorEvent(Bno085_t *b, sh2_SensorId_t sensor, sh2_SensorValue_t *value) {
    b->sensor_value = value;

    value->timestamp = 0;

    sh2_service();

    if (value->timestamp == 0 && value->sensorId != SH2_GYRO_INTEGRATED_RV) {
        // no new events
        return false;
    }

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
    uint8_t soft_reset[5] = {5, 0, 1, 0, 1};

    imu->onWrite(BNO085_I2C_ADDR, soft_reset, 5);

    return 0;
}

static void Close(sh2_Hal_t *self) {
}

static int Read(sh2_Hal_t *self, uint8_t *buffer, unsigned length, uint32_t *t_us) {
    uint8_t header[4];
    
    Bno085_t* imu = (Bno085_t*)self->instance_data;
    imu->onRead(BNO085_I2C_ADDR, header, 4);

    uint16_t packet_size = (uint16_t)header[0] | (uint16_t)header[1] << 8;
    packet_size &= ~0x8000;

    uint16_t cargo_remaining = packet_size;
    uint8_t i2c_buffer[imu->buffer_size];
    uint16_t read_size;
    uint16_t cargo_read_amount = 0;
    bool first_read = true;

    while (cargo_remaining > 0) {
        if (first_read) {
          read_size = min(imu->buffer_size, (size_t)cargo_remaining);
        } else {
          read_size = min(imu->buffer_size, (size_t)cargo_remaining + 4);
        }

        imu->onRead(BNO085_I2C_ADDR, i2c_buffer, read_size);

        if (first_read) {
          cargo_read_amount = read_size;
          memcpy(buffer, i2c_buffer, cargo_read_amount);
          first_read = false;
        } else {
          cargo_read_amount = read_size - 4;
          memcpy(buffer, i2c_buffer + 4, cargo_read_amount);
        }
        buffer += cargo_read_amount;
        cargo_remaining -= cargo_read_amount;
    }


    return cargo_remaining == 0 ? 0 : packet_size;
}

static int Write(sh2_Hal_t *self, uint8_t *buffer, unsigned length) {
    Bno085_t* imu = (Bno085_t*)self->instance_data;

    uint32_t write_size = min(length, imu->buffer_size);
    imu->onWrite(BNO085_I2C_ADDR, buffer, write_size);

    if (length - write_size == 0) {
        return 0;
    } else {
        return write_size;
    }
}


static uint32_t GetTimeUs(sh2_Hal_t *self) {
    Bno085_t* imu = (Bno085_t*)self->instance_data;
    return imu->getTime_us() * 1000;
}

static void HalCallback(void *cookie, sh2_AsyncEvent_t *pEvent) {
    Bno085_t* imu = (Bno085_t*)cookie;
    // If we see a reset, set a flag so that sensors will be reconfigured.
    if (pEvent->eventId == SH2_RESET) {
    }
}

static void SensorHandler(void *cookie, sh2_SensorEvent_t *event) {
    Bno085_t* imu = (Bno085_t*)cookie;
    int rc;

    rc = sh2_decodeSensorEvent(imu->sensor_value, event);
    if (rc != SH2_OK) {
        imu->sensor_value->timestamp = 0;
    }
}
