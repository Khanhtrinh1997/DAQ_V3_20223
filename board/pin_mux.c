/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v11.0
processor: MIMXRT1052xxxxB
package_id: MIMXRT1052DVL6B
mcu_data: ksdk2_0
processor_version: 11.0.1
board: IMXRT1050-EVKB
pin_labels:
- {pin_num: A7, pin_signal: GPIO_EMC_40, label: DIR_RE_LPUART3, identifier: ENET_MDC;DIR_RE;DIR_RE_LPUART3}
- {pin_num: B8, pin_signal: GPIO_B0_05, label: LED_1, identifier: LCDIF_D1;LED_1}
- {pin_num: A8, pin_signal: GPIO_B0_06, label: DIR_DE_LPUART3, identifier: LCDIF_D2;DIR_DE;DIR_DE_LPUART3}
- {pin_num: A9, pin_signal: GPIO_B0_07, label: DIR_RE_1, identifier: LCDIF_D3;DIR_RE_1}
- {pin_num: B9, pin_signal: GPIO_B0_08, label: LPUART_TX, identifier: LCDIF_D4;LPUART_TX}
- {pin_num: C9, pin_signal: GPIO_B0_09, label: LPUART_RX, identifier: LCDIF_D5;LPUART3_RX}
- {pin_num: A10, pin_signal: GPIO_B0_11, label: DIR_DE_1, identifier: LCDIF_D7;DIR_DE_1}
- {pin_num: A11, pin_signal: GPIO_B1_00, label: LPUART4_TX, identifier: LCDIF_D12;LPUART_TX}
- {pin_num: B11, pin_signal: GPIO_B1_01, label: LPUART4_RX, identifier: LCDIF_D13;LPUART4_RX}
- {pin_num: D13, pin_signal: GPIO_B1_12, label: LPUART5_TX, identifier: SD_CD_SW;UART5_RXD;UART5_TXD;LPUART5_TX}
- {pin_num: D14, pin_signal: GPIO_B1_13, label: LPUART5_RX, identifier: WDOG_B;UART5_RXD;LPUART5_RX}
- {pin_num: M14, pin_signal: GPIO_AD_B0_00, label: LPSPI3_CLK, identifier: LPSPI3_CLK}
- {pin_num: H10, pin_signal: GPIO_AD_B0_01, label: LPSPI3_SDO, identifier: LPSPI3_SDO}
- {pin_num: M11, pin_signal: GPIO_AD_B0_02, label: LPSPI3_SDI, identifier: LPSPI3_SDI}
- {pin_num: L10, pin_signal: GPIO_AD_B0_15, label: LPSPI3_CS, identifier: CAN2_RX;LPSPI3_CS}
- {pin_num: H13, pin_signal: GPIO_AD_B1_08, label: DIR_LPUART5, identifier: CSI_D9;DIR_LPUART5}
- {pin_num: M5, pin_signal: GPIO_SD_B1_01, label: BTN_DEFAULT, identifier: FlexSPI_D2_B;BTN_DEFAULT}
- {pin_num: M4, pin_signal: GPIO_SD_B1_03, label: LED_RUN, identifier: FlexSPI_D0_B;LED_RUN}
- {pin_num: P4, pin_signal: GPIO_SD_B1_10, label: BT_DEFAULT, identifier: FlexSPI_D2_A;BT_DEFAULT}
- {pin_num: P5, pin_signal: GPIO_SD_B1_11, label: LED_RUN, identifier: FlexSPI_D3_A;LED_RUN}
- {pin_num: K12, pin_signal: GPIO_AD_B1_05, label: DIR_LPUART5, identifier: CSI_MCLK;DIR_LPUART5}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

#include "fsl_common.h"
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
    BOARD_InitSDRAM();
    BOARD_InitLPUART3_modbusAB();
    BOARD_InitGPIO();
    BOARD_InitLPUART4_DCCabinetAB();
    BOAD_InitFLASH();
    BOAD_InitLPUART5_DCCabinet232();
}

/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitSDRAM:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitSDRAM
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitSDRAM(void) {
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLPUART3_modbusAB:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: C9, peripheral: LPUART3, signal: RX, pin_signal: GPIO_B0_09, identifier: LPUART3_RX}
  - {pin_num: A8, peripheral: GPIO2, signal: 'gpio_io, 06', pin_signal: GPIO_B0_06, identifier: DIR_DE_LPUART3, direction: OUTPUT}
  - {pin_num: A7, peripheral: GPIO3, signal: 'gpio_io, 26', pin_signal: GPIO_EMC_40, identifier: DIR_RE_LPUART3, direction: OUTPUT}
  - {pin_num: B9, peripheral: LPUART3, signal: TX, pin_signal: GPIO_B0_08, identifier: LPUART_TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLPUART3_modbusAB
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLPUART3_modbusAB(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  /* GPIO configuration of DIR_DE_LPUART3 on GPIO_B0_06 (pin A8) */
  gpio_pin_config_t DIR_DE_LPUART3_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_B0_06 (pin A8) */
  GPIO_PinInit(GPIO2, 6U, &DIR_DE_LPUART3_config);

  /* GPIO configuration of DIR_RE_LPUART3 on GPIO_EMC_40 (pin A7) */
  gpio_pin_config_t DIR_RE_LPUART3_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_EMC_40 (pin A7) */
  GPIO_PinInit(GPIO3, 26U, &DIR_RE_LPUART3_config);

  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_GPIO2_IO06, 0U); 
#if FSL_IOMUXC_DRIVER_VERSION >= MAKE_VERSION(2, 0, 3)
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LPUART3_TXD, 0U); 
#else
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LPUART3_TX, 0U); 
#endif
#if FSL_IOMUXC_DRIVER_VERSION >= MAKE_VERSION(2, 0, 3)
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LPUART3_RXD, 0U); 
#else
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LPUART3_RX, 0U); 
#endif
  IOMUXC_SetPinMux(IOMUXC_GPIO_EMC_40_GPIO3_IO26, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitGPIO:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B8, peripheral: GPIO2, signal: 'gpio_io, 05', pin_signal: GPIO_B0_05, identifier: LED_1, direction: OUTPUT}
  - {pin_num: M5, peripheral: GPIO3, signal: 'gpio_io, 01', pin_signal: GPIO_SD_B1_01, identifier: BTN_DEFAULT, direction: INPUT}
  - {pin_num: M4, peripheral: GPIO3, signal: 'gpio_io, 03', pin_signal: GPIO_SD_B1_03, identifier: LED_RUN, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitGPIO
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitGPIO(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  /* GPIO configuration of LED_1 on GPIO_B0_05 (pin B8) */
  gpio_pin_config_t LED_1_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_B0_05 (pin B8) */
  GPIO_PinInit(GPIO2, 5U, &LED_1_config);

  /* GPIO configuration of BTN_DEFAULT on GPIO_SD_B1_01 (pin M5) */
  gpio_pin_config_t BTN_DEFAULT_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_B1_01 (pin M5) */
  GPIO_PinInit(GPIO3, 1U, &BTN_DEFAULT_config);

  /* GPIO configuration of LED_RUN on GPIO_SD_B1_03 (pin M4) */
  gpio_pin_config_t LED_RUN_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_SD_B1_03 (pin M4) */
  GPIO_PinInit(GPIO3, 3U, &LED_RUN_config);

  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_GPIO2_IO05, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_01_GPIO3_IO01, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_SD_B1_03_GPIO3_IO03, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitLPUART4_DCCabinetAB:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: B11, peripheral: LPUART4, signal: RX, pin_signal: GPIO_B1_01, identifier: LPUART4_RX}
  - {pin_num: A9, peripheral: GPIO2, signal: 'gpio_io, 07', pin_signal: GPIO_B0_07, identifier: DIR_RE_1, direction: OUTPUT}
  - {pin_num: A10, peripheral: GPIO2, signal: 'gpio_io, 11', pin_signal: GPIO_B0_11, identifier: DIR_DE_1, direction: OUTPUT}
  - {pin_num: A11, peripheral: LPUART4, signal: TX, pin_signal: GPIO_B1_00, identifier: LPUART_TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitLPUART4_DCCabinetAB
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitLPUART4_DCCabinetAB(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  /* GPIO configuration of DIR_RE_1 on GPIO_B0_07 (pin A9) */
  gpio_pin_config_t DIR_RE_1_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_B0_07 (pin A9) */
  GPIO_PinInit(GPIO2, 7U, &DIR_RE_1_config);

  /* GPIO configuration of DIR_DE_1 on GPIO_B0_11 (pin A10) */
  gpio_pin_config_t DIR_DE_1_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_B0_11 (pin A10) */
  GPIO_PinInit(GPIO2, 11U, &DIR_DE_1_config);

  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_GPIO2_IO07, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_GPIO2_IO11, 0U); 
#if FSL_IOMUXC_DRIVER_VERSION >= MAKE_VERSION(2, 0, 3)
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LPUART4_TXD, 0U); 
#else
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LPUART4_TX, 0U); 
#endif
#if FSL_IOMUXC_DRIVER_VERSION >= MAKE_VERSION(2, 0, 3)
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LPUART4_RXD, 0U); 
#else
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LPUART4_RX, 0U); 
#endif
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOAD_InitFLASH:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: H10, peripheral: LPSPI3, signal: SDO, pin_signal: GPIO_AD_B0_01}
  - {pin_num: M11, peripheral: LPSPI3, signal: SDI, pin_signal: GPIO_AD_B0_02}
  - {pin_num: M14, peripheral: LPSPI3, signal: SCK, pin_signal: GPIO_AD_B0_00}
  - {pin_num: L10, peripheral: GPIO1, signal: 'gpio_io, 15', pin_signal: GPIO_AD_B0_15, identifier: LPSPI3_CS, direction: OUTPUT}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOAD_InitFLASH
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOAD_InitFLASH(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

  /* GPIO configuration of LPSPI3_CS on GPIO_AD_B0_15 (pin L10) */
  gpio_pin_config_t LPSPI3_CS_config = {
      .direction = kGPIO_DigitalOutput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
  };
  /* Initialize GPIO functionality on GPIO_AD_B0_15 (pin L10) */
  GPIO_PinInit(GPIO1, 15U, &LPSPI3_CS_config);

  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_00_LPSPI3_SCK, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_01_LPSPI3_SDO, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_02_LPSPI3_SDI, 0U); 
  IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, 0U); 
}


/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOAD_InitLPUART5_DCCabinet232:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: D14, peripheral: LPUART5, signal: RX, pin_signal: GPIO_B1_13, identifier: LPUART5_RX}
  - {pin_num: D13, peripheral: LPUART5, signal: TX, pin_signal: GPIO_B1_12, identifier: LPUART5_TX}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOAD_InitLPUART5_DCCabinet232
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOAD_InitLPUART5_DCCabinet232(void) {
  CLOCK_EnableClock(kCLOCK_Iomuxc);           

#if FSL_IOMUXC_DRIVER_VERSION >= MAKE_VERSION(2, 0, 3)
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_LPUART5_TXD, 0U); 
#else
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_12_LPUART5_TX, 0U); 
#endif
#if FSL_IOMUXC_DRIVER_VERSION >= MAKE_VERSION(2, 0, 3)
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_LPUART5_RXD, 0U); 
#else
  IOMUXC_SetPinMux(IOMUXC_GPIO_B1_13_LPUART5_RX, 0U); 
#endif
}

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
