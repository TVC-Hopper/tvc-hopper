/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_lpuart.h"
#include "fsl_lpuart_freertos.h"
#include "fsl_clock.h"
#include "fsl_pwm.h"
#include "fsl_lpi2c.h"
#include "fsl_lpi2c_freertos.h"
#include "fsl_gpio.h"
#include "fsl_lpspi.h"
#include "fsl_lpspi_freertos.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals functional group */
/* Definition of peripheral ID */
#define COMMS_UART_PERIPHERAL LPUART1
/* Definition of the backround buffer size */
#define COMMS_UART_BACKGROUND_BUFFER_SIZE 32
/* COMMS_UART interrupt vector ID (number). */
#define COMMS_UART_IRQN LPUART1_IRQn
/* COMMS_UART interrupt vector priority. */
#define COMMS_UART_IRQ_PRIORITY 5
/* Definition of peripheral ID */
#define PWM1_PERIPHERAL PWM1
/* Definition of submodule 1 ID */
#define PWM1_SM1 kPWM_Module_1
/* Definition of clock source of submodule 1 frequency in Hertz */
#define PWM1_SM1_SM_CLK_SOURCE_FREQ_HZ 125000000U
/* Definition of submodule 1 counter clock source frequency in Hertz - PWM1_SM1_SM_CLK_SOURCE_FREQ_HZ divided by prescaler */
#define PWM1_SM1_COUNTER_CLK_SOURCE_FREQ_HZ 125000000U
/* Definition of submodule 1 counter (PWM) frequency in Hertz */
#define PWM1_SM1_COUNTER_FREQ_HZ 16001U
/* Definition of submodule 1 channel A ID */
#define PWM1_SM1_A kPWM_PwmA
/* Definition of submodule 1 channel B ID */
#define PWM1_SM1_B kPWM_PwmB
/* Definition of submodule 1 channel X ID */
#define PWM1_SM1_X kPWM_PwmX
/* Definition of submodule 2 ID */
#define PWM1_SM2 kPWM_Module_2
/* Definition of clock source of submodule 2 frequency in Hertz */
#define PWM1_SM2_SM_CLK_SOURCE_FREQ_HZ 125000000U
/* Definition of submodule 2 counter clock source frequency in Hertz - PWM1_SM2_SM_CLK_SOURCE_FREQ_HZ divided by prescaler */
#define PWM1_SM2_COUNTER_CLK_SOURCE_FREQ_HZ 1953125U
/* Definition of submodule 2 counter (PWM) frequency in Hertz */
#define PWM1_SM2_COUNTER_FREQ_HZ 50U
/* Definition of submodule 2 channel A ID */
#define PWM1_SM2_A kPWM_PwmA
/* Definition of submodule 2 channel B ID */
#define PWM1_SM2_B kPWM_PwmB
/* Definition of submodule 2 channel X ID */
#define PWM1_SM2_X kPWM_PwmX
/* Definition of submodule 3 ID */
#define PWM1_SM3 kPWM_Module_3
/* Definition of clock source of submodule 3 frequency in Hertz */
#define PWM1_SM3_SM_CLK_SOURCE_FREQ_HZ 125000000U
/* Definition of submodule 3 counter clock source frequency in Hertz - PWM1_SM3_SM_CLK_SOURCE_FREQ_HZ divided by prescaler */
#define PWM1_SM3_COUNTER_CLK_SOURCE_FREQ_HZ 1953125U
/* Definition of submodule 3 counter (PWM) frequency in Hertz */
#define PWM1_SM3_COUNTER_FREQ_HZ 50U
/* Definition of submodule 3 channel A ID */
#define PWM1_SM3_A kPWM_PwmA
/* Definition of submodule 3 channel B ID */
#define PWM1_SM3_B kPWM_PwmB
/* Definition of submodule 3 channel X ID */
#define PWM1_SM3_X kPWM_PwmX
/* Definition of fault Fault0 ID */
#define PWM1_F0_FAULT0 kPWM_Fault_0
/* Definition of fault Fault1 ID */
#define PWM1_F0_FAULT1 kPWM_Fault_1
/* Definition of fault Fault2 ID */
#define PWM1_F0_FAULT2 kPWM_Fault_2
/* Definition of fault Fault3 ID */
#define PWM1_F0_FAULT3 kPWM_Fault_3
/* BOARD_InitPeripherals defines for LPI2C1 */
/* Definition of peripheral ID */
#define LPI2C1_PERIPHERAL LPI2C1
/* Definition of clock source */
#define LPI2C1_CLOCK_FREQ 60000000UL
/* Transfer buffer size */
#define LPI2C1_MASTER_BUFFER_SIZE 16
/* Definition of slave address */
#define LPI2C1_MASTER_SLAVE_ADDRESS 0
/* GPIO1 interrupt vector ID (number). */
#define GPIO1_GPIO_COMB_0_15_IRQN GPIO1_Combined_0_15_IRQn
/* GPIO1 interrupt vector priority. */
#define GPIO1_GPIO_COMB_0_15_IRQ_PRIORITY 5
/* GPIO1 interrupt handler identifier. */
#define GPIO1_GPIO_COMB_0_15_IRQHANDLER GPIO1_Combined_0_15_IRQHandler
/* GPIO1 interrupt vector ID (number). */
#define GPIO1_GPIO_COMB_16_31_IRQN GPIO1_Combined_16_31_IRQn
/* GPIO1 interrupt vector priority. */
#define GPIO1_GPIO_COMB_16_31_IRQ_PRIORITY 5
/* GPIO1 interrupt handler identifier. */
#define GPIO1_GPIO_COMB_16_31_IRQHANDLER GPIO1_Combined_16_31_IRQHandler
/* GPIO2 interrupt vector ID (number). */
#define GPIO2_GPIO_COMB_0_15_IRQN GPIO2_Combined_0_15_IRQn
/* GPIO2 interrupt vector priority. */
#define GPIO2_GPIO_COMB_0_15_IRQ_PRIORITY 3
/* GPIO2 interrupt handler identifier. */
#define GPIO2_0_15_IRQn GPIO2_Combined_0_15_IRQHandler
/* BOARD_InitPeripherals defines for LPSPI1 */
/* Definition of peripheral ID */
#define LPSPI1_PERIPHERAL LPSPI1
/* Definition of clock source */
#define LPSPI1_CLOCK_FREQ 105600000UL
/* LPSPI1 interrupt vector ID (number). */
#define LPSPI1_IRQN LPSPI1_IRQn
/* LPSPI1 interrupt vector priority. */
#define LPSPI1_IRQ_PRIORITY 4
/* Transfer buffer size */
#define LPSPI1_BUFFER_SIZE 10

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern lpuart_rtos_handle_t COMMS_UART_rtos_handle;
extern lpuart_handle_t COMMS_UART_lpuart_handle;
extern lpuart_rtos_config_t COMMS_UART_rtos_config;
extern pwm_config_t PWM1_SM1_config;

extern pwm_signal_param_t PWM1_SM1_pwm_function_config[1];
extern pwm_config_t PWM1_SM2_config;

extern pwm_signal_param_t PWM1_SM2_pwm_function_config[2];
extern pwm_config_t PWM1_SM3_config;

extern pwm_signal_param_t PWM1_SM3_pwm_function_config[2];
extern const pwm_fault_input_filter_param_t PWM1_faultInputFilter_config;
extern const pwm_fault_param_t PWM1_Fault0_fault_config;
extern const pwm_fault_param_t PWM1_Fault1_fault_config;
extern const pwm_fault_param_t PWM1_Fault2_fault_config;
extern const pwm_fault_param_t PWM1_Fault3_fault_config;
extern const lpi2c_master_config_t LPI2C1_masterConfig;
extern lpi2c_master_transfer_t LPI2C1_masterTransfer;
extern uint8_t LPI2C1_masterBuffer[LPI2C1_MASTER_BUFFER_SIZE];
extern lpi2c_rtos_handle_t LPI2C1_masterHandle;
extern const lpspi_master_config_t LPSPI1_config;
extern lpspi_transfer_t LPSPI1_transfer;
extern lpspi_rtos_handle_t LPSPI1_handle;
extern uint8_t LPSPI1_txBuffer[LPSPI1_BUFFER_SIZE];
extern uint8_t LPSPI1_rxBuffer[LPSPI1_BUFFER_SIZE];

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
