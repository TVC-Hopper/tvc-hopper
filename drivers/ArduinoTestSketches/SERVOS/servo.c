#include "servo.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

uint8_t Servo_Init(Servo_t* Servo, ServoInit_t* ServoInit){
    float width = 0;
    
    if (ServoInit->pulseWidth0deg_us < ServoInit->pulseWidth180deg_us) {return BAD_MIN_MAX_THRESH;}
    else {}
    //register cbk
    Servo->callbacks.onNewDutyCycle = ServoInit->onNewDutyCycle;
    //set pulsewidth min and max
    Servo->pulseWidthMin_us = ServoInit->pulseWidth0deg_us;
    Servo->pulseWidthMax_us = ServoInit->pulseWidth180deg_us;
    //set conversion between pulswidth and degrees
    //0 deg corresponds to max pulse width
    width = ServoInit->pulseWidth0deg_us - ServoInit->pulseWidth180deg_us;
    Servo->degToMicros = width / DEGREES_MAX;
    Servo->microsToDeg = 1 / Servo->degToMicros;
    Servo_WritePosDeg(Servo, ServoInit->posStart_deg);
    return 0;
}

int Servo_GetLastWriteDeg(Servo_t* Servo){return Servo->lastWrite_deg;}

uint32_t Servo_GetLastWriteMicros(Servo_t* Servo){return Servo->lastWrite_us;}

void Servo_WritePosDeg(Servo_t *Servo, int deg){
    uint32_t micros = 0;
    if (deg < DEGREES_MIN){ 
        micros = Servo->pulseWidthMax_us;
        deg = DEGREES_MIN;    
    }
    else if (deg > DEGREES_MAX){ 
        micros = Servo->pulseWidthMin_us;
        deg = DEGREES_MAX; 
    }
    else {micros = Servo->pulseWidthMin_us - Servo->degToMicros * deg;}
    Servo->lastWrite_deg = deg;
    Servo->lastWrite_us = micros;
    Servo->callbacks.onNewDutyCycle(micros);
}

void Servo_WritePosMicros(Servo_t *Servo, uint32_t micros){
    if (micros < Servo->pulseWidthMin_us){ 
        micros = Servo->pulseWidthMin_us;
        Servo->lastWrite_deg = DEGREES_MAX;    
    }
    else if (micros > Servo->pulseWidthMax_us){ 
        micros = Servo->pulseWidthMax_us;
        Servo->lastWrite_deg = DEGREES_MIN; 
    }
    else {
      Servo->lastWrite_deg = Servo->microsToDeg * micros;
    }
    Servo->lastWrite_us = micros;
    Servo->callbacks.onNewDutyCycle(micros);
}
