#include "servo.h"
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

uint8_t Servo_Init(Servo_t* Servo, ServoInit_t* ServoInit){
    if (ServoInit->pulseWidth0deg_us < ServoInit->pulseWidth180deg_us) {return BAD_MIN_MAX_THRESH;}
    Servo->callbacks.onNewDutyCycle = ServoInit->onNewDutyCycle;
    float width = ServoInit->pulseWidth0deg_us - ServoInit->pulseWidth180deg_us;
    Servo->pulseWidthMin_us = ServoInit->pulseWidth0deg_us;
    Servo->pulseWidthMax_us = ServoInit->pulseWidth180deg_us;
    Servo->degToMicros = width / DEGREES_MAX;
    Servo->microsToDeg = 1 / Servo->degToMicros;
    if (ServoInit->applyDB){Servo->writeMicros = &Servo_WriteMicrosDB;}
    else {Servo->writeMicros = &Servo_WriteMicrosNoDB;}
    Servo_WritePosDeg(Servo, ServoInit->posStart_deg);

}

uint8_t Servo_GetLastWriteDeg(Servo_t* Servo){return Servo->lastWrite_deg;}

uint32_t Servo_GetLastWriteMicros(Servo_t* Servo){return Servo->lastWrite_us;}

uint32_t Servo_applyDeadband(Servo_t* Servo, uint32_t micros){
    if (abs((int32_t)micros - (int32_t)Servo->lastWrite_us) < Servo->deadBand_us) {}
    else if (micros > Servo->lastWrite_us && 
             micros < (Servo->pulseWidthMax_us - Servo->deadBand_us)){
                micros += Servo->deadBand_us;
    }
    else if (micros < Servo->lastWrite_us && 
             micros > (Servo->pulseWidthMin_us + Servo->deadBand_us)){
                micros -= Servo->deadBand_us;
             }
    else {}
    return micros;
}

void Servo_WritePosDeg(Servo_t *Servo, uint8_t deg){
    uint32_t micros = 0;
    if (deg < DEGREES_MIN){ 
        micros = Servo->pulseWidthMin_us;
        deg = DEGREES_MIN;    
    }
    else if (deg > DEGREES_MAX){ 
        micros = Servo->pulseWidthMax_us;
        deg = DEGREES_MAX; 
    }
    else {micros = Servo->pulseWidthMin_us + Servo->degToMicros * deg;}
    Servo->lastWrite_deg = deg;
    Servo->lastWrite_us = micros;
    Servo->writeMicros(Servo, micros);
}

void Servo_WritePosMicros(Servo_t *Servo, uint32_t micros){
    if (micros < Servo->pulseWidthMin_us){ 
        micros = Servo->pulseWidthMin_us;
        Servo->lastWrite_deg = DEGREES_MIN;    
    }
    else if (micros > Servo->pulseWidthMax_us){ 
        micros = Servo->pulseWidthMax_us;
        Servo->lastWrite_deg = DEGREES_MAX; 
    }
    else {micros = Servo->microsToDeg * micros;}
    Servo->lastWrite_deg = micros * Servo->microsToDeg;
    Servo->lastWrite_us = micros;
    Servo->writeMicros(Servo, micros);
}


void Servo_WriteMicrosDB(Servo_t* Servo, uint32_t micros){
    micros = applyDeadband(Servo, micros);
    Servo->callbacks.onNewDutyCycle(micros);
}

void Servo_WriteMicrosNoDB(Servo_t* Servo, uint32_t micros){
    Servo->callbacks.onNewDutyCycle(micros);
}