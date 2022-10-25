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

#include "hw/imu.h"
#include "hw/lidar.h"
#include "hw/thrust_vanes.h"

#include "app_hal_xconnect.h"


#define PRIORITY_IMU_ACC                    5
#define PRIORITY_THRUST_VANES               5
#define PRIORITY_IMU_GYRO                   5
#define PRIORITY_COMMAND_CONTROL_COMMS      2
#define PROIRITY_UART_LISTENER              1

static void CreateTasks();

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    // this needs to be after debug console init
    // for some reason
    BOARD_InitBootPeripherals();
    
    AppHal_Init();

    UartListener_Init();
    CommandControlComms_Init();

    HwImu_Init();
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

    if (xTaskCreate(HwImu_GyroTask,
                        "hwimu_gyro",
                        configMINIMAL_STACK_SIZE + 128,
                        NULL,
                        PRIORITY_IMU_GYRO,
                        NULL) != pdPASS)
    {
        while (1) {}
    }

    if (xTaskCreate(HwImu_AccTask,
                        "hwimu_acc",
                        configMINIMAL_STACK_SIZE + 128,
                        NULL,
                        PRIORITY_IMU_ACC,
                        NULL) != pdPASS)
    {
        while (1) {}
    }

    if (xTaskCreate(HwThrustVane_Task,
                        "hwthrustvanes",
                        configMINIMAL_STACK_SIZE + 128,
                        NULL,
                        PRIORITY_THRUST_VANES,
                        NULL) != pdPASS)
    {
        while (1) {}
    }
}
