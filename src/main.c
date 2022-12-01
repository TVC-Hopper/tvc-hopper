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

#include "modules/lqr_control.h"
#include "modules/command_control_comms.h"
#include "modules/control_inputs.h"

#include "hw/imu.h"
#include "hw/esc.h"
#include "hw/lidar.h"
#include "hw/thrust_vanes.h"

#include "app_hal_xconnect.h"

#define PRIORITY_STARTUP                    4
#define PRIORITY_CTL_INPUTS                 2
#define PRIORITY_COMMAND_CONTROL_COMMS      3
#define PRIORITY_UART_LISTENER              5
#define PRIORITY_HOVER_CONTROL              1 // FIXME
#define PRIORITY_AUTO_LAND                  4 // FIXME: between comms and startup?

static void CreateTasks();

static void Setup_Task(void* task_args) {
    // depends on i2c freertos drivers, need to start freertos first
    HwImu_Start();

    while(1) {
        vTaskSuspend(NULL);
    }
}

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // setup crossbar for PWM
    XBARA_Init(XBARA);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault0);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault1);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault2);
    XBARA_SetSignalsConnection(XBARA, kXBARA1_InputLogicHigh, kXBARA1_OutputFlexpwm1Fault3);

    // this needs to be after debug console init
    // for some reason
    BOARD_InitBootPeripherals();
   
    // connect app and hal
    AppHal_Init();
   
    // module initialization
    ControlsInputs_Init();
    UartListener_Init();
    CommandControlComms_Init();
    HoverControl_Init();

    // taskless wrappers for drivers
    HwEsc_Init();
    HwThrustVane_Init();
    HwImu_Init();
    HwLidar_Init();

    CreateTasks();

    vTaskStartScheduler();

    for (;;) {}
}

static void CreateTasks() {
    if (xTaskCreate(CommandControlComms_Task,
                        "cc_comms",
                        configMINIMAL_STACK_SIZE + 640,
                        NULL,
                        PRIORITY_COMMAND_CONTROL_COMMS,
                        NULL
                    ) != pdPASS)
    {
        while (1) {}
    }

    if (xTaskCreate(UartListener_Task,
                        "uart_comms",
                        configMINIMAL_STACK_SIZE + 256,
                        NULL,
                        PRIORITY_UART_LISTENER,
                        NULL) != pdPASS)
    {
        while (1) {}
    }

    if (xTaskCreate(Setup_Task,
                        "setup",
                        configMINIMAL_STACK_SIZE + 128,
                        NULL,
                        PRIORITY_STARTUP,
                        NULL) != pdPASS)
    {
        while(1) {}
    }

    
    if (xTaskCreate(ControlsInputs_Task,
                        "ctl_inputs",
                        configMINIMAL_STACK_SIZE + 384,
                        NULL,
                        PRIORITY_CTL_INPUTS,
                        NULL) != pdPASS)
    {
        while (1) {}
    }

    if(xTaskCreate(HoverControl_Task,
                        "hov_ctrl",
                        configMINIMAL_STACK_SIZE + 384,
                        NULL,
                        PRIORITY_HOVER_CONTROL,
                        NULL) != pdPASS)
    {
        while (1) {}
    }


    if(xTaskCreate(HoverControlAutoLanding_Task,
                        "auto_landing",
                        configMINIMAL_STACK_SIZE + 384,
                        NULL,
                        PRIORITY_HOVER_CONTROL,
                        NULL) != pdPASS)
    {
        while (1) {}
    }
    

}
