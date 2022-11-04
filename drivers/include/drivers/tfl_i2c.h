#ifndef DRIVER_LIDAR_TFL_H
#define DRIVER_LIDAR_TFL_H

#include <stdint.h>
#include <stdbool.h>

#define TFL_DEF_ADR         ((uint8_t) 0x10)
#define TFL_DEF_FPS         ((uint8_t) 0x64)

#define TFL_DIST_L          ((uint8_t) 0x00)
#define TFL_DIST_H          ((uint8_t) 0x01)
#define TFL_FLUX_L          ((uint8_t) 0x02)
#define TFL_FLUX_H          ((uint8_t) 0x03)
#define TFL_TEMP_L          ((uint8_t) 0x04)
#define TFL_TEMP_H          ((uint8_t) 0x05)
#define TFL_TICK_L          ((uint8_t) 0x06)
#define TFL_TICK_H          ((uint8_t) 0x07)
#define TFL_ERR_L           ((uint8_t) 0x08)
#define TFL_ERR_H           ((uint8_t) 0x09)
#define TFL_VER_REV         ((uint8_t) 0x0A)
#define TFL_VER_MIN         ((uint8_t) 0x0B)
#define TFL_VER_MAJ         ((uint8_t) 0x0C)

#define TFL_SAVE_SETTINGS   ((uint8_t) 0x20)
#define TFL_SOFT_RESET      ((uint8_t) 0x21)


#define TFL_SET_I2C_ADDR    ((uint8_t) 0x22)
#define TFL_SET_TRIG_MODE   ((uint8_t) 0x23)
#define TFL_TRIGGER         ((uint8_t) 0x24)
#define TFL_DISABLE         ((uint8_t) 0x25)
#define TFL_FPS_L           ((uint8_t) 0x26)
#define TFL_FPS_H           ((uint8_t) 0x27)
#define TFL_SET_L_PWR       ((uint8_t) 0x28)
#define TFL_HARD_RESET      ((uint8_t) 0x29)

#define TFL_FPS_1               0x01
#define TFL_FPS_2               0x02
#define TFL_FPS_3               0x03
#define TFL_FPS_4               0x04
#define TFL_FPS_5               0x05
#define TFL_FPS_6               0x06
#define TFL_FPS_7               0x07
#define TFL_FPS_8               0x08
#define TFL_FPS_9               0x09
#define TFL_FPS_10              0x0A

#define TFL_FPS_35              0x23
#define TFL_FPS_50              0x32
#define TFL_FPS_100             0x64
#define TFL_FPS_125             0x7D
#define TFL_FPS_250             0xFA


#define TFL_READY            0  // no error
#define TFL_SERIAL           1  // serial timeout
#define TFL_HEADER           2  // no header found
#define TFL_CHECKSUM         3  // checksum doesn't match
#define TFL_TIMEOUT          4  // I2C timeout
#define TFL_PASS             5  // reply from some system commands
#define TFL_FAIL             6  //           "
#define TFL_I2CREAD          7
#define TFL_I2CWRITE         8
#define TFL_I2CLENGTH        9
#define TFL_WEAK            10  // Signal Strength ≤ 100
#define TFL_STRONG          11  // Signal Strength saturation
#define TFL_FLOOD           12  // Ambient Light saturation
#define TFL_MEASURE         13
#define TFL_INVALID         14  // Invalid operation sent to sendCommand()

#define TFL_DEFAULT_I2C     ((uint8_t) 0x10)

typedef struct TfLidarInitParams_s {
    uint8_t address;
    uint8_t (*onWrite)(uint8_t address, uint8_t subaddress, uint8_t* buffer, uint16_t size);
    uint8_t (*onRead)(uint8_t address, uint8_t subaddress, uint8_t* buffer, uint16_t size);
} TfLidarInitParams_t;


typedef struct TfLidar_s {
    uint8_t address;
    uint8_t buffer[6];
    uint8_t reply_reg;
    uint8_t status;
    uint8_t (*onWrite)(uint8_t address, uint8_t subaddress, uint8_t* buffer, uint16_t size);
    uint8_t (*onRead)(uint8_t address, uint8_t subaddress, uint8_t* buffer, uint16_t size);
} TfLidar_t;

extern void Tfl_Init(TfLidar_t *tfl, TfLidarInitParams_t *ip);

extern bool Tfl_GetDistance(TfLidar_t *tfl, uint16_t *distance);

extern bool Tfl_SetContinuousMode(TfLidar_t *tfl);

extern bool Tfl_ReadRegister(TfLidar_t *tfl, uint8_t reg);

extern bool Tfl_WriteRegister(TfLidar_t *tfl, uint8_t reg, uint8_t data);

extern bool Tfl_GetFrameRate(TfLidar_t *tfl, uint16_t *frame_rate);

extern bool Tfl_SetFrameRate(TfLidar_t *tfl, uint16_t frame_rate);

extern bool Tfl_SetEnable(TfLidar_t *tfl);

extern bool Tfl_SetDisable(TfLidar_t *tfl);

extern bool Tfl_ResetSoft(TfLidar_t *tfl);

extern bool Tfl_ResetHard(TfLidar_t *tfl);

extern bool Tfl_SaveSettings(TfLidar_t *tfl);

#endif
