#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>

typedef struct ServoCallbacks_s {
    
    void (*onNewDutyCycle)(uint16_t pulseWidth_us);

} ServoCallbacks_t;


typedef struct Servo_s {
    
    ServoCallbacks_t callbacks;

    uint16_t lastWrite_us;
    uint8_t lastWrite_deg;

    uint16_t pulseWidth_min;
    uint16_t pulseWidth_max;

} Servo_t;

void Servo_Init(Servo_t* Servo, uint16_t pulseWidth0deg_us, uint16_t pulseWidth180deg_us);

uint16_t Servo_GetLastWriteDeg(Servo_t *Servo);

uint16_t Servo_GetLastWriteMicros(Servo_t *Servo);

uint16_t Servo_WritePosDeg(Servo_t *Servo, uint8_t deg);

uint16_t Servo_WritePosMicros(Servo_t *Servo, uint16_t micros);

#endif
