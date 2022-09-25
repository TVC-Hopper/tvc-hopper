#include "imu.h"
#include <stdint.h>

/////////////////// IMU Functions ////////////////////

/*
 * intialize the IMU
 */
int IMU_Init(IMU_t *imu /* other args TBD */){return 0;}


/////////////// Accelerometer Functions ///////////////

/* 
 * update x,y,z components of acceleration
 */
void IMU_GetAccReading(IMU_t *imu){return;}

/* 
 * write data to accelerometer reg
 */
int IMU_WriteAccReg(IMU_t *imu, AccRegister_t reg, uint8_t data){return 0;}

/* 
 * read data from accelerometer reg
 */
uint8_t IMU_ReadAccReg(IMU_t *imu, AccReg_t reg){return 0;}

///////////////// Gyroscope Functions /////////////////

/* 
 * update x,y,z components of Angular Velocity
 */
void IMU_GetGyroReading(IMU_t *imu){return;}

/* 
 * write data to gyroscope reg
 */
int IMU_WriteGyroReg(IMU_t *imu, GyroRegister_t reg, uint8_t data){return 0;}

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_ReadGyroReg(IMU_t *imu, GyroRegister_t reg){return 0;}

//////////////// Magnetometer Functions ////////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_GetMagReading(IMU_t *imu){return;}

/* 
 * write data to gyroscope reg
 */
int IMU_WriteMagReg(IMU_t *imu, MagReg_t reg, uint8_t data){return 0;}

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_ReadMagReg(IMU_t *imu, MagReg_t reg){return 0;}

///////////// Temperature Sensor Functions //////////////

/* 
 * update x,y,z components of magnetic field
 */
void IMU_GetTempReading(IMU_t *imu){return;}

/* 
 * write data to gyroscope reg
 */
int IMU_WriteTempReg(IMU_t *imu, TempReg_t reg, uint8_t data){return 0;}

/* 
 * read data from gyroscope reg
 */
uint8_t IMU_ReadTempReg(IMU_t *imu, TempReg_t reg){return 0;}
