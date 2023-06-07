//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
lpspi_master_config_t flashConfig = {0};
lpspi_transfer_t xferFlash = {0};
uint32_t flashSrcFreq = 0;
flashInfoTypedef flashInfo;

/**
  * @brief  Send a byte to the SPI Flash.
  * @note   
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */

#define EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER (kLPSPI_MasterPcs0)

/* Select USB1 PLL PFD0 (720 MHz) as lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT (1U)
/* Clock divider for master lpspi clock source */
#define EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER (7U)

void sFLASH_Init(void) {
  /* Define the init structure for the output LED pin*/
//  gpio_pin_config_t flash_CS = {
//      kGPIO_DigitalOutput, 1,
//  };
//  gpio_pin_config_t flash_SIN = {
//      kGPIO_DigitalInput, 0,
//  };
//  
//  GPIO_PinInit(GPIO1, 15u, &flash_CS);//cs
//  GPIO_PinInit(GPIO1, 0u, &flash_CS);//clk
//  GPIO_PinInit(GPIO1, 1u, &flash_CS);//sdo
//  GPIO_PinInit(GPIO1, 2u, &flash_SIN);//sdi
  
  CLOCK_SetMux(kCLOCK_LpspiMux, EXAMPLE_LPSPI_CLOCK_SOURCE_SELECT);
  CLOCK_SetDiv(kCLOCK_LpspiDiv, EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER);
  LPSPI_MasterGetDefaultConfig(&flashConfig);
  flashConfig.baudRate = 500000U;
  flashConfig.whichPcs = kLPSPI_Pcs0;
  flashSrcFreq = (CLOCK_GetFreq(kCLOCK_Usb1PllPfd0Clk) / (EXAMPLE_LPSPI_CLOCK_SOURCE_DIVIDER + 1U));
  LPSPI_MasterInit(LPSPI3, &flashConfig, flashSrcFreq);
}

/**
  * @brief  Select SPI Flash.
  * @note   
  * @param  None
  * @retval None
  */
void sFLASH_ChipSelect(uint8_t State) {
	/* Set High or low the chip select line on PD.11 pin */
	if (State == LOW)
		GPIO_PinWrite(GPIO1,15u,0);
	else
		GPIO_PinWrite(GPIO1,15u,1);
}
/**
  * @brief  Send a byte to the SPI Flash.
  * @note   
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t sFLASH_WriteByte(uint8_t byte) {
    uint8_t retVal=0;
    
    xferFlash.txData = &byte;
    xferFlash.rxData = &retVal;
    xferFlash.dataSize = 1; 
    LPSPI_MasterTransferBlocking(LPSPI3, &xferFlash);//LPSPI_MasterTransferBlocking
    return retVal;
}

/**
  * @brief  Reads a byte from the SPI Flash.
  * @note   This function must be used only if the Start_Read_Sequence function
  *         has been previously called.
  * @param  None
  * @retval Byte Read from the SPI Flash.
  */
uint8_t sFLASH_ReadByte(void)
{
  return (sFLASH_WriteByte(sFLASH_DUMMY_BYTE));
}

/**
  * @brief  Enables the write access to the FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_WriteEnable(void)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);

  /*!< Send "Write Enable" instruction */
  sFLASH_WriteByte(sFLASH_CMD_WREN);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_ChipSelect(HIGH);
}

/**
  * @brief  Polls the status of the Write In Progress (WIP) flag in the FLASH's
  *         status register and loop until write opertaion has completed.
  * @param  None
  * @retval None
  */
void sFLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);

  /*!< Send "Read Status Register" instruction */
  sFLASH_WriteByte(sFLASH_CMD_RDSR);

  /*!< Loop as long as the memory is busy with a write cycle */
  do
  {
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    flashstatus = sFLASH_WriteByte(sFLASH_DUMMY_BYTE);

  }
  while ((flashstatus & sFLASH_WIP_FLAG) == SET); /* Write in progress */

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_ChipSelect(HIGH);
}

/**
  * @brief  Erases the specified FLASH sector.
  * @param  SectorAddr: address of the sector to erase.
  * @retval None
  */
void sFLASH_EraseSector(uint32_t SectorAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);
  /*!< Send Sector Erase instruction */
  sFLASH_WriteByte(sFLASH_CMD_SE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_WriteByte((SectorAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_WriteByte((SectorAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_WriteByte(SectorAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_ChipSelect(HIGH);

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd();
}


/**
  * @brief  Erases the 64K Block FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_EraseBulk(uint32_t BlockAddr)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Sector Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);
  /*!< Send Sector Erase instruction */
  sFLASH_WriteByte(sFLASH_CMD_BE);
  /*!< Send SectorAddr high nibble address byte */
  sFLASH_WriteByte((BlockAddr & 0xFF0000) >> 16);
  /*!< Send SectorAddr medium nibble address byte */
  sFLASH_WriteByte((BlockAddr & 0xFF00) >> 8);
  /*!< Send SectorAddr low nibble address byte */
  sFLASH_WriteByte(BlockAddr & 0xFF);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_ChipSelect(HIGH);

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd();  
}

/**
  * @brief  Erases the entire FLASH.
  * @param  None
  * @retval None
  */
void sFLASH_EraseChip(void)
{
  /*!< Send write enable instruction */
  sFLASH_WriteEnable();

  /*!< Bulk Erase */
  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);
  /*!< Send Bulk Erase instruction  */
  sFLASH_WriteByte(sFLASH_CMD_CE);
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_ChipSelect(HIGH);

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes more than one byte to the FLASH with a single WRITE cycle
  *         (Page WRITE sequence).
  * @note   The number of byte can't exceed the FLASH page size.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH, must be equal
  *         or less than "sFLASH_PAGESIZE" value.
  * @retval None
  */
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  /*!< Enable the write access to the FLASH */
  sFLASH_WriteEnable();

  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);
  /*!< Send "Write to Memory " instruction */
  sFLASH_WriteByte(sFLASH_CMD_WRITE);
  /*!< Send WriteAddr high nibble address byte to write to */
  sFLASH_WriteByte((WriteAddr & 0xFF0000) >> 16);
  /*!< Send WriteAddr medium nibble address byte to write to */
  sFLASH_WriteByte((WriteAddr & 0xFF00) >> 8);
  /*!< Send WriteAddr low nibble address byte to write to */
  sFLASH_WriteByte(WriteAddr & 0xFF);

  /*!< while there is data to be written on the FLASH */
  lpspi_transfer_t masterXfer;
  masterXfer.txData   = pBuffer;
  masterXfer.rxData   = NULL;
  masterXfer.dataSize = NumByteToWrite;
  masterXfer.configFlags = EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
  LPSPI_MasterTransferBlocking(LPSPI3, &masterXfer);        
        
  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_ChipSelect(HIGH);

  /*!< Wait the end of Flash writing */
  sFLASH_WaitForWriteEnd();
}

/**
  * @brief  Writes block of data to the FLASH. In this function, the number of
  *         WRITE cycles are reduced, using Page WRITE sequence.
  * @param  pBuffer: pointer to the buffer  containing the data to be written
  *         to the FLASH.
  * @param  WriteAddr: FLASH's internal address to write to.
  * @param  NumByteToWrite: number of bytes to write to the FLASH.
  * @retval None
  */
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % sFLASH_SPI_PAGESIZE;
  count = sFLASH_SPI_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
  NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

  if (Addr == 0) /*!< WriteAddr is sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /*!< WriteAddr is not sFLASH_PAGESIZE aligned  */
  {
    if (NumOfPage == 0) /*!< NumByteToWrite < sFLASH_PAGESIZE */
    {
      if (NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > sFLASH_PAGESIZE */
      {
        temp = NumOfSingle - count;

        sFLASH_WritePage(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        sFLASH_WritePage(pBuffer, WriteAddr, temp);
      }
      else
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /*!< NumByteToWrite > sFLASH_PAGESIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sFLASH_SPI_PAGESIZE;
      NumOfSingle = NumByteToWrite % sFLASH_SPI_PAGESIZE;

      sFLASH_WritePage(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, sFLASH_SPI_PAGESIZE);
        WriteAddr +=  sFLASH_SPI_PAGESIZE;
        pBuffer += sFLASH_SPI_PAGESIZE;
      }

      if (NumOfSingle != 0)
      {
        sFLASH_WritePage(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
  * @brief  Reads a block of data from the FLASH.
  * @param  pBuffer: pointer to the buffer that receives the data read from the FLASH.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @param  NumByteToRead: number of bytes to read from the FLASH.
  * @retval None
  */
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);

  /*!< Send "Read from Memory " instruction */
  sFLASH_WriteByte(sFLASH_CMD_READ);

  /*!< Send ReadAddr high nibble address byte to read from */
  sFLASH_WriteByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte to read from */
  sFLASH_WriteByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte to read from */
  sFLASH_WriteByte(ReadAddr & 0xFF);

//  while (NumByteToRead--) /*!< while there is data to be read */
//  {
//    /*!< Read a byte from the FLASH */
//    *pBuffer = sFLASH_WriteByte(sFLASH_DUMMY_BYTE);
//    /*!< Point to the next location where the byte read will be saved */
//    pBuffer++;
//  }
  //
  lpspi_transfer_t masterXfer;
  masterXfer.txData   = NULL;
  masterXfer.rxData   = pBuffer;
  masterXfer.dataSize = NumByteToRead;
  masterXfer.configFlags =
    EXAMPLE_LPSPI_MASTER_PCS_FOR_TRANSFER | kLPSPI_MasterPcsContinuous | kLPSPI_MasterByteSwap;
  LPSPI_MasterTransferBlocking(LPSPI3, &masterXfer);

  /*!< Deselect the FLASH: Chip Select high */
  sFLASH_ChipSelect(HIGH);
}

/**
  * @brief  Reads FLASH identification.
  * @param  flashInfoTypedef 
  * @retval None
  */
void sFLASH_ReadID(flashInfoTypedef * pFlashInfo)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);

  /*!< Send "RDID " instruction */
  sFLASH_WriteByte(sFLASH_CMD_RDID);

  /*!< Read a byte from the FLASH */
  pFlashInfo->ManufacturerID = sFLASH_WriteByte(sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  pFlashInfo->DeviceID[0] = sFLASH_WriteByte(sFLASH_DUMMY_BYTE);

  /*!< Read a byte from the FLASH */
  pFlashInfo->DeviceID[1] = sFLASH_WriteByte(sFLASH_DUMMY_BYTE);

  /*!< Deselect the FLASH: Chip Select high sFLASH_CS_HIGH*/
  sFLASH_ChipSelect(HIGH);  
}

/**
  * @brief  Initiates a read data byte (READ) sequence from the Flash.
  *   This is done by driving the /CS line low to select the device, then the READ
  *   instruction is transmitted followed by 3 bytes address. This function exit
  *   and keep the /CS line low, so the Flash still being selected. With this
  *   technique the whole content of the Flash is read with a single READ instruction.
  * @param  ReadAddr: FLASH's internal address to read from.
  * @retval None
  */
void sFLASH_StartReadSequence(uint32_t ReadAddr)
{
  /*!< Select the FLASH: Chip Select low */
  sFLASH_ChipSelect(LOW);

  /*!< Send "Read from Memory " instruction */
  sFLASH_WriteByte(sFLASH_CMD_READ);

  /*!< Send the 24-bit address of the address to read from -------------------*/
  /*!< Send ReadAddr high nibble address byte */
  sFLASH_WriteByte((ReadAddr & 0xFF0000) >> 16);
  /*!< Send ReadAddr medium nibble address byte */
  sFLASH_WriteByte((ReadAddr& 0xFF00) >> 8);
  /*!< Send ReadAddr low nibble address byte */
  sFLASH_WriteByte(ReadAddr & 0xFF);
}


