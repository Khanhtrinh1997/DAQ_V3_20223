//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
//modbus rtu task-----------------------------------------------
void modbus_rtu_task(void* pv_parameters){
  private_mib_base_struct.cntGroup.task.modbus_rtu                         = 0;
  config_handler_struct.u8FwUpdateFlag        = 0;
  config_handler_struct.cnt_timeout_update    = 0;
  config_handler_struct.state_update          = _NO_FW_UPDATE;
  modbus_manager_struct.setting_cmd           = SET_DAQ_NOOP;
  modbus_rtu_struct.running_step              = _WAIT_MODE;
  
  fan_dpc_switch_uart_t.rx_enable             = STATE_OFF;
  fan_dpc_switch_uart_t.fan_state_e           = _DKD51_FAN_SYS_REQ;
  fan_dpc_switch_uart_t.check_sum_calculator  = 0;
  fan_dpc_switch_uart_t.check_sum_read        = 0xFFFF;
  //modbus_manager->vac_info_struct[i].vac_reset_ok = 1;
//  //test vac-------------------
//  modbus_manager_struct.number_of_vac                          = 1;
//  modbus_manager_struct.vac_manager_struct[0].vac_slave_offset = 34;
//  modbus_manager_struct.vac_manager_struct[0].vac_abs_slave_id = 35;
//  modbus_manager_struct.vac_manager_struct[0].vac_base_addr[0] = 0;
//  modbus_manager_struct.vac_manager_struct[0].vac_base_addr[1] = 0;
//  modbus_manager_struct.vac_manager_struct[0].vac_base_addr[2] = 80;
//  
//  modbus_manager_struct.vac_manager_struct[0].vac_num_of_reg[0] = 14;
//  modbus_manager_struct.vac_manager_struct[0].vac_num_of_reg[1] = 39;
//  modbus_manager_struct.vac_manager_struct[0].vac_num_of_reg[2] = 3;
//  //modbus_manager_struct.vac_manager_struct[0].vac_running_step  =  VAC_VIETTEL_INFO_1; 
//  modbus_manager_struct.vac_manager_struct[0].vac_type          = 1;
//  
//  //test pin=====================================================================
//  modbus_manager_struct.number_of_lib =1;
//  //uint8_t jj =1;
//  for(uint8_t jj=1;jj<=1;jj++){
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_slave_offset = 0;
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_abs_slave_id = jj;
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_base_addr[0] = 0; // FC:0x04
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_num_of_reg[0] = 100;   
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_base_addr[1] = 130;// FC:0x04
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_num_of_reg[1] = 22;  //20
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_base_addr[2] = 0;// FC:0x03
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_num_of_reg[2] = 80; 
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_base_addr[3] = 162;// FC:0x03
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_num_of_reg[3] = 3;//2 
//    
//    modbus_manager_struct.lib_manager_struct[jj-1].lib_type  = 5;
//  }
  
  //test pin=====================================================================
  //test-------------------
  
  
  while(true){
    private_mib_base_struct.cntGroup.task.modbus_rtu++;
    switch(config_handler_struct.state_update){
    case _DAQ_V3_FW_:
    case _VAC_FW_:
    case _VAC_V2_FW_:
      update_device();
      break;
    case _NO_FW_UPDATE:
      vTaskDelay(500);
      
      master_modbus(&modbus_rtu_struct,&modbus_manager_struct);
      //update info-----------------------------------------------
      update_info(&private_mib_base_struct,&modbus_manager_struct);
      break; 
    }
    
    vTaskDelay(1);
  }
  
}

//function update DAQ_V3 FW-----------------------------------------------------
void update_device(){
  switch(config_handler_struct.state_update){
  case _DAQ_V3_FW_:// update daq v3---------------------------------------------
    {
      if(config_handler_struct.u8FwUpdateFlag == 1){
        
      }else{
        if(config_handler_struct.cnt_timeout_update++>=20){
          
          config_handler_struct.cnt_timeout_update =0;
          config_handler_struct.state_update = _NO_FW_UPDATE;
        }
      }
      vTaskDelay(1000);
    }
    break;
  case _VAC_FW_://update vac ---------------------------------------------------
    {
      if(config_handler_struct.u8FwUpdateFlag == 1){
        
      }else{
        if(config_handler_struct.cnt_timeout_update++>=20){
          
          config_handler_struct.cnt_timeout_update =0;
          config_handler_struct.state_update = _NO_FW_UPDATE;
        }
      }
      vTaskDelay(1000);
    }
    break;
  case _VAC_V2_FW_:// update vac v2---------------------------------------------
    {
      if(config_handler_struct.u8FwUpdateFlag == 1){
        
      }else{
        if(config_handler_struct.cnt_timeout_update++>=20){
          
           config_handler_struct.cnt_timeout_update =0;
           config_handler_struct.state_update = _NO_FW_UPDATE;
        }
      }
      vTaskDelay(1000);
    }
    break;
  default:
    config_handler_struct.state_update = _NO_FW_UPDATE;
    break;
  }
}

//function master modbus--------------------------------------------------------
void master_modbus(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  if(sTestRS485.requesttestRS485 ==1)
  {
    p_modbus->running_step = _TEST_MODE;
  }
  //mater set-------------------------------------------------------------------------
  switch(p_modbus_manager->setting_cmd){
  //set fan dpc------------------------------------------- 
  case SET_FAN_DPC_STARTING_POINT:
    {
      if(TYPE == DKD51_BDP){
        fan_dpc_switch_uart_t.rx_enable   = STATE_ON;
        p_modbus->modbus_state = START;
        p_modbus->data_pointer = 0;
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        DKD51_set_fan_starting_point((uint16_t)private_mib_base_struct.fan_dpc_info.mib.temp_set);
        private_mib_base_struct.fan_dpc_info.mib.temp_set = 0;
        //code here
        p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      }
      else{
        p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
      }
    }
    break;
  case SET_FAN_DPC_SENSITIVE_POINT:
    {
      if(TYPE == DKD51_BDP){
        fan_dpc_switch_uart_t.rx_enable   = STATE_ON;
        p_modbus->modbus_state = START;
        p_modbus->data_pointer = 0;
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        DKD51_set_fan_sensitive_point((uint16_t)private_mib_base_struct.fan_dpc_info.mib.temp_set);
        private_mib_base_struct.fan_dpc_info.mib.temp_set = 0;
        //code here
        p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      }
      else{
        p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
      }
    }
    break;
  case SET_FAN_DPC_H_TEMP_W_POINT:
    {
      if(TYPE == DKD51_BDP){
        fan_dpc_switch_uart_t.rx_enable   = STATE_ON;
        p_modbus->modbus_state = START;
        p_modbus->data_pointer = 0;
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        DKD51_set_fan_H_temp_W_point((uint16_t)private_mib_base_struct.fan_dpc_info.mib.temp_set);
        private_mib_base_struct.fan_dpc_info.mib.temp_set = 0;
        //code here
        p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      }
      else{
        p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
      }
    }
    break;
  case SET_FAN_DPC_L_TEMP_W_POINT:
    {
      if(TYPE == DKD51_BDP){
        fan_dpc_switch_uart_t.rx_enable   = STATE_ON;
        p_modbus->modbus_state = START;
        p_modbus->data_pointer = 0;
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        DKD51_set_fan_L_temp_W_point((int16_t)private_mib_base_struct.fan_dpc_info.mib.i_tem_set);
        private_mib_base_struct.fan_dpc_info.mib.i_tem_set = 0;
        //code here
        p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      }
      else{
        p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
      }
    }
    break;
  case SET_FAN_DPC_HEATING_START_POINT:
    {
      if(TYPE == DKD51_BDP){
        fan_dpc_switch_uart_t.rx_enable   = STATE_ON;
        p_modbus->modbus_state = START;
        p_modbus->data_pointer = 0;
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        DKD51_set_fan_heating_start_point((int16_t)private_mib_base_struct.fan_dpc_info.mib.i_tem_set);
        private_mib_base_struct.fan_dpc_info.mib.i_tem_set = 0;
        //code here
        p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      }
      else{
        p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
      }             
    }
    break;
  case SET_FAN_DPC_HEATING_SENSITIVE_POINT:
    {
      if(TYPE == DKD51_BDP){
        fan_dpc_switch_uart_t.rx_enable   = STATE_ON;
        p_modbus->modbus_state = START;
        p_modbus->data_pointer = 0;
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        DKD51_set_fan_heating_sensitive_point((int16_t)private_mib_base_struct.fan_dpc_info.mib.i_tem_set);
        private_mib_base_struct.fan_dpc_info.mib.i_tem_set = 0;
        //code here
        p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      }
      else{
        p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
      }             
    }
    break;
    //set vac-----------------------------
  case SET_VAC_SYSMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,0,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_sys_mode_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 0, private_mib_base_struct.vacGroup.vacTable[0].vacSysModeWrite);
      p_modbus->running_step     = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_FANSPEEDMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,1,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_fan_speed_mode_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 1, private_mib_base_struct.vacGroup.vacTable[0].vacFanSpeedModeWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_FANSTARTTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,2,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_fan_start_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 2, private_mib_base_struct.vacGroup.vacTable[0].vacFanStartTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_SETTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,3,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_set_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 3, private_mib_base_struct.vacGroup.vacTable[0].vacSetTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_PIDOFFSETTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,4,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_pid_offset_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 4, private_mib_base_struct.vacGroup.vacTable[0].vacPidOffsetTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_FANMINSPEED:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,5,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_fan_min_speed_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 5, private_mib_base_struct.vacGroup.vacTable[0].vacFanMinSpeedWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_FANMAXSPEED:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,6,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_fan_max_speed_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 6, private_mib_base_struct.vacGroup.vacTable[0].vacFanMaxSpeedWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_FILTERSTUCKTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,7,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_filter_stuck_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 7, private_mib_base_struct.vacGroup.vacTable[0].vacFilterStuckTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_NIGHTMODEEN:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,8,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_night_mode_en_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 8, private_mib_base_struct.vacGroup.vacTable[0].vacNightModeEnWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_NIGHTMODESTART:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,9,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_night_mode_start_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 9, private_mib_base_struct.vacGroup.vacTable[0].vacNightModeStartWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_NIGHTMODEEND:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,10,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_night_mode_end_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 10, private_mib_base_struct.vacGroup.vacTable[0].vacNightModeEndWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_NIGHTMAXSPEED:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,11,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_night_max_speed_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 11, private_mib_base_struct.vacGroup.vacTable[0].vacNightMaxSpeedWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_MANUALMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,12,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_manual_mode_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 12, private_mib_base_struct.vacGroup.vacTable[0].vacManualModeWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_MANUALMAXSPEED:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,13,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_manual_max_speed_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 13, private_mib_base_struct.vacGroup.vacTable[0].vacManualMaxSpeedWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_INMAXTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,14,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_in_max_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 14, private_mib_base_struct.vacGroup.vacTable[0].vacInMaxTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_EXMAXTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,15,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_ex_max_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 15, private_mib_base_struct.vacGroup.vacTable[0].vacExMaxTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_FROSTMAXTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,16,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_frost_max_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 16, private_mib_base_struct.vacGroup.vacTable[0].vacFrostMaxTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_INMINTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,17,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_in_min_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 17, private_mib_base_struct.vacGroup.vacTable[0].vacInMinTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_EXMINTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,18,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_ex_min_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 18, private_mib_base_struct.vacGroup.vacTable[0].vacExMinTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_FROSTMINTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,19,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_frost_min_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 19, private_mib_base_struct.vacGroup.vacTable[0].vacFrostMinTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_MINOUTTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,20,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_min_out_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 20, private_mib_base_struct.vacGroup.vacTable[0].vacMinOutTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_DELTATEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,21,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_delta_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 21, private_mib_base_struct.vacGroup.vacTable[0].vacDeltaTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_PANICTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,22,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_panic_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 22, private_mib_base_struct.vacGroup.vacTable[0].vacPanicTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_ACU1ONTEMP:
    {   
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,23,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_acu1_on_temp,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 23, private_mib_base_struct.vacGroup.vacTable[0].vacACU1OnTemp);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_ACU2ONTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,24,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_acu2_on_temp,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 24, private_mib_base_struct.vacGroup.vacTable[0].vacACU2OnTemp);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_ACU2EN:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,25,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_acu2_en,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 25, private_mib_base_struct.vacGroup.vacTable[0].vacACU2En);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_SYNCTIME:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      set_var_time(p_modbus,p_modbus_manager,35,26,sync_time,
                   _VAC_SLAVE, 4500);
      //SetVar_Time(35, 26, SyncTime);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCON1MODEL:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,30,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con1_model_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 30, private_mib_base_struct.vacGroup.vacTable[0].vacAirCon1ModelWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCON1TYPE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,31,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con1_type_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 31, private_mib_base_struct.vacGroup.vacTable[0].vacAirCon1TypeWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCON2MODEL:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,32,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con2_model_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 32, private_mib_base_struct.vacGroup.vacTable[0].vacAirCon2ModelWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCON2TYPE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,33,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con2_type_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 33, private_mib_base_struct.vacGroup.vacTable[0].vacAirCon2TypeWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCONONOFF:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,34,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con_on_off_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 34, private_mib_base_struct.vacGroup.vacTable[0].vacAirConOnOffWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCONMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,35,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con_mode_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 35, private_mib_base_struct.vacGroup.vacTable[0].vacAirConModeWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCONTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,36,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con_temp_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 36, private_mib_base_struct.vacGroup.vacTable[0].vacAirConTempWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCONSPEED:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,37,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_con_speed_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 37, private_mib_base_struct.vacGroup.vacTable[0].vacAirConSpeedWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_VAC_AIRCONDDIR:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,38,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_air_cond_dir_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 38, private_mib_base_struct.vacGroup.vacTable[0].vacAircondDirWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;  
  case SET_VAC_W_ENABLE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,80,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_w_enable_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 80, private_mib_base_struct.vacGroup.vacTable[0].vacWEnableWrite);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break; 
  case SET_VAC_W_SERIAL1:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,81,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_w_serial1_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 81, private_mib_base_struct.vacGroup.vacTable[0].vacWSerial1Write);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break; 
  case SET_VAC_W_SERIAL2:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,82,private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_w_serial2_write,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 82, private_mib_base_struct.vacGroup.vacTable[0].vacWSerial2Write);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break; 
  case SET_VAC_RESET:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state     = START;
      p_modbus->data_pointer     = 0;
      mater_write_i16(p_modbus,p_modbus_manager,35,47,1,
                      _VAC_SLAVE,4500);
      //SetVar_i16VAC(35, 47, 1);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break; 
    //set lib batt--------------------------------
  case SET_LIB_PACKHIGHVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = START;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 0, private_mib_base_struct.liBattGroup.liBattTable[0].i32PackHighVolt_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BATTHIGHVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = START;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 2, private_mib_base_struct.liBattGroup.liBattTable[0].i32BattHighVolt_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CELLHIGHVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = START;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 4, private_mib_base_struct.liBattGroup.liBattTable[0].i32CellHighVolt_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BATTLOWVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = START;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 6, private_mib_base_struct.liBattGroup.liBattTable[0].i32BattLowVolt_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;   
    }
    break;
  case SET_LIB_CELLLOWVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = START;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 8, private_mib_base_struct.liBattGroup.liBattTable[0].i32CellLowVolt_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;  
    }
    break;
  case SET_LIB_CHARHIGHCURR:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 10, private_mib_base_struct.liBattGroup.liBattTable[0].i32CharHighCurr_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
    }
    break;
  case SET_LIB_DISCHARHIGHCURR:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 12, private_mib_base_struct.liBattGroup.liBattTable[0].i32DisCharHighCurr_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CHARHIGHTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 14, private_mib_base_struct.liBattGroup.liBattTable[0].i32CharHighTemp_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DISCHARHIGHTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 16, private_mib_base_struct.liBattGroup.liBattTable[0].i32DisCharHighTemp_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CHARLOWTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 18, private_mib_base_struct.liBattGroup.liBattTable[0].i32CharLowTemp_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DISCHARLOWTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 20, private_mib_base_struct.liBattGroup.liBattTable[0].i32DisCharLowtemp_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_LOWCAP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 22, private_mib_base_struct.liBattGroup.liBattTable[0].i32LowCap_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BMSHIGHTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 24, private_mib_base_struct.liBattGroup.liBattTable[0].i32BMSHighTemp_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BMSLOWTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 26, private_mib_base_struct.liBattGroup.liBattTable[0].i32BMSLowTemp_AWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_PACKOVERVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 28, private_mib_base_struct.liBattGroup.liBattTable[0].i32PackOverVolt_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BATTOVERVOLT:
    {  
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 30, private_mib_base_struct.liBattGroup.liBattTable[0].i32BattOverVolt_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CELLOVERVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 32, private_mib_base_struct.liBattGroup.liBattTable[0].i32CellOverVolt_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BATTUNDERVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 34, private_mib_base_struct.liBattGroup.liBattTable[0].i32BattUnderVolt_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CELLUNDERVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 36, private_mib_base_struct.liBattGroup.liBattTable[0].i32CellUnderVolt_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CHAROVERCURR:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 38, private_mib_base_struct.liBattGroup.liBattTable[0].i32CharOverCurr_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DISCHAROVERCURR:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 40, private_mib_base_struct.liBattGroup.liBattTable[0].i32DisCharOverCurr_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CHAROVERTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 42, private_mib_base_struct.liBattGroup.liBattTable[0].i32CharOverTemp_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DISCHAROVERTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 44, private_mib_base_struct.liBattGroup.liBattTable[0].i32DisCharOverTemp_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CHARUNDERTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 46, private_mib_base_struct.liBattGroup.liBattTable[0].i32CharUnderTemp_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DISCHARUNDERTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 48, private_mib_base_struct.liBattGroup.liBattTable[0].i32DisCharUnderTemp_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_UNDERCAP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 50, private_mib_base_struct.liBattGroup.liBattTable[0].i32UnderCap_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BMSOVERTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 52, private_mib_base_struct.liBattGroup.liBattTable[0].i32BMSOverTemp_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_BMSUNDERTEMP:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 54, private_mib_base_struct.liBattGroup.liBattTable[0].i32BMSUnderTemp_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DIFFERENTVOLT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 56, private_mib_base_struct.liBattGroup.liBattTable[0].i32DifferentVolt_PWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_VOLTBALANCE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 60, private_mib_base_struct.liBattGroup.liBattTable[0].u32VoltBalanceWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DELTAVOLTBALANCE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 62, private_mib_base_struct.liBattGroup.liBattTable[0].u32DeltaVoltBalanceWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_DISCHARCURRLIMIT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 64, private_mib_base_struct.liBattGroup.liBattTable[0].u32DisCharCurrLimitWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CHARCURRLIMIT:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 66, private_mib_base_struct.liBattGroup.liBattTable[0].u32CharCurrLimitWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_VOLTDISCHARREF:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 72, private_mib_base_struct.liBattGroup.liBattTable[0].u32VoltDisCharRefWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_VOLTDISCHARCMD:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,LibM1Addr, 74, private_mib_base_struct.liBattGroup.liBattTable[0].u32VoltDisCharCMDWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_SYSTEMBYTE://////////////////////////////
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_u16(p_modbus,p_modbus_manager,LibM1Addr, 76, (uint16_t)private_mib_base_struct.liBattGroup.liBattTable[0].u32SystemByteWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_KEYTIME:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_u16(p_modbus,p_modbus_manager,LibM1Addr, 77, (uint16_t)private_mib_base_struct.liBattGroup.liBattTable[0].u32KeyTimeWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_INIMAXIM:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_u16(p_modbus,p_modbus_manager,LibM1Addr, 162, (uint16_t)private_mib_base_struct.liBattGroup.liBattTable[0].u32IniMaximWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_ENABLELOCK:
    {   
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_u16(p_modbus,p_modbus_manager,LibM1Addr, 115, (uint16_t)private_mib_base_struct.liBattGroup.liBattTable[0].u32EnableLockWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_LIB_CAPINIT:
    {   
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      p_modbus->data_pointer = 0;
      mater_write_u16(p_modbus,p_modbus_manager,LibM1Addr, 164, (uint16_t)private_mib_base_struct.liBattGroup.liBattTable[0].u16_liionCapInitWrite,
                      _LIB_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
    //set gen -------------------------------------------------------------
  case SET_GEN_AUTOMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 1) // KOBUTA
      {
        p_modbus->data_pointer = 0;
        mater_write_06_u16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID, 760,0x10,
                           _GEN_SLAVE,4500);
      }
      else if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 3) // DEEPSEA
      {
        p_modbus->data_pointer = 0;
        mater_write_i16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID,4104,35701,
                        _GEN_SLAVE,4500);

      }
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_GEN_MANUALMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 1) // KOBUTA
      {
        p_modbus->data_pointer = 0;
        mater_write_06_u16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID, 760,0x40,
                           _GEN_SLAVE,4500);
      }
      else if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 3) // DEEPSEA
      {
        p_modbus->data_pointer = 0;
        mater_write_i16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID,4104,35702,
                        _GEN_SLAVE,4500);
        
      }
       
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_GEN_STARTMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 1) // KOBUTA
      {
        p_modbus->data_pointer = 0;
        mater_write_06_u16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID, 760,0x01,
                           _GEN_SLAVE,4500);
        
      }
      else if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 3) // DEEPSEA
      {
        p_modbus->data_pointer = 0;
        mater_write_i16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID,4104,35705,
                        _GEN_SLAVE,4500);
        
      }
     
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_GEN_STOPMODE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 1) // KOBUTA
      {
        p_modbus->data_pointer = 0;
        mater_write_06_u16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID, 760,0x02,
                           _GEN_SLAVE,4500);
       
      }
      else if (p_modbus_manager->gen_manager_struct[private_mib_base_struct.genGroup.genflag].gen_type == 3) // DEEPSEA
      {
        p_modbus->data_pointer = 0;
        mater_write_i16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.genGroup.genTable[private_mib_base_struct.genGroup.genflag].genStatusModbusID,4104,35700,
                        _GEN_SLAVE,4500);
        
      }
      
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  //set smbc--------------------------------------------------------------------------------------
  case SET_SMCB_STATE:
    {
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->modbus_state = 0;
      vTaskDelay(1000); // do toc do xu ly ban tin ko dung id cham, nen phai them tre
      p_modbus->data_pointer = 0;
      if (p_modbus_manager->smcb_manager_struct[private_mib_base_struct.smcbGroup.flag].smcb_type == 1) // OPEN
        mater_write_06_u16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.smcbGroup.SmcbTable[private_mib_base_struct.smcbGroup.flag].SmcbModbusID, 0,(uint16_t) private_mib_base_struct.smcbGroup.SmcbTable[private_mib_base_struct.smcbGroup.flag].SmcbStateWrite,
                           _SMBC_SLAVE,4500);
      else if (p_modbus_manager->smcb_manager_struct[private_mib_base_struct.smcbGroup.flag].smcb_type == 2) // MATIS
        mater_write_06_u16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.smcbGroup.SmcbTable[private_mib_base_struct.smcbGroup.flag].SmcbModbusID, 17,(uint16_t) private_mib_base_struct.smcbGroup.SmcbTable[private_mib_base_struct.smcbGroup.flag].SmcbStateWrite+1,
                           _SMBC_SLAVE,4500);
      else if (p_modbus_manager->smcb_manager_struct[private_mib_base_struct.smcbGroup.flag].smcb_type == 3) // GOL
        mater_write_06_u16(p_modbus,p_modbus_manager,(uint8_t)private_mib_base_struct.smcbGroup.SmcbTable[private_mib_base_struct.smcbGroup.flag].SmcbModbusID, 32768,(uint16_t) private_mib_base_struct.smcbGroup.SmcbTable[private_mib_base_struct.smcbGroup.flag].SmcbStateWrite+1,
                           _SMBC_SLAVE,4500);
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  // set bm--------------------------------------------------------------------------------------
  case SET_BM_VOLTDIFF:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)  // thiet bi hien tai > tong so thiet bi
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
       mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 32, private_mib_base_struct.bmGroup.bmTable[0].bmVoltDiff,
                       _BM_SLAVE,4500);   
      p_modbus_manager->bm_current_index++;                                      // chuyen sang thiet bi tiep
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_MAXTEMP:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 34, private_mib_base_struct.bmGroup.bmTable[0].bmMaxTemp,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_VOLTTHRES:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 36, private_mib_base_struct.bmGroup.bmTable[0].bmVoltThres,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_CURRTHRES:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 38, private_mib_base_struct.bmGroup.bmTable[0].bmCurrThres,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_TIMETHRES:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 40, private_mib_base_struct.bmGroup.bmTable[0].bmTimeThres,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_SOCTHRES:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 42, private_mib_base_struct.bmGroup.bmTable[0].bmSOCThres,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_MINTEMP:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 44, private_mib_base_struct.bmGroup.bmTable[0].bmMinTemp,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_LOWCAPTIME:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 46, private_mib_base_struct.bmGroup.bmTable[0].bmLowCapTime,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_HALFVOLTALARM:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 50, private_mib_base_struct.bmGroup.bmTable[0].bmHalfVoltAlarm,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_RECHARGEVOLT:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 52, private_mib_base_struct.bmGroup.bmTable[0].bmRechargeVolt,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  case SET_BM_CURRBALANCETHRES:
    {
      if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
      {
        p_modbus_manager->bm_current_index = 0;
      }
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      p_modbus->data_pointer = 0;
      mater_write_i32(p_modbus,p_modbus_manager,70 + p_modbus_manager->bm_current_index, 54, private_mib_base_struct.bmGroup.bmTable[0].bmCurrBalanceThres,
                      _BM_SLAVE,4500);
      p_modbus_manager->bm_current_index++;
      p_modbus->running_step = _WAIT_WRITE_MULTI_REG;
      
    }
    break;
  default:
    p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
    break;
    
  }
  //marter read------------------------------------------------------------------------
  switch(p_modbus->running_step){
    case _TEST_MODE:    
    {
      sTestRS485.Cnt++;
      if(sTestRS485.RS485CabRecv!=0)
      {
        
      }
      else
      {
        for(uint8_t i=0;i<15;i++)
        {
          if(sTestRS485.CabBuff[i] ==i)
          {
            sTestRS485.Numberofbyterecvok++;
          } 
        }
        if(sTestRS485.Numberofbyterecvok==15)
        {
          sTestRS485.RS485CabRecv =1;
          sTestRS485.Numberofbyterecvok = 0;
          
          GPIO_PinWrite(GPIO2,7U,1);
          GPIO_PinWrite(GPIO2,11U,1);
          LPUART_WriteBlocking(LPUART4, &sTestRS485.CabBuff[0], 15);
          delay_modbus_rtu(4500);
          GPIO_PinWrite(GPIO2,7U,0);
          GPIO_PinWrite(GPIO2,11U,0); 
          
        }
        else 
        {
          sTestRS485.RS485CabRecv =0;
          sTestRS485.Numberofbyterecvok = 0;
        }
        
      }
      if(sTestRS485.RS485GenRecv!=0)
      {
        
      }
      else
      {
        for(uint8_t i=0;i<15;i++)
        {
          if(sTestRS485.GenBuff[i] ==i)
          {
            sTestRS485.Numberofbyterecvok++;
          } 
        }
        if(sTestRS485.Numberofbyterecvok==15)
        {
          sTestRS485.RS485GenRecv =1;
          sTestRS485.Numberofbyterecvok = 0;
          
          GPIO_PinWrite(GPIO2, 6U,1);
          GPIO_PinWrite(GPIO3, 26U,1);
          LPUART_WriteBlocking(LPUART3, &sTestRS485.GenBuff[0], 15);
          delay_modbus_rtu(4500);
          GPIO_PinWrite(GPIO2, 6U,0);
          GPIO_PinWrite(GPIO3, 26U,0); 
          
        }
        else 
        {
          sTestRS485.RS485GenRecv =0;
          sTestRS485.Numberofbyterecvok = 0;
        }
        
      }
      if(sTestRS485.Cnt>10) 
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY); 
        p_modbus->running_step = _WAIT_MODE;
        sTestRS485.requesttestRS485=0;
        sTestRS485.InitUart=0;
      }
      
    }
    break;
    //wait mode----------------------------
    case _WAIT_MODE:
      fan_dpc_switch_uart_t.rx_enable    = STATE_OFF;
      fan_dpc_switch_uart_t.fan_state_e  = _DKD51_FAN_SYS_REQ;
      vTaskDelay(500);
      p_modbus->running_step = _READ_LIB_STATUS;
      
      //init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
      break;
    //slave lib-----------------------------
    case _READ_LIB_STATUS:
      {
        p_modbus->running_step    = read_lib_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_LIB_RESPOND:
      {
        p_modbus->running_step    = wait_lib_respond(p_modbus,p_modbus_manager);
      }
      break;
    //slave gen------------------------------
    case _READ_GEN_STATUS:
      {
        p_modbus->running_step    = read_gen_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_GEN_RESPOND:
      {
        p_modbus->running_step    = wait_gen_respond(p_modbus,p_modbus_manager);
      }
      break;
    //slave pm------------------------------
    case _READ_PM_STATUS:
      {
        p_modbus->running_step    = read_pm_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_PM_RESPOND:
      {
        p_modbus->running_step    = wait_pm_respond(p_modbus,p_modbus_manager);
      }
      break;
    //slave vac-----------------------------
    case _READ_VAC_STATUS:
      {
        p_modbus->running_step    = read_vac_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_VAC_RESPOND:
      {
        p_modbus->running_step    = wait_vac_respond(p_modbus,p_modbus_manager);
      }
      break;
    //slave smcb----------------------------
    case _READ_SMCB_STATUS:
      {
        p_modbus->running_step    = read_smbc_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_SMCB_RESPOND:
      p_modbus->running_step      = wait_smcb_respond(p_modbus,p_modbus_manager);
      break;
    //slave smcb-----------------------------
    case _READ_FUEL_STATUS:
      {
        p_modbus->running_step    = read_fuel_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_FUEL_RESPOND:
      {
        p_modbus->running_step    = wait_fuel_respond(p_modbus,p_modbus_manager);
      }
      break;
    //slave isense---------------------------
    case _READ_ISENSE_STATUS:
      {
        p_modbus->running_step    = read_isense_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_ISENSE_RESPOND:
      {
        p_modbus->running_step    = wait_isense_respond(p_modbus,p_modbus_manager);
      }
      break;
    //slave pmdc----------------------------
    case _READ_PM_DC_STATUS:
      {
        p_modbus->running_step    = read_pmdc_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_PM_DC_RESPOND:
      {
        p_modbus->running_step    = wait_pmdc_respond(p_modbus,p_modbus_manager);
      }
      break; 
    //slave bm-------------------------------
    case _READ_BM_STATUS:
      {
        p_modbus->running_step    = read_bm_status(p_modbus,p_modbus_manager);
      }
      break;
    case _WAIT_BM_RESPOND:
      {
        p_modbus->running_step    = wait_bm_respond(p_modbus,p_modbus_manager);
      }
      break;
    //fan dpc--------------------------------  
    case _READ_FAN_DPC_STATUS:
      {
        p_modbus->running_step    = read_fan_dpc_status(p_modbus,p_modbus_manager);
       
      }
      break;
    case _WAIT_FAN_DPC_RESPOND:
      {
        p_modbus->running_step   = wait_fan_dpc_respond(p_modbus,p_modbus_manager);
      }
      break;
    //wait set-------------------------------
    case _WAIT_WRITE_MULTI_REG:
      {
        vTaskDelay(300);
        //check respond?
        switch(p_modbus_manager->setting_cmd){
          //slave vac------------------------------------------------------------
        case SET_VAC_SYSMODE:
        case SET_VAC_FANSPEEDMODE:
        case SET_VAC_FANSTARTTEMP:
        case SET_VAC_SETTEMP:
        case SET_VAC_PIDOFFSETTEMP:
        case SET_VAC_FANMINSPEED:
        case SET_VAC_FANMAXSPEED:
        case SET_VAC_FILTERSTUCKTEMP:
        case SET_VAC_NIGHTMODEEN:
        case SET_VAC_NIGHTMODESTART:
        case SET_VAC_NIGHTMODEEND:
        case SET_VAC_NIGHTMAXSPEED:
        case SET_VAC_MANUALMODE:
        case SET_VAC_MANUALMAXSPEED:
        case SET_VAC_INMAXTEMP:
        case SET_VAC_EXMAXTEMP:
        case SET_VAC_FROSTMAXTEMP:
        case SET_VAC_INMINTEMP:
        case SET_VAC_EXMINTEMP:
        case SET_VAC_FROSTMINTEMP:
        case SET_VAC_MINOUTTEMP:
        case SET_VAC_DELTATEMP:
        case SET_VAC_PANICTEMP:
        case SET_VAC_ACU1ONTEMP:
        case SET_VAC_ACU2ONTEMP:
        case SET_VAC_ACU2EN:
        case SET_VAC_SYNCTIME:
        case SET_VAC_AIRCON1MODEL:
        case SET_VAC_AIRCON1TYPE:
        case SET_VAC_AIRCON2MODEL:
        case SET_VAC_AIRCON2TYPE:
        case SET_VAC_AIRCONONOFF:
        case SET_VAC_AIRCONMODE:
        case SET_VAC_AIRCONTEMP:
        case SET_VAC_AIRCONSPEED:
        case SET_VAC_AIRCONDDIR:
        case SET_VAC_W_ENABLE:
        case SET_VAC_W_SERIAL1:
        case SET_VAC_W_SERIAL2:
        case SET_VAC_RESET:
          {
            p_modbus_manager->vac_current_index = 0;
            p_modbus->running_step              = _READ_VAC_STATUS;                     
            p_modbus_manager->setting_cmd       = SET_DAQ_NOOP; 
          }break;
          //slave lib batt-------------------------------------------------------
        case SET_LIB_PACKHIGHVOLT:
        case SET_LIB_BATTHIGHVOLT:
        case SET_LIB_CELLHIGHVOLT:
        case SET_LIB_BATTLOWVOLT:
        case SET_LIB_CELLLOWVOLT:
        case SET_LIB_CHARHIGHCURR:
        case SET_LIB_DISCHARHIGHCURR:
        case SET_LIB_CHARHIGHTEMP:
        case SET_LIB_DISCHARHIGHTEMP:
        case SET_LIB_CHARLOWTEMP:
        case SET_LIB_DISCHARLOWTEMP:
        case SET_LIB_LOWCAP:
        case SET_LIB_BMSHIGHTEMP:
        case SET_LIB_BMSLOWTEMP:
        case SET_LIB_PACKOVERVOLT:
        case SET_LIB_BATTOVERVOLT:
        case SET_LIB_CELLOVERVOLT:
        case SET_LIB_BATTUNDERVOLT:
        case SET_LIB_CELLUNDERVOLT:
        case SET_LIB_CHAROVERCURR:
        case SET_LIB_DISCHAROVERCURR:
        case SET_LIB_CHAROVERTEMP:
        case SET_LIB_DISCHAROVERTEMP:
        case SET_LIB_CHARUNDERTEMP:
        case SET_LIB_DISCHARUNDERTEMP:
        case SET_LIB_UNDERCAP:
        case SET_LIB_BMSOVERTEMP:
        case SET_LIB_BMSUNDERTEMP:
        case SET_LIB_DIFFERENTVOLT:
        case SET_LIB_VOLTBALANCE:
        case SET_LIB_DELTAVOLTBALANCE:
        case SET_LIB_DISCHARCURRLIMIT:
        case SET_LIB_CHARCURRLIMIT:
        case SET_LIB_VOLTDISCHARREF:
        case SET_LIB_VOLTDISCHARCMD:
        case SET_LIB_SYSTEMBYTE:
        case SET_LIB_KEYTIME:
        case SET_LIB_INIMAXIM:
        case SET_LIB_ENABLEKCS:
        case SET_LIB_ENABLELOCK:
        case SET_LIB_CAPINIT:
          {
            p_modbus_manager->lib_current_index = 0;
            p_modbus->running_step              = _READ_LIB_STATUS;                     
            p_modbus_manager->setting_cmd       = SET_DAQ_NOOP; 
          }break;
        case SET_GEN_AUTOMODE:
        case SET_GEN_STOPMODE:
        case SET_GEN_STARTMODE:
        case SET_GEN_MANUALMODE:
          {
            p_modbus_manager->gen_current_index   = 0;
            p_modbus->running_step                = _READ_GEN_STATUS;                     
            p_modbus_manager->setting_cmd         = SET_DAQ_NOOP; 
          }break;
          //slave smbc-------------------------------------------------
        case SET_SMCB_STATE:
          {
            vTaskDelay(2000); // do SMCB update thong so cham, nen phai doi 1 thoi gian sau khi cai dat moi hoi lai
            p_modbus_manager->smcb_current_index = 0;
            p_modbus->running_step               = _READ_SMCB_STATUS;                     
            p_modbus_manager->setting_cmd        = SET_DAQ_NOOP; 
          }break;
        case SET_BM_VOLTDIFF:
        case SET_BM_MAXTEMP:
        case SET_BM_VOLTTHRES:
        case SET_BM_CURRTHRES:
        case SET_BM_TIMETHRES:
        case SET_BM_SOCTHRES:
        case SET_BM_MINTEMP:
        case SET_BM_LOWCAPTIME:
        case SET_BM_HALFVOLTALARM:
        case SET_BM_RECHARGEVOLT:
        case SET_BM_CURRBALANCETHRES: 
          {
            if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
            {
              p_modbus_manager->bm_current_index = 0;
              p_modbus->running_step = _READ_BM_STATUS;                    
              p_modbus_manager->setting_cmd = SET_DAQ_NOOP;  
            }
          }break;
        case SET_FAN_DPC_STARTING_POINT:
        case SET_FAN_DPC_SENSITIVE_POINT:
        case SET_FAN_DPC_H_TEMP_W_POINT:
        case SET_FAN_DPC_L_TEMP_W_POINT:
        case SET_FAN_DPC_HEATING_START_POINT:
        case SET_FAN_DPC_HEATING_SENSITIVE_POINT:
          {
            p_modbus->running_step = _READ_FAN_DPC_STATUS;                     
            p_modbus_manager->setting_cmd = SET_DAQ_NOOP;
          }
          break;
        default:
          break;
          
        }
      }
      break;
    //default--------------------------------  
    default:
      p_modbus->running_step = _WAIT_MODE;
      break;
  }
  //code here
}

//function slave----------------------------------------

// read lib status----------------
RTU_STATE read_lib_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->modbus_state = START;
  p_modbus->data_pointer = 0;
  
  if(p_modbus_manager->number_of_lib !=0 ){
    if(p_modbus_manager->lib_current_index > p_modbus_manager->number_of_lib -1 ){
      p_modbus_manager->lib_current_index =0;  
    }
    switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_type){
    case 13:// COSLIGHT_CF4850T---------------------------------------------------------------  
    case 1:// COSLIGHT------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_19200,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +1;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_COSLIGHT_INFO_1:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            2500);
          break;
        case LIB_COSLIGHT_INFO_2:
          master_read_modbus(p_modbus,0x02, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            4500);          
          
          break;
        case LIB_COSLIGHT_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            4500);          
          break;
          default:
            break;
        }
      }
      break;
    case 2:// COSLIGHT_OLD_V1.1---------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_19200,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +1;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_COSLIGHT_OLD_INFO_1:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            2500);
          break;
        case LIB_COSLIGHT_OLD_INFO_2:
          master_read_modbus(p_modbus,0x02, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            4500);          
          
          break;
        case LIB_COSLIGHT_OLD_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            4500);          
          break;
          default:
            break;
        }
      }
      break;
    case 12:// SHOTO_SDA10_48100--------------------------------------------------------------  
    case 3:// SHOTO_2019----------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +1;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_SHOTO_INFO_1:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            2500);
          break;
        case LIB_SHOTO_INFO_2:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            2500);          
          
          break;
        case LIB_SHOTO_INFO_3:
          master_read_modbus(p_modbus,0x01, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            2500);          
          break;
        case LIB_SHOTO_INFO_4:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[3],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[3],
                                            2500);          
          break;         
          default:
            break;
        }
        
      }
      break;
    case 4:// HUAWEI -------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        if(p_modbus_manager->lib_current_index <8){
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +214;
        }else{
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +216;
        }
           
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_HUAWEI_INFO_1:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            4500);
          break;
        case LIB_HUAWEI_INFO_2:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            4500);          
          
          break;
        case LIB_HUAWEI_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            4500);          
          break;
        case LIB_HUAWEI_INFO_4:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[3],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[3],
                                            4500);          
          break;         
          default:
            break;
        }       
      }
      break;
    case 5:// M1Viettel150--------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +1;
        
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_VIETTEL_INFO_1:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            2500);
          break;
        case LIB_VIETTEL_INFO_2:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            2500);          
          
          break;
        case LIB_VIETTEL_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            2500);          
          break;
        case LIB_VIETTEL_INFO_4:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[3],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[3],
                                            2500);          
          break;
        case LIB_VIETTEL_INFO_5:
          LoadCurr     = private_mib_base_struct.loadGroup.loadCurrent;
          ACFaultM1    = private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains;
          PMUConnectM1 = private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect;
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            110,
                                            6,
                                            2500);
          break;
          default:
            break;
        }         
      }
      break;
    case 14:// HUAFU_HF48100C----------------------------------------------------------------- 
    case 6:// ZTT_2020------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +1;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_ZTT_2020_INFO_1:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            4500);
          break;
        case LIB_ZTT_2020_INFO_2:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            4500);          
          
          break;
        case LIB_ZTT_2020_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            4500);          
          break;
          default:
            break;
        }        
      }
      break; 
    case 7:// ZTT50--------------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +39;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_ZTT_INFO_1:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            2500);
          break;
        case LIB_ZTT_INFO_2:
          master_read_modbus(p_modbus,0x11, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            0,
                                            0,
                                            4500);          
          
          break;
          default:
            break;
        }        
      }
      break;       
    case 8:// HUAWEI_A1-----------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        
        if(p_modbus_manager->lib_current_index <8){
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +214;
        }else{
          p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +216;
        } 
        
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_HUAWEI_A1_INFO_1:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            4500);
          break;
        case LIB_HUAWEI_A1_INFO_2:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            4500);          
          
          break;
        case LIB_HUAWEI_A1_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            4500);          
          break;
          default:
            break;
        }        
      }
      break;
    case 9://SAFT-----------------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +1;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_SAFT_INFO_1:
          {
          //code here--------------
            uint8_t key_saft[4] = {0};
            
          mater_write_multi_reg (p_modbus,p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                           0x04, 
                           &key_saft[0], 
                           2,
                           4500);
          vTaskDelay(1000);
          p_modbus->modbus_state =START;
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            2500);
          }
          break;
          default:
            break;
        }
      }
      break;
    case 10:// Narada75-----------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +39;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_NARADA75_INFO_1:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            2500);
          break;
        case LIB_NARADA75_INFO_2:
          master_read_modbus(p_modbus,0x11, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            0,
                                            0,
                                            4500);          
          
          break;
          default:
            break;
        }        
      }
      break;
    case 11:// EVE
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id = p_modbus_manager->lib_current_index +1;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step){
        case LIB_EVE_INFO_1:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[0],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[0],
                                            4500);
          break;
        case LIB_EVE_INFO_2:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[1],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[1],
                                            4500);          
          
          break;
        case LIB_EVE_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_abs_slave_id,
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_base_addr[2],
                                            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_num_of_reg[2],
                                            4500);          
          break;
          default:
            break;
        }       
      }
      break; 
    }
    return _WAIT_LIB_RESPOND;
  }else{
    return _READ_GEN_STATUS;
  }
  
}

//wait lib respond---------------
RTU_STATE wait_lib_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_LIB_SLAVE);
  
  if( p_modbus_manager->lib_current_index <= (p_modbus_manager->number_of_lib - 1))
  {
    if(check_val != 1)
    {
      p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8NoResponseCnt++;
      if (p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8NoResponseCnt >= 5)
      {
        p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8ErrorFlag = 1;
        p_modbus_manager->cnt_dis_connect_lib[p_modbus_manager->lib_current_index]++;
        switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_type)
        {
        case 13:   
        case 1: // coslight
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_INFO_1;
          }
          break;
        case 2:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_COSLIGHT_OLD_INFO_1;
          }
          break;
        case 12:
        case 3:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_SHOTO_INFO_1;
          }
          break;
        case 4:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_INFO_1;
          }
          break;
        case 5:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_VIETTEL_INFO_1;
          }
          break;
        case 14:
        case 6:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_ZTT_2020_INFO_1;
          }
          break;
        case 7:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_ZTT_INFO_1;
          }
          break;
        case 8:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_HUAWEI_A1_INFO_1;
          }
          break;
        case 9:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_SAFT_INFO_1;
          }
          break;
        case 10:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_NARADA75_INFO_1;
          }
          break;
        case 11:
          {
            p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step = LIB_EVE_INFO_1;
          }
          break;
        };      
        p_modbus_manager->lib_current_index++;
      }
    }
    else
    {
      p_modbus_manager->lib_info_struct[p_modbus_manager->lib_current_index].u8NoResponseCnt = 0;
      //                      p_modbus_manager->lib_info_struct[p_modbus_manager->u8LIBCurrentIndex].u8ErrorFlag = 0;
      switch(p_modbus_manager->lib_manager_struct[p_modbus_manager->lib_current_index].lib_running_step)
      {
      case LIB_COSLIGHT_INFO_1:
      case LIB_COSLIGHT_OLD_INFO_1:
      case LIB_SHOTO_INFO_1:
      case LIB_HUAWEI_INFO_1:
      case LIB_VIETTEL_INFO_1:
      case LIB_ZTT_2020_INFO_1:
      case LIB_ZTT_INFO_1:
      case LIB_HUAWEI_A1_INFO_1:
      case LIB_SAFT_INFO_1:
      case LIB_NARADA75_INFO_1:
      case LIB_EVE_INFO_1:
        {
          p_modbus_manager->lib_current_index++;
        }break;
      default:
        break;
      };
    }
    
    if (p_modbus_manager->lib_current_index > p_modbus_manager->number_of_lib - 1)
    {
      return _READ_GEN_STATUS;
    }
    else
    {
      return _READ_LIB_STATUS;
    }
  }
  else 
  {
    return _READ_LIB_STATUS;
  }
  
}

//read gen staus------------------
RTU_STATE read_gen_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->data_pointer = 0;
  if(p_modbus_manager->number_of_gen != 0)
  {
    if (p_modbus_manager->gen_current_index > p_modbus_manager->number_of_gen - 1)
    {
      p_modbus_manager->gen_current_index = 0;
    }
    switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_type)
    {
    case 1:// KUBOTA
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
        {
        case GEN_KUBOTA_INFO_1:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[0],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[0],
                                             2500);//1,0x00,85
          }
          break;
        case GEN_KUBOTA_INFO_2:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[1],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[1],
                                             2500);//1,0x20,5
          }
          break;
        default:
          break;
        };
      }
      break;
    case 2:// BE142
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
        {
        case GEN_BE142_INFO_1:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[0],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[0],2500);
          }
          break;
        case GEN_BE142_INFO_2:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[1],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[1],
                                             2500);
          }
          break;
        case GEN_BE142_INFO_3:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[2],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[2],2500);
          }
          break;
        default:
          break;
        };
      }
      break;
    case 3:// DEEPSEA
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
        {
        case GEN_DEEPSEA_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[0],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[0],
                                             2500);
          }
          break;
        case GEN_DEEPSEA_INFO_2:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[1],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[1],
                                             2500);
          }
          break;
        case GEN_DEEPSEA_INFO_3:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[2],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[2],
                                             2500);
          }
          break;
        case GEN_DEEPSEA_INFO_4:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[3],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[3],
                                             2500);
          }
          break;
        default:
          break;
        };
      }
      break;
    case 4: // LR2057
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
        {
        case GEN_LR2057_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[0],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[0],
                                             4500);
          }
          break;
        case GEN_LR2057_INFO_2:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[1],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[1],
                                             4500);
          }
          break;
        case GEN_LR2057_INFO_3:
          {
            master_read_modbus(p_modbus,0x01,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[2],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[2],
                                             2500);
          }
          break;
        default:
          break;
        }
      }
      break;
    case 5:// HIMOINSA
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
        {
        case GEN_HIMOINSA_INFO_1:
          {
            master_read_modbus(p_modbus,0x02,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[0],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[0],
                                             2500);
          }
          break;
        case GEN_HIMOINSA_INFO_2:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[1],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[1],
                                             2500);
          }
          break;
        default:
          break;
        };
      }
      break;
    case 6:// QC315
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
        {
        case GEN_QC315_INFO_1:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[0],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[0],
                                             2500);
          }
          break;
        case GEN_QC315_INFO_2:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[1],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[1],
                                             2500);
          }
          break;
        case GEN_QC315_INFO_3:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[2],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[2],
                                             2500);
          }
          break;
        default:
          break;
        };
      }
      break;
    case 7:// CUMMIN
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
        {
        case GEN_CUMMIN_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[0],
                                             p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[0],
                                             2500);
          }
          break;
          //                      case GEN_CUMMIN_INFO_2:
          //                      {
          //                          Read_Input_Regs_Query_fast(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
          //                                                p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[1],
          //                                                p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[1]);
          //                      }
          //                      break;
          //                      case GEN_CUMMIN_INFO_3:
          //                      {
          //                          Read_Input_Regs_Query_fast(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_abs_slave_id,
          //                                                p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_base_addr[2],
          //                                                p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_num_of_reg[2]);
          //                      }
          //                      break;
        default:
          break;
        };
      }
      break;
    default:
      break;
    }
    return _WAIT_GEN_RESPOND;
  }
  else
  {
    return _READ_PM_STATUS;
    
    
  }
}

//wait gen respond---------------
RTU_STATE wait_gen_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_GEN_SLAVE);
  //reVal = RS4851_Check_Respond_Data_GEN();
  if(p_modbus_manager->gen_current_index <= (p_modbus_manager->number_of_gen - 1))
  {
    if(check_val != 1)
    {
      p_modbus_manager->gen_info_struct[p_modbus_manager->gen_current_index].u8NoResponseCnt++;
      if (p_modbus_manager->gen_info_struct[p_modbus_manager->gen_current_index].u8NoResponseCnt >= 5)
      {
        p_modbus_manager->gen_info_struct[p_modbus_manager->gen_current_index].u8NoResponseCnt = 2;
        p_modbus_manager->gen_info_struct[p_modbus_manager->gen_current_index].u8ErrorFlag = 1;
        p_modbus_manager->cnt_dis_connect_gen[p_modbus_manager->gen_current_index]++;
        switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_type)
        {
        case 1://KUBOTA
          {
            p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_KUBOTA_INFO_1;
          }
          break;
        case 2://BE142
          {
            p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_BE142_INFO_1;
          }
          break;
        case 3://DEEPSEA
          {
            p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_DEEPSEA_INFO_1;
          }
          break;
        case 4: //LR2057
          {
            p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_LR2057_INFO_1;
          }
          break;
        case 5: //HIMOINSA
          {
            p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_HIMOINSA_INFO_1;
          }
          break;
        case 6: //QC315
          {
            p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_QC315_INFO_1;
          }
          break;
        case 7: //CUMMIN
          {
            p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step = GEN_CUMMIN_INFO_1;
          }
          break;
        };  
        p_modbus_manager->gen_current_index++;
      }
    }
    else
    {
      p_modbus_manager->gen_info_struct[p_modbus_manager->gen_current_index].u8NoResponseCnt = 0;
      p_modbus_manager->gen_info_struct[p_modbus_manager->gen_current_index].u8ErrorFlag = 0;
      switch(p_modbus_manager->gen_manager_struct[p_modbus_manager->gen_current_index].gen_running_step)
      {
      case GEN_KUBOTA_INFO_1:
      case GEN_DEEPSEA_INFO_1:
      case GEN_BE142_INFO_1:
      case GEN_LR2057_INFO_1:
      case GEN_HIMOINSA_INFO_1:
      case GEN_QC315_INFO_1:
      case GEN_CUMMIN_INFO_1:
        {
          p_modbus_manager->gen_current_index++;
        }break;                        
      };
    }
    
    if (p_modbus_manager->gen_current_index > p_modbus_manager->number_of_gen - 1)
    {
      return  _READ_PM_STATUS; 
    }
    else
    {
      return _READ_GEN_STATUS;
    }
  }
  else {
    return  _READ_GEN_STATUS;
  }
}

//read pm status-----------------
RTU_STATE read_pm_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->data_pointer = 0;
  if(p_modbus_manager->number_of_pm != 0)           
  {
    //                    ReInit_RS485_MODBUS_UART();
    if (p_modbus_manager->pm_current_index > p_modbus_manager->number_of_pm - 1)     
    {
      p_modbus_manager->pm_current_index = 0;
    }
    vTaskDelay(500);
    switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_type) 
    {
      //vTaskDelay(500); // sua loi mat ket noi
    case 1:// FINECO
      {   
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)      
        {
        case PM_FINECO_INFO_1:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                               4500);//1,0x10,50
          }
          break;
        case PM_FINECO_INFO_2:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                               4500);//1,0x20,5
          }
          break;
        default:
          break;
        };
      }
      break;
    case 2:// ASCENT
      {   
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
        {
        case PM_ASCENT_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                               4500);//1,0x10,50
          }
          break;
        case PM_ASCENT_INFO_2:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                               4500);//1,0x20,5
          }
          break;
        case PM_ASCENT_INFO_3:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[2],
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[2],
                                    4500);//1,0x20,5
          }
          break;
        case PM_ASCENT_INFO_4:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[3],
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[3],
                                    4500);//1,0x20,5
          }
          break;
        case PM_ASCENT_INFO_5:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[4],
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[4],
                                    4500);//1,0x20,5
          }
          break;
        case PM_ASCENT_INFO_6:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[5],
                                    p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[5],
                                    4500);//1,0x20,5
          }
          break;
        default:
          break;
        };
      }
      break;
    case 3:// EASTRON
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
        {
        case PM_EASTRON_INFO_1:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                               2500);
          }
          break;
        case PM_EASTRON_INFO_2:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                               2500);
          }
          break;
        case PM_EASTRON_INFO_3:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[2],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[2],
                               2500);
          }
          break;
        case PM_EASTRON_INFO_4:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[3],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[3],
                               4500);
          }
          break;
        default:
          break;
        };
      }
      break;
    case 4:// CET1
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
        {
        case PM_CET1_INFO_1:
          {
             master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                                p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                                p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                                4500);//1,0x10,50
          }
          break;
        case PM_CET1_INFO_2:
          {
             master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                                p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                                p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                                4500);//1,0x20,5
          }
          break;
        case PM_CET1_INFO_3:
          {
             master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                                p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[2],
                                p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[2],
                                4500);//1,0x20,5
          }
          break;
        default:
          break;
        };
      }
      break;
    case 5://PILOT
      {   
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
        {
        case PM_PILOT_INFO_1:
          {
           master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                              p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                              p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                              4500);
          }                   
          break;
        case PM_PILOT_INFO_2:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                               4500);
          }
          break;
        default:
          break;
        }
      }
      break;
      //                    case 6://FORLONG
      //                    {   
      //                      UART_Deinit(RS4851R_UART);
      //                      Init_RS485_MODBUS_UART();
      //                        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      //                        {
      //                        case PM_FORLONG_INFO_1:
      //                        {
      //                            Read_Input_Regs_Query(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
      //                                                  p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
      //                                                  p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0]);
      //                        }                   
      //                        break;
      //                        default:
      //                        break;
      //                        }
      //                    }
      //                    break;
    case 15://PILOT_3PHASE
      {   
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
        {
        case PM_PILOT_3PHASE_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                               4500);
          }                   
          break;
        case PM_PILOT_3PHASE_INFO_2:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                               4500);
          }
          break;
        case PM_PILOT_3PHASE_INFO_3:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[2],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[2],
                               4500);
          }
          break;
        default:
          break;
        }
      }
      break;
    case 16://YADA_3PHASE_DPC
      {
        init_rs485_modbus_rtu(BAUD_RATE_2400,EVEN_PARITY);//BAUD_RATE_2400
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step){
        case PM_YADA_3PHASE_DPC_INFO_1:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0], 
                                       20000);
          break;
        case PM_YADA_3PHASE_DPC_INFO_2:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                                       20000);
          break;
        case PM_YADA_3PHASE_DPC_INFO_3:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[2],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[2],
                                       20000);
          break;
        case PM_YADA_3PHASE_DPC_INFO_4:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[3],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[3],
                                       20000);
          break;
        case PM_YADA_3PHASE_DPC_INFO_5:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[4],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[4],
                                       20000);
          break;
        case PM_YADA_3PHASE_DPC_INFO_6:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[5],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[5],
                                       20000);
          break;
        case PM_YADA_3PHASE_DPC_INFO_7:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[6],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[6],
                                       20000);
          break;
        case PM_YADA_3PHASE_DPC_INFO_8:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[7],
                                       p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[7],
                                       20000);
          break;
        default:
          break;
        }  
      }
      break;
      
    case 17: //Schneider 2022
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
        {
        case PM_SCHNEDER_3PHASE_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                               4500);
          }                   
          break;
        case PM_SCHNEDER_3PHASE_INFO_2:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                               4500);
          }
          break;
        case PM_SCHNEDER_3PHASE_INFO_3:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[2],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[2],
                               4500);
          }
          break;
          case PM_SCHNEDER_3PHASE_INFO_4:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[3],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[3],
                               4500);
          }                   
          break;
          case PM_SCHNEDER_3PHASE_INFO_5:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[4],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[4],
                               4500);
          }                   
          break;
          case PM_SCHNEDER_3PHASE_INFO_6:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[5],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[5],
                               4500);
          }                   
          break;
          case PM_SCHNEDER_3PHASE_INFO_7:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[6],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[6],
                               4500);
          }                   
          break;
          case PM_SCHNEDER_3PHASE_INFO_8:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[7],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[7],
                               4500);
          }                   
          break;
          case PM_SCHNEDER_3PHASE_INFO_9:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id, 
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[8],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[8],
                               4500);
          }                   
          break;
        
        default:
          break;
        }
        
      }
      break;
      
    case 18: ////EASTRON SMD72D 2022
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)      
        {
        case PM_EASTRON_SDM72D_INFO_1:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[0],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[0],
                               4500);
          }
          break;
        case PM_EASTRON_SDM72D_INFO_2:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[1],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[1],
                               4500);
          }
          break;
        case PM_EASTRON_SDM72D_INFO_3:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[2],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[2],
                               4500);
          }
          break;
          case PM_EASTRON_SDM72D_INFO_4:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[3],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[3],
                               4500);
          }
          break;
          case PM_EASTRON_SDM72D_INFO_5:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[4],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[4],
                               4500);
          }
          break;
          case PM_EASTRON_SDM72D_INFO_6:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_abs_slave_id,
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_base_addr[5],
                               p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_num_of_reg[5],
                               4500);
          }
          break;
        default:
          break;
        };
      }
      break;
   
    default:
      break;
    }
    return  _WAIT_PM_RESPOND;
  }
  else
  {
    return  _READ_VAC_STATUS;
  }
  
}
//wait pm respond----------------
RTU_STATE wait_pm_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  //reVal = RS4851_Check_Respond_Data_PM();
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_PM_SLAVE);//chua code
  if(p_modbus_manager->pm_current_index <= (p_modbus_manager->number_of_pm - 1))        //
  {
    if(check_val != 1)
    {
      
      p_modbus_manager->pm_info_struct[p_modbus_manager->pm_current_index].u8NoResponseCnt++;
      if (p_modbus_manager->pm_info_struct[p_modbus_manager->pm_current_index].u8NoResponseCnt >= 5)
      {
        p_modbus_manager->pm_info_struct[p_modbus_manager->pm_current_index].u8ErrorFlag = 1;
        p_modbus_manager->cnt_dis_connect_pm[p_modbus_manager->pm_current_index]++;
        switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_type)
        {
        case 1://FINECO
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_FINECO_INFO_1;
          }
          break;
        case 2://ASCENT
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_ASCENT_INFO_1;
          }
          break;
        case 3://EASTRON
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_EASTRON_INFO_1;
          }
          break;
        case 4://CET1
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_CET1_INFO_1;
          }
          break;
        case 5://PILOT
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_PILOT_INFO_1;
          }
          break;
          //                            case 6://FORLONG
          //                            {
          //                              p_modbus_manager->bm_manager_struct[p_modbus_manager->pm_current_index].bm_running_step = PM_FORLONG_INFO_1;
          //                            }
          //                            break;
        case 15://PILOT_3PHASE
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_PILOT_3PHASE_INFO_1;
          }
          break;
        case 16: //YADA_3PHASE_DPC
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_YADA_3PHASE_DPC_INFO_1;
          }
          break;
        case 17: //Schneider 2022
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_SCHNEDER_3PHASE_INFO_1;
          }
          break; 
          
        case 18: //EASTRON SMD72D 2022
          {
            p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step = PM_EASTRON_SDM72D_INFO_1;
          }
          break;
        };  
        p_modbus_manager->pm_current_index++;
      }
    }
    else
    {
      p_modbus_manager->pm_info_struct[p_modbus_manager->pm_current_index].u8NoResponseCnt = 0;
      p_modbus_manager->pm_info_struct[p_modbus_manager->pm_current_index].u8ErrorFlag = 0; 
      switch(p_modbus_manager->pm_manager_struct[p_modbus_manager->pm_current_index].pm_running_step)
      {
      case PM_FINECO_INFO_1:
      case PM_ASCENT_INFO_1:
      case PM_EASTRON_INFO_1:
      case PM_CET1_INFO_1:
      case PM_PILOT_INFO_1:
        //                        case PM_FORLONG_INFO_1:
      case PM_YADA_3PHASE_DPC_INFO_1:
      case PM_PILOT_3PHASE_INFO_1:
      case PM_SCHNEDER_3PHASE_INFO_1:
      case PM_EASTRON_SDM72D_INFO_1:
        {
          p_modbus_manager->pm_current_index++;
        }break;  
      default:
        break;
      };                        
    }
    
    if (p_modbus_manager->pm_current_index > p_modbus_manager->number_of_pm - 1)
    {
      p_modbus_manager->u8PMBootPassFlag = 1;
      return _READ_VAC_STATUS;
      
    }
    else
    {
      return  _READ_PM_STATUS;   
    }
  }
  else  
  {
    return _READ_PM_STATUS;
  }
  
}

//read vac status----------------
RTU_STATE read_vac_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->modbus_state = START;
  p_modbus->data_pointer = 0;
  
  if(p_modbus_manager->number_of_vac != 0){
    if(p_modbus_manager->vac_current_index > p_modbus_manager->number_of_vac - 1){
      p_modbus_manager->vac_current_index = 0;
    }
    switch(p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_type)
    {
    case 1:// VIETTEL------------------------------------------------------------------------
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_running_step)
        {
        case VAC_VIETTEL_INFO_1:
          master_read_modbus(p_modbus,0x04, p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_abs_slave_id,
                                            p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_base_addr[0],
                                            p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_num_of_reg[0],
                                            2500);
          break;
        case VAC_VIETTEL_INFO_2:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_abs_slave_id,
                                            p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_base_addr[1],
                                            p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_num_of_reg[1],
                                            4500);          
          break; 
        case VAC_VIETTEL_INFO_3:
          master_read_modbus(p_modbus,0x03, p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_abs_slave_id,
                                            p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_base_addr[2],
                                            p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_num_of_reg[2],
                                            4500);
          break;
        default:
          break;
        }
      }
      break;
    default:
      break;
    }
    return _WAIT_VAC_RESPOND;
  }
  else{
    return _READ_SMCB_STATUS;
  } 
}

//wait vac respond--------------
RTU_STATE wait_vac_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_VAC_SLAVE);
  if(p_modbus_manager->vac_current_index <= (p_modbus_manager->number_of_vac -1)){
    if(check_val != 1){
      p_modbus_manager->vac_info_struct[p_modbus_manager->vac_current_index].no_response_cnt++;
      if(p_modbus_manager->vac_info_struct[p_modbus_manager->vac_current_index].no_response_cnt>=5){
        p_modbus_manager->vac_info_struct[p_modbus_manager->vac_current_index].error_flag = 1;
        p_modbus_manager->cnt_dis_connect_vac[p_modbus_manager->vac_current_index]++;
        
        switch(p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_type)
        {
        case 1:
          {
            p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_running_step = VAC_VIETTEL_INFO_1;
          }
          break;
        }
        p_modbus_manager->vac_current_index++; 
      } 
    }
    else{
      p_modbus_manager->vac_info_struct[p_modbus_manager->vac_current_index].no_response_cnt = 0;
      p_modbus_manager->vac_info_struct[p_modbus_manager->vac_current_index].error_flag      = 0;
      switch (p_modbus_manager->vac_manager_struct[p_modbus_manager->vac_current_index].vac_running_step)
      {
      case VAC_VIETTEL_INFO_1:
        p_modbus_manager->vac_current_index++;
        break;
      default:
        break;
      }
    }
    if(p_modbus_manager->vac_current_index>(p_modbus_manager->number_of_vac -1)){
      return _READ_SMCB_STATUS;
    }
    else{
      return _READ_VAC_STATUS;
    }
    
  }else{
    return _READ_VAC_STATUS;
  }
}

//read smbc status--------------
RTU_STATE read_smbc_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->data_pointer = 0;
  if(p_modbus_manager->number_of_smcb != 0)
  {
    if (p_modbus_manager->smcb_current_index > p_modbus_manager->number_of_smcb - 1)
    {
      p_modbus_manager->smcb_current_index = 0;
    }
    switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_type)
    {
    case 1:// OPEN
      {
        vTaskDelay(1000);
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step)
        {
        case SMCB_OPEN_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_abs_slave_id,
                               p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_base_addr[0],
                               p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_num_of_reg[0],
                               4500);//35,0x00,20
          }
          break;
        default:
          break;
        };
      }
      break;
    case 2:// MATIS
      {
        vTaskDelay(1000);
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step)
        {
        case SMCB_MATIS_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_abs_slave_id,
                                    p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_base_addr[0],
                                    p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_num_of_reg[0],
                                    4500);//35,0x00,20
          }
          break;
        default:
          break;
        };
      }
      break;
      
    case 3:// GOL
      {
        vTaskDelay(1000);
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step)
        {
        case SMCB_GOL_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_abs_slave_id,
                                    p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_base_addr[0],
                                    p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_num_of_reg[0],
                                    4500);//35,0x00,20
          }
          break;
        default:
          break;
        };
      }
      break;
    default:
      break;
    }
    return _WAIT_SMCB_RESPOND;
  }
  else
  {
    return _READ_FUEL_STATUS;
  }
  
}

//wait vac respond--------------
RTU_STATE wait_smcb_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  //reVal = RS4851_Check_Respond_Data_SMCB();
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_SMBC_SLAVE);
  if(p_modbus_manager->smcb_current_index <= (p_modbus_manager->number_of_smcb - 1))
  {
    if(check_val != 1)
    {
      p_modbus_manager->smcb_info_struct[p_modbus_manager->smcb_current_index].u8NoResponseCnt++;
      if (p_modbus_manager->smcb_info_struct[p_modbus_manager->smcb_current_index].u8NoResponseCnt >= 5)
      {
        p_modbus_manager->smcb_info_struct[p_modbus_manager->smcb_current_index].u8ErrorFlag = 1;
        p_modbus_manager->cnt_dis_connect_smbc[p_modbus_manager->smcb_current_index]++;
        switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_type)
        {
        case 1: //open
          {
            p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step = SMCB_OPEN_INFO_1;
          }
          break;
        case 2: //matis
          {
            p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step = SMCB_MATIS_INFO_1;
          }
          break;
        case 3: //gol
          {
            p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step = SMCB_GOL_INFO_1;
          }
          break;
        };                        
        p_modbus_manager->smcb_current_index++;
      }
    }
    else
    {
      p_modbus_manager->smcb_info_struct[p_modbus_manager->smcb_current_index].u8NoResponseCnt = 0;
      p_modbus_manager->smcb_info_struct[p_modbus_manager->smcb_current_index].u8ErrorFlag = 0;
      switch(p_modbus_manager->smcb_manager_struct[p_modbus_manager->smcb_current_index].smcb_running_step)
      {
      case SMCB_OPEN_INFO_1:
      case SMCB_MATIS_INFO_1:
      case SMCB_GOL_INFO_1:
        {
          p_modbus_manager->smcb_current_index++;
        }break;                      
      };
    }
    
    if (p_modbus_manager->smcb_current_index > p_modbus_manager->number_of_smcb - 1)
    {
      return _READ_FUEL_STATUS;
    }
    else
    {
      return _READ_SMCB_STATUS;
    }
  }
  else return _READ_SMCB_STATUS;
  
}

//read fuel status--------------
RTU_STATE read_fuel_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  p_modbus->data_pointer = 0;
  if(p_modbus_manager->number_of_fuel != 0)
  {
    if (p_modbus_manager->fuel_current_index > p_modbus_manager->number_of_fuel - 1)
    {
      p_modbus_manager->fuel_current_index = 0;
    }
    switch(p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_type)
    {
    case 1:// HPT621
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_running_step)
        {
        case FUEL_HPT621_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_abs_slave_id,
                                    p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_base_addr[0],
                                    p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_num_of_reg[0],
                                    4500);
          }
          break;
        default:
          break;
        };
      }
      break;
    default:
      break;
    }
    return _WAIT_FUEL_RESPOND;
  }
  else
  {
    return _READ_ISENSE_STATUS;
  }
}

//wait fuel respond-------------
RTU_STATE wait_fuel_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_FUEL_SLAVE);
  //reVal = RS4851_Check_Respond_Data_FUEL();
  if(p_modbus_manager->fuel_current_index <= (p_modbus_manager->number_of_fuel - 1))
  {
    if(check_val != 1)
    {
      p_modbus_manager->fuel_info_struct[p_modbus_manager->fuel_current_index].u8NoResponseCnt++;
      if (p_modbus_manager->fuel_info_struct[p_modbus_manager->fuel_current_index].u8NoResponseCnt >= 5)
      {
        p_modbus_manager->fuel_info_struct[p_modbus_manager->fuel_current_index].u8ErrorFlag = 1;
        p_modbus_manager->cnt_dis_connect_fuel[p_modbus_manager->fuel_current_index]++;
        switch(p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_type)
        {
        case 1:
          {
            p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_running_step = FUEL_HPT621_INFO_1;
          }
          break;
        };                        
        p_modbus_manager->fuel_current_index++;
      }
    }
    else
    {
      p_modbus_manager->fuel_info_struct[p_modbus_manager->fuel_current_index].u8NoResponseCnt = 0;
      p_modbus_manager->fuel_info_struct[p_modbus_manager->fuel_current_index].u8ErrorFlag = 0;
      switch(p_modbus_manager->fuel_manager_struct[p_modbus_manager->fuel_current_index].fuel_running_step)
      {
      case FUEL_HPT621_INFO_1:
        {
          p_modbus_manager->fuel_current_index++;
        }break;                      
      };
    }
    
    if (p_modbus_manager->fuel_current_index > p_modbus_manager->number_of_fuel - 1)
    {
      return _READ_ISENSE_STATUS;
    }
    else
    {
      return _READ_FUEL_STATUS;
    }
  }
  else{
    
    return _READ_FUEL_STATUS; 
  }
  
}

//read isense status------------
RTU_STATE read_isense_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  p_modbus->data_pointer = 0;
  if(p_modbus_manager->number_of_isense != 0)           
  {
    if (p_modbus_manager->isense_current_index > p_modbus_manager->number_of_isense - 1)     
    {
      p_modbus_manager->isense_current_index = 0;
    }
    vTaskDelay(500);
    switch(p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_type) 
    {
      //vTaskDelay(500); 
    case 1://FORLONG
      {   
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_running_step)
        {
        case ISENSE_FORLONG_INFO_1:
          {
            master_read_modbus(p_modbus,0x04,p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_abs_slave_id, 
                                             p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_base_addr[0],
                                             p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_num_of_reg[0],
                                             2500);
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
    return _WAIT_ISENSE_RESPOND;
  }
  else
  {
    return _READ_PM_DC_STATUS;
  }
}

//wait isense respond-----------
RTU_STATE wait_isense_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_ISENSE_SLAVE);
  //reVal = RS4851_Check_Respond_Data_ISENSE();
  if(p_modbus_manager->isense_current_index <= (p_modbus_manager->number_of_isense - 1))        //
  {
    if(check_val != 1)
    {
      
      p_modbus_manager->isense_info_struct[p_modbus_manager->isense_current_index].u8NoResponseCnt++;
      if (p_modbus_manager->isense_info_struct[p_modbus_manager->isense_current_index].u8NoResponseCnt >= 5)
      {
        p_modbus_manager->isense_info_struct[p_modbus_manager->isense_current_index].u8ErrorFlag = 1;
        p_modbus_manager->cnt_dis_connect_isense[p_modbus_manager->isense_current_index]++;
        switch(p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_type)
        {
        case 1://FORLONG
          {
            p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_running_step = ISENSE_FORLONG_INFO_1;
          }
          break;
        }
        p_modbus_manager->isense_current_index++;
      }
    }
    else
    {
      p_modbus_manager->isense_info_struct[p_modbus_manager->isense_current_index].u8NoResponseCnt = 0;
      p_modbus_manager->isense_info_struct[p_modbus_manager->isense_current_index].u8ErrorFlag = 0; 
      switch(p_modbus_manager->isense_manager_struct[p_modbus_manager->isense_current_index].isense_running_step)
      {
      case ISENSE_FORLONG_INFO_1:
        {
          p_modbus_manager->isense_current_index++;
        }break;                      
      };                        
    }
    
    if (p_modbus_manager->isense_current_index > p_modbus_manager->number_of_isense - 1)
    {
      return _READ_PM_DC_STATUS;
    }
    else
    {
      return _READ_ISENSE_STATUS;   
    }
  }
  else 
  {
    return _READ_ISENSE_STATUS;
  }
}

//read pmdc status--------------
RTU_STATE read_pmdc_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  p_modbus->data_pointer = 0;
  if(p_modbus_manager->number_of_pmdc != 0){
    if(p_modbus_manager->pmdc_current_index > p_modbus_manager->number_of_pmdc - 1){
      p_modbus_manager->pmdc_current_index = 0;
    }
    //vTaskDelay(500);
    switch(p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_type){
    case 1://YADA_DC
      {
        init_rs485_modbus_rtu(BAUD_RATE_2400,EVEN_PARITY);//BAUD_RATE_2400
        switch(p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step){
        case PM_DC_YADA_INFO_1:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_abs_slave_id, 
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_base_addr[0],
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_num_of_reg[0],
                                       20000);
          break;
        case PM_DC_YADA_INFO_2:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_abs_slave_id, 
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_base_addr[1],
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_num_of_reg[1],
                                       20000);
          break;
        case PM_DC_YADA_INFO_3:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_abs_slave_id, 
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_base_addr[2],
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_num_of_reg[2],
                                       20000);
          break;
        case PM_DC_YADA_INFO_4:
          master_read_modbus(p_modbus,0x03,p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_abs_slave_id, 
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_base_addr[3],
                                       p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_num_of_reg[3],
                                       20000);
          break;
        default:
          break;
        }
      }
      break;
    default:
      break;
    }
    return _WAIT_PM_DC_RESPOND;
  }else{
    return _READ_BM_STATUS;
  }
  
}

//wait pmdc respond-------------
RTU_STATE wait_pmdc_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_PMDC_SLAVE);
  //reVal = RS4851_Check_Respond_Data_PM_DC();
  if(p_modbus_manager->pmdc_current_index <= (p_modbus_manager->number_of_pmdc - 1)){
    if(check_val != 1){
      p_modbus_manager->pmdc_info_struct[p_modbus_manager->pmdc_current_index].u8_no_response_cnt++;
      if (p_modbus_manager->pmdc_info_struct[p_modbus_manager->pmdc_current_index].u8_no_response_cnt >= 5)
      {
        p_modbus_manager->pmdc_info_struct[p_modbus_manager->pmdc_current_index].u8_error_flag = 1;
        p_modbus_manager->cnt_dis_connect_pmdc[p_modbus_manager->pmdc_current_index]++;
        switch(p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_type)
        {
        case 1://YADA_DC
          {
            p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step = PM_DC_YADA_INFO_1;
          }
          break;
        }
        p_modbus_manager->pmdc_current_index++;
      }
      
    }
    else{
      p_modbus_manager->pmdc_info_struct[p_modbus_manager->pmdc_current_index].u8_no_response_cnt = 0;
      p_modbus_manager->pmdc_info_struct[p_modbus_manager->pmdc_current_index].u8_error_flag = 0; 
      switch(p_modbus_manager->pmdc_manager_struct[p_modbus_manager->pmdc_current_index].pmdc_running_step)
      {
      case PM_DC_YADA_INFO_1:
        {
          p_modbus_manager->pmdc_current_index++;
        }break;                      
      };
    }
    
    if (p_modbus_manager->pmdc_current_index > p_modbus_manager->number_of_pmdc - 1)
    {
      return _READ_BM_STATUS;
    }
    else
    {
      return _READ_PM_DC_STATUS;   
    } 
  }
  else{
    return _READ_PM_DC_STATUS;
  }
}

//read bm status---------------
RTU_STATE read_bm_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->data_pointer = 0;
  if(p_modbus_manager->number_of_bm != 0)
  {
    if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
    {
      p_modbus_manager->bm_current_index = 0;
    }
    switch(p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_type)
    {
    case 1:// VIETTEL
      {
        init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
        switch(p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_running_step)
        {
        case BM_VIETTEL_INFO_1:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_abs_slave_id,
                                    p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_base_addr[0],
                                    p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_num_of_reg[0],
                                    4500);//1,0x00,28
          }
          break;
        case BM_VIETTEL_INFO_2:
          {
            master_read_modbus(p_modbus,0x03,p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_abs_slave_id,
                                    p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_base_addr[1],
                                    p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_num_of_reg[1],
                                    4500);//1,0x20,5
          }
          break;
        default:
          break;
        };
      }
      break;
    default:
      break;
    }
    return _WAIT_BM_RESPOND;
  }
  else
  {
    return _READ_FAN_DPC_STATUS;
  }
}

//wait bm respond--------------
RTU_STATE wait_bm_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  int8_t check_val = mater_check_respond_data(p_modbus,p_modbus_manager,_BM_SLAVE);
  //reVal = RS4851_Check_Respond_Data_BM();
  if(p_modbus_manager->bm_current_index <= (p_modbus_manager->number_of_bm - 1))
  {
    if(check_val != 1)
    {
      p_modbus_manager->bm_info_struct[p_modbus_manager->bm_current_index].u8NoResponseCnt++;
      if (p_modbus_manager->bm_info_struct[p_modbus_manager->bm_current_index].u8NoResponseCnt >= 5)
      {
        p_modbus_manager->bm_info_struct[p_modbus_manager->bm_current_index].u8ErrorFlag = 1;
        switch(p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_type)
        {
        case 1:
          {
            p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_running_step = BM_VIETTEL_INFO_1;
          }
          break;
        }; 
        p_modbus_manager->bm_current_index++;
      }
    }
    else
    {
      p_modbus_manager->bm_info_struct[p_modbus_manager->bm_current_index].u8NoResponseCnt = 0;
      p_modbus_manager->bm_info_struct[p_modbus_manager->bm_current_index].u8ErrorFlag = 0;
      switch(p_modbus_manager->bm_manager_struct[p_modbus_manager->bm_current_index].bm_running_step)
      {
      case BM_VIETTEL_INFO_1:
        {
          p_modbus_manager->bm_current_index++;
        }break;                      
      };
    }
    
    if (p_modbus_manager->bm_current_index > p_modbus_manager->number_of_bm - 1)
    {
      return _READ_FAN_DPC_STATUS;
    }
    else
    {
      return _READ_BM_STATUS;
    }
  }else {
    return _READ_BM_STATUS;
  }
}

//read fan dpc status----------
RTU_STATE read_fan_dpc_status(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->data_pointer = 0;
  if(TYPE == DKD51_BDP){
    init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
    //clearn-------------------------------------------------- 
    DKD51_clean_buff_fan(&fan_dpc_switch_uart_t);
    //clearn--------------------------------------------------
    switch(fan_dpc_switch_uart_t.fan_state_e){
    case _DKD51_FAN_SYS_REQ:
      fan_dpc_switch_uart_t.rx_enable = STATE_ON;
      //code here
      DKD51_send_message_resquest_fan((uint8_t*)&DKD51_FAN_REQ_sys_analog_data[0],(uint8_t)DKD51_FAN_REQ_sys_analog_data_len);
      return _WAIT_FAN_DPC_RESPOND;
      break;
    case _DKD51_FAN_STATE_REQ:
      fan_dpc_switch_uart_t.rx_enable = STATE_ON;
      //code here
      DKD51_send_message_resquest_fan((uint8_t*)&DKD51_FAN_REQ_switch_input_state[0],(uint8_t)DKD51_FAN_REQ_switch_input_state_len);
      return _WAIT_FAN_DPC_RESPOND;
      break;
    case _DKD51_FAN_WARNING_REQ:
      fan_dpc_switch_uart_t.rx_enable = STATE_ON;
      //code here
      DKD51_send_message_resquest_fan((uint8_t*)&DKD51_FAN_REQ_warning_status[0],(uint8_t)DKD51_FAN_REQ_warning_status_len);
      return _WAIT_FAN_DPC_RESPOND;
      break;
    case _DKD51_FAN_PARAMETER_REQ:
      fan_dpc_switch_uart_t.rx_enable = STATE_ON;
      //code here
      DKD51_send_message_resquest_fan((uint8_t*)&DKD51_FAN_REQ_read_parameter[0],(uint8_t)DKD51_FAN_REQ_read_parameter_len);
      return _WAIT_FAN_DPC_RESPOND;
      break;
    case _DKD51_FAN_EQUIPMENT_REQ:
      fan_dpc_switch_uart_t.rx_enable = STATE_ON;
      //code here
      return _WAIT_FAN_DPC_RESPOND;
      break;
    default:
      fan_dpc_switch_uart_t.rx_enable  = STATE_OFF;
      return _WAIT_MODE;
      break;
    }
    
  }else{
    private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect = 0;
    return _WAIT_MODE;
  }
  
}

//wait fan dpc respond------------------------------------------------------------------------------
RTU_STATE wait_fan_dpc_respond(MODBUS_RTU_STRUCT *p_modbus,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  p_modbus->data_pointer = 0;
  if(TYPE == DKD51_BDP){
    switch(fan_dpc_switch_uart_t.fan_state_e){
    case _DKD51_FAN_SYS_REQ:
      //code here------------------------------------------------------------------------
      if(DKD51_check_sum_fan(&fan_dpc_switch_uart_t) == NO_ERR){
        fan_info_dpc_t.no_response =0; 
        private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect = 0;
        if(DKD51_check_RTN_fan(&fan_dpc_switch_uart_t) == NO_ERR){
          fan_info_dpc_t.env_temp          = hex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],15);  // realx100
          fan_info_dpc_t.airflow_temp      = hex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],19);  // realx100
          fan_info_dpc_t.voltage           = hex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],23);  // realx100
          fan_info_dpc_t.working_current   = hex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],27);  // realx100
        } 
      }
      else{
        fan_dpc_switch_uart_t.check_sum_calculator = 0;
        fan_dpc_switch_uart_t.check_sum_read       = 0xFFFF;
        fan_info_dpc_t.no_response++;
        if(fan_info_dpc_t.no_response>3){
          fan_info_dpc_t.no_response = 10;
          private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect =1;
          //clean-----------------------------------------------
          fan_info_dpc_t.env_temp                    = 0;
          fan_info_dpc_t.airflow_temp                = 0;
          fan_info_dpc_t.voltage                     = 0;
          fan_info_dpc_t.working_current             = 0;
          
          fan_info_dpc_t.state_device                = 0;
          fan_info_dpc_t.state_fan                   = 0;
          
          fan_info_dpc_t.alarm_fail_refrigeration    = 0;
          fan_info_dpc_t.alarm_high_temp             = 0;
          fan_info_dpc_t.alarm_low_temp              = 0;
          fan_info_dpc_t.alarm_fail_sensor_temp      = 0;
          fan_info_dpc_t.alarm_high_voltage          = 0;
          fan_info_dpc_t.alarm_low_voltage           = 0;
          
          fan_info_dpc_t.para_dc_starting_point      = 0;
          fan_info_dpc_t.para_dc_sensivive_point     = 0;
          fan_info_dpc_t.para_dc_heating_start_point = 0;
          fan_info_dpc_t.para_dc_heating_sensivive   = 0;
          fan_info_dpc_t.para_alarm_high_temp        = 0;
          fan_info_dpc_t.para_alarm_low_temp         = 0;
          //------------------------------------------------------
        }
      }
      fan_dpc_switch_uart_t.uart_rx.cnt =0;
      //code here-------------------------------------------------------------------------
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;
      fan_dpc_switch_uart_t.fan_state_e = _DKD51_FAN_STATE_REQ;
      return _READ_FAN_DPC_STATUS;
      break;
    case _DKD51_FAN_STATE_REQ:
      //code here-------------------------------------------------------------------------
      if(DKD51_check_sum_fan(&fan_dpc_switch_uart_t) == NO_ERR){
        if(DKD51_check_RTN_fan(&fan_dpc_switch_uart_t) == NO_ERR){
          fan_info_dpc_t.state_device = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],15);  // 0x00H:Shut down; 0x01H: boot
          fan_info_dpc_t.state_fan    = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],17);  // 0x00H:Shut down; 0x01H: boot
        } 
      }
      else{
        fan_dpc_switch_uart_t.check_sum_calculator = 0;
        fan_dpc_switch_uart_t.check_sum_read       = 0xFFFF;
      }
      fan_dpc_switch_uart_t.uart_rx.cnt =0;                
      //code here-------------------------------------------------------------------------
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;        
      fan_dpc_switch_uart_t.fan_state_e = _DKD51_FAN_WARNING_REQ;
      return _READ_FAN_DPC_STATUS;
      break;
    case _DKD51_FAN_WARNING_REQ:
      //code here------------------------------------------------------------------------
      if(DKD51_check_sum_fan(&fan_dpc_switch_uart_t) == NO_ERR){
        if(DKD51_check_RTN_fan(&fan_dpc_switch_uart_t) == NO_ERR){
          fan_info_dpc_t.alarm_fail_refrigeration   = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],15);  //0x00H: normal; 0xF0H:Falty
          fan_info_dpc_t.alarm_high_temp            = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],17);  //0x00H: normal; 0xF0H:Falty
          fan_info_dpc_t.alarm_low_temp             = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],19);  //0x00H: normal; 0xF0H:Falty
          fan_info_dpc_t.alarm_fail_sensor_temp     = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],23);  //0x00H: normal; 0xF0H:Falty
          fan_info_dpc_t.alarm_high_voltage         = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],25);  //0x00H: normal; 0xF0H:Falty
          fan_info_dpc_t.alarm_low_voltage          = hex2byte(&fan_dpc_switch_uart_t.rx.buff_485[0],27);  //0x00H: normal; 0xF0H:Falty
        } 
      }
      else{
        fan_dpc_switch_uart_t.check_sum_calculator = 0;
        fan_dpc_switch_uart_t.check_sum_read       = 0xFFFF;
      }
      fan_dpc_switch_uart_t.uart_rx.cnt =0;
      //code here-------------------------------------------------------------------------
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF; 
      fan_dpc_switch_uart_t.fan_state_e = _DKD51_FAN_PARAMETER_REQ;
      return _READ_FAN_DPC_STATUS;
      break;
    case _DKD51_FAN_PARAMETER_REQ:
      //code here-------------------------------------------------------------------------
      if(DKD51_check_sum_fan(&fan_dpc_switch_uart_t) == NO_ERR){
        if(DKD51_check_RTN_fan(&fan_dpc_switch_uart_t) == NO_ERR){
          fan_info_dpc_t.para_dc_starting_point    = hex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],13);  // realx100
          fan_info_dpc_t.para_dc_sensivive_point   = hex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],17);  // realx100
          fan_info_dpc_t.para_dc_heating_start_point = ihex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],21);// realx100
          fan_info_dpc_t.para_dc_heating_sensivive   = iihex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],25);// realx100
          fan_info_dpc_t.para_alarm_high_temp      = hex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],29);  // realx100
          fan_info_dpc_t.para_alarm_low_temp       = ihex2byte2(&fan_dpc_switch_uart_t.rx.buff_485[0],33);  // realx100
        } 
      }
      else{
        fan_dpc_switch_uart_t.check_sum_calculator = 0;
        fan_dpc_switch_uart_t.check_sum_read       = 0xFFFF;
      }
      fan_dpc_switch_uart_t.uart_rx.cnt =0;
      //code here-------------------------------------------------------------------------
      fan_dpc_switch_uart_t.rx_enable   = STATE_OFF;    
      fan_dpc_switch_uart_t.fan_state_e = _DKD51_FAN_EQUIPMENT_REQ;
      return _READ_FAN_DPC_STATUS;
      break;
    case _DKD51_FAN_EQUIPMENT_REQ:
      //code here
      fan_dpc_switch_uart_t.rx_enable  = STATE_OFF;
      return _WAIT_MODE;
      break;
    default:
      fan_dpc_switch_uart_t.rx_enable  = STATE_OFF;
      return _WAIT_MODE;
      break;
    }
    
  }else{
    private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect = 0;
    return _WAIT_MODE;
  }
  
}

//update info-------------------
void update_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  //slave vac update info------------------------------------------------------
  update_vac_info(p_mib,p_modbus_manager);
  //slave lib update info------------------------------------------------------
  update_lib_info(p_mib,p_modbus_manager);
  //slave gen update info------------------------------------------------------
  update_gen_info(p_mib,p_modbus_manager);
  //slave pm update info-------------------------------------------------------
  update_pm_info(p_mib,p_modbus_manager);
  //slave smbc update info-----------------------------------------------------
  update_smbc_info(p_mib,p_modbus_manager);
  //slave fuel update info-----------------------------------------------------
  update_fuel_info(p_mib,p_modbus_manager);
  //slave isense update info---------------------------------------------------
  update_isense_info(p_mib,p_modbus_manager);
  //slave pmdc update info-----------------------------------------------------
  update_pmdc_info(p_mib,p_modbus_manager);
  //slave bm update info-------------------------------------------------------
  update_bm_info(p_mib,p_modbus_manager);
  //fan dpc update info--------------------------------------------------------
  update_fan_dpc_info(p_mib);
  
}

//update vac info--------------
void update_vac_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint32_t i=0;
  
  p_mib->vac_group_struct.vac_installed_vac                = p_modbus_manager->number_of_vac;
  p_mib->connect_alarm_group_struct.alarm_vac_connect      = 0;
  p_mib->main_alarm_group_struct.alarm_vac                 = 0;
  p_mib->vac_group_struct.vac_active_vac                   = p_mib->vac_group_struct.vac_installed_vac;
  
  for(i = 0; i< p_mib->vac_group_struct.vac_installed_vac; i++){
    p_mib->main_alarm_group_struct.alarm_vac_pack[i] = 0;
    if(p_modbus_manager->vac_info_struct[i].error_flag == 1){
      
      if(p_modbus_manager->cnt_dis_connect_vac[i]>2){
        p_modbus_manager->cnt_dis_connect_vac[i] = 3;
        p_mib->vac_group_struct.vac_active_vac--;
        
        if(p_mib->vac_group_struct.vac_active_vac <= 0){
          p_mib->vac_group_struct.vac_active_vac =0;
        }
        
        p_mib->connect_alarm_group_struct.alarm_vac_connect |= (p_modbus_manager->vac_info_struct[i].error_flag<<i);
        p_mib->vac_group_struct.vac_table_struct[i].vac_status =  DISCONNECTED;
      }
      
    }
    else
    {
      p_modbus_manager->cnt_dis_connect_vac[i] = 0;
      
      if(p_mib->vac_group_struct.vac_table_struct[i].vac_alarm_status.all !=0){
        p_mib->vac_group_struct.vac_table_struct[i].vac_status =  WARNING;
      }
      else{
        p_mib->vac_group_struct.vac_table_struct[i].vac_status =  CONNECTED;
      }
      
    }
    
    if(p_mib->vac_group_struct.vac_table_struct[i].vac_status != DISCONNECTED)
    {
      if(p_mib->vac_group_struct.vac_table_struct[i].vac_status == WARNING){
        p_mib->main_alarm_group_struct.alarm_vac_pack[i] = p_mib->vac_group_struct.vac_table_struct[i].vac_alarm_status.all;
      }
      
      switch(p_modbus_manager->vac_manager_struct[i].vac_type)
      {
      case 1://VETTEL------------------------------------------------------------------
        {
          p_mib->vac_group_struct.vac_table_struct[i].vac_id                    = i+1;
          if(p_modbus_manager->vac_info_struct[i].vac_reset_ok == 0){
            p_mib->vac_group_struct.vac_table_struct[i].vac_alarm_status.all    = (uint32_t) (p_modbus_manager->vac_info_struct[i].alarm_status);
          }
          p_mib->vac_group_struct.vac_table_struct[i].vac_in_temp               = (int32_t)  (p_modbus_manager->vac_info_struct[i].in_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_ex_temp               = (int32_t)  (p_modbus_manager->vac_info_struct[i].ex_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_frost1_temp           = (int32_t)  (p_modbus_manager->vac_info_struct[i].frost1_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_frost2_temp           = (int32_t)  (p_modbus_manager->vac_info_struct[i].frost2_temp);
          
          p_mib->vac_group_struct.vac_table_struct[i].vac_humid                 = (uint32_t) (p_modbus_manager->vac_info_struct[i].humid);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_duty             = (uint32_t) (p_modbus_manager->vac_info_struct[i].fan1_duty);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_rpm              = (uint32_t) (p_modbus_manager->vac_info_struct[i].fan1_rpm);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan2_duty             = (uint32_t) (p_modbus_manager->vac_info_struct[i].fan2_duty);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan2_rpm              = (uint32_t) (p_modbus_manager->vac_info_struct[i].fan2_rpm);
          
          p_mib->vac_group_struct.vac_table_struct[i].vac_sys_mode              = (uint32_t) (p_modbus_manager->vac_info_struct[i].sys_mode);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan_speed_mode        = (uint32_t) (p_modbus_manager->vac_info_struct[i].fan_speed_mode);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan_start_temp        = (int32_t)  (p_modbus_manager->vac_info_struct[i].fan_start_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_set_temp              = (int32_t)  (p_modbus_manager->vac_info_struct[i].set_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_pid_offset_temp       = (int32_t)  (p_modbus_manager->vac_info_struct[i].pid_offset_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan_min_speed         = (uint32_t) (p_modbus_manager->vac_info_struct[i].fan_min_speed);
          p_mib->vac_group_struct.vac_table_struct[i].vac_fan_max_speed         = (uint32_t) (p_modbus_manager->vac_info_struct[i].fan_max_speed);
          p_mib->vac_group_struct.vac_table_struct[i].vac_filter_stuck_temp     = (int32_t)  (p_modbus_manager->vac_info_struct[i].filter_stuck_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_en         = (uint32_t) (p_modbus_manager->vac_info_struct[i].night_mode_en);
          p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_start      = (uint32_t) (p_modbus_manager->vac_info_struct[i].night_mode_start);
          p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_end        = (uint32_t) (p_modbus_manager->vac_info_struct[i].night_mode_end);
          p_mib->vac_group_struct.vac_table_struct[i].vac_night_max_speed       = (uint32_t) (p_modbus_manager->vac_info_struct[i].night_max_speed);
          p_mib->vac_group_struct.vac_table_struct[i].vac_manual_mode           = (uint32_t) (p_modbus_manager->vac_info_struct[i].manual_mode);
          p_mib->vac_group_struct.vac_table_struct[i].vac_manual_max_speed      = (uint32_t) (p_modbus_manager->vac_info_struct[i].manual_max_speed);
          p_mib->vac_group_struct.vac_table_struct[i].vac_in_max_temp           = (int32_t)  (p_modbus_manager->vac_info_struct[i].in_max_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_ex_max_temp           = (int32_t)  (p_modbus_manager->vac_info_struct[i].ex_max_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_frost_max_temp        = (int32_t)  (p_modbus_manager->vac_info_struct[i].frost_max_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_in_min_temp           = (int32_t)  (p_modbus_manager->vac_info_struct[i].in_min_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_ex_min_temp           = (int32_t)  (p_modbus_manager->vac_info_struct[i].ex_min_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_frost_min_temp        = (int32_t)  (p_modbus_manager->vac_info_struct[i].frost_min_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_min_out_temp          = (int32_t)  (p_modbus_manager->vac_info_struct[i].min_out_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_delta_temp            = (int32_t)  (p_modbus_manager->vac_info_struct[i].delta_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_panic_temp            = (int32_t)  (p_modbus_manager->vac_info_struct[i].panic_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_acu1_on_temp          = (int32_t)  (p_modbus_manager->vac_info_struct[i].acu1_on_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_acu2_on_temp          = (int32_t)  (p_modbus_manager->vac_info_struct[i].acu2_on_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_acu2_en               = (uint32_t) (p_modbus_manager->vac_info_struct[i].acu2_en);
          
          if (p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_rpm == 0) p_mib->vac_group_struct.vac_table_struct[i].vac_active_fan = 0;
          else p_mib->vac_group_struct.vac_table_struct[i].vac_active_fan = 1;
          
          p_mib->vac_group_struct.vac_table_struct[i].vac_installed_fan         = (uint32_t) (p_modbus_manager->vac_info_struct[i].installed_fan);
          p_mib->vac_group_struct.vac_table_struct[i].vac_installed_air_con     = (uint32_t) (p_modbus_manager->vac_info_struct[i].installed_air_con);
          //xem lai.-----------------
          sprintf(&p_mib->vac_group_struct.vac_table_struct[i].c_sync_time[0], "%02d:%02d:%02d-%02d/%02d/%02d",
                                                                                sync_time.hour,
                                                                                sync_time.min,
                                                                                sync_time.sec,
                                                                                sync_time.date,
                                                                                sync_time.month,
                                                                                sync_time.month);
          p_mib->vac_group_struct.vac_table_struct[i].c_sync_time_len           = 20;
          //xem lai-------------------
          sprintf(&p_mib->vac_group_struct.vac_table_struct[i].vac_serial[0],       "%04d%05d",
                                                                                p_modbus_manager->vac_info_struct[i].sn_hi, 
                                                                                p_modbus_manager->vac_info_struct[i].sn_lo);                  
          p_mib->vac_group_struct.vac_table_struct[i].vac_serial_len            = 10;
          
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con1_model        = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con1_model);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con1_type         = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con1_type);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con2_model        = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con2_model);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con2_type         = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con2_type);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_on_off        = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_on_off);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_mode          = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_mode);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_temp          = (int32_t)  (p_modbus_manager->vac_info_struct[i].air_con_temp);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_speed         = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_speed);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_cond_dir          = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_cond_dir);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_volt          = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_volt);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_current       = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_current);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_power         = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_power);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_frequency     = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_frequency);
          p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_pf            = (uint32_t) (p_modbus_manager->vac_info_struct[i].air_con_pf);
          p_mib->vac_group_struct.vac_table_struct[i].vac_firm_version          = (uint32_t) (p_modbus_manager->vac_info_struct[i].firm_version);
          p_mib->vac_group_struct.vac_table_struct[i].vac_w_enable              = (uint32_t) (p_modbus_manager->vac_info_struct[i].w_enable);
          p_mib->vac_group_struct.vac_table_struct[i].vac_w_serial1             = (uint32_t) (p_modbus_manager->vac_info_struct[i].w_serial1);
          p_mib->vac_group_struct.vac_table_struct[i].vac_w_serial2             = (uint32_t) (p_modbus_manager->vac_info_struct[i].w_serial2);
          
          p_mib->vac_group_struct.vac_table_struct[i].vac_type                  = p_modbus_manager->vac_manager_struct[i].vac_type;
          p_mib->vac_group_struct.vac_table_struct[i].vac_modbus_id             = p_modbus_manager->vac_manager_struct[i].vac_abs_slave_id;
          
          
          
        }
        break;
      default:
        break;
        
      }
    }
    else
    {
      p_mib->vac_group_struct.vac_table_struct[i].vac_alarm_status.all          = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_status                    = DISCONNECTED;
      p_mib->vac_group_struct.vac_table_struct[i].vac_type                      = p_modbus_manager->vac_manager_struct[i].vac_type;
      p_mib->vac_group_struct.vac_table_struct[i].vac_modbus_id                 = p_modbus_manager->vac_manager_struct[i].vac_abs_slave_id;
      
      p_mib->vac_group_struct.vac_table_struct[i].vac_in_temp                   = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_ex_temp                   = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_frost1_temp               = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_frost2_temp               = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_humid                     = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_duty                 = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_rpm                  = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan2_duty                 = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_rpm                  = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_sys_mode                  = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan_speed_mode            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan_start_temp            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_set_temp                  = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_pid_offset_temp           = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan_min_speed             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_fan_max_speed             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_filter_stuck_temp         = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_en             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_start          = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_end            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_night_max_speed           = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_manual_mode               = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_manual_max_speed          = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_in_max_temp               = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_ex_max_temp               = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_frost_max_temp            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_in_min_temp               = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_ex_min_temp               = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_frost_min_temp            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_min_out_temp              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_delta_temp                = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_panic_temp                = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_acu1_on_temp              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_acu1_on_temp              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_acu2_en                   = 0;
      
      p_mib->vac_group_struct.vac_table_struct[i].vac_active_fan                = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_installed_fan             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_installed_air_con         = 0;
      
      sprintf(&p_mib->vac_group_struct.vac_table_struct[i].c_sync_time[0],      "%02d:%02d:%02d-%02d/%02d/%02d",
                                                                                 0, 
                                                                                 0, 
                                                                                 0, 
                                                                                 0, 
                                                                                 0, 
                                                                                 0);
      p_mib->vac_group_struct.vac_table_struct[i].c_sync_time_len               = 20;
      
      sprintf(&p_mib->vac_group_struct.vac_table_struct[i].vac_serial[0], " ");                  
      p_mib->vac_group_struct.vac_table_struct[i].vac_serial_len                = 1;
      
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con1_model            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con1_type             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con2_model            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con2_type             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_on_off            = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_mode              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_temp              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_speed             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_cond_dir              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_volt              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_current           = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_power             = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_frequency         = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_pf                = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_firm_version              = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_w_enable                  = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_w_serial1                 = 0;
      p_mib->vac_group_struct.vac_table_struct[i].vac_w_serial2                 = 0;
      
    }
    
  }
  //clean vac---------------------------------------------------------------------------
  for(i=p_mib->vac_group_struct.vac_installed_vac; i<1; i++){
    p_mib->vac_group_struct.vac_table_struct[i].vac_alarm_status.all          = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_status                    = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_type                      = p_modbus_manager->vac_manager_struct[i].vac_type;
    p_mib->vac_group_struct.vac_table_struct[i].vac_modbus_id                 = p_modbus_manager->vac_manager_struct[i].vac_abs_slave_id;
    
    p_mib->vac_group_struct.vac_table_struct[i].vac_in_temp                   = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_ex_temp                   = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_frost1_temp               = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_frost2_temp               = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_humid                     = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_duty                 = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_rpm                  = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan2_duty                 = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan1_rpm                  = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_sys_mode                  = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan_speed_mode            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan_start_temp            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_set_temp                  = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_pid_offset_temp           = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan_min_speed             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_fan_max_speed             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_filter_stuck_temp         = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_en             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_start          = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_night_mode_end            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_night_max_speed           = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_manual_mode               = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_manual_max_speed          = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_in_max_temp               = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_ex_max_temp               = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_frost_max_temp            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_in_min_temp               = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_ex_min_temp               = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_frost_min_temp            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_min_out_temp              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_delta_temp                = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_panic_temp                = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_acu1_on_temp              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_acu1_on_temp              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_acu2_en                   = 0;
    
    p_mib->vac_group_struct.vac_table_struct[i].vac_active_fan                = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_installed_fan             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_installed_air_con         = 0;
    
    sprintf(&p_mib->vac_group_struct.vac_table_struct[i].c_sync_time[0],      "%02d:%02d:%02d-%02d/%02d/%02d",
            0, 
            0, 
            0, 
            0, 
            0, 
            0);
    p_mib->vac_group_struct.vac_table_struct[i].c_sync_time_len               = 20;
    
    sprintf(&p_mib->vac_group_struct.vac_table_struct[i].vac_serial[0], " ");                  
    p_mib->vac_group_struct.vac_table_struct[i].vac_serial_len                = 1;
    
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con1_model            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con1_type             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con2_model            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con2_type             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_on_off            = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_mode              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_temp              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_speed             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_cond_dir              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_volt              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_current           = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_power             = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_frequency         = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_air_con_pf                = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_firm_version              = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_w_enable                  = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_w_serial1                 = 0;
    p_mib->vac_group_struct.vac_table_struct[i].vac_w_serial2                 = 0;
    
  }
  
}

//update lib info--------------------------------------------------------------------------
void update_lib_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
    uint32_t j,i=0;
    
    p_mib->liBattGroup.liBattInstalledPack = p_modbus_manager->number_of_lib;
    p_mib->connect_alarm_group_struct.alarm_lib_connect = 0;
    p_mib->main_alarm_group_struct.alarm_lib = 0;
    p_mib->liBattGroup.liBattActivePack = p_mib->liBattGroup.liBattInstalledPack;
    for (i = 0; i < p_mib->liBattGroup.liBattInstalledPack; i++)
    {
//        p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = 0;
        p_mib->main_alarm_group_struct.alarm_lib_pack[i] = 0;
        if (p_modbus_manager->lib_info_struct[i].u8ErrorFlag == 1)
        {
          if(p_modbus_manager->cnt_dis_connect_lib[i] > 2){ // mat ket noi 3 lan moi bao len
            p_modbus_manager->cnt_dis_connect_lib[i] = 3;
            p_mib->liBattGroup.liBattActivePack--;
            
            if (p_mib->liBattGroup.liBattActivePack <= 0)
            {
              p_mib->liBattGroup.liBattActivePack = 0;
            }
            p_mib->connect_alarm_group_struct.alarm_lib_connect |= (p_modbus_manager->lib_info_struct[i].u8ErrorFlag << i);
            p_mib->liBattGroup.liBattTable[i].liBattStatus = 170;
          }     
        }
        else
        {
            p_modbus_manager->cnt_dis_connect_lib[i] = 0;

            if(p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all != 0)
            {
                p_mib->liBattGroup.liBattTable[i].liBattStatus = 165;
            }
            else
            {
                p_mib->liBattGroup.liBattTable[i].liBattStatus = 255;
            }
        }

        if (p_mib->liBattGroup.liBattTable[i].liBattStatus != 170)
        {
            if (p_mib->liBattGroup.liBattTable[i].liBattStatus == 165)
            {
                p_mib->main_alarm_group_struct.alarm_lib_pack[i] = p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all;
            }

            switch(p_modbus_manager->lib_manager_struct[i].lib_type)
            {
            case 13:// COSLIGHT_CF4850T
            case 1:// COSLIGHT 
              {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1; 
                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = p_modbus_manager->lib_info_struct[i].u16CellVolt[j];
                }
                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = p_modbus_manager->lib_info_struct[i].u16CellTemp[j] * 10;
                }
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell * 10;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16EnvTemp * 10;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 100;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 100;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain;
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u32PackCurr;
                p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattMode = p_modbus_manager->lib_info_struct[i].u16ModuleMode;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "GYFP4850T");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = 9;
 
                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32DSGCapacity;  
                float SV;
               
                SV = (float)p_modbus_manager->lib_info_struct[i].u16SoftwareVersion/100;
                sprintf(&p_mib->liBattGroup.liBattTable[i].liBattSWVer[0],"%.2f",SV);
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattSerialNo,"%d",p_modbus_manager->lib_info_struct[i].u32Serial);
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;

                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
              }
              break;
            case 2:// COSLIGHT_OLD_V1.1 
              {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1; 
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell * 10;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 100;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 100;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain;
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u32PackCurr;
                p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;

                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "GYFP4850T");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = 9;
                

                
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, "V1.1");
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattSerialNo,"%d",p_modbus_manager->lib_info_struct[i].u32Serial);
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;

                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
              }
              break;
            case 12:// SHOTO_SDA10_48100
            case 3:// SHOTO_2019
              {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = (int16_t)p_modbus_manager->lib_info_struct[i].u32PackCurr;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 10;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 10;
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16EnvTemp ;
                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32DSGCapacity * 1000;  
                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = p_modbus_manager->lib_info_struct[i].u16CellVolt[j];
                }
                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = p_modbus_manager->lib_info_struct[i].u16CellTemp[j];
                }
                
                p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattMode = p_modbus_manager->lib_info_struct[i].u16ModuleMode;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "SDA10-4850");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = 10;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, (const char*)&p_modbus_manager->lib_info_struct[i].u8swversion[0]);
//                sprintf(p_mib->liBattGroup.liBattTable[i].liBattSerialNo,"%d",p_modbus_manager->lib_info_struct[i].u32Serial);
                for(int k=0;k<30;k++)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattSerialNo[k] = p_modbus_manager->lib_info_struct[i].u8mode[k];
                }
                p_mib->liBattGroup.liBattTable[i].liBattSerialNo[30] = '\0';
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = 2;
                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;
//                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = 30;

                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
              }
              break;
            case 4:// HUAWEI
            {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u16PackCurr;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain;
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell * 10;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16EnvTemp * 10;
                p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = p_modbus_manager->lib_info_struct[i].u32ProtectStatus;
                p_mib->liBattGroup.liBattTable[i].liBattFaultStat = p_modbus_manager->lib_info_struct[i].u32FaultStatus;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 100;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 100;
                //thanhcm3 fix---------------------------------------------------------------
                if(p_modbus_manager->lib_info_struct[i].u16ModuleMode == 2)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 1;
                }else if(p_modbus_manager->lib_info_struct[i].u16ModuleMode == 3)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 2;
                }else{
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 4;
                }
                //p_mib->liBattGroup.liBattTable[i].liBattMode = p_modbus_manager->lib_info_struct[i].u16ModuleMode;
                //thanhcm3 fix----------------------------------------------------------------
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, (const char*)&p_modbus_manager->lib_info_struct[i].u8mode[0]);
                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32CapRemain * 100;  
//                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = 12;
                 for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = p_modbus_manager->lib_info_struct[i].u16CellVolt[j];
                }

                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = p_modbus_manager->lib_info_struct[i].u16CellTemp[j] * 10;
                }

                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "ESM-48100A2");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattModel);
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattSWVer,"%d",p_modbus_manager->lib_info_struct[i].u16SoftwareVersion);
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattHWVer,"%d",p_modbus_manager->lib_info_struct[i].u16HardwareVersion);
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
//                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, "unavailable");
//                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000020) == 0x00000020)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000010) == 0x00000010)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 0;
                }
                
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000080) == 0x00000080)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 0;
                }
                
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000040) == 0x00000040)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 0;
                }
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_VoltDiff = 1;
//                }
//                if ((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000001) == 0x00000001)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 1;
//                }
                if ((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000010) == 0x00000010)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000004) == 0x00000004))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000002) == 0x00000002) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000008) == 0x00000008))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 0;
                }
                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 1;
//                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00200000) == 0x00200000)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 0;
                }                
                
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00100000) == 0x00100000)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 0;
                }
                
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00800000) == 0x00800000)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 0;
                }                
                
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00400000) == 0x00400000)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 0;
                }
                
                if ((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000020) == 0x00000020)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 0;
                }
                
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00080000) == 0x00080000)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00020000) == 0x00020000))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 0;
                }
                
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00080000) == 0x00080000))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00010000) == 0x00010000))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00040000) == 0x00040000))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 0;
                }
                
//                p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;

                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            }
            break;
            case 5:// M1Viettel50
            {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].u32ADCPackVolt = p_modbus_manager->lib_info_struct[i].u32ADCPackVolt;
                p_mib->liBattGroup.liBattTable[i].u32ADCBattVolt = p_modbus_manager->lib_info_struct[i].u32ADCBattVolt;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u32PackCurr;
                p_mib->liBattGroup.liBattTable[i].i32ADCPackCurr = p_modbus_manager->lib_info_struct[i].i32ADCPackCurr;
                p_mib->liBattGroup.liBattTable[i].i32ADCBattCurr = p_modbus_manager->lib_info_struct[i].i32ADCBattCurr;
                
                for (j = 0; j < 15; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = p_modbus_manager->lib_info_struct[i].u16CellVolt[j];
                }

                for (j = 0; j < 4; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = p_modbus_manager->lib_info_struct[i].u16CellTemp[j]/10;
                }
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell/10;
                
                int32_t cellTemp1 = 0,cellTemp2 = 0;
                for (j = 0; j < 4; j++)
                {
                  if((p_modbus_manager->lib_info_struct[i].u16CellTemp[j]>0)&&(p_modbus_manager->lib_info_struct[i].u16CellTemp[j]<8000)){
                    cellTemp1++;
                    cellTemp2 = cellTemp2 + p_modbus_manager->lib_info_struct[i].u16CellTemp[j];
                  }
                }
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = cellTemp2 / cellTemp1;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_mib->liBattGroup.liBattTable[i].liBattAmbTemp / 10;
                
                p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = p_modbus_manager->lib_info_struct[i].u32ProtectStatus;
                p_mib->liBattGroup.liBattTable[i].liBattFaultStat = p_modbus_manager->lib_info_struct[i].u32FaultStatus;
                p_mib->liBattGroup.liBattTable[i].liBattMode = p_modbus_manager->lib_info_struct[i].u16ModuleMode;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC*10;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH*100;
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;  
                p_mib->liBattGroup.liBattTable[i].u32ConverterState = p_modbus_manager->lib_info_struct[i].u16ConverterState;     
                p_mib->liBattGroup.liBattTable[i].u32ErrCode = p_modbus_manager->lib_info_struct[i].u16ErrCode;     
                p_mib->liBattGroup.liBattTable[i].u32BalanceStatus = p_modbus_manager->lib_info_struct[i].u16BalanceStatus; 
                p_mib->liBattGroup.liBattTable[i].u32MosfetMode = p_modbus_manager->lib_info_struct[i].u16MosfetMode;     
                p_mib->liBattGroup.liBattTable[i].u32Mcu2McuErr = p_modbus_manager->lib_info_struct[i].u32Mcu2McuErr;     
                p_mib->liBattGroup.liBattTable[i].u32CHGCapacity = p_modbus_manager->lib_info_struct[i].u32CHGCapacity;     
                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32DSGCapacity;     
                p_mib->liBattGroup.liBattTable[i].u32Efficiency = p_modbus_manager->lib_info_struct[i].u32Efficiency;     
                p_mib->liBattGroup.liBattTable[i].u32NumberOfCan = p_modbus_manager->lib_info_struct[i].u16NumberOfCan;  
                //strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, &p_modbus_manager->lib_info_struct[i].u8swversion[0]);
                sprintf(&p_mib->liBattGroup.liBattTable[i].liBattSWVer[0],"%d",p_modbus_manager->lib_info_struct[i].sw_ver);
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);
                  
                p_mib->liBattGroup.liBattTable[i].u32SOCMaxim = (uint32_t)(p_modbus_manager->lib_info_struct[i].u16SOCMaxim*100/256);    
                p_mib->liBattGroup.liBattTable[i].u32SOHMaxim = (uint32_t)(p_modbus_manager->lib_info_struct[i].u16SOHMaxim*100/256); 
                if(p_mib->liBattGroup.liBattTable[i].u32SOHMaxim > 10000)
                  p_mib->liBattGroup.liBattTable[i].u32SOHMaxim = 10000;
                p_mib->liBattGroup.liBattTable[i].u32FullCapRepMaxim = p_modbus_manager->lib_info_struct[i].u16FullCapRepMaxim;    
                p_mib->liBattGroup.liBattTable[i].u32VoltMaxim = p_modbus_manager->lib_info_struct[i].u16VoltMaxim;    
                p_mib->liBattGroup.liBattTable[i].u32CurrMaxim = p_modbus_manager->lib_info_struct[i].u16CurrMaxim;  
                  
                p_mib->liBattGroup.liBattTable[i].u32IKalamn = p_modbus_manager->lib_info_struct[i].u32IKalamn;    
                p_mib->liBattGroup.liBattTable[i].u32SOCKalamn = p_modbus_manager->lib_info_struct[i].u32SOCKalamn;    
                p_mib->liBattGroup.liBattTable[i].u32VpriKalamn = p_modbus_manager->lib_info_struct[i].u32VpriKalamn;    
                p_mib->liBattGroup.liBattTable[i].u32VmesKalamn = p_modbus_manager->lib_info_struct[i].u32VmesKalamn;    
                p_mib->liBattGroup.liBattTable[i].u32CapKalamn = p_modbus_manager->lib_info_struct[i].u32CapKalamn;    
                p_mib->liBattGroup.liBattTable[i].u32InternalR0Kalamn = p_modbus_manager->lib_info_struct[i].u32InternalR0Kalamn;  
                
                p_mib->liBattGroup.liBattTable[i].i32PackHighVolt_A = p_modbus_manager->lib_info_struct[i].i32PackHighVolt_A;  
                p_mib->liBattGroup.liBattTable[i].i32BattHighVolt_A = p_modbus_manager->lib_info_struct[i].i32BattHighVolt_A;  
                p_mib->liBattGroup.liBattTable[i].i32CellHighVolt_A = p_modbus_manager->lib_info_struct[i].i32CellHighVolt_A;  
                p_mib->liBattGroup.liBattTable[i].i32BattLowVolt_A = p_modbus_manager->lib_info_struct[i].i32BattLowVolt_A;  
                p_mib->liBattGroup.liBattTable[i].i32CellLowVolt_A = p_modbus_manager->lib_info_struct[i].i32CellLowVolt_A;  
                p_mib->liBattGroup.liBattTable[i].i32CharHighCurr_A = p_modbus_manager->lib_info_struct[i].i32CharHighCurr_A;  
                p_mib->liBattGroup.liBattTable[i].i32DisCharHighCurr_A = p_modbus_manager->lib_info_struct[i].i32DisCharHighCurr_A;  
                p_mib->liBattGroup.liBattTable[i].i32CharHighTemp_A = p_modbus_manager->lib_info_struct[i].i32CharHighTemp_A;  
                p_mib->liBattGroup.liBattTable[i].i32DisCharHighTemp_A = p_modbus_manager->lib_info_struct[i].i32DisCharHighTemp_A;  
                p_mib->liBattGroup.liBattTable[i].i32CharLowTemp_A = p_modbus_manager->lib_info_struct[i].i32CharLowTemp_A;  
                p_mib->liBattGroup.liBattTable[i].i32DisCharLowtemp_A = p_modbus_manager->lib_info_struct[i].i32DisCharLowtemp_A;  
                p_mib->liBattGroup.liBattTable[i].i32LowCap_A = p_modbus_manager->lib_info_struct[i].i32LowCap_A;  
                p_mib->liBattGroup.liBattTable[i].i32BMSHighTemp_A = p_modbus_manager->lib_info_struct[i].i32BMSHighTemp_A;  
                p_mib->liBattGroup.liBattTable[i].i32BMSLowTemp_A = p_modbus_manager->lib_info_struct[i].i32BMSLowTemp_A;  
                p_mib->liBattGroup.liBattTable[i].i32PackOverVolt_P = p_modbus_manager->lib_info_struct[i].i32PackOverVolt_P;  
                p_mib->liBattGroup.liBattTable[i].i32BattOverVolt_P = p_modbus_manager->lib_info_struct[i].i32BattOverVolt_P;  
                p_mib->liBattGroup.liBattTable[i].i32CellOverVolt_P = p_modbus_manager->lib_info_struct[i].i32CellOverVolt_P;  
                p_mib->liBattGroup.liBattTable[i].i32BattUnderVolt_P = p_modbus_manager->lib_info_struct[i].i32BattUnderVolt_P;  
                p_mib->liBattGroup.liBattTable[i].i32CellUnderVolt_P = p_modbus_manager->lib_info_struct[i].i32CellUnderVolt_P;  
                p_mib->liBattGroup.liBattTable[i].i32CharOverCurr_P = p_modbus_manager->lib_info_struct[i].i32CharOverCurr_P; 
                p_mib->liBattGroup.liBattTable[i].i32DisCharOverCurr_P = p_modbus_manager->lib_info_struct[i].i32DisCharOverCurr_P;  
                p_mib->liBattGroup.liBattTable[i].i32CharOverTemp_P = p_modbus_manager->lib_info_struct[i].i32CharOverTemp_P;  
                p_mib->liBattGroup.liBattTable[i].i32DisCharOverTemp_P = p_modbus_manager->lib_info_struct[i].i32DisCharOverTemp_P;  
                p_mib->liBattGroup.liBattTable[i].i32CharUnderTemp_P = p_modbus_manager->lib_info_struct[i].i32CharUnderTemp_P;  
                p_mib->liBattGroup.liBattTable[i].i32DisCharUnderTemp_P = p_modbus_manager->lib_info_struct[i].i32DisCharUnderTemp_P;  
                p_mib->liBattGroup.liBattTable[i].i32UnderCap_P = p_modbus_manager->lib_info_struct[i].i32UnderCap_P;  
                p_mib->liBattGroup.liBattTable[i].i32BMSOverTemp_P = p_modbus_manager->lib_info_struct[i].i32BMSOverTemp_P;  
                p_mib->liBattGroup.liBattTable[i].i32BMSUnderTemp_P = p_modbus_manager->lib_info_struct[i].i32BMSUnderTemp_P;  
                p_mib->liBattGroup.liBattTable[i].i32DifferentVolt_P = p_modbus_manager->lib_info_struct[i].i32DifferentVolt_P;
                p_mib->liBattGroup.liBattTable[i].u32VoltBalance = p_modbus_manager->lib_info_struct[i].u32VoltBalance; 
                p_mib->liBattGroup.liBattTable[i].u32DeltaVoltBalance = p_modbus_manager->lib_info_struct[i].u32DeltaVoltBalance; 
                p_mib->liBattGroup.liBattTable[i].u32DisCharCurrLimit = p_modbus_manager->lib_info_struct[i].u32DisCharCurrLimit; 
                p_mib->liBattGroup.liBattTable[i].u32CharCurrLimit = p_modbus_manager->lib_info_struct[i].u32CharCurrLimit; 
                p_mib->liBattGroup.liBattTable[i].u32VoltDisCharRef = p_modbus_manager->lib_info_struct[i].u32VoltDisCharRef; 
                p_mib->liBattGroup.liBattTable[i].u32VoltDisCharCMD = p_modbus_manager->lib_info_struct[i].u32VoltDisCharCMD; 
                p_mib->liBattGroup.liBattTable[i].u32SystemByte = p_modbus_manager->lib_info_struct[i].u16SystemByte; 
                p_mib->liBattGroup.liBattTable[i].u32KeyTime = p_modbus_manager->lib_info_struct[i].u16KeyTime; 
                p_mib->liBattGroup.liBattTable[i].u16_liionCapInit = p_modbus_manager->lib_info_struct[i].u16_liionCapInit;

                p_mib->liBattGroup.liBattTable[i].u32EnableLock = p_modbus_manager->lib_info_struct[i].u16EnableLock;  
                
                if(p_modbus_manager->lib_info_struct[i].u16BattType == 50){
                  strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "VLB48C50F");
                  p_mib->liBattGroup.liBattTable[i].liBattModelLen = 9;
                } else if (p_modbus_manager->lib_info_struct[i].u16BattType == 100){
                  strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "VLB48C100F");
                  p_mib->liBattGroup.liBattTable[i].liBattModelLen = 10;
                }
                
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattSerialNo,"%d",(uint16_t)(p_modbus_manager->lib_info_struct[i].u32Serial>>16));
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattSerialNo+4,"%d",(uint16_t)(p_modbus_manager->lib_info_struct[i].u32Serial));
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 1;
                } else if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000002) == 0x00000002){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000004) == 0x00000004){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000010) == 0x00000010){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000020) == 0x00000020){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000040) == 0x00000040){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000080) == 0x00000080){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000100) == 0x00000100){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 1;
                } else if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000200) == 0x00000200){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000400) == 0x00000400){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 1;
                } else if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000800) == 0x00000800){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00001000) == 0x00001000){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00002000) == 0x00002000){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PCBHighTemp = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PCBHighTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000001) == 0x00000001){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 1;
                } else if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000002) == 0x00000002){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000004) == 0x00000004){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000010) == 0x00000010){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000020) == 0x00000020){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000040) == 0x00000040){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 1;
                } else if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000080) == 0x00000080){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000100) == 0x00000100){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000200) == 0x00000200){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000400) == 0x00000400){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000800) == 0x00000800){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00002000) == 0x00002000){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00004000) == 0x00004000){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_VoltDiff = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_VoltDiff = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00020000) == 0x00020000){
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_reverse = 1;
                } else {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_reverse = 0;
                }
                
                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            }
            break;
            case 8:// HUAWEI_A1
            {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u16PackCurr;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain;
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell * 10;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16EnvTemp * 10;
                p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = p_modbus_manager->lib_info_struct[i].u32ProtectStatus;
                p_mib->liBattGroup.liBattTable[i].liBattFaultStat = p_modbus_manager->lib_info_struct[i].u32FaultStatus;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 100;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 100;
                //thanhcm3 fix---------------------------------------------------------------------------------
                if(p_modbus_manager->lib_info_struct[i].u16ModuleMode == 2)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 1;
                }else if(p_modbus_manager->lib_info_struct[i].u16ModuleMode == 3)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 2;
                }else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 4;
                }
                //p_mib->liBattGroup.liBattTable[i].liBattMode = p_modbus_manager->lib_info_struct[i].u16ModuleMode;
                //thanhcm3 fix---------------------------------------------------------------------------------
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;
//                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, &p_modbus_manager->lib_info_struct[i].u8mode[0]);
//                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = 12;
                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32DSGCapacity * 100;  
                 for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = p_modbus_manager->lib_info_struct[i].u16CellVolt[j];
                }

                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = p_modbus_manager->lib_info_struct[i].u16CellTemp[j] * 10;
                }

                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32CapRemain * 100;  
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "ESM-48100A2");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattModel);
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattSWVer,"%d",p_modbus_manager->lib_info_struct[i].u16SoftwareVersion);
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);
                sprintf(p_mib->liBattGroup.liBattTable[i].liBattHWVer,"%d",p_modbus_manager->lib_info_struct[i].u16HardwareVersion);
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000020) == 0x00000020)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 0;
//                }
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000010) == 0x00000010)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 0;
//                }
//                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000080) == 0x00000080)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 0;
//                }
//                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000040) == 0x00000040)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 0;
//                }

//                if ((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000010) == 0x00000010)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 0;
//                }
//                
//                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000004) == 0x00000004))
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 0;
//                }
//                
//                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000002) == 0x00000002) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000008) == 0x00000008))
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 0;
//                }
                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00200000) == 0x00200000)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 0;
//                }                
//                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00100000) == 0x00100000)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 0;
//                }
//                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00800000) == 0x00800000)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 0;
//                }                
//                
//                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00400000) == 0x00400000)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 0;
//                }
//                
//                if ((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000020) == 0x00000020)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 0;
//                }
//                
//                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00080000) == 0x00080000)
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 0;
//                }
//                
//                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00020000) == 0x00020000))
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 0;
//                }
//                
//                
//                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00080000) == 0x00080000))
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 0;
//                }
//                
//                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00010000) == 0x00010000))
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 0;
//                }
//                
//                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00040000) == 0x00040000))
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 1;
//                }
//                else
//                {
//                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 0;
//                }
                
                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;

                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            }
            break;
            case 14:// HUAFU_HF48100C
            case 6:// ZTT_2020
            {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u16PackCurr;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 100;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 100;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain;
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16EnvTemp;
                p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = p_modbus_manager->lib_info_struct[i].u32ProtectStatus;
                p_mib->liBattGroup.liBattTable[i].liBattFaultStat = p_modbus_manager->lib_info_struct[i].u32FaultStatus;    
                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32DSGCapacity;  
                
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel,"ZTT50");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = 5;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, (const char*)&p_modbus_manager->lib_info_struct[i].u8mode[0]);
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = 20;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, (const char*)&p_modbus_manager->lib_info_struct[i].u8swversion[0]);
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = 20;
                
                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = p_modbus_manager->lib_info_struct[i].u16CellVolt[j];
                }

                for (j = 0; j < 8; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = p_modbus_manager->lib_info_struct[i].u16CellTemp[j];
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000002) == 0x00000002)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000004) == 0x00000004)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000008) == 0x00000008)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000010) == 0x00000010)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000020) == 0x00000020)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000100) == 0x00000100) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000400) == 0x00000400) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000800) == 0x00000800))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00001000) == 0x00001000) 
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvHighTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00002000) == 0x00002000) 
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvLowTemp = 0;
                }
                 if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00008000) == 0x00008000) 
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000001) == 0x00000001)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 0;
                }  
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000002) == 0x00000002)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000004) == 0x00000004)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000008) == 0x00000008)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000010) == 0x00000010)||((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000020) == 0x00000020))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000040) == 0x00000040)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000100) == 0x00000100))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000400) == 0x00000400))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000800) == 0x00000800))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000004) == 0x00000004))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_lost_sensor = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_lost_sensor = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000100) == 0x00000100))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 1;
                }
                else if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 2;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 3;
                }
                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            }
            break;
            case 9:// SAFT
            {
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt * 10;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u32PackCurr * 10;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 100;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 100;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell * 10;
                p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
              
                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            }
            break;
            case 10:// Narada75
            {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = (p_modbus_manager->lib_info_struct[i].u32PackCurr - 10000) * 10;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain * 10;
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell - 400;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16EnvTemp - 400;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC;
                
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattModel);
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000002) == 0x00000002))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000004) == 0x00000004))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000008) == 0x00000008))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000010) == 0x00000010))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000020) == 0x00000020))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000040) == 0x00000040))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000080) == 0x00000080))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000100) == 0x00000100))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvLowTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000400) == 0x00000400))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PCBHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PCBHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000800) == 0x00000800))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00001000) == 0x00001000))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_VoltDiff = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_VoltDiff = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000001) == 0x00000001))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000002) == 0x00000002))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000004) == 0x00000004))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000008) == 0x00000008))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000010) == 0x00000010))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000020) == 0x00000020))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000040) == 0x00000040))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000080) == 0x00000080))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000100) == 0x00000100))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000100) == 0x00000100))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 1;
                }
                else if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 2;
                }
                else 
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 0;
                }
                        
                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;
                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;   
            }
            break;
            case 7:// ZTT50
            {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = (int32_t)(p_modbus_manager->lib_info_struct[i].fPackVolt * 100);
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = (int32_t)(p_modbus_manager->lib_info_struct[i].fPackCurr * 100);
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = (int32_t)(p_modbus_manager->lib_info_struct[i].fCapRemain * 10);
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp =(int32_t) (p_modbus_manager->lib_info_struct[i].fAverTempCell * 10);
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = (int32_t)(p_modbus_manager->lib_info_struct[i].fEnvTemp * 10);
                p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = (uint16_t)p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = (uint16_t)p_modbus_manager->lib_info_struct[i].u32ProtectStatus;
                p_mib->liBattGroup.liBattTable[i].liBattFaultStat = (uint16_t) p_modbus_manager->lib_info_struct[i].u32FaultStatus;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = (uint32_t)(p_modbus_manager->lib_info_struct[i].fSOC * 100);
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH;
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;

                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "LD4850-X");
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattModel);

//                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, "V10.10");
                for(int j=0;j<7;j++)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattSWVer[j] = p_modbus_manager->lib_info_struct[i].u8swversion[j];
                }
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = 6;

//                strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "V01.10.11.02.00");
                for(int j=0;j<16;j++)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattHWVer[j] = p_modbus_manager->lib_info_struct[i].u8hwversion[j];
                }
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = 15;

//                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, "2015111101180");
                for(int j=0;j<21;j++)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattSerialNo[j] = p_modbus_manager->lib_info_struct[i].u8mode[j];
                }
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = 20;
                
                p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = (p_mib->liBattGroup.liBattTable[i].liBattProtectFlag << 16) |
                                                                               p_mib->liBattGroup.liBattTable[i].liBattWarningFlag;
                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;

                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            }
            break;
            case 11:// EVE
            {
                p_mib->liBattGroup.liBattTable[i].liBattIndex = i+1;
                p_mib->liBattGroup.liBattTable[i].liBattPackCurr = p_modbus_manager->lib_info_struct[i].u16PackCurr;
                p_mib->liBattGroup.liBattTable[i].liBattPackVolt = p_modbus_manager->lib_info_struct[i].u32PackVolt;
                p_mib->liBattGroup.liBattTable[i].liBattSOC = p_modbus_manager->lib_info_struct[i].u32SOC * 100;
                p_mib->liBattGroup.liBattTable[i].liBattSOH = p_modbus_manager->lib_info_struct[i].u32SOH * 100;
                p_mib->liBattGroup.liBattTable[i].liBattRemainCap = p_modbus_manager->lib_info_struct[i].u32CapRemain;
                p_mib->liBattGroup.liBattTable[i].liBattPeriod = p_modbus_manager->lib_info_struct[i].u32DischargeTime;
                p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = p_modbus_manager->lib_info_struct[i].u16AverTempCell;
                p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = p_modbus_manager->lib_info_struct[i].u16EnvTemp;
                p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = p_modbus_manager->lib_info_struct[i].u32AlarmStatus;
                p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = p_modbus_manager->lib_info_struct[i].u32ProtectStatus;
                p_mib->liBattGroup.liBattTable[i].liBattFaultStat = p_modbus_manager->lib_info_struct[i].u32FaultStatus;    
                p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = p_modbus_manager->lib_info_struct[i].u32DSGCapacity;  
                
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel,"EVE4850A");
//                p_mib->liBattGroup.liBattTable[i].liBattModelLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattModel);
                p_mib->liBattGroup.liBattTable[i].liBattModelLen = 8;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
                p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, (const char*)&p_modbus_manager->lib_info_struct[i].u8mode[0]);
//                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
                p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = 20;
                strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer,(const char*) &p_modbus_manager->lib_info_struct[i].u8swversion[0]);
                p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = 20;
                
                for (j = 0; j < 16; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = p_modbus_manager->lib_info_struct[i].u16CellVolt[j];
                }

                for (j = 0; j < 8; j++)
                {
                    p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = p_modbus_manager->lib_info_struct[i].u16CellTemp[j];
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000001) == 0x00000001)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000002) == 0x00000002)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CellLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000004) == 0x00000004)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000008) == 0x00000008)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_PackLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000010) == 0x00000010)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_CharOverCurrent = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000020) == 0x00000020)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_DisCharOverCurrent = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000100) == 0x00000100) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000400) == 0x00000400) || ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00000800) == 0x00000800))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_BattLowTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00001000) == 0x00001000) 
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvHighTemp = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00002000) == 0x00002000) 
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_EnvLowTemp = 0;
                }
                 if ((p_modbus_manager->lib_info_struct[i].u32AlarmStatus & 0x00008000) == 0x00008000) 
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.alarm_LowCapacity = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000001) == 0x00000001)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellOverVolt = 0;
                }  
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000002) == 0x00000002)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CellLowVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000004) == 0x00000004)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackOverVolt = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000008) == 0x00000008)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_PackLowVolt = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000010) == 0x00000010)||((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000020) == 0x00000020))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_OverCurr = 0;
                }
                if ((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000040) == 0x00000040)
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_ShortCircuit = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000100) == 0x00000100))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharHighTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000400) == 0x00000400))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_CharLowTemp = 0;
                }
                
                if (((p_modbus_manager->lib_info_struct[i].u32ProtectStatus & 0x00000800) == 0x00000800))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_DisCharLowTemp = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000004) == 0x00000004))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_lost_sensor = 1;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.bit.protect_lost_sensor = 0;
                }
                if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000100) == 0x00000100))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 1;
                }
                else if (((p_modbus_manager->lib_info_struct[i].u32FaultStatus & 0x00000200) == 0x00000200))
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 2;
                }
                else
                {
                  p_mib->liBattGroup.liBattTable[i].liBattMode = 3;
                }
                
//                p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;
                p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
                p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            }
            break;
            };


        }
        else
        {
            p_mib->liBattGroup.liBattTable[i].liBattIndex = 0;
            p_mib->liBattGroup.liBattTable[i].liBattPackVolt = 0;
            p_mib->liBattGroup.liBattTable[i].liBattPackCurr = 0;
            p_mib->liBattGroup.liBattTable[i].liBattRemainCap = 0;
            p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = 0;
            p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = 0;
            p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = 0;
            p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = 0;
            p_mib->liBattGroup.liBattTable[i].liBattFaultStat = 0;
            p_mib->liBattGroup.liBattTable[i].liBattMode = 0;
            p_mib->liBattGroup.liBattTable[i].liBattSOC = 0;
            p_mib->liBattGroup.liBattTable[i].liBattSOH = 0;
            p_mib->liBattGroup.liBattTable[i].liBattPeriod = 0;

            for (j = 0; j < 16; j++)
            {
                p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = 0;
            }

            for (j = 0; j < 16; j++)
            {
                p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = 0;
            }

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattModelLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattModel);

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
            
            p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = (p_mib->liBattGroup.liBattTable[i].liBattProtectFlag << 16) |
                                                                               p_mib->liBattGroup.liBattTable[i].liBattWarningFlag;
            p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;

            p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
            p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            
            //=== M1
            p_mib->liBattGroup.liBattTable[i].u32ConverterState = 0;
            p_mib->liBattGroup.liBattTable[i].u32ErrCode = 0;
            p_mib->liBattGroup.liBattTable[i].u32BalanceStatus = 0;
            p_mib->liBattGroup.liBattTable[i].u32MosfetMode = 0;
            p_mib->liBattGroup.liBattTable[i].u32Mcu2McuErr = 0;
            p_mib->liBattGroup.liBattTable[i].u32CHGCapacity = 0;
            p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = 0;
            p_mib->liBattGroup.liBattTable[i].u32Efficiency = 0;
            p_mib->liBattGroup.liBattTable[i].u32NumberOfCan = 0;
            p_mib->liBattGroup.liBattTable[i].i32PackHighVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattHighVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellHighVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattLowVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellLowVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharHighCurr_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharHighCurr_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharHighTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharHighTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharLowTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharLowtemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32LowCap_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSHighTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSLowTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32PackOverVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattOverVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellOverVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattUnderVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellUnderVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharOverCurr_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharOverCurr_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharOverTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharOverTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharUnderTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharUnderTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32UnderCap_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSOverTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSUnderTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DifferentVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltBalance = 0;
            p_mib->liBattGroup.liBattTable[i].u32DeltaVoltBalance = 0;
            p_mib->liBattGroup.liBattTable[i].u32DisCharCurrLimit = 0;
            p_mib->liBattGroup.liBattTable[i].u32CharCurrLimit = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltDisCharRef = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltDisCharCMD = 0;
            p_mib->liBattGroup.liBattTable[i].u32SystemByte = 0;
            p_mib->liBattGroup.liBattTable[i].u32KeyTime = 0;
            p_mib->liBattGroup.liBattTable[i].u32IniMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32EnableKCS = 0;
            p_mib->liBattGroup.liBattTable[i].u32EnableLock = 0;
            p_mib->liBattGroup.liBattTable[i].u32EnableLock = 0;
            p_mib->liBattGroup.liBattTable[i].u32ADCPackVolt = 0;
            p_mib->liBattGroup.liBattTable[i].u32ADCBattVolt = 0;
            p_mib->liBattGroup.liBattTable[i].i32ADCPackCurr = 0;
            p_mib->liBattGroup.liBattTable[i].i32ADCBattCurr = 0;
            p_mib->liBattGroup.liBattTable[i].u32SOCMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32SOHMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32FullCapRepMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32CurrMaxim = 0; 
            p_mib->liBattGroup.liBattTable[i].u32IKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32SOCKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32VpriKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32VmesKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32CapKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32InternalR0Kalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u16_liionCapInit    = 0;

        }
    }
    //clean---------------------------------------------------------------
    for(i = p_mib->liBattGroup.liBattInstalledPack;i<16;i++){
            p_mib->liBattGroup.liBattTable[i].liBattIndex = 0;
            p_mib->liBattGroup.liBattTable[i].liBattPackVolt = 0;
            p_mib->liBattGroup.liBattTable[i].liBattPackCurr = 0;
            p_mib->liBattGroup.liBattTable[i].liBattRemainCap = 0;
            p_mib->liBattGroup.liBattTable[i].liBattAvrCellTemp = 0;
            p_mib->liBattGroup.liBattTable[i].liBattAmbTemp = 0;
            p_mib->liBattGroup.liBattTable[i].liBattWarningFlag = 0;
            p_mib->liBattGroup.liBattTable[i].liBattProtectFlag = 0;
            p_mib->liBattGroup.liBattTable[i].liBattFaultStat = 0;
            p_mib->liBattGroup.liBattTable[i].liBattMode = 0;
            p_mib->liBattGroup.liBattTable[i].liBattSOC = 0;
            p_mib->liBattGroup.liBattTable[i].liBattSOH = 0;
            p_mib->liBattGroup.liBattTable[i].liBattPeriod = 0;

            for (j = 0; j < 16; j++)
            {
                p_mib->liBattGroup.liBattTable[i].liBattCellVolt[j] = 0;
            }

            for (j = 0; j < 16; j++)
            {
                p_mib->liBattGroup.liBattTable[i].liBattCellTemp[j] = 0;
            }

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattModel, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattModelLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattModel);

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattSWVer, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattSWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSWVer);

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattHWVer, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattHWVerLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattHWVer);

            strcpy(p_mib->liBattGroup.liBattTable[i].liBattSerialNo, "unavailable");
            p_mib->liBattGroup.liBattTable[i].liBattSerialNoLen = strlen(p_mib->liBattGroup.liBattTable[i].liBattSerialNo);
            
            p_mib->liBattGroup.liBattTable[i].liBattAlarmStatus.all = (p_mib->liBattGroup.liBattTable[i].liBattProtectFlag << 16) |
                                                                               p_mib->liBattGroup.liBattTable[i].liBattWarningFlag;
            p_mib->liBattGroup.liBattTable[i].liBattStatusCCL = 0;

            p_mib->liBattGroup.liBattTable[i].liBattType = p_modbus_manager->lib_manager_struct[i].lib_type;
            p_mib->liBattGroup.liBattTable[i].liBattModbusID = p_modbus_manager->lib_manager_struct[i].lib_abs_slave_id;
            
            //=== M1
            p_mib->liBattGroup.liBattTable[i].u32ConverterState = 0;
            p_mib->liBattGroup.liBattTable[i].u32ErrCode = 0;
            p_mib->liBattGroup.liBattTable[i].u32BalanceStatus = 0;
            p_mib->liBattGroup.liBattTable[i].u32MosfetMode = 0;
            p_mib->liBattGroup.liBattTable[i].u32Mcu2McuErr = 0;
            p_mib->liBattGroup.liBattTable[i].u32CHGCapacity = 0;
            p_mib->liBattGroup.liBattTable[i].u32DSGCapacity = 0;
            p_mib->liBattGroup.liBattTable[i].u32Efficiency = 0;
            p_mib->liBattGroup.liBattTable[i].u32NumberOfCan = 0;
            p_mib->liBattGroup.liBattTable[i].i32PackHighVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattHighVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellHighVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattLowVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellLowVolt_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharHighCurr_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharHighCurr_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharHighTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharHighTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharLowTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharLowtemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32LowCap_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSHighTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSLowTemp_A = 0;
            p_mib->liBattGroup.liBattTable[i].i32PackOverVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattOverVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellOverVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BattUnderVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CellUnderVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharOverCurr_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharOverCurr_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharOverTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharOverTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32CharUnderTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DisCharUnderTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32UnderCap_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSOverTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32BMSUnderTemp_P = 0;
            p_mib->liBattGroup.liBattTable[i].i32DifferentVolt_P = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltBalance = 0;
            p_mib->liBattGroup.liBattTable[i].u32DeltaVoltBalance = 0;
            p_mib->liBattGroup.liBattTable[i].u32DisCharCurrLimit = 0;
            p_mib->liBattGroup.liBattTable[i].u32CharCurrLimit = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltDisCharRef = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltDisCharCMD = 0;
            p_mib->liBattGroup.liBattTable[i].u32SystemByte = 0;
            p_mib->liBattGroup.liBattTable[i].u32KeyTime = 0;
            p_mib->liBattGroup.liBattTable[i].u32IniMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32EnableKCS = 0;
            p_mib->liBattGroup.liBattTable[i].u32EnableLock = 0;
            p_mib->liBattGroup.liBattTable[i].u32EnableLock = 0;
            p_mib->liBattGroup.liBattTable[i].u32ADCPackVolt = 0;
            p_mib->liBattGroup.liBattTable[i].u32ADCBattVolt = 0;
            p_mib->liBattGroup.liBattTable[i].i32ADCPackCurr = 0;
            p_mib->liBattGroup.liBattTable[i].i32ADCBattCurr = 0;
            p_mib->liBattGroup.liBattTable[i].u32SOCMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32SOHMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32FullCapRepMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32VoltMaxim = 0;
            p_mib->liBattGroup.liBattTable[i].u32CurrMaxim = 0; 
            p_mib->liBattGroup.liBattTable[i].u32IKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32SOCKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32VpriKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32VmesKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32CapKalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u32InternalR0Kalamn = 0;
            p_mib->liBattGroup.liBattTable[i].u16_liionCapInit    = 0;
    }
}
  
//update gen info--------------------------------------------------------------
void update_gen_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  uint32_t i=0;
  
  p_mib->genGroup.genInstalledGen = p_modbus_manager->number_of_gen;
  p_mib->connect_alarm_group_struct.alarm_gen_connect = 0;
  p_mib->main_alarm_group_struct.alarm_gen = 0;
  p_mib->genGroup.genActiveGen = p_mib->genGroup.genInstalledGen;
  for (i = 0; i < p_mib->genGroup.genInstalledGen; i++)
  {
      p_mib->main_alarm_group_struct.alarm_gen_pack[i] = 0;
      if (p_modbus_manager->gen_info_struct[i].u8ErrorFlag == 1)
      {
        if(p_modbus_manager->cnt_dis_connect_gen[i] > 2){ // mat ket noi 3 lan moi bao len
          p_modbus_manager->cnt_dis_connect_gen[i] = 3;
          p_mib->genGroup.genActiveGen--;
          if (p_mib->genGroup.genActiveGen <= 0)
          {
            p_mib->genGroup.genActiveGen = 0;
          }
          p_mib->connect_alarm_group_struct.alarm_gen_connect |= (p_modbus_manager->gen_info_struct[i].u8ErrorFlag << i);
          p_mib->genGroup.genTable[i].genStatusStatus = 170;
        }
      }
      else
      {
        p_modbus_manager->cnt_dis_connect_gen[i] = 0;
//          p_mib->genGroup.genTable[i].genStatusStatus = 255;
            if(p_mib->genGroup.genTable[i].genStatusAlarmStatus.all != 0)
            {
                p_mib->genGroup.genTable[i].genStatusStatus = 165;
            }
            else
            {
                p_mib->genGroup.genTable[i].genStatusStatus = 255;
            }
      }

      if (p_mib->genGroup.genTable[i].genStatusStatus != 170)
      {
          if (p_mib->genGroup.genTable[i].genStatusStatus == 165)
          {
              p_mib->main_alarm_group_struct.alarm_gen_pack[i] = p_mib->genGroup.genTable[i].genStatusAlarmStatus.all;
          }

          switch(p_modbus_manager->gen_manager_struct[i].gen_type)
          {
          case 1:// KUBOTA
          {
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              p_mib->genGroup.genTable[i].genStatusFrequency = p_modbus_manager->gen_info_struct[i].u32GenFrequency;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = p_modbus_manager->gen_info_struct[i].u32EngineSpeed;
              p_mib->genGroup.genTable[i].genStatusOilPressure = p_modbus_manager->gen_info_struct[i].u32OilPressure;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp * 10;
              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel;
              p_mib->genGroup.genTable[i].genStatusBattVolt = p_modbus_manager->gen_info_struct[i].u32BattVolt;
              
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * 100;
              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = p_modbus_manager->gen_info_struct[i].u32LNCurr1 * 10;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = p_modbus_manager->gen_info_struct[i].u32LNCurr2 * 10;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = p_modbus_manager->gen_info_struct[i].u32LNCurr3 * 10;
              
              p_mib->genGroup.genTable[i].genStatusLNSPower1 = p_modbus_manager->gen_info_struct[i].u32LNSPower1;
              p_mib->genGroup.genTable[i].genStatusLNSPower2 = p_modbus_manager->gen_info_struct[i].u32LNSPower2;
              p_mib->genGroup.genTable[i].genStatusLNSPower3 = p_modbus_manager->gen_info_struct[i].u32LNSPower3;
              
              p_mib->genGroup.genTable[i].genStatusLNPower1 = p_modbus_manager->gen_info_struct[i].u32LNPower1;
              p_mib->genGroup.genTable[i].genStatusLNPower2 = p_modbus_manager->gen_info_struct[i].u32LNPower2;
              p_mib->genGroup.genTable[i].genStatusLNPower3 = p_modbus_manager->gen_info_struct[i].u32LNPower3;
              
              p_mib->genGroup.genTable[i].genStatusWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour;
              p_mib->genGroup.genTable[i].genStatusWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin;
              p_mib->genGroup.genTable[i].genStatusActiveEnergyLow = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyLow;
              p_mib->genGroup.genTable[i].genStatusActiveEnergyHigh = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyHigh;
              
              p_modbus_manager->gen_info_struct[i].u32AlarmStatus = (p_modbus_manager->gen_info_struct[i].u16Error2Status << 16) | (p_modbus_manager->gen_info_struct[i].u16Error1Status);
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = p_modbus_manager->gen_info_struct[i].u32AlarmStatus;
              //thanhcm3 fix -----------------------------------------------------------
              //p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowFuel = 0;//thanhcm3 fix
              //thanhcm3 fix------------------------------------------------------------
              if(p_modbus_manager->gen_info_struct[i].u16ErrorBattVolt == 1) 
              {
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_HiBattVol = 1;
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowBattVol = 0;
                
              }
              else if(p_modbus_manager->gen_info_struct[i].u16ErrorBattVolt == 2) 
              {
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_HiBattVol = 0;
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowBattVol = 1;          
              }
              else 
              {
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_HiBattVol = 0;
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowBattVol = 0; 
              }       
              
              if((p_modbus_manager->gen_info_struct[i].u16StatusBits1 & 0x02) == 0x02)
              {
                p_mib->genGroup.genTable[i].genStopMode = 1;
                p_mib->genGroup.genTable[i].genAutoMode = 0;
                p_mib->genGroup.genTable[i].genManualMode = 0;
              }
              else if((p_modbus_manager->gen_info_struct[i].u16StatusBits1 & 0x04) == 0x04)
              {
                p_mib->genGroup.genTable[i].genStopMode = 0;
                p_mib->genGroup.genTable[i].genAutoMode = 0;
                p_mib->genGroup.genTable[i].genManualMode = 1;
              }
              else if((p_modbus_manager->gen_info_struct[i].u16StatusBits2 & 0x08) == 0x08)
              {
                p_mib->genGroup.genTable[i].genStopMode = 0;
                p_mib->genGroup.genTable[i].genAutoMode = 1;
                p_mib->genGroup.genTable[i].genManualMode = 0;
              }
              else 
              {
                p_mib->genGroup.genTable[i].genStopMode = 0;
                p_mib->genGroup.genTable[i].genAutoMode = 0;
                p_mib->genGroup.genTable[i].genManualMode = 0;             
              }
              
              if (p_modbus_manager->gen_info_struct[i].u32GenFrequency == 0)
                p_mib->genGroup.genTable[i].genStartMode = 0;
              else 
                p_mib->genGroup.genTable[i].genStartMode = 1;
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
          }
          break;
          case 2:// BE142
          {
            
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              
              p_mib->genGroup.genTable[i].genStatusFrequency = p_modbus_manager->gen_info_struct[i].u32GenFrequency;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = p_modbus_manager->gen_info_struct[i].u32EngineSpeed;
              p_mib->genGroup.genTable[i].genStatusOilPressure = p_modbus_manager->gen_info_struct[i].u32OilPressure;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp * 10;
              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel;
              p_mib->genGroup.genTable[i].genStatusBattVolt = p_modbus_manager->gen_info_struct[i].u32BattVolt;
              
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * 100;
              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = p_modbus_manager->gen_info_struct[i].u32LNCurr1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = p_modbus_manager->gen_info_struct[i].u32LNCurr2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = p_modbus_manager->gen_info_struct[i].u32LNCurr3 * 100;
//              
//              p_mib->genGroup.genTable[i].genStatusLNPower1 = p_modbus_manager->gen_info_struct[i].u32LNPower1;
//              p_mib->genGroup.genTable[i].genStatusLNPower2 = p_modbus_manager->gen_info_struct[i].u32LNPower2;
//              p_mib->genGroup.genTable[i].genStatusLNPower3 = p_modbus_manager->gen_info_struct[i].u32LNPower3;
//              
              p_mib->genGroup.genTable[i].genStatusWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour;
//              p_mib->genGroup.genTable[i].genStatusWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin;
//              p_mib->genGroup.genTable[i].genStatusActiveEnergyLow = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyLow;
//              p_mib->genGroup.genTable[i].genStatusActiveEnergyHigh = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyHigh;
//              
//              p_modbus_manager->gen_info_struct[i].u32AlarmStatus = (p_modbus_manager->gen_info_struct[i].u16Error2Status << 16) | (p_modbus_manager->gen_info_struct[i].u16Error1Status);
//             
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = p_modbus_manager->gen_info_struct[i].u32AlarmStatus;
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
          }
          break;
          case 3:// DEEPSEA
          {
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              p_mib->genGroup.genTable[i].genStatusFrequency = p_modbus_manager->gen_info_struct[i].u32GenFrequency;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = p_modbus_manager->gen_info_struct[i].u32EngineSpeed;
              p_mib->genGroup.genTable[i].genStatusOilPressure = p_modbus_manager->gen_info_struct[i].u32OilPressure / 10;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp * 10;
              //thanhcm3 fix ----------------------------------------------------------------------------------------
              p_mib->genGroup.genTable[i].genStatusOilTemp     = p_modbus_manager->gen_info_struct[i].oil_temperature *10;
              //thanhcm3 fix ----------------------------------------------------------------------------------------  
              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel;
              p_mib->genGroup.genTable[i].genStatusBattVolt = p_modbus_manager->gen_info_struct[i].u32BattVolt;
              
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * 10;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * 10;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * 10;
              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = p_modbus_manager->gen_info_struct[i].u32LNCurr1 * 10;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = p_modbus_manager->gen_info_struct[i].u32LNCurr2 * 10;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = p_modbus_manager->gen_info_struct[i].u32LNCurr3 * 10;
              
              p_mib->genGroup.genTable[i].genStatusLNPower1 = p_modbus_manager->gen_info_struct[i].u32LNPower1;
              p_mib->genGroup.genTable[i].genStatusLNPower2 = p_modbus_manager->gen_info_struct[i].u32LNPower2;
              p_mib->genGroup.genTable[i].genStatusLNPower3 = p_modbus_manager->gen_info_struct[i].u32LNPower3;
              
              p_mib->genGroup.genTable[i].genStatusWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour;
              p_mib->genGroup.genTable[i].genStatusWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin;
//              p_mib->genGroup.genTable[i].genStatusActiveEnergyLow = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyLow;
//              p_mib->genGroup.genTable[i].genStatusActiveEnergyHigh = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyHigh;
              
//              p_modbus_manager->gen_info_struct[i].u32AlarmStatus = (p_modbus_manager->gen_info_struct[i].u16Error2Status << 16) | (p_modbus_manager->gen_info_struct[i].u16Error1Status);
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = p_modbus_manager->gen_info_struct[i].u32AlarmStatus;
              
              if(p_modbus_manager->gen_info_struct[i].u16StatusBits1 == 0)
              {
                p_mib->genGroup.genTable[i].genStopMode = 1;
                p_mib->genGroup.genTable[i].genAutoMode = 0;
                p_mib->genGroup.genTable[i].genManualMode = 0;
              }
              else if(p_modbus_manager->gen_info_struct[i].u16StatusBits1 == 2)
              {
                p_mib->genGroup.genTable[i].genStopMode = 0;
                p_mib->genGroup.genTable[i].genAutoMode = 0;
                p_mib->genGroup.genTable[i].genManualMode = 1;
              }
              else if(p_modbus_manager->gen_info_struct[i].u16StatusBits1 == 1)
              {
                p_mib->genGroup.genTable[i].genStopMode = 0;
                p_mib->genGroup.genTable[i].genAutoMode = 1;
                p_mib->genGroup.genTable[i].genManualMode = 0;
              }
              else 
              {
                p_mib->genGroup.genTable[i].genStopMode = 0;
                p_mib->genGroup.genTable[i].genAutoMode = 0;
                p_mib->genGroup.genTable[i].genManualMode = 0;             
              }
              
              if (p_modbus_manager->gen_info_struct[i].u32GenFrequency == 0)
                p_mib->genGroup.genTable[i].genStartMode = 0;
              else 
                p_mib->genGroup.genTable[i].genStartMode = 1;              
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
          }
          break;
          case 4:// LR2057
          {
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * 100;
              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = p_modbus_manager->gen_info_struct[i].u32LNCurr1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = p_modbus_manager->gen_info_struct[i].u32LNCurr2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = p_modbus_manager->gen_info_struct[i].u32LNCurr3 * 100;
              
              p_mib->genGroup.genTable[i].genStatusLNPower1 = p_modbus_manager->gen_info_struct[i].u32LNPower1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNPower2 = p_modbus_manager->gen_info_struct[i].u32LNPower2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNPower3 = p_modbus_manager->gen_info_struct[i].u32LNPower3 * 100;
              
              p_mib->genGroup.genTable[i].genStatusFrequency = p_modbus_manager->gen_info_struct[i].u32GenFrequency;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = p_modbus_manager->gen_info_struct[i].u32EngineSpeed;
              p_mib->genGroup.genTable[i].genStatusBattVolt = p_modbus_manager->gen_info_struct[i].u32BattVolt;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp * 10;
              p_mib->genGroup.genTable[i].genStatusOilPressure = p_modbus_manager->gen_info_struct[i].u32OilPressure / 10;
              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel;
              p_mib->genGroup.genTable[i].genStatusWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour;
              p_mib->genGroup.genTable[i].genStatusWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin;
              
              
//              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp;
//              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel;
//              p_mib->genGroup.genTable[i].genStatusActiveEnergyLow = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyLow;
//              p_mib->genGroup.genTable[i].genStatusActiveEnergyHigh = p_modbus_manager->gen_info_struct[i].u32ActiveEnergyHigh;
              
//              p_modbus_manager->gen_info_struct[i].u32AlarmStatus = (p_modbus_manager->gen_info_struct[i].u16Error2Status << 16) | (p_modbus_manager->gen_info_struct[i].u16Error1Status);
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = p_modbus_manager->gen_info_struct[i].u32AlarmStatus;
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
          }
          break;
          case 5:// HIMOINSA
          {
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              p_mib->genGroup.genTable[i].genStatusFrequency = p_modbus_manager->gen_info_struct[i].u32GenFrequency;
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * 100;
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = p_modbus_manager->gen_info_struct[i].u32LNCurr1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = p_modbus_manager->gen_info_struct[i].u32LNCurr2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = p_modbus_manager->gen_info_struct[i].u32LNCurr3 * 100;
              p_mib->genGroup.genTable[i].genStatusLNPower1 = p_modbus_manager->gen_info_struct[i].u32LNPower1 * 100;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = p_modbus_manager->gen_info_struct[i].u32EngineSpeed;
              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel / 10;
              p_mib->genGroup.genTable[i].genStatusBattVolt = p_modbus_manager->gen_info_struct[i].u32BattVolt;
              p_mib->genGroup.genTable[i].genStatusOilPressure = p_modbus_manager->gen_info_struct[i].u32OilPressure;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp;
              p_mib->genGroup.genTable[i].genStatusWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour;
//              p_mib->genGroup.genTable[i].genStatusWorkingMin = p_modbus_manager->gen_info_struct[i].u32EngineWorkingMin;
             
             
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = p_modbus_manager->gen_info_struct[i].u32AlarmStatus;
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
          }
          break;
          case 6:// QC315
          {
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * 100 / 256;
              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = p_modbus_manager->gen_info_struct[i].u32LNCurr1 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = p_modbus_manager->gen_info_struct[i].u32LNCurr2 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = p_modbus_manager->gen_info_struct[i].u32LNCurr3 * 100 / 256;
              
              p_mib->genGroup.genTable[i].genStatusLNPower1 = p_modbus_manager->gen_info_struct[i].u32LNPower1 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNPower2 = p_modbus_manager->gen_info_struct[i].u32LNPower2 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNPower3 = p_modbus_manager->gen_info_struct[i].u32LNPower3 * 100 / 256;
              
              p_mib->genGroup.genTable[i].genStatusLNSPower1 = p_modbus_manager->gen_info_struct[i].u32LNSPower1 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNSPower2 = p_modbus_manager->gen_info_struct[i].u32LNSPower2 * 100 / 256;
              p_mib->genGroup.genTable[i].genStatusLNSPower3 = p_modbus_manager->gen_info_struct[i].u32LNSPower3 * 100 / 256;
              
              p_mib->genGroup.genTable[i].genStatusFrequency = p_modbus_manager->gen_info_struct[i].u32GenFrequency *10 / 256;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = p_modbus_manager->gen_info_struct[i].u32EngineSpeed;
              p_mib->genGroup.genTable[i].genStatusOilPressure = p_modbus_manager->gen_info_struct[i].u32OilPressure * 10 / 256;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp * 10 / 256;
              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel / 256;
              p_mib->genGroup.genTable[i].genStatusBattVolt = p_modbus_manager->gen_info_struct[i].u32BattVolt * 10 / 256;
              
              p_mib->genGroup.genTable[i].genStatusWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour;
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = p_modbus_manager->gen_info_struct[i].u32AlarmStatus;
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
          }
          break;
          case 7:// CUMMIN
          {
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = p_modbus_manager->gen_info_struct[i].u32LNVolt1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = p_modbus_manager->gen_info_struct[i].u32LNVolt2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = p_modbus_manager->gen_info_struct[i].u32LNVolt3 * 100;
//              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = p_modbus_manager->gen_info_struct[i].u32LNCurr1 * 10;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = p_modbus_manager->gen_info_struct[i].u32LNCurr2 * 10;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = p_modbus_manager->gen_info_struct[i].u32LNCurr3 * 10;
//              
//              p_mib->genGroup.genTable[i].genStatusLNPower1 = p_modbus_manager->gen_info_struct[i].u32LNPower1 * 100 / 256;
//              p_mib->genGroup.genTable[i].genStatusLNPower2 = p_modbus_manager->gen_info_struct[i].u32LNPower2 * 100 / 256;
//              p_mib->genGroup.genTable[i].genStatusLNPower3 = p_modbus_manager->gen_info_struct[i].u32LNPower3 * 100 / 256;
//  
              p_mib->genGroup.genTable[i].genStatusLNSPower1 = p_modbus_manager->gen_info_struct[i].u32LNSPower1 * 100;
              p_mib->genGroup.genTable[i].genStatusLNSPower2 = p_modbus_manager->gen_info_struct[i].u32LNSPower2 * 100;
              p_mib->genGroup.genTable[i].genStatusLNSPower3 = p_modbus_manager->gen_info_struct[i].u32LNSPower3 * 100;
              
//              p_mib->genGroup.genTable[i].genStatusFrequency = p_modbus_manager->gen_info_struct[i].u32GenFrequency *10 / 256;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = p_modbus_manager->gen_info_struct[i].u32EngineSpeed;
              p_mib->genGroup.genTable[i].genStatusOilPressure = (p_modbus_manager->gen_info_struct[i].u32OilPressure * 10)/100;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = p_modbus_manager->gen_info_struct[i].u32CoolantTemp;
//              p_mib->genGroup.genTable[i].genStatusFuelLevel = p_modbus_manager->gen_info_struct[i].u32FuelLevel / 256;
              p_mib->genGroup.genTable[i].genStatusBattVolt = p_modbus_manager->gen_info_struct[i].u32BattVolt;//thanhcm3 fix
//          
              p_mib->genGroup.genTable[i].genStatusWorkingHour = p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour/3600;
              // thanhcm3 fix ----------------------------------------------------------------------------
              p_mib->genGroup.genTable[i].genStatusWorkingMin  = (p_modbus_manager->gen_info_struct[i].u32EngineWorkingHour%3600)/60;
              p_mib->genGroup.genTable[i].genStatusFrequency   = p_modbus_manager->gen_info_struct[i].u32GenFrequency;
              // thanhcm3 fix ----------------------------------------------------------------------------
              
              if(p_modbus_manager->gen_info_struct[i].u32AlarmStatus == 1448)
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_UnderFreq = 1;
              else
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_UnderFreq = 0;
              
              if((p_modbus_manager->gen_info_struct[i].u32AlarmStatus == 146)||(p_modbus_manager->gen_info_struct[i].u32AlarmStatus == 151))
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_HiCoolantTemp = 1;
              else
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_HiCoolantTemp = 0;
              
              if((p_modbus_manager->gen_info_struct[i].u32AlarmStatus == 143)||(p_modbus_manager->gen_info_struct[i].u32AlarmStatus == 415))
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowOilPressure = 1;
              else
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowOilPressure = 0;
              
              if(p_modbus_manager->gen_info_struct[i].u32AlarmStatus == 442)
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_HiBattVol = 1;
              else
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_HiBattVol = 0;
              
              if(p_modbus_manager->gen_info_struct[i].u32AlarmStatus == 441)
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowBattVol = 1;
              else
                p_mib->genGroup.genTable[i].genStatusAlarmStatus.bit.err_LowBattVol = 0;
              
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
          }
          break;
          };
      }
      else
      {
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              p_mib->genGroup.genTable[i].genStatusFrequency = 0;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = 0;
              p_mib->genGroup.genTable[i].genStatusOilPressure = 0;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = 0;
              p_mib->genGroup.genTable[i].genStatusFuelLevel = 0;
              p_mib->genGroup.genTable[i].genStatusBattVolt = 0;
              
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = 0;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = 0;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = 0;
              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = 0;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = 0;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = 0;
              
              p_mib->genGroup.genTable[i].genStatusLNPower1 = 0;
              p_mib->genGroup.genTable[i].genStatusLNPower2 = 0;
              p_mib->genGroup.genTable[i].genStatusLNPower3 = 0;
              
              p_mib->genGroup.genTable[i].genStatusWorkingHour = 0;
              p_mib->genGroup.genTable[i].genStatusWorkingMin = 0;
              p_mib->genGroup.genTable[i].genStatusActiveEnergyLow = 0;
              p_mib->genGroup.genTable[i].genStatusActiveEnergyHigh = 0;
              
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = 0;
              
              p_mib->genGroup.genTable[i].genStopMode = 0;
              p_mib->genGroup.genTable[i].genAutoMode = 0;
              p_mib->genGroup.genTable[i].genManualMode = 0;
              p_mib->genGroup.genTable[i].genStartMode = 0;
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
      }
  }
  //clean gen---------------------------------------------------------------
  for(i= p_mib->genGroup.genInstalledGen;i<1;i++){
              p_mib->genGroup.genTable[i].genStatusStatus = 0;
                
              p_mib->genGroup.genTable[i].genStatusID = i+1;
              p_mib->genGroup.genTable[i].genStatusFrequency = 0;
              p_mib->genGroup.genTable[i].genStatusEngineSpeed = 0;
              p_mib->genGroup.genTable[i].genStatusOilPressure = 0;
              p_mib->genGroup.genTable[i].genStatusCoolantTemp = 0;
              p_mib->genGroup.genTable[i].genStatusFuelLevel = 0;
              p_mib->genGroup.genTable[i].genStatusBattVolt = 0;
              
              p_mib->genGroup.genTable[i].genStatusLNVolt1 = 0;
              p_mib->genGroup.genTable[i].genStatusLNVolt2 = 0;
              p_mib->genGroup.genTable[i].genStatusLNVolt3 = 0;
              
              p_mib->genGroup.genTable[i].genStatusLNCurr1 = 0;
              p_mib->genGroup.genTable[i].genStatusLNCurr2 = 0;
              p_mib->genGroup.genTable[i].genStatusLNCurr3 = 0;
              
              p_mib->genGroup.genTable[i].genStatusLNPower1 = 0;
              p_mib->genGroup.genTable[i].genStatusLNPower2 = 0;
              p_mib->genGroup.genTable[i].genStatusLNPower3 = 0;
              
              p_mib->genGroup.genTable[i].genStatusWorkingHour = 0;
              p_mib->genGroup.genTable[i].genStatusWorkingMin = 0;
              p_mib->genGroup.genTable[i].genStatusActiveEnergyLow = 0;
              p_mib->genGroup.genTable[i].genStatusActiveEnergyHigh = 0;
              
              p_mib->genGroup.genTable[i].genStatusAlarmStatus.all = 0;
              
              p_mib->genGroup.genTable[i].genStopMode = 0;
              p_mib->genGroup.genTable[i].genAutoMode = 0;
              p_mib->genGroup.genTable[i].genManualMode = 0;
              p_mib->genGroup.genTable[i].genStartMode = 0;
              
              p_mib->genGroup.genTable[i].genStatusType = p_modbus_manager->gen_manager_struct[i].gen_type;
              p_mib->genGroup.genTable[i].genStatusModbusID = p_modbus_manager->gen_manager_struct[i].gen_abs_slave_id;
  }

  
}

//update pm info--------------------------------------------------------------
uint8_t cntOutFrq, cntInFrq;
void update_pm_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  uint32_t i=0;
  
  p_mib->pmGroup.pmInstalledPM = p_modbus_manager->number_of_pm;
  p_mib->connect_alarm_group_struct.alarm_pm_connect = 0;
  p_mib->main_alarm_group_struct.alarm_pm = 0;
  p_mib->pmGroup.pmActivePM = p_mib->pmGroup.pmInstalledPM;
  for (i = 0; i < p_mib->pmGroup.pmInstalledPM; i++)
  {
    if (p_modbus_manager->pm_info_struct[i].u8ErrorFlag == 1)
    {
      if(p_modbus_manager->cnt_dis_connect_pm[i] > 2){ // mat ket noi 3 lan moi bao len
        p_modbus_manager->cnt_dis_connect_pm[i] = 3;
        p_mib->pmGroup.pmActivePM--;
        if (p_mib->pmGroup.pmActivePM <= 0)
        {
          p_mib->pmGroup.pmActivePM = 0;
        }
        p_mib->connect_alarm_group_struct.alarm_pm_connect |= (p_modbus_manager->pm_info_struct[i].u8ErrorFlag << i);
        p_mib->pmGroup.pmTable[i].pmStatus = 170;
      }
    }
    else
    {
      p_modbus_manager->cnt_dis_connect_pm[i] = 0;
      p_mib->pmGroup.pmTable[i].pmStatus = 255;
      //            if(p_mib->pmGroup.pmTable[i].liBattWarningFlag != 0)
      //            {
      //                p_mib->pmGroup.pmTable[i].pmStatusStatus = 165;
      //            }
      //            else
      //            {
      //                p_mib->pmGroup.pmTable[i].pmStatusStatus = 255;
      //            }
    }
    
    if (p_mib->pmGroup.pmTable[i].pmStatus != 170)
    {
      if (p_mib->pmGroup.pmTable[i].pmStatus == 165)
      {
        p_mib->main_alarm_group_struct.alarm_pm = 1;
      }
      
      switch(p_modbus_manager->pm_manager_struct[i].pm_type)
      {
      case 1:// FINECO
        {
          p_mib->pmGroup.pmTable[i].pmID = i+1;
          p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportActiveE * 100);
          p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportActiveE * 100);
          p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportReActiveE * 100) ;
          p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportReActiveE * 1000) ;              
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE * 1000) ;             
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE * 1000) ;             
          p_mib->pmGroup.pmTable[i].pmPowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor * 100) ; 
          p_mib->pmGroup.pmTable[i].pmFrequency = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fFrequency * 10) ;
          p_mib->pmGroup.pmTable[i].pmTotalCurrent = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent*100);
          p_mib->pmGroup.pmTable[i].pmActivePower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower*1000);
          p_mib->pmGroup.pmTable[i].pmReactivePower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower*1000);
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent * 100);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage * 100);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower * 1000);
          p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower * 1000);              
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower * 1000);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor * 100);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent * 100);
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fVoltage * 100);
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fActivePower * 1000);
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fApparentPower * 1000);              
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower * 1000);
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fPowerFactor * 100);
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent * 100);
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fVoltage * 100);
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fActivePower * 1000);
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fApparentPower * 1000);              
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower * 1000);
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fPowerFactor * 100);
          
          p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
          p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        }
        break;
      case 2:// ASCENT
        {
          p_mib->pmGroup.pmTable[i].pmID = i+1;
          p_mib->pmGroup.pmTable[i].pmFrequency = (uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency * 10) ;
          
          p_mib->pmGroup.pmTable[i].pmPowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor*100) ; 
          p_mib->pmGroup.pmTable[i].pmPhase1Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent * 100);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage * 100);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower/10);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor * 100);
          p_mib->pmGroup.pmTable[i].pmTotalCurrent = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent*100);
          p_mib->pmGroup.pmTable[i].pmActivePower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower/10);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = 0;
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = 0;
          
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].fRealEnergy *100);
          
          p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
          p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        }
        break;
      case 3:// EASTRON
        {
          p_mib->pmGroup.pmTable[i].pmID = i+1;
          
          p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportActiveE);
          p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportActiveE);
          p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportReActiveE) ;
          p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportReActiveE) ;              
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmPowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor*100) ; 
          p_mib->pmGroup.pmTable[i].pmFrequency = (uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency*10);
          p_mib->pmGroup.pmTable[i].pmTotalCurrent = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent*100);
          p_mib->pmGroup.pmTable[i].pmActivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower/10);
          p_mib->pmGroup.pmTable[i].pmReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower/10);
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent*100);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage*100);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower/10);
          p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower/10);              
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower/10);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor*100);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = 0;
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = 0;
          
          sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0],"%d",p_modbus_manager->pm_info_struct[i].u32SerialNumber);
          p_mib->pmGroup.pmTable[i].pmSerialLen = 8;
          sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"SDM220");
          p_mib->pmGroup.pmTable[i].pmModelLen = 6;
          p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
          p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
          
        }
        break;
      case 4:// CET1
        {
          p_mib->pmGroup.pmTable[i].pmID = i+1;
          p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportActiveE);
          p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportActiveE);
          p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportReActiveE) ;
          p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportReActiveE) ;              
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmPowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor * 100) ; 
          p_mib->pmGroup.pmTable[i].pmFrequency = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fFrequency * 10) ;
          p_mib->pmGroup.pmTable[i].pmTotalCurrent = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent*100);
          p_mib->pmGroup.pmTable[i].pmActivePower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower*100);
          p_mib->pmGroup.pmTable[i].pmReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower*100);
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent * 100);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage * 100);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower *100);
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower *100);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor * 100);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = 0;
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = 0;
          
          sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"%s",&p_modbus_manager->pm_info_struct[i].u8Model[0]);
          p_mib->pmGroup.pmTable[i].pmModelLen = 20;
          sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0],"%d",p_modbus_manager->pm_info_struct[i].u32SerialNumber);
          p_mib->pmGroup.pmTable[i].pmSerialLen = 20;
          
          p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
          p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        }
        break;
      case 5://PILOT
        {
          p_mib->pmGroup.pmTable[i].pmID = i+1;
          p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportActiveE*10);
          p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportActiveE*10);
          p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportReActiveE*10) ;
          p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportReActiveE*10) ;              
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE*10) ;             
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE*10) ;             
          p_mib->pmGroup.pmTable[i].pmPowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor*0.1) ; 
          p_mib->pmGroup.pmTable[i].pmFrequency = (uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency*0.1);
          p_mib->pmGroup.pmTable[i].pmTotalCurrent = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent*0.1);
          p_mib->pmGroup.pmTable[i].pmActivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower/100);
          p_mib->pmGroup.pmTable[i].pmReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower/100);
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent*0.1);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower/100);
          p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower/100);              
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower/100);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor/10);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = 0;
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = 0;              
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = 0;
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = 0;
          
          p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
          sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"SPM91");
          p_mib->pmGroup.pmTable[i].pmModelLen = 5;
          sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0]," ");
          p_mib->pmGroup.pmTable[0].pmSerialLen = 1;
          p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        }
        break;
        //          case 6://FORLONG
        //          {
        //              p_mib->pmGroup.pmTable[i].pmID = i+1;
        //              
        //              p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = 0;
        //              p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = 0;
        //              p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = 0 ;
        //              p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = 0 ;              
        //              p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE) ;             
        //              p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE) ;             
        //              p_mib->pmGroup.pmTable[i].pmPowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor/10) ; 
        //              p_mib->pmGroup.pmTable[i].pmFrequency = (uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency/10);
        //              p_mib->pmGroup.pmTable[i].pmTotalCurrent = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent);
        //              p_mib->pmGroup.pmTable[i].pmActivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower*10);
        //              p_mib->pmGroup.pmTable[i].pmReactivePower = 0;
        //
        //              p_mib->pmGroup.pmTable[i].pmPhase1Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent);
        //              p_mib->pmGroup.pmTable[i].pmPhase1Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage*10);
        //              p_mib->pmGroup.pmTable[i].pmPhase1RealPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower*10);
        //              p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower = 0;              
        //              p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor/10);
        //              
        //              p_mib->pmGroup.pmTable[i].pmPhase2Current = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase2Voltage = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase2RealPower = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = 0;              
        //              p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = 0;
        //              
        //              p_mib->pmGroup.pmTable[i].pmPhase3Current = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase3Voltage = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase3RealPower = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = 0;              
        //              p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = 0;
        //              p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = 0;
        //              
        //              p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
        //              sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"DRS-205C");
        //              p_mib->pmGroup.pmTable[i].pmModelLen = 8;
        ////              sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0]," ");
        ////              p_mib->pmGroup.pmTable[0].pmSerialLen = 1;
        //              p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
        //              
        //              if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
        //              {
        //                if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
        //                    (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
        //                      cntInFrq = 0;
        //                      if(cntOutFrq++ > 10){
        //                        cntOutFrq = 11;
        //                        p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
        //                      }
        //                    }
        //                else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
        //                         (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
        //                           cntOutFrq = 0;
        //                           if(cntInFrq++ > 2){
        //                              cntInFrq = 3;
        //                              p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
        //                           }
        //                         }
        //              }
        //              else {
        //                cntOutFrq = 0;
        //                cntInFrq  = 0;
        //                p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
        //              }
        //              
        //              p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        //          }
        //          break;
      case 15://PILOT_3PHASE
        {
          p_mib->pmGroup.pmTable[i].pmID = i+1;
          p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportActiveE);
          p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportActiveE);
          p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportReActiveE) ;
          p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportReActiveE) ;              
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmPowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].fPowerFactor) ; 
          p_mib->pmGroup.pmTable[i].pmFrequency = (uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency);
          p_mib->pmGroup.pmTable[i].pmTotalCurrent = (uint32_t) (p_modbus_manager->pm_info_struct[i].fTotalCurrent);
          p_mib->pmGroup.pmTable[i].pmActivePower = (uint32_t) p_modbus_manager->pm_info_struct[i].fRealPower;
          p_mib->pmGroup.pmTable[i].pmReactivePower = (uint32_t) p_modbus_manager->pm_info_struct[i].fReactivePower;
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower);              
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fApparentPower);              
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fPowerFactor);
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fApparentPower);              
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fPowerFactor);
          
          
          
          p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
          sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"SPM93");
          p_mib->pmGroup.pmTable[i].pmModelLen = 5;
          sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0],"%d",p_modbus_manager->pm_info_struct[i].u32SerialNumber);
          p_mib->pmGroup.pmTable[i].pmSerialLen = 20;
          p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        }
        break;
      case 16: //YADA_3PHASE_DPC
        {
          p_mib->pmGroup.pmTable[i].pmID = i+1;
          p_mib->pmGroup.pmTable[i].pmImportActiveEnergy     = 0; //(uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportActiveE);
          p_mib->pmGroup.pmTable[i].pmExportActiveEnergy     = 0; //(uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportActiveE);
          p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy   = 0; //(uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportReActiveE) ;
          p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy   = 0; //(uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportReActiveE) ;              
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy      = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy    = (uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE) ;             
          p_mib->pmGroup.pmTable[i].pmPowerFactor            = (uint32_t) (p_modbus_manager->pm_info_struct[i].fPowerFactor) ; 
          p_mib->pmGroup.pmTable[i].pmFrequency              = (uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency);
          p_mib->pmGroup.pmTable[i].pmTotalCurrent           = (uint32_t) (p_modbus_manager->pm_info_struct[i].fTotalCurrent);
          p_mib->pmGroup.pmTable[i].pmActivePower            = (uint32_t) p_modbus_manager->pm_info_struct[i].fRealPower;
          p_mib->pmGroup.pmTable[i].pmReactivePower          = (int32_t) p_modbus_manager->pm_info_struct[i].fReactivePower;
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current          = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage          = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower        = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower    = 0; // (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower);              
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower    = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor      = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current          = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage          = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower        = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower    = 0; //(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fApparentPower);              
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower    = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor      = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fPowerFactor);
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current          = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage          = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower        = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower    = 0; //(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fApparentPower);              
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower    = (int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor      = (uint32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fPowerFactor);
          
          p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
          sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"YADA");
          p_mib->pmGroup.pmTable[i].pmModelLen = 4;
          p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
          
        }
        break;
      case 17: //Schneider 2022
        {
          p_mib->pmGroup.pmTable[i].pmID                        = i+1;
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy         =(uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE);
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy       =(uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE);
          p_mib->pmGroup.pmTable[i].pmPowerFactor               =(uint32_t) (p_modbus_manager->pm_info_struct[i].fPowerFactor);
          p_mib->pmGroup.pmTable[i].pmFrequency                 =(uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency); 
          p_mib->pmGroup.pmTable[i].pmActivePower               =(uint32_t) (p_modbus_manager->pm_info_struct[i].fRealPower);
          p_mib->pmGroup.pmTable[i].pmReactivePower             =(uint32_t) (p_modbus_manager->pm_info_struct[i].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmTotalCurrent              =(uint32_t) (p_modbus_manager->pm_info_struct[i].fTotalCurrent);
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower           =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower);
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor         =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower           =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fApparentPower);
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor         =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower);
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower           =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fApparentPower);
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor         =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower);
          
          p_mib->pmGroup.pmTable[i].pmType                      =p_modbus_manager->pm_manager_struct[i].pm_type;
          p_mib->pmGroup.pmTable[i].pmModbusID                  =p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0],"%d%s%d",p_modbus_manager->pm_info_struct[i].id_SerialNumber,"0",p_modbus_manager->pm_info_struct[i].u32SerialNumber);
          p_mib->pmGroup.pmTable[i].pmSerialLen = 12;
          sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"%s","PM2230");
          p_mib->pmGroup.pmTable[i].pmModelLen = 6;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        }
        break;
      case 18: //EASTRON SMD72D 2022
        {
          p_mib->pmGroup.pmTable[i].pmImportActiveEnergy        =(uint32_t) (p_modbus_manager->pm_info_struct[i].u32ImportActiveE);
          p_mib->pmGroup.pmTable[i].pmExportActiveEnergy        =(uint32_t) (p_modbus_manager->pm_info_struct[i].u32ExportActiveE);
          p_mib->pmGroup.pmTable[i].pmID                        = i+1;
          p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy         =(uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalActiveE);
          p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy       =(uint32_t) (p_modbus_manager->pm_info_struct[i].u32TotalReActiveE);
          p_mib->pmGroup.pmTable[i].pmPowerFactor               =(uint32_t) (p_modbus_manager->pm_info_struct[i].fPowerFactor);
          p_mib->pmGroup.pmTable[i].pmFrequency                 =(uint32_t) (p_modbus_manager->pm_info_struct[i].fFrequency); 
          p_mib->pmGroup.pmTable[i].pmActivePower               =(uint32_t) (p_modbus_manager->pm_info_struct[i].fRealPower);
          p_mib->pmGroup.pmTable[i].pmReactivePower             =(uint32_t) (p_modbus_manager->pm_info_struct[i].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmTotalCurrent              =(uint32_t) (p_modbus_manager->pm_info_struct[i].fTotalCurrent);
          
          p_mib->pmGroup.pmTable[i].pmPhase1Current             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase1Voltage             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase1RealPower           =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fApparentPower);
          p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor         =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[0].fPowerFactor);
          
          p_mib->pmGroup.pmTable[i].pmPhase2Current             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase2Voltage             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase2RealPower           =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fApparentPower);
          p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor         =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[1].fReactivePower);
          
          p_mib->pmGroup.pmTable[i].pmPhase3Current             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fCurrent);
          p_mib->pmGroup.pmTable[i].pmPhase3Voltage             =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fVoltage);
          p_mib->pmGroup.pmTable[i].pmPhase3RealPower           =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fActivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fApparentPower);
          p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower       =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower);
          p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor         =(int32_t) (p_modbus_manager->pm_info_struct[i].sPhaseInfo[2].fReactivePower);
          
          p_mib->pmGroup.pmTable[i].pmType                      =p_modbus_manager->pm_manager_struct[i].pm_type;
          p_mib->pmGroup.pmTable[i].pmModbusID                  =p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
          
          sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0],"%d",p_modbus_manager->pm_info_struct[i].u32SerialNumber);
          p_mib->pmGroup.pmTable[i].pmSerialLen = 8;
          sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0],"%s","SDM72D");
          p_mib->pmGroup.pmTable[i].pmModelLen = 6;
          
          if((p_mib->pmGroup.pmTable[i].pmFrequency != 0) && (p_mib->pmGroup.pmTable[i].pmEnableFreqTrap == 1))
          {
            if ((p_mib->pmGroup.pmTable[i].pmFrequency >= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect)) || 
                (p_mib->pmGroup.pmTable[i].pmFrequency <= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqDisConnect))){
                  cntInFrq = 0;
                  if(cntOutFrq++ > 10){
                    cntOutFrq = 11;
                    p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->pmGroup.pmTable[i].pmFrequency <= (500 + p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect)) && 
                     (p_mib->pmGroup.pmTable[i].pmFrequency >= (500 - p_mib->pmGroup.pmTable[i].pmDeltaFreqReConnect))){
                       cntOutFrq = 0;
                       if(cntInFrq++ > 10){
                         cntInFrq = 11;
                         p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq = 0;
            cntInFrq  = 0;
            p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = p_mib->pmGroup.pmTable[i].pmOutOfRangeFreq;
        }
        break;
      default:
        break;
      };
    }
    else
    {
      p_mib->pmGroup.pmTable[i].pmID = i+1;
      p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = 0;
      p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = 0;
      p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = 0;
      p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = 0;            
      p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = 0;          
      p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = 0;          
      p_mib->pmGroup.pmTable[i].pmPowerFactor = 0;
      p_mib->pmGroup.pmTable[i].pmFrequency = 0;
      p_mib->pmGroup.pmTable[i].pmTotalCurrent = 0;
      p_mib->pmGroup.pmTable[i].pmActivePower = 0;
      p_mib->pmGroup.pmTable[i].pmReactivePower =0;
      
      p_mib->pmGroup.pmTable[i].pmPhase1Current = 0;
      p_mib->pmGroup.pmTable[i].pmPhase1Voltage = 0;
      p_mib->pmGroup.pmTable[i].pmPhase1RealPower = 0;
      p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower = 0;              
      p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = 0;
      p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = 0;
      
      p_mib->pmGroup.pmTable[i].pmPhase2Current = 0;
      p_mib->pmGroup.pmTable[i].pmPhase2Voltage = 0;
      p_mib->pmGroup.pmTable[i].pmPhase2RealPower = 0;
      p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = 0;              
      p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = 0;
      p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = 0;
      
      p_mib->pmGroup.pmTable[i].pmPhase3Current = 0;
      p_mib->pmGroup.pmTable[i].pmPhase3Voltage = 0;
      p_mib->pmGroup.pmTable[i].pmPhase3RealPower = 0;
      p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = 0;              
      p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = 0;
      p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = 0;
      
      sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0]," ");
      p_mib->pmGroup.pmTable[0].pmSerialLen = 1;
      p_mib->pmGroup.pmTable[0].pmModelLen = 1;
      sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0]," ");
      
      p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = 0;
      
      p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
      p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
    }
  }
  //clear------------------------------------------------------------------------------------------------------
  for(i = p_mib->pmGroup.pmInstalledPM;i<1;i++){
    p_mib->pmGroup.pmTable[i].pmStatus =0;
    
    p_mib->pmGroup.pmTable[i].pmID = i+1;
    p_mib->pmGroup.pmTable[i].pmImportActiveEnergy = 0;
    p_mib->pmGroup.pmTable[i].pmExportActiveEnergy = 0;
    p_mib->pmGroup.pmTable[i].pmImportReactiveEnergy = 0;
    p_mib->pmGroup.pmTable[i].pmExportReactiveEnergy = 0;            
    p_mib->pmGroup.pmTable[i].pmTotalActiveEnergy = 0;          
    p_mib->pmGroup.pmTable[i].pmTotalReactiveEnergy = 0;          
    p_mib->pmGroup.pmTable[i].pmPowerFactor = 0;
    p_mib->pmGroup.pmTable[i].pmFrequency = 0;
    p_mib->pmGroup.pmTable[i].pmTotalCurrent = 0;
    p_mib->pmGroup.pmTable[i].pmActivePower = 0;
    p_mib->pmGroup.pmTable[i].pmReactivePower =0;
    
    p_mib->pmGroup.pmTable[i].pmPhase1Current = 0;
    p_mib->pmGroup.pmTable[i].pmPhase1Voltage = 0;
    p_mib->pmGroup.pmTable[i].pmPhase1RealPower = 0;
    p_mib->pmGroup.pmTable[i].pmPhase1ApparentPower = 0;              
    p_mib->pmGroup.pmTable[i].pmPhase1ReactivePower = 0;
    p_mib->pmGroup.pmTable[i].pmPhase1PowerFactor = 0;
    
    p_mib->pmGroup.pmTable[i].pmPhase2Current = 0;
    p_mib->pmGroup.pmTable[i].pmPhase2Voltage = 0;
    p_mib->pmGroup.pmTable[i].pmPhase2RealPower = 0;
    p_mib->pmGroup.pmTable[i].pmPhase2ApparentPower = 0;              
    p_mib->pmGroup.pmTable[i].pmPhase2ReactivePower = 0;
    p_mib->pmGroup.pmTable[i].pmPhase2PowerFactor = 0;
    
    p_mib->pmGroup.pmTable[i].pmPhase3Current = 0;
    p_mib->pmGroup.pmTable[i].pmPhase3Voltage = 0;
    p_mib->pmGroup.pmTable[i].pmPhase3RealPower = 0;
    p_mib->pmGroup.pmTable[i].pmPhase3ApparentPower = 0;              
    p_mib->pmGroup.pmTable[i].pmPhase3ReactivePower = 0;
    p_mib->pmGroup.pmTable[i].pmPhase3PowerFactor = 0;
    
    sprintf(&p_mib->pmGroup.pmTable[i].pmModel[0]," ");
    p_mib->pmGroup.pmTable[0].pmSerialLen = 1;
    p_mib->pmGroup.pmTable[0].pmModelLen = 1;
    sprintf(&p_mib->pmGroup.pmTable[i].pmSerial[0]," ");
    
    p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = 0;
    
    p_mib->pmGroup.pmTable[i].pmType = p_modbus_manager->pm_manager_struct[i].pm_type;
    p_mib->pmGroup.pmTable[i].pmModbusID = p_modbus_manager->pm_manager_struct[i].pm_abs_slave_id;
    
  }
  
}

//update smbc info-----------------------------------------------------------
void update_smbc_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint32_t i=0;
  
  p_mib->smcbGroup.SmcbInstalledSMCB = p_modbus_manager->number_of_smcb;
  p_mib->connect_alarm_group_struct.alarm_smcb_connect = 0;
  p_mib->main_alarm_group_struct.alarm_smcb = 0;
  p_mib->smcbGroup.SmcbActiveSMCB = p_mib->smcbGroup.SmcbInstalledSMCB;
  for (i = 0; i < p_mib->smcbGroup.SmcbInstalledSMCB; i++)
  {
    p_mib->main_alarm_group_struct.alarm_smcb_pack[i] = 0;
    if (p_modbus_manager->smcb_info_struct[i].u8ErrorFlag == 1)
    {
      if(p_modbus_manager->cnt_dis_connect_smbc[i] > 2){ // mat ket noi 3 lan moi bao len
        p_modbus_manager->cnt_dis_connect_smbc[i] = 3;
        p_mib->smcbGroup.SmcbActiveSMCB--;
        if (p_mib->smcbGroup.SmcbActiveSMCB <= 0)
        {
          p_mib->smcbGroup.SmcbActiveSMCB = 0;
        }
        p_mib->connect_alarm_group_struct.alarm_smcb_connect |= (p_modbus_manager->smcb_info_struct[i].u8ErrorFlag << i);
        p_mib->smcbGroup.SmcbTable[i].SmcbStatus = 170;
      }
    }
    else
    {        
      p_modbus_manager->cnt_dis_connect_smbc[i] = 0;
      p_mib->smcbGroup.SmcbTable[i].SmcbStatus = 255;
    }
    
    if (p_mib->smcbGroup.SmcbTable[i].SmcbStatus != 170)
    {
      switch(p_modbus_manager->smcb_manager_struct[i].smcb_type)
      {
      case 1:// OPEN
        {
          p_mib->smcbGroup.SmcbTable[i].SmcbStatusID = i+1;
          p_mib->smcbGroup.SmcbTable[i].SmcbType = p_modbus_manager->smcb_manager_struct[i].smcb_type;
          if(p_modbus_manager->setting_cmd != SET_SMCB_STATE){
            p_mib->smcbGroup.SmcbTable[i].SmcbState = p_modbus_manager->smcb_info_struct[i].u32State;
          }
          p_mib->smcbGroup.SmcbTable[i].SmcbModbusID = p_modbus_manager->smcb_manager_struct[i].smcb_abs_slave_id;
          p_mib->main_alarm_group_struct.alarm_smcb_pack[i] = p_mib->smcbGroup.SmcbTable[i].SmcbState;
        }
        break;
      case 2:// MATIS
        {
          p_mib->smcbGroup.SmcbTable[i].SmcbStatusID = i+1;
          p_mib->smcbGroup.SmcbTable[i].SmcbType = p_modbus_manager->smcb_manager_struct[i].smcb_type;
          if(p_modbus_manager->setting_cmd != SET_SMCB_STATE){
            if((p_modbus_manager->smcb_info_struct[i].u32State == 1)||(p_modbus_manager->smcb_info_struct[i].u32State == 2)||(p_modbus_manager->smcb_info_struct[i].u32State == 20))
              p_mib->smcbGroup.SmcbTable[i].SmcbState = 0;
            else if((p_modbus_manager->smcb_info_struct[i].u32State == 10)||(p_modbus_manager->smcb_info_struct[i].u32State == 11)||(p_modbus_manager->smcb_info_struct[i].u32State == 30))
              p_mib->smcbGroup.SmcbTable[i].SmcbState = 1;
            else
              p_mib->smcbGroup.SmcbTable[i].SmcbState = 2;
          }
          p_mib->smcbGroup.SmcbTable[i].SmcbModbusID = p_modbus_manager->smcb_manager_struct[i].smcb_abs_slave_id;
          p_mib->main_alarm_group_struct.alarm_smcb_pack[i] = p_mib->smcbGroup.SmcbTable[i].SmcbState;
        }
        break;
        
      case 3: //GOL
        p_mib->smcbGroup.SmcbTable[i].SmcbStatusID = i+1;
        p_mib->smcbGroup.SmcbTable[i].SmcbType = p_modbus_manager->smcb_manager_struct[i].smcb_type;
        if(p_modbus_manager->setting_cmd != SET_SMCB_STATE){
            if(p_modbus_manager->smcb_info_struct[i].u32State == 1)
              p_mib->smcbGroup.SmcbTable[i].SmcbState = 0;
            else if(p_modbus_manager->smcb_info_struct[i].u32State == 2)
              p_mib->smcbGroup.SmcbTable[i].SmcbState = 1;
            else
              p_mib->smcbGroup.SmcbTable[i].SmcbState = 2;
          }
          p_mib->smcbGroup.SmcbTable[i].SmcbModbusID = p_modbus_manager->smcb_manager_struct[i].smcb_abs_slave_id;
          p_mib->main_alarm_group_struct.alarm_smcb_pack[i] = p_mib->smcbGroup.SmcbTable[i].SmcbState;
      };
    }
    else
    {
      p_mib->smcbGroup.SmcbTable[i].SmcbStatus = 170;   
      p_mib->smcbGroup.SmcbTable[i].SmcbType = p_modbus_manager->smcb_manager_struct[i].smcb_type;
      p_mib->smcbGroup.SmcbTable[i].SmcbState = 0;
      p_mib->smcbGroup.SmcbTable[i].SmcbModbusID = p_modbus_manager->smcb_manager_struct[i].smcb_abs_slave_id;    
    }
  }
}
  
//update fuel info-----------------------------------------------------------
void update_fuel_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint32_t i=0;
  
  p_mib->fuelGroup.FuelInstalledFUEL = p_modbus_manager->number_of_fuel;
  p_mib->connect_alarm_group_struct.alarm_fuel_connect = 0;
  p_mib->main_alarm_group_struct.alarm_fuel = 0;
  p_mib->fuelGroup.FuelActiveFUEL = p_mib->fuelGroup.FuelInstalledFUEL;
  for (i = 0; i < p_mib->fuelGroup.FuelInstalledFUEL; i++)
  {
    p_mib->main_alarm_group_struct.alarm_fuel_pack[i] = 0;
    if (p_modbus_manager->fuel_info_struct[i].u8ErrorFlag == 1)
    {
      if(p_modbus_manager->cnt_dis_connect_fuel[i] > 2){ // mat ket noi 3 lan moi bao len
        p_modbus_manager->cnt_dis_connect_fuel[i] = 3;
        p_mib->fuelGroup.FuelActiveFUEL--;
        if (p_mib->fuelGroup.FuelActiveFUEL <= 0)
        {
          p_mib->fuelGroup.FuelActiveFUEL = 0;
        }
        p_mib->connect_alarm_group_struct.alarm_fuel_connect |= (p_modbus_manager->fuel_info_struct[i].u8ErrorFlag << i);
        p_mib->fuelGroup.FuelTable[i].FuelStatus = 170;
      }
    }
    else
    {        
      p_modbus_manager->cnt_dis_connect_fuel[i] = 0;
      p_mib->fuelGroup.FuelTable[i].FuelStatus = 255;
    }
    
    if (p_mib->fuelGroup.FuelTable[i].FuelStatus != 170)
    {
      switch(p_modbus_manager->fuel_manager_struct[i].fuel_type)
      {
      case 1:// HPT621
        {
          p_mib->fuelGroup.FuelTable[i].FuelStatusID = i+1;
          p_mib->fuelGroup.FuelTable[i].FuelType = p_modbus_manager->fuel_manager_struct[i].fuel_type;
          p_mib->fuelGroup.FuelTable[i].FuelLevel = p_modbus_manager->fuel_info_struct[i].u32FuelLevel;
          p_mib->fuelGroup.FuelTable[i].FuelModbusID = p_modbus_manager->fuel_manager_struct[i].fuel_abs_slave_id;
        }
        break;
      };
    }
    else
    {
      p_mib->fuelGroup.FuelTable[i].FuelStatus = 170;   
      p_mib->fuelGroup.FuelTable[i].FuelType = p_modbus_manager->fuel_manager_struct[i].fuel_type;
      p_mib->fuelGroup.FuelTable[i].FuelModbusID = p_modbus_manager->fuel_manager_struct[i].fuel_abs_slave_id;    
      p_mib->fuelGroup.FuelTable[i].FuelLevel = 0;
    }
    
  }
  //clean-------------------------------------------------------------------------------------------------------
  for(i = p_mib->fuelGroup.FuelInstalledFUEL; i<2;i++){
    p_mib->fuelGroup.FuelTable[i].FuelStatus = 0;   
    p_mib->fuelGroup.FuelTable[i].FuelType = p_modbus_manager->fuel_manager_struct[i].fuel_type;
    p_mib->fuelGroup.FuelTable[i].FuelModbusID = p_modbus_manager->fuel_manager_struct[i].fuel_abs_slave_id;    
    p_mib->fuelGroup.FuelTable[i].FuelLevel = 0; 
  }
  
  
}

//update isense info----------------------------------------------------------------------------
uint32_t cntOutFrq2, cntInFrq2;
void update_isense_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint32_t i=0;
  
  p_mib->isenseGroup.isenseInstalledISENSE = p_modbus_manager->number_of_isense;
  p_mib->connect_alarm_group_struct.alarm_isense_connect = 0;
  p_mib->main_alarm_group_struct.alarm_isense = 0;
  p_mib->isenseGroup.isenseActiveISENSE = p_mib->isenseGroup.isenseInstalledISENSE;
  for (i = 0; i < p_mib->isenseGroup.isenseInstalledISENSE; i++)
  {
    p_mib->main_alarm_group_struct.alarm_isense_pack[i] = 0;
    if (p_modbus_manager->isense_info_struct[i].u8ErrorFlag == 1)
    {
      if(p_modbus_manager->cnt_dis_connect_isense[i] > 2){ // mat ket noi 3 lan moi bao len
        p_modbus_manager->cnt_dis_connect_isense[i] = 3;
        p_mib->isenseGroup.isenseActiveISENSE--;
        if (p_mib->isenseGroup.isenseActiveISENSE <= 0)
        {
          p_mib->isenseGroup.isenseActiveISENSE = 0;
        }
        p_mib->connect_alarm_group_struct.alarm_isense_connect |= (p_modbus_manager->isense_info_struct[i].u8ErrorFlag << i);
        p_mib->isenseGroup.isenseTable.isenseStatus = 170;
        p_mib->main_alarm_group_struct.alarm_isense_pack[i]=p_mib->connect_alarm_group_struct.alarm_isense_connect; // them trap bao mat ket noi
      }
    }
    else
    {
      p_modbus_manager->cnt_dis_connect_isense[i] = 0;
      p_mib->isenseGroup.isenseTable.isenseStatus = 255;
    }
    
    if (p_mib->isenseGroup.isenseTable.isenseStatus != 170)
    {
      if (p_mib->isenseGroup.isenseTable.isenseStatus == 165)
      {
        p_mib->main_alarm_group_struct.alarm_isense = 1;
      }
      
      switch(p_modbus_manager->isense_manager_struct[i].isense_type)
      {
      case 1://FORLONG
        {
          p_mib->isenseGroup.isenseTable.isenseID = i+1;
          
          p_mib->isenseGroup.isenseTable.isenseImportActiveEnergy = 0;
          p_mib->isenseGroup.isenseTable.isenseExportActiveEnergy = 0;
          p_mib->isenseGroup.isenseTable.isenseImportReactiveEnergy = 0 ;
          p_mib->isenseGroup.isenseTable.isenseExportReactiveEnergy = 0 ;              
          p_mib->isenseGroup.isenseTable.isenseTotalActiveEnergy = (uint32_t) (p_modbus_manager->isense_info_struct[i].u32TotalActiveE) ;             
          p_mib->isenseGroup.isenseTable.isenseTotalReactiveEnergy = (uint32_t) (p_modbus_manager->isense_info_struct[i].u32TotalReActiveE) ;             
          p_mib->isenseGroup.isenseTable.isensePowerFactor = (uint32_t) (p_modbus_manager->isense_info_struct[i].fPowerFactor/10) ; 
          p_mib->isenseGroup.isenseTable.isenseFrequency = (uint32_t) (p_modbus_manager->isense_info_struct[i].fFrequency/10);
          p_mib->isenseGroup.isenseTable.isenseCurrent = (uint32_t) (p_modbus_manager->isense_info_struct[i].fCurrent);
          p_mib->isenseGroup.isenseTable.isenseActivePower = (int32_t) (p_modbus_manager->isense_info_struct[i].fActivePower*10);
          p_mib->isenseGroup.isenseTable.isenseReactivePower = 0;
          p_mib->isenseGroup.isenseTable.isenseVoltage = (uint32_t) (p_modbus_manager->isense_info_struct[i].fVoltage*10);
          
          
          p_mib->isenseGroup.isenseTable.isenseType = p_modbus_manager->isense_manager_struct[i].isense_type;
          sprintf(&p_mib->isenseGroup.isenseTable.isenseModel[0],"DRS-205C");
          p_mib->isenseGroup.isenseTable.isenseModelLen = 8;
          sprintf(&p_mib->isenseGroup.isenseTable.isenseSerial[0],"%08d",p_modbus_manager->isense_info_struct[i].u32SerialNumber);
          p_mib->isenseGroup.isenseTable.isenseSerialLen = 8;
          p_mib->isenseGroup.isenseTable.isenseModbusID = p_modbus_manager->isense_manager_struct[i].isense_abs_slave_id;
          
          if((p_mib->isenseGroup.isenseTable.isenseFrequency != 0) && (p_mib->isenseGroup.isenseTable.isenseEnableFreqTrap == 1))
          {
            if ((p_mib->isenseGroup.isenseTable.isenseFrequency >= (500 + p_mib->isenseGroup.isenseTable.isenseDeltaFreqDisConnect)) || 
                (p_mib->isenseGroup.isenseTable.isenseFrequency <= (500 - p_mib->isenseGroup.isenseTable.isenseDeltaFreqDisConnect))){
                  cntInFrq2 = 0;
                  if(cntOutFrq2++ > 10){
                    cntOutFrq2 = 11;
                    p_mib->isenseGroup.isenseTable.isenseOutOfRangeFreq = 1;
                  }
                }
            else if ((p_mib->isenseGroup.isenseTable.isenseFrequency <= (500 + p_mib->isenseGroup.isenseTable.isenseDeltaFreqReConnect)) && 
                     (p_mib->isenseGroup.isenseTable.isenseFrequency >= (500 - p_mib->isenseGroup.isenseTable.isenseDeltaFreqReConnect))){
                       cntOutFrq2 = 0;
                       if(cntInFrq2++ > 10){
                         cntInFrq2 = 11;
                         p_mib->isenseGroup.isenseTable.isenseOutOfRangeFreq = 0;
                       }
                     }
          }
          else {
            cntOutFrq2 = 0;
            cntInFrq2  = 0;
            p_mib->isenseGroup.isenseTable.isenseOutOfRangeFreq = 0;
          }
          
          p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack2[i] = p_mib->isenseGroup.isenseTable.isenseOutOfRangeFreq;
        }
        break;
      default:
        break;
      };
    }
    else
    {
      p_mib->isenseGroup.isenseTable.isenseID = i+1;
      p_mib->isenseGroup.isenseTable.isenseImportActiveEnergy = 0;
      p_mib->isenseGroup.isenseTable.isenseExportActiveEnergy = 0;
      p_mib->isenseGroup.isenseTable.isenseImportReactiveEnergy = 0;
      p_mib->isenseGroup.isenseTable.isenseExportReactiveEnergy = 0;            
      p_mib->isenseGroup.isenseTable.isenseTotalActiveEnergy = 0;          
      p_mib->isenseGroup.isenseTable.isenseTotalReactiveEnergy = 0;          
      p_mib->isenseGroup.isenseTable.isensePowerFactor = 0;
      p_mib->isenseGroup.isenseTable.isenseFrequency = 0;
      p_mib->isenseGroup.isenseTable.isenseCurrent = 0;
      p_mib->isenseGroup.isenseTable.isenseActivePower = 0;
      p_mib->isenseGroup.isenseTable.isenseVoltage = 0;
      
      sprintf(&p_mib->isenseGroup.isenseTable.isenseModel[0]," ");
      p_mib->isenseGroup.isenseTable.isenseSerialLen = 1;
      p_mib->isenseGroup.isenseTable.isenseModelLen = 1;
      sprintf(&p_mib->isenseGroup.isenseTable.isenseSerial[0]," ");
      
      p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = 0;
      
      p_mib->isenseGroup.isenseTable.isenseType = p_modbus_manager->isense_manager_struct[i].isense_type;
      p_mib->isenseGroup.isenseTable.isenseModbusID = p_modbus_manager->isense_manager_struct[i].isense_abs_slave_id;
    }
  }
  //clean----------------------------------------------------------------------------------------------------------------
  for(i = p_mib->isenseGroup.isenseInstalledISENSE; i<1;i++){
    p_mib->isenseGroup.isenseTable.isenseStatus =0;
    
    p_mib->isenseGroup.isenseTable.isenseID = i+1;
    p_mib->isenseGroup.isenseTable.isenseImportActiveEnergy = 0;
    p_mib->isenseGroup.isenseTable.isenseExportActiveEnergy = 0;
    p_mib->isenseGroup.isenseTable.isenseImportReactiveEnergy = 0;
    p_mib->isenseGroup.isenseTable.isenseExportReactiveEnergy = 0;            
    p_mib->isenseGroup.isenseTable.isenseTotalActiveEnergy = 0;          
    p_mib->isenseGroup.isenseTable.isenseTotalReactiveEnergy = 0;          
    p_mib->isenseGroup.isenseTable.isensePowerFactor = 0;
    p_mib->isenseGroup.isenseTable.isenseFrequency = 0;
    p_mib->isenseGroup.isenseTable.isenseCurrent = 0;
    p_mib->isenseGroup.isenseTable.isenseActivePower = 0;
    p_mib->isenseGroup.isenseTable.isenseVoltage = 0;
    
    sprintf(&p_mib->isenseGroup.isenseTable.isenseModel[0]," ");
    p_mib->isenseGroup.isenseTable.isenseSerialLen = 1;
    p_mib->isenseGroup.isenseTable.isenseModelLen = 1;
    sprintf(&p_mib->isenseGroup.isenseTable.isenseSerial[0]," ");
    
    p_mib->main_alarm_group_struct.alarm_out_of_range_freq_pack[i] = 0;
    
    p_mib->isenseGroup.isenseTable.isenseType = p_modbus_manager->isense_manager_struct[i].isense_type;
    p_mib->isenseGroup.isenseTable.isenseModbusID = p_modbus_manager->isense_manager_struct[i].isense_abs_slave_id;
    
  }
  
  
}
 
//update pmdc info------------------------------------------------------------------------------
void update_pmdc_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  uint32_t i=0;
  p_mib->pm_dc_group.pm_dc_installed_PM_DC = p_modbus_manager->number_of_pmdc;
  p_mib->connect_alarm_group_struct.alarm_pmdc_connect = 0;
  p_mib->main_alarm_group_struct.alarm_pm_dc = 0;
  p_mib->pm_dc_group.pm_dc_active_PM_DC = p_mib->pm_dc_group.pm_dc_installed_PM_DC;
  for(i = 0; i < p_mib->pm_dc_group.pm_dc_installed_PM_DC; i++){
    p_mib->main_alarm_group_struct.alarm_pm_dc_pack[i] = 0;
    if (p_modbus_manager->pmdc_info_struct[i].u8_error_flag == 1)
    {
      if(p_modbus_manager->cnt_dis_connect_pmdc[i] > 2){ // mat ket noi 3 lan moi bao len
        p_modbus_manager->cnt_dis_connect_pmdc[i] = 3;
        p_mib->pm_dc_group.pm_dc_active_PM_DC--;
        if (p_mib->pm_dc_group.pm_dc_active_PM_DC <= 0)
        {
          p_mib->pm_dc_group.pm_dc_active_PM_DC = 0;
        }
        p_mib->connect_alarm_group_struct.alarm_pmdc_connect |= (p_modbus_manager->pmdc_info_struct[i].u8_error_flag << i);
        p_mib->pm_dc_group.pm_dc_table.pm_dc_status = 170;
        p_mib->main_alarm_group_struct.alarm_pm_dc_pack[i]=p_mib->connect_alarm_group_struct.alarm_pmdc_connect; // them trap bao mat ket noi
      }
    }
    else {
      p_modbus_manager->cnt_dis_connect_pmdc[i] = 0;
      p_mib->pm_dc_group.pm_dc_table.pm_dc_status = 255;
    }
    
    if (p_mib->pm_dc_group.pm_dc_table.pm_dc_status != 170)
    {
      if (p_mib->pm_dc_group.pm_dc_table.pm_dc_status == 165)
      {
        p_mib->main_alarm_group_struct.alarm_pm_dc = 1;
      }
      
      switch(p_modbus_manager->pmdc_manager_struct[i].pmdc_type)
      {
      case 1://YADA_DC
        {
          p_mib->pm_dc_group.pm_dc_table.pm_dc_id                   = i+1;
          p_mib->pm_dc_group.pm_dc_table.pm_dc_voltage              = (uint32_t)p_modbus_manager->pmdc_info_struct[i].f_voltage;
          p_mib->pm_dc_group.pm_dc_table.pm_dc_current              = (uint32_t)p_modbus_manager->pmdc_info_struct[i].f_current;
          p_mib->pm_dc_group.pm_dc_table.pm_dc_active_power         = (uint32_t)p_modbus_manager->pmdc_info_struct[i].f_active_power;
          p_mib->pm_dc_group.pm_dc_table.pm_dc_active_energy        = (uint32_t)p_modbus_manager->pmdc_info_struct[i].f_active_energy;
          p_mib->pm_dc_group.pm_dc_table.pm_dc_type                 = p_modbus_manager->pmdc_manager_struct[i].pmdc_type;
          sprintf(&p_mib->pm_dc_group.pm_dc_table.pm_dc_model_[0],"YADA");
          p_mib->pm_dc_group.pm_dc_table.pm_dc_model_len            = 4;
          p_mib->pm_dc_group.pm_dc_table.pm_dc_modbus_ID            = p_modbus_manager->pmdc_manager_struct[i].pmdc_abs_slave_id; 
        }
        break;
      default:
        break;
      };
    }
    else
    {
      p_mib->pm_dc_group.pm_dc_table.pm_dc_id                   = i+1;
      p_mib->pm_dc_group.pm_dc_table.pm_dc_voltage              = 0;
      p_mib->pm_dc_group.pm_dc_table.pm_dc_current              = 0;
      p_mib->pm_dc_group.pm_dc_table.pm_dc_active_power         = 0;
      p_mib->pm_dc_group.pm_dc_table.pm_dc_active_energy        = 0;
      p_mib->pm_dc_group.pm_dc_table.pm_dc_type                 = p_modbus_manager->pmdc_manager_struct[i].pmdc_type;
      p_mib->pm_dc_group.pm_dc_table.pm_dc_modbus_ID            = p_modbus_manager->pmdc_manager_struct[i].pmdc_abs_slave_id;
      sprintf(&p_mib->pm_dc_group.pm_dc_table.pm_dc_model_[0]," ");
      p_mib->pm_dc_group.pm_dc_table.pm_dc_model_len            = 1;       
    }
    
  }
  //clean--------------------------------------------------------------------------------------------------------------------------------------
  for (i = p_mib->pm_dc_group.pm_dc_installed_PM_DC; i<1; i++){
    p_mib->pm_dc_group.pm_dc_table.pm_dc_status = 0;
    p_mib->pm_dc_group.pm_dc_table.pm_dc_id                   = i+1;
    p_mib->pm_dc_group.pm_dc_table.pm_dc_voltage              = 0;
    p_mib->pm_dc_group.pm_dc_table.pm_dc_current              = 0;
    p_mib->pm_dc_group.pm_dc_table.pm_dc_active_power         = 0;
    p_mib->pm_dc_group.pm_dc_table.pm_dc_active_energy        = 0;
    p_mib->pm_dc_group.pm_dc_table.pm_dc_type                 = p_modbus_manager->pmdc_manager_struct[i].pmdc_type;
    p_mib->pm_dc_group.pm_dc_table.pm_dc_modbus_ID            = p_modbus_manager->pmdc_manager_struct[i].pmdc_abs_slave_id;
    sprintf(&p_mib->pm_dc_group.pm_dc_table.pm_dc_model_[0]," ");
    p_mib->pm_dc_group.pm_dc_table.pm_dc_model_len            = 1;
  }
  
}

//update bm info------------------------------------------------------------------------------
void update_bm_info(PRIVATE_MIB_BASE_STRUCT *p_mib,MODBUS_MANAGER_STRUCT *p_modbus_manager){
  
  uint32_t i=0;
  
  p_mib->bmGroup.bmInstalledBM = p_modbus_manager->number_of_bm;
  p_mib->connect_alarm_group_struct.alarm_bm_connect = 0;
  p_mib->main_alarm_group_struct.alarm_bm = 0;
  p_mib->bmGroup.bmActiveBM = p_mib->bmGroup.bmInstalledBM;
  for (i = 0; i < p_mib->bmGroup.bmInstalledBM; i++)
  {
    p_mib->main_alarm_group_struct.alarm_bm_pack[i] = 0;
    if (p_modbus_manager->bm_info_struct[i].u8ErrorFlag == 1)
    {
      p_mib->bmGroup.bmActiveBM--;
      if (p_mib->bmGroup.bmActiveBM <= 0)
      {
        p_mib->bmGroup.bmActiveBM = 0;
      }
      p_mib->connect_alarm_group_struct.alarm_bm_connect |= (p_modbus_manager->bm_info_struct[i].u8ErrorFlag << i);
      p_mib->bmGroup.bmTable[i].bmStatus = 170;
    }
    else
    {        
      //          p_mib->bmGroup.bmTable[i].bmStatus = 255;
      if(p_mib->bmGroup.bmTable[i].bmAlarmStatus.all != 0)
      {
        p_mib->bmGroup.bmTable[i].bmStatus = 165;
      }
      else
      {
        p_mib->bmGroup.bmTable[i].bmStatus = 255;
      }
    }
    
    if (p_mib->bmGroup.bmTable[i].bmStatus != 170)
    {
      if (p_mib->bmGroup.bmTable[i].bmStatus == 165)
      {
        p_mib->main_alarm_group_struct.alarm_bm_pack[i] = p_mib->bmGroup.bmTable[i].bmAlarmStatus.all;//1;
      }
      
      switch(p_modbus_manager->bm_manager_struct[i].bm_type)
      {
      case 1:// VIETTEL
        {
          p_mib->bmGroup.bmTable[i].bmID = i+1;
          p_mib->bmGroup.bmTable[i].bmBattVolt = p_modbus_manager->bm_info_struct[i].u32BattVolt;
          p_mib->bmGroup.bmTable[i].bmPackVolt = p_modbus_manager->bm_info_struct[i].u32PackVolt;
          p_mib->bmGroup.bmTable[i].bmPackCurr = p_modbus_manager->bm_info_struct[i].i32PackCurr;
          
          p_mib->bmGroup.bmTable[i].bmCellVolt1 = p_modbus_manager->bm_info_struct[i].u16CellVolt[0];              
          p_mib->bmGroup.bmTable[i].bmCellVolt2 = p_modbus_manager->bm_info_struct[i].u16CellVolt[1];             
          p_mib->bmGroup.bmTable[i].bmCellVolt3 = p_modbus_manager->bm_info_struct[i].u16CellVolt[2];             
          p_mib->bmGroup.bmTable[i].bmCellVolt4 = p_modbus_manager->bm_info_struct[i].u16CellVolt[3];          
          
          p_mib->bmGroup.bmTable[i].bmCellTemp1 = p_modbus_manager->bm_info_struct[i].u16CellTemp[0];
          p_mib->bmGroup.bmTable[i].bmCellTemp2 = p_modbus_manager->bm_info_struct[i].u16CellTemp[1];
          p_mib->bmGroup.bmTable[i].bmCellTemp3 = p_modbus_manager->bm_info_struct[i].u16CellTemp[2];
          p_mib->bmGroup.bmTable[i].bmCellTemp4 = p_modbus_manager->bm_info_struct[i].u16CellTemp[3];
          p_mib->bmGroup.bmTable[i].bmCellTemp5 = p_modbus_manager->bm_info_struct[i].u16CellTemp[4];
          p_mib->bmGroup.bmTable[i].bmCellTemp6 = p_modbus_manager->bm_info_struct[i].u16CellTemp[5];
          
          p_mib->bmGroup.bmTable[i].bmSOC = p_modbus_manager->bm_info_struct[i].u16SOC;              
          p_mib->bmGroup.bmTable[i].bmSOH = p_modbus_manager->bm_info_struct[i].u32SOH;
          p_mib->bmGroup.bmTable[i].bmAlarmStatus.all = p_modbus_manager->bm_info_struct[i].u32AlarmStatus;
          p_mib->bmGroup.bmTable[i].bmBalanceStatus = p_modbus_manager->bm_info_struct[i].u32BalanceStatus;
          p_mib->bmGroup.bmTable[i].bmBattStatus = p_modbus_manager->bm_info_struct[i].u16BattStatus;
          
          
          p_mib->bmGroup.bmTable[i].bmDischargeTime = p_modbus_manager->bm_info_struct[i].u32DischargeTime;
          p_mib->bmGroup.bmTable[i].bmTotalRunTime = p_modbus_manager->bm_info_struct[i].u32TotalRunTime;             
          
          p_mib->bmGroup.bmTable[i].bmVoltDiff = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32VoltDiff);
          p_mib->bmGroup.bmTable[i].bmMaxTemp = (int32_t) (p_modbus_manager->bm_info_struct[i].u32MaxTemp);
          p_mib->bmGroup.bmTable[i].bmVoltThres = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32VoltThres);
          p_mib->bmGroup.bmTable[i].bmCurrThres = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32CurrThres);
          p_mib->bmGroup.bmTable[i].bmTimeThres = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32TimeThres);
          p_mib->bmGroup.bmTable[i].bmSOCThres = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32SOCThres);
          p_mib->bmGroup.bmTable[i].bmMinTemp = (int32_t) (p_modbus_manager->bm_info_struct[i].u32MinTemp);
          p_mib->bmGroup.bmTable[i].bmLowCapTime = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32LowCapTime);
          p_mib->bmGroup.bmTable[i].bmTotalDisAH = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32TotalDisAH);
          p_mib->bmGroup.bmTable[i].bmHalfVoltAlarm = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32HalfVoltAlarm);
          p_mib->bmGroup.bmTable[i].bmRechargeVolt = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32RechargeVolt);
          p_mib->bmGroup.bmTable[i].bmCurrBalanceThres = (uint32_t) (p_modbus_manager->bm_info_struct[i].u32CurrBalanceThres);
          
          p_mib->bmGroup.bmTable[i].bmType = p_modbus_manager->bm_manager_struct[i].bm_type;
          p_mib->bmGroup.bmTable[i].bmModbusID = p_modbus_manager->bm_manager_struct[i].bm_abs_slave_id;
        }
        break;
      };
    }
    else
    {
      p_mib->bmGroup.bmTable[i].bmID = i+1;
      p_mib->bmGroup.bmTable[i].bmBattVolt = 0;
      p_mib->bmGroup.bmTable[i].bmPackVolt = 0;
      p_mib->bmGroup.bmTable[i].bmPackCurr = 0;
      
      p_mib->bmGroup.bmTable[i].bmCellVolt1 = 0;              
      p_mib->bmGroup.bmTable[i].bmCellVolt2 = 0;             
      p_mib->bmGroup.bmTable[i].bmCellVolt3 = 0;             
      p_mib->bmGroup.bmTable[i].bmCellVolt4 = 0;          
      
      p_mib->bmGroup.bmTable[i].bmCellTemp1 = 0;
      p_mib->bmGroup.bmTable[i].bmCellTemp2 = 0;
      p_mib->bmGroup.bmTable[i].bmCellTemp3 = 0;
      p_mib->bmGroup.bmTable[i].bmCellTemp4 = 0;
      p_mib->bmGroup.bmTable[i].bmCellTemp5 = 0;
      p_mib->bmGroup.bmTable[i].bmCellTemp6 = 0;
      
      p_mib->bmGroup.bmTable[i].bmSOC = 0;              
      p_mib->bmGroup.bmTable[i].bmSOH = 0;
      
      p_mib->bmGroup.bmTable[i].bmDischargeTime = 0;
      p_mib->bmGroup.bmTable[i].bmTotalRunTime = 0;
      
      p_mib->bmGroup.bmTable[i].bmVoltDiff = 0;
      p_mib->bmGroup.bmTable[i].bmMaxTemp = 0;
      p_mib->bmGroup.bmTable[i].bmVoltThres = 0;
      p_mib->bmGroup.bmTable[i].bmCurrThres = 0;
      p_mib->bmGroup.bmTable[i].bmTimeThres = 0;
      p_mib->bmGroup.bmTable[i].bmSOCThres = 0;
      p_mib->bmGroup.bmTable[i].bmMinTemp = 0;
      p_mib->bmGroup.bmTable[i].bmLowCapTime = 0;
      p_mib->bmGroup.bmTable[i].bmTotalDisAH = 0;
      p_mib->bmGroup.bmTable[i].bmHalfVoltAlarm = 0;
      p_mib->bmGroup.bmTable[i].bmRechargeVolt = 0;
      p_mib->bmGroup.bmTable[i].bmCurrBalanceThres = 0;
      p_mib->bmGroup.bmTable[i].bmAlarmStatus.all = 0;
      
      p_mib->bmGroup.bmTable[i].bmType = p_modbus_manager->bm_manager_struct[i].bm_type;
      p_mib->bmGroup.bmTable[i].bmModbusID = p_modbus_manager->bm_manager_struct[i].bm_abs_slave_id;
    }
  }
  
}

//update fan dpc info-------------------------------------------------------------------------
void update_fan_dpc_info(PRIVATE_MIB_BASE_STRUCT *p_mib){
  if(TYPE == DKD51_BDP){
    //FAN ===============================================================================================
    //parameter setting----------------------------------------------------------------------------------
    p_mib->fan_dpc_info.mib.para_dc_starting_point   = fan_info_dpc_t.para_dc_starting_point; 
    p_mib->fan_dpc_info.mib.para_dc_sensivive_point  = fan_info_dpc_t.para_dc_sensivive_point;
    p_mib->fan_dpc_info.mib.para_dc_heating_start_point = fan_info_dpc_t.para_dc_heating_start_point;
    p_mib->fan_dpc_info.mib.para_dc_heating_sensivive   = fan_info_dpc_t.para_dc_heating_sensivive;
    p_mib->fan_dpc_info.mib.para_alarm_high_temp     = fan_info_dpc_t.para_alarm_high_temp;
    p_mib->fan_dpc_info.mib.para_alarm_low_temp      = fan_info_dpc_t.para_alarm_low_temp;
    //parameter-----------------------------------------------------------------------------------------
    p_mib->fan_dpc_info.mib.env_temp                 = fan_info_dpc_t.env_temp;
    p_mib->fan_dpc_info.mib.airflow_temp             = fan_info_dpc_t.airflow_temp;
    p_mib->fan_dpc_info.mib.voltage                  = fan_info_dpc_t.voltage;
    p_mib->fan_dpc_info.mib.working_current          = fan_info_dpc_t.working_current;
    p_mib->fan_dpc_info.mib.state_device             = fan_info_dpc_t.state_device;
    p_mib->fan_dpc_info.mib.state_fan                = fan_info_dpc_t.state_fan;
    //alarm----------------------------------------------------------------------------------------------
    p_mib->fan_dpc_info.mib.alarm_fail_refrigeration = fan_info_dpc_t.alarm_fail_refrigeration;
    p_mib->fan_dpc_info.mib.alarm_high_temp          = fan_info_dpc_t.alarm_high_temp;
    p_mib->fan_dpc_info.mib.alarm_low_temp           = fan_info_dpc_t.alarm_low_temp;   
    p_mib->fan_dpc_info.mib.alarm_fail_sensor_temp   = fan_info_dpc_t.alarm_fail_sensor_temp;
    p_mib->fan_dpc_info.mib.alarm_high_voltage       = fan_info_dpc_t.alarm_high_voltage;
    p_mib->fan_dpc_info.mib.alarm_low_voltage        = fan_info_dpc_t.alarm_low_voltage;
  }else{
    //parameter setting----------------------------------------------------------------------------------
    p_mib->fan_dpc_info.mib.para_dc_starting_point   = 0;
    p_mib->fan_dpc_info.mib.para_dc_sensivive_point  = 0;
    p_mib->fan_dpc_info.mib.para_dc_heating_start_point = 0;
    p_mib->fan_dpc_info.mib.para_dc_heating_sensivive   = 0;
    p_mib->fan_dpc_info.mib.para_alarm_high_temp     = 0;
    p_mib->fan_dpc_info.mib.para_alarm_low_temp      = 0;
    //parameter-----------------------------------------------------------------------------------------
    p_mib->fan_dpc_info.mib.env_temp                 = 0;
    p_mib->fan_dpc_info.mib.airflow_temp             = 0;
    p_mib->fan_dpc_info.mib.voltage                  = 0;
    p_mib->fan_dpc_info.mib.working_current          = 0;
    p_mib->fan_dpc_info.mib.state_device             = 0;
    p_mib->fan_dpc_info.mib.state_fan                = 0;
    //alarm-----------------------------------------------------------------------------------------------
    p_mib->fan_dpc_info.mib.alarm_fail_refrigeration = 0;
    p_mib->fan_dpc_info.mib.alarm_high_temp          = 0;
    p_mib->fan_dpc_info.mib.alarm_low_temp           = 0;  
    p_mib->fan_dpc_info.mib.alarm_fail_sensor_temp   = 0;
    p_mib->fan_dpc_info.mib.alarm_high_voltage       = 0;
    p_mib->fan_dpc_info.mib.alarm_low_voltage        = 0;
    //alarm connect----------------------------------------------------------------------------------------
    p_mib->connect_alarm_group_struct.alarm_dpc_fan_connect      = 0;
  }
  
}








