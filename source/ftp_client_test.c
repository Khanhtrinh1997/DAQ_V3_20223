/*
 * .c
 */

#include "ftp/ftp_client.h"
#include "ftp_client_test.h"
#include "daq_v3_flash_spi.h"
#include "crc32_cal.h"
#include "header_files.h"

uint32_t totalByteToWrite, byteToWrite;
error_t TestFTPClient(uint32_t *pCRC32) {
   error_t error;
   static uint8_t buffer[1024];
   memset(buffer, 0, sizeof(buffer));
   byteToWrite = 0;
   totalByteToWrite = 0;
   
   /// Test connect to FTP Server.
   IpAddr ipAddr;
   static FtpClientContext ftpContext;
   memset(&ftpContext, 0, sizeof(ftpContext));
   ftpClientInit(&ftpContext);
   error = getHostByName(NULL, private_mib_base_struct.cfgNetworkGroup.cFTPServer, &ipAddr, 0); //thanhcm need edit--done
   if (error) return error;
   
   error = ftpClientConnect(&ftpContext, &ipAddr, private_mib_base_struct.cfgNetworkGroup.ftp_port, FTP_NO_SECURITY | FTP_PASSIVE_MODE); //thanhcm need edit--done
   if (error) return error;
   
   error = ftpClientLogin(&ftpContext, "qltunguon", "vtnet@2019");
   if (error) {
      ftpClientClose(&ftpContext);
      return error;
   }
   
   /// Get firmware crc32.

   switch(config_handler_struct.state_update){
   case _DAQ_V3_FW_:
     {
       error = ftpClientOpenFile(&ftpContext, "DAQV3_IMXRT105.hex", FTP_FOR_READING | FTP_BINARY_TYPE);   
     }
     break;
   case _VAC_FW_:
     {
       error = ftpClientOpenFile(&ftpContext, "MKE06_VAC03_6000_srec.hex", FTP_FOR_READING | FTP_BINARY_TYPE);
     }
     break;
   default:
     break;
   }
   
   //check error----------------------------------------------------------------
   if (error) {
      ftpClientClose(&ftpContext);
      return error;
   }
   
   /// Download & save firmware.
   crc32cal_Init();
   while (1) {
     error = ftpReadFile(&ftpContext, buffer, sizeof(buffer) - 1, &byteToWrite, 0);
     if (error) break;
     sFLASH_WriteBuffer(buffer, totalByteToWrite + FLASH_FW_START, byteToWrite);
     crc32cal_WriteData(buffer, byteToWrite);
     totalByteToWrite += byteToWrite;
   }  
   
   // Test Check CRC23
   /*
   uint32_t cycle = totalByteToWrite/1024;
   uint32_t byteRemain = totalByteToWrite%1024;
   uint32_t fAddr = FLASH_FW_START;
   crc32cal_Init();
   
   while (cycle--) {      
     sFLASH_ReadBuffer(buffer, fAddr, 1024);
     crc32cal_WriteData(buffer, 1024);
     fAddr += 1024;
   }
   sFLASH_ReadBuffer(buffer, fAddr, byteRemain);
   crc32cal_WriteData(buffer, byteRemain);
   *pCRC32 = crc32cal_Get32bitResult();
   */
   
   if (error == ERROR_TIMEOUT) {
     ftpClientCloseFile(&ftpContext);   
     ftpClientClose(&ftpContext);
     return ERROR_TIMEOUT;
   }
   else {
     *pCRC32 = crc32cal_Get32bitResult();
     config_handler_struct.total_byte_to_write = totalByteToWrite;
     error = ftpClientCloseFile(&ftpContext);   
     return ftpClientClose(&ftpContext);
   }
}
