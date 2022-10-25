#ifndef APP_HAL_XCONNECT_H
#define APP_HAL_XCONNECT_H

#include <stdint.h>

#include <stcp/stcp.h>

#include <sppclient/api.h>
#include <sppclient/defs.h>

#include <modules/command_control_comms.h>

extern void AppHal_Init();

extern StcpStatus_t XCCb_StcpSendPacket(void* buffer, uint16_t len, void* instance_data);

extern void UartListener_Init();
extern void UartListener_Task(void* task_args);

extern uint8_t XCCb_I2CWrite(uint8_t address, uint8_t* data, uint16_t size);
extern uint8_t XCCb_I2CRead(uint8_t address, uint8_t* data, uint16_t size);
extern void XCCb_DelayMs(uint32_t microseconds);

extern void XCCb_ThrustVane_Servo0_Write(int us);
extern void XCCb_ThrustVane_Servo1_Write(int us);
extern void XCCb_ThrustVane_Servo2_Write(int us);
extern void XCCb_ThrustVane_Servo3_Write(int us);

#endif
