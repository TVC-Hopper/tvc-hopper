#include "spp_property_list.h"

extern const SppPropertyDefinition_t property_list[4] = {
    {
        .id          = PROP_start_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = SPP_FLAG_READABLE,
        .name        = start ,
        .name_length = 6,
    },
    {
        .id          = PROP_stop_ID,
        .type        = SPP_PROP_T_BOOL,
        .size        = 1,
        .flags       = SPP_FLAG_READABLE,
        .name        = stop ,
        .name_length = 5,
    },
    {
        .id          = PROP_status_ID,
        .type        = SPP_PROP_T_U32,
        .size        = 4,
        .flags       = SPP_FLAG_WRITEABLE,
        .name        = status ,
        .name_length = 7,
    },
    {
        .id          = PROP_telemetry_ID,
        .type        = SPP_PROP_T_ARR,
        .size        = 10,
        .flags       = SPP_FLAG_WRITEABLE,
        .name        = telemetry ,
        .name_length = 10,
    },
};


