#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/timers.h>

#include <bsp/board.h>
#include <bsp/pin_mux.h>
#include <bsp/peripherals.h>

#include <fslhal/fsl_lpuart_freertos.h>
#include <fslhal/fsl_lpuart.h>
#include <fslhal/fsl_xbara.h>

#include "modules/command_control_comms.h"

#include "lqr_control.h"

#include "hw/imu.h"
#include "hw/esc.h"
#include "hw/lidar.h"
#include "hw/thrust_vanes.h"

#include "app_hal_xconnect.h"


#define PRIORITY_IMU_ACC                    5
#define PRIORITY_THRUST_VANES               5
#define PRIORITY_IMU_GYRO                   5
#define PRIORITY_COMMAND_CONTROL_COMMS      2
#define PROIRITY_UART_LISTENER              1
#define PRIORITY_HOVER_CONTROL              4 // FIXME

static void CreateTasks();

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    XBARA_Init(XBARA);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault2);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault3);

    // this needs to be after debug console init
    // for some reason
    BOARD_InitBootPeripherals();
    
    AppHal_Init();

    UartListener_Init();
    CommandControlComms_Init();

    // taskless wrappers for drivers
    HwImu_Init();
    HwLidar_Init();
    HwEsc_Init();
    HwThrustVane_Init();

    CreateTasks();

    vTaskStartScheduler();

    for (;;) {}
}

static void CreateTasks() {
    if (xTaskCreate(CommandControlComms_Task,
                        "cc_comms",
                        configMINIMAL_STACK_SIZE + 512,
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

    if(xTaskCreate(HoverControl_Task,
                        "hov_ctrl",
                        configMINIMAL_STACK_SIZE + 128,
                        NULL,
                        PRIORITY_HOVER_CONTROL,
                        NULL) != pdPASS)
    {
        while (1) {}
    }
}
