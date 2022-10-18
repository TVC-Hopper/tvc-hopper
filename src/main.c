#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/timers.h>

#include <bsp/board.h>
#include <bsp/pin_mux.h>
#include <bsp/peripherals.h>

#include <fslhal/fsl_lpuart_freertos.h>
#include <fslhal/fsl_lpuart.h>

#include "modules/command_control_comms.h"

#include "app_hal_xconnect.h"


#define PRIORITY_COMMAND_CONTROL_COMMS      1
#define PROIRITY_UART_LISTENER              2

static void CreateTasks();

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPeripherals();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();
    

    UartListener_Init();
    CommandControlComms_Init();


    CreateTasks();

    vTaskStartScheduler();

    for (;;) {}
}

static void CreateTasks() {
    if (xTaskCreate(CommandControlComms_Task,
                        "cc_comms",
                        configMINIMAL_STACK_SIZE,
                        NULL,
                        PRIORITY_COMMAND_CONTROL_COMMS,
                        NULL
                    ) != pdPASS)
    {
        while (1) {}
    }

    if (xTaskCreate(UartListener_Task,
                        "uart_comms",
                        configMINIMAL_STACK_SIZE + 128,
                        NULL,
                        PROIRITY_UART_LISTENER,
                        NULL) != pdPASS)
    {
        while (1) {}
    }
}
