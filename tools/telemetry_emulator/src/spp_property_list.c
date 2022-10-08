
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
        .size        = 10,
        .flags       = {SPP_FLAG_WRITEABLE},
        .name        = "telemetry\0",
        .name_length = 10,
    },
};




extern const SppPropertyDefinition_t* SppGetPropertyList() { return property_list; }

