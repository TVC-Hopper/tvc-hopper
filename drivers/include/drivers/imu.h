#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>

//////////////////ACCELEROMETER DATA AND ASSOC. FUNCTIONS///////////////////

//Accelerometer specific data
typedef struct Accelerometer {
    float LSB;
    float xComp; 
    float yComp; 
    float zComp;

    uint8_t devAddr; //accelerometer device addr

} Accel_t;

//3 axis accelerometer read
void IMU_getAccReading(IMU_t *imu);

//convert measurement from LSBs to Gs
float _convertToGs(int16_t reading, float range);


///////////accelerometer registers and associated functions
//acceleromter registers
typedef enum {
    ACC_CHIP_ID    = 0x00,
    ACC_ERR_REG    = 0x02,
    ACC_STATUS     = 0x03,
    ACC_X_LSB      = 0x12,
    ACC_X_MSB      = 0x13,
    ACC_Y_LSB      = 0x14,
    ACC_Y_MSB      = 0x15,
    ACC_Z_LSB      = 0x16,
    ACC_Z_MSB      = 0x17,
    SENSORTIME_0   = 0x18,
    SENSORTIME_1   = 0x19,
    SENSORTIME_2   = 0x1A,
    ACC_INT_STAT_1 = 0x1D,
    TEMP_MSB       = 0x22,
    TEMP_LSB       = 0x23,
    FIFO_LENGTH_0  = 0x24,
    FIFO_LENGTH_1  = 0x25,
    FIFO_DATA      = 0x26,
    ACC_CONF       = 0x40,
    ACC_RANGE      = 0x41,
    FIFO_DOWNS     = 0x45,
    FIFO_WTM_0     = 0x46,
    FIFO_WTM_1     = 0x47,
    FIFO_CONFIG_0  = 0x48,
    FIFO_CONFIG_1  = 0x49,
    INT1_IOCTRL    = 0x53,
    INT2_IOCTRL    = 0x54,
    INT_MAP_DATA   = 0x58,
    ACC_SELF_TEST  = 0x6D,
    ACC_PWR_CONF   = 0x7C,
    ACC_PWR_CTRL   = 0x7D,
    ACC_SOFT_RESET = 0x7E
} AccReg_t;

//set accelerometer register
void IMU_setAccReg(IMU_t* imu, AccReg_t regAddr, uint8_t regWriteVal);

//return accelerometer register value
uint8_t IMU_getAccReg(IMU_t* imu, AccReg_t regAddr);

// get temp reeading (temp registers are in accelerometer)
void IMU_getTempReading(IMU_t* imu);

//set range
typedef enum {
  RANGE_3G  = 0x00, //.0000916 g/LSB
  RANGE_6G  = 0x01, //.000183  g/LSB
  RANGE_12G = 0x02, //.000366  g/LSB
  RANGE_24G = 0x03  //.000632  g/LSB
} AccRange_t;

void IMU_setAccelRange(IMU_t *imu, AccRange_t range);

//set oversampling ratio and output data rate (implicitly sets LP filter bandwidth)
typedef enum {
  OSR4 = 0x08, //4x Nyquist Rate
  OSR2 = 0x09, //2x Nyquist Rate
  NONE = 0x0A  //1x Nyquist Rate
} ACC_OSR_t; 

typedef enum {
  HZ_12_5 = 0x05,
  HZ_25   = 0x06,
  HZ_50   = 0x07,
  HZ_100  = 0x08,
  HZ_200  = 0x09,
  HZ_400  = 0x0A,
  HZ_800  = 0x0B,
  HZ_1600 = 0x0C
} ACC_ODR_t;

void IMU_setAccelOsrOdr(IMU_t *imu, ACC_OSR_t osr, ACC_ODR_t odr);

//Accelerometer device addresses
typedef enum {
    SDO1_LOW  = 0,
    SDO1_HIGH
} SD01State_t;



//////////////////GYROSCOPE PARAMETERS AND ASSOC. FUNCTIONS///////////////////
// Gyroscope specific data
typedef struct Gyroscope { 
    float LSB;
    float xComp; 
    float yComp; 
    float zComp;
    uint8_t devAddr;
} Gyro_t;

// update x,y,z components of Angular Velocity
void IMU_getGyroReading(IMU_t *imu);

//convert measurement from LSBs to angular velocity
float _convertToDegPerSec(uint16_t reading, float LSB);

// Gyroscope register addresses
typedef enum {
    GYRO_CHIP_ID      = 0x00,
    RATE_X_LSB        = 0x02,
    RATE_X_MSB        = 0x03,
    RATE_Y_LSB        = 0x04,
    RATE_Y_MSB        = 0x05,
    RATE_Z_LSB        = 0x06,
    RATE_Z_MSB        = 0x07,
    GYRO_INT_STAT_1   = 0x0A,
    FIFO_STATUS       = 0x0E,
    GYRO_RANGE        = 0x0F,
    GYRO_BANDWIDTH    = 0x10,
    GYRO_LPM1         = 0x11,
    GYRO_SOFT_RESET   = 0x14,
    GYRO_INT_CTRL     = 0x15,
    INT3_INT4_IO_CONF = 0x16,
    INT3_INT4_IO_CONF = 0x18,
    FIFO_WM_EN        = 0x1E,
    FIFO_EXT_INT_S    = 0x34,
    GYRO_SELF_TEST    = 0x3C,
    FIFO_CONFIG_0     = 0x3D,
    FIFO_CONFIG_1     = 0x3E,
    FIFO_DATA         = 0x3F
} GyroReg_t;

//set gyroscope register
void IMU_setGyroReg(IMU_t *imu, GyroReg_t regAddr, uint8_t regWriteVal);

//return gyro register value
uint8_t IMU_getGyroReg(IMU_t* imu, GyroReg_t regAddr);

typedef enum {
  RATE_2000_DS  = 0x00, //.061  deg/sec/LSB
  RATE_1000_DS  = 0x01, //.0305 deg/sec/LSB
  RATE_500_DS   = 0x02, //.0153 deg/sec/LSB
  RATE_250_DS   = 0x03, //.0076 deg/sec/LSB
  RATE_125_DS   = 0x04  //.0038 deg/sec/LSB
} GyroRange_t;

//set range
void IMU_setGyroRange(IMU_t *imu, GyroRange_t range);

//set output data rate (implicitly sets LP filter bandwidth)
typedef enum {
    ODR_2000_BW_532 = 0x00,
    ODR_2000_BW_230 = 0x01,
    ODR_1000_BW_116 = 0x02,
    ODR_400_BW_47   = 0x03,
    ODR_200_BW_23   = 0x04,
    ODR_100_BW_12   = 0x05,
    ODR_200_BW_64   = 0x06,
    ODR_100_BW_32   = 0x07
} GyroOdrBw_t;

void IMU_setGyroOdrBW(IMU_t *imu, GyroOdrBw_t odrBw);

////////////// IMU OBJECT DATA AND FUNCTIONS ///////////////////////

// struct to register I2C callbacks set by application
typedef struct IMUcbk {
    
    uint8_t (*onI2Cwrite)(uint8_t devAddr, uint8_t *bus_data, uint16_t size);
    uint8_t (*onI2Cread)(uint8_t devAddr, uint8_t *bus_data, uint16_t size);

} IMUcbk_t;

typedef union rawData{
    uint8_t bytes[6];
    int16_t shorts[3];
}rawData_t;

typedef enum{
  ACCEL = 0,
  GYRO
}deviceType_t;

typedef enum{
  SLAVE_REG_ADDR = 0,
  SLAVE_REG_VAL
}addrType_t;

typedef struct IMU {
    /* Callbacks */
    IMUcbk_t i2cCallBacks;
    
    /* Sensor Data */
    Accel_t Acc;
    Gyro_t Gyro;
    float degC;

    /*Rx Tx buffer*/
    rawData_t _RxTxData;
    
    /* I2C params */
    uint16_t _msgLen;
    uint8_t _curI2CdevAddr;
} IMU_t;

/////////////////// IMU initialization Functions and Parameters////////////////////////////
typedef struct ImuInit{
  SD01State_t sd01;
}ImuInit_t;
typedef enum{
  PUSH_PULL, 
  OPEN_DRAIN
} IntPpOd_t;

typedef enum{
  ACT_HIGH, 
  ACT_LOW
} IntLvl_t;

typedef enum{
  INT1, 
  INT2
} AccIntPins_t;

typedef enum{
  INT3, 
  INT4
} GyroIntPins_t;

//Accelerometer initialization parameters 
typedef struct AccelInit{
  AccRange_t range;
  ACC_OSR_t osr; 
  ACC_ODR_t odr;
  AccelInt_t AccInts;
} AccelInit_t;

//intialize accelerometer
void _IMU_AccInit(IMU_t* imu, AccelInit_t* acc);

//configure accelerometer interrupts
typedef struct AccelInt{
  AccIntPins_t IntPin;
  bool pinIsInput;
  uint8_t PpOd;
  uint8_t activeLvl;
} AccelInt_t;

//configure accelerometer interrupts
void _IMU_AccIntConfig(IMU_t* imu, AccelInt_t* acc);

//gyroscope initialization parameters
typedef struct GyroInit{
  GyroRange_t range;
  GyroOdrBw_t OdrBw;
  AccelInt_t GyroInts;
} GyroInit_t;

//intialize gyroscope
void _IMU_GyroInit(IMU_t* imu, GyroInit_t* gyro);

//gyroscope interrupt initialization parameters
typedef struct GyroInt{
  GyroIntPins_t IntPin;
  bool pinIsInput;
  uint8_t PpOd;
  uint8_t activeLvl;
} GyroInt_t;

//configure gyroscope interrupts
void _IMU_GyroIntConfig(IMU_t* imu, GyroInt_t* acc);

/////////////////// IMU Private Functions ////////////////////

//convert raw data measurement to LSBs
void _processRawData(rawData_t *reading);

//set imu register
uint8_t _IMU_setReg(IMU_t *imu, deviceType_t dev, uint8_t regAddr, uint8_t regWriteVal);

//write IMU register
uint8_t _IMU_writeReg(IMU_t *imu, deviceType_t dev, uint8_t regAddr, uint8_t regWriteVal);

//Read IMU register
uint8_t _IMU_readReg(IMU_t *imu, deviceType_t dev, uint8_t regAddr, uint16_t msgLen);

//set device address for I2C
void _IMU_setDevAddr(IMU_t *imu, deviceType_t dev);

//set message length for I2C
void _IMU_setMsgLen(IMU_t* imu, uint16_t msgLen);

//set register address for I2C
void _IMU_setRegAddr(IMU_t *imu, uint8_t regAddr);

//set register write value for I2C
void _IMU_setRegVal(IMU_t *imu, uint8_t regWriteVal);

//invoke write callback
uint8_t _IMU_write(IMU_t *imu);

//invoke read callback
uint8_t _IMU_read(IMU_t *imu);

#endif