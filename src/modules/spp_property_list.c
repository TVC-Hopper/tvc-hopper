
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
        .id          = PROP_telem_data_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 32,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "telem_data\0",
        .name_length = 11,
    },
    {
        .id          = PROP_telem_cap_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 64,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "telem_cap\0",
        .name_length = 10,
    },
    {
        .id          = PROP_telem_cap_start_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "telem_cap_start\0",
        .name_length = 16,
    },
    {
        .id          = PROP_telem_cap_stop_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "telem_cap_stop\0",
        .name_length = 15,
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
        .id          = PROP_reset_system_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "reset_system\0",
        .name_length = 13,
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
        .id          = PROP_raw_lidar_ID,
        .type        = SPP_PROP_T_U32,
        .size        = 4,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "raw_lidar\0",
        .name_length = 10,
    },
    {
        .id          = PROP_raw_imu_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 40,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "raw_imu\0",
        .name_length = 8,
    },
    {
        .id          = PROP_imu_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 24,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "imu\0",
        .name_length = 4,
    },
    {
        .id          = PROP_esc_pwm_ID,
        .type        = SPP_PROP_T_FLOAT,
        .size        = 4,
        .flags       = {SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE},
        .name        = "esc_pwm\0",
        .name_length = 8,
    },
    {
        .id          = PROP_k_matrix_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 40,
        .flags       = {SPP_FLAG_READABLE | SPP_FLAG_WRITEABLE},
        .name        = "k_matrix\0",
        .name_length = 9,
    },
    {
        .id          = PROP_z_lim_spinup_ID,
        .type        = SPP_PROP_T_FLOAT,
        .size        = 4,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "z_lim_spinup\0",
        .name_length = 13,
    },
    {
        .id          = PROP_z_lim_normal_ID,
        .type        = SPP_PROP_T_FLOAT,
        .size        = 4,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "z_lim_normal\0",
        .name_length = 13,
    },
    {
        .id          = PROP_max_zint_ID,
        .type        = SPP_PROP_T_FLOAT,
        .size        = 4,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "max_zint\0",
        .name_length = 9,
    },
    {
        .id          = PROP_max_esc_out_ID,
        .type        = SPP_PROP_T_FLOAT,
        .size        = 4,
        .flags       = {SPP_FLAG_READABLE},
        .name        = "max_esc_out\0",
        .name_length = 12,
    },
};




extern const SppPropertyDefinition_t* SppGetPropertyList() { return property_list; }

