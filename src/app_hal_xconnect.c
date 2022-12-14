#include "app_hal_xconnect.h"

#include <stdint.h>

#include <fslhal/fsl_lpuart_freertos.h>
#include <fslhal/fsl_lpi2c_freertos.h>
#include <fslhal/fsl_lpuart.h>
#include <fslhal/fsl_lpi2c.h>
#include <fslhal/fsl_gpio.h>

#include <bsp/peripherals.h>

#include <stcp/stcp.h>

#include <sppclient/api.h>
#include <sppclient/defs.h>

#include <circbuf/cbuf.h>

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

#include "modules/command_control_comms.h"
#include "hw/imu.h"


#define UART_RCV_BUFFER_SIZE 1
#define UART_MSG_PACKET_SIZE 256

// TODO: unsure if these pins are correct, are these IOMUX values?
#define IMU_ACCEL_INTTERRUPT_GPIO 23
#define IMU_GYRO_INTTERRUPT_GPIO 24

// temporary buffers for receiving and parsing messages
static uint8_t uart_rcv_buffer[UART_RCV_BUFFER_SIZE];
static uint8_t uart_msg_buffer[UART_MSG_PACKET_SIZE];

// uart listener needs STCP to unescape
static StcpEngine_t* stcp;
static lpuart_rtos_handle_t* uart_handle = &COMMS_UART_rtos_handle;

// protect i2c bus
static SemaphoreHandle_t i2c_mx;

extern void AppHal_Init() {
    NVIC_SetPriority(LPI2C1_IRQn, 8);
    i2c_mx = xSemaphoreCreateMutex();
}

void GPIO1_GPIO_COMB_0_15_IRQHANDLER(void) {
    uint32_t pins_flags = GPIO_GetPinsInterruptFlags(GPIO1); 

    // code goes here

    GPIO_ClearPinsInterruptFlags(GPIO1, pins_flags); 

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

void GPIO1_GPIO_COMB_16_31_IRQHANDLER(void) {
    uint32_t pins_flags = GPIO_GetPinsInterruptFlags(GPIO1); 

    // code goes here

    GPIO_ClearPinsInterruptFlags(GPIO1, pins_flags); 

    /* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F
     Store immediate overlapping exception return operation might vector to incorrect interrupt. */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}


extern StcpStatus_t XCCb_StcpSendPacket(void* buffer, uint16_t len, void* instance_data) {
	int error = LPUART_RTOS_Send(uart_handle, buffer, len);
    
    if (kStatus_Success == error) {
        return SPP_STATUS_OK;
    } else {
        return SPP_STATUS_ERROR;
    }
}

extern uint8_t XCCb_I2CWrite(uint8_t address, uint8_t* data, uint16_t size) {
    xSemaphoreTake(i2c_mx, 0xFFFF);
    
    LPI2C1_masterTransfer.slaveAddress = address;
    LPI2C1_masterTransfer.direction = kLPI2C_Write;
    
    // set subaddress size to 0 for no subaddress
    LPI2C1_masterTransfer.subaddress = 0;
    LPI2C1_masterTransfer.subaddressSize = 0;
    LPI2C1_masterTransfer.data = data;
    LPI2C1_masterTransfer.dataSize = size;

    status_t status = LPI2C_RTOS_Transfer(&LPI2C1_masterHandle, &LPI2C1_masterTransfer);

    xSemaphoreGive(i2c_mx);

    return status;
}

extern uint8_t XCCb_I2CRead(uint8_t address, uint8_t* data, uint16_t size) {
    xSemaphoreTake(i2c_mx, 0xFFFF);

    LPI2C1_masterTransfer.slaveAddress = address;
    LPI2C1_masterTransfer.direction = kLPI2C_Read;

    // set subaddress size to 0 for no subaddress
    LPI2C1_masterTransfer.subaddress = 0;
    LPI2C1_masterTransfer.subaddressSize = 0;
    LPI2C1_masterTransfer.data = data;
    LPI2C1_masterTransfer.dataSize = size;

    status_t status = LPI2C_RTOS_Transfer(&LPI2C1_masterHandle, &LPI2C1_masterTransfer);

    xSemaphoreGive(i2c_mx);

    return status;
}

extern uint8_t XCCb_I2CRegisterWrite(uint8_t address, uint8_t subaddress, uint8_t* data, uint16_t size) {
    xSemaphoreTake(i2c_mx, 0xFFFF);
    
    LPI2C1_masterTransfer.slaveAddress = address;
    LPI2C1_masterTransfer.direction = kLPI2C_Write;
    LPI2C1_masterTransfer.subaddress = subaddress;
    LPI2C1_masterTransfer.subaddressSize = 1;
    LPI2C1_masterTransfer.data = data;
    LPI2C1_masterTransfer.dataSize = size;

    status_t status = LPI2C_RTOS_Transfer(&LPI2C1_masterHandle, &LPI2C1_masterTransfer);

    xSemaphoreGive(i2c_mx);

    return status;
}

extern uint8_t XCCb_I2CRegisterRead(uint8_t address, uint8_t subaddress, uint8_t* data, uint16_t size) {
    xSemaphoreTake(i2c_mx, 0xFFFF);

    LPI2C1_masterTransfer.slaveAddress = address;
    LPI2C1_masterTransfer.direction = kLPI2C_Read;
    LPI2C1_masterTransfer.subaddress = subaddress;
    LPI2C1_masterTransfer.subaddressSize = 1;
    LPI2C1_masterTransfer.data = data;
    LPI2C1_masterTransfer.dataSize = size;

    status_t status = LPI2C_RTOS_Transfer(&LPI2C1_masterHandle, &LPI2C1_masterTransfer);

    xSemaphoreGive(i2c_mx);

    return status;
}

extern void XCCb_DelayMs(uint32_t ms) {
    vTaskDelay(ms);
}

extern void UartListener_Init() {
    stcp = GetStcpEngine();
}


extern void UartListener_Task(void* task_args) {
    
    int error;
    size_t bytes_rcv_count = 0;

    uint32_t msg_rcv_idx = 0;

    while (true) {
        // get bytes
        error = LPUART_RTOS_Receive(uart_handle, uart_rcv_buffer, UART_RCV_BUFFER_SIZE, &bytes_rcv_count);

        if (error != kStatus_Success) {
            vTaskSuspend(NULL);
        }

        if (bytes_rcv_count > 0) {
            for (uint32_t i = 0; i < bytes_rcv_count; ++i) {
                // copy bytes to buffer
                uart_msg_buffer[msg_rcv_idx++] = uart_rcv_buffer[i];
                if (msg_rcv_idx >= 2) {
                    // look for footer
                    if (uart_msg_buffer[msg_rcv_idx - 2] == STCP_FOOTER
                            && uart_msg_buffer[msg_rcv_idx - 1] == STCP_FOOTER) {
                        // handle STCP message if complete
                        StcpHandleMessage(stcp, uart_msg_buffer, msg_rcv_idx);

                        // start filling buffer again
                        msg_rcv_idx = 0;
                    }
                }
            }
        }
    }

    // should not get here
    LPUART_RTOS_Deinit(uart_handle);
    vTaskSuspend(NULL);
}










