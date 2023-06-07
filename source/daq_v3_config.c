//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//declaration-------------------------------------------------------------------
static const SYSTEM_CONFIG_STRUCT system_config_parameter_factory =
{
  0x80,
  //ip parameter---------------------------------------------------
  {
    //an ascii string web name----------------------
    
    {
      'V','i','e','t','t','e','l',' ','P','o',
      'w','e','r',' ','S','y','s','t','e','m',
      ' ','M','o','n','i','t','o','r','\0',0 ,
      0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 
    },
    
    //static ip address-----------------------------
    
    0xc0a8010a,
    
    //default gateway ip address--------------------
    
    0xc0a80101,
    
    //subnet mask-----------------------------------
    
    0xffffff00,  
    
    //snmp ip---------------------------------------
    
    0xc0a8010c,
    
    //ftp server ip---------------------------------
    
    0xc0a8010c,
    
    //port sever ftp-------------------------------
    
    0x20c0,
      
    //period of snmp trap--------------------------
    
    0x00000005,
      
    //snmp trap mode-------------------------------
    
    0x01,
    
    //the firmware version-------------------------
    
    0xFFFFFFFF,
    
    //ftp server name-------------------------------
    {
      'V','i','e','t','t','e','l',' ','P','o',
      'w','e','r',' ','S','y','s','t','e','m',
      ' ','M','o','n','i','t','o','r','\0',0 ,
      0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 , 0 
    },
    //future expansion of ip parameter-------------
    {
      0xaaaaaaaa , 0xaaaaaaaa , 0xaaaaaaaa , 0xaaaaaaaa , 0xaaaaaaaa,
      0xaaaaaaaa , 0xaaaaaaaa , 0xaaaaaaaa , 0xaaaaaaaa , 0xaaaaaaaa
    }
  },
  //device number--------------------------------------------------
  {
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00, 0x00, 0x00,
    0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00 , 0x00, 0x00, 0x00
  },
  //modbus parameter-----------------------------------------------
  {
    
    //1----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //2---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //3----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //4---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //5----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //6---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //7----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //8---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //9----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //10---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //11----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //12---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //13----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //14---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //15----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //16---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //17----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //18---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //19----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //20---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },

    //21----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //22---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //23----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //24---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //25----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //26---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //27----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //28---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //29----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //30---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },    
    
    //31----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //32---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //33----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //34---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //35----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //36---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //37----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //38---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //39----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //40---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //41----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //42---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //43----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //44---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //45----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //46---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //47----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //48---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //49----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //50---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },

    //51----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //52---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //53----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //54---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //55----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //56---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //57----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //58---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //59----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //60---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //61----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //62---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //63----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //64---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //65----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //66---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //67----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //68---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //69----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //70---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //71----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //72---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //73----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //74---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //75----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //76---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //77----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //78---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //79----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //80---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
     //81----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //82---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //83----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //84---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //85----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //86---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //87----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //88---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //89----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //90---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //91----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //92---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //93----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //94---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },  
    
    //95----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //96---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    }, 
    
    //97----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //98---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },   
    
    //99----------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
    //100---------------------
    {
      0x00,
      0x00,
      0x00,
      0x00000000,
      {
        0x00000000,
        0x00000000,
      }
    },
    
  },
   0x00,
  //reserved-------------------------------------------------------
  0x66,
  {0x05,0x05},
  {0x03,0x03},
  0x00, 
  0x66,
  0x05,
  0x03,
  0x00,
  0x0000,
  0x00000000,
  0x00000000,
  0x00000000,
  0x00000000,
};
static SYSTEM_CONFIG_STRUCT_2 system_config_parameter_factory_2 =
{
    //
    // Is Serial Number and MAC is written to flash?
    //
    0,

    //
    // The user MAC
    //
    {
      0x00, 0x1A, 0xB6, 0x20, 0x02, 0x90
    },
    //
    // The user Serial Number
    //
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    },
    0,
    {
      0x20,0x20,0x20,0x20,
      0x20,0x20,0x20,0x20,
      0x20,0x20,0x20,0x20,
      0x20,0x20,0x20,0x20,
      0x20,0x20,0x20,0x20,
      0x20,0x20,0x20,0x20,
      0x20,0x20,0x20,0x20
    },
    //
    // FirmwareCRC_old
    //
    0xaaaaaaaa, 
    //
    // FirmwareCRC_new
    //
    0xaaaaaaaa,    
    //
    // u32Reserved2 (compiler will pad to the full length)
    //
    {
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa,
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa,
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa,
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa,
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa,
        0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa, 0xaaaaaaaa,      
        0xaaaaaaaa, 0xaaaaaaaa
    } 
    
};
SYSTEM_CONFIG_STRUCT         working_default_parameter;
SYSTEM_CONFIG_STRUCT_2       working_default_parameter_2;
SYSTEM_CONFIG_STRUCT         parameter;
SYSTEM_CONFIG_STRUCT_2       parameter2;
SYSTEM_CONFIG_STRUCT         *ptr_default_parameter = &working_default_parameter;
SYSTEM_CONFIG_STRUCT_2       *ptr_default_parameter_2 = &parameter2;
const SYSTEM_CONFIG_STRUCT   *const ptr_system_config_parameter_factory = &system_config_parameter_factory;
//funciton----------------------------------------------------------------------

//config init ------------------------------------------------------------------
uint8_t u8Buffer[512];
void config_init(void)
{
//  uint8_t u8Buffer[512];
  uint8_t *pui8Buffer = &u8Buffer[0];
//  tHisFlashLog sHisFlashLogBuf;
//  tHisFlashLog *pHisFlashLogBuf=&sHisFlashLogBuf;
  sFLASH_ReadBuffer((uint8_t *)&u8Buffer[0],FLASH_CFG_START_PARAM1, sizeof(SYSTEM_CONFIG_STRUCT));//FLASH_PB_SIZE * 2);
  
  if (u8Buffer[0] != 0x80)
  {
    // Copy the factory default parameter set to the active and working
    // parameter blocks.
    parameter = system_config_parameter_factory;
    working_default_parameter = system_config_parameter_factory;
         
    sFLASH_WriteBuffer((uint8_t *)ptr_default_parameter, FLASH_CFG_START_PARAM1, sizeof(SYSTEM_CONFIG_STRUCT));//FLASH_PB_SIZE * 2);
    //TRACE_INFO("Flash Not Write yet!...:%d\r\n",u8Buffer[0]);  
  }
  else
  {
    //TRACE_INFO("Read from Flash...:%d",u8Buffer[0]);
    //
    // A parameter block was found so copy the contents to both our
    // active parameter set and the working default set.
    //
    parameter = *(SYSTEM_CONFIG_STRUCT *)pui8Buffer;//pui8Buffer;
    working_default_parameter = parameter;
  }    

//  if ((u8Buffer[888] != 0x66)&&(u8Buffer[888] != 0x55))
//  {
//    parameter.u8FlagFreqTrap = 0x66;
//    parameter.u8DeltaFreqDisConnect[0] = 5;
//    parameter.u8DeltaFreqReConnect[0] = 3;
//    parameter.u8DeltaFreqDisConnect[1] = 5;
//    parameter.u8DeltaFreqReConnect[1] = 3;
//    parameter.u8EnableFreqTrap = 0;
//    parameter.u8DeltaFreqDisConnect2 = 5;
//    parameter.u8DeltaFreqReConnect2 = 3;
//    parameter.u8EnableFreqTrap2 = 0;
//    working_default_parameter = parameter;
//    config_save();
//    flag_struct.save_config_flag &= ~(0x01);  
//  } else if (u8Buffer[888] == 0x55)
//  {
//    parameter.u8FlagFreqTrap = 0x66;
//    parameter.u8DeltaFreqDisConnect2 = 5;
//    parameter.u8DeltaFreqReConnect2 = 3;
//    parameter.u8EnableFreqTrap2 = 0;
//    working_default_parameter = parameter;
//    config_save();
//    flag_struct.save_config_flag &= ~(0x01);  
//  }

  sFLASH_ReadBuffer((uint8_t *)&u8Buffer[0],FLASH_CFG_START_SN, FLASH_PB_SIZE);
  if (u8Buffer[0] == 0x53)
  {
    parameter2 = *(SYSTEM_CONFIG_STRUCT_2 *)pui8Buffer;  
    working_default_parameter_2 = parameter2;
  }
  else
  {
    parameter2 = system_config_parameter_factory_2;
    working_default_parameter_2 = parameter2;
  }
  sprintf((char*)working_default_parameter_2.u8CompileInfo,"%s %s", __DATE__, __TIME__);
  load_hisLog();
   
}

//config save------------------------------------------------------------------
void config_save(void)
{
  uint8_t NumOfPage = 0, /*NumOfSingle = 0,*/ i = 0;
  
  NumOfPage =  sizeof(SYSTEM_CONFIG_STRUCT) / sFLASH_SPI_PAGESIZE;
  //NumOfSingle = sizeof(SYSTEM_CONFIG_STRUCT) % sFLASH_SPI_PAGESIZE;
  for(i = 0; i <= NumOfPage; i++)
  {
     sFLASH_EraseSector(FLASH_CFG_START_PARAM1 + sFLASH_SPI_PAGESIZE *i);
  }
    //
    // Save the working defaults parameter block to flash.
    //
    sFLASH_WriteBuffer((uint8_t *)ptr_default_parameter, FLASH_CFG_START_PARAM1, sizeof(SYSTEM_CONFIG_STRUCT));//FLASH_PB_SIZE * 2);
    //TRACE_INFO("DONE Save!!!%d",g_sWorkingDefaultParameters.ucSequenceNum);
}

//config hislog
void load_hisLog(void)
{
  int i =0, j = 0; 
  for(i=0;i<5;i++)
  {
    pHisFlashLog = &sHisFlashLog[i];
    switch(i)
    {
    case 0:
           sFLASH_ReadBuffer((uint8_t *)pHisFlashLog,FLASH_PB_LOG_START, FLASH_PB_SIZE);
      break;
    case 1:
           sFLASH_ReadBuffer((uint8_t *)pHisFlashLog,FLASH_PB1_LOG_START, FLASH_PB_SIZE);
      break;
    case 2:
           sFLASH_ReadBuffer((uint8_t *)pHisFlashLog,FLASH_PB2_LOG_START, FLASH_PB_SIZE);
      break;
    case 3:
            sFLASH_ReadBuffer((uint8_t *)pHisFlashLog,FLASH_PB3_LOG_START, FLASH_PB_SIZE);
      break;
    case 4:
            sFLASH_ReadBuffer((uint8_t *)pHisFlashLog,FLASH_PB4_LOG_START, FLASH_PB_SIZE);
      break;
    default:
      break;
    }
   
    if(( sHisFlashLog[i].CODE[0]==0x09)&&(sHisFlashLog[i].CODE[1]==0x03))
      {
        //Do nothing
        if(j==0){
          if(sHisFlashLog[i].Count ==40){
            ih++;
            if(ih>4){
            sHisFlashLog[0].Count_old=0;
            sHisFlashLog[0].Count =0;
            ih=0;
            }
            }
          else j = 1;
        }        
      }
    else
      {
          sHisFlashLog[i].Count_old=0;
          sHisFlashLog[i].Count =0;
          sHisFlashLog[i].CODE[0]=0x09;
          sHisFlashLog[i].CODE[1]=0x03;
      }   
      HisCount_old[i] = sHisFlashLog[i].Count;
  }
  
}

//
void ConfigLoadFactory(void)
{
    parameter = system_config_parameter_factory;
    working_default_parameter = system_config_parameter_factory;
}


