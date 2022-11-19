#ifndef HW_BATT_MONITOR_H
#define HW_BATT_MONITOR_H

/**
 *  Initialize the battery monitor hardware interface
 */
extern void BattMon_Init();

/**
 *  Return battery voltage
 */
extern float BattMon_GetVoltage();


#endif
