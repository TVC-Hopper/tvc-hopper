#ifndef MODULE_HOVER_CONTROL_H
#define MODULE_HOVER_CONTROL_H

#define SETPOINT_MAX_Z                  1.0 // Max altitude (meters)
#define SETPOINT_MAX_ROLL               0.1 // Max roll (radians)
#define SETPOINT_MAX_PITCH              0.1 // Max pitch (radians)

// Maps the kRPM output of the controller to % ESC actuation.
#define MOTOR_KRPM_TO_ESC_PERCENT       72.43f // FIXME: placeholder
#define CONTROL_LOOP_INTERVAL           5 // (milliseconds)

// Math
#define PI                              3.142857f

#define STATE_VECTOR_SIZE               9
#define ACTUATION_VECTOR_SIZE           5

#define LANDING_SPEED_FACTOR_CLOSE      1.2f
#define LANDING_SPEED_FACTOR_FAR        0.2f

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

#define ACTUATOR_IDX_T                  uint8_t
#define ACTUATOR_IDX_VANE1              ((uint16_t)0x00)
#define ACTUATOR_IDX_VANE2              ((uint16_t)0x01)
#define ACTUATOR_IDX_VANE3              ((uint16_t)0x02)
#define ACTUATOR_IDX_VANE4              ((uint16_t)0x03)
#define ACTUATOR_IDX_ESC                ((uint16_t)0x04)

#define HOVCTRL_MATH_STATUS_T           uint8_t
#define HOVCTRL_MATH_STATUS_OK          ((uint8_t)0x00)
#define HOVCTRL_MATH_STATUS_ERROR       ((uint8_t)0x01)

typedef enum hovctrl_status_e {
    HOVCTRL_STATUS_STATIONARY,
    HOVCTRL_STATUS_TAKEOFF,
    HOVCTRL_STATUS_FLYING,
    HOVCTRL_STATUS_LANDING_CLOSE,
    HOVCTRL_STATUS_LANDING_FAR
} hovctrl_status_t; 

typedef enum control_setpoint_e {
    SETPOINT_X,
    SETPOINT_Y,
    SETPOINT_Z
    // SETPOINT_ROLL,
    // SETPOINT_PITCH,
    // SETPOINT_YAW
} control_setpoint_t;

extern void HoverControl_Init();

extern void HoverControl_Task();

extern void HoverControl_SetReference(float* setpoints);

extern void HoverControl_GetState(float* tlm);

extern hovctrl_status_t HoverControl_GetStatus();

#endif
