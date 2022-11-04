#include "tfl_i2c.h"

#include <stdint.h>
#include <stdbool.h>

extern void Tfl_Init(TfLidar_t *tfl, TfLidarInitParams_t *ip) {
    tfl->address = ip->address;
    tfl->onWrite = ip->onWrite;
    tfl->onRead = ip->onRead;
}

extern bool Tfl_GetDistance(TfLidar_t *tfl, uint16_t *distance) {
    tfl->status = TFL_READY;

    uint8_t* distance_bytes = (uint8_t*)distance;

    if (!Tfl_ReadRegister(tfl, TFL_DIST_L)) {
            return false;
    } else {
        distance_bytes[0] = tfl->reply_reg;
    }
        
    if (!Tfl_ReadRegister(tfl, TFL_DIST_H)) {
            return false;
    } else {
        distance_bytes[1] = tfl->reply_reg;
    }

    return true;

}

extern bool Tfl_ReadRegister(TfLidar_t *tfl, uint8_t reg) {
    tfl->onRead(tfl->address, reg, tfl->buffer, 1);
    tfl->reply_reg = tfl->buffer[0];
    return true;
}

extern bool Tfl_WriteRegister(TfLidar_t *tfl, uint8_t reg, uint8_t data) {
    tfl->buffer[0] = data;
    tfl->onRead(tfl->address, reg, tfl->buffer, 1);
    return true;
}

extern bool Tfl_GetFrameRate(TfLidar_t *tfl, uint16_t *frame_rate) {
    uint8_t* frame_rate_bytes = (uint8_t*)frame_rate;

    if (!Tfl_ReadRegister(tfl, TFL_FPS_L)) {
            return false;
    } else {
        frame_rate_bytes[0] = tfl->reply_reg;
    }

    if (!Tfl_ReadRegister(tfl, TFL_FPS_H)) {
            return false;
    } else {
        frame_rate_bytes[1] = tfl->reply_reg;
    }

    return true;
}

extern bool Tfl_SetFrameRate(TfLidar_t *tfl, uint16_t frame_rate) {
    uint8_t* frame_rate_bytes = (uint8_t*)(&frame_rate);

    if (!Tfl_WriteRegister(tfl, TFL_FPS_L, frame_rate_bytes[0])) {
        return false;
    }

    if (!Tfl_WriteRegister(tfl, TFL_FPS_H, frame_rate_bytes[1])) {
        return false;
    }

    return true;
}

extern bool Tfl_SetEnable(TfLidar_t *tfl) {
    return Tfl_WriteRegister(tfl, TFL_DISABLE, 1);
}

extern bool Tfl_SetDisable(TfLidar_t *tfl) {
    return Tfl_WriteRegister(tfl, TFL_DISABLE, 0);
}

extern bool Tfl_SetContinuousMode(TfLidar_t *tfl) {
    return Tfl_WriteRegister(tfl, TFL_SET_TRIG_MODE, 0);
}

extern bool Tfl_ResetSoft(TfLidar_t *tfl) {
    return Tfl_WriteRegister(tfl, TFL_SOFT_RESET, 2);
}

extern bool Tfl_ResetHard(TfLidar_t *tfl) {
    return Tfl_WriteRegister(tfl, TFL_HARD_RESET, 1);
}

extern bool Tfl_SaveSettings(TfLidar_t *tfl) {
    return Tfl_WriteRegister(tfl, TFL_SAVE_SETTINGS, 1);
}

