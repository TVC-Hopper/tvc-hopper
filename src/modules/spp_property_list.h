
/*
 * WARNING: AUTO-GENERATED CODE DO NOT MODIFY
 *
 *
 *  To make changes to properties, edit support/prop_list.toml.
 *  To make changes to the format of this file, edit tools/generate_spp_lists.py
 */


#ifndef SPP_PROPERTY_LIST_GENERATED_H
#define SPP_PROPERTY_LIST_GENERATED_H

#include <sppclient/defs.h>
#include <sppclient/api.h>

#define SPP_PROP_COUNT 12

#define PROP_start_ID ((uint16_t) 0)
#define PROP_stop_ID ((uint16_t) 1)
#define PROP_status_ID ((uint16_t) 2)
#define PROP_telemetry_ID ((uint16_t) 3)
#define PROP_battery_v_ID ((uint16_t) 4)
#define PROP_timestamp_ms_ID ((uint16_t) 5)
#define PROP_telem_filter_en_ID ((uint16_t) 6)
#define PROP_reset_system_ID ((uint16_t) 7)
#define PROP_reset_controls_ID ((uint16_t) 8)
#define PROP_servo_positions_ID ((uint16_t) 9)
#define PROP_target_position_ID ((uint16_t) 10)
#define PROP_param_bounds_ID ((uint16_t) 11)


const SppPropertyDefinition_t* SppGetPropertyList();


#endif

