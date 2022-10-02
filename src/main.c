#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>
#include <freertos/timers.h>

#include <bsp/board.h>
#include <bsp/pin_mux.h>

#include "modules/command_control_comms.h"

static void CreateTasks();

int main(void)
{
    BOARD_ConfigMPU();
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitDebugConsole();

    CommandControlComms_Init();

    CreateTasks();

    vTaskStartScheduler();

    for (;;) {}
}

static void CreateTasks() {
    if (xTaskCreate(CommandControlComms_Task, "cc_comms", configMINIMAL_STACK_SIZE + 100, NULL, 1, NULL) !=
        pdPASS)
    {
        while (1) {}
    }

}
