#ifndef TELEMETRY_COMMS_SPP_HOST_H
#define TELEMETRY_COMMS_SPP_HOST_H

#include <stdint.h>

#define PROP_start_ID ((uint16_t) 0)
#define PROP_stop_ID ((uint16_t) 1)
#define PROP_status_ID ((uint16_t) 2)
#define PROP_telem_data_ID ((uint16_t) 10)
#define PROP_telem_cap_ID ((uint16_t) 11)
#define PROP_telem_cap_start_ID ((uint16_t) 12)
#define PROP_telem_cap_stop_ID ((uint16_t) 13)
#define PROP_battery_v_ID ((uint16_t) 18)
#define PROP_timestamp_ms_ID ((uint16_t) 19)
#define PROP_reset_system_ID ((uint16_t) 30)
#define PROP_servo_positions_ID ((uint16_t) 40)
#define PROP_target_position_ID ((uint16_t) 41)
#define PROP_raw_lidar_ID ((uint16_t) 50)
#define PROP_raw_imu_ID ((uint16_t) 51)
#define PROP_imu_ID ((uint16_t) 52)
#define PROP_esc_pwm_ID ((uint16_t) 53)
#define PROP_k_matrix_ID ((uint16_t) 54)
#define PROP_z_lim_spinup_ID ((uint16_t) 55)
#define PROP_z_lim_normal_ID ((uint16_t) 56)
#define PROP_max_zint_ID ((uint16_t) 57)
#define PROP_max_esc_out_ID ((uint16_t) 58)
#define PROP_set_pitch_offset_ID ((uint16_t) 59)
#define PROP_set_roll_offset_ID ((uint16_t) 60)


#endif