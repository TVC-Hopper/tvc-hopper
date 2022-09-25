#ifndef IMU_H
#define IMU_H

#include <stdint.h>

/*
 * struct to register I2C callbacks
 * set by application
 */

typedef struct IMUCallbacks_s {
    
    int (*onWrite)(uint16_t addr, uint8_t *bus_data, uint16_t size);
    int (*onRead)(uint16_t addr, uint8_t *bus_data, uint16_t size);

} IMUCallbacks_t;

/* 
 * Accelerometer specific data
 */
typedef struct Accelerometer_s {
    uint8_t addr; //accelerometer device addr
    
    float x; 
    float y; 
    float z;
} Accelerometer_t;

/* 
 * Accelerometer register addresses
 */
typedef enum AccelRegister_e {
    ACC_X = 0x00,
    ACC_Y = 0x01,
    ACC_Z = 0x02
    /* .
       .
       .
          */
} AccelRegister_t;

/* 
 * Gyroscope specific data
 */
typedef struct Gyroscope_s {
    uint8_t addr; //gyroscope device addr

    float x; 
    float y; 
    float z;
} Gyroscope_t;

/* 
 * Gyroscope register addresses
 */
typedef enum GyroRegister_e {
    GYR_X = 0x00,
    GYR_Y = 0x01,
    GYR_Z = 0x02
    /* .
       .
       .
          */
} GyroRegister_t;

/* 
 * Magnetometer specific data
 */
typedef struct Magnetometer_s {
    uint8_t addr; //Magnetometer device addr
    
    float x; 
    float y; 
    float z;
} Magnetometer_t;

/* 
 * Magnetomoeter register addresses
 */
typedef enum MagRegister_e {
    MAG_X = 0x00,
    MAG_Y = 0x01,
    MAG_Z = 0x02
    /* .
       .
       .
          */
} MagRegister_t;

/* 
 * Temperature Sensor specific data
 */
typedef struct TempSensor_s {
    uint8_t addr; //Temperature sensor device addr
    
    float degK;
    
} TempSensor_t;

/* 
 * Temperature register addresses
 */
typedef enum TempRegister_e {
    TEMP_K = 0x00,
    /* .
       .
       .
          */
} TempRegister_t;
;
/* 
 * IMU data
 */
typedef struct IMU_s {
    
    /* Callbacks */
    IMUCallbacks_t callbacks;
    
    /* Sensor Data */
    Accelerometer_t accel;
    Gyroscope_t gyro;
    Magnetometer_t mag;
    TempSensor_t tsense;

} IMU_t;

/////////////////// IMU Functions ////////////////////

/*
 * intialize the IMU
 */
int IMU_Init(IMU_t *imu /* other args TBD */);


/////////////// Accelerometer Functions ///////////////

/* 
 * update x,y,z components of acceleration
 */
void IMU_GetAccReading(IMU_t *imu);

/* 
 * write data to accelerometer reg
 */
int IMU_WriteAccReg(IMU_t *imu, AccelRegister_t reg, uint8_t data);

/* 
 * read data from accelerometer reg
 */
uint8_t IMU_ReadAccReg(IMU_t *imu, AccelRegister_t reg);

///////////////// Gyroscope Functions /////////////////

/* 
 * update x,y,z components of Angular Velocity
 */
void IMU_GetGyroReading(IMU_t *imu);

/* 
 * write data to gyroscope reg
 */
int IMU_WriteGyroReg(IMU_t *imu, GyroRegister_t reg, uint8_t data);

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_ReadGyroReg(IMU_t *imu, uint8_t reg);

//////////////// Magnetometer Functions ////////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_GetMagReading(IMU_t *imu);

/* 
 * write data to gyroscope reg
 */
int IMU_WriteMagReg(IMU_t *imu, MagRegister_t reg, uint8_t data);

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_ReadMagReg(IMU_t *imu, MagRegister_t reg);

///////////// Temperature Sensor Functions //////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_GetTempReading(IMU_t *imu);

/* 
 * write data to gyroscope reg
 */
int IMU_WriteTempReg(IMU_t *imu, TempRegister_t reg, uint8_t data);

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_ReadTempReg(IMU_t *imu, TempRegister_t reg);



#endif
