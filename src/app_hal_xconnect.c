#include "app_hal_xconnect.h"

#include <stdint.h>

#include <fslhal/fsl_lpuart_freertos.h>
#include <fslhal/fsl_lpuart.h>

#include <bsp/peripherals.h>

#include <stcp/stcp.h>

#include <sppclient/api.h>
#include <sppclient/defs.h>

#include <circbuf/cbuf.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include "modules/command_control_comms.h"


#define UART_RCV_BUFFER_SIZE 1
#define UART_MSG_PACKET_SIZE 256

static uint8_t uart_rcv_buffer[UART_RCV_BUFFER_SIZE];
static uint8_t uart_msg_buffer[UART_MSG_PACKET_SIZE];

char *to_send               = "FreeRTOS LPUART driver example!\r\n";
char *send_ring_overrun     = "\r\nRing buffer overrun!\r\n";
char *send_hardware_overrun = "\r\nHardware buffer overrun!\r\n";
char *send_timeout          = "\r\nTimeout expired!\r\n";


static StcpEngine_t* stcp;

static lpuart_rtos_handle_t* uart_handle = &COMMS_UART_rtos_handle;

extern SPP_STATUS_T XCCb_StcpSendPacket(uint8_t* buffer, uint16_t len, void* instance_data) {
	int error = LPUART_RTOS_Send(uart_handle, buffer, len);
    
    if (kStatus_Success == error) {
        return SPP_STATUS_OK;
    } else {
        return SPP_STATUS_ERROR;
    }
}

extern void UartListener_Init() {
    stcp = GetStcpEngine();
}


extern void UartListener_Task(void* task_args) {
    
    int error;
    size_t bytes_rcv_count = 0;

    uint32_t msg_rcv_idx = 0;

    while (true) {
        error = LPUART_RTOS_Receive(uart_handle, uart_rcv_buffer, UART_RCV_BUFFER_SIZE, &bytes_rcv_count);

        if (error != kStatus_Success) {
            vTaskSuspend(NULL);
        }

        if (bytes_rcv_count > 0) {
            for (uint32_t i = 0; i < bytes_rcv_count; ++i) {
                uart_msg_buffer[msg_rcv_idx++] = uart_rcv_buffer[i];
                if (msg_rcv_idx >= 2) {
                    if (uart_msg_buffer[msg_rcv_idx - 2] == STCP_FOOTER
                            && uart_msg_buffer[msg_rcv_idx - 1] == STCP_FOOTER) {
                        StcpHandleMessage(stcp, uart_msg_buffer, msg_rcv_idx);
                        msg_rcv_idx = 0;
                    }
                }
            }
        }
    }

    LPUART_RTOS_Deinit(uart_handle);
    vTaskSuspend(NULL);
}










