extern "C"{
  #include "imu.h"
}
#include "stdint.h"
#include <Wire.h>

IMU_t imu;
int regVal;

uint8_t writeI2c(uint8_t addr, uint8_t* bus_data, uint16_t size);
uint8_t readI2c(uint8_t addr, uint8_t* bus_data, uint16_t size);
void onDelay(uint32_t micros);


void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  IMUinit_t init;
  init.sd01 = SDO1_LOW;
  init.onDelay_us = &onDelay;
  init.onI2Cwrite = &writeI2c;
  init.onI2Cread = &readI2c;
  IMU_init(&imu, NULL, NULL, &init);

}

void loop() {
  delay(1000);
  regVal = IMU_getAccReg(&imu, ACC_CHIP_ID);
  Serial.println(regVal);
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