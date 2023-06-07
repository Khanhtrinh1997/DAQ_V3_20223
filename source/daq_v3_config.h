//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_CONFIG_H
#define _DAQ_V3_CONFIG_H
//include-----------------------------------------------------------------------
//define------------------------------------------------------------------------
#define TOTAL_LIB_INDEX                 0
#define TOTAL_GEN_INDEX                 1 
#define TOTAL_BM_INDEX                  2
#define TOTAL_PM_INDEX                  3 
#define TOTAL_VAC_INDEX                 4
#define TOTAL_SMCB_INDEX                5 
#define TOTAL_FUEL_INDEX                6
#define TOTAL_ISENSE_INDEX              7
#define TOTAL_PMDC_INDEX                8

#define MAX_NUM_OF_LIB                  18
#define MAX_NUM_OF_GEN                  3
#define MAX_NUM_OF_BM                   18
#define MAX_NUM_OF_PM                   3
#define MAX_NUM_OF_VAC                  18
#define MAX_NUM_OF_SMCB                 6
#define MAX_NUM_OF_FUEL                 3
#define MAX_NUM_OF_ISENSE               3
#define MAX_NUM_OF_PMDC                 3


#define FLASH_PB_END                    0x410000
#define FLASH_PB_SIZE                   256
//typedef struct----------------------------------------------------------------
//modbus parameter--------------------------------------------
typedef struct{
  //the type of modbus device----------------------
  uint8_t                    device_type;
  //the index of modbus device--------------------
  uint8_t                    device_index;
  //the sub type of modbus device-----------------
  uint8_t                    device_vendor;
  //rhe modbus slave id---------------------------
  uint32_t                   slave_id;
  //future expansion of modbus parameter----------
  uint32_t                   reserved[2];
}MODBUS_PARAMETER_STRUCT;

//the ip config struct----------------------------------------
typedef struct{
  //an ascii string web name----------------------
  unsigned char              site_name[40];
  //static ip address-----------------------------
  uint32_t                   static_ip;
  //default gateway ip address--------------------
  uint32_t                   gateway_ip;
  //subnet mask-----------------------------------
  uint32_t                   subnet_mask;
  //snmp IP--------------------------------------
  uint32_t                   snmp_ip;
  //ftp server ip---------------------------------
  uint32_t                   ftp_sever_ip;
  //port sever ftp-------------------------------
  uint16_t                   ftp_port;
  //period of snmp trap--------------------------
  uint32_t                   trap_period;
  //snmp trap mode-------------------------------
  uint8_t                    trap_mode;
  //the firmware version-------------------------
  uint32_t                   firmware_crc;
  //ftp server name-------------------------------
  unsigned char              uc_ftp_server[40];
  //future expansion of ip parameter-------------
  uint32_t                   reserved[10];
}IP_PARAMETER_STRUCT;

//the system config struct----------------------------------
typedef struct{
  uint8_t                    flag_init;
  IP_PARAMETER_STRUCT        ip_parameter;
  uint8_t                    device_number[20];
  MODBUS_PARAMETER_STRUCT    modbus_parameter[100];
  uint8_t                    u8ControllerType;
  //! Freq Trap. 
  uint8_t u8FlagFreqTrap;
  uint8_t u8DeltaFreqDisConnect[2];
  uint8_t u8DeltaFreqReConnect[2];
  uint8_t u8EnableFreqTrap;
  //! ISENSE Freq Trap.  
  uint8_t u8FlagFreqTrap2;
  uint8_t u8DeltaFreqDisConnect2;
  uint8_t u8DeltaFreqReConnect2;
  uint8_t u8EnableFreqTrap2;
  uint16_t temp;
  uint32_t reserved1;
  uint32_t reserved2;
  uint32_t reserved3;
  uint32_t reserved4;
  
}SYSTEM_CONFIG_STRUCT;

typedef struct
{
    uint8_t u8IsWritten;
    uint8_t u8UserMAC[6];
    uint8_t u8UserSerialNo[20];   
    uint8_t u8Reserved1;
    uint8_t u8CompileInfo[28];    
    uint32_t u32FirmwareCRC_old;    
    uint32_t u32FirmwareCRC;
    uint32_t u32Reserved2[48];
}
SYSTEM_CONFIG_STRUCT_2;

extern SYSTEM_CONFIG_STRUCT         working_default_parameter;
extern SYSTEM_CONFIG_STRUCT_2       working_default_parameter_2;
extern SYSTEM_CONFIG_STRUCT         parameter;
extern SYSTEM_CONFIG_STRUCT_2       parameter2;
extern SYSTEM_CONFIG_STRUCT         *ptr_default_parameter;
extern SYSTEM_CONFIG_STRUCT_2       *ptr_default_parameter_2;
extern const SYSTEM_CONFIG_STRUCT   *const ptr_system_config_parameter_factory;  
//function----------------------------------------------------------------------
void config_init(void);
void config_save(void);
void load_hisLog(void);
void ConfigLoadFactory(void);

#endif /* _DAQ_V3_CONFIG_H */