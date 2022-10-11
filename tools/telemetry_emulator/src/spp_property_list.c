
/*
 * WARNING: AUTO-GENERATED CODE DO NOT MODIFY
 *
 *
 *  To make changes to properties, edit support/prop_list.toml.
 *  To make changes to the format of this file, edit tools/generate_spp_lists.py
 */

#include "spp_property_list.h"

static const SppPropertyDefinition_t property_list[SPP_PROP_COUNT] = {
    {
        .id          = PROP_start_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "start\0",
        .name_length = 6,
    },
    {
        .id          = PROP_stop_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "stop\0",
        .name_length = 5,
    },
    {
        .id          = PROP_status_ID,
        .type        = SPP_PROP_T_U32,
        .size        = 4,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "status\0",
        .name_length = 7,
    },
    {
        .id          = PROP_telemetry_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 52,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "telemetry\0",
        .name_length = 10,
    },
    {
        .id          = PROP_battery_v_ID,
        .type        = SPP_PROP_T_FLOAT,
        .size        = 4,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "battery_v\0",
        .name_length = 10,
    },
    {
        .id          = PROP_timestamp_ms_ID,
        .type        = SPP_PROP_T_U32,
        .size        = 4,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "timestamp_ms\0",
        .name_length = 13,
    },
    {
        .id          = PROP_telem_filter_en_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "telem_filter_en\0",
        .name_length = 16,
    },
    {
        .id          = PROP_reset_system_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "reset_system\0",
        .name_length = 13,
    },
    {
        .id          = PROP_reset_controls_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "reset_controls\0",
        .name_length = 15,
    },
    {
        .id          = PROP_servo_positions_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 16,
        .flags       = {SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE},
        .name        = "servo_positions\0",
        .name_length = 16,
    },
    {
        .id          = PROP_target_position_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 12,
        .flags       = {SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE},
        .name        = "target_position\0",
        .name_length = 16,
    },
    {
        .id          = PROP_param_bounds_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 36,
        .flags       = {SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE},
        .name        = "param_bounds\0",
        .name_length = 13,
    },
};




extern const SppPropertyDefinition_t* SppGetPropertyList() { return property_list; }

