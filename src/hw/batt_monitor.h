#ifndef HW_BATT_MONITOR_H
#define HW_BATT_MONITOR_H

/**
 *  Initialize the battery monitor hardware interface
 */
extern void BattMon_Init();

/**
 *  Get battery voltage
 *
 *  @return measured battery voltage (volts)
 */
extern float BattMon_GetVoltage();


#endif
