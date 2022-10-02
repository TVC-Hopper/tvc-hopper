#include "imu.h"
#include <stdint.h>
#include <stdbool.h>

#define SIZE_3AXIS_READ 6
#define I2C_READ_FAIL 1
#define I2C_WRITE_FAIL 2

/************************************/
/* accelerometer specific functions */
/************************************/

//turn on accelerometer
#define ACC_ON 0x04
void IMU_powerUpAcc(IMU_t *imu){
  _IMU_setDevAddr(imu, ACCEL);
  _IMU_writeReg(imu, ACC_PWR_CTRL, ACC_ON);
  imu->Callbacks.onDelay_us(450);
}

//reset all registers to defaults
#define SOFT_RESET 0xB6
void IMU_softResetAcc(IMU_t* imu){
  _IMU_setDevAddr(imu, ACCEL);
  _IMU_writeReg(imu, ACC_SOFT_RESET, SOFT_RESET);
}

uint8_t IMU_readErrorStatusAcc(IMU_t* imu){
  return IMU_getAccReg(imu, ACC_ERR_REG);
}

// update x,y,z components of acceleration
void IMU_getAccReading(IMU_t *imu){
  _IMU_setDevAddr(imu, ACCEL);
  _IMU_readReg(imu, ACC_X_LSB, SIZE_3AXIS_READ);
  _processRawData(&(imu->_RxTxData));
  imu->Acc.xComp = _convertToGs(imu->_RxTxData.shorts[0], imu->Acc.LSB);
  imu->Acc.yComp = _convertToGs(imu->_RxTxData.shorts[1], imu->Acc.LSB);
  imu->Acc.zComp = _convertToGs(imu->_RxTxData.shorts[2], imu->Acc.LSB);
}


// get temp reading
#define SIZE_TEMP_READ 2
void IMU_getTempReading(IMU_t* imu){
  _IMU_setDevAddr(imu, ACCEL);
  _IMU_readReg(imu, TEMP_MSB, SIZE_TEMP_READ);
  int16_t upper_8bits = (int8_t)imu->_RxTxData.bytes[0];
  int16_t lower_3bits = imu->_RxTxData.bytes[1] >> 5;
  int16_t tempDataLsb = (upper_8bits << 3) | lower_3bits;
  imu->degC = tempDataLsb * .125 + 23;
}

//read accelerometer register
uint8_t IMU_getAccReg(IMU_t* imu, AccReg_t regAddr){
  _IMU_setDevAddr(imu, ACCEL);
  _IMU_readReg(imu, regAddr, 1);
  return imu->_RxTxData.bytes[0];
}

//set accelerometer osr odr 
void IMU_setAccelOsrOdr(IMU_t *imu, ACC_OSR_t osr, ACC_ODR_t odr){
  uint8_t writeVal = (osr << 4) | odr;
  _IMU_setAccReg(imu, ACC_CONF, writeVal, MASK_ALL);
}

//set accelerometer range
void IMU_setAccelRange(IMU_t *imu, AccRange_t range){
  _IMU_setAccReg(imu, ACC_RANGE, range, ACC_RANGE_MASK);
  imu->Acc.LSB = ((2 << (range + 1)) * 1.5) / 32768.0;
}

//convert between lsbs to gs
float _convertToGs(int16_t reading, float LSB){
  return reading * LSB;
}

//set accelerometer register
void _IMU_setAccReg(IMU_t* imu, AccReg_t regAddr, uint8_t regWriteVal, uint8_t mask){
  _IMU_setDevAddr(imu, ACCEL);
  _IMU_setReg(imu, regAddr, regWriteVal, mask);
}

//configure accelerometer interrupts
void _IMU_AccIntConfig(IMU_t* imu, IntInit_t* accInts){ 
  uint8_t inOutShift = 0;
  uint8_t regWriteVal = 0;
  uint8_t regAddr = 0;
  if (accInts->pinIsInput) {inOutShift = 4;}
  else {inOutShift = 3;}
  regWriteVal = (1 << inOutShift) | (accInts->PpOd << 2) | (accInts->activeLvl << 1);
  if (accInts->IntPin == INT1){regAddr = INT1_IOCTRL;}
  else {regAddr = INT2_IOCTRL;}
  _IMU_setAccReg(imu, regAddr, regWriteVal, INT_IO_CTRL_MASK);
  _IMU_setAccReg(imu, INT_MAP_DATA, accInts->IntPin, INT_MAP_DATA_MASK);
}

//intialize accelerometer
void _IMU_AccInit(IMU_t* imu, AccelInit_t* acc){
  IMU_setAccelRange(imu, acc->range);
  IMU_setAccelOsrOdr(imu, acc->osr, acc->odr); 
  _IMU_AccIntConfig(imu, &(acc->AccInts)); 
  return;
}

/********************************/
/* gyroscope specific functions */
/********************************/

//reset all registers to defaults
void IMU_softResetGyro(IMU_t* imu){
  _IMU_setDevAddr(imu, GYRO);
  _IMU_writeReg(imu, GYRO_SOFT_RESET, SOFT_RESET);
  imu->Callbacks.onDelay_us(30000);
}

//get error status gyro
uint8_t IMU_readErrorStatusGyro(IMU_t* imu){
  _IMU_setDevAddr(imu, GYRO);
  _IMU_writeReg(imu, GYRO_SELF_TEST, 0x01);
  while (!(IMU_getGyroReg(imu, GYRO_SELF_TEST) & GYRO_TEST_READY_MASK)){}
  return IMU_getGyroReg(imu, GYRO_SELF_TEST) & GYRO_TEST_STATUS_MASK;
}

// update x,y,z components of Angular Velocity
void IMU_getGyroReading(IMU_t *imu){
  _IMU_setDevAddr(imu, GYRO);
  _IMU_readReg(imu, RATE_X_LSB, SIZE_3AXIS_READ);
  _processRawData(&(imu->_RxTxData));
  imu->Gyro.xComp = _convertToDegPerSec(imu->_RxTxData.shorts[0], imu->Gyro.LSB);
  imu->Gyro.yComp = _convertToDegPerSec(imu->_RxTxData.shorts[1], imu->Gyro.LSB);
  imu->Gyro.zComp = _convertToDegPerSec(imu->_RxTxData.shorts[2], imu->Gyro.LSB);
}

//return gyro register value
uint8_t IMU_getGyroReg(IMU_t* imu, GyroReg_t regAddr){
  _IMU_setDevAddr(imu, GYRO);
  _IMU_readReg(imu, regAddr, 1);
  return imu->_RxTxData.bytes[0];
}

//set gyro register
void _IMU_setGyroReg(IMU_t *imu, GyroReg_t regAddr, uint8_t regWriteVal, uint8_t mask){
  _IMU_setDevAddr(imu, GYRO);
  _IMU_setReg(imu, regAddr, regWriteVal, mask);
}

//set gyro odr and BW
void IMU_setGyroOdrBW(IMU_t *imu, GyroOdrBw_t odrBw){
  _IMU_setGyroReg(imu, GYRO_BANDWIDTH, odrBw, MASK_ALL);
}

//set gyro range range
void IMU_setGyroRange(IMU_t *imu, GyroRange_t range){
  _IMU_setGyroReg(imu, GYRO_RANGE, range, MASK_ALL);
  imu->Gyro.LSB = (2000.0 / (2 << range)) / 32767.0;
}

//convert measurement from LSBs to angular velocity
float _convertToDegPerSec(uint16_t reading, float LSB){
  return reading * LSB;
}

//initialize gyro interrupts
#define GYRO_INT_ENABLE 0x80
void _IMU_GyroIntConfig(IMU_t* imu, IntInit_t* gyroInts){
  uint8_t regWriteVal = 0;
  uint8_t pinConfShift = 0;
  if(gyroInts->IntPin == INT4){pinConfShift = 2;}
  regWriteVal = ((gyroInts->PpOd << 1) | (gyroInts->activeLvl)) << pinConfShift;
  _IMU_setGyroReg(imu, INT3_INT4_IO_CONF, regWriteVal, GYRO_INT_IO_CONF_MASK);
  _IMU_setGyroReg(imu, INT3_INT4_IO_MAP, gyroInts->IntPin, GYRO_INT_IO_MAP_MASK);
  _IMU_setGyroReg(imu, GYRO_INT_CTRL, GYRO_INT_ENABLE, GYRO_INT_CTRL_MASK);
}

//initialize gyroscope device
void _IMU_GyroInit(IMU_t* imu, GyroInit_t* gyro){
  IMU_setGyroRange(imu, gyro->range);
  IMU_setGyroOdrBW(imu, gyro->OdrBw);
  _IMU_GyroIntConfig(imu, &(gyro->GyroInts));
  return;
}
/****************************************/
/* IMU driver object specific functions */
/****************************************/

#define REG_SEL_MSG_LEN 1
#define WRITE_REG_MSG_LEN 2

//initialize gyroscope driver object
uint8_t IMU_init(IMU_t* imu, AccelInit_t* accInit, GyroInit_t* gyroInit, IMUinit_t* imuInit){
  if (imuInit->sd01 == SDO1_HIGH){
    imu->Acc.devAddr = 0x19;
    imu->Gyro.devAddr = 0x69;
  }
  else {
    imu->Acc.devAddr = 0x18;
    imu->Gyro.devAddr = 0x68;
  }
  imu->Callbacks.onDelay_us = imuInit->onDelay_us;
  imu->Callbacks.onI2Cwrite = imuInit->onI2Cwrite;
  imu->Callbacks.onI2Cread = imuInit->onI2Cread;

  imu->Callbacks.onDelay_us(30000);
  IMU_powerUpAcc(imu);

  if (accInit) {_IMU_AccInit(imu, accInit);}
  if (gyroInit) {_IMU_GyroInit(imu, gyroInit);}
  
  return IMU_readErrorStatusAcc(imu) | IMU_readErrorStatusGyro(imu);
  
}

//convert raw data measurement to LSBs
void _processRawData(rawData_t *reading){
  for (int i = 0; i < 3; ++i){
    reading->shorts[i] = (reading->bytes[i + 1] << 8) | reading->bytes[i];
  }
}

//set imu register
uint8_t _IMU_setReg(IMU_t *imu, uint8_t regAddr, uint8_t regWriteVal, uint8_t mask){
  _IMU_readReg(imu, regAddr, 1); 
  uint8_t regReadVal = imu->_RxTxData.bytes[0];  // 1) preserve non functional bits
  regReadVal &= ~mask;                           // 2) clear functional bits
  regWriteVal |= regReadVal;                     // 3) set functional bits
  _IMU_writeReg(imu, regAddr, regWriteVal); 
  return 0; 
}

//write IMU register
uint8_t _IMU_writeReg(IMU_t *imu, uint8_t regAddr, uint8_t regWriteVal){
  _IMU_setMsgLen(imu, WRITE_REG_MSG_LEN);
  _IMU_setRegAddr(imu, regAddr);
  _IMU_setRegVal(imu, regWriteVal);
  return _IMU_write(imu);
}

//Read IMU register
uint8_t _IMU_readReg(IMU_t *imu, uint8_t regAddr, uint16_t msgLen){
  _IMU_setMsgLen(imu, REG_SEL_MSG_LEN);
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
  return imu->Callbacks.onI2Cwrite(imu->_curI2CdevAddr, imu->_RxTxData.bytes, imu->_msgLen);  
}

//invoke read callback
uint8_t _IMU_read(IMU_t *imu){
  return imu->Callbacks.onI2Cread(imu->_curI2CdevAddr, imu->_RxTxData.bytes, imu->_msgLen);
}
