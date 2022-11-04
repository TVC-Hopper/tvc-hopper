extern "C"{
  #include "imu.h"
}
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "stdint.h"
#include <Wire.h>
#include <stdbool.h>


IMU_t imu;
bool gyroRead = false;
bool accRead = false;

SemaphoreHandle_t gyroSemaphore;
SemaphoreHandle_t accSemaphore;

SemaphoreHandle_t i2cMutex;

uint8_t writeI2c(uint8_t addr, uint8_t* bus_data, uint16_t size);
uint8_t readI2c(uint8_t addr, uint8_t* bus_data, uint16_t size);
void onDelay(uint32_t micros);

void gyroDrdyISR();
void accDrdyISR();
void readGyro(void* pvParameters);
void readAcc(void* pvParameters);
void printReadings(void* pvParameters);


void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  gyroSemaphore = xSemaphoreCreateBinary();
  accSemaphore = xSemaphoreCreateBinary();
  i2cMutex = xSemaphoreCreateMutex();

  IMUinit_t init;
  init.sd01 = SDO1_LOW;
  init.onDelay_us = &onDelay;
  init.onI2Cwrite = &writeI2c;
  init.onI2Cread = &readI2c;

  IntInit_t acc_int_init;
  acc_int_init.IntPin = INT_1;
  acc_int_init.pinIsInput = 0;
  acc_int_init.PpOd = PUSH_PULL;
  acc_int_init.activeLvl = ACT_LOW;
  
  AccelInit_t acc_init;
  acc_init.range = RANGE_3G;
  acc_init.osr = NONE;
  acc_init.odr = HZ_12_5;
  acc_init.AccInts = acc_int_init;

  IntInit_t gyro_int_init;
  gyro_int_init.IntPin = INT_3;
  gyro_int_init.pinIsInput = 0;
  gyro_int_init.PpOd = PUSH_PULL;
  gyro_int_init.activeLvl = ACT_LOW;

  GyroInit_t gyro_init;
  gyro_init.range = RATE_2000_DS;
  gyro_init.OdrBw = ODR_100_BW_12;
  gyro_init.GyroInts = gyro_int_init;



  pinMode(2, INPUT_PULLUP);
  pinMode(3, INPUT_PULLUP);

  Serial.println(IMU_init(&imu, &acc_init, &gyro_init, &init));

  attachInterrupt(digitalPinToInterrupt(2), gyroDrdyISR, RISING);
  attachInterrupt(digitalPinToInterrupt(3), accDrdyISR, RISING);

  xTaskCreate(
      readGyro
      ,  "readGyro"   
      ,  128  
      ,  NULL
      ,  2  
      ,  NULL );

  xTaskCreate(
      readAcc
      ,  "readAcc"   
      ,  128 
      ,  NULL
      ,  2 
      ,  NULL );

  xTaskCreate(
      printReadings
      ,  "printReadings"   
      ,  128 
      ,  NULL
      ,  2 
      ,  NULL );
}

void loop()
{
  // Empty. Things are done in Tasks.
}

void gyroDrdyISR(){
  xSemaphoreGiveFromISR( gyroSemaphore, NULL);
}
void accDrdyISR(){
  xSemaphoreGiveFromISR( accSemaphore, NULL);
}

void readGyro(void* pvParameters){
 const TickType_t xDelay = pdMS_TO_TICKS(100);
  for( ;; )
  {
    xSemaphoreTake(gyroSemaphore, xDelay);
    xSemaphoreTake(i2cMutex, xDelay);
    IMU_getGyroReading(&imu);
    xSemaphoreGive(i2cMutex);
  }
}

void readAcc(void* pvParameters){
  //TickType_t xLastWakeTime;
  const TickType_t xDelay = pdMS_TO_TICKS(100);
  //xLastWakeTime = xTaskGetTickCount();
  for( ;; )
  {
    xSemaphoreTake(accSemaphore, xDelay);
    xSemaphoreTake(i2cMutex, xDelay);
    IMU_getAccReading(&imu);
    xSemaphoreGive(i2cMutex);
  }
}

void printReadings(void* pvParameters){
  TickType_t xLastWakeTime;
  const TickType_t xDelay = pdMS_TO_TICKS(1000);
  xLastWakeTime = xTaskGetTickCount();
  for( ;; )
  {
    vTaskDelayUntil( &xLastWakeTime, xDelay);
    xSemaphoreTake(i2cMutex, 0);
    Serial.println("GYRO:");
    Serial.print("x: ");
    Serial.println(imu.Gyro.xComp);
    Serial.print("y: ");
    Serial.println(imu.Gyro.yComp);
    Serial.print("z: ");
    Serial.println(imu.Gyro.zComp);
    Serial.println("ACC:");
    Serial.print("x: ");
    Serial.println(imu.Acc.xComp);
    Serial.print("y: ");
    Serial.println(imu.Acc.yComp);
    Serial.print("z: ");
    Serial.println(imu.Acc.zComp);
    xSemaphoreGive(i2cMutex);
  }
}

void onDelay(uint32_t micros){
  delayMicroseconds(micros);
}

uint8_t writeI2c(uint8_t addr, uint8_t* bus_data, uint16_t size){
  Wire.beginTransmission(addr);
  Wire.write(bus_data, size);
  return Wire.endTransmission();
}

uint8_t readI2c(uint8_t addr, uint8_t* bus_data, uint16_t size){
  Wire.requestFrom(addr, size);
  while(Wire.available() < size){}
  for (int i = 0; i < size; ++i){
    bus_data[i] = Wire.read();
  }
}