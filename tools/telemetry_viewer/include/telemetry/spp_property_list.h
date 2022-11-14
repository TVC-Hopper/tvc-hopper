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


#endif