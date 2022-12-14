#ifndef SERVO_H
#define SERVO_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define BAD_MIN_MAX_THRESH 1
#define DEGREES_MIN 0
#define DEGREES_MAX 180

/**
 * @brief Servo Initialization parameters
 *  onNewDutyCycle : ptr to PWM callback
 *  pulseWidth0deg_us : pulsewidth in microseconds corresponding to 0 deg
 *  pulseWidth180deg_us : pulsewidth in microseconds corresponding to 180 deg
 *  deadband : half deadband width
 *  posStart_deg : servo start position in degrees
 *  applyDB : apply deadband compensation
 */
typedef struct ServoInit{
    void (*onNewDutyCycle)(int);
    uint32_t pulseWidth0deg_us;
    uint32_t pulseWidth180deg_us;
    int posStart_deg;
}ServoInit_t;

/**
 * @brief Servo callbacks
 * onNewDutyCycle : set pulsewidth in microseconds
 */
typedef struct ServoCallbacks {
    void (*onNewDutyCycle)(int pulseWidth_us);
} ServoCallbacks_t;

/**
 * @brief Servo driver object
 *  callbacks : callback to PWM write method
 *  lastWrite_us : last commanded position in micros
 *  lastWrite_deg : last commanded position in degrees
 *  pulseWidth0deg_us : pulsewidth in microseconds corresponding to 0 deg
 *  pulseWidth180deg_us : pulsewidth in microseconds corresponding to 180 deg
 *  degToMicros : conversion between degrees and micros
 *  microsToDeg : conversion between micros and degrees
 */

//rename servo struct to Servo_test to prevent conflict with arduino servo library
typedef struct Servo_test {
    ServoCallbacks_t callbacks;

    uint32_t lastWrite_us;
    int lastWrite_deg;

    uint32_t pulseWidthMin_us;
    uint32_t pulseWidthMax_us;

    float degToMicros;
    float microsToDeg;
} Servo_t;

/**
 * @brief Initializes Servo
 * 
 * @param Servo : ptr to object of type Servo_t
 * @param ServoInit : servo initialization structure
 * @return uint8_t : 0 -> no errors, 1 -> init failed
 */
uint8_t Servo_Init(Servo_t* Servo, ServoInit_t* ServoInit);

/**
 * @brief return current position in degrees
 * 
 * @param Servo ptr to object of type Servo_t
 * @return uint8_t : degrees between 0 and 180
 */
int Servo_GetLastWriteDeg(Servo_t* Servo);

/**
 * @brief return current position in micros
 * 
 * @param Servo ptr to object of type Servo_t
 * @return uint32_t 
 */
uint32_t Servo_GetLastWriteMicros(Servo_t* Servo);

/**
 * @brief write new position in degrees
 * 
 * @param Servo ptr to object of type Servo_t
 * @param deg position commanded in degrees
 */
void Servo_WritePosDeg(Servo_t *Servo, int deg);

/**
 * @brief write new position in micros
 * 
 * @param Servo ptr to object of type Servo_t
 * @param micros position commanded in microseconds
 */
void Servo_WritePosMicros(Servo_t *Servo, uint32_t micros);

/**
 * @brief compensate for servo deadband
 * 
 * @param Servo ptr to object of type Servo_t
 * @param micros : position commanded in microseconds
 * @return uint32_t : returns deadband compensated position in microseconds
 */
uint32_t applyDeadband(Servo_t* Servo, uint32_t micros);

/**
 * @brief write new position with deadband compensation
 * 
 * @param Servo ptr to object of type Servo_t
 * @param micros position commanded in microseconds
 */
void Servo_WriteMicros(Servo_t* Servo, uint32_t micros);


#endif
