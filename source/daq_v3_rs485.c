//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//declaration-------------------------------------------------------------------
static const unsigned char CRCHighTable[] =
{
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40
};

static const unsigned char CRCLowTable[] =
{
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
    0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8,
    0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11, 0xD1, 0xD0, 0x10,
    0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4,
    0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C,
    0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0,
    0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68,
    0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C,
    0xB4, 0x74, 0x75, 0xB5, 0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54,
    0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98,
    0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40
};

MODBUS_RTU_STRUCT modbus_rtu_struct;
//LPUART5 function======================================================================
#define LPUART5_CLOCK_SOURCE 80000000UL
void init_rs232_dc_cabinet(uint32_t baudrate, uint8_t parity){
  LPUART_Deinit(LPUART5);
  lpuart_config_t config;
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = baudrate;
  if(parity == NONE_PARITY){
    config.parityMode = kLPUART_ParityDisabled;
  }else if(parity == ODD_PARITY){
    config.parityMode = kLPUART_ParityOdd;
  }else if(parity == EVEN_PARITY){
    config.parityMode = kLPUART_ParityEven;
  }
  config.enableTx     = true;
  config.enableRx     = true;
  LPUART_Init(LPUART5, &config, LPUART5_CLOCK_SOURCE);
  LPUART_EnableInterrupts(LPUART5, kLPUART_RxDataRegFullInterruptEnable|kLPUART_RxOverrunInterruptEnable);
  EnableIRQ(LPUART5_IRQn);
}
//LPUART4 9 bit function================================================================
#define LPUART4_CLOCK_SOURCE 80000000UL
void init_rs485_dc_cabinet_9_bit(uint32_t baudrate, uint8_t parity){
  LPUART_Deinit(LPUART4);
  lpuart_config_t config;
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = baudrate;
  if(parity == NONE_PARITY){
    config.parityMode = kLPUART_ParityDisabled;
  }else if(parity == ODD_PARITY){
    config.parityMode = kLPUART_ParityOdd;
  }else if(parity == EVEN_PARITY){
    config.parityMode = kLPUART_ParityEven;
  }
  config.enableTx     = true;
  config.enableRx     = true;
  
  LPUART_Init(LPUART4, &config, LPUART4_CLOCK_SOURCE);
  
   LPUART_Enable9bitMode(LPUART4, false);
   
  LPUART_EnableInterrupts(LPUART4, kLPUART_RxDataRegFullInterruptEnable|kLPUART_RxOverrunInterruptEnable);
  EnableIRQ(LPUART4_IRQn);
}
uint32_t ctrl =0u;
//LPUART4  9 bit send----------------------------------------------------------------------
void lpuart_send_9_bit_mode(LPUART_Type *base,uint8_t data,uint8_t addr_value){
  uint8_t data_send;
  data_send =data;
  if(addr_value == 0){
    //LPUART_Enable9bitMode(LPUART4, true);
    base->CTRL|=LPUART_CTRL_M_MASK;
    LPUART_WriteBlocking(base,&data_send,1);   // co ff
    base->CTRL &= ~(uint32_t)LPUART_CTRL_M_MASK;
    //LPUART_Enable9bitMode(LPUART4,false);
    ctrl = base->CTRL;
  }
  else if(addr_value == 1){

    LPUART_WriteBlocking(base,&data_send,1);
  }
}
//=======================================================================================

//funciton----------------------------------------------------------------------
//init rs485 dc cabinet --------------------------------------
void init_rs485_dc_cabinet(uint32_t baudrate, uint8_t parity){
  LPUART_Deinit(LPUART4);
  lpuart_config_t config;
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = baudrate;
  if(parity == NONE_PARITY){
    config.parityMode = kLPUART_ParityDisabled;
  }else if(parity == ODD_PARITY){
    config.parityMode = kLPUART_ParityOdd;
  }else if(parity == EVEN_PARITY){
    config.parityMode = kLPUART_ParityEven;
  }
  config.enableTx     = true;
  config.enableRx     = true;
  LPUART_Init(LPUART4, &config, BOARD_DebugConsoleSrcFreq());
  LPUART_EnableInterrupts(LPUART4, kLPUART_RxDataRegFullInterruptEnable|kLPUART_RxOverrunInterruptEnable);
  EnableIRQ(LPUART4_IRQn);
}

//init rs485 modbus rtu--------------------------------------
void init_rs485_modbus_rtu(uint32_t baudrate, uint8_t parity){
  LPUART_Deinit(LPUART3);
  lpuart_config_t config;
  LPUART_GetDefaultConfig(&config);
  config.baudRate_Bps = baudrate;
  if(parity == NONE_PARITY){
    config.parityMode = kLPUART_ParityDisabled;
  }else if(parity == ODD_PARITY){
    config.parityMode = kLPUART_ParityOdd;
  }else if(parity == EVEN_PARITY){
    config.parityMode = kLPUART_ParityEven;
  }
  config.enableTx     = true;
  config.enableRx     = true;
  LPUART_Init(LPUART3, &config, BOARD_DebugConsoleSrcFreq());
  LPUART_EnableInterrupts(LPUART3, kLPUART_RxDataRegFullInterruptEnable|kLPUART_RxOverrunInterruptEnable);
  EnableIRQ(LPUART3_IRQn);
}

//cmd read function code 0x01--0x02--0x03--0x04--0x11-------------------------
void master_read_modbus(MODBUS_RTU_STRUCT* p,uint8_t function_code, uint8_t slave_addr,uint16_t starting_addr,uint16_t no_point,uint32_t delay){
  p->slave_id                      = slave_addr;
  p->function_code                 = function_code;
  p->start_high                    = (uint8_t)(starting_addr>>8);
  p->start_low                     = (uint8_t)(starting_addr);
  p->number_reg_high               = (uint8_t)(no_point>>8);
  p->number_reg_low                = (uint8_t)(no_point);
  
  p->buff_write[0]                 = p->slave_id;
  p->buff_write[1]                 = p->function_code;
  p->buff_write[2]                 = p->start_high;
  p->buff_write[3]                 = p->start_low;
  p->buff_write[4]                 = p->number_reg_high;
  p->buff_write[5]                 = p->number_reg_low;
  
  switch(function_code){
    case 0x01:
    case 0x02:
        if((no_point%8)==0){
          no_point = no_point/8; 
        }else{
           no_point = (no_point/8)+1;
        }
        p->number_reg_high              = (uint8_t)(no_point>>8);
        p->number_reg_low               = (uint8_t)(no_point);
      break;
    case 0x03:
    case 0x04:
    case 0x11:
    default:
      break;
  }
  //calculate crc----------------------------------------
  modbus_crc(p,6,_CRC_W);
  
  p->buff_write[6]                = p->crc_high;
  p->buff_write[7]                = p->crc_low;
  //send------------------------------------------------
  GPIO_PinWrite(GPIO2, 6U,1);
  GPIO_PinWrite(GPIO3, 26U,1);
   
  
  LPUART_WriteBlocking(LPUART3,p->buff_write,8);
  
  
  delay_modbus_rtu(delay);
  
  GPIO_PinWrite(GPIO2, 6U,0);
  GPIO_PinWrite(GPIO3, 26U,0);
  
}

//cmd check crc-------------------------------------------
void modbus_crc (MODBUS_RTU_STRUCT* p, unsigned char len, CRC_MODBUS_TYPE crc_type){
  unsigned char temp;
  uint8_t i;
  p->crc_high = 0xff;
  p->crc_low  = 0xff;
  i           = 0;
  while(len--){
    if(crc_type == _CRC_W){
      temp                    = (p->buff_write[i]) ^ p->crc_high;
    }
    else if(crc_type == _CRC_R)
    {
      temp                    = (p->buff_read[i]) ^ p->crc_high;
    }
    p->crc_high             = CRCHighTable[temp] ^ p->crc_low;
    p->crc_low              = CRCLowTable[temp];
    i++;
  }
}
//test check crc ======================================
void check_crc (uint8_t *crc_high, uint8_t *crc_low,uint8_t *buff, uint32_t len){
  unsigned char temp;
  *crc_high =0xff;
  *crc_low = 0xff;
  uint32_t i =0;
  while(len--){
    temp                 = buff[i]^(*crc_high);
    *crc_high            = CRCHighTable[temp] ^ (*crc_low);
    *crc_low             = CRCLowTable[temp];
    i++;
  }
}
//cmd delay----------------------------------------------
void delay_modbus_rtu(uint32_t delay){
  volatile uint32_t i =0;
  for(i=0;i<delay; ++i)
  {
    __NOP();
  }
}

//cmd modbus parse bit----------------------------------
void modbus_parse_bit(uint8_t *buf, uint8_t *var_type_u8, uint32_t pos, uint8_t coil ){
  uint8_t temp;
  switch(coil)
  {
  case 0:
    {
      temp = buf[3 + pos];
      temp = temp&0x01;
      *var_type_u8 = temp;
    }
    break;
  case 1:
    {
      temp = buf[3 + pos];
      temp = (temp>>1)&0x01;
      *var_type_u8 = temp;
    }
    break;
  case 2:
    {
      temp = buf[3 + pos];
      temp = (temp>>2)&0x01;
      *var_type_u8 = temp;
    }
    break;
  case 3:
    {
      temp = buf[3 + pos];
      temp = (temp>>3)&0x01;
      *var_type_u8 = temp;
    }
    break;
  case 4:
    {
      temp = buf[3 + pos];
      temp = (temp>>4)&0x01;
      *var_type_u8 = temp;
    }
    break;
  case 5:
    {
      temp = buf[3 + pos];
      temp = (temp>>5)&0x01;
      *var_type_u8 = temp;
    }
    break;
  case 6:
    {
      temp = buf[3 + pos];
      temp = (temp>>6)&0x01;
      *var_type_u8 = temp;
    }
    break;
  case 7:
    {
      temp = buf[3 + pos];
      temp = (temp>>7)&0x01;
      *var_type_u8 = temp;
    }
    break;
  }
  
}

//cmd modbus parse info--------------------------------
void modbus_parse_info(uint8_t *buf, uint8_t *var_type_u8, uint16_t *var_type_u16, uint32_t *var_type_u32, uint32_t pos, uint8_t type){
  uint8_t  u8Var;
  uint16_t u16Var;
  uint32_t u32Var;
  switch(type)
  {
  case 8:// 8-byte Integer
    {
      u8Var = buf[3 + pos * 2];
      *var_type_u8 = u8Var;
    }
    break;
  case 16:// 16-byte Integer
    {
      u16Var = (buf[3 + pos * 2] << 8) | buf[3 + pos * 2 + 1];
      *var_type_u16 = u16Var;
    }
    break;
  case 32:// 32-byte Integer
    {
      u32Var = (buf[3 + pos * 2] << 24) | (buf[3 + pos * 2 + 1] << 16) | (buf[3 + pos * 2 + 2] << 8) |(buf[3 + pos * 2 + 3]);
      *var_type_u32 = u32Var;
    }
    break;
  };
}
//cmd modbus parse info inverse-----------------------
void modbus_parse_info_inverse(uint8_t *buf, uint8_t *var_type_u8, uint16_t *var_type_u16, uint32_t *var_type_u32, uint32_t pos, uint8_t type){
  uint8_t  u8Var;
  uint16_t u16Var;
  uint32_t u32Var;
  switch(type)
  {
  case 8:// 8-byte Integer
    {
      u8Var = buf[3 + pos * 2];
      *var_type_u8 = u8Var;
    }
    break;
  case 16:// 16-byte Integer
    {
      u16Var = (buf[3 + pos * 2 + 1] << 8) | buf[3 + pos * 2];
      *var_type_u16 = u16Var;
    }
    break;
  case 32:// 32-byte Integer
    {
      u32Var = (buf[3 + pos * 2 + 2] << 24) | (buf[3 + pos * 2 + 3] << 16) | (buf[3 + pos * 2 + 0] << 8) |(buf[3 + pos * 2 + 1]);
      *var_type_u32 = u32Var;
    }
    break;
  };
}

//cmd modbus parse info sign-------------------------
void modbus_parse_info_sign(uint8_t *buf, int8_t *var_type_i8, int16_t *var_type_i16, int32_t *var_type_i32, uint32_t pos, uint8_t type){
  int8_t  u8Var;
  int16_t u16Var;
  int32_t u32Var;
  switch(type)
  {
  case 8:// 8-byte Integer
    {
      u8Var = buf[3 + pos * 2];
      *var_type_i8 = u8Var;
    }
    break;
  case 16:// 16-byte Integer
    {
      u16Var = (buf[3 + pos * 2] << 8) | buf[3 + pos * 2 + 1];
      *var_type_i16 = u16Var;
    }
    break;
  case 32:// 32-byte Integer
    {
      u32Var = (buf[3 + pos * 2] << 24) | (buf[3 + pos * 2 + 1] << 16) | (buf[3 + pos * 2 + 2] << 8) |(buf[3 + pos * 2 + 3]);
      *var_type_i32 = u32Var;
    }
    break;
  };  
}
//cmd modbus parse info sign inverse----------------
void modbus_parse_info_sign_inverse(uint8_t *buf, int8_t *var_type_i8, int16_t *var_type_i16, int32_t *var_type_i32, uint32_t pos, uint8_t type){
      int8_t u8Var;
    int16_t u16Var;
    int32_t u32Var;
    switch(type)
    {
    case 8:// 8-byte Integer
    {
        u8Var = buf[3 + pos * 2];
        *var_type_i8 = u8Var;
    }
    break;
    case 16:// 16-byte Integer
    {
        u16Var = (buf[3 + pos * 2 + 1] << 8) | buf[3 + pos * 2];
        *var_type_i16 = u16Var;
    }
    break;
    case 32:// 32-byte Integer
    {
        u32Var = (buf[3 + pos * 2 + 2] << 24) | (buf[3 + pos * 2 + 3] << 16) | (buf[3 + pos * 2 + 0] << 8) |(buf[3 + pos * 2 + 1]);
        *var_type_i32 = u32Var;
    }
    break;
    };
}

//cmd modbus parse info f--------------------------
void modbus_parse_info_f(uint8_t *buf, float *var_type_f, uint32_t pos){
  uint32_t u32Temp = 0;
  float fVar;
  modbus_parse_info(buf, 0, 0, &u32Temp, pos, 32);
  memcpy(&fVar,&u32Temp,4);
  *var_type_f = fVar;
}

//cmd modbus parse info f inverse-----------------
void modbus_parse_info_f_inverse(uint8_t *buf, float *var_type_f, uint32_t pos, uint8_t type_1_2){
  uint32_t u32Temp = 0;
  float fVar;
  if(type_1_2 == 1){
    u32Temp = (buf[3 + pos * 2 + 2] << 24) | (buf[3 + pos * 2 + 3] << 16) | (buf[3 + pos * 2 + 0] << 8) |(buf[3 + pos * 2 + 1]);
  }else if(type_1_2 == 2){
    u32Temp = (buf[3 + pos * 2] << 24) | (buf[3 + pos * 2 + 1] << 16) | (buf[3 + pos * 2 + 2] << 8) |(buf[3 + pos * 2 + 3]);
  }
  memcpy(&fVar,&u32Temp,4);
  *var_type_f = fVar;
}

//cmd modbus parse info string--------------------
void modbus_parse_info_string (uint8_t *buf, uint8_t *dst_buf, uint8_t num_of_byte, uint32_t pos, uint8_t type_1_2){
  uint16_t temp;
  for (uint16_t i = 0; i < num_of_byte; i++)
  {
    if(type_1_2 == 1){
      modbus_parse_info(buf, 0, &temp,0,pos + i,16);
      dst_buf[i] = (uint8_t) temp;
      if(dst_buf[i] == 0x20)
      {
        dst_buf[i] = 0;
        break;
      }
      
    }else if(type_1_2 == 2){ //doc cac thanh ghi 16bit chua 2 ky tu ASCII
      modbus_parse_info(buf, 0, &temp,0,pos + i,16);
      dst_buf[i*2+1] = (uint8_t) temp;
      dst_buf[i*2] = (uint8_t) (temp>>8);
      if(dst_buf[i] == 0x20)
      {
        dst_buf[i] = 0;
        break;
      }
      
    }
  }
}

//cmd modbus parse info bcd-----------------------
void modbus_parse_info_bcd (uint8_t *buf, uint8_t *var_type_u8, uint16_t *var_type_u16, uint32_t *var_type_u32, uint32_t pos, uint8_t type){
  uint8_t u8Var;
  uint16_t u16Var;
  uint32_t u32Var;
  uint8_t u8temp1,u8temp2,u8temp3,u8temp4;
  switch(type)
  {
  case 8:// 8-byte bcd
    {
      u8Var = (buf[3 + pos * 2]/0x10*0x0A)+(buf[3 + pos * 2]%0x10);
      *var_type_u8 = u8Var;
    }
    break;
  case 16:// 16-byte bcd
    {
      u8temp1 = (buf[3 + pos * 2]/0x10*0x0A)+(buf[3 + pos * 2]%0x10);
      u8temp2 = (buf[3 + pos * 2 + 1]/0x10*0x0A)+(buf[3 + pos * 2 + 1]%0x10);
      
      u16Var = u8temp1*100 + u8temp2;
      *var_type_u16 = u16Var;
    }
    break;
  case 32:// 32-byte bcd
    {
      u8temp1 = (buf[3 + pos * 2]/0x10*0x0A)+(buf[3 + pos * 2]%0x10);
      u8temp2 = (buf[3 + pos * 2 + 1]/0x10*0x0A)+(buf[3 + pos * 2 + 1]%0x10);
      u8temp3 = (buf[3 + pos * 2 + 2]/0x10*0x0A)+(buf[3 + pos * 2 + 2]%0x10);
      u8temp4 = (buf[3 + pos * 2 + 3]/0x10*0x0A)+(buf[3 + pos * 2 + 3]%0x10);
      
      u32Var = u8temp1*1000000 + u8temp2*10000 + u8temp3*100 + u8temp4;
      *var_type_u32 = u32Var;
    }
    break;
  };
  
}

//cmd modbus parse time--------------------------
void modbus_parse_time(uint8_t *buf, TIME_FORMAT_STRUCT  *set_time, uint32_t pos){
  set_time->min   = buf[3 + pos * 2];
  set_time->sec   = buf[3 + pos * 2 + 1];
  set_time->date  = buf[3 + pos * 2 + 2];
  set_time->hour  = buf[3 + pos * 2 + 3];
  set_time->month = buf[3 + pos * 2 + 5];
  set_time->year  = (buf[3 + pos * 2 + 6] << 8) | (buf[3 + pos * 2 + 7]) ;
}

//cmd write single reg----------------------------
void mater_write_single_reg(MODBUS_RTU_STRUCT* p, uint8_t slave_addr, uint16_t reg_addr,uint16_t write_val,uint32_t delay){
  p->slave_id                                     = slave_addr;
  p->function_code                                = 0x06;
  p->start_high                                   = (uint8_t)(reg_addr>>8);
  p->start_low                                    = (uint8_t)(reg_addr);
  p->number_reg_high                              = (uint8_t)(write_val>>8);
  p->number_reg_low                               = (uint8_t)(write_val);
  
  p->buff_write[0]                                = p->slave_id;
  p->buff_write[1]                                = p->function_code;
  p->buff_write[2]                                = p->start_high;
  p->buff_write[3]                                = p->start_low;
  p->buff_write[4]                                = p->number_reg_high;
  p->buff_write[5]                                = p->number_reg_low;
  
  modbus_crc(p,6,_CRC_W);
  p->buff_write[6]                                = p->crc_high;
  p->buff_write[7]                                = p->crc_low;
  
  //send------------------------------------------------
  GPIO_PinWrite(GPIO2, 6U,1);
  GPIO_PinWrite(GPIO3, 26U,1);
   
  LPUART_WriteBlocking(LPUART3,p->buff_write,8);
  delay_modbus_rtu(delay);
  
  GPIO_PinWrite(GPIO2, 6U,0);
  GPIO_PinWrite(GPIO3, 26U,0);  
}

//cmd write_multi_reg-----------------------------
void mater_write_multi_reg (MODBUS_RTU_STRUCT* p, uint8_t slave_addr, uint16_t reg_addr, uint8_t *write_buff, uint8_t size,uint32_t delay){
  uint8_t i, temp[256];
  uint8_t num_of_byte;
  
  if(size <= 127)
  {
    num_of_byte = size*2;
  }
  else
  {
    num_of_byte = 2;
  }
  
  p->slave_id                                      = slave_addr;
  p->function_code                                 = 0x10;
  p->start_high                                    = (uint8_t)(reg_addr >>8);
  p->start_low                                     = (uint8_t)(reg_addr);
  p->number_reg_high                               = (uint8_t)(size >>8);
  p->number_reg_low                                = (uint8_t)(size);
  
  p->buff_write[0]                                 = p->slave_id;
  p->buff_write[1]                                 = p->function_code;
  p->buff_write[2]                                 = p->start_high;
  p->buff_write[3]                                 = p->start_low;
  p->buff_write[4]                                 = p->number_reg_high;
  p->buff_write[5]                                 = p->number_reg_low;
  p->buff_write[6]                                 = num_of_byte;
  
  for(i = 0; i < num_of_byte; i++ ){
    temp[i] = *(write_buff +i);
    p->buff_write [7+ i] = temp[i];
  }
  
  modbus_crc(p, 7+num_of_byte,_CRC_W);
  p->buff_write[7+num_of_byte]             = p->crc_high;
  p->buff_write[8+num_of_byte]             = p->crc_low;
  //send------------------------------------------------
  GPIO_PinWrite(GPIO2, 6U,1);
  GPIO_PinWrite(GPIO3, 26U,1);
   
  LPUART_WriteBlocking(LPUART3,p->buff_write,9+num_of_byte);
  delay_modbus_rtu(delay);
  
  GPIO_PinWrite(GPIO2, 6U,0);
  GPIO_PinWrite(GPIO3, 26U,0);  
  
  
}
//cmd set single var u16 f06-------------------
void mater_write_06_u16(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager,uint8_t slave_add, uint16_t reg_addr, uint16_t val, DEVICE_SLAVE slave_type, uint32_t delay){
  switch(slave_type){
  case _GEN_SLAVE://GEN-------------------------------------
    {
      init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      for(uint8_t i=0; i<3;i++){
        mater_write_single_reg(p_modbus,slave_add,reg_addr,val,delay);
          vTaskDelay(500);
        if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==6){
          break;
        } 
      }
      
    }
    break;
  case _SMBC_SLAVE://SMCB-----------------------------------
    {
      init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      for(uint8_t i=0; i<3;i++){
        mater_write_single_reg(p_modbus,slave_add,reg_addr,val,delay);
          vTaskDelay(500);
        if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==6){
          break;
        } 
      }
    }
    break;
  default:
    vTaskDelay(300);
    break;
  }
}
//cmd set var u16-------------------------------
void mater_write_u16(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_add, uint16_t reg_addr, uint16_t val, DEVICE_SLAVE slave_type, uint32_t delay)
{
  uint8_t data_buff[4];
  data_buff[0] = (uint8_t) (val>> 8);
  data_buff[1] = (uint8_t) (val);
  //check respond-------------------------------------------------------------
  switch (slave_type){
  case _VAC_SLAVE://vac-------------------------------
    init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
    for(uint8_t i=0; i<3;i++){
      mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 1 , delay);
      vTaskDelay(300);
      if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
        break;
      }
    }
    break;
  case _LIB_SLAVE://lib-------------------------------
    init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
    for(uint8_t i=0; i<3;i++){
      mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 1 , delay);
      vTaskDelay(500);
      if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
        break;
      }
    }
    break;
  default:
    vTaskDelay(300);
    break;
  }
}
//cmd set var i32--------------------------------
void mater_write_i32(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_add, uint16_t reg_addr, int32_t val, DEVICE_SLAVE slave_type, uint32_t delay){
  uint8_t data_buff[4];
  data_buff[0] = (uint8_t) (val >> 24);
  data_buff[1] = (uint8_t) (val >> 16);
  data_buff[2] = (uint8_t) (val >> 8);
  data_buff[3] = (uint8_t) (val);
  //check respond-------------------------------------------------------------
  switch (slave_type){
  case _VAC_SLAVE://vac-------------------------------
    {
      init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      for(uint8_t i=0; i<3;i++){
        mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 2 , delay);
        vTaskDelay(300);
        if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
          break;
        }
      }
    }
    break;
  case _LIB_SLAVE://lib-------------------------------
    {
      init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      for(uint8_t i=0; i<3;i++){
        mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 2 , delay);
        vTaskDelay(500);
        if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
          break;
        }
      }
    }
    break;
  case _BM_SLAVE://bm--------------------------------
    {
      init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      for(uint8_t i=0; i<3;i++){
        mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 2 , delay);
        vTaskDelay(500);
        if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
          break;
        }
      }
      
    }
  default:
    vTaskDelay(300);
    break;
  }
}

//cmd set var i16---------------------------------
void mater_write_i16(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_add, uint16_t reg_addr, int16_t val, DEVICE_SLAVE slave_type, uint32_t delay){
  uint8_t data_buff[4];
  data_buff[0] = (uint8_t) (val>> 8);
  data_buff[1] = (uint8_t) (val);
  //check respond-------------------------------------------------------------
  switch (slave_type){
  case _VAC_SLAVE://vac-------------------------------
    init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
    for(uint8_t i=0; i<3;i++){
      mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 1 , delay);
      
      vTaskDelay(300);
      if(config_handler_struct.state_update == _VAC_FW_) vTaskDelay(700);
      
      if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
        break;
      }
    }
    break;
  case _LIB_SLAVE://lib-------------------------------
    init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
    for(uint8_t i=0; i<3;i++){
      mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 1 , delay);
      vTaskDelay(500);
      if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
        break;
      }
    }
    break;
  case _GEN_SLAVE://gen------------------------------
    {
      init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      for(uint8_t i=0; i<3;i++){
        mater_write_multi_reg(p_modbus, slave_add , reg_addr , &data_buff[0] , 1 , delay);
        vTaskDelay(500);
        if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
          break;
        }
    }
      
    }
  default:
    vTaskDelay(300);
    break;
  }
}

//cmd set var time ----------------------------------------------
void set_var_time(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_addr, uint16_t reg_addr,TIME_FORMAT_STRUCT set_time,DEVICE_SLAVE slave_type, uint32_t delay){
  uint8_t data_buff[16];
  switch(slave_addr){
  case _VAC_SLAVE: //VAC------------------------------------
    {
      data_buff[0] = (uint8_t) (set_time.min);
      data_buff[1] = (uint8_t) (set_time.sec);
      data_buff[2] = (uint8_t) (set_time.date);
      data_buff[3] = (uint8_t) (set_time.hour);
      data_buff[4] = 0;
      data_buff[5] = (uint8_t) (set_time.month);
      data_buff[6] = (uint8_t) ((set_time.year) >>8);
      data_buff[7] = (uint8_t) (set_time.year);
      
      init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      for (uint8_t i=0; i<3;i++){
        mater_write_multi_reg(p_modbus,slave_addr,reg_addr,&data_buff[0],4,delay);
        vTaskDelay(300);
        if(mater_check_respond_data(p_modbus,p_modbus_manager,slave_type)==16){
          break;
        }
      } 
    }
    break;
  case _LIB_SLAVE://LIB--------------------------------------
    //code here
    break;
  default:
    break; 
  }
}

//check respond data vac--------------------------
int8_t mater_check_respond_data(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager,DEVICE_SLAVE slave_type){
  uint16_t m_temp = 0 ; 
  if(p_modbus->modbus_state == READING_END){
    p_modbus->crc_high_read = p_modbus->buff_read[p_modbus->byte_count -2];
    p_modbus->crc_low_read  = p_modbus->buff_read[p_modbus->byte_count -1];
    modbus_crc(p_modbus,p_modbus->byte_count -2,_CRC_R);
    
    if((p_modbus->crc_high == p_modbus->crc_high_read)
       &&(p_modbus->crc_low == p_modbus->crc_low_read))
    {
      p_modbus->function_code      = p_modbus->buff_read[1];
      
      switch(p_modbus->function_code)
      {
      case _READ_COIL_STATUS://0x01---------------
      case _READ_INPUT_STATUS://0x02--------------
        {
          m_temp = (p_modbus->number_reg_high<<8)|(p_modbus->number_reg_low);
          
          if(p_modbus->buff_read[2] == m_temp){
            //select slave---------------------------------------------------
            switch(slave_type){
            case _VAC_SLAVE://VAC------------------------------
              //code here
              break;
            case _LIB_SLAVE://LIB------------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_slave_offset)   
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                  {
                    extract_holding_regs_data_lib(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
                
              }
              break;
            case _GEN_SLAVE://GEN------------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_slave_offset)
                {
                case 1:
                case 2:
                  {
                    extract_holding_regs_data_gen(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
                
              }
              break;
            default:
              break;
              
            }
            //-----------------------------------------------------------------  
          }
          else {
            p_modbus->modbus_state =START;
            return -3;
          }
          
        }
        break;
      case _READ_HOLDING_REGS://0x03--------------
      case _READ_INPUT_REG://0x04----------------
        {
          m_temp = (p_modbus->number_reg_high<<8)|(p_modbus->number_reg_low);
          if(p_modbus->buff_read[2] == (m_temp*2)){
            //slave type ---------------------------------------------------
            switch(slave_type){
            case _VAC_SLAVE://VAC------------------------------
              {
                switch (p_modbus->slave_id - p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_slave_offset){
                case 1:
                case 2:
                  extract_holding_regs_data_vac(p_modbus,p_modbus_manager);
                  break;
                default:
                  break; 
                }
              }
              break;
            case _LIB_SLAVE://LIB------------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_slave_offset)   // ???????????????
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                  {
                    extract_holding_regs_data_lib(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
                
              }
              break;
            case _GEN_SLAVE://GEN------------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_slave_offset)
                {
                case 1:
                case 2:
                  {
                    extract_holding_regs_data_gen(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
              }
              break;
            case _PM_SLAVE://PM--------------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_slave_offset)      // 
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                  {
                    extract_holding_regs_data_pm(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
                
              }
              break;
            case _SMBC_SLAVE://SMCB----------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_slave_offset)   
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                  {
                    extract_holding_regs_data_smcb(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
              }
              break;
            case _FUEL_SLAVE://FUEL----------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_slave_offset)   
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                  {
                    extract_holding_regs_data_fuel(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
              }
              break;
            case _ISENSE_SLAVE://ISENSE------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_slave_offset)      // 
                {
                case 1:
                  {
                    extract_holding_regs_data_isense(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
              }
              break;
            case _PMDC_SLAVE://PMDC----------------------------
              {
                switch(p_modbus->slave_id - p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_slave_offset)      // 
                {
                case 1:
                  {
                    extract_holding_reg_data_pmdc(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
              }
              break;
            case _BM_SLAVE:
              {
                
                switch(p_modbus->slave_id - p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_slave_offset)
                {
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                  {
                    extract_holding_reg_data_bm(p_modbus,p_modbus_manager);
                  }
                  break;
                  
                default:
                  break;
                }
              }
              break;
              
            default:
              break;
            }
            //---------------------------------------------------------------
            
          }else{
            p_modbus->modbus_state =START;
            return -3;
          }
        }
        break;
      case _EXCEPTION_READ_HOLDING://0x83-------
        {
          p_modbus->modbus_state = START;
          return -4;
        }
        break;
      case _PRESET_SINGLE_REG:
        {
          p_modbus->modbus_state = START;
          return 6;
        } 
      case _PRESET_MULTIPLE_REGS://0x10------------
        {
          p_modbus->modbus_state = START;
          return 16;         
        }
        break;
      case _REPORT_SLAVE_ID://0x11--------------------
        {
          switch(slave_type){
          case _VAC_SLAVE:
            //code here
            break;
          case _LIB_SLAVE:
            {
              switch(p_modbus->slave_id - p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_slave_offset)   
              {
              case 1:
              case 2:
              case 3:
              case 4:
              case 5:
              case 6:
              case 7:
              case 8:
              case 9:
              case 10:
              case 11:
              case 12:
              case 13:
              case 14:
              case 15:
              case 16:
                {
                  extract_holding_regs_data_lib(p_modbus,p_modbus_manager);
                }
                break;
                
              default:
                break;
              }
              
            }
          default:
            break;
          }
        }
        break;
        default:
          break; 
        }
      p_modbus->modbus_state = START;
    }else{
      p_modbus->modbus_state = START;
      return -2;
    }
    
  }else{
    p_modbus->modbus_state = START;
    return -1;
  }
  return 1;
}

//extract holding regs data vac-------------------
void extract_holding_regs_data_vac(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint8_t i;
  uint8_t *ptr = &p_modbus->buff_read[0];
  
  switch(p_modbus->slave_id - p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_slave_offset){
  case 1:
    i = 0;
    break;
  case 2:
    i = 1;
    break;
  }
  
  switch (p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_type){
  case 1:
    {
      switch(p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_running_step)
      {
      case VAC_VIETTEL_INFO_1:
        {
          modbus_parse_info(ptr,0,&p_modbus_manager->vac_info_struct[i].firm_version,0,0,16);
          modbus_parse_info(ptr,0,&p_modbus_manager->vac_info_struct[i].sn_hi,0,1,16);
          modbus_parse_info(ptr,0,&p_modbus_manager->vac_info_struct[i].sn_lo,0,2,16);
          
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].in_temp,0,3,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].ex_temp,0,4,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].frost1_temp,0,5,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].frost2_temp,0,6,16);
          
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].humid,0,7,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].fan1_duty,0,8,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].fan1_rpm,0,9,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].fan2_duty,0,10,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].fan2_rpm,0,11,16);
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->vac_info_struct[i].alarm_status,12,32);
          
          if((p_modbus_manager->vac_info_struct[i].alarm_status&0x00008000) == 0x00008000 ){
            p_modbus_manager->vac_info_struct[i].vac_reset_ok = 1;
          }else{
            p_modbus_manager->vac_info_struct[i].vac_reset_ok = 0;
          }
     
          p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_running_step = VAC_VIETTEL_INFO_2;
        }
        break;
      case VAC_VIETTEL_INFO_2:
        {
          
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].sys_mode,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].fan_speed_mode,0,1,16);
          
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].fan_start_temp,0,2,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].set_temp,0,3,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].pid_offset_temp,0,4,16);
          
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].fan_min_speed,0,5,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].fan_max_speed,0,6,16);
          
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].filter_stuck_temp,0,7,16);
          
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].night_mode_en,0,8,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].night_mode_start,0,9,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].night_mode_end,0,10,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].night_max_speed,0,11,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].manual_mode,0,12,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].manual_max_speed,0,13,16);
          
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].in_max_temp,0,14,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].ex_max_temp,0,15,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].frost_max_temp,0,16,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].in_min_temp,0,17,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].ex_min_temp,0,18,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].frost_min_temp,0,19,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].min_out_temp,0,20,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].delta_temp,0,21,16);
          modbus_parse_info_sign(ptr,0,&p_modbus_manager->vac_info_struct[i].panic_temp,0,22,16);
          
          modbus_parse_info(ptr,0,&p_modbus_manager->vac_info_struct[i].active_fan,0,23,16);
          modbus_parse_info(ptr,0,&p_modbus_manager->vac_info_struct[i].installed_air_con,0,24,16);
          modbus_parse_info(ptr,0,&p_modbus_manager->vac_info_struct[i].installed_fan,0,25,16);
          
          modbus_parse_time(ptr, &sync_time, 26);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_con1_model,0,30,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_con1_type,0,31,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_con2_model,0,32,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_con2_type,0,33,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_con_on_off,0,34,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_con_mode,0,35,16);
          modbus_parse_info(ptr,0,&p_modbus_manager->vac_info_struct[i].air_con_temp,0,36,16);// xem lai
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_con_speed,0,37,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].air_cond_dir,0,38,16);
          
          p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_running_step = VAC_VIETTEL_INFO_3;
        }
        break;
      case VAC_VIETTEL_INFO_3:
        {
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].w_enable,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].w_serial1,0,1,16);
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->vac_info_struct[i].w_serial2,0,2,16);
          
          p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_running_step = VAC_VIETTEL_INFO_1;
        }
        break;
      default:
        break;
        
      }
    }
    break;
  default:
    break;
    
  }
 
}

//extract holding reg data lib-------------------
uint16_t LoadCurr, ACFaultM1, PMUConnectM1;

float TempSaft = 0;
void extract_holding_regs_data_lib(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager)
{
  uint8_t i,j;
  switch(p_modbus->slave_id - p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_slave_offset)
  {
  case 1:
    i = 0;
    break;
  case 2:
    i = 1;
    break;
  case 3:
    i = 2;
    break;
  case 4:
    i = 3;
    break;
  case 5:
    i = 4;
    break;
  case 6:
    i = 5;
    break;
  case 7:
    i = 6;
    break;
  case 8:
    i = 7;
    break;
  case 9:
    i = 8;
    break;
  case 10:
    i = 9;
    break;
  case 11:
    i = 10;
    break;
  case 12:
    i = 11;
    break;
  case 13:
    i = 12;
    break;
  case 14:
    i = 13;
    break;
  case 15:
    i = 14;
    break;
  case 16:
    i = 15;
    break;
  };
  switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_type)
  {
  case 1:// COSLIGHT
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_COSLIGHT_INFO_1:
        {
          uint16_t CH, DCH;
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellVolt[j],0,j,16);
          }
          for (j = 0; j < 4; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellTemp[j],0,16+j,16);
          }
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16AverTempCell,0,23,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,26,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,33,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&CH,0,34,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&DCH,0,35,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,36,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOH,0,37,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32CapRemain,39,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32DSGCapacity,43,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DischargeTime,0,53,16);
          //modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32Serial,0,55,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ModuleMode,0,57,16);
          if(p_modbus_manager->lib_info_struct[i].u16ModuleMode == 1) p_modbus_manager->lib_info_struct[i].u32PackCurr = CH;
          else if (p_modbus_manager->lib_info_struct[i].u16ModuleMode == 2) p_modbus_manager->lib_info_struct[i].u32PackCurr = 0 - (int32_t)DCH;
          else p_modbus_manager->lib_info_struct[i].u32PackCurr = 0;
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_INFO_2;
        }
        break;
      case LIB_COSLIGHT_INFO_2:
        {
          uint8_t alarm_CellOverVolt, alarm_PackOverVolt, alarm_VoltDiff, alarm_CellLowVolt, alarm_PackLowVolt; 
          uint8_t alarm_CharOverCurrent, alarm_DisCharOverCurrent, alarm_BattHighTemp1, alarm_BattHighTemp2, alarm_BattLowTemp1, alarm_BattLowTemp2, alarm_LowCapacity;
          uint8_t protect_CellOverVolt, protect_PackOverVolt, protect_CellLowVolt, protect_PackLowVolt, protect_OverCurr1, protect_OverCurr2; 
          uint8_t protect_ShortCircuit, protect_CharHighTemp, protect_DisCharHighTemp, protect_CharLowTemp, protect_DisCharLowTemp, protect_lost_sensor;
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_PackOverVolt,0,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_PackLowVolt,0,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_PackOverVolt,0,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_PackLowVolt,0,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CharOverCurrent,0,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_DisCharOverCurrent,0,5);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_OverCurr1,0,6);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_OverCurr2,0,7);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_LowCapacity,1,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_ShortCircuit,1,1);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CellOverVolt,2,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CellLowVolt,2,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CellOverVolt,2,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CellLowVolt,2,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattHighTemp1,2,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattLowTemp1,2,5);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CharHighTemp,2,6);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CharLowTemp,2,7);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattHighTemp2,3,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattLowTemp2,3,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_DisCharHighTemp,3,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_DisCharLowTemp,3,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_VoltDiff,3,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_lost_sensor,3,5);
          
          if(alarm_BattHighTemp2 == 1) alarm_BattHighTemp1 = 1;
          if(alarm_BattLowTemp2 == 1) alarm_BattLowTemp1 = 1;
          if(protect_OverCurr2 == 1) protect_OverCurr1 = 1;
          
          p_modbus_manager->lib_info_struct[i].u32AlarmStatus = alarm_CellOverVolt|(alarm_CellLowVolt<<1)|(alarm_PackOverVolt<<2)|(alarm_PackLowVolt<<3)|(alarm_CharOverCurrent<<4)|
            (alarm_DisCharOverCurrent<<5)|(alarm_BattHighTemp1<<6)|(alarm_BattLowTemp1<<7)|(alarm_LowCapacity<<11)|(alarm_VoltDiff<<12)|
              (protect_CellOverVolt<<16)|(protect_CellLowVolt<<17)|(protect_PackOverVolt<<18)|(protect_PackLowVolt<<19)|
                (protect_ShortCircuit<<20)|(protect_OverCurr1<<21)|(protect_CharHighTemp<<22)|(protect_CharLowTemp<<23)|(protect_DisCharHighTemp<<24)|
                  (protect_DisCharLowTemp<<25)|(protect_lost_sensor<<26);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_INFO_3;
        }
        break;
      case LIB_COSLIGHT_INFO_3:
        {
          uint16_t seri1, seri2;
          modbus_parse_info(&p_modbus->buff_read[0],0,&seri1,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&seri2,0,1,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16SoftwareVersion,0,3,16);
          p_modbus_manager->lib_info_struct[i].u32Serial = ((uint32_t)seri1<<16)|seri2;
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 2:// COSLIGHT_OLD_V1.1
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_COSLIGHT_OLD_INFO_1:
        {
          uint16_t CH, DCH;
          int16_t temp1, temp2;
          
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,&temp1,0,0,16);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,&temp2,0,1,16);
          p_modbus_manager->lib_info_struct[i].u16AverTempCell = (temp1+temp2) / 2;
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&CH,0,4,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&DCH,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,6,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOH,0,7,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32CapRemain,9,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DischargeTime,0,23,16);            
          p_modbus_manager->lib_info_struct[i].u32PackCurr = (int32_t)CH - (int32_t)DCH;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_OLD_INFO_2;
        }
        break;
      case LIB_COSLIGHT_OLD_INFO_2:
        {
          uint8_t alarm_CellOverVolt, alarm_PackOverVolt, alarm_VoltDiff, alarm_CellLowVolt, alarm_PackLowVolt; 
          uint8_t alarm_CharOverCurrent, alarm_DisCharOverCurrent, alarm_BattHighTemp1, alarm_BattHighTemp2, alarm_BattLowTemp1, alarm_BattLowTemp2, alarm_LowCapacity;
          uint8_t protect_CellOverVolt, protect_PackOverVolt, protect_CellLowVolt, protect_PackLowVolt, protect_OverCurr1, protect_OverCurr2; 
          uint8_t protect_ShortCircuit, protect_CharHighTemp, protect_DisCharHighTemp, protect_CharLowTemp, protect_DisCharLowTemp, protect_lost_sensor;
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_PackOverVolt,0,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_PackLowVolt,0,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_PackOverVolt,0,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_PackLowVolt,0,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CharOverCurrent,0,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_DisCharOverCurrent,0,5);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_OverCurr1,0,6);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_OverCurr2,0,7);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_LowCapacity,1,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_ShortCircuit,1,1);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CellOverVolt,2,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CellLowVolt,2,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CellOverVolt,2,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CellLowVolt,2,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattHighTemp1,2,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattLowTemp1,2,5);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CharHighTemp,2,6);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CharLowTemp,2,7);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattHighTemp2,3,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_BattLowTemp2,3,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_DisCharHighTemp,3,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_DisCharLowTemp,3,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_VoltDiff,3,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_lost_sensor,3,5);
          
          if(alarm_BattHighTemp2 == 1) alarm_BattHighTemp1 = 1;
          if(alarm_BattLowTemp2 == 1) alarm_BattLowTemp1 = 1;
          if(protect_OverCurr2 == 1) protect_OverCurr1 = 1;
          
          p_modbus_manager->lib_info_struct[i].u32AlarmStatus = alarm_CellOverVolt|(alarm_CellLowVolt<<1)|(alarm_PackOverVolt<<2)|(alarm_PackLowVolt<<3)|(alarm_CharOverCurrent<<4)|
            (alarm_DisCharOverCurrent<<5)|(alarm_BattHighTemp1<<6)|(alarm_BattLowTemp1<<7)|(alarm_LowCapacity<<11)|(alarm_VoltDiff<<12)|
              (protect_CellOverVolt<<16)|(protect_CellLowVolt<<17)|(protect_PackOverVolt<<18)|(protect_PackLowVolt<<19)|
                (protect_ShortCircuit<<20)|(protect_OverCurr1<<21)|(protect_CharHighTemp<<22)|(protect_CharLowTemp<<23)|(protect_DisCharHighTemp<<24)|
                  (protect_DisCharLowTemp<<25)|(protect_lost_sensor<<26);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_OLD_INFO_3;
        }
        break;
      case LIB_COSLIGHT_OLD_INFO_3:
        {
          uint16_t seri1, seri2;
          modbus_parse_info(&p_modbus->buff_read[0],0,&seri1,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&seri2,0,1,16);
          //            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16SoftwareVersion,0,3,16);
          p_modbus_manager->lib_info_struct[i].u32Serial = ((uint32_t)seri1<<16)|seri2;
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0; 
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_OLD_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 3:// SHOTO
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_SHOTO_INFO_1:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackCurr,0,1,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32CapRemain,0,2,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DSGCapacity,0,4,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOH,0,6,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DischargeTime,0,7,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16AverTempCell,0,9,16);
          p_modbus_manager->lib_info_struct[i].u16AverTempCell = (uint16_t)(p_modbus_manager->lib_info_struct[i].u16AverTempCell - 2731.5);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_SHOTO_INFO_2;
        }
        break;
      case LIB_SHOTO_INFO_2:
        {
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellVolt[j],0,j,16);
          }
          for (j = 0; j < 8; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellTemp[j],0,16+j,16);
            p_modbus_manager->lib_info_struct[i].u16CellTemp[j] =(uint16_t) (p_modbus_manager->lib_info_struct[i].u16CellTemp[j] - 2731.5);
          }
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,24,16);
          p_modbus_manager->lib_info_struct[i].u16EnvTemp =(uint16_t) (p_modbus_manager->lib_info_struct[i].u16EnvTemp - 2731.5);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_SHOTO_INFO_3;
        }
        break;
      case LIB_SHOTO_INFO_3:
        {
          uint8_t mode_DisChar, mode_Char, mode_FloatingChar, mode_FullChar, mode_Standby, mode_Turn_off;
          uint8_t alarm_CellOverVolt, alarm_PackOverVolt, alarm_CellLowVolt, alarm_PackLowVolt; 
          uint8_t alarm_CharOverCurrent, alarm_DisCharOverCurrent, alarm_CharHighTemp, alarm_DisCharHighTemp, alarm_CharLowTemp, alarm_DisCharLowTemp, alarm_LowCapacity;
          uint8_t protect_CellOverVolt, protect_PackOverVolt, protect_CellLowVolt, protect_PackLowVolt, protect_CharOverCurrent, protect_DisCharOverCurrent; 
          uint8_t protect_ShortCircuit, protect_CharHighTemp, protect_DisCharHighTemp, protect_CharLowTemp, protect_DisCharLowTemp;
          
          modbus_parse_bit(&p_modbus->buff_read[0],&mode_DisChar,0,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&mode_Char,0,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&mode_FloatingChar,0,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&mode_FullChar,0,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&mode_Standby,0,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&mode_Turn_off,0,5);
          
          if(mode_FloatingChar==1) p_modbus_manager->lib_info_struct[i].u16ModuleMode = 1;
          else if(mode_FullChar==1) p_modbus_manager->lib_info_struct[i].u16ModuleMode = 1;
          else if(mode_Standby==1) p_modbus_manager->lib_info_struct[i].u16ModuleMode = 3;
          else if(mode_DisChar==1) p_modbus_manager->lib_info_struct[i].u16ModuleMode = 2;
          else if(mode_Char==1) p_modbus_manager->lib_info_struct[i].u16ModuleMode = 1;
          else p_modbus_manager->lib_info_struct[i].u16ModuleMode = 4;
          
          
          
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CellOverVolt,1,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CellOverVolt,1,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CellLowVolt,1,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CellLowVolt,1,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_PackOverVolt,1,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_PackOverVolt,1,5);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_PackLowVolt,1,6);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_PackLowVolt,1,7);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CharHighTemp,2,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CharHighTemp,2,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CharLowTemp,2,2);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CharLowTemp,2,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_DisCharHighTemp,2,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_DisCharHighTemp,2,5);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_DisCharLowTemp,2,6);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_DisCharLowTemp,2,7);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_CharOverCurrent,4,0);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_CharOverCurrent,4,1);
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_DisCharOverCurrent,4,3);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_DisCharOverCurrent,4,4);
          modbus_parse_bit(&p_modbus->buff_read[0],&protect_ShortCircuit,4,6);
          
          modbus_parse_bit(&p_modbus->buff_read[0],&alarm_LowCapacity,6,2);
          
          if(alarm_DisCharHighTemp == 1) alarm_CharHighTemp = 1;
          if(alarm_DisCharLowTemp == 1) alarm_CharLowTemp = 1;
          if(protect_DisCharOverCurrent == 1) protect_CharOverCurrent = 1;
          
          p_modbus_manager->lib_info_struct[i].u32AlarmStatus = alarm_CellOverVolt|(alarm_CellLowVolt<<1)|(alarm_PackOverVolt<<2)|(alarm_PackLowVolt<<3)|(alarm_CharOverCurrent<<4)|
            (alarm_DisCharOverCurrent<<5)|(alarm_CharHighTemp<<6)|(alarm_CharLowTemp<<7)|(alarm_LowCapacity<<11)|
              (protect_CellOverVolt<<16)|(protect_CellLowVolt<<17)|(protect_PackOverVolt<<18)|(protect_PackLowVolt<<19)|
                (protect_ShortCircuit<<20)|(protect_CharOverCurrent<<21)|(protect_CharHighTemp<<22)|(protect_CharLowTemp<<23)|(protect_DisCharHighTemp<<24)|
                  (protect_DisCharLowTemp<<25);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_SHOTO_INFO_4;
        }
        break;
      case LIB_SHOTO_INFO_4:
        {
          uint16_t temp; 
          modbus_parse_info(&p_modbus->buff_read[0], 0, &temp,0,0,16);
          p_modbus_manager->lib_info_struct[i].u8swversion[1] =  (uint8_t) temp;
          p_modbus_manager->lib_info_struct[i].u8swversion[0] = (uint8_t) (temp>>8);
          modbus_parse_info_string(&p_modbus->buff_read[0],&p_modbus_manager->lib_info_struct[i].u8mode[0], 20, 1,2);
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_SHOTO_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 4:// HUAWEI
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_HUAWEI_INFO_1:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32BattVolt,0,1,16);            
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,(int16_t *)&p_modbus_manager->lib_info_struct[i].u16PackCurr,0,2,16);
          //            if (p_modbus_manager->lib_info_struct[i].u16PackCurr > 0xF000)
          //            {
          //              p_modbus_manager->lib_info_struct[i].u16PackCurr = ~p_modbus_manager->lib_info_struct[i].u16PackCurr + 1;
          //            }
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOH,0,4,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16AverTempCell,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,6,16);
          
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ModuleMode,0,14,16);
          
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellTemp[j],0,18+j,16);
          }
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellVolt[j],0,34+j,16);
          }
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_INFO_2;
        }
        break;
      case LIB_HUAWEI_INFO_2:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DischargeTime,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16Protect1,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16Protect2,0,4,16);            
          p_modbus_manager->lib_info_struct[i].u32ProtectStatus = (p_modbus_manager->lib_info_struct[i].u16Protect1 << 16) | p_modbus_manager->lib_info_struct[i].u16Protect2;
          
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16MajorAlarm,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16MinorAlarm,0,6,16);            
          p_modbus_manager->lib_info_struct[i].u32AlarmStatus = (p_modbus_manager->lib_info_struct[i].u16MajorAlarm << 16) | p_modbus_manager->lib_info_struct[i].u16MinorAlarm;
          
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ModuleAlarm,0,7,16);
          p_modbus_manager->lib_info_struct[i].u32FaultStatus = (p_modbus_manager->lib_info_struct[i].u16ModuleMode << 16) | p_modbus_manager->lib_info_struct[i].u16ModuleAlarm;
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_INFO_3;
        }
        break;
      case LIB_HUAWEI_INFO_3:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16SoftwareVersion,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16HardwareVersion,0,1,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32CapRemain,0,6,16);
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_INFO_4;
        }
        break;
      case LIB_HUAWEI_INFO_4:
        {
          modbus_parse_info_string(&p_modbus->buff_read[0],&p_modbus_manager->lib_info_struct[i].u8mode[0], 12, 0,2);
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 5:// M1Viettel50
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_VIETTEL_INFO_1:
        {
          uint16_t tempsw;
          
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32ADCPackVolt,0,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32ADCBattVolt,2,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32PackVolt,4,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32ADCPackCurr,6,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32ADCBattCurr,8,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32PackCurr,10,32);
          
          for (j = 0; j < 15; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellVolt[j],0,12+j,16);
          }
          
          for (j = 0; j < 4; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellTemp[j],0,27+j,16);
          }
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,31,16); 
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16AverTempCell,0,35,16); 
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32AlarmStatus,36,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32ProtectStatus,38,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32FaultStatus,40,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ModuleMode,0,42,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32SOC,43,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32SOH,45,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32DischargeTime,47,32);
          
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ConverterState,0,51,16); 
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ErrCode,0,52,16); 
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16BalanceStatus,0,53,16); 
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16MosfetMode,0,55,16); 
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32Mcu2McuErr,58,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32CHGCapacity,62,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32DSGCapacity,64,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32Efficiency,66,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16NumberOfCan,0,68,16);
          
          modbus_parse_info(&p_modbus->buff_read[0], 0, &p_modbus_manager->lib_info_struct[i].u16BattType,0,80,16);
          modbus_parse_info(&p_modbus->buff_read[0], 0, &tempsw,0,81,16);
          p_modbus_manager->lib_info_struct[i].sw_ver         = tempsw;// thanhcm3 fix
          p_modbus_manager->lib_info_struct[i].u8swversion[1] =  (uint8_t) tempsw;
          p_modbus_manager->lib_info_struct[i].u8swversion[0] = (uint8_t) (tempsw>>8);  
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32Serial,82,32);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_VIETTEL_INFO_2;
        }
        break;
      case LIB_VIETTEL_INFO_2:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16SOCMaxim,0,2,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16SOHMaxim,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16FullCapRepMaxim,0,4,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16VoltMaxim,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CurrMaxim,0,6,16);
          if(p_modbus_manager->lib_info_struct[i].sw_ver == 110){
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32IKalamn,8,32);
            //
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32SOCKalamn,12,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32VpriKalamn,14,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32VmesKalamn,16,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32CapKalamn,18,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32InternalR0Kalamn,20,32);
            p_modbus_manager->lib_info_struct[i].u32CapKalamn = p_modbus_manager->lib_info_struct[i].u32CapKalamn*10;
            
          }else{
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32IKalamn,8,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32SOCKalamn,10,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32VpriKalamn,12,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32VmesKalamn,14,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32CapKalamn,16,32);
            modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32InternalR0Kalamn,18,32);
            //modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32InternalR0Kalamn,20,32);//thanhcm3 fix
            float temp_cap_kalamn;
            temp_cap_kalamn = (float) p_modbus_manager->lib_info_struct[i].u32CapKalamn;
            temp_cap_kalamn = temp_cap_kalamn/10000.0;
            p_modbus_manager->lib_info_struct[i].u32CapKalamn =(uint32_t)temp_cap_kalamn;
          }
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_VIETTEL_INFO_3;
        }
        break;
      case LIB_VIETTEL_INFO_3:
        {
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32PackHighVolt_A,0,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BattHighVolt_A,2,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CellHighVolt_A,4,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BattLowVolt_A,6,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CellLowVolt_A,8,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CharHighCurr_A,10,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32DisCharHighCurr_A,12,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CharHighTemp_A,14,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32DisCharHighTemp_A,16,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CharLowTemp_A,18,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32DisCharLowtemp_A,20,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32LowCap_A,22,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BMSHighTemp_A,24,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BMSLowTemp_A,26,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32PackOverVolt_P,28,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BattOverVolt_P,30,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CellOverVolt_P,32,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BattUnderVolt_P,34,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CellUnderVolt_P,36,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CharOverCurr_P,38,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32DisCharOverCurr_P,40,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CharOverTemp_P,42,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32DisCharOverTemp_P,44,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32CharUnderTemp_P,46,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32DisCharUnderTemp_P,48,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32UnderCap_P,50,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BMSOverTemp_P,52,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32BMSUnderTemp_P,54,32);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].i32DifferentVolt_P,56,32);
          
          
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32VoltBalance,60,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32DeltaVoltBalance,62,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32DisCharCurrLimit,64,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32CharCurrLimit,66,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32VoltDisCharRef,72,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32VoltDisCharCMD,74,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16SystemByte,0,76,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16KeyTime,0,77,16);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_VIETTEL_INFO_4;
        }
        break;
      case LIB_VIETTEL_INFO_4:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16IniMaxim,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16EnableKCS,0,1,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16_liionCapInit,0,2,16);
          
          //          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_VIETTEL_INFO_5;
        }
        break;
      case LIB_VIETTEL_INFO_5:
        {
          uint8_t u8Key[10];
          uint32_t Key2 = 0;
          uint16_t u16Temp;
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16EnableLock,0,5,16);
          for(j=0;j<10;j++)
            u8Key[j] = p_modbus->buff_read[3+j];
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&Key2,3,32);
          Key2 += 95;
          u8Key[6] = (uint8_t)(Key2>>24);
          u8Key[7] = (uint8_t)(Key2>>16);
          u8Key[8] = (uint8_t)(Key2>>8);
          u8Key[9] = (uint8_t)Key2;
          vTaskDelay(500);
          init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
          mater_write_multi_reg(p_modbus,p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id, 116, &u8Key[0], 5,4500);
          vTaskDelay(500);
          if(PMUConnectM1 == 1)
            u8Key[1] = 0x00;
          else {
            if(ACFaultM1 == 0)
              u8Key[1] = 0x19; 
            else
              u8Key[1] = 0x11;
          }
          u8Key[0] = 0;
          
          init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
          mater_write_multi_reg (p_modbus,p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id, 76, &u8Key[0], 1,4500); // System Byte
          
          vTaskDelay(500);
          u16Temp = (uint16_t)LoadCurr;
          u8Key[0] = (uint8_t)(u16Temp>>8);
          u8Key[1] = (uint8_t)(u16Temp);
          mater_write_multi_reg (p_modbus,p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id, 78, &u8Key[0], 1,4500); // Total Curr
          vTaskDelay(500);
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_VIETTEL_INFO_1;
        }
        break;
      default:
        break;
      };
      
    }
    break;
  case 6:// ZTT_New
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_ZTT_2020_INFO_1:
        {
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,(int16_t *)&p_modbus_manager->lib_info_struct[i].u16PackCurr,0,0,16);      
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,1,16);        
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,2,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOH,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32CapRemain,0,4,16);       
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DischargeTime,0,7,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32AlarmStatus,0,9,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32ProtectStatus,0,10,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32FaultStatus,0,11,16);
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellVolt[j],0,15+j,16);
          }
          for (j = 0; j < 4; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellTemp[j],0,31+j,16);
          }
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,36,16);      
          p_modbus_manager->lib_info_struct[i].u16AverTempCell = (p_modbus_manager->lib_info_struct[i].u16CellTemp[0] + p_modbus_manager->lib_info_struct[i].u16CellTemp[1] + p_modbus_manager->lib_info_struct[i].u16CellTemp[2] + p_modbus_manager->lib_info_struct[i].u16CellTemp[3])/4; 
          
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32DSGCapacity,40,32);
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_ZTT_2020_INFO_2;
        }
        break;
      case LIB_ZTT_2020_INFO_2:
        {
          modbus_parse_info_string(&p_modbus->buff_read[0],&p_modbus_manager->lib_info_struct[i].u8swversion[0], 20, 0,2);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_ZTT_2020_INFO_3;
        }
        break;
      case LIB_ZTT_2020_INFO_3:
        {
          modbus_parse_info_string(&p_modbus->buff_read[0],&p_modbus_manager->lib_info_struct[i].u8mode[0], 20, 0,2);
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_ZTT_2020_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 9:// SAFT
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_SAFT_INFO_1:
        {
          uint16_t alarm1, alarm2, alarm3, tempSaft;
          
          uint8_t protect_CellOverVolt, protect_PackOverVolt, protect_CellLowVolt, protect_PackLowVolt, protect_CharOverCurrent/*,protect_DisCharOverCurrent*/; 
          uint8_t protect_ShortCircuit, protect_CharHighTemp, protect_DisCharHighTemp, protect_CharLowTemp, protect_DisCharLowTemp;
          
          modbus_parse_info_inverse(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,32);
          modbus_parse_info_inverse(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32PackCurr,2,32);
          modbus_parse_info(&p_modbus->buff_read[0],0,&alarm1,0,8,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&alarm2,0,9,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&alarm3,0,10,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&tempSaft,0,12,16);
          p_modbus_manager->lib_info_struct[i].u32SOH = tempSaft & 0x00FF;
          modbus_parse_info(&p_modbus->buff_read[0],0,&tempSaft,0,14,16);
          p_modbus_manager->lib_info_struct[i].u32SOC = tempSaft & 0x00FF;
          p_modbus_manager->lib_info_struct[i].u16AverTempCell = (tempSaft & 0xFF00) >> 8;
          
          protect_ShortCircuit = (uint8_t)((alarm1&0x0002) >> 1); 
          if((alarm1&0x000C) != 0x0000){
            if(p_modbus_manager->lib_info_struct[i].u32PackCurr > 0){
              protect_CharHighTemp = 1;
              protect_DisCharHighTemp = 0;
            } else {
              protect_CharHighTemp = 0;
              protect_DisCharHighTemp = 1;
            }
          } else {
            protect_CharHighTemp = 0;
            protect_DisCharHighTemp = 0;
          } 
          if((alarm1&0x0030) != 0x0000){
            if(p_modbus_manager->lib_info_struct[i].u32PackCurr > 0){
              protect_CharLowTemp = 1;
              protect_DisCharLowTemp = 0;
            } else {
              protect_CharLowTemp = 0;
              protect_DisCharLowTemp = 1;
            }
          } else {
            protect_CharLowTemp = 0;
            protect_DisCharLowTemp = 0;
          }
          if((alarm1&0x00C0) != 0x0000)
            protect_CellOverVolt = 1;
          else
            protect_CellOverVolt = 0;
          if((alarm1&0x0300) != 0x0000)
            protect_CellLowVolt = 1;
          else
            protect_CellLowVolt = 0;
          if(((alarm1&0x8000) != 0x0000)|((alarm2&0x0001) != 0x0000))
            protect_CharOverCurrent = 1;
          else
            protect_CharOverCurrent = 0;
          if((alarm2&0x0080) != 0x0000)
            protect_PackOverVolt = 1;
          else
            protect_PackOverVolt = 0;
          if((alarm2&0x0100) != 0x0000)
            protect_PackLowVolt = 1;
          else
            protect_PackLowVolt = 0;
          p_modbus_manager->lib_info_struct[i].u32AlarmStatus = (protect_CellOverVolt<<16)|(protect_CellLowVolt<<17)|(protect_PackOverVolt<<18)|(protect_PackLowVolt<<19)|
            (protect_ShortCircuit<<20)|(protect_CharOverCurrent<<21)|(protect_CharHighTemp<<22)|(protect_CharLowTemp<<23)|(protect_DisCharHighTemp<<24)|
              (protect_DisCharLowTemp<<25);
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_SAFT_INFO_1;
        }
        break;
      default:
        break;
      }
    }
    break;
  case 10:// Narada75
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_NARADA75_INFO_1:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,0,16);  
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,(int16_t *)&p_modbus_manager->lib_info_struct[i].u32PackCurr,0,1,16);      
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32CapRemain,0,2,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16AverTempCell,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,4,16); 
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32AlarmStatus,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32ProtectStatus,0,6,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32FaultStatus,0,7,16);           
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,8,16);
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_NARADA75_INFO_2;
        }
        break;
      case LIB_NARADA75_INFO_2:
        {
          char str1[] = "Product Style";
          char str2[] = "Serial Number Version";
          char serial[20] = {0};
          char *pdest1, *pdest2;
          pdest1 = strstr((const char*)p_modbus->buff_read,str1);
          int j = 0;
          while(*pdest1 != 'S')
          {
            p_modbus_manager->lib_info_struct[i].u8mode[j++] =  *pdest1;
            pdest1++;
          }
          p_modbus_manager->lib_info_struct[i].u8mode[j] = '\0';
          j = 0;
          pdest2 = strstr((const char*)p_modbus->buff_read,str2);
          while(*pdest2 != '\0')
          {
            serial[j++] =  *pdest2;
            pdest2++;
          }       
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_NARADA75_INFO_1;
        }
        break;
      default:
        break;
      };
      
    }
    break;
  case 7:// ZTT50
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_ZTT_INFO_1:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,0,16);
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,(int16_t *)&p_modbus_manager->lib_info_struct[i].u32PackCurr,0,1,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32CapRemain,0,2,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16AverTempCell,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,4,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32AlarmStatus,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32ProtectStatus,0,6,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32FaultStatus,0,7,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,8,16);
          
          p_modbus_manager->lib_info_struct[i].fPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt * 0.01;
          p_modbus_manager->lib_info_struct[i].fPackCurr = (p_modbus_manager->lib_info_struct[i].u32PackCurr - 10000) * 0.1;
          p_modbus_manager->lib_info_struct[i].fCapRemain = p_modbus_manager->lib_info_struct[i].u32CapRemain * 0.1;
          p_modbus_manager->lib_info_struct[i].fAverTempCell = (p_modbus_manager->lib_info_struct[i].u16AverTempCell - 400) * 0.1;
          p_modbus_manager->lib_info_struct[i].fEnvTemp = (p_modbus_manager->lib_info_struct[i].u16EnvTemp -400) * 0.1;
          p_modbus_manager->lib_info_struct[i].fSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 0.01;
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_ZTT_INFO_2;
        }
        break;
      case LIB_ZTT_INFO_2:
        {
          uint8_t sw1, sw2, hw1, hw2, hw3, hw4, hw5;
          uint8_t *s_temp;
          s_temp = (uint8_t*)strchr((const char*)p_modbus->buff_read,'*');
          sw1 = s_temp[1];
          sw2 = s_temp[2];
          hw1 = s_temp[4];
          hw2 = s_temp[5];
          hw3 = s_temp[6];
          hw4 = s_temp[7];
          hw5 = s_temp[8];
          uint8_t j=0;
          for( j=0;j<20;j++)
          {
            p_modbus_manager->lib_info_struct[i].u8mode[j] = (uint8_t)s_temp[j+10];
          }
          p_modbus_manager->lib_info_struct[i].u8mode[j+1] = '\0';
          
          sprintf((char *)p_modbus_manager->lib_info_struct[i].u8swversion,"V%2d.%2d",sw1,sw2);
          sprintf((char *)p_modbus_manager->lib_info_struct[i].u8hwversion,"V%2d.%2d.%2d.%2d.%2d",hw1,hw2,hw3,hw4,hw5);
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_ZTT_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 8:// HUAWEI_A1
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_HUAWEI_A1_INFO_1:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32BattVolt,0,1,16);            
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,(int16_t *)&p_modbus_manager->lib_info_struct[i].u16PackCurr,0,2,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOH,0,4,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16AverTempCell,0,5,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,6,16);
          
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ModuleMode,0,14,16);
          
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellTemp[j],0,18+j,16);
          }
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellVolt[j],0,34+j,16);
          }
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_A1_INFO_2;
        }
        break;
      case LIB_HUAWEI_A1_INFO_2:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DischargeTime,0,0,16);
          //            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16Protect1,0,3,16);
          //            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16Protect2,0,4,16);            
          //            p_modbus_manager->lib_info_struct[i].u32ProtectStatus = (p_modbus_manager->lib_info_struct[i].u16Protect1 << 16) | p_modbus_manager->lib_info_struct[i].u16Protect2;
          //            
          //            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16MajorAlarm,0,5,16);
          //            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16MinorAlarm,0,6,16);            
          //            p_modbus_manager->lib_info_struct[i].u32AlarmStatus = (p_modbus_manager->lib_info_struct[i].u16MajorAlarm << 16) | p_modbus_manager->lib_info_struct[i].u16MinorAlarm;
          //            
          //            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16ModuleAlarm,0,7,16);
          //            p_modbus_manager->lib_info_struct[i].u32FaultStatus = (p_modbus_manager->lib_info_struct[i].u16ModuleMode << 16) | p_modbus_manager->lib_info_struct[i].u16ModuleAlarm;
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_A1_INFO_3;
        }
        break;
      case LIB_HUAWEI_A1_INFO_3:
        {
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16SoftwareVersion,0,0,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16HardwareVersion,0,1,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32CapRemain,0,6,16);
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_A1_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 11:// EVE
    {
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_EVE_INFO_1:
        {
          modbus_parse_info_sign(&p_modbus->buff_read[0],0,(int16_t *)&p_modbus_manager->lib_info_struct[i].u16PackCurr,0,0,16);      // x100
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32PackVolt,0,1,16);         // x100
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOC,0,2,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32SOH,0,3,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32CapRemain,0,4,16);        // x100
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32DischargeTime,0,7,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32AlarmStatus,0,9,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32ProtectStatus,0,10,16);
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u32FaultStatus,0,11,16);
          for (j = 0; j < 16; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellVolt[j],0,15+j,16);
          }
          for (j = 0; j < 4; j++)
          {
            modbus_parse_info(&p_modbus->buff_read[0],0,&p_modbus_manager->lib_info_struct[i].u16CellTemp[j],0,31+j,16);
          }
          modbus_parse_info(&p_modbus->buff_read[0],0,(uint16_t *)&p_modbus_manager->lib_info_struct[i].u16EnvTemp,0,36,16);      
          p_modbus_manager->lib_info_struct[i].u16AverTempCell = (p_modbus_manager->lib_info_struct[i].u16CellTemp[0] + p_modbus_manager->lib_info_struct[i].u16CellTemp[1] + p_modbus_manager->lib_info_struct[i].u16CellTemp[2] + p_modbus_manager->lib_info_struct[i].u16CellTemp[3])/4; 
          
          modbus_parse_info(&p_modbus->buff_read[0],0,0,&p_modbus_manager->lib_info_struct[i].u32DSGCapacity,40,32);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_EVE_INFO_2;
        }
        break;
      case LIB_EVE_INFO_2:
        {
          modbus_parse_info_string(&p_modbus->buff_read[0],&p_modbus_manager->lib_info_struct[i].u8swversion[0], 20, 0,2);
          
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_EVE_INFO_3;
        }
        break;
      case LIB_EVE_INFO_3:
        {
          modbus_parse_info_string(&p_modbus->buff_read[0],&p_modbus_manager->lib_info_struct[i].u8mode[0], 20, 0,2);
          
          p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 0;
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_EVE_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  };
}

//extract holding reg data gen --------------------------------------------------
void extract_holding_regs_data_gen(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager)
{
  uint8_t i;
  uint8_t *ptr = &p_modbus->buff_read[0];
  switch(p_modbus->slave_id - p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_slave_offset)
  {
  case 1:
    i = 0;
    break;
  case 2:
    i = 1;
    break;
  };
  switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_type)
  {
  case 1:// KUBOTA
    {
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_KUBOTA_INFO_1:
        {
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32GenFrequency,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32MainFrequency,0,1,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineSpeed,0,2,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32OilPressure,0,6,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32CoolantTemp,0,7,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32FuelLevel,0,8,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32BattVolt,0,9,16);
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour,14,32);
          
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr1,0,16,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr2,0,17,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr3,0,18,16);
          
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt1,0,26,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt2,0,27,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt3,0,28,16);
          
          modbus_parse_info_f_inverse(ptr, &p_modbus_manager->gen_info_struct[i].fPF1, 41,1);
          modbus_parse_info_f_inverse(ptr, &p_modbus_manager->gen_info_struct[i].fPF2, 43,1);
          modbus_parse_info_f_inverse(ptr, &p_modbus_manager->gen_info_struct[i].fPF3, 45,1);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin,0,48,16);            
          
          modbus_parse_info_inverse(ptr,0,0, &p_modbus_manager->gen_info_struct[i].u32ActiveEnergyLow,77,32);
          modbus_parse_info_inverse(ptr,0,0, &p_modbus_manager->gen_info_struct[i].u32ActiveEnergyHigh,79,32);
          
          //            p_modbus_manager->gen_info_struct[i].u32LNPower1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * p_modbus_manager->gen_info_struct[i].u32LNCurr1 * p_modbus_manager->gen_info_struct[i].fPF1 * 0.01;
          //            p_modbus_manager->gen_info_struct[i].u32LNPower2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * p_modbus_manager->gen_info_struct[i].u32LNCurr2 * p_modbus_manager->gen_info_struct[i].fPF2 * 0.01;
          //            p_modbus_manager->gen_info_struct[i].u32LNPower3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * p_modbus_manager->gen_info_struct[i].u32LNCurr3 * p_modbus_manager->gen_info_struct[i].fPF3 * 0.01;
          
          modbus_parse_info_inverse(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNSPower1,53,32);   
          modbus_parse_info_inverse(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNSPower2,55,32);
          modbus_parse_info_inverse(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNSPower3,57,32);
          
          modbus_parse_info_inverse(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower1,61,32);   
          modbus_parse_info_inverse(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower2,63,32);
          modbus_parse_info_inverse(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower3,65,32);
          
          modbus_parse_info(ptr,0,&p_modbus_manager->gen_info_struct[i].u16Error1Status,0,32,16);
          modbus_parse_info(ptr,0,&p_modbus_manager->gen_info_struct[i].u16Error2Status,0,35,16);
          
          modbus_parse_info(ptr,0,&p_modbus_manager->gen_info_struct[i].u16StatusBits1,0,37,16);
          modbus_parse_info(ptr,0,&p_modbus_manager->gen_info_struct[i].u16StatusBits2,0,38,16);
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_KUBOTA_INFO_2;
        }
        break;
      case GEN_KUBOTA_INFO_2:
        {             
          uint16_t u16temp1, u16temp2 ;
          modbus_parse_info(ptr,0,&u16temp1,0,0,16);
          u16temp2 = u16temp1;
          p_modbus_manager->gen_info_struct[i].u16ErrorBattVolt = 0;
          if(((u16temp1 >> 1) & 0x01) == 1) p_modbus_manager->gen_info_struct[i].u16ErrorBattVolt = 1;
          if(((u16temp2 >> 2) & 0x01) == 1) p_modbus_manager->gen_info_struct[i].u16ErrorBattVolt = 2;   
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_KUBOTA_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 2:// BE142
    {
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_BE142_INFO_1:
        {
          
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt1,0,0,16); 
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr1,0,6,16);             
          //            
          //            modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32MainFrequency,0,6,16);         
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32GenFrequency,0,9,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32BattVolt,0,30,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineSpeed,0,32,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32OilPressure,0,34,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32CoolantTemp,0,35,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32FuelLevel,0,36,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour,0,37,16);
          
          //            modbus_parse_info(ptr,0,&p_modbus_manager->gen_info_struct[i].u16Error1Status,0,32,16);
          //            modbus_parse_info(ptr,0,&p_modbus_manager->gen_info_struct[i].u16Error2Status,0,35,16);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_BE142_INFO_2;
        }
        break;
      case GEN_BE142_INFO_2:
        {             
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr2,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr3,0,1,16); 
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt2,0,2,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt3,0,3,16); 
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_BE142_INFO_3;
        }
        break;
      case GEN_BE142_INFO_3:
        {         
          uint16_t gen_high_frq, gen_low_frq, gen_high_vol, gen_low_vol, high_curr, high_curr1, high_curr2, low_oil_press, low_oil_press1, low_oil_press2, high_coolant_temp,
          high_coolant_temp1, high_coolant_temp2, low_batt_vol, high_bat_vol, low_fuel;
          
          modbus_parse_info(ptr,0,&gen_low_frq,0,0,16);
          modbus_parse_info(ptr,0,&gen_high_frq,0,1,16);
          modbus_parse_info(ptr,0,&gen_high_vol,0,3,16);
          modbus_parse_info(ptr,0,&gen_low_vol,0,4,16);
          modbus_parse_info(ptr,0,&high_curr1,0,10,16);
          modbus_parse_info(ptr,0,&high_curr2,0,11,16);
          if((high_curr1)||(high_curr2))
            high_curr = 1;
          else
            high_curr = 0;
          modbus_parse_info(ptr,0,&low_oil_press1,0,14,16);
          modbus_parse_info(ptr,0,&low_oil_press2,0,15,16);
          if((low_oil_press1)||(low_oil_press2))
            low_oil_press = 1;
          else
            low_oil_press = 0;
          modbus_parse_info(ptr,0,&high_coolant_temp1,0,18,16);
          modbus_parse_info(ptr,0,&high_coolant_temp2,0,21,16);
          if((high_coolant_temp1)||(high_coolant_temp2))
            high_coolant_temp = 1;
          else
            high_coolant_temp = 0;
          modbus_parse_info(ptr,0,&low_batt_vol,0,23,16);
          modbus_parse_info(ptr,0,&high_bat_vol,0,24,16);
          modbus_parse_info(ptr,0,&low_fuel,0,26,16);
          
          p_modbus_manager->gen_info_struct[i].u32AlarmStatus = low_fuel|(gen_low_frq<<1)|(gen_high_frq<<2)|(high_coolant_temp<<3)|(gen_low_vol<<4)|
            (gen_high_vol<<5)|(low_oil_press<<6)|(high_bat_vol<<7)|(low_batt_vol<<8)|(high_curr<<27);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_BE142_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 3:// DEEPSEA
    {
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_DEEPSEA_INFO_1:
        {
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32OilPressure,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32CoolantTemp,0,1,16);
          //thanhcm3 fix -----------------------------------------------------------------------------------
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].oil_temperature,0,2,16);
          //thanhcm3 fix ----------------------------------------------------------------------------------
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32FuelLevel,0,3,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32BattVolt,0,5,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineSpeed,0,6,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32GenFrequency,0,7,16);
          
          //            modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour,14,32);   
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNVolt1,8,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNVolt2,10,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNVolt3,12,32);
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNCurr1,20,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNCurr2,22,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNCurr3,24,32);
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower1,28,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower2,30,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower3,32,32);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_DEEPSEA_INFO_2;
        }
        break;
      case GEN_DEEPSEA_INFO_2:
        {      
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin,6,32);  
          p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin / 60;
          p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin / 60;
          p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin % 60;
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_DEEPSEA_INFO_3;
        }
        break;
      case GEN_DEEPSEA_INFO_3:
        {             
          uint16_t u16_temp;
          uint8_t gen_high_vol, gen_low_vol, gen_low_frq, gen_high_frq, high_curr, low_fuel, high_coolant_temp, low_oil_press, low_batt_vol, high_bat_vol,emergency_stop; 
          
          modbus_parse_info(ptr,0,&u16_temp,0,1,16);
          emergency_stop = (uint8_t)((u16_temp>>12)&0x0F);           // emergency_stop
          if((emergency_stop==2)||(emergency_stop==3)) emergency_stop = 1;
          else emergency_stop = 0;
          low_oil_press = (uint8_t)((u16_temp>>8)&0x0F);     // Low_oil_press
          if((low_oil_press==2)||(low_oil_press==3)) low_oil_press = 1;
          else low_oil_press = 0;
          high_coolant_temp = (uint8_t)((u16_temp>>4)&0x0F); // high_coolant_temp
          if((high_coolant_temp==2)||(high_coolant_temp==3)) high_coolant_temp = 1; 
          else high_coolant_temp = 0; 
          
          modbus_parse_info(ptr,0,&u16_temp,0,3,16);
          gen_low_vol = (uint8_t)((u16_temp>>8)&0x0F);       // gen_low_vol
          if((gen_low_vol==2)||(gen_low_vol==3)) gen_low_vol = 1;
          else gen_low_vol = 0;
          gen_high_vol = (uint8_t)((u16_temp>>4)&0x0F);      // gen_high_vol
          if((gen_high_vol==2)||(gen_high_vol==3)) gen_high_vol = 1;
          else gen_high_vol = 0;
          gen_low_frq = (uint8_t)(u16_temp&0x0F);      // gen_low_frq
          if((gen_low_frq==2)||(gen_low_frq==3)) gen_low_frq = 1;     
          else gen_low_frq = 0;
          
          modbus_parse_info(ptr,0,&u16_temp,0,4,16);
          gen_high_frq = (uint8_t)((u16_temp>>12)&0x0F);           // gen_high_frq
          if((gen_high_frq==2)||(gen_high_frq==3)) gen_high_frq = 1;
          else gen_high_frq = 0;
          high_curr = (uint8_t)((u16_temp>>8)&0x0F);         // high_curr
          if((high_curr==2)||(high_curr==3)) high_curr = 1;
          else high_curr = 0;
          
          modbus_parse_info(ptr,0,&u16_temp,0,7,16);
          low_batt_vol = (uint8_t)((u16_temp>>12)&0x0F);           // low_batt_vol
          if((low_batt_vol==2)||(low_batt_vol==3)) low_batt_vol = 1;
          else low_batt_vol = 0;      
          high_bat_vol = (uint8_t)((u16_temp>>8)&0x0F);      // high_bat_vol
          if((high_bat_vol==2)||(high_bat_vol==3)) high_bat_vol = 1;
          else high_bat_vol = 0;
          low_fuel = (uint8_t)((u16_temp>>4)&0x0F);          // low_fuel
          if((low_fuel==2)||(low_fuel==3)) low_fuel = 1;
          else low_fuel = 0;
          
          p_modbus_manager->gen_info_struct[i].u32AlarmStatus = low_fuel|(gen_low_frq<<1)|(gen_high_frq<<2)|(high_coolant_temp<<3)|(gen_low_vol<<4)|
            (gen_high_vol<<5)|(low_oil_press<<6)|(high_bat_vol<<7)|(low_batt_vol<<8)|(emergency_stop<<16)|(high_curr<<27);
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_DEEPSEA_INFO_4;
        }
        break;
      case GEN_DEEPSEA_INFO_4:
        {      
          modbus_parse_info(ptr,0,&p_modbus_manager->gen_info_struct[i].u16StatusBits1,0,0,16);
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_DEEPSEA_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 4:// LR2057
    {
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_LR2057_INFO_1:
        {
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32MainFrequency,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt1,0,1,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt2,0,2,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt3,0,3,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32GenFrequency,0,7,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr1,0,8,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr2,0,9,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr3,0,10,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32CoolantTemp,0,11,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32OilPressure,0,13,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32FuelLevel,0,15,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineSpeed,0,17,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32BattVolt,0,18,16);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_LR2057_INFO_2;
        }
        break;
      case GEN_LR2057_INFO_2:
        {      
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour,0,32);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin,0,2,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNPower1,0,10,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNPower2,0,11,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNPower3,0,12,16);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_LR2057_INFO_3;
        }
        break;
      case GEN_LR2057_INFO_3:
        {      
          uint8_t gen_high_vol, gen_low_vol, gen_low_frq, gen_high_frq, high_curr, low_fuel, low_oil_press, low_batt_vol, high_bat_vol,high_temp; 
          uint8_t gen_high_vol_1, gen_low_vol_1, gen_low_frq_1, gen_high_frq_1, high_curr_1, high_curr_2, low_fuel_1, low_oil_press_1,high_temp1; 
          modbus_parse_bit(ptr,&gen_high_frq,0,0);
          modbus_parse_bit(ptr,&gen_low_frq,0,1);
          modbus_parse_bit(ptr,&gen_high_vol,0,2);
          modbus_parse_bit(ptr,&gen_low_vol,0,3);
          modbus_parse_bit(ptr,&high_curr,0,4);
          modbus_parse_bit(ptr,&high_temp,0,6);
          modbus_parse_bit(ptr,&low_oil_press,0,7);
          modbus_parse_bit(ptr,&low_fuel,1,2);
          modbus_parse_bit(ptr,&high_temp1,1,4);
          modbus_parse_bit(ptr,&low_oil_press_1,1,5);
          modbus_parse_bit(ptr,&high_curr_1,1,6);
          modbus_parse_bit(ptr,&low_fuel_1,2,0);
          modbus_parse_bit(ptr,&low_batt_vol,2,2);
          modbus_parse_bit(ptr,&high_bat_vol,2,3);
          modbus_parse_bit(ptr,&gen_high_vol_1,7,5);
          modbus_parse_bit(ptr,&gen_low_vol_1,7,6);
          modbus_parse_bit(ptr,&gen_high_frq_1,7,7);
          modbus_parse_bit(ptr,&gen_low_frq_1,8,0);
          modbus_parse_bit(ptr,&high_curr_2,8,1);
          
          if(gen_high_vol_1 == 1) gen_high_vol = 1;
          if(gen_low_vol_1 == 1) gen_low_vol = 1;
          if(gen_low_frq_1 == 1) gen_low_frq = 1;
          if(gen_high_frq_1 == 1) gen_high_frq = 1;
          if(high_temp1 == 1) high_temp = 1;
          if(low_fuel_1 == 1) low_fuel = 1;
          if(low_oil_press_1 == 1) low_oil_press = 1;
          if((high_curr_1 == 1) || (high_curr_2 == 1)) high_curr = 1;
          
          p_modbus_manager->gen_info_struct[i].u32AlarmStatus = low_fuel|(gen_low_frq<<1)|(gen_high_frq<<2)|(high_temp<<3)|(gen_low_vol<<4)|(gen_high_vol<<5)|
            (low_oil_press<<6)|(high_bat_vol<<7)|(low_batt_vol<<8)|(high_curr<<27);        
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_LR2057_INFO_1;
        }
        break;
        
      default:
        break;
      };
    }
  case 5:// HIMOINSA
    {
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_HIMOINSA_INFO_1:
        {
          uint8_t high_coolant_temp, low_oil_press, emergency_stop, gen_high_vol, gen_high_frq, low_batt_vol, low_fuel, gen_low_vol, gen_low_frq,over_load,short_circuit;
          
          modbus_parse_bit(ptr,&high_coolant_temp,0,0);
          modbus_parse_bit(ptr,&low_oil_press,0,1);
          modbus_parse_bit(ptr,&emergency_stop,0,2);
          
          //thanhcm3 fix --------------------------
          modbus_parse_bit(ptr,&over_load,1,1);
          modbus_parse_bit(ptr,&short_circuit,2,3);
          //thanhcm3 fix -------------------------
          modbus_parse_bit(ptr,&gen_high_vol,1,3);
          modbus_parse_bit(ptr,&gen_high_frq,1,4);
          modbus_parse_bit(ptr,&low_batt_vol,1,7);
          
          modbus_parse_bit(ptr,&low_fuel,2,2);
          modbus_parse_bit(ptr,&gen_low_vol,2,4);
          modbus_parse_bit(ptr,&gen_low_frq,2,5);       
          
          p_modbus_manager->gen_info_struct[i].u32AlarmStatus = low_fuel|(gen_low_frq<<1)|(gen_high_frq<<2)|(high_coolant_temp<<3)|(gen_low_vol<<4)|(gen_high_vol<<5)|
            (low_oil_press<<6)|(low_batt_vol<<8)|(emergency_stop<<16)|(short_circuit<<28)|(over_load<<11);
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_HIMOINSA_INFO_2;
        }
        break;
      case GEN_HIMOINSA_INFO_2:
        {      
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32GenFrequency,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt1,0,4,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt2,0,5,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt3,0,6,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr1,0,7,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr2,0,8,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr3,0,9,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNPower1,0,15,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineSpeed,0,18,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32FuelLevel,0,19,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32BattVolt,0,21,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32CoolantTemp,0,22,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32OilPressure,0,23,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour,0,35,16);
          
          //            p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin / 60;
          //            p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin % 60;
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_HIMOINSA_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
    
  case 6:// QC315
    {
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_QC315_INFO_1:
        { 
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNCurr1,0,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNCurr2,2,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNCurr3,4,32);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32GenFrequency,0,8,16);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower1,10,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower2,20,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNPower3,30,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNSPower1,14,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNSPower2,24,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNSPower3,34,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour,51,32);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32BattVolt,0,53,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32OilPressure,0,56,16);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32CoolantTemp,57,32);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32FuelLevel,0,59,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineSpeed,0,60,16);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_QC315_INFO_2;
        }
        break;
      case GEN_QC315_INFO_2:
        {    
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNVolt1,0,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNVolt2,2,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->gen_info_struct[i].u32LNVolt3,4,32);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_QC315_INFO_3;
        }
        break;
      case GEN_QC315_INFO_3:
        {             
          uint16_t alarmTemp0, alarmTemp1, alarmTemp3, alarmTemp4, alarmTemp5, alarmTemp6;
          uint8_t gen_low_vol, gen_high_vol, gen_high_frq, gen_low_frq, over_curr, low_fuel, high_coolant_temp, low_oil_press, low_batt_vol, high_batt_volt;
          
          modbus_parse_info(ptr,0,(uint16_t*) &alarmTemp0,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t*) &alarmTemp1,0,1,16);
          modbus_parse_info(ptr,0,(uint16_t*) &alarmTemp3,0,3,16);
          modbus_parse_info(ptr,0,(uint16_t*) &alarmTemp4,0,4,16);
          modbus_parse_info(ptr,0,(uint16_t*) &alarmTemp5,0,5,16);
          modbus_parse_info(ptr,0,(uint16_t*) &alarmTemp6,0,6,16);
          
          if((alarmTemp1&0x200) || (alarmTemp6&0x04))
            gen_low_vol = 1;
          else
            gen_low_vol = 0;
          if((alarmTemp1&0x800) || (alarmTemp6&0x08))
            gen_high_vol = 1;
          else
            gen_high_vol = 0;
          if((alarmTemp1&0x1000) || (alarmTemp6&0x02))
            gen_high_frq = 1;
          else
            gen_high_frq = 0;
          if((alarmTemp1&0x400) || (alarmTemp6&0x01))
            gen_low_frq = 1;
          else
            gen_low_frq = 0;
          if((alarmTemp1&0x4000) || (alarmTemp1&0x2000))
            over_curr = 1;
          else
            over_curr = 0;
          if((alarmTemp0&0x40) || (alarmTemp0&0x80) || (alarmTemp0&0x100) || (alarmTemp0&0x200))
            low_fuel = 1;
          else
            low_fuel = 0;
          if((alarmTemp0&0x1000) || (alarmTemp0&0x2000) || (alarmTemp0&0x4000) || (alarmTemp0&0x8000)|| (alarmTemp4&0x1000)|| (alarmTemp4&0x2000))
            high_coolant_temp = 1;
          else
            high_coolant_temp = 0;
          if((alarmTemp1&0x1) || (alarmTemp1&0x2) || (alarmTemp1&0x4) || (alarmTemp1&0x8)|| (alarmTemp4&0x100)|| (alarmTemp4&0x200))
            low_oil_press = 1;
          else
            low_oil_press = 0;
          if((alarmTemp3&0x20) || (alarmTemp5&0x2))
            low_batt_vol = 1;
          else
            low_batt_vol = 0;
          if(alarmTemp3&0x40)
            high_batt_volt = 1;
          else
            high_batt_volt = 0;
          
          p_modbus_manager->gen_info_struct[i].u32AlarmStatus = low_fuel|(gen_low_frq<<1)|(gen_high_frq<<2)|(high_coolant_temp<<3)|(gen_low_vol<<4)|(gen_high_vol<<5)|
            (low_oil_press<<6)|(high_batt_volt<<7)|(low_batt_vol<<8)|(over_curr<<27);
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_QC315_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 7:// CUMMIN
    {
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_CUMMIN_INFO_1:
        { 
          //            uint16_t GenErr = 0;
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32AlarmStatus ,0,0,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt1,0,6,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt2,0,7,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNVolt3,0,8,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr1,0,14,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr2,0,15,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNCurr3,0,16,16);
          //            modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNPower1,0,19,16);
          //            modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNPower2,0,20,16);
          //            modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNPower3,0,21,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNSPower1,0,28,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNSPower2,0,29,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32LNSPower3,0,30,16);
          
          //thanhcm3 fix -----------------------------------------------------------------------------------
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32GenFrequency,0,32,16);
          //thanhcm3 fix -----------------------------------------------------------------------------------
          
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32BattVolt,0,49,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32OilPressure,0,50,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32CoolantTemp,0,52,16);
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->gen_info_struct[i].u32EngineSpeed,0,56,16);
          modbus_parse_info(ptr,0,0, &p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour,58,32);
          
          
          p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_CUMMIN_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  default:
    break;
  };
  
}

//extract holding reg data pm --------------------------------------------------
void extract_holding_regs_data_pm(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint8_t i,j;
  uint8_t *ptr = &p_modbus->buff_read[0];
  switch(p_modbus->slave_id - p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_slave_offset)
  {
  case 1:
    i = 0;
    break;
  case 2:
    i = 1;
    break;
  };
  switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_type)
  {
  case 1:// FINECO
    {
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      {
      case PM_FINECO_INFO_1:
        {
          for(j = 0; j < 3; j++)
          {
            modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[j].fVoltage, 0 + j*2);
          }
          for(j = 0; j < 3; j++)
          {
            modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[j].fCurrent, 6 + j*2);
          }
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fNeutralCurrent, 12);
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fTotalCurrent, 14);
          for(j = 0; j < 3; j++)
          {
            modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[j].fActivePower, 16 + j*2);
          }
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fRealPower, 22);
          for(j = 0; j < 3; j++)
          {
            modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[j].fReactivePower, 24 + j*2);
          }
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fReactivePower, 30);          
          for(j = 0; j < 3; j++)
          {
            modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[j].fApparentPower, 32 + j*2);
          }
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fApparentPower, 38); 
          for(j = 0; j < 3; j++)
          {
            modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[j].fPowerFactor, 40 + j*2);
          }
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fPowerFactor, 46);
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fFrequency, 48);
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_FINECO_INFO_2;
        }
        break;
      case PM_FINECO_INFO_2:
        {
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fRealEnergy, 0);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_FINECO_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 2:// ASCENT
    {
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      {
      case PM_ASCENT_INFO_1:
        {   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fRealEnergy, 0);     
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_ASCENT_INFO_2;
        }
        break;
      case PM_ASCENT_INFO_2:
        {
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage, 0);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_ASCENT_INFO_3;
        }
        break;
      case PM_ASCENT_INFO_3:
        {
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent, 0);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_ASCENT_INFO_4;
        }
        break;
      case PM_ASCENT_INFO_4:
        {
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower, 0);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_ASCENT_INFO_5;
        }
        break;
      case PM_ASCENT_INFO_5:
        {
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor, 0);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_ASCENT_INFO_6;
        }
        break;
      case PM_ASCENT_INFO_6:
        {
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fFrequency, 0);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_ASCENT_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 3:// EASTRON
    {
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      {
      case PM_EASTRON_INFO_1:
        {   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage,0);   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent,6);   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower,12);   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower,18);   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower,24);   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor,30);   
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_EASTRON_INFO_2;
        }
        break;
      case PM_EASTRON_INFO_2:
        {
          float u32ImportActiveE_PM, u32ExportActiveE_PM, u32ImportReActiveE_PM, u32ExportReActiveE_PM;
          
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].fFrequency, 0);
          modbus_parse_info_f(ptr, &u32ImportActiveE_PM, 2);
          modbus_parse_info_f(ptr, &u32ExportActiveE_PM, 4);
          modbus_parse_info_f(ptr, &u32ImportReActiveE_PM, 6);
          modbus_parse_info_f(ptr, &u32ExportReActiveE_PM, 8);  
          
          p_modbus_manager->pm_info_struct[i].u32ImportActiveE = (int32_t)(u32ImportActiveE_PM*100);
          p_modbus_manager->pm_info_struct[i].u32ExportActiveE = (int32_t)(u32ExportActiveE_PM*100);
          p_modbus_manager->pm_info_struct[i].u32ImportReActiveE = (int32_t)(u32ImportReActiveE_PM*100);
          p_modbus_manager->pm_info_struct[i].u32ExportReActiveE = (int32_t)(u32ExportReActiveE_PM*100);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_EASTRON_INFO_3;
        }
        break;
      case PM_EASTRON_INFO_3:
        {       
          float u32TotalActiveE_PM, u32TotalReActiveE_PM;
          
          modbus_parse_info_f(ptr, &u32TotalActiveE_PM, 0);
          modbus_parse_info_f(ptr, &u32TotalReActiveE_PM, 2);           
          
          p_modbus_manager->pm_info_struct[i].u32TotalActiveE = (int32_t)(u32TotalActiveE_PM*100);
          p_modbus_manager->pm_info_struct[i].u32TotalReActiveE = (int32_t)(u32TotalReActiveE_PM*100);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_EASTRON_INFO_4;
        }
        break;
      case PM_EASTRON_INFO_4:
        {       
          modbus_parse_info(ptr,0,0,&p_modbus_manager->pm_info_struct[i].u32SerialNumber,0,32);
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_EASTRON_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 4:// CET1
    {
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      {
      case PM_CET1_INFO_1:
        {   
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage, 0);    
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent, 2);    
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower, 4);    
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower, 6);    
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower, 8);    
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor, 10);    
          modbus_parse_info_f(ptr, &p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fFrequency, 12); 
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_CET1_INFO_2;
        }
        break;
      case PM_CET1_INFO_2:
        {
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ImportActiveE,0, 32);
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ExportActiveE,2, 32);
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32NetActiveE,4, 32);
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32TotalActiveE,6, 32);
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ImportReActiveE,8, 32);
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ExportReActiveE,10, 32);
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32NetReActiveE,12, 32);
          modbus_parse_info_sign(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32TotalReActiveE,14, 32);
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_CET1_INFO_3;
        }
        break;
      case PM_CET1_INFO_3:
        {
          modbus_parse_info_string(ptr,&p_modbus_manager->pm_info_struct[i].u8Model[0], 20, 0,1);
          modbus_parse_info(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32SerialNumber,25, 32);
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_CET1_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 5:// PILOT
    {
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      {
      case PM_PILOT_INFO_1:
        {
          uint16_t u16Voltage_PM, u16Frequency_PM, u16PowerFactor_PM;
          uint32_t u32Current_PM;
          int32_t u32ActivePower, u32ApparentPower, u32ReactivePower;
          modbus_parse_info_inverse(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32TotalActiveE,0, 32);
          modbus_parse_info(ptr,0, &u16Voltage_PM,0,2,16);
          modbus_parse_info_inverse(ptr,0,0,&u32Current_PM,3,32);
          modbus_parse_info_sign_inverse(ptr,0,0,&u32ActivePower,5,32);
          modbus_parse_info_sign_inverse(ptr,0,0,&u32ApparentPower,7,32);
          modbus_parse_info_sign_inverse(ptr,0,0,&u32ReactivePower,9,32);          
          modbus_parse_info(ptr,0, &u16Frequency_PM,0,11,16);
          modbus_parse_info(ptr,0,&u16PowerFactor_PM,0,12,16);
          modbus_parse_info_inverse(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ImportActiveE,13, 32);
          modbus_parse_info_inverse(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ExportActiveE,15, 32);
          
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage = (float)u16Voltage_PM;
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent = (float)u32Current_PM;
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower = (float)u32ActivePower;
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower = (float)u32ApparentPower;
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower = (float)u32ReactivePower;
          p_modbus_manager->pm_info_struct[i].fFrequency = (float)u16Frequency_PM;
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor = (float)u16PowerFactor_PM;                                              
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_PILOT_INFO_2;
        }
        break;
      case PM_PILOT_INFO_2:
        {
          
          modbus_parse_info_sign_inverse(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ImportReActiveE,0, 32);
          modbus_parse_info_sign_inverse(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32ExportReActiveE,2, 32);
          modbus_parse_info_sign_inverse(ptr, 0,0,&p_modbus_manager->pm_info_struct[i].u32TotalReActiveE,4, 32);
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_PILOT_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 15:// PILOT_3PHASE
    {
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      {
      case PM_PILOT_3PHASE_INFO_1:
        {          
          uint16_t u16tempPM = 0;
          uint32_t u32tempPM = 0;
          int32_t i32tempPM = 0;
          int16_t i16tempPM = 0;
          modbus_parse_info(ptr,0, &u16tempPM,0,0,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage = (float)u16tempPM;
          modbus_parse_info(ptr,0, &u16tempPM,0,1,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fVoltage = (float)u16tempPM;
          modbus_parse_info(ptr,0, &u16tempPM,0,2,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fVoltage = (float)u16tempPM;
          
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,6,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent = (float)(u32tempPM/10);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,8,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent = (float)(u32tempPM/10);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,10,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent = (float)(u32tempPM/10);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,12,32);
          p_modbus_manager->pm_info_struct[i].fTotalCurrent = (float)(u32tempPM/10);
          
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,14,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower = (float)(i32tempPM/1000);
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,16,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fActivePower = (float)(i32tempPM/1000);
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,18,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fActivePower = (float)(i32tempPM/1000);
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,20,32);
          p_modbus_manager->pm_info_struct[i].fRealPower = (float)(i32tempPM/1000);
          
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,22,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower = (float)(i32tempPM/1000);
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,24,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower = (float)(i32tempPM/1000);
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,26,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower = (float)(i32tempPM/1000);
          modbus_parse_info_sign_inverse(ptr,0, 0,&i32tempPM,28,32);
          p_modbus_manager->pm_info_struct[i].fReactivePower = (float)(i32tempPM/1000);
          
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,30,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower = (float)(u32tempPM/1000);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,32,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fApparentPower = (float)(u32tempPM/1000);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,34,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fApparentPower = (float)(u32tempPM/1000);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM,36,32);
          p_modbus_manager->pm_info_struct[i].fApparentPower = (float)(u32tempPM/1000);
          
          modbus_parse_info_sign(ptr,0,&i16tempPM, 0,38,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor = (float)(i16tempPM/10);
          modbus_parse_info_sign(ptr,0,&i16tempPM, 0,39,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fPowerFactor = (float)(i16tempPM/10);
          modbus_parse_info_sign(ptr,0,&i16tempPM, 0,40,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fPowerFactor = (float)(i16tempPM/10);
          modbus_parse_info_sign(ptr,0,&i16tempPM, 0,41,16);
          p_modbus_manager->pm_info_struct[i].fPowerFactor = (float)(i16tempPM/10);
          
          modbus_parse_info(ptr,0,&u16tempPM, 0,42,16);
          p_modbus_manager->pm_info_struct[i].fFrequency = (float)(u16tempPM/10);
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_PILOT_3PHASE_INFO_2;
        }
        break;
      case PM_PILOT_3PHASE_INFO_2:
        {
          uint32_t u32tempPM2 = 0;
          
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM2,0,32);
          p_modbus_manager->pm_info_struct[i].u32ImportActiveE = (int32_t)(u32tempPM2*10);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM2,2,32);
          p_modbus_manager->pm_info_struct[i].u32ExportActiveE = (int32_t)(u32tempPM2*10);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM2,4,32);
          p_modbus_manager->pm_info_struct[i].u32TotalActiveE = (int32_t)(u32tempPM2*10);
          
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM2,6,32);
          p_modbus_manager->pm_info_struct[i].u32ImportReActiveE = (int32_t)(u32tempPM2*10);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM2,8,32);
          p_modbus_manager->pm_info_struct[i].u32ExportReActiveE = (int32_t)(u32tempPM2*10);
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM2,10,32);
          p_modbus_manager->pm_info_struct[i].u32TotalReActiveE = (int32_t)(u32tempPM2*10);
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_PILOT_3PHASE_INFO_3;
        }
        break;
      case PM_PILOT_3PHASE_INFO_3:
        {
          uint32_t u32tempPM2 = 0;
          
          modbus_parse_info_inverse(ptr,0, 0,&u32tempPM2,0,32);
          p_modbus_manager->pm_info_struct[i].u32SerialNumber = u32tempPM2;
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_PILOT_3PHASE_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 16://YADA_3PHASE_DPC
    {
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step){
      case PM_YADA_3PHASE_DPC_INFO_1:
        {
          uint32_t u32yadatempPM = 0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM,0,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage = ((float)u32yadatempPM)/100.0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM,2,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fVoltage = ((float)u32yadatempPM)/100.0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM,4,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fVoltage = ((float)u32yadatempPM)/100.0;
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_2;
        }
        break;
      case PM_YADA_3PHASE_DPC_INFO_2:
        {
          uint32_t u32yadatempPM2 = 0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM2,0,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent = ((float)u32yadatempPM2)/100.0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM2,2,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent = ((float)u32yadatempPM2)/100.0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM2,4,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent = ((float)u32yadatempPM2)/100.0;
          p_modbus_manager->pm_info_struct[i].fTotalCurrent = p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent 
            +p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent
              +p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent;
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_3;
        }
        break;
      case PM_YADA_3PHASE_DPC_INFO_3:
        {
          int16_t i16yadatempPM3 = 0;
          modbus_parse_info_sign(ptr,0, &i16yadatempPM3,0,0,16);
          p_modbus_manager->pm_info_struct[i].fPowerFactor = ((float)i16yadatempPM3)/10.0;
          modbus_parse_info_sign(ptr,0, &i16yadatempPM3,0,1,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor = ((float)i16yadatempPM3)/10.0;
          modbus_parse_info_sign(ptr,0, &i16yadatempPM3,0,2,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fPowerFactor = ((float)i16yadatempPM3)/10.0;
          modbus_parse_info_sign(ptr,0, &i16yadatempPM3,0,3,16);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fPowerFactor = ((float)i16yadatempPM3)/10.0;
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_4;
        }
        break;
      case PM_YADA_3PHASE_DPC_INFO_4:
        {
          uint16_t u16yadatempPM4 = 0;
          modbus_parse_info(ptr,0, &u16yadatempPM4,0,0,16);
          p_modbus_manager->pm_info_struct[i].fFrequency = ((float)u16yadatempPM4)/10.0;
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_5;
        }
        break;
      case PM_YADA_3PHASE_DPC_INFO_5:
        {
          uint32_t u32yadatempPM5 = 0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM5,0,32);
          p_modbus_manager->pm_info_struct[i].u32TotalReActiveE = u32yadatempPM5;
          
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_6;
        }
        break;
      case PM_YADA_3PHASE_DPC_INFO_6:
        {
          uint32_t u32yadatempPM6 = 0;
          modbus_parse_info(ptr,0, 0,&u32yadatempPM6,0,32);
          p_modbus_manager->pm_info_struct[i].u32TotalActiveE = u32yadatempPM6;
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_7;
        }
        break;
      case PM_YADA_3PHASE_DPC_INFO_7:
        {
          int32_t i32yadatempPM7 = 0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM7,0,32);
          p_modbus_manager->pm_info_struct[i].fRealPower = ((float)i32yadatempPM7)/100.0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM7,2,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower = ((float)i32yadatempPM7)/100.0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM7,4,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fActivePower = ((float)i32yadatempPM7)/100.0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM7,6,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fActivePower = ((float)i32yadatempPM7)/100.0;
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_8;
        }
        break;
      case PM_YADA_3PHASE_DPC_INFO_8:
        {
          int32_t i32yadatempPM8 = 0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM8,0,32);
          p_modbus_manager->pm_info_struct[i].fReactivePower = ((float)i32yadatempPM8)/100.0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM8,2,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower = ((float)i32yadatempPM8)/100.0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM8,4,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower = ((float)i32yadatempPM8)/100.0;
          modbus_parse_info_sign(ptr,0, 0,&i32yadatempPM8,6,32);
          p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower = ((float)i32yadatempPM8)/100.0;
          p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_1;
        }
        break;
      default:
        break;
      }
    }
    break;
  default:
    break;
  };
}

//extract holding regs data smcb---------------------------------
void extract_holding_regs_data_smcb(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint8_t i;
  uint8_t *ptr = &p_modbus->buff_read[0];
  switch(p_modbus->slave_id - p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_slave_offset)
  {
  case 1:
    i = 0;
    break;
  case 2:
    i = 1;
    break;
  case 3:
    i = 2;
    break;
  case 4:
    i = 3;
    break;
  case 5:
    i = 4;
    break;
  };
  switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_type)
  {
  case 1:// OPEN
    {
      switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step)
      {
      case SMCB_OPEN_INFO_1:
        {
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->smcb_info_struct[i].u32State,0,0,16);
          
          p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step = SMCB_OPEN_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  case 2:// MATIS
    {
      switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step)
      {
      case SMCB_MATIS_INFO_1:
        {
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->smcb_info_struct[i].u32State,0,0,16);
          
          p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step = SMCB_MATIS_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  default:
    break;
  };
}

//extract holding regs data fuel--------------------------------
void extract_holding_regs_data_fuel(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint8_t i;
    uint8_t *ptr = &p_modbus->buff_read[0];
    switch(p_modbus->slave_id - p_modbus_manager->fuel_manager_struct[p_modbus_manager->bm_current_index].fuel_slave_offset)
    {
    case 1:
        i = 0;
        break;
    case 2:
        i = 1;
        break;
    };
    switch(p_modbus_manager->fuel_manager_struct[p_modbus_manager->bm_current_index].fuel_type)
    {
    case 1:// HPT621
    {
        switch(p_modbus_manager->fuel_manager_struct[p_modbus_manager->bm_current_index].fuel_running_step)
        {
        case FUEL_HPT621_INFO_1:
        {
          modbus_parse_info(ptr,0,(uint16_t*) &p_modbus_manager->fuel_info_struct[i].u32FuelLevel,0,0,16);
           
          p_modbus_manager->fuel_manager_struct[p_modbus_manager->bm_current_index].fuel_running_step = FUEL_HPT621_INFO_1;
        }
        break;
        default:
            break;
        };
    }
    break;
    default:
        break;
    };
}

//extract holding regs data isense--------------------------------
void extract_holding_regs_data_isense(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint8_t i;
  uint8_t *ptr = &p_modbus->buff_read[0];
  switch(p_modbus->slave_id - p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_slave_offset)
  {
  case 1:
    i = 0;
    break;
  };
  switch(p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_type)
  {
  case 1:// FORLONG
    {
      switch(p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_running_step)
      {
      case ISENSE_FORLONG_INFO_1:
        {
          uint32_t u32Forlong;
          modbus_parse_info_bcd(ptr,0,0,&p_modbus_manager->isense_info_struct[i].u32TotalActiveE,0,32);
          modbus_parse_info_bcd(ptr,0,0,&p_modbus_manager->isense_info_struct[i].u32TotalReActiveE,4,32);
          modbus_parse_info_bcd(ptr,0,0,&u32Forlong,6,32);
          p_modbus_manager->isense_info_struct[i].fVoltage = (float)u32Forlong; 
          modbus_parse_info_bcd(ptr,0,0,&u32Forlong,8,32);
          p_modbus_manager->isense_info_struct[i].fCurrent = (float)u32Forlong;
          modbus_parse_info_bcd(ptr,0,0,&u32Forlong,10,32);
          p_modbus_manager->isense_info_struct[i].fActivePower = (float)u32Forlong;
          modbus_parse_info_bcd(ptr,0,0,&u32Forlong,16,32);
          p_modbus_manager->isense_info_struct[i].fPowerFactor = (float)u32Forlong;
          modbus_parse_info_bcd(ptr,0,0,&u32Forlong,18,32);
          p_modbus_manager->isense_info_struct[i].fFrequency = (float)u32Forlong;
          modbus_parse_info_bcd(ptr,0,0,&u32Forlong,20,32);
          p_modbus_manager->isense_info_struct[i].u32SerialNumber =u32Forlong;
          
          p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_running_step = ISENSE_FORLONG_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  default:
    break;
  };
  
}

//extract holding regs data pmdc--------------------------------
void extract_holding_reg_data_pmdc(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint8_t i;
  uint8_t *ptr = &p_modbus->buff_read[0];
  switch(p_modbus->slave_id - p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_slave_offset)
  {
  case 1:
      i = 0;
      break;
  };
  switch(p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_type){
  case 1:// YADA_DC
  {
    switch(p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step){
    case PM_DC_YADA_INFO_1:
      {
      uint32_t temp_yada_pm_dc_1;
      modbus_parse_info(ptr,0, 0,&temp_yada_pm_dc_1,0,32);
      p_modbus_manager->pmdc_info_struct[i].f_voltage = ((float)temp_yada_pm_dc_1)/10.0;
      p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step = PM_DC_YADA_INFO_2;
      }
      break;
    case PM_DC_YADA_INFO_2:
      {
      uint32_t temp_yada_pm_dc_2;
      modbus_parse_info(ptr,0, 0,&temp_yada_pm_dc_2,0,32);
      p_modbus_manager->pmdc_info_struct[i].f_current = ((float)temp_yada_pm_dc_2)/100.0;
      p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step = PM_DC_YADA_INFO_3;
      }
      break;
    case PM_DC_YADA_INFO_3:
      {
      uint32_t temp_yada_pm_dc_3;
      modbus_parse_info(ptr,0, 0,&temp_yada_pm_dc_3,0,32);
      p_modbus_manager->pmdc_info_struct[i].f_active_power = ((float)temp_yada_pm_dc_3)/100.0;
      p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step = PM_DC_YADA_INFO_4;
      }
      break;
    case PM_DC_YADA_INFO_4:
      {
      uint32_t temp_yada_pm_dc_4;
      modbus_parse_info(ptr,0, 0,&temp_yada_pm_dc_4,0,32);
      p_modbus_manager->pmdc_info_struct[i].f_active_energy = ((float)temp_yada_pm_dc_4)*10;
      p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step = PM_DC_YADA_INFO_1;
      }
      break;
    default:
      break;
    }
  }
  break;
  default:
  break; 
  }
}

//extract holding regs data bm--------------------------------
void extract_holding_reg_data_bm(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  uint8_t i,j;
  uint8_t *ptr = &p_modbus->buff_read[0];
  switch(p_modbus->slave_id - p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_slave_offset)
  {
  case 1:
    i = 0;
    break;
  case 2:
    i = 1;
    break;
  case 3:
    i = 2;
    break;
  case 4:
    i = 3;
    break;
  case 5:
    i = 4;
    break;
  case 6:
    i = 5;
    break;
  case 7:
    i = 6;
    break;
  case 8:
    i = 7;
    break;
  case 9:
    i = 8;
    break;
  case 10:
    i = 9;
    break;
  case 11:
    i = 10;
    break;
  case 12:
    i = 11;
    break;
  case 13:
    i = 12;
    break;
  case 14:
    i = 13;
    break;
  case 15:
    i = 14;
    break;
  case 16:
    i = 15;
    break;
  };
  switch(p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_type)
  {
  case 1:// VIETTEL
    {
      switch(p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_running_step)
      {
      case BM_VIETTEL_INFO_1:
        {          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32BattVolt,0,32);       
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32PackVolt,2,32);
          
          for (j = 0; j < 4; j++)
          {
            modbus_parse_info(ptr,0,&p_modbus_manager->bm_info_struct[i].u16CellVolt[j],0,4+j,16);
          }
          modbus_parse_info_sign(ptr,0,0,&p_modbus_manager->bm_info_struct[i].i32PackCurr,8,32);
          for (j = 0; j < 6; j++)
          {
            modbus_parse_info(ptr,0,&p_modbus_manager->bm_info_struct[i].u16CellTemp[j],0,10+j,16);
          }
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32AlarmStatus,16,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32BalanceStatus,18,32);
          modbus_parse_info(ptr,0,&p_modbus_manager->bm_info_struct[i].u16BattStatus,0,20,16);
          
          modbus_parse_info(ptr,0,(uint16_t *)&p_modbus_manager->bm_info_struct[i].u16SOC,0,21,16);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32SOH,22,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32DischargeTime,24,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32TotalRunTime,26,32);
          
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32VoltDiff,32,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32MaxTemp,34,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32VoltThres,36,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32CurrThres,38,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32TimeThres,40,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32SOCThres,42,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32MinTemp,44,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32LowCapTime,46,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32TotalDisAH,48,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32HalfVoltAlarm,50,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32RechargeVolt,52,32);
          modbus_parse_info(ptr,0,0,&p_modbus_manager->bm_info_struct[i].u32CurrBalanceThres,54,32);
          
          p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_running_step = BM_VIETTEL_INFO_1;
        }
        break;
      case BM_VIETTEL_INFO_2:
        {
          p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_running_step = BM_VIETTEL_INFO_1;
        }
        break;
      default:
        break;
      };
    }
    break;
  default:
    break;
  };
}



//check sum message for fan-----------------------------------------------------
uint8_t DKD51_check_sum_fan(FAN_DPC_SWITCH_UART_T* p){
  for(uint16_t i=0;i<p->uart_rx.cnt;i++){
    p->rx.buff_485[i]=p->uart_rx.buff_485[i];
    p->uart_rx.buff_485[i]=0;
  }
  p->rx.cnt =p->uart_rx.cnt;
  if(p->rx.cnt>6){
    p->check_sum_read = hex2byte2(&p->rx.buff_485[0],p->rx.cnt-5);
    p->check_sum_calculator = Checksum16(&p->rx.buff_485[0],p->rx.cnt);
  }else{
    p->check_sum_calculator = 0;
    p->check_sum_read       = 0xFFFF;
  }
  if(p->check_sum_read==p->check_sum_calculator) return NO_ERR;
  else return ERR;
}
//check return------------------------------------------------------------------
uint8_t DKD51_check_RTN_fan(FAN_DPC_SWITCH_UART_T* p){
  if(hex2byte(&p->rx.buff_485[0],7)== 0) return NO_ERR;
  else return ERR;
}
//clean fan---------------------------------------------------------------------
void DKD51_clean_buff_fan(FAN_DPC_SWITCH_UART_T* p){
  for(uint16_t i=0; i<500;i++){
    p->uart_rx.buff_485[i] =0;
    p->rx.buff_485[i]      =0;        
  }
  p->uart_rx.cnt = 0;
  p->rx.cnt      = 0;
}


























