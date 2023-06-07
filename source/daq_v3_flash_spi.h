//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_FLASH_SPI_H
#define _DAQ_V3_FLASH_SPI_H
//include-----------------------------------------------------------------------
//define------------------------------------------------------------------------
#define FLASH_CFG_START_SN              0x000000//0x40FF00//0x000F0400//0x00FF00// External Flash Block 0 - Sector 0 - Line 0
#define FLASH_CFG_START_FW              0x001000//0x40FF00//0x000F0400//0x00FF00// External Flash Block 0 - Sector 1 - Line 0
#define FLASH_CFG_START_PARAM1          0x002000//0x40FF00//0x000F0400//0x00FF00// External Flash Block 0 - Sector 2 - Line 0  
#define FLASH_PB_LOG_START              0x003000// External Flash Block 0 - Sector 3 - Line 0
#define FLASH_PB1_LOG_START             0x004000// External Flash Block 0 - Sector 4 - Line 0
#define FLASH_PB2_LOG_START             0x005000// External Flash Block 0 - Sector 5 - Line 0
#define FLASH_PB3_LOG_START             0x006000// External Flash Block 0 - Sector 6 - Line 0 
#define FLASH_PB4_LOG_START             0x007000// External Flash Block 0 - Sector 7 - Line 0 
#define FLASH_CHECK_ETH                 0x008000// External Flash Block 0 - Sector 8 - Line 0 
#define FLASH_FW_START                  0x010000// External Flash Block 1 - Sector 0 - Line 0 

#define LPSPI3_CLOCK_FREQ 105600000UL
//function----------------------------------------------------------------------

/* Exported types ------------------------------------------------------------*/
typedef struct
{
  uint8_t ManufacturerID;
  uint8_t DeviceID[2];
}flashInfoTypedef;
extern flashInfoTypedef flashInfo;

/* Exported constants --------------------------------------------------------*/

/* M25P SPI Flash supported commands */
#define sFLASH_CMD_WREN           0x06  /* Write enable instruction */
#define sFLASH_CMD_RDSR           0x05  /* Read Status Register instruction  */
#define sFLASH_CMD_WRSR           0x01  /* Write Status Register instruction */
#define sFLASH_CMD_READ           0x03  /* Read from Memory instruction */

#define sFLASH_CMD_WRITE          0x02  /* Write to Memory instruction */
#define sFLASH_CMD_SE             0x20  /* Sector Erase instruction */
#define sFLASH_CMD_BE             0xD8//0x52  /* Bulk Erase instruction */
#define sFLASH_CMD_CE             0xC7  /* Chip Erase instruction */
#define sFLASH_CMD_PP             0x02  /* Page Program instruction */
#define sFLASH_CMD_RDID           0x9F  /* Read identification */

#define sFLASH_WIP_FLAG           0x01  /* Write In Progress (WIP) flag */

#define sFLASH_DUMMY_BYTE         0xA5
#define sFLASH_SPI_PAGESIZE       0x100

#define sFLASH_M25P128_ID         0x202018
#define sFLASH_M25P64_ID          0x202017
#define sFLASH_W25Q80_ID          0xef4014
#define sFLASH_W25Q16_ID          0xef4015
#define sFLASH_W25Q32_ID          0xef4016
//#define Check_Flash_W25Q32        sFLASH_ReadID
#define LOW    	 			0x00  /* Chip Select line low */
#define HIGH    			   0x01  /* Chip Select line high */
#define SET    			   0x01  /*  */
/* M25P FLASH SPI Interface pins  */
//#define sFLASH_SPI                           SPI1
#define SEC_SIZE    0x1000
#define BLOCK_SIZE  0x10000  /* Sector size 4096*16 byte */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* High layer functions  */
void sFLASH_Init(void);
void sFLASH_EraseSector(uint32_t SectorAddr);
void sFLASH_EraseBulk(uint32_t BlockAddr);
void sFLASH_EraseChip(void);
void sFLASH_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite);
void sFLASH_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void sFLASH_ReadID(flashInfoTypedef * pFlashInfo);
void sFLASH_StartReadSequence(uint32_t ReadAddr);

/* Low layer functions */

void sFLASH_ChipSelect(uint8_t State);
uint8_t sFLASH_WriteByte(uint8_t byte);
uint8_t sFLASH_ReadByte(void);
void sFLASH_WriteEnable(void);
void sFLASH_WaitForWriteEnd(void);
uint16_t sFLASH_SendHalfWord(uint16_t HalfWord);



#endif /* _DAQ_V3_FLASH_SPI_H */