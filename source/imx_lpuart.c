/*
* imx_lpuart.c
*/

/*___________________________________Includes________________________________________*/
//#include "header_files.h"
#include "fsl_lpuart.h"
#include "board.h"
//#include "imx_lpuart.h"

lpuart_handle_t  lpuart1_handle;

/*___________________________________Code____________________________________________*/
void LPUART1_UserCallback(LPUART_Type *base, lpuart_handle_t *handle, status_t status, void *userData) {
  userData = userData;
  
  if (kStatus_LPUART_TxIdle == status)
  {
    
  }
  
  //    if (kStatus_LPUART_RxIdle == status)
  //    {
  //    }
}

uint8_t testUARTdata[200];
uint8_t testUARTcnt = 0;
void LPUART1_IRQHandler(void) {
  /* If new data arrived. */
  if ((kLPUART_RxDataRegFullFlag)&LPUART_GetStatusFlags(LPUART1))
  {
    testUARTdata[testUARTcnt++] = LPUART_ReadByte(LPUART1);
  }
  SDK_ISR_EXIT_BARRIER;
}

void LPUART1_Init(uint32_t baudRate) {
  lpuart_config_t config;
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = baudRate;
  config.enableTx     = true;
  config.enableRx     = true;
  LPUART_Init(LPUART1, &config, BOARD_DebugConsoleSrcFreq());
  /* Enable RX interrupt. */
  LPUART_EnableInterrupts(LPUART1, kLPUART_RxDataRegFullInterruptEnable);
  EnableIRQ(LPUART1_IRQn);
}

void LPUART1_Send(uint8_t *data, uint32_t dataSize) {
  LPUART_WriteBlocking(LPUART1, data, dataSize);
}

void LPUART1_Send_Test(void) {
  LPUART1_Send(testUARTdata, testUARTcnt);
  testUARTcnt = 0;
}

/***********************************************************************************************************************
 * 
 **********************************************************************************************************************/



