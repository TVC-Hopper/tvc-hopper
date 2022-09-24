#ifndef IMU_H
#define IMU_H

#include <stdint.h>

/*
 * struct to register I2C callbacks
 * set by application
 */

typedef struct IMUcbk {
    
    int (*onI2Cwrite)(uint16_t addr, uint8_t *bus_data, uint16_t size);
    int (*onI2Cread)(uint16_t addr, uint8_t *bus_data, uint16_t size);

} IMUcbk_t;

/* 
 * Accelerometer specific data
 */
typedef struct Accelerometer {
    uint8_t AccelAddr; //accelerometer device addr
    
    float xComp; 
    float yComp; 
    float zComp;
} Accel_t;

/* 
 * Accelerometer register addresses
 */
typedef enum {
    ACC_X = 0x00,
    ACC_Y = 0x01,
    ACC_Z = 0x02
    /* .
       .
       .
          */
} AccReg_t;

/* 
 * Gyroscope specific data
 */
typedef struct Gyroscope {
    uint8_t GyroAddr; //gyroscope device addr

    float xComp; 
    float yComp; 
    float zComp;
} Gyro_t;

/* 
 * Gyroscope register addresses
 */
typedef enum {
    GYR_X = 0x00,
    GYR_Y = 0x01,
    GYR_Z = 0x02
    /* .
       .
       .
          */
} GyroReg_t;

/* 
 * Magnetometer specific data
 */
typedef struct Magnetometer {
    uint8_t MagAddr; //Magnetometer device addr
    
    float xComp; 
    float yComp; 
    float zCOmp;
} Mag_t;

/* 
 * Magnetomoeter register addresses
 */
typedef enum {
    MAG_X = 0x00,
    MAG_Y = 0x01,
    MAG_Z = 0x02
    /* .
       .
       .
          */
} MagReg_t;

/* 
 * Temperature Sensor specific data
 */
typedef struct TempSensor {
    uint8_t TempSenseAddr; //Temperature sensor device addr
    
    float degK;
    
} TempSense_t;

/* 
 * Temperature register addresses
 */
typedef enum {
    TEMP_K = 0x00,
    /* .
       .
       .
          */
} TempReg_t;
;
/* 
 * IMU data
 */
typedef struct IMU {
    
    /* Callbacks */
    IMUcbk_t i2cCallBacks;
    
    /* Sensor Data */
    Accel_t Acc;
    Gyro_t Gyro;
    Mag_t Mag;
    TempSense_t Tsense;

} IMU_t;

/////////////////// IMU Functions ////////////////////

/*
 * intialize the IMU
 */
int IMU_init(IMU_t *imu /* other args TBD */);


/////////////// Accelerometer Functions ///////////////

/* 
 * update x,y,z components of acceleration
 */
void IMU_getAccReading(IMU_t *imu);

/* 
 * write data to accelerometer reg
 */
int IMU_writeAccReg(IMU_t *imu, uint8_t reg, AccReg_t data);

/* 
 * read data from accelerometer reg
 */
uint8_t IMU_readAccReg(IMU_t *imu, AccReg_t reg);

///////////////// Gyroscope Functions /////////////////

/* 
 * update x,y,z components of Angular Velocity
 */
void IMU_getGyroReading(IMU_t *imu);

/* 
 * write data to gyroscope reg
 */
int IMU_writeGyroReg(IMU_t *imu, uint8_t reg, GyroReg_t data);

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_readGyroReg(IMU_t *imu, uint8_t reg);

//////////////// Magnetometer Functions ////////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_getMagReading(IMU_t *imu);

/* 
 * write data to gyroscope reg
 */
int IMU_writeMagReg(IMU_t *imu, MagReg_t reg, uint8_t data);

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_readMagReg(IMU_t *imu, MagReg_t reg);

///////////// Temperature Sensor Functions //////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_getTempReading(IMU_t *imu);

/* 
 * write data to gyroscope reg
 */
int IMU_writeTempReg(IMU_t *imu, TempReg_t reg, uint8_t data);

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_readTempReg(IMU_t *imu, TempReg_t reg);



#endif