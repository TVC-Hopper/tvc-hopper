#include "servo.h"
#include <stdint.h>

void ServoInit(Servo_t* Servo, uint16_t pulseWidth0deg_us, uint16_t pulseWidth180deg_us){return;}

uint16_t returnLastWriteDeg(Servo_t *Servo){return 0;}

uint16_t returnLastWriteMicros(Servo_t *Servo){return 0;}

uint16_t writePosDeg(Servo_t *Servo, uint8_t deg){return 0;}

uint16_t writePosMicros(Servo_t *Servo, uint16_t micros){return 0;}