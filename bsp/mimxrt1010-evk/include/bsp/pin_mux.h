/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/

/*! @brief Direction type  */
typedef enum _pin_mux_direction
{
  kPIN_MUX_DirectionInput = 0U,         /* Input direction */
  kPIN_MUX_DirectionOutput = 1U,        /* Output direction */
  kPIN_MUX_DirectionInputOrOutput = 2U  /* Input or output direction */
} pin_mux_direction_t;

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

/* GPIO_02 (number 11), I2C1_SCL/U10[17]/J57[20]/U26[4] */
/* Routed pin properties */
#define BOARD_INITPINS_I2C1_SCL_PERIPHERAL                                LPI2C1   /*!< Peripheral name */
#define BOARD_INITPINS_I2C1_SCL_SIGNAL                                       SCL   /*!< Signal name */

/* GPIO_03 (number 10), SAI1_RXD0/U10[16] */
/* Routed pin properties */
#define BOARD_INITPINS_SAI1_RXD0_PERIPHERAL                                 PWM1   /*!< Peripheral name */
#define BOARD_INITPINS_SAI1_RXD0_SIGNAL                                        B   /*!< Signal name */
#define BOARD_INITPINS_SAI1_RXD0_CHANNEL                                      1U   /*!< Signal channel */

/* GPIO_06 (number 6), SAI1_TX_BCLK/U10[12] */
/* Routed pin properties */
#define BOARD_INITPINS_SAI1_TX_BCLK_PERIPHERAL                              PWM1   /*!< Peripheral name */
#define BOARD_INITPINS_SAI1_TX_BCLK_SIGNAL                                     A   /*!< Signal name */
#define BOARD_INITPINS_SAI1_TX_BCLK_CHANNEL                                   2U   /*!< Signal channel */

/* GPIO_05 (number 8), SAI1_TXD1 */
/* Routed pin properties */
#define BOARD_INITPINS_SAI1_TXD1_PERIPHERAL                                 PWM1   /*!< Peripheral name */
#define BOARD_INITPINS_SAI1_TXD1_SIGNAL                                        B   /*!< Signal name */
#define BOARD_INITPINS_SAI1_TXD1_CHANNEL                                      2U   /*!< Signal channel */

/* GPIO_AD_07 (number 51), SAI1_TX_SYNC/U10[13] */
/* Routed pin properties */
#define BOARD_INITPINS_ADC12_3_PERIPHERAL                                   PWM1   /*!< Peripheral name */
#define BOARD_INITPINS_ADC12_3_SIGNAL                                      FAULT   /*!< Signal name */
#define BOARD_INITPINS_ADC12_3_CHANNEL                                        0U   /*!< Signal channel */

/* GPIO_AD_06 (number 52), LPSPI1_SCK/INT2_COMBO/J56[8]/J57[12]/U26[9]/U27[6] */
/* Routed pin properties */
#define BOARD_INITPINS_PWM1_A_3_PERIPHERAL                                  PWM1   /*!< Peripheral name */
#define BOARD_INITPINS_PWM1_A_3_SIGNAL                                         A   /*!< Signal name */
#define BOARD_INITPINS_PWM1_A_3_CHANNEL                                       3U   /*!< Signal channel */

/* GPIO_AD_05 (number 55), LPSPI1_PCS0/INT1_COMBO/J56[6]/J57[6]/U26[11]/U27[1] */
/* Routed pin properties */
#define BOARD_INITPINS_PWM1_B_3_PERIPHERAL                                  PWM1   /*!< Peripheral name */
#define BOARD_INITPINS_PWM1_B_3_SIGNAL                                         B   /*!< Signal name */
#define BOARD_INITPINS_PWM1_B_3_CHANNEL                                       3U   /*!< Signal channel */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

/* GPIO_10 (number 2), LPUART1_TXD/J56[4] */
/* Routed pin properties */
#define BOARD_INITDEBUG_UARTPINS_UART1_TXD_PERIPHERAL                    LPUART1   /*!< Peripheral name */
#define BOARD_INITDEBUG_UARTPINS_UART1_TXD_SIGNAL                            TXD   /*!< Signal name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitDEBUG_UARTPins(void);

/* GPIO_SD_10 (number 65), FlexSPI_CLK_A/U13[6] */
/* Routed pin properties */
#define BOARD_INITQSPIPINS_FlexSPI_CLK_PERIPHERAL                        FLEXSPI   /*!< Peripheral name */
#define BOARD_INITQSPIPINS_FlexSPI_CLK_SIGNAL                     FLEXSPI_A_SCLK   /*!< Signal name */

/* GPIO_SD_09 (number 66), FlexSPI_D0_A/U13[5] */
/* Routed pin properties */
#define BOARD_INITQSPIPINS_FlexSPI_D0_A_PERIPHERAL                       FLEXSPI   /*!< Peripheral name */
#define BOARD_INITQSPIPINS_FlexSPI_D0_A_SIGNAL                   FLEXSPI_A_DATA0   /*!< Signal name */

/* GPIO_SD_07 (number 68), FlexSPI_D1_A/U13[2] */
/* Routed pin properties */
#define BOARD_INITQSPIPINS_FlexSPI_D1_A_PERIPHERAL                       FLEXSPI   /*!< Peripheral name */
#define BOARD_INITQSPIPINS_FlexSPI_D1_A_SIGNAL                   FLEXSPI_A_DATA1   /*!< Signal name */

/* GPIO_SD_08 (number 67), FlexSPI_D2_A/U13[3] */
/* Routed pin properties */
#define BOARD_INITQSPIPINS_FlexSPI_D2_A_PERIPHERAL                       FLEXSPI   /*!< Peripheral name */
#define BOARD_INITQSPIPINS_FlexSPI_D2_A_SIGNAL                   FLEXSPI_A_DATA2   /*!< Signal name */

/* GPIO_SD_11 (number 64), FlexSPI_D3_A/U13[7] */
/* Routed pin properties */
#define BOARD_INITQSPIPINS_FlexSPI_D3_A_PERIPHERAL                       FLEXSPI   /*!< Peripheral name */
#define BOARD_INITQSPIPINS_FlexSPI_D3_A_SIGNAL                   FLEXSPI_A_DATA3   /*!< Signal name */

/* GPIO_SD_06 (number 69), FlexSPI_SS0/U13[1] */
/* Routed pin properties */
#define BOARD_INITQSPIPINS_FlexSPI_SS0_PERIPHERAL                        FLEXSPI   /*!< Peripheral name */
#define BOARD_INITQSPIPINS_FlexSPI_SS0_SIGNAL                    FLEXSPI_A_SS0_B   /*!< Signal name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitQSPIPins(void);

#define BOARD_INITLEDPINS_IOMUXC_GPR_GPR26_GPIO_SEL_MASK 0x0800U /*!< Select GPIO1 or GPIO2: affected bits mask */

/* GPIO_11 (number 1), GPIO_11 */
/* Routed pin properties */
#define BOARD_INITLEDPINS_GPIO_11_PERIPHERAL                               GPIO1   /*!< Peripheral name */
#define BOARD_INITLEDPINS_GPIO_11_SIGNAL                              gpiomux_io   /*!< Signal name */
#define BOARD_INITLEDPINS_GPIO_11_CHANNEL                                    11U   /*!< Signal channel */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitLEDPins(void);

#define BOARD_INITBUTTONPINS_IOMUXC_GPR_GPR26_GPIO_SEL_MASK 0x20U /*!< Select GPIO1 or GPIO2: affected bits mask */

/* GPIO_SD_05 (number 70), USER_BUTTON */
/* Routed pin properties */
#define BOARD_INITBUTTONPINS_USER_BUTTON_PERIPHERAL                        GPIO2   /*!< Peripheral name */
#define BOARD_INITBUTTONPINS_USER_BUTTON_SIGNAL                          gpio_io   /*!< Signal name */
#define BOARD_INITBUTTONPINS_USER_BUTTON_CHANNEL                              5U   /*!< Signal channel */

/* Symbols to be used with GPIO driver */
#define BOARD_INITBUTTONPINS_USER_BUTTON_GPIO                              GPIO2   /*!< GPIO peripheral base pointer */
#define BOARD_INITBUTTONPINS_USER_BUTTON_GPIO_PIN                             5U   /*!< GPIO pin number */
#define BOARD_INITBUTTONPINS_USER_BUTTON_GPIO_PIN_MASK                (1U << 5U)   /*!< GPIO pin mask */
#define BOARD_INITBUTTONPINS_USER_BUTTON_PORT                              GPIO2   /*!< PORT peripheral base pointer */
#define BOARD_INITBUTTONPINS_USER_BUTTON_PIN                                  5U   /*!< PORT pin number */
#define BOARD_INITBUTTONPINS_USER_BUTTON_PIN_MASK                     (1U << 5U)   /*!< PORT pin mask */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitBUTTONPins(void);

/* GPIO_02 (number 11), I2C1_SCL/U10[17]/J57[20]/U26[4] */
/* Routed pin properties */
#define BOARD_INITCOMBO_SENSORPINS_I2C1_SCL_PERIPHERAL                    LPI2C1   /*!< Peripheral name */
#define BOARD_INITCOMBO_SENSORPINS_I2C1_SCL_SIGNAL                           SCL   /*!< Signal name */

/* GPIO_01 (number 12), I2C1_SDA/U10[18]/J57[18]/U26[6] */
/* Routed pin properties */
#define BOARD_INITCOMBO_SENSORPINS_I2C1_SDA_PERIPHERAL                    LPI2C1   /*!< Peripheral name */
#define BOARD_INITCOMBO_SENSORPINS_I2C1_SDA_SIGNAL                           SDA   /*!< Signal name */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitCOMBO_SENSORPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
