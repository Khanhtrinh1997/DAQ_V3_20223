//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_MODBUS_RTU_H
#define _DAQ_V3_MODBUS_RTU_H
//include-----------------------------------------------------------------------
#include "header_files.h"
//define------------------------------------------------------------------------
#define DISCONNECTED                               170
#define CONNECTED                                  255
#define WARNING                                    165
//define fan dpc----------------------------------------
#define STATE_ON                                   1
#define STATE_OFF                                  0
//function----------------------------------------------------------------------
void modbus_rtu_task(void* pv_parameters);
void update_device();
void master_modbus(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

//code here
RTU_STATE read_lib_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_lib_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_gen_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_gen_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_pm_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_pm_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_vac_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_vac_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_smbc_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_smcb_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_fuel_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_fuel_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_isense_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_isense_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_pmdc_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_pmdc_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_bm_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_bm_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);

RTU_STATE read_fan_dpc_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);
RTU_STATE wait_fan_dpc_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager);




//code here
void update_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_vac_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_lib_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_gen_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_pm_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_smbc_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_fuel_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_isense_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_pmdc_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_bm_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager);
void update_fan_dpc_info(PRIVATE_MIB_BASE_STRUCT *p_mib);



#endif /* _DAQ_V3_MODBUS_RTU_H */