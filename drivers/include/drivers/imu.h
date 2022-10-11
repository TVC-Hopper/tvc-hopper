#ifndef IMU_H
#define IMU_H

#include <stdint.h>
#include <stdbool.h>

/**************************************************************/
/* accelerometer and gyroscope interrupt configuration params */
/**************************************************************/

/**
 * @brief imu interrupt pin configuration parameters
 *        set pin as push-pull or open drain
 */
typedef enum{
  PUSH_PULL, 
  OPEN_DRAIN
} IntPpOd_t;

/**
 * @brief imu interrupt pin configuration parameters
 *        configure pin as active high or active low
 */
typedef enum{
  ACT_HIGH, 
  ACT_LOW
} IntLvl_t;

/**
 * @brief accelerometer interrupt pin select
 */
typedef enum{
  INT_1 = 0x04, 
  INT_2 = 0x40
} AccIntPins_t;

/**
 * @brief gyroscope interrupt pin select
 */
typedef enum{
  INT_3 = 0x01, 
  INT_4 = 0x80
} GyroIntPins_t;

/**
 * @brief interrupt pin initialization parameters
 * IntPin : desired interrupt pin
 * pin : set pin as input
 * PpOd : set pin as push pull or open drain
 * activeLvl : set active logic level of pin
 */
typedef struct IntInit{
  uint8_t IntPin;
  bool pinIsInput;
  uint8_t PpOd;
  uint8_t activeLvl;
} IntInit_t;
/*********************************************************/
/* accelerometer device object and associated parameters */
/*********************************************************/

/**
 * @brief Structure containing accelerometer data
 * LSB : conversion factor from LSB to Gs
 * xComp : x axis component of acceleration
 * yComp : y axis component of acceleration
 * zComp : z axis component of acceleration
 * devAddr : i2c device address of accelerometer 
 */
typedef struct Accelerometer {
    float LSB;
    float xComp; 
    float yComp; 
    float zComp;
    uint8_t devAddr; 
} Accel_t;

/**
 * @brief Accelerometer data and control registers
 */
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

#define MASK_ALL 0xFF
#define INT_MAP_DATA_MASK 0b01110111
#define INT_IO_CTRL_MASK  0b00011110
#define ACC_RANGE_MASK    0b00000011
#define ACC_ERR_REG_MASK  0b00000101


/**
 * @brief allowable accelerometer measurement range params
 */
typedef enum {
  RANGE_3G  = 0x00, //.0000916 g/LSB
  RANGE_6G  = 0x01, //.000183  g/LSB
  RANGE_12G = 0x02, //.000366  g/LSB
  RANGE_24G = 0x03  //.000632  g/LSB
} AccRange_t;

/**
 * @brief allowable oversampling ratio params
 */
typedef enum {
  OSR4 = 0x08, //4x Nyquist Rate
  OSR2 = 0x09, //2x Nyquist Rate
  NONE = 0x0A  //1x Nyquist Rate
} ACC_OSR_t; 

/**
 * @brief allowable output data rate settings (Hz)
 */
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

/**
 * @brief initialization parameters for accelerometer
 * range : desired measurement range
 * osr : desired oversampling ratio
 * odr : desired output data rate
 * AccInts : structure containing interrupt configuration params for accelerometer
 */
typedef struct AccelInit{
  AccRange_t range;
  ACC_OSR_t osr; 
  ACC_ODR_t odr;
  IntInit_t AccInts;
} AccelInit_t;

/*****************************************************/
/* gyroscope device object and associated parameters */
/*****************************************************/

/**
 * @brief Structure containing gyroscope data
 * LSB : conversion factor from LSB angular velocity
 * xComp : x axis component of angular velocity
 * yComp : y axis component of angular velocity
 * zComp : z axis component of angular velocity
 * devAddr : i2c device address of gyroscope 
 */
typedef struct Gyroscope { 
    float LSB;
    float xComp; 
    float yComp; 
    float zComp;
    uint8_t devAddr;
} Gyro_t;

/**
 * @brief allowable gyroscope measurement range params
 */
typedef enum {
  RATE_2000_DS  = 0x00, //.061  deg/sec/LSB
  RATE_1000_DS  = 0x01, //.0305 deg/sec/LSB
  RATE_500_DS   = 0x02, //.0153 deg/sec/LSB
  RATE_250_DS   = 0x03, //.0076 deg/sec/LSB
  RATE_125_DS   = 0x04  //.0038 deg/sec/LSB
} GyroRange_t;

/**
 * @brief allowable gyroscope output data rate params
 *        with associated LP filter bandwidth
 */
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

/**
 * @brief initialization parameters for gyroscope
 * range : desired measurement range
 * odr : desired output data rate and LP filter bandwidth
 * GyroInts : structure containing interrupt configuration params for gyroscope
 */
typedef struct GyroInit{
  GyroRange_t range;
  GyroOdrBw_t OdrBw;
  IntInit_t GyroInts;
} GyroInit_t;

/**
 * @brief Gyroscope data and control registers
 */
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
    INT3_INT4_IO_MAP  = 0x18,
    FIFO_WM_EN        = 0x1E,
    FIFO_EXT_INT_S    = 0x34,
    GYRO_SELF_TEST    = 0x3C,
    FIFO_CONFIG_0_G   = 0x3D,
    FIFO_CONFIG_1_G   = 0x3E,
    FIFO_DATA_G       = 0x3F
} GyroReg_t;

#define GYRO_INT_IO_CONF_MASK  0b00001111
#define GYRO_INT_IO_MAP_MASK   0b10100101
#define GYRO_INT_CTRL_MASK     0b11000000
#define GYRO_TEST_READY_MASK   0b00000010
#define GYRO_TEST_STATUS_MASK  0b00010000



/*************************************/
/* IMU driver object data parameters */
/*************************************/

/**
 * @brief struct containing i2c callbacks set by application
 * onDelay_us : delay callback
 * onI2Cwrite : i2c write callback
 * onI2Cread : i2c read callback
 */
typedef struct IMUcbk {
    void (*onDelay_us)(uint32_t micros);
    uint8_t (*onI2Cwrite)(uint8_t devAddr, uint8_t *bus_data, uint16_t size);
    uint8_t (*onI2Cread)(uint8_t devAddr, uint8_t *bus_data, uint16_t size);
} IMUcbk_t;

/**
 * @brief send and receive i2c buffer 
 * bytes : i2c data in bytes
 * shorts : i2c data in shorts
 */
typedef union rawData{
    uint8_t bytes[6];
    int16_t shorts[3];
}rawData_t;

/**
 * @brief parameter indicating target i2c device on IMU
 * ACCEL : accelerometer
 * GYRO : gyroscope
 */
typedef enum{
  ACCEL = 0,
  GYRO
}deviceType_t;


/**
 * @brief i2c buffer indexes
 * SLAVE_REG_ADDRESS : i2c slave device register address idx
 * SLAVE_REG_VAL : i2c slave device register value idx
 */
typedef enum{
  SLAVE_REG_ADDR = 0,
  SLAVE_REG_VAL
}addrType_t;

/**
 * @brief SD01 pin state (determines device addresses)
 */
typedef enum {
    SDO1_LOW  = 0,
    SDO1_HIGH
} SD01State_t;

/**
 * @brief structure to initialize i2c addresses and callbacks
 * sd01 : sd01 pin state
 * onDelay_us : ptr to delay callback
 * onI2Cwrite : ptr to i2c write callback
 * onI2Cread : ptr to i2c read callback
 */
typedef struct IMUinit{
  SD01State_t sd01;
  void (*onDelay_us)(uint32_t micros);
  uint8_t (*onI2Cwrite)(uint8_t devAddr, uint8_t *bus_data, uint16_t size);
  uint8_t (*onI2Cread)(uint8_t devAddr, uint8_t *bus_data, uint16_t size);
}IMUinit_t;

/**
 * @brief IMU driver object
 * i2cCallBacks : i2c callbacks set by user
 * Accel_t Acc : accelerometer object with accelerometer data
 * Gyro_t Gyro : gyroscope object with gyroscope data
 * float degC : temperature recorded by device in deg C
 * _RxTxData : i2c data buffer
 * _msgLen : i2c message length
 * _curI2CdevAddr : currently targeted IMU device
 */
typedef struct IMU {
    IMUcbk_t Callbacks;
    Accel_t Acc;
    Gyro_t Gyro;
    float degC;

    rawData_t _RxTxData;
    uint16_t _msgLen;
    uint8_t _curI2CdevAddr;
} IMU_t;


/************************************/
/* accelerometer specific functions */
/************************************/

/**
 * @brief turn on accelerometer, necessary after PORST, or soft reset
 * @param imu ptr to object of type IMU_t
 */
void IMU_powerUpAcc(IMU_t *imu);

/**
 * @brief reset accelerometer, defaults all registers
 * @param imu ptr to object of type IMU_t
 */
void IMU_softResetAcc(IMU_t* imu);

/**
 * @brief returns error status of accelerometer
 *        bit2 == 1: configuration error in ACC_CONF reg
 *        bit0 == 1: fatal error, chip not in operational state
 * @param imu 
 * @return uint8_t Error status of Accelerometer     
 */
uint8_t IMU_readErrorStatusAcc(IMU_t* imu);

/**
 * @brief 3 axis accelerometer read
 *        updates xComp, yComp, zComp of Accel_t
 * @param imu ptr to object of type IMU_t
 */
void IMU_getAccReading(IMU_t *imu);

/**
 * @brief return temperature sensor reading in deg C
 * (temp registers are in accelerometer)
 * @param imu ptr to object of type IMU_t
 */
void IMU_getTempReading(IMU_t* imu);

/**
 * @brief function that returns contents of accelerometer register
 * @param imu ptr to object of type IMU_t
 * @param regAddr address of accelerometer device register
 * @return contents of register
 */
uint8_t IMU_getAccReg(IMU_t* imu, AccReg_t regAddr);

/**
 * @brief set output data rate and oversampling ratio 
 * (ODR and OSR settings implicitly sets LP filter bandwidth)
 * @param imu ptr to object of type IMU_t
 * @param osr oversampling ratio
 * @param odr output data rate
 */
void IMU_setAccelOsrOdr(IMU_t *imu, ACC_OSR_t osr, ACC_ODR_t odr);

/**
 * @brief function to set desired measurement range of accelerometer
 * @param imu ptr to object of type IMU_t
 * @param range desired range
 */
void IMU_setAccelRange(IMU_t *imu, AccRange_t range);

/**
 * @brief convert measurement from LSBs to Gs
 * @param reading sensor reading in LSBs
 * @param LSB conversion factor from LSBs to Gs
 * @return acceleration in Gs
 */
float _convertToGs(int16_t reading, float LSB);

/**
 * @brief function that sets Accelerometer device register
 * @param imu ptr to object of type IMU_t
 * @param regAddr address of accelerometer device register to set
 * @param regWriteVal register value to set
 */
void _IMU_setAccReg(IMU_t* imu, AccReg_t regAddr, uint8_t regWriteVal, uint8_t mask);

/**
 * @brief configure accelerometer interrupts 
 * @param imu ptr to object of type IMU_t 
 * @param accInts accelerometer interrupt configuration params
 */
void _IMU_AccIntConfig(IMU_t* imu, IntInit_t* accInts);

/**
 * @brief initialize accelerometer device
 * @param imu ptr to object of type IMU_t 
 * @param acc structure of type AccelInit_t containing accelerometer 
 *        initialization parameters
 */
void _IMU_AccInit(IMU_t* imu, AccelInit_t* accInit);

/********************************/
/* gyroscope specific functions */
/********************************/

/**
 * @brief reset all registers in gyro, must wait 30ms before
 *        writing or reading to device
 * @param imu ptr to object of type IMU_t
 */
void IMU_softResetGyro(IMU_t* imu);

/**
 * @brief return error status of gyroscope
 *        bit2: sensor values may not be in expected range
 *        bit4: sensor not working properly
 * @param imu 
 * @return uint8_t 
 */
uint8_t IMU_readErrorStatusGyro(IMU_t* imu);

/**
 * @brief 3 axis gyroscope read
 *        updates xComp, yComp, zComp of Gyro_t
 * @param imu ptr to object of type IMU_t
 */
void IMU_getGyroReading(IMU_t *imu);

/**
 * @brief function that returns contents of gyroscope register
 * @param imu ptr to object of type IMU_t
 * @param regAddr address of gyroscope device register
 * @return contents of register
 */
uint8_t IMU_getGyroReg(IMU_t* imu, GyroReg_t regAddr);

/**
 * @brief set output data rate 
 * (ODR setting implicitly sets LP filter bandwidth)
 * @param imu ptr to object of type IMU_t
 * @param odrBw output data rate
 */
void IMU_setGyroOdrBW(IMU_t *imu, GyroOdrBw_t odrBw);

/**
 * @brief function to set desired measurement range of gyroscope
 * @param imu ptr to object of type IMU_t
 * @param range desired range
 */
void IMU_setGyroRange(IMU_t *imu, GyroRange_t range);

/**
 * @brief convert measurement from LSBs to deg/s
 * @param reading sensor reading in LSBs
 * @param LSB conversion factor from LSBs to deg/s
 * @return angular velocity in deg/sec
 */
float _convertToDegPerSec(uint16_t reading, float LSB);

/**
 * @brief function that sets Gyroscope device register
 * @param imu ptr to object of type IMU_t
 * @param regAddr address of gyroscope device register to set
 * @param regWriteVal register value to set
 */
void _IMU_setGyroReg(IMU_t *imu, GyroReg_t regAddr, uint8_t regWriteVal, uint8_t mask);

/**
 * @brief configure accelerometer interrupts 
 * @param imu ptr to object of type IMU_t 
 * @param gyroInts gyroscope interrupt configuration params
 */
void _IMU_GyroIntConfig(IMU_t* imu, IntInit_t* gyroInts);

/**
 * @brief initialize gyroscope device
 * @param imu ptr to object of type IMU_t 
 * @param gyro structure of type GyroInit_t containing gyroscope 
 *        initialization parameters
 */
void _IMU_GyroInit(IMU_t* imu, GyroInit_t* gyroInit);

/****************************************/
/* IMU driver object specific functions */
/****************************************/

/**
 * @brief initialize IMU driver object 
 * @param imu ptr to object of type IMU_t
 * @param Acc structure containing accelerometer initialization params
 * @param gyro structure containing gyroscope initialization params
 * @param Imuinit structure containing callbacks and sd01 pin state
 * @return error code, 0 if success, see gyro and acc error funcs for non zero bit meanings
 */
uint8_t IMU_init(IMU_t* imu, AccelInit_t* Acc, GyroInit_t* gyro, IMUinit_t* imuInit);


/**
 * @brief set imu register
 * @param imu ptr to object of type IMU_t 
 * @param regAddr device register address
 * @param regWriteVal value to set
 * @param mask non-functional bit mask
 * @return 0 on success 1 on failure
 */
uint8_t _IMU_setReg(IMU_t *imu, uint8_t regAddr, uint8_t regWriteVal, uint8_t mask);

/**
 * @brief write IMU register
 * @param imu ptr to object of type IMU_t 
 * @param regAddr device register address
 * @param regWriteVal value to write to reg
 * @return 0 on success 1 on failure
 */
uint8_t _IMU_writeReg(IMU_t *imu, uint8_t regAddr, uint8_t regWriteVal);

/**
 * @brief Read IMU register
 * @param imu ptr to object of type IMU_t 
 * @param regAddr device register address
 * @param msgLen length of message to write
 * @return 0 on success 1 on failure
 */
uint8_t _IMU_readReg(IMU_t *imu, uint8_t regAddr, uint16_t msgLen);

/**
 * @brief set device address for i2c
 * @param imu ptr to object of type IMU_t 
 * @param dev target device address
 */
void _IMU_setDevAddr(IMU_t *imu, deviceType_t dev);

/**
 * @brief set message length for i2c
 * @param imu ptr to object of type IMU_t 
 * @param msgLen i2c message length
 */
void _IMU_setMsgLen(IMU_t* imu, uint16_t msgLen);

/**
 * @brief set register address in i2c buffer
 * @param imu ptr to object of type IMU_t 
 * @param regAddr device register address
 */
void _IMU_setRegAddr(IMU_t *imu, uint8_t regAddr);

/**
 * @brief set register write value in i2c buffer
 * @param imu ptr to object of type IMU_t 
 * @param regWriteVal value to write
 */
void _IMU_setRegVal(IMU_t *imu, uint8_t regWriteVal);

/**
 * @brief invoke i2c write callback
 * @param imu ptr to object of type IMU_t 
 * @return 0 on success 1 on failure
 */
uint8_t _IMU_write(IMU_t *imu);

/**
 * @brief invoke i2c read callback
 * @param imu ptr to object of type IMU_t 
 * @return 0 on success 1 on failure 
 */
uint8_t _IMU_read(IMU_t *imu);

#endif
