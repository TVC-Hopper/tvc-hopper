#ifndef APP_HAL_XCONNECT_H
#define APP_HAL_XCONNECT_H

#include <stdint.h>

#include <stcp/stcp.h>

#include <sppclient/api.h>
#include <sppclient/defs.h>

#include <modules/command_control_comms.h>

/**
 *  Initialize Application-HAL interconnect
 */
extern void AppHal_Init();

/**
 *  Send packet through STCP
 *
 *  @param buffer data to send
 *  @param len number of bytes to send
 *  @param instance_data user data
 */
extern StcpStatus_t XCCb_StcpSendPacket(void* buffer, uint16_t len, void* instance_data);

/**
 *  Initialize UART listener task
 */
extern void UartListener_Init();

/**
 * UART listener task
 */
extern void UartListener_Task(void* task_args);

/**
 *  Write to I2C register
 *
 *  @param address I2C device address
 *  @param subaddress I2C subaddress (register address)
 *  @param data bytes to send
 *  @param size number of bytes to send
 */
extern uint8_t XCCb_I2CRegisterWrite(uint8_t address, uint8_t subaddress, uint8_t* data, uint16_t size);

/**
 *  Read from a register with I2C
 *
 *  @param address I2C device address
 *  @param subaddress I2C subaddress (register address)
 *  @param data bytes to send
 *  @param size number of bytes to send
 */
extern uint8_t XCCb_I2CRegisterRead(uint8_t address, uint8_t subaddress, uint8_t* data, uint16_t size);

/**
 *  Write to an I2C device without a register interface
 *
 *  @param address I2C device address
 *  @param data bytes to write
 *  @param size number of bytes to write
 */
extern uint8_t XCCb_I2CWrite(uint8_t address, uint8_t* data, uint16_t size);

/**
 *  Read from an I2C device without a register interface
 *
 *  @param address I2C device address
 *  @param data buffer to fill 
 *  @param size number of bytes read 
 */
extern uint8_t XCCb_I2CRead(uint8_t address, uint8_t* data, uint16_t size);

/**
 *  Delay the task for the given number of microseconds
 *
 *  TODO: fix precision, millisecond precision only
 *
 *  @param microseconds number of microseconds to delay
 */
extern void XCCb_DelayMs(uint32_t microseconds);

#endif
