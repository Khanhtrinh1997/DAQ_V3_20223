//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
uint32_t crc32FTP;
extern error_t TestFTPClient(uint32_t *pCRC32);
error_t ftpTransferErr;
uint8_t flagUpdateFW[2]={0};
uint8_t flag;
//control task -------------------------------------------
void control_task(void* pv_parameters){
  private_mib_base_struct.cntGroup.task.control = 0;
  system_flag_struct.cnt_reboot =0;
  while(true){
    private_mib_base_struct.cntGroup.task.control++;
    

    //user config save --------------
    user_config_save();
    //user write crc32---------------
    user_write_crc32();
    //user reboot--------------------
    user_reboot();
    //bnt factory default-----------
    bnt_factory_default();
    //user update-------------------
    user_update();
    
//    if(flag == 11){
//      working_default_parameter_2.u8UserSerialNo[0] ='D';
//      working_default_parameter_2.u8UserSerialNo[1] ='A';
//      working_default_parameter_2.u8UserSerialNo[2] ='Q';
//      working_default_parameter_2.u8UserSerialNo[3] ='V';
//      working_default_parameter_2.u8UserSerialNo[4] ='3';
//      working_default_parameter_2.u8UserSerialNo[5] ='2';
//      working_default_parameter_2.u8UserSerialNo[6] ='2';
//      working_default_parameter_2.u8UserSerialNo[7] ='0';
//      working_default_parameter_2.u8UserSerialNo[8] ='5';
//      working_default_parameter_2.u8UserSerialNo[9] ='T';
//      working_default_parameter_2.u8UserSerialNo[10] ='N';
//      working_default_parameter_2.u8UserSerialNo[11] ='0';
//      working_default_parameter_2.u8UserSerialNo[12] ='0';
//      working_default_parameter_2.u8UserSerialNo[13] ='0';
//      working_default_parameter_2.u8UserSerialNo[14] ='1';
//      //parameter2.u8UserSerialNo[15] ='0';
//      system_flag_struct.u8IsRewriteSN =1;
//      
//      flag =0;  
//    }
//    
//    if(flag == 12){
//      sFLASH_EraseChip();
//      flag =0;
//    }
   
    
    
    vTaskDelay(50);
  }
}

//blink led user ---------------------------------------------------------------
void blink_led_user(){
  if(system_flag_struct.led_user_blink == true){
    GPIO_PinWrite(GPIO3, 3U,1);
    if(system_flag_struct.eth_type == DP83825 ){
      GPIO_PinWrite(GPIO2, 5U,1);
    }
    system_flag_struct.led_user_blink = false;
  }
  else
  {
    GPIO_PinWrite(GPIO3, 3U,0);
    if(system_flag_struct.eth_type == DP83825 ){
      GPIO_PinWrite(GPIO2, 5U,0);
    }
    system_flag_struct.led_user_blink = true;
  }
}

//user config save-------------------------------------------------------------
void user_config_save(){
  if ((flag_struct.save_config_flag & 0x01) == 0x01)
  {
    config_save();  
    flag_struct.save_config_flag &= ~(0x01);   
  }
}
// user_write_crc32-------------------------------------------------------------
void user_write_crc32(){
  if (system_flag_struct.u8IsRewriteSN == 1)
  {
    parameter2.u8IsWritten       = 0x53;
    working_default_parameter_2.u8IsWritten = 0x53;
    sFLASH_EraseSector(FLASH_CFG_START_SN);
    sFLASH_WritePage((uint8_t *)&working_default_parameter_2, FLASH_CFG_START_SN, sizeof(SYSTEM_CONFIG_STRUCT_2));
    vTaskDelay(1000);
    system_flag_struct.u8IsRewriteSN = 0;
  }
  
}
// user_reboot------------------------------------------------------------------
void user_reboot(){
  if (system_flag_struct.u8IsRebootFlag == 1)
  {
    if(system_flag_struct.cnt_reboot++>=5){
      private_mib_base_struct.siteGroup.siteSystemReset = 0x53;
      system_flag_struct.u8IsRebootFlag = 0;
      system_flag_struct.cnt_reboot =0;
    }
  }
}

//user_update-------------------------------------------------------------------
typedef struct
{
  uint16_t tokenCnt;
  uint8_t line[NUM_BYTE_OF_S19_LINE]; 
  uint8_t bytePerLine;
  uint8_t eol;
  uint16_t linePerFile;
  uint32_t totalByteToSend;
  int32_t remainByteToSend;
  uint32_t sentByte;
}tS19FileParser;

tS19FileParser s19FileParser;  

uint16_t s19FileParse(uint32_t startAddr)
{
  uint8_t data[60];
  sFLASH_ReadBuffer(&data[0], startAddr, NUM_BYTE_OF_S19_LINE); // doc 53 byte tu startAddr --> data
  s19FileParser.eol = 0;
  s19FileParser.tokenCnt = 0;
  s19FileParser.bytePerLine = 0;
  for (uint16_t i = 0; i < NUM_BYTE_OF_S19_LINE; i++)
  {
    switch(data[i])
    {
    case 'S':
      {        
        s19FileParser.tokenCnt++;
        if (s19FileParser.tokenCnt > 1)
        {
          s19FileParser.eol = 1;          
        }
        else
        {
          s19FileParser.line[i] = data[i];
        }
      }
      break;
    default:
      {
        s19FileParser.line[i] = data[i];
      }
      break;
    };
    if (s19FileParser.eol == 1)
    {
      s19FileParser.bytePerLine = i;
      break;
    }
  }  
  return s19FileParser.bytePerLine;
}

  
void user_update(){
  if (config_handler_struct.u8FwUpdateFlag == 1) { //thanhcm need edit
    for (int i = 0; i < 200; i++) sFLASH_EraseBulk(FLASH_FW_START + i*BLOCK_SIZE);
    ftpTransferErr = TestFTPClient(&crc32FTP);
    if (ftpTransferErr == NO_ERROR) {
      
      switch(config_handler_struct.state_update){
      case _DAQ_V3_FW_://_DAQ_FW_--------------------------------------------
        {
          uint8_t flagUpdateFW[2];
          flagUpdateFW[0] = 0xAA;
          flagUpdateFW[1] = 0xAA;
          sFLASH_EraseSector(FLASH_CFG_START_FW);
          sFLASH_WriteBuffer(flagUpdateFW, FLASH_CFG_START_FW, 2);
          vTaskDelay(1000);
          
          parameter2.u8IsWritten       = 0x53;
          private_mib_base_struct.siteGroup.siteFirmwareCRC = crc32FTP;
          parameter2.u32FirmwareCRC    = private_mib_base_struct.siteGroup.siteFirmwareCRC;
          working_default_parameter_2  = parameter2;
          sFLASH_EraseSector(FLASH_CFG_START_SN);
          sFLASH_WritePage((uint8_t *)&working_default_parameter_2, FLASH_CFG_START_SN, sizeof(SYSTEM_CONFIG_STRUCT_2));
          vTaskDelay(1000);
          
          NVIC_SystemReset();        
        }
        break;
      case _VAC_FW_://_VAC_FW_-----------------------------------------------
        {
          
          modbus_rtu_struct.modbus_state = 0;
          modbus_rtu_struct.data_pointer = 0; 
          //SetVar_i16VACUpdate(35, 100, 1);
          mater_write_i16(&modbus_rtu_struct,&modbus_manager_struct,35,100,1,_VAC_SLAVE,4500);// gui 3 lan
          
          init_rs485_modbus_rtu(BAUD_RATE_115200,NONE_PARITY);            
          s19FileParser.totalByteToSend = config_handler_struct.total_byte_to_write;
          s19FileParser.remainByteToSend = s19FileParser.totalByteToSend;
          s19FileParser.bytePerLine = 0;
          s19FileParser.sentByte = 0;
          s19FileParser.linePerFile = 0;
          vTaskDelay(12000);
          while(s19FileParser.remainByteToSend > 0)
          {
            
            DIR_MODBUS_AB_ON
            s19FileParse(FLASH_FW_START + s19FileParser.sentByte);
            s19FileParser.linePerFile += 1;
            if (s19FileParser.bytePerLine == 0)
            {
              s19FileParser.bytePerLine = s19FileParser.remainByteToSend;
            }
            
            LPUART_WriteBlocking(LPUART3, &s19FileParser.line[0], s19FileParser.bytePerLine);
            
            s19FileParser.sentByte += s19FileParser.bytePerLine;
            s19FileParser.remainByteToSend = s19FileParser.totalByteToSend - s19FileParser.sentByte;
            vTaskDelay(15);
            
          }
          vTaskDelay(1000);
          DIR_MODBUS_AB_OFF
          vTaskDelay(5000);
          modbus_rtu_struct.modbus_state = 0;
          modbus_rtu_struct.data_pointer = 0;
          
          init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
          config_handler_struct.state_update                   = _NO_FW_UPDATE;
          private_mib_base_struct.siteGroup.siteFirmwareUpdate = 0;
          config_handler_struct.cnt_timeout_update             = 0;
          
        }
        break;
      case _VAC_V2_FW_:// _VAC_V2_FW_----------------------------------------
        {
          //code here;
        }
        break;
      default:
        break;
      }
    }
    config_handler_struct.u8FwUpdateFlag = 0;
  }
}

//bnt factory default-----------------------------------------------------------
void bnt_factory_default(){
  if(system_flag_struct.flag_bnt == 0){
    
    if(!STATE_BNT_DEFAULT){
      if(system_flag_struct.cnt_time_bnt_default++>60){
        system_flag_struct.flag_bnt =1;
        system_flag_struct.cnt_time_bnt_default = 0;
        
      }
    }else{
      system_flag_struct.cnt_time_bnt_default =0;
    }
    
  }
  else if(system_flag_struct.flag_bnt == 1){
    
    ConfigLoadFactory(); 
    flag_struct.save_config_flag |= 1;
    system_flag_struct.u8IsRebootFlag = 1;
    system_flag_struct.u8IsRewriteSN = 1;
    
    system_flag_struct.flag_bnt =0;
    system_flag_struct.cnt_time_bnt_default = 0;
  }
  
}










