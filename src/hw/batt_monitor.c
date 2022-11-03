#include "hw/batt_monitor.h"

#include <bsp/peripherals.h>

#include <fslhal/fsl_adc.h>

#define UPPER_VOLTAGE_READING   3.2f
#define ADC_COUNT_MAX           (4096.0f)
#define UPPER_BATTERY_VOLTAGE   26.0f

extern void BattMon_Init() {
    ADC_DoAutoCalibration(ADC1);
}

extern float BattMon_GetVoltage() {
    ADC_SetChannelConfig(ADC1, ADC1_CH0_CONTROL_GROUP, &ADC1_channels_config[0]);
    while (0U == ADC_GetChannelStatusFlags(ADC1, ADC1_CH0_CONTROL_GROUP)) {}
    uint32_t adc_val = ADC_GetChannelConversionValue(ADC1, ADC1_CH0_CONTROL_GROUP);

    return UPPER_VOLTAGE_READING * (adc_val / ADC_COUNT_MAX);
}
