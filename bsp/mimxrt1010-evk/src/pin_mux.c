/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v12.0
processor: MIMXRT1011xxxxx
package_id: MIMXRT1011DAE5A
mcu_data: ksdk2_0
processor_version: 12.0.0
board: MIMXRT1010-EVK
pin_labels:
- {pin_num: '57', pin_signal: GPIO_AD_03, label: 'LPSPI1_SDI/J57[10]/U27[2]', identifier: LPSPI1_SDI;PWM1_B_2}
- {pin_num: '56', pin_signal: GPIO_AD_04, label: 'LPSPI1_SDO/J57[8]/U27[5]', identifier: LPSPI1_SDO;PWM_A_2;PWM1_A_2}
- {pin_num: '55', pin_signal: GPIO_AD_05, label: 'LPSPI1_PCS0/INT1_COMBO/J56[6]/J57[6]/U26[11]/U27[1]', identifier: LPSPI1_PCS0;PWM1_B_3}
- {pin_num: '52', pin_signal: GPIO_AD_06, label: 'LPSPI1_SCK/INT2_COMBO/J56[8]/J57[12]/U26[9]/U27[6]', identifier: LPSPI1_SCK;PWM1_A_3}
- {pin_num: '74', pin_signal: GPIO_SD_02, label: 'GPIO_SD_02/BT_CFG[0]/J57[2]/TP34', identifier: GPIO_SD_02;PWM1_A_0}
power_domains: {NVCC_GPIO: '3.3'}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
#include "fsl_xbara.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 * 
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 * 
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void) {
    BOARD_InitPins();
    BOARD_InitDEBUG_UARTPins();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '55', peripheral: PWM1, signal: 'B, 3', pin_signal: GPIO_AD_05, identifier: PWM1_B_3, direction: OUTPUT}
  - {pin_num: '52', peripheral: PWM1, signal: 'A, 3', pin_signal: GPIO_AD_06, identifier: PWM1_A_3, direction: OUTPUT}
  - {pin_num: '74', peripheral: PWM1, signal: 'A, 0', pin_signal: GPIO_SD_02, identifier: PWM1_A_0, direction: OUTPUT}
  - {pin_num: '57', peripheral: PWM1, signal: 'B, 2', pin_signal: GPIO_AD_03, identifier: PWM1_B_2, direction: OUTPUT}
  - {pin_num: '56', peripheral: PWM1, signal: 'A, 2', pin_signal: GPIO_AD_04, identifier: PWM1_A_2, direction: OUTPUT}
  - {pin_num: '11', peripheral: LPI2C1, signal: SCL, pin_signal: GPIO_02}
  - {pin_num: '12', peripheral: LPI2C1, signal: SDA, pin_signal: GPIO_01, identifier: ''}
  - {pin_num: '51', peripheral: PWM1, signal: 'FAULT, 3', pin_signal: GPIO_AD_07, identifier: ''}
  - {pin_num: '51', peripheral: PWM1, signal: 'FAULT, 2', pin_signal: GPIO_AD_07, identifier: ''}
  - {pin_num: '51', peripheral: PWM1, signal: 'FAULT, 1', pin_signal: GPIO_AD_07, identifier: ''}
  - {pin_num: '51', peripheral: PWM1, signal: 'FAULT, 0', pin_signal: GPIO_AD_07, identifier: ''}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           
  CLOCK_EnableClock(kCLOCK_Xbar1);            

  IOMUXC_SetPinMux(IOMUXC_GPIO_01_LPI2C1_SDA, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_02_LPI2C1_SCL, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_03_FLEXPWM1_PWM2_B, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_04_FLEXPWM1_PWM2_A, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_05_FLEXPWM1_PWM3_B, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_06_FLEXPWM1_PWM3_A, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_07_XBAR1_INOUT03, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_02_FLEXPWM1_PWM0_A, 0U); 
  IOMUXC_GPR->GPR6 = ((IOMUXC_GPR->GPR6 &
    (~(IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_3_MASK))) 
      | IOMUXC_GPR_GPR6_IOMUXC_XBAR_DIR_SEL_3(0x00U) 
    );
  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputIomuxXbarInout03, kXBARA1_OutputFlexpwm1Fault0); 
  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputIomuxXbarInout03, kXBARA1_OutputFlexpwm1Fault1); 
  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputIomuxXbarInout03, kXBARA1_OutputFlexpwm1Fault2); 
  XBARA_SetSignalsConnection(XBARA, kXBARA1_InputIomuxXbarInout03, kXBARA1_OutputFlexpwm1Fault3); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitDEBUG_UARTPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '2', peripheral: LPUART1, signal: TXD, pin_signal: GPIO_10}
  - {pin_num: '3', peripheral: LPUART1, signal: RXD, pin_signal: GPIO_09, identifier: ''}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitDEBUG_UARTPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitDEBUG_UARTPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  IOMUXC_SetPinMux(IOMUXC_GPIO_09_LPUART1_RXD, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_10_LPUART1_TXD, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitQSPIPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '65', peripheral: FLEXSPI, signal: FLEXSPI_A_SCLK, pin_signal: GPIO_SD_10}
  - {pin_num: '66', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA0, pin_signal: GPIO_SD_09}
  - {pin_num: '68', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA1, pin_signal: GPIO_SD_07}
  - {pin_num: '67', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA2, pin_signal: GPIO_SD_08}
  - {pin_num: '64', peripheral: FLEXSPI, signal: FLEXSPI_A_DATA3, pin_signal: GPIO_SD_11}
  - {pin_num: '69', peripheral: FLEXSPI, signal: FLEXSPI_A_SS0_B, pin_signal: GPIO_SD_06}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitQSPIPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitQSPIPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_06_FLEXSPI_A_SS0_B, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_07_FLEXSPI_A_DATA1, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_08_FLEXSPI_A_DATA2, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_09_FLEXSPI_A_DATA0, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_10_FLEXSPI_A_SCLK, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_11_FLEXSPI_A_DATA3, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLEDPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '1', peripheral: GPIO1, signal: 'gpiomux_io, 11', pin_signal: GPIO_11, direction: OUTPUT, gpio_init_state: 'true', slew_rate: Slow, software_input_on: Disable,
    open_drain: Disable, drive_strength: R0_4, pull_keeper_select: Keeper, pull_keeper_enable: Enable, pull_up_down_config: Pull_Down_100K_Ohm, hysteresis_enable: Disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLEDPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLEDPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  /* GPIO configuration of GPIO_11 on GPIO_11 (pin 1) */
  gpio_pin_config_t GPIO_11_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 1U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_11 (pin 1) */
  GPIO_PinInit(GPIO1, 11U, &GPIO_11_config);

  IOMUXC_SetPinMux(IOMUXC_GPIO_11_GPIOMUX_IO11, 0U); 
  IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
    (~(BOARD_INITLEDPINS_IOMUXC_GPR_GPR26_GPIO_SEL_MASK))) 
      | IOMUXC_GPR_GPR26_GPIO_SEL(0x00U)      
    );
  IOMUXC_SetPinConfig(IOMUXC_GPIO_11_GPIOMUX_IO11, 0x10A0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitBUTTONPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '70', peripheral: GPIO2, signal: 'gpio_io, 05', pin_signal: GPIO_SD_05, direction: INPUT, gpio_interrupt: kGPIO_NoIntmode, slew_rate: Slow, software_input_on: Disable,
    open_drain: Disable, drive_strength: R0_4, pull_keeper_select: Pull, pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_100K_Ohm, hysteresis_enable: Enable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBUTTONPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBUTTONPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  /* GPIO configuration of USER_BUTTON on GPIO_SD_05 (pin 70) */
  gpio_pin_config_t USER_BUTTON_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_05 (pin 70) */
  GPIO_PinInit(GPIO2, 5U, &USER_BUTTON_config);

  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_05_GPIO2_IO05, 0U); 
  IOMUXC_GPR->GPR26 = ((IOMUXC_GPR->GPR26 &
    (~(BOARD_INITBUTTONPINS_IOMUXC_GPR_GPR26_GPIO_SEL_MASK))) 
      | IOMUXC_GPR_GPR26_GPIO_SEL(0x00U)      
    );
  IOMUXC_SetPinConfig(IOMUXC_GPIO_SD_05_GPIO2_IO05, 0x01B0A0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitCOMBO_SENSORPins:
- options: {callFromInitBoot: 'false', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '11', peripheral: LPI2C1, signal: SCL, pin_signal: GPIO_02, slew_rate: Slow, software_input_on: Enable, open_drain: Enable, drive_strength: R0_4, pull_keeper_select: Keeper,
    pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_22K_Ohm, hysteresis_enable: Disable}
  - {pin_num: '12', peripheral: LPI2C1, signal: SDA, pin_signal: GPIO_01, slew_rate: Slow, software_input_on: Enable, open_drain: Enable, drive_strength: R0_4, pull_keeper_select: Keeper,
    pull_keeper_enable: Enable, pull_up_down_config: Pull_Up_22K_Ohm, hysteresis_enable: Disable}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitCOMBO_SENSORPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitCOMBO_SENSORPins(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  IOMUXC_SetPinMux(IOMUXC_GPIO_01_LPI2C1_SDA, 1U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_02_LPI2C1_SCL, 1U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_01_LPI2C1_SDA, 0xD8A0U); 
  IOMUXC_SetPinConfig(IOMUXC_GPIO_02_LPI2C1_SCL, 0xD8A0U); 
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
