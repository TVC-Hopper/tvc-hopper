#include "imu.h"
#include <stdint.h>

/////////////////// IMU Functions ////////////////////

/*
 * intialize the IMU
 */
int IMU_init(IMU_t *imu /* other args TBD */){return 0;}


/////////////// Accelerometer Functions ///////////////

/* 
 * update x,y,z components of acceleration
 */
void IMU_getAccReading(IMU_t *imu){return;}

/* 
 * write data to accelerometer reg
 */
int IMU_writeAccReg(IMU_t *imu, uint8_t reg, AccReg_t data){return 0;}

/* 
 * read data from accelerometer reg
 */
uint8_t IMU_readAccReg(IMU_t *imu, AccReg_t reg){return 0;}

///////////////// Gyroscope Functions /////////////////

/* 
 * update x,y,z components of Angular Velocity
 */
void IMU_getGyroReading(IMU_t *imu){return;}

/* 
 * write data to gyroscope reg
 */
int IMU_writeGyroReg(IMU_t *imu, uint8_t reg, GyroReg_t data){return 0;}

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_readGyroReg(IMU_t *imu, uint8_t reg){return 0;}

//////////////// Magnetometer Functions ////////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_getMagReading(IMU_t *imu){return;}

/* 
 * write data to gyroscope reg
 */
int IMU_writeMagReg(IMU_t *imu, MagReg_t reg, uint8_t data){return 0;}

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_readMagReg(IMU_t *imu, MagReg_t reg){return 0;}

///////////// Temperature Sensor Functions //////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_getTempReading(IMU_t *imu){return;}

/* 
 * write data to gyroscope reg
 */
int IMU_writeTempReg(IMU_t *imu, TempReg_t reg, uint8_t data){return 0;}

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_readTempReg(IMU_t *imu, TempReg_t reg){return 0;}