/**
 * @file debug.c
 * @brief Debugging facilities
 *
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2010-2022 Oryx Embedded SARL. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.4
 **/

//Dependencies

#include "debug.h"
#include "fsl_device_registers.h"
#include "fsl_iomuxc.h"
#include "fsl_lpuart.h"
void debugInit(uint32_t baudrate)
{
   uint32_t freq;
   lpuart_config_t lpuartConfig;

   //Enable IOMUXC clock
   CLOCK_EnableClock(kCLOCK_Iomuxc);

   //Configure GPIO_AD_B0_12 pin as LPUART1_TX
   IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_12_LPUART1_TXD, 0);

   //Set GPIO_AD_B0_12 pad properties
   IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_12_LPUART1_TXD,
      IOMUXC_SW_PAD_CTL_PAD_HYS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(0) |
      IOMUXC_SW_PAD_CTL_PAD_PKE(0) |
      IOMUXC_SW_PAD_CTL_PAD_ODE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SPEED(0) |
      IOMUXC_SW_PAD_CTL_PAD_DSE(1) |
      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

   //Configure GPIO_AD_B0_13 pin as LPUART1_RX
   IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_13_LPUART1_RXD, 0);

   //Set GPIO_AD_B0_13 pad properties
   IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_13_LPUART1_RXD,
      IOMUXC_SW_PAD_CTL_PAD_HYS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUS(2) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(1) |
      IOMUXC_SW_PAD_CTL_PAD_PKE(1) |
      IOMUXC_SW_PAD_CTL_PAD_ODE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SPEED(0) |
      IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

   //Set UART clock source (PLL3)
   CLOCK_SetMux(kCLOCK_UartMux, 0);
   //Configure UART divider to 1
   CLOCK_SetDiv(kCLOCK_UartDiv, 0);

   //Get PLL3 frequency (80MHz)
   freq = CLOCK_GetPllFreq(kCLOCK_PllUsb1) / 6;

   //Get default UART configuration
   LPUART_GetDefaultConfig(&lpuartConfig);
   //Adjust baudrate
   lpuartConfig.baudRate_Bps = baudrate;

   //Configure UART module
   LPUART_Init(LPUART1, &lpuartConfig, freq);

   //Enable transmitter and receiver
   LPUART_EnableTx(LPUART1, true);
   LPUART_EnableRx(LPUART1, true);
}

/**
 * @brief Display the contents of an array
 * @param[in] stream Pointer to a FILE object that identifies an output stream
 * @param[in] prepend String to prepend to the left of each line
 * @param[in] data Pointer to the data array
 * @param[in] length Number of bytes to display
 **/

void debugDisplayArray(FILE *stream,
   const char_t *prepend, const void *data, size_t length)
{
   uint_t i;

   //Dump the contents of the array
   for(i = 0; i < length; i++)
   {
      //Beginning of a new line?
      if((i % 16) == 0)
      {
         //TRACE_PRINTF("%s", prepend);
      }

      //Display current data byte
      //TRACE_PRINTF("%02" PRIX8 " ", *((const uint8_t *) data + i));

      //End of current line?
      if((i % 16) == 15 || i == (length - 1))
      {
         //TRACE_PRINTF("\r\n");
      }
   }
}
