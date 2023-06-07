//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_RS485_H
#define _DAQ_V3_RS485_H
//include-----------------------------------------------------------------------

//define------------------------------------------------------------------------
//baudrate-------------------------------------
#define BAUD_RATE_2400                  2400
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_115200                115200
//parity---------------------------------------
#define NONE_PARITY                     0
#define ODD_PARITY                      1
#define EVEN_PARITY                     2 
//---------------------------------------------
#define MAX_BUFFER_RTU                  256
//modbus_state---------------------------------
#define START                           0
#define READING                         1
#define READING_END                     2 
//---------------------------------------------
enum
{
    _READ_COIL_STATUS 			= 1,
    _READ_INPUT_STATUS			= 2,
    _READ_HOLDING_REGS 			= 3,    // 4x
    _READ_INPUT_REG 			= 4,    // 3x
    _FORCE_SINGLE_COIL 			= 5,
    _PRESET_SINGLE_REG 			= 6,
    _READ_EXCEPTION_STATUS 		= 7,
    _FETCH_COMM_EVENT_COUNTER 	        = 11,
    _FET_COMM_EVENT_LOG 		= 12,
    _FORCE_MULTIPLE_COILS  		= 15,
    _PRESET_MULTIPLE_REGS 		= 16,
    _REPORT_SLAVE_ID 			= 17,
    _READ_GENERAL_REFERENCE 	        = 20,
    _WRITE_GENERAL_REFERENCE 	        = 21,
    _MASK_WRITE_4X_REG 			= 22,
    _READ_WRITE_4X_REGS 		= 23,
    _READ_FIFO_QUEUE 			= 24,
    _SETUP_REGISTER		        = 49,
    _USER_REGISTER 		        = 50,    
    _EXCEPTION_READ_HOLDING             = 0x83,
    _EXCEPTION_READ_INPUT               = 0x84,
    _EXCEPTION_WRITE_MULTI_REG          = 0x90,
};
//typedef enum------------------------------------------------------------------
typedef enum
{
  _CRC_W = 0,
  _CRC_R,
  
}CRC_MODBUS_TYPE;
//------------------------------------------------------------------------------
typedef enum{
  _OTHER_SLAVE,
  _VAC_SLAVE,
  _LIB_SLAVE,
  _GEN_SLAVE,
  _PM_SLAVE,
  _SMBC_SLAVE,
  _FUEL_SLAVE,
  _ISENSE_SLAVE,
  _PMDC_SLAVE,
  _BM_SLAVE,
  
}DEVICE_SLAVE;

typedef enum{
  _WAIT_MODE,
  _READ_BATT_STATUS,    
  _WAIT_BATT_RESPOND,
  _READ_LIB_STATUS,    
  _WAIT_LIB_RESPOND,
  _READ_GEN_STATUS,    
  _WAIT_GEN_RESPOND,
  _READ_PM_STATUS,    
  _WAIT_PM_RESPOND,
  _READ_VAC_STATUS,    
  _WAIT_VAC_RESPOND,
  _READ_SMCB_STATUS,    
  _WAIT_SMCB_RESPOND,
  _READ_FUEL_STATUS,    
  _WAIT_FUEL_RESPOND,
  _READ_ISENSE_STATUS,    
  _WAIT_ISENSE_RESPOND,
  _READ_PM_DC_STATUS,
  _WAIT_PM_DC_RESPOND,
  _READ_BM_STATUS,    
  _WAIT_BM_RESPOND,
  _READ_FAN_DPC_STATUS,
  _WAIT_FAN_DPC_RESPOND,
  _READ_TAZ_STATUS,    
  _WAIT_TAZ_RESPOND,
  _READ_ACM_STATUS,    
  _WAIT_ACM_RESPOND,
  _READ_DCU_STATUS,    
  _WAIT_DCU_RESPOND,
  _WRITE_SINGLE_REG,    
  _WAIT_WRITE_MULTI_REG,
  _TEST_MODE,
}RTU_STATE;
//typedef struct----------------------------------------------------------------
typedef struct{
  RTU_STATE                     running_step;
  uint8_t                       buff_write[MAX_BUFFER_RTU];                 
  uint8_t                       buff_read[MAX_BUFFER_RTU];
  uint8_t                       crc_high;
  uint8_t                       crc_low;
  uint8_t                       crc_high_read;
  uint8_t                       crc_low_read;      
  uint8_t                       slave_id;
  uint8_t                       function_code;
  uint8_t                       start_high;
  uint8_t                       start_low;
  uint8_t                       id_high;
  uint8_t                       id_low;
  uint8_t                       number_reg_high;
  uint8_t                       number_reg_low;
  uint8_t                       read_write_setup;
  uint8_t                       setup_type;
  uint8_t                       byte_count;
  uint8_t                       modbus_state;
  uint8_t                       data_pointer;
}MODBUS_RTU_STRUCT;
extern MODBUS_RTU_STRUCT modbus_rtu_struct;
extern uint16_t LoadCurr, ACFaultM1,PMUConnectM1;
//function----------------------------------------------------------------------
void init_rs232_dc_cabinet(uint32_t baudrate, uint8_t parity);
void init_rs485_dc_cabinet(uint32_t baudrate, uint8_t parity);
void init_rs485_dc_cabinet_9_bit(uint32_t baudrate, uint8_t parity);
void lpuart_send_9_bit_mode(LPUART_Type *base,uint8_t data,uint8_t addr_value);

void init_rs485_modbus_rtu(uint32_t baudrate,uint8_t parity);
void master_read_modbus(MODBUS_RTU_STRUCT* p,uint8_t function_code, uint8_t slave_addr,uint16_t starting_addr,uint16_t no_point,uint32_t delay);
void modbus_crc (MODBUS_RTU_STRUCT* p, unsigned char len,CRC_MODBUS_TYPE crc_type);
void delay_modbus_rtu(uint32_t delay);
void modbus_parse_bit(uint8_t *buf, uint8_t *var_type_u8, uint32_t pos, uint8_t coil );
void modbus_parse_info(uint8_t *buf, uint8_t *var_type_u8, uint16_t *var_type_u16, uint32_t *var_type_u32, uint32_t pos, uint8_t type);
void modbus_parse_info_inverse(uint8_t *buf, uint8_t *var_type_u8, uint16_t *var_type_u16, uint32_t *var_type_u32, uint32_t pos, uint8_t type);
void modbus_parse_info_sign(uint8_t *buf, int8_t *var_type_i8, int16_t *var_type_i16, int32_t *var_type_i32, uint32_t pos, uint8_t type);
void modbus_parse_info_sign_inverse(uint8_t *buf, int8_t *var_type_i8, int16_t *var_type_i16, int32_t *var_type_i32, uint32_t pos, uint8_t type);
void modbus_parse_info_f(uint8_t *buf, float *var_type_f, uint32_t pos);
void modbus_parse_info_f_inverse(uint8_t *buf, float *var_type_f, uint32_t pos, uint8_t type_1_2);
void modbus_parse_info_string (uint8_t *buf, uint8_t *dst_buf, uint8_t num_of_byte, uint32_t pos, uint8_t type_1_2);
void modbus_parse_info_bcd (uint8_t *buf, uint8_t *var_type_u8, uint16_t *var_type_u16, uint32_t *var_type_u32, uint32_t pos, uint8_t type);
void modbus_parse_time(uint8_t *buf, TIME_FORMAT_STRUCT  *set_time, uint32_t pos);
void mater_write_single_reg(MODBUS_RTU_STRUCT* p, uint8_t slave_addr, uint16_t reg_addr,uint16_t write_val,uint32_t delay);
void mater_write_multi_reg (MODBUS_RTU_STRUCT* p, uint8_t slave_addr, uint16_t reg_addr, uint8_t *write_buff, uint8_t size,uint32_t delay);
void mater_write_i16(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_add, uint16_t reg_addr, int16_t val, DEVICE_SLAVE slave_type, uint32_t delay);
void set_var_time(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_addr, uint16_t reg_addr,TIME_FORMAT_STRUCT set_time,DEVICE_SLAVE slave_type, uint32_t delay);
void mater_write_i32(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_add, uint16_t reg_addr, int32_t val, DEVICE_SLAVE slave_type, uint32_t delay);
void mater_write_u16(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager, uint8_t slave_add, uint16_t reg_addr, uint16_t val, DEVICE_SLAVE slave_type, uint32_t delay);
void mater_write_06_u16(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager,uint8_t slave_add, uint16_t reg_addr, uint16_t val, DEVICE_SLAVE slave_type, uint32_t delay);
int8_t mater_check_respond_data(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager,DEVICE_SLAVE slave_type);

void extract_holding_regs_data_vac(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_regs_data_lib(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_regs_data_gen(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_regs_data_pm(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_regs_data_smcb(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_regs_data_fuel(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_regs_data_isense(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_reg_data_pmdc(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void extract_holding_reg_data_bm(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
uint8_t DKD51_check_sum_fan(FAN_DPC_SWITCH_UART_T* p);
uint8_t DKD51_check_RTN_fan(FAN_DPC_SWITCH_UART_T* p);
void DKD51_clean_buff_fan(FAN_DPC_SWITCH_UART_T* p);



void check_crc (uint8_t *crc_high, uint8_t *crc_low,uint8_t *buff, uint32_t len);
#endif /* _DAQ_V3_RS485_H */