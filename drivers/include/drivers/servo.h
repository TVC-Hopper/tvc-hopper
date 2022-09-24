#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

typedef struct ServoCbk {
    
    void (*onPWMwrite)(uint16_t pulseWidth_us);

} ServoCbk_t;


typedef struct Servo {
    
    ServoCbk_t pwmCbk;

    uint16_t lastWrite_us;
    uint8_t lastWrite_deg;

    uint16_t pulseWidth_min;
    uint16_t pulseWidth_max;

} Servo_t;

void ServoInit(Servo_t* Servo, uint16_t pulseWidth0deg_us, uint16_t pulseWidth180deg_us);

uint16_t returnLastWriteDeg(Servo_t *Servo);

uint16_t returnLastWriteMicros(Servo_t *Servo);

uint16_t writePosDeg(Servo_t *Servo, uint8_t deg);

uint16_t writePosMicros(Servo_t *Servo, uint16_t micros);

#endif