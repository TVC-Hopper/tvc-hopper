/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v11.0
processor: MIMXRT1011xxxxx
package_id: MIMXRT1011DAE5A
mcu_data: ksdk2_0
processor_version: 12.0.1
board: MIMXRT1010-EVK
functionalGroups:
- name: BOARD_InitPeripherals
  UUID: 5e519eed-bd94-4950-84de-0f29de85bcea
  called_from_default_init: true
  selectedCore: core0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system'
- global_system_definitions:
  - user_definitions: ''
  - user_includes: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'uart_cmsis_common'
- type_id: 'uart_cmsis_common_9cb8e302497aa696fdbb5a4fd622c2a8'
- global_USART_CMSIS_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'gpio_adapter_common'
- type_id: 'gpio_adapter_common_57579b9ac814fe26bf95df0a384c36b6'
- global_gpio_adapter_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * COMMS_UART initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'COMMS_UART'
- type: 'lpuart'
- mode: 'freertos'
- custom_name_enabled: 'true'
- type_id: 'lpuart_bf01db7d964092f3cf860852cba17f7e'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPUART1'
- config_sets:
  - fsl_lpuart_freertos:
    - lpuart_rtos_configuration:
      - clockSource: 'LpuartClock'
      - srcclk: 'BOARD_BootClockRUN'
      - baudrate: '115200'
      - parity: 'kLPUART_ParityDisabled'
      - stopbits: 'kLPUART_OneStopBit'
      - buffer_size: '32'
      - enableRxRTS: 'false'
      - enableTxCTS: 'false'
      - txCtsSource: 'kLPUART_CtsSourcePin'
      - txCtsConfig: 'kLPUART_CtsSampleAtStart'
    - interrupt_rx_tx:
      - IRQn: 'LPUART1_IRQn'
      - enable_priority: 'true'
      - priority: '5'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
lpuart_rtos_handle_t COMMS_UART_rtos_handle;
lpuart_handle_t COMMS_UART_lpuart_handle;
uint8_t COMMS_UART_background_buffer[COMMS_UART_BACKGROUND_BUFFER_SIZE];
lpuart_rtos_config_t COMMS_UART_rtos_config = {
  .base = COMMS_UART_PERIPHERAL,
  .baudrate = 115200UL,
  .srcclk = 80000000UL,
  .parity = kLPUART_ParityDisabled,
  .stopbits = kLPUART_OneStopBit,
  .buffer = COMMS_UART_background_buffer,
  .buffer_size = COMMS_UART_BACKGROUND_BUFFER_SIZE,
  .enableRxRTS = false,
  .enableTxCTS = false,
  .txCtsSource = kLPUART_CtsSourcePin,
  .txCtsConfig = kLPUART_CtsSampleAtStart,
};

static void COMMS_UART_init(void) {
  /* LPUART rtos initialization */
  LPUART_RTOS_Init(&COMMS_UART_rtos_handle, &COMMS_UART_lpuart_handle, &COMMS_UART_rtos_config);
  /* Interrupt vector LPUART1_IRQn priority settings in the NVIC. */
  NVIC_SetPriority(COMMS_UART_IRQN, COMMS_UART_IRQ_PRIORITY);
}

/***********************************************************************************************************************
 * NVIC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'NVIC'
- type: 'nvic'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'nvic_57b5eef3774cc60acaede6f5b8bddc67'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'NVIC'
- config_sets:
  - nvic:
    - interrupt_table:
      - 0: []
      - 1: []
      - 2: []
      - 3: []
      - 4: []
    - interrupts: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/* Empty initialization function (commented out)
static void NVIC_init(void) {
} */

/***********************************************************************************************************************
 * PWM1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'PWM1'
- type: 'pwm'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'pwm_403b1696012c269dda52095c45493820'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'PWM1'
- config_sets:
  - fsl_pwm:
    - clockSource: 'SystemClock'
    - clockSourceFreq: 'BOARD_BootClockRUN'
    - submodules:
      - 0:
        - sm: 'kPWM_Module_1'
        - sm_id: 'SM1'
        - config:
          - clockSource: 'kPWM_BusClock'
          - prescale: 'kPWM_Prescale_Divide_1'
          - pwmFreq: '16 khz'
          - pairOperation: 'kPWM_Independent'
          - operationMode: 'kPWM_EdgeAligned'
          - initializationControl: 'kPWM_Initialize_LocalSync'
          - reloadLogic: 'kPWM_ReloadPwmFullCycle'
          - reloadSelect: 'kPWM_LocalReload'
          - reloadFrequency: 'kPWM_LoadEveryOportunity'
          - forceTrigger: 'kPWM_Force_Local'
          - enableDebugMode: 'true'
          - enableWait: 'false'
          - outputTrigger_sel: ''
          - loadOK: 'true'
          - startCounter: 'true'
          - interrupt_sel: ''
          - dma_used: 'false'
          - dma:
            - pwmDMA_activate: 'false'
            - captureDMA_enable: ''
            - captureDMA_source: 'kPWM_DMARequestDisable'
            - captureDMA_watermark_control: 'kPWM_FIFOWatermarksOR'
        - channels:
          - 0:
            - channel_id: 'A'
            - functionSel: 'notUsed'
          - 1:
            - channel_id: 'B'
            - functionSel: 'pwmOutput'
            - pwm:
              - dutyCyclePercent: '0'
              - level: 'kPWM_HighTrue'
              - fault_channel0:
                - dismap: ''
              - faultState: 'kPWM_PwmFaultState0'
              - deadtime_input_by_force: 'kPWM_UsePwm'
              - clockSource: 'kPWM_BusClock'
              - deadtimeValue: '0'
              - interrupt_sel: ''
          - 2:
            - channel_id: 'X'
            - functionSel: 'notUsed'
        - common_interruptEn: 'false'
        - common_interrupt:
          - IRQn: 'PWM1_1_IRQn'
          - enable_interrrupt: 'enabled'
          - enable_priority: 'false'
          - priority: '0'
          - enable_custom_name: 'false'
      - 1:
        - sm: 'kPWM_Module_2'
        - sm_id: 'SM2'
        - config:
          - clockSource: 'kPWM_BusClock'
          - prescale: 'kPWM_Prescale_Divide_64'
          - pwmFreq: '50 Hz'
          - pairOperation: 'kPWM_Independent'
          - operationMode: 'kPWM_EdgeAligned'
          - initializationControl: 'kPWM_Initialize_LocalSync'
          - reloadLogic: 'kPWM_ReloadPwmFullCycle'
          - reloadSelect: 'kPWM_LocalReload'
          - reloadFrequency: 'kPWM_LoadEveryOportunity'
          - forceTrigger: 'kPWM_Force_Local'
          - enableDebugMode: 'true'
          - enableWait: 'false'
          - outputTrigger_sel: ''
          - loadOK: 'true'
          - startCounter: 'true'
          - interrupt_sel: ''
          - dma_used: 'false'
          - dma:
            - pwmDMA_activate: 'false'
            - captureDMA_enable: ''
            - captureDMA_source: 'kPWM_DMARequestDisable'
            - captureDMA_watermark_control: 'kPWM_FIFOWatermarksOR'
        - channels:
          - 0:
            - channel_id: 'A'
            - functionSel: 'pwmOutput'
            - pwm:
              - dutyCyclePercent: '0'
              - level: 'kPWM_HighTrue'
              - fault_channel0:
                - dismap: ''
              - faultState: 'kPWM_PwmFaultState0'
              - deadtime_input_by_force: 'kPWM_UsePwm'
              - clockSource: 'kPWM_BusClock'
              - deadtimeValue: '0'
              - interrupt_sel: ''
          - 1:
            - channel_id: 'B'
            - functionSel: 'pwmOutput'
            - pwm:
              - dutyCyclePercent: '0'
              - level: 'kPWM_HighTrue'
              - fault_channel0:
                - dismap: ''
              - faultState: 'kPWM_PwmFaultState0'
              - deadtime_input_by_force: 'kPWM_UsePwm'
              - clockSource: 'kPWM_BusClock'
              - deadtimeValue: '0'
              - interrupt_sel: ''
          - 2:
            - channel_id: 'X'
            - functionSel: 'notUsed'
        - common_interruptEn: 'false'
        - common_interrupt:
          - IRQn: 'PWM1_2_IRQn'
          - enable_interrrupt: 'enabled'
          - enable_priority: 'false'
          - priority: '0'
          - enable_custom_name: 'false'
      - 2:
        - sm: 'kPWM_Module_3'
        - sm_id: 'SM3'
        - config:
          - clockSource: 'kPWM_BusClock'
          - prescale: 'kPWM_Prescale_Divide_64'
          - pwmFreq: '50 Hz'
          - pairOperation: 'kPWM_Independent'
          - operationMode: 'kPWM_EdgeAligned'
          - initializationControl: 'kPWM_Initialize_LocalSync'
          - reloadLogic: 'kPWM_ReloadPwmFullCycle'
          - reloadSelect: 'kPWM_LocalReload'
          - reloadFrequency: 'kPWM_LoadEveryOportunity'
          - forceTrigger: 'kPWM_Force_Local'
          - enableDebugMode: 'true'
          - enableWait: 'false'
          - outputTrigger_sel: ''
          - loadOK: 'true'
          - startCounter: 'true'
          - interrupt_sel: ''
          - dma_used: 'false'
          - dma:
            - pwmDMA_activate: 'false'
            - captureDMA_enable: ''
            - captureDMA_source: 'kPWM_DMARequestDisable'
            - captureDMA_watermark_control: 'kPWM_FIFOWatermarksOR'
        - channels:
          - 0:
            - channel_id: 'A'
            - functionSel: 'pwmOutput'
            - pwm:
              - dutyCyclePercent: '0'
              - level: 'kPWM_HighTrue'
              - fault_channel0:
                - dismap: ''
              - faultState: 'kPWM_PwmFaultState0'
              - deadtime_input_by_force: 'kPWM_UsePwm'
              - clockSource: 'kPWM_BusClock'
              - deadtimeValue: '0'
              - interrupt_sel: ''
          - 1:
            - channel_id: 'B'
            - functionSel: 'pwmOutput'
            - pwm:
              - dutyCyclePercent: '0'
              - level: 'kPWM_HighTrue'
              - fault_channel0:
                - dismap: ''
              - faultState: 'kPWM_PwmFaultState0'
              - deadtime_input_by_force: 'kPWM_UsePwm'
              - clockSource: 'kPWM_BusClock'
              - deadtimeValue: '0'
              - interrupt_sel: ''
          - 2:
            - channel_id: 'X'
            - functionSel: 'notUsed'
        - common_interruptEn: 'false'
        - common_interrupt:
          - IRQn: 'PWM1_3_IRQn'
          - enable_interrrupt: 'enabled'
          - enable_priority: 'false'
          - priority: '0'
          - enable_custom_name: 'false'
    - faultChannels:
      - 0:
        - commonFaultSetting:
          - clockSource: 'kPWM_BusClock'
          - faultFilterPeriod: '1'
          - faultFilterCount: '3'
          - faultGlitchStretch: 'false'
        - faults:
          - 0:
            - fault_id: 'Fault0'
            - faultClearingMode: 'kPWM_Automatic'
            - faultLevelR: 'low'
            - enableCombinationalPathR: 'nonFiltered'
            - recoverMode: 'kPWM_NoRecovery'
            - fault_int_source: 'false'
          - 1:
            - fault_id: 'Fault1'
            - faultClearingMode: 'kPWM_Automatic'
            - faultLevelR: 'low'
            - enableCombinationalPathR: 'nonFiltered'
            - recoverMode: 'kPWM_NoRecovery'
            - fault_int_source: 'false'
          - 2:
            - fault_id: 'Fault2'
            - faultClearingMode: 'kPWM_Automatic'
            - faultLevelR: 'low'
            - enableCombinationalPathR: 'nonFiltered'
            - recoverMode: 'kPWM_NoRecovery'
            - fault_int_source: 'false'
          - 3:
            - fault_id: 'Fault3'
            - faultClearingMode: 'kPWM_Automatic'
            - faultLevelR: 'low'
            - enableCombinationalPathR: 'nonFiltered'
            - recoverMode: 'kPWM_NoRecovery'
            - fault_int_source: 'false'
    - fault_error_interruptEn: 'false'
    - fault_error_interrupt:
      - IRQn: 'PWM1_FAULT_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
/* PWM main configuration */
pwm_config_t PWM1_SM1_config = {
  .clockSource = kPWM_BusClock,
  .prescale = kPWM_Prescale_Divide_1,
  .pairOperation = kPWM_Independent,
  .initializationControl = kPWM_Initialize_LocalSync,
  .reloadLogic = kPWM_ReloadPwmFullCycle,
  .reloadSelect = kPWM_LocalReload,
  .reloadFrequency = kPWM_LoadEveryOportunity,
  .forceTrigger = kPWM_Force_Local,
  .enableDebugMode = true,
  .enableWait = false
};

pwm_signal_param_t PWM1_SM1_pwm_function_config[1]= {
  {
    .pwmChannel = kPWM_PwmB,
    .dutyCyclePercent = 0U,
    .level = kPWM_HighTrue,
    .faultState = kPWM_PwmFaultState0,
    .deadtimeValue = 0U
  },
};

pwm_config_t PWM1_SM2_config = {
  .clockSource = kPWM_BusClock,
  .prescale = kPWM_Prescale_Divide_64,
  .pairOperation = kPWM_Independent,
  .initializationControl = kPWM_Initialize_LocalSync,
  .reloadLogic = kPWM_ReloadPwmFullCycle,
  .reloadSelect = kPWM_LocalReload,
  .reloadFrequency = kPWM_LoadEveryOportunity,
  .forceTrigger = kPWM_Force_Local,
  .enableDebugMode = true,
  .enableWait = false
};

pwm_signal_param_t PWM1_SM2_pwm_function_config[2]= {
  {
    .pwmChannel = kPWM_PwmA,
    .dutyCyclePercent = 0U,
    .level = kPWM_HighTrue,
    .faultState = kPWM_PwmFaultState0,
    .deadtimeValue = 0U
  },
  {
    .pwmChannel = kPWM_PwmB,
    .dutyCyclePercent = 0U,
    .level = kPWM_HighTrue,
    .faultState = kPWM_PwmFaultState0,
    .deadtimeValue = 0U
  },
};

pwm_config_t PWM1_SM3_config = {
  .clockSource = kPWM_BusClock,
  .prescale = kPWM_Prescale_Divide_64,
  .pairOperation = kPWM_Independent,
  .initializationControl = kPWM_Initialize_LocalSync,
  .reloadLogic = kPWM_ReloadPwmFullCycle,
  .reloadSelect = kPWM_LocalReload,
  .reloadFrequency = kPWM_LoadEveryOportunity,
  .forceTrigger = kPWM_Force_Local,
  .enableDebugMode = true,
  .enableWait = false
};

pwm_signal_param_t PWM1_SM3_pwm_function_config[2]= {
  {
    .pwmChannel = kPWM_PwmA,
    .dutyCyclePercent = 0U,
    .level = kPWM_HighTrue,
    .faultState = kPWM_PwmFaultState0,
    .deadtimeValue = 0U
  },
  {
    .pwmChannel = kPWM_PwmB,
    .dutyCyclePercent = 0U,
    .level = kPWM_HighTrue,
    .faultState = kPWM_PwmFaultState0,
    .deadtimeValue = 0U
  },
};

const pwm_fault_input_filter_param_t PWM1_faultInputFilter_config = {
  .faultFilterPeriod = 1U,
  .faultFilterCount = 3U,
  .faultGlitchStretch = false
};
const pwm_fault_param_t PWM1_Fault0_fault_config = {
  .faultClearingMode = kPWM_Automatic,
  .faultLevel = false,
  .enableCombinationalPath = false,
  .recoverMode = kPWM_NoRecovery
};
const pwm_fault_param_t PWM1_Fault1_fault_config = {
  .faultClearingMode = kPWM_Automatic,
  .faultLevel = false,
  .enableCombinationalPath = false,
  .recoverMode = kPWM_NoRecovery
};
const pwm_fault_param_t PWM1_Fault2_fault_config = {
  .faultClearingMode = kPWM_Automatic,
  .faultLevel = false,
  .enableCombinationalPath = false,
  .recoverMode = kPWM_NoRecovery
};
const pwm_fault_param_t PWM1_Fault3_fault_config = {
  .faultClearingMode = kPWM_Automatic,
  .faultLevel = false,
  .enableCombinationalPath = false,
  .recoverMode = kPWM_NoRecovery
};

static void PWM1_init(void) {
  /* Initialize PWM submodule SM1 main configuration */
  PWM_Init(PWM1_PERIPHERAL, PWM1_SM1, &PWM1_SM1_config);
  /* Initialize PWM submodule SM2 main configuration */
  PWM_Init(PWM1_PERIPHERAL, PWM1_SM2, &PWM1_SM2_config);
  /* Initialize PWM submodule SM3 main configuration */
  PWM_Init(PWM1_PERIPHERAL, PWM1_SM3, &PWM1_SM3_config);
  /* Initialize fault input filter configuration */
  PWM_SetupFaultInputFilter(PWM1_PERIPHERAL, &PWM1_faultInputFilter_config);
  /* Initialize fault channel 0 fault Fault0 configuration */
  PWM_SetupFaults(PWM1_PERIPHERAL, PWM1_F0_FAULT0, &PWM1_Fault0_fault_config);
  /* Initialize fault channel 0 fault Fault1 configuration */
  PWM_SetupFaults(PWM1_PERIPHERAL, PWM1_F0_FAULT1, &PWM1_Fault1_fault_config);
  /* Initialize fault channel 0 fault Fault2 configuration */
  PWM_SetupFaults(PWM1_PERIPHERAL, PWM1_F0_FAULT2, &PWM1_Fault2_fault_config);
  /* Initialize fault channel 0 fault Fault3 configuration */
  PWM_SetupFaults(PWM1_PERIPHERAL, PWM1_F0_FAULT3, &PWM1_Fault3_fault_config);
  /* Initialize deadtime logic input for the channel B */
  PWM_SetupForceSignal(PWM1_PERIPHERAL, PWM1_SM1, PWM1_SM1_B, kPWM_UsePwm);
  /* Initialize deadtime logic input for the channel A */
  PWM_SetupForceSignal(PWM1_PERIPHERAL, PWM1_SM2, PWM1_SM2_A, kPWM_UsePwm);
  /* Initialize deadtime logic input for the channel B */
  PWM_SetupForceSignal(PWM1_PERIPHERAL, PWM1_SM2, PWM1_SM2_B, kPWM_UsePwm);
  /* Initialize deadtime logic input for the channel A */
  PWM_SetupForceSignal(PWM1_PERIPHERAL, PWM1_SM3, PWM1_SM3_A, kPWM_UsePwm);
  /* Initialize deadtime logic input for the channel B */
  PWM_SetupForceSignal(PWM1_PERIPHERAL, PWM1_SM3, PWM1_SM3_B, kPWM_UsePwm);
  /* Setup PWM output setting for submodule SM1 */
  PWM_SetupPwm(PWM1_PERIPHERAL, PWM1_SM1, PWM1_SM1_pwm_function_config, 1U, kPWM_EdgeAligned, PWM1_SM1_COUNTER_FREQ_HZ, PWM1_SM1_SM_CLK_SOURCE_FREQ_HZ);
  /* Setup PWM output setting for submodule SM2 */
  PWM_SetupPwm(PWM1_PERIPHERAL, PWM1_SM2, PWM1_SM2_pwm_function_config, 2U, kPWM_EdgeAligned, PWM1_SM2_COUNTER_FREQ_HZ, PWM1_SM2_SM_CLK_SOURCE_FREQ_HZ);
  /* Setup PWM output setting for submodule SM3 */
  PWM_SetupPwm(PWM1_PERIPHERAL, PWM1_SM3, PWM1_SM3_pwm_function_config, 2U, kPWM_EdgeAligned, PWM1_SM3_COUNTER_FREQ_HZ, PWM1_SM3_SM_CLK_SOURCE_FREQ_HZ);
  /* Initialize LDOK for update of the working registers */
  PWM_SetPwmLdok(PWM1_PERIPHERAL, (kPWM_Control_Module_1 | kPWM_Control_Module_2 | kPWM_Control_Module_3), true);
  /* Start selected counters */
  PWM_StartTimer(PWM1_PERIPHERAL, (kPWM_Control_Module_1 | kPWM_Control_Module_2 | kPWM_Control_Module_3));
}

/***********************************************************************************************************************
 * LPI2C1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'LPI2C1'
- type: 'lpi2c'
- mode: 'master'
- custom_name_enabled: 'false'
- type_id: 'lpi2c_6b71962515c3208facfccd030afebc98'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPI2C1'
- config_sets:
  - main:
    - clockSource: 'Lpi2cClock'
    - clockSourceFreq: 'BOARD_BootClockRUN'
  - interrupt_vector: []
  - master:
    - mode: 'freertos'
    - config:
      - enableMaster: 'true'
      - enableDoze: 'true'
      - debugEnable: 'true'
      - ignoreAck: 'false'
      - pinConfig: 'kLPI2C_2PinOpenDrain'
      - baudRate_Hz: '100000'
      - busIdleTimeout_ns: '0'
      - pinLowTimeout_ns: '0'
      - sdaGlitchFilterWidth_ns: '0'
      - sclGlitchFilterWidth_ns: '0'
      - hostRequest:
        - enable: 'false'
        - source: 'kLPI2C_HostRequestExternalPin'
        - polarity: 'kLPI2C_HostRequestPinActiveHigh'
      - edmaRequestSources: ''
    - transfer:
      - enable_custom_handle: 'false'
      - flags: ''
      - slaveAddress: '0'
      - direction: 'kLPI2C_Write'
      - subaddress: '0'
      - subaddressSize: '1'
      - blocking_buffer: 'false'
      - enable_custom_buffer: 'false'
      - dataSize: '16'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const lpi2c_master_config_t LPI2C1_masterConfig = {
  .enableMaster = true,
  .enableDoze = true,
  .debugEnable = true,
  .ignoreAck = false,
  .pinConfig = kLPI2C_2PinOpenDrain,
  .baudRate_Hz = 100000UL,
  .busIdleTimeout_ns = 0UL,
  .pinLowTimeout_ns = 0UL,
  .sdaGlitchFilterWidth_ns = 0U,
  .sclGlitchFilterWidth_ns = 0U,
  .hostRequest = {
    .enable = false,
    .source = kLPI2C_HostRequestExternalPin,
    .polarity = kLPI2C_HostRequestPinActiveHigh
  }
};
lpi2c_master_transfer_t LPI2C1_masterTransfer = {
  .flags = kLPI2C_TransferDefaultFlag,
  .slaveAddress = 0,
  .direction = kLPI2C_Write,
  .subaddress = 0,
  .subaddressSize = 1,
  .data = LPI2C1_masterBuffer,
  .dataSize = 16
};
lpi2c_rtos_handle_t LPI2C1_masterHandle;
uint8_t LPI2C1_masterBuffer[LPI2C1_MASTER_BUFFER_SIZE];

static void LPI2C1_init(void) {
  LPI2C_RTOS_Init(&LPI2C1_masterHandle, LPI2C1_PERIPHERAL, &LPI2C1_masterConfig, LPI2C1_CLOCK_FREQ);
}

/***********************************************************************************************************************
 * GPIO1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'GPIO1'
- type: 'igpio'
- mode: 'GPIO'
- custom_name_enabled: 'false'
- type_id: 'igpio_b1c1fa279aa7069dca167502b8589cb7'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'GPIO1'
- config_sets:
  - fsl_gpio:
    - enable_irq_comb_0_15: 'true'
    - gpio_interrupt_comb_0_15:
      - IRQn: 'GPIO1_Combined_0_15_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'true'
      - priority: '5'
      - enable_custom_name: 'false'
    - enable_irq_comb_16_31: 'true'
    - gpio_interrupt_comb_16_31:
      - IRQn: 'GPIO1_Combined_16_31_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'true'
      - priority: '5'
      - enable_custom_name: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

static void GPIO1_init(void) {
  /* Make sure, the clock gate for GPIO1 is enabled (e. g. in pin_mux.c) */
  /* Interrupt vector GPIO1_Combined_0_15_IRQn priority settings in the NVIC. */
  NVIC_SetPriority(GPIO1_GPIO_COMB_0_15_IRQN, GPIO1_GPIO_COMB_0_15_IRQ_PRIORITY);
  /* Enable interrupt GPIO1_Combined_0_15_IRQn request in the NVIC. */
  EnableIRQ(GPIO1_GPIO_COMB_0_15_IRQN);
  /* Interrupt vector GPIO1_Combined_16_31_IRQn priority settings in the NVIC. */
  NVIC_SetPriority(GPIO1_GPIO_COMB_16_31_IRQN, GPIO1_GPIO_COMB_16_31_IRQ_PRIORITY);
  /* Enable interrupt GPIO1_Combined_16_31_IRQn request in the NVIC. */
  EnableIRQ(GPIO1_GPIO_COMB_16_31_IRQN);
}

/***********************************************************************************************************************
 * GPIO2 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'GPIO2'
- type: 'igpio'
- mode: 'GPIO'
- custom_name_enabled: 'false'
- type_id: 'igpio_b1c1fa279aa7069dca167502b8589cb7'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'GPIO2'
- config_sets:
  - fsl_gpio:
    - enable_irq_comb_0_15: 'true'
    - gpio_interrupt_comb_0_15:
      - IRQn: 'GPIO2_Combined_0_15_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'true'
      - priority: '3'
      - enable_custom_name: 'true'
      - handler_custom_name: 'GPIO2_0_15_IRQn'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

static void GPIO2_init(void) {
  /* Make sure, the clock gate for GPIO2 is enabled (e. g. in pin_mux.c) */
  /* Interrupt vector GPIO2_Combined_0_15_IRQn priority settings in the NVIC. */
  NVIC_SetPriority(GPIO2_GPIO_COMB_0_15_IRQN, GPIO2_GPIO_COMB_0_15_IRQ_PRIORITY);
  /* Enable interrupt GPIO2_Combined_0_15_IRQn request in the NVIC. */
  EnableIRQ(GPIO2_GPIO_COMB_0_15_IRQN);
}

/***********************************************************************************************************************
 * LPSPI1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'LPSPI1'
- type: 'lpspi'
- mode: 'freertos'
- custom_name_enabled: 'false'
- type_id: 'lpspi_6e21a1e0a09f0a012d683c4f91752db8'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'LPSPI1'
- config_sets:
  - transfer:
    - config:
      - transmitBuffer:
        - init: 'true'
      - receiveBuffer:
        - init: 'true'
      - dataSize: '10'
      - enableTransferStruct: 'defined'
      - flags: 'kLPSPI_MasterPcs0'
  - main:
    - mode: 'kLPSPI_Master'
    - clockSource: 'LpspiClock'
    - clockSourceFreq: 'BOARD_BootClockRUN'
    - master:
      - baudRate: '500000'
      - bitsPerFrame: '8'
      - cpol: 'kLPSPI_ClockPolarityActiveHigh'
      - cpha: 'kLPSPI_ClockPhaseFirstEdge'
      - direction: 'kLPSPI_MsbFirst'
      - pcsToSckDelayInNanoSec: '1000'
      - lastSckToPcsDelayInNanoSec: '1000'
      - betweenTransferDelayInNanoSec: '1000'
      - whichPcs: 'kLPSPI_Pcs0'
      - pcsActiveHighOrLow: 'kLPSPI_PcsActiveLow'
      - pinCfg: 'kLPSPI_SdiInSdoOut'
      - dataOutConfig: 'kLpspiDataOutRetained'
    - allPcsPolarityEnable: 'false'
    - allPcsPolarity:
      - kLPSPI_Pcs1Active: 'kLPSPI_PcsActiveLow'
      - kLPSPI_Pcs2Active: 'kLPSPI_PcsActiveLow'
      - kLPSPI_Pcs3Active: 'kLPSPI_PcsActiveLow'
    - interrupt_priority:
      - IRQn: 'LPSPI1_IRQn'
      - enable_priority: 'true'
      - priority: '4'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const lpspi_master_config_t LPSPI1_config = {
  .baudRate = 500000UL,
  .bitsPerFrame = 8UL,
  .cpol = kLPSPI_ClockPolarityActiveHigh,
  .cpha = kLPSPI_ClockPhaseFirstEdge,
  .direction = kLPSPI_MsbFirst,
  .pcsToSckDelayInNanoSec = 1000UL,
  .lastSckToPcsDelayInNanoSec = 1000UL,
  .betweenTransferDelayInNanoSec = 1000UL,
  .whichPcs = kLPSPI_Pcs0,
  .pcsActiveHighOrLow = kLPSPI_PcsActiveLow,
  .pinCfg = kLPSPI_SdiInSdoOut,
  .dataOutConfig = kLpspiDataOutRetained
};
lpspi_transfer_t LPSPI1_transfer = {
  .txData = LPSPI1_txBuffer,
  .rxData = LPSPI1_rxBuffer,
  .dataSize = 10,
  .configFlags = kLPSPI_MasterPcs0
};
lpspi_rtos_handle_t LPSPI1_handle;
uint8_t LPSPI1_txBuffer[LPSPI1_BUFFER_SIZE];
uint8_t LPSPI1_rxBuffer[LPSPI1_BUFFER_SIZE];

static void LPSPI1_init(void) {
  /* Interrupt vector LPSPI1_IRQn priority settings in the NVIC. */
  NVIC_SetPriority(LPSPI1_IRQN, LPSPI1_IRQ_PRIORITY);
  LPSPI_RTOS_Init(&LPSPI1_handle, LPSPI1_PERIPHERAL, &LPSPI1_config, LPSPI1_CLOCK_FREQ);
}

/***********************************************************************************************************************
 * ADC1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'ADC1'
- type: 'adc_12b1msps_sar'
- mode: 'ADC_GENERAL'
- custom_name_enabled: 'false'
- type_id: 'adc_12b1msps_sar_6a490e886349a7b2b07bed10ce7b299b'
- functional_group: 'BOARD_InitPeripherals'
- peripheral: 'ADC1'
- config_sets:
  - fsl_adc:
    - clockConfig:
      - clockSource: 'kADC_ClockSourceAD'
      - clockSourceFreq: 'custom:10 MHz'
      - clockDriver: 'kADC_ClockDriver2'
      - samplePeriodMode: 'kADC_SamplePeriodShort2Clocks'
      - enableAsynchronousClockOutput: 'true'
    - conversionConfig:
      - resolution: 'kADC_Resolution12Bit'
      - hardwareAverageMode: 'kADC_HardwareAverageDisable'
      - enableHardwareTrigger: 'software'
      - enableHighSpeed: 'false'
      - enableLowPower: 'false'
      - enableContinuousConversion: 'false'
      - enableOverWrite: 'false'
      - enableDma: 'false'
    - resultingTime: []
    - resultCorrection:
      - doAutoCalibration: 'false'
      - offset: '0'
    - hardwareCompareConfiguration:
      - hardwareCompareMode: 'disabled'
      - value1: '0'
      - value2: '0'
    - enableInterrupt: 'false'
    - adc_interrupt:
      - IRQn: 'ADC1_IRQn'
      - enable_interrrupt: 'enabled'
      - enable_priority: 'false'
      - priority: '0'
      - enable_custom_name: 'false'
    - adc_channels_config: []
    - quick_selection: 'QS_ADC_1'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const adc_config_t ADC1_config = {
  .enableOverWrite = false,
  .enableContinuousConversion = false,
  .enableHighSpeed = false,
  .enableLowPower = false,
  .enableLongSample = false,
  .enableAsynchronousClockOutput = true,
  .referenceVoltageSource = kADC_ReferenceVoltageSourceAlt0,
  .samplePeriodMode = kADC_SamplePeriodShort2Clocks,
  .clockSource = kADC_ClockSourceAD,
  .clockDriver = kADC_ClockDriver2,
  .resolution = kADC_Resolution12Bit
};

static void ADC1_init(void) {
  /* Initialize ADC1 peripheral. */
  ADC_Init(ADC1_PERIPHERAL, &ADC1_config);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals(void)
{
  /* Initialize components */
  COMMS_UART_init();
  PWM1_init();
  LPI2C1_init();
  GPIO1_init();
  GPIO2_init();
  LPSPI1_init();
  ADC1_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals();
}
