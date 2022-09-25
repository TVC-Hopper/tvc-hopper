#include "servo.h"
#include <stdint.h>

void Servo_Init(Servo_t* Servo, uint16_t pulseWidth0deg_us, uint16_t pulseWidth180deg_us){return;}

uint16_t Servo_GetLastWriteDeg(Servo_t *Servo){return 0;}

uint16_t Servo_GetLastWriteMicros(Servo_t *Servo){return 0;}

uint16_t Servo_WritePosDeg(Servo_t *Servo, uint8_t deg){return 0;}

uint16_t Servo_WritePosMicros(Servo_t *Servo, uint16_t micros){return 0;}
