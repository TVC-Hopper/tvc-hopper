#ifndef ESC_H
#define ESC_H

#include <stdint.h>

#define SIZE_LUT 1024

/**
 * @brief ESC callbacks
 * onNewDutyCycle : set pulsewidth in microseconds
 */
typedef struct EscCallbacks {

  void (*onNewDutyCycle)(uint32_t pulseWidth_us);

} EscCallbacks_t;

/**
 * @brief Esc driver object
 * callbacks : callback to pwm write method (sets duty cycle in microseconds)
 * currentDutyCycle_us : current duty cycle of motor controller
 * pwmPeriod_us : period of PWM signal
 * linearizationLUT[SIZE_LUT] : look up table in case motors throttle response is not linear
 * motorStartDutyCycle : duty cycle required to start motor
 * curLUTidx : current idx in Lut
 */
typedef struct Esc{
  
  EscCallbacks_t callbacks;
  uint32_t currentDutyCycle_us;
  uint32_t pwmPeriod_us;
  uint32_t linearizationLUT[SIZE_LUT];
  float motorStartDutyCycle;
  uint16_t curLUTidx;

}Esc_t;

/**
 * @brief method to initialize ESC driver
 * 
 * @param Esc ptr to Esc_t object
 * @param motorStartDutyCycle Duty cycle required to start motor
 * @param onNewDutyCycle Pwm callback to be registered with driver
 * @param pwmPeriod period of pwm callback signal
 */
void Esc_init(Esc_t* Esc, uint8_t motorStartDutyCycle, void (*onNewDutyCycle)(uint32_t), uint32_t pwmPeriod);

/**
 * @brief write duty cycle of motor
 * 
 * @param Esc ptr to Esc_t object
 * @param dutyCycle duty cycle in percent must be between 0 and 1.0
 */
void Esc_writeDutyCycle(Esc_t* Esc, float dutyCycle);

/**
 * @brief write duty cycle in us
 * 
 * @param Esc ptr to Esc_t object
 * @param dutyCycle_us width of high pulse in us, must be less than PWM period
 */
void Esc_writeDutyCycle_us(Esc_t* Esc, uint32_t dutyCycle_us);

/**
 * @brief populate Lut with 
 * 
 * @param Esc ptr to Esc_t object
 * @param arr array containg desired lut valuse in micros
 * @param size must be equal to lut size
 * @return 0 on success, 1 on failure
 */
uint8_t Esc_writeLut(Esc_t* Esc, uint32_t* arr, uint16_t size);

/**
 * @brief return current Duty Cycle of motor controller in micros
 * 
 * @param Esc ptr to Esc_t object
 * @return value of Duty cycle in micros
 */
uint32_t Esc_returnCurrentDutyCycle_us(Esc_t* Esc);

/**
 * @brief sets duty cycle to value that causes motor to start
 * 
 * @param Esc ptr to Esc_t object
 */
void Esc_startMotor(Esc_t* Esc);

/**
 * @brief sets duty cycle to 0
 * 
 * @param Esc ptr to Esc_t object
 */
void Esc_cutMotor(Esc_t* Esc);

/**
 * @brief increase current lut idx by one
 * 
 * @param Esc ptr to Esc_t object
 * @return current lut idx
 */
uint16_t Esc_incLutIdx(Esc_t* Esc);

/**
 * @brief decrease current lut idx by one
 * 
 * @param Esc ptr to Esc_t object
 * @return current lut idx
 */
uint16_t Esc_decLutIdx(Esc_t* Esc);

/**
 * @brief set cur lut idx to idx
 * 
 * @param Esc ptr to Esc_t object
 * @param idx must be less than SIZELUT
 * @return current lut idx
 */
uint16_t  Esc_setLutIdx(Esc_t* Esc, uint16_t idx);

/**
 * @brief sets centry at cur lut idx to current Duty Cycle
 * 
 * @param Esc ptr to Esc_t object
 */
void Esc_setCurLutEntryToCurDC(Esc_t* Esc);

/**
 * @brief sets cur lut idx to 0
 * 
 * @param Esc ptr to Esc_t object
 */
void Esc_resetLutIdx(Esc_t* Esc);

/**
 * @brief returns duty cycle at current lut entry in micros
 * 
 * @param Esc ptr to Esc_t object
 * @return duty cycle at cur lut idx in micros
 */
uint32_t Esc_getCurLutEntry(Esc_t* Esc);

/**
 * @brief increments cur lut idx by step and write DutyCycle entry at that idx
 * 
 * @param Esc ptr to Esc_t object
 * @param step num lut entries by which to increase
 * @return duty cycle at cur lut idx in micros
 */
uint32_t Esc_incSpeedLut(Esc_t* Esc, uint16_t step);

/**
 * @brief decrements cur lut idx by step and writes duty cycle at that idx
 * 
 * @param Esc ptr to Esc_t object
 * @param step num lut entries by which to decrease
 * @return duty cycle at cur lut idx in micros
 */
uint32_t Esc_decSpeedLut(Esc_t* Esc, uint16_t step);

#endif