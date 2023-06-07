//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_CONTROL_TASK_H
#define _DAQ_V3_CONTROL_TASK_H
//include-----------------------------------------------------------------------
//define------------------------------------------------------------------------
#define NUM_BYTE_OF_S19_LINE                             53



#define STATE_BNT_DEFAULT                                GPIO_PinRead(GPIO3, 1U)
//function----------------------------------------------------------------------
void control_task(void* pv_parameters);
void blink_led_user();
void user_config_save();
void user_reboot();
void user_write_crc32();
void user_update();
void bnt_factory_default();
uint16_t s19FileParse(uint32_t startAddr);
//code here


#endif /* _DAQ_V3_CONTROL_TASK_H */