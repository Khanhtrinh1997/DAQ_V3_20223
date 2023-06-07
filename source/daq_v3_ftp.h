//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_FTP_H
#define _DAQ_V3_FTP_H
//include-----------------------------------------------------------------------
//define------------------------------------------------------------------------
#define FILE_NAME_LENGTH              30
#define USER_NAME_LENGTH              30
#define PASSWORD_LENGTH               30
#define ACCOUNT_LENGTH                30
//type enum---------------------------------------------------------------------
typedef enum{
  _DAQ_V3_FW_,
  _VAC_FW_,
  _VAC_V2_FW_,
  _NO_FW_UPDATE = 0xff
}STATE_UPDATE_ENUM;
//type struct-------------------------------------------------------------------
//ftp file struct---------------------------------------------------
typedef struct{
  STATE_UPDATE_ENUM         state_update;
  char                      file_name[FILE_NAME_LENGTH];
  char                      user[USER_NAME_LENGTH];
  char                      password[PASSWORD_LENGTH];
  char                      account[ACCOUNT_LENGTH];
}FTP_FILE_STRUCT;

//config handler----------------------------------------------------
typedef struct{
  STATE_UPDATE_ENUM          state_update;
  
  
  uint32_t                   page_to_write;
  uint32_t                   byte_to_write;
  uint32_t                   total_byte_to_write;
  uint8_t                    is_fw_update;
  uint8_t                    fw_update_complete;
  uint8_t                    num_of_block_to_erase;
  uint8_t                    is_reboot;
  uint8_t                    is_save_config;
  uint8_t                    is_vac_update;
  uint8_t                    vac_send_update_ctrl;
  uint8_t                    is_sync_time;
  
  uint32_t                   Check_vac_firm_version;
  uint8_t                    u8FwUpdateFlag;
  uint8_t                    u8FwUpdateCompleteFlag;
  uint32_t                   cnt_timeout_update;
  
}CONFIG_HANDLER_STRUCT;

extern CONFIG_HANDLER_STRUCT config_handler_struct;
//function----------------------------------------------------------------------

//code here


#endif /* _DAQ_V3_FTP_H */
//---------------------------------------------------------------