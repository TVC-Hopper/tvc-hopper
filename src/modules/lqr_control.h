#ifndef _SRUAV_CONTROL_H
#define _SRUAV_CONTROL_H

#define SETPOINT_MAX_Z                  1.0f // Max altitude (meters)
#define SETPOINT_MAX_ROLL               0.1f // Max roll (radians)
#define SETPOINT_MAX_PITCH              0.1f // Max pitch (radians)

// Maps the kRPM output of the controller to DSHOT values. This varies with voltage, thus should probably implement RPM controller at some point
#define MOTOR_KRPM_TO_DSHOT 72.43f 
#define CONTROL_LOOP_INTERVAL 0.005f

#define STATE_VECTOR_SIZE               9
#define ACTUATION_VECTOR_SIZE           5

#define HOVCTRL_STATUS_T                uint8_t
#define HOVCTRL_STATUS_OK               ((uint8_t)0x00)
#define HOVCTRL_STATUS_ERROR            ((uint8_t)0x01)

static const float K_hover[ACTUATION_VECTOR_SIZE][STATE_VECTOR_SIZE] = {    
    70.711,   0.000,      5.000,      12.161,     0.000,      5.217,      0.000,    0.000,    0.000,
    0.000,    -70.711,    -5.000,     0.000,      -12.162,    -5.217,     0.000,    0.000,    0.000,
    70.711,   0.000,      -5.000,     12.161,     0.000,      -5.217,     0.000,    0.000,    0.000,
    0.000,    -70.711,    5.000,      0.000,      -12.162,    5.217,      0.000,    0.000,    0.000,
    0.000,    0.000,      0.000,      0.000,      0.000,      0.000,      7.716,    4.140,    7.071 
};

typedef enum{
    CONTROL_STATUS_STATIONARY = 0,
    CONTROL_STATUS_FLYING,
    CONTROL_STATUS_LANDING,
} control_status_t; 

typedef enum{
    SETPOINT_X = 0,
    SETPOINT_Y,
    SETPOINT_Z,
    SETPOINT_ROLL,
    SETPOINT_PITCH,
    SETPOINT_YAW
} control_setpoint_t;

extern void HoverControl_Task(void* task_args);

#endif