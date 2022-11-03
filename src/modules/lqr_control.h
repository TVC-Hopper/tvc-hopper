#ifndef MODULE_HOVER_CONTROL_H
#define MODULE_HOVER_CONTROL_H

#define SETPOINT_MAX_Z                  1.0 // Max altitude (meters)
#define SETPOINT_MAX_ROLL               0.1 // Max roll (radians)
#define SETPOINT_MAX_PITCH              0.1 // Max pitch (radians)

// Maps the kRPM output of the controller to % ESC actuation.
#define MOTOR_KRPM_TO_ESC_PERCENT       72.43 // placeholder, FIXME
#define CONTROL_LOOP_INTERVAL           5.0 // (milliseconds)

#define STATE_VECTOR_SIZE               9
#define ACTUATION_VECTOR_SIZE           5

#define STATE_IDX_T                     uint16_t
#define STATE_IDX_ROLL                  ((uint16_t)0x00)
#define STATE_IDX_PITCH                 ((uint16_t)0x01)
#define STATE_IDX_YAW                   ((uint16_t)0x02)
#define STATE_IDX_GX                    ((uint16_t)0x03)
#define STATE_IDX_GY                    ((uint16_t)0x04)
#define STATE_IDX_GZ                    ((uint16_t)0x05)
#define STATE_IDX_Z                     ((uint16_t)0x06)
#define STATE_IDX_VZ                    ((uint16_t)0x07)
#define STATE_IDX_ZINT                  ((uint16_t)0x08)

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
//  roll,     pitch,      yaw,        gx,         gy,         gz,         z,        vz,       zint

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

extern void HoverControl_Init();
extern void HoverControl_Task(void* task_args);

#endif