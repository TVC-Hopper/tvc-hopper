#include "esc.h"
#include <stdint.h>
#include <string.h>

void Esc_init(Esc_t* Esc, uint8_t motorStartDutyCycle, void (*onNewDutyCycle)(uint32_t), uint32_t pwmPeriod){
  Esc->callbacks.onNewDutyCycle = onNewDutyCycle;
  Esc->currentDutyCycle_us = 0;
  Esc->pwmPeriod_us = pwmPeriod;
  Esc->motorStartDutyCycle = motorStartDutyCycle;
  Esc->curLUTidx = 0; 
}

void Esc_writeDutyCycle(Esc_t* Esc, float dutyCycle){
  if (dutyCycle > 1){dutyCycle = 1;}
  else if (dutyCycle < 0) {dutyCycle = 0;}
  Esc->currentDutyCycle_us = dutyCycle * Esc->pwmPeriod_us;
  Esc->callbacks.onNewDutyCycle(Esc->currentDutyCycle_us);
}

void Esc_writeDutyCycle_us(Esc_t* Esc, uint32_t dutyCycle_us){
  if (dutyCycle_us < Esc->pwmPeriod_us && dutyCycle_us > 0){
    Esc->callbacks.onNewDutyCycle(dutyCycle_us);
    Esc->currentDutyCycle_us = dutyCycle_us;
  }
}

uint8_t Esc_writeLut(Esc_t* Esc, uint32_t* arr, uint16_t size){
  uint8_t ret = 1;
  if (size == SIZE_LUT){
    memcpy(Esc->linearizationLUT, arr, SIZE_LUT);
    ret = 0;
  }
  return ret;
}

uint32_t Esc_returnCurrentDutyCycle_us(Esc_t* Esc){
  return Esc->currentDutyCycle_us;
}

void Esc_startMotor(Esc_t* Esc){
  Esc_writeDutyCycle(Esc, Esc->motorStartDutyCycle);
}

void Esc_cutMotor(Esc_t* Esc){
  Esc_writeDutyCycle(Esc, 0);
  Esc->currentDutyCycle_us = 0;
}

uint16_t Esc_incLutIdx(Esc_t* Esc){
  if(Esc->curLUTidx <  SIZE_LUT - 1){++Esc->curLUTidx;}
  return Esc->curLUTidx;
}

uint16_t Esc_decLutIdx(Esc_t* Esc){
  if(Esc->curLUTidx >  0){--Esc->curLUTidx;}
  return Esc->curLUTidx;
}

uint16_t  Esc_setLutIdx(Esc_t* Esc, uint16_t idx){
  if (idx >= 0 && idx < SIZE_LUT){Esc->curLUTidx = idx;}
  return Esc->curLUTidx;
}

void Esc_setCurLutEntryToCurDC(Esc_t* Esc){ 
  Esc->linearizationLUT[Esc->curLUTidx] = Esc->currentDutyCycle_us;
}

void Esc_resetLutIdx(Esc_t* Esc){Esc->curLUTidx = 0;}

uint32_t Esc_getCurLutEntry(Esc_t* Esc){
  return Esc->linearizationLUT[Esc->curLUTidx];
}

uint32_t Esc_incSpeedLut(Esc_t* Esc, uint16_t step){
  if (Esc->curLUTidx + step < SIZE_LUT){
    Esc->curLUTidx += step;
  }
  else {Esc->curLUTidx = SIZE_LUT - 1;}
  Esc_writeDutyCycle_us(Esc, Esc->linearizationLUT[Esc->curLUTidx]);
  return Esc->linearizationLUT[Esc->curLUTidx];
}

uint32_t Esc_decSpeedLut(Esc_t* Esc, uint16_t step){
  if (Esc->curLUTidx >= step){
    Esc->curLUTidx -= step;
  }
  else {Esc->curLUTidx = 0;}
  Esc_writeDutyCycle_us(Esc, Esc->linearizationLUT[Esc->curLUTidx]);
  return Esc->linearizationLUT[Esc->curLUTidx];
}