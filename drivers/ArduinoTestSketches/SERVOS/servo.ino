extern "C"{
  #include "servo.h"
}
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include "stdint.h"
#include <Wire.h>
#include <stdbool.h>
#include <Servo.h>

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

Servo_t S1;
Servo_t S2;
Servo_t S3;
Servo_t S4;

ServoInit_t servoInit;
void onNewDuty1(int micros);
void onNewDuty2(int micros);
void onNewDuty3(int micros);
void onNewDuty4(int micros);
void testServo();
void testServo1();

void setup() {
  // put your setup code here, to run once:
  // Serial.begin(9600);
  servo1.attach(9); 
  servo2.attach(10);
  servo3.attach(11);
  servo4.attach(6);

  servoInit.posStart_deg = 90;
  servoInit.pulseWidth0deg_us = 2000;
  servoInit.pulseWidth180deg_us = 1000;
  
  servoInit.onNewDutyCycle = &onNewDuty1;
  Servo_Init(&S1, &servoInit);
  
  servoInit.onNewDutyCycle = &onNewDuty2;
  Servo_Init(&S2, &servoInit);
  
  servoInit.onNewDutyCycle = &onNewDuty3;
  Servo_Init(&S3, &servoInit);
  
  servoInit.onNewDutyCycle = &onNewDuty4;
  Servo_Init(&S4, &servoInit);
  
  // xTaskCreate(
  //   testServo
  //   ,  "testServo"   
  //   ,  256  
  //   ,  NULL
  //   ,  2  
  //   ,  NULL );
  
  xTaskCreate(
    testServo1
    ,  "testServo1"   
    ,  256  
    ,  NULL
    ,  2  
    ,  NULL );
}

void loop() {
  
}
void onNewDuty1(int micros){
  servo1.writeMicroseconds(micros);
}
void onNewDuty2(int micros){
  servo2.writeMicroseconds(micros);
}
void onNewDuty3(int micros){
  servo3.writeMicroseconds(micros);
}
void onNewDuty4(int micros){
  servo4.writeMicroseconds(micros);
}

void testServo(){
  TickType_t xLastWakeTime;
  const TickType_t xDelay = pdMS_TO_TICKS(1000);
  xLastWakeTime = xTaskGetTickCount();
  uint32_t micros = 1000;
  for( ;; )
  {
    vTaskDelayUntil( &xLastWakeTime, xDelay);
    if (micros > 2000){micros = 1000;}
    Servo_WritePosMicros(&S1, micros);
    Servo_WritePosMicros(&S2, micros);
    Servo_WritePosMicros(&S3, micros);
    Servo_WritePosMicros(&S4, micros);
    micros += 500;
  }
}

void testServo1(){
  TickType_t xLastWakeTime;
  const TickType_t xDelay = pdMS_TO_TICKS(1000);
  xLastWakeTime = xTaskGetTickCount();
  int deg = 0;
  for( ;; )
  {
    vTaskDelayUntil( &xLastWakeTime, xDelay);
    if (deg > 180){deg = 0;}
    Servo_WritePosDeg(&S1, deg);
    Servo_WritePosDeg(&S2, deg);
    Servo_WritePosDeg(&S3, deg);
    Servo_WritePosDeg(&S4, deg);
    deg += 90;
  }
}