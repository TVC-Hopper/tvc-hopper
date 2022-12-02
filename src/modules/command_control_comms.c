#include "command_control_comms.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <stcp/stcp.h>
#include <sppclient/api.h>
#include <sppclient/defs.h>

#include "spp_property_list.h"
#include "app_hal_xconnect.h"

#include "modules/lqr_control.h"
#include "modules/control_inputs.h"

#include "hw/imu.h"
#include "hw/esc.h"
#include "hw/batt_monitor.h"
#include "hw/lidar.h"
#include "hw/thrust_vanes.h"


static SPP_STATUS_T SppSendPacket(uint8_t* bytes, uint16_t len, void* instance_data);
static StcpStatus_t HandleStcpPacket(void* buffer, uint16_t length, void* instance_data);
static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data);
static SPP_STATUS_T GetValue(uint16_t id, void* value, void* instance_data);

static void GetTelemetry(uint8_t* value);

SppClientEngine_t spp;
StcpEngine_t stcp;

static uint8_t broadcast_addr_raw = 0x00;
static uint8_t addr_raw = 0x01;
static uint8_t property_list_id = 0x00;
static uint8_t client_address_buffer[3];

static bool is_telemetry_filtered = false;

static SppAddress_t broadcast_addr = { &broadcast_addr_raw };
static SppAddress_t address = { &addr_raw };

extern StcpEngine_t* GetStcpEngine() {
    return &stcp;
}


extern void CommandControlComms_Init() {
    SppClientEngineInitParams_t spp_ip;
    spp_ip.broadcast_address = broadcast_addr;
    spp_ip.client_address = address;
    spp_ip.host_address_buffer = &client_address_buffer;
    spp_ip.property_count = SPP_PROP_COUNT;
    spp_ip.property_list_id = property_list_id;
    spp_ip.address_length = sizeof(uint8_t);
    spp_ip.properties = SppGetPropertyList();

    SppClientCallbacks_t spp_callbacks = {
        .Send = SppSendPacket,
        .SetValue = SetValue,
        .GetValue = GetValue,
    };

    spp_ip.callbacks = spp_callbacks;
    spp_ip.instance_data = NULL;

    SppClientEngineInit(&spp_ip, &spp);

    StcpCallbacks_t stcp_callbacks = {
        .Send = XCCb_StcpSendPacket,
        .HandleMessage = HandleStcpPacket,
    };
    stcp.callbacks = stcp_callbacks;
    stcp.instance_data = NULL;
}


extern void CommandControlComms_Task(void* task_args) {
    SppClientStart(&spp);
    uint32_t last = xTaskGetTickCount();
    for(;;) {
        uint32_t now = xTaskGetTickCount();
        SppProcessStreams(&spp, now, now - last);
        last = now;
        vTaskDelay(5);
    }
}

static SPP_STATUS_T SetValue(uint16_t id, void* value, void* instance_data) {
    switch(id) {
        case PROP_start_ID:
        {
            if (*((uint8_t*) value) == 1) {
                HoverControl_Start();
            }
            break;
        }
        case PROP_stop_ID:
        {
            if (*((uint8_t*) value) == 1) {
                HoverControl_Stop();
            }
            break;
        }
        case PROP_telem_cap_start_ID:
        {
            break;
        }
        case PROP_telem_cap_stop_ID:
        {
            break;
        }
        case PROP_reset_system_ID:
        {
            break;
        }
        case PROP_servo_positions_ID:
        {
            float* positions = (float*)value;
            HwThrustVane_SetPositions(positions);
            break;
        }
        case PROP_target_position_ID:
        {
            float* setpoints = (float*)value;
            // TODO: verify this follows indexing [x, y, z]
            // only z_pos will be used for hover control
            HoverControl_SetReference(setpoints);
            break;
        }
        case PROP_esc_pwm_ID:
        {
            float* pulse_width = (float*)value;
            HwEsc_SetOutput(*pulse_width, true);
            break;
        }
        case PROP_k_matrix_ID:
        {
            //TODO: FIX
            float* new_K = (float*)value;
            HoverControl_WriteK(new_K);
            break;
        }
        case PROP_z_lim_spinup_ID:
        {
            //TODO
            break;
        }
        case PROP_z_lim_normal_ID:
        {
            //TODO
            break;
        }
        case PROP_max_zint_ID:
        {
            //TODO
            break;
        }
        case PROP_max_esc_out_ID:
        {
            //TODO
            break;
        }        
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }

    return SPP_STATUS_OK;
}

static SPP_STATUS_T GetValue(uint16_t id, void* value, void* instance_data) {
    switch(id) {
        case PROP_telem_data_ID:
        {
            GetTelemetry(value);
            break;
        }
        case PROP_telem_cap_ID:
        {
            break;
        }
        case PROP_battery_v_ID:
        {
            float v = BattMon_GetVoltage();
            memcpy((uint8_t*)value, &v, sizeof(v));
            break;
        }
        case PROP_timestamp_ms_ID:
        {
            uint32_t t = xTaskGetTickCount();
            memcpy((uint8_t*)value, &t, sizeof(t));
            break;
        }
        case PROP_servo_positions_ID:
        {
            float positions[4];
            HwThrustVane_GetPositions(positions);
            memcpy((uint8_t*)value, positions, sizeof(positions));
            break;
        }
        case PROP_target_position_ID:
        {
            
            break;
        }
        case PROP_raw_lidar_ID:
        {
            uint32_t dist = HwLidar_GetDistance();
            memcpy((uint8_t*)value, &dist, sizeof(dist));
            break;
        }
        case PROP_raw_imu_ID:
        {
            float data[10];
            HwImu_GetReadings(data);
            memcpy((uint8_t*)value, data, sizeof(data));
            break;
        }
        case PROP_imu_ID:
        {
            // roll pitch yaw, x, y, z
            ControlsInputs_GetIMU(value);
        }
        case PROP_esc_pwm_ID:
        {
            HoverControl_GetThrottlePercent(value);
        }
        default:
        {
            return SPP_STATUS_UNKNOWN_PROPERTY;
        }
    }

    return SPP_STATUS_OK;
}

static void GetTelemetry(uint8_t* value) {
    float* value_f = (float*)value;

    ControlsInputs_GetIMU(value_f);
    value_f += 6;

    value[7] = 0.0;
//    ControlsInputs_GetLidar(value_f);
//    value_f += 1;

    HwEsc_GetOutput(&value_f[8]);
}

static StcpStatus_t HandleStcpPacket(void* buffer, uint16_t length, void* instance_data) {
    SppClientProcessMessage(&spp, buffer, length);

    return STCP_STATUS_SUCCESS;
}

static SPP_STATUS_T SppSendPacket(uint8_t* bytes, uint16_t len, void* instance_data) {
   StcpWrite(&stcp, bytes, len);

   return SPP_STATUS_OK;
}

