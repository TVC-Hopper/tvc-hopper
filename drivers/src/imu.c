#include "imu.h"
#include <stdint.h>
#include <stdbool.h>

#define SIZE_3AXIS_READ 6
#define I2C_READ_FAIL 1
#define I2C_WRITE_FAIL 2

/////////////////////////Initialize IMU /////////////////////////////
int IMU_init(IMU_t* imu, AccelInit_t* Acc, GyroInit_t* gyro){return 0;}

/////////////// Accelerometer Functions ///////////////

//set accelerometer register
void IMU_setAccReg(IMU_t* imu, AccReg_t regAddr, uint8_t regWriteVal){
  _IMU_setReg(imu, ACCEL, regAddr, regWriteVal);
}

//return accelerometer register value
uint8_t IMU_getAccReg(IMU_t* imu, AccReg_t regAddr){
  _IMU_readReg(imu, ACCEL, regAddr, 1);
  return imu->_RxTxData.bytes[0];
}

// set range
void IMU_setAccelRange(IMU_t *imu, AccRange_t range){
  IMU_setAccReg(imu, ACC_RANGE, range);
  imu->Acc.LSB = ((2 << (range + 1)) * 1.5) / 32768.0;
}

// set over sampling ratio, output datarate
void IMU_setAccelOsrOdr(IMU_t *imu, ACC_OSR_t osr, ACC_ODR_t odr){
  uint8_t writeVal = (osr << 4) | odr;
  IMU_setAccReg(imu, ACC_CONF, writeVal);
}

// update x,y,z components of acceleration
void IMU_getAccReading(IMU_t *imu){
  _IMU_readReg(imu, ACCEL, ACC_X_LSB, SIZE_3AXIS_READ);
  _processRawData(&(imu->_RxTxData));
  imu->Acc.xComp = _convertToGs(imu->_RxTxData.shorts[0], imu->Acc.LSB);
  imu->Acc.yComp = _convertToGs(imu->_RxTxData.shorts[1], imu->Acc.LSB);
  imu->Acc.zComp = _convertToGs(imu->_RxTxData.shorts[2], imu->Acc.LSB);
}

//convert measurement from LSBs to Gs
float _convertToGs(int16_t reading, float LSB){
  return reading * LSB;
}

//intialize accelerometer
void _IMU_AccInit(IMU_t* imu, AccelInit_t* acc){
  IMU_setAccelRange(imu, acc->range);
  IMU_setAccelOsrOdr(imu, acc->osr, acc->odr);  
  return;
}

//configure accelerometer interrupts
void _IMU_AccIntConfig(IMU_t* imu, AccelInt_t* acc){ return;}

///////////// Temperature Sensor Functions //////////////
#define SIZE_TEMP_READ 2
void IMU_getTempReading(IMU_t* imu){
    _IMU_readReg(imu, ACCEL, TEMP_MSB, SIZE_TEMP_READ);
    int16_t upper_8bits = (int8_t)imu->_RxTxData.bytes[0];
    int16_t lower_3bits = (int8_t)imu->_RxTxData.bytes[1];
    int16_t tempDataLsb = (upper_8bits << 3) | lower_3bits;
    imu->degC = tempDataLsb * .125 + 23;
}

///////////////// Gyroscope Functions /////////////////

//set gyro register
void IMU_setGyroReg(IMU_t *imu, GyroReg_t regAddr, uint8_t regWriteVal){
  _IMU_setReg(imu, GYRO, regAddr, regWriteVal);
}

//return gyro register value
uint8_t IMU_getGyroReg(IMU_t* imu, GyroReg_t regAddr){
  _IMU_readReg(imu, GYRO, regAddr, 1);
  return imu->_RxTxData.bytes[0];
}

// set range
void IMU_setGyroRange(IMU_t *imu, GyroRange_t range){
  IMU_setGyroReg(imu, GYRO_RANGE, range);
  imu->Gyro.LSB = (2000.0 / (2 << range)) / 32767.0;
}

// set over sampling ratio, output datarate
void IMU_setGyroOdrBW(IMU_t *imu, GyroOdrBw_t odrBw){
  IMU_setGyroReg(imu, GYRO_BANDWIDTH, odrBw);
}

// update x,y,z components of Angular Velocity
void IMU_getGyroReading(IMU_t *imu){
  _IMU_readReg(imu, GYRO, RATE_X_LSB, SIZE_3AXIS_READ);
  _processRawData(&(imu->_RxTxData));
  imu->Gyro.xComp = _convertToDegPerSec(imu->_RxTxData.shorts[0], imu->Gyro.LSB);
  imu->Gyro.yComp = _convertToDegPerSec(imu->_RxTxData.shorts[1], imu->Gyro.LSB);
  imu->Gyro.zComp = _convertToDegPerSec(imu->_RxTxData.shorts[2], imu->Gyro.LSB);
}

//convert measurement from LSBs to angular velocity
float _convertToDegPerSec(uint16_t reading, float LSB){
  return reading * LSB;
}

//intialize gyroscope
void _IMU_GyroInit(IMU_t* imu, GyroInit_t* gyro){
  IMU_setGyroRange(imu, gyro->range);
  IMU_setGyroOdrBW(imu, gyro->OdrBw);
  return;
}

//configure gyroscope interrupts
void _IMU_GyroIntConfig(IMU_t* imu, GyroInt_t* acc){ return;}

/////////////////////////private general IMU implementation functions///////////////////////
#define REG_SEL_MSG_LEN 1
#define WRITE_REG_MSG_LEN 2

//convert raw data measurement to LSBs
void _processRawData(rawData_t *reading){
  for (int i = 0; i < 3; ++i){
    reading->shorts[i] = (reading->bytes[i + 1] << 8) | reading->bytes[i];
  }
}

//set imu register
uint8_t _IMU_setReg(IMU_t *imu, deviceType_t dev, uint8_t regAddr, uint8_t regWriteVal){
  _IMU_readReg(imu, dev, regAddr, 1);
  uint8_t regReadVal = imu->_RxTxData.bytes[0];
  regReadVal &= ~regWriteVal;
  regWriteVal |= regReadVal;
  _IMU_writeReg(imu, dev, regAddr, regWriteVal);
}

//write IMU register
uint8_t _IMU_writeReg(IMU_t *imu, deviceType_t dev, uint8_t regAddr, uint8_t regWriteVal){
  _IMU_setMsgLen(imu, WRITE_REG_MSG_LEN);
  _IMU_setDevAddr(imu, dev);
  _IMU_setRegAddr(imu, regAddr);
  _IMU_setRegVal(imu, regWriteVal);
  return _IMU_write(imu);
}

//Read IMU register
uint8_t _IMU_readReg(IMU_t *imu, deviceType_t dev, uint8_t regAddr, uint16_t msgLen){
  _IMU_setMsgLen(imu, REG_SEL_MSG_LEN);
  _IMU_setDevAddr(imu, dev);
  _IMU_setRegAddr(imu, regAddr);
  _IMU_write(imu);
  _IMU_setMsgLen(imu, msgLen);
  return _IMU_read(imu);
}

//set device address for I2C
void _IMU_setDevAddr(IMU_t *imu, deviceType_t dev){
  if (dev == ACCEL){imu->_curI2CdevAddr = imu->Acc.devAddr;}
  else {imu->_curI2CdevAddr = imu->Gyro.devAddr;}
}

//set message length for I2C
void _IMU_setMsgLen(IMU_t* imu, uint16_t msgLen){
  imu->_msgLen = msgLen;
}

//set register address for I2C
void _IMU_setRegAddr(IMU_t *imu, uint8_t regAddr){
  imu->_RxTxData.bytes[SLAVE_REG_ADDR] = regAddr;
}

//set register write value for I2C
void _IMU_setRegVal(IMU_t *imu, uint8_t regWriteVal){
  imu->_RxTxData.bytes[SLAVE_REG_VAL] = regWriteVal;
}

//invoke write callback
uint8_t _IMU_write(IMU_t *imu){
  return imu->i2cCallBacks.onI2Cwrite(imu->_curI2CdevAddr, imu->_RxTxData.bytes, imu->_msgLen);  
}

//invoke read callback
uint8_t _IMU_read(IMU_t *imu){
  return imu->i2cCallBacks.onI2Cread(imu->_curI2CdevAddr, imu->_RxTxData.bytes, imu->_msgLen);
}
