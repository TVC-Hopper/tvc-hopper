#include "hw.h"

#include <fslhal/fsl_pwm.h>
#include <bsp/peripherals.h>


static const pwm_module_control_t all_ctl_modules = kPWM_Control_Module_0
                                                    | kPWM_Control_Module_1
                                                    | kPWM_Control_Module_2
                                                    | kPWM_Control_Module_3;
extern void Hw_UpdatePwm() {
    PWM_SetPwmLdok(PWM1_PERIPHERAL, all_ctl_modules, true);
}
