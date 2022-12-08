#ifndef HW_H
#define HW_H

/**
 *  Set ldok PWM bit to move all buffered PWM
 *  values into registers. This is a synchronization
 *  mechanism for updating multiple PWM outputs
 *  simultaneously.
 */
extern void Hw_UpdatePwm();

#endif
