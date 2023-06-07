//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
void DKD51_set_command(unsigned char *arrCmd,uint16_t templateCmd,uint16_t setNum, unsigned char type)
{
        uint8_t	i=0;
	unsigned char cmd[24];
	unsigned char* cmd_ptr = (unsigned char*)&cmd[0];
	unsigned char* Header_Set_Code_ptr;
	unsigned char hexNum[2];
	unsigned char hexArray[4] = {0,0,0,0};
	uint16_t pChecksum=0;
	if(type==DKD51_AC) 
	{
		Header_Set_Code_ptr= (unsigned char*)&DKD51_header_ac_setcode[0];
	}
	if(type==DKD51_DC)
	{
		Header_Set_Code_ptr= (unsigned char*)&DKD51_header_dc_setcode[0];
	}
        if(type==DKD51_FAN)
        {
                Header_Set_Code_ptr= (unsigned char*)&DKD51_header_fan_dpc_setcode[0];
        }

	while (i < 13)
	{
		i++;
		*cmd_ptr++ = *Header_Set_Code_ptr++;
	}
	hex2char(&hexArray[0],templateCmd,2);
	while (i < 15)
	{
		i++;
		*cmd_ptr++ = hexArray[i-14];
	}

	memcpy(&hexNum,&setNum,2);
	for (i = 0; i < 2; i++)
	{
		hex2char(&hexArray[i*2],hexNum[1-i],2);
	}
	for (i = 0; i < 4; i++)
	{
		cmd[15+i] = hexArray[i];
	}

	pChecksum = Checksum16(&cmd[0],24);
	hex2char(&cmd[19],pChecksum,4);

	cmd[23] = 0x0d;
	cmd_ptr = &cmd[0];
	i = 0;
	while (i < 24)
	{
		i++;
		*arrCmd++ = *cmd_ptr++;
	}
	
}
void DKD51_set_command_1(unsigned char *arrCmd,uint16_t templateCmd,int16_t setNum, unsigned char type)
{
        uint8_t	i=0;
	unsigned char cmd[24];
	unsigned char* cmd_ptr = (unsigned char*)&cmd[0];
	unsigned char* Header_Set_Code_ptr;
	unsigned char hexNum[2];
	unsigned char hexArray[4] = {0,0,0,0};
	uint16_t pChecksum=0;
	if(type==DKD51_AC)
	{
		Header_Set_Code_ptr= (unsigned char*)&DKD51_header_ac_setcode[0];
	}
	if(type==DKD51_DC)
	{
		Header_Set_Code_ptr= (unsigned char*)&DKD51_header_dc_setcode[0];
	}
        if(type==DKD51_FAN)
        {
                Header_Set_Code_ptr= (unsigned char*)&DKD51_header_fan_dpc_setcode[0];
        }

	while (i < 13)
	{
		i++;
		*cmd_ptr++ = *Header_Set_Code_ptr++;
	}
	hex2char(&hexArray[0],templateCmd,2);
	while (i < 15)
	{
		i++;
		*cmd_ptr++ = hexArray[i-14];
	}

	memcpy(&hexNum,&setNum,2);
	for (i = 0; i < 2; i++)
	{
		hex2char(&hexArray[i*2],hexNum[1-i],2);
	}
	for (i = 0; i < 4; i++)
	{
		cmd[15+i] = hexArray[i];
	}

	pChecksum = Checksum16(&cmd[0],24);
	hex2char(&cmd[19],pChecksum,4);

	cmd[23] = 0x0d;
	cmd_ptr = &cmd[0];
	i = 0;
	while (i < 24)
	{
		i++;
		*arrCmd++ = *cmd_ptr++;
	}
	
}
//set ac low----------------------------------------------------------------------------------
void DKD51_set_ac_low(){
  sAcInfo.fAcLowThres = (float)private_mib_base_struct.configGroup.cfgAcLowLevel;
  uint16_t temp = (uint16_t)(sAcInfo.fAcLowThres*100);
  DKD51_set_command(&g_setInfo[0],DKD51_AC_VOLT_LOWER_LIMIT,temp,DKD51_AC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set ac high---------------------------------------------------------------------------------
void DKD51_set_ac_high(){
  sAcInfo.fAcHighThres = (float)private_mib_base_struct.configGroup.cfgAcHighLevel;
  uint16_t temp = (uint16_t)(sAcInfo.fAcHighThres*100);
  DKD51_set_command(&g_setInfo[0],DKD51_AC_VOLT_UPPER_LIMIT,temp,DKD51_AC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set dc volt high-----------------------------------------------------------------------------
void DKD51_set_dc_over(){
  sBattInfo.sRAWBattInfo.fDCOverCfg = ((float)private_mib_base_struct.configGroup.cfgDCOverVolt)/100;
  uint16_t temp = (uint16_t) (sBattInfo.sRAWBattInfo.fDCOverCfg*100);
  DKD51_set_command(&g_setInfo[0],DKD51_DC_VOLT_UPPER_LIMIT,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
  
}
//set dc low volt-------------------------------------------------------------------------------
void DKD51_set_dc_low(){
  sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = ((float)private_mib_base_struct.configGroup.cfgDCLowVolt)/100;
  uint16_t temp = (uint16_t) (sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg*100);
  DKD51_set_command(&g_setInfo[0],DKD51_DC_VOLT_LOWER_LIMIT,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
  
}
//set float volt---------------------------------------------------------------------------------
void DKD51_set_float_volt(){
  sBattInfo.sRAWBattInfo.fFltVoltCfg = ((float)private_mib_base_struct.configGroup.cfgFloatVolt)/100;
  uint16_t temp =(uint16_t) (sBattInfo.sRAWBattInfo.fFltVoltCfg*100);
  
  DKD51_set_command(&g_setInfo[0],DKD51_FLOAT_VOLT,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set boost volt------------------------------------------------------------------------------
void DKD51_set_boost_volt(){
  sBattInfo.sRAWBattInfo.fBotVoltCfg = ((float)private_mib_base_struct.configGroup.cfgBoostVolt)/100;
  uint16_t temp =(uint16_t)(sBattInfo.sRAWBattInfo.fBotVoltCfg*100);
  DKD51_set_command(&g_setInfo[0],DKD51_BOOST_VOLT,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set charge current limit---------------------------------------------------------------------
void DKD51_set_charge_current_limit(){
  //sBattInfo.sRAWBattInfo.fCCLVal = (float)private_mib_base_struct.configGroup.cfgCurrentLimit/1000;
  sBattInfo.sRAWBattInfo.fCapCCLVal  = ((float)private_mib_base_struct.configGroup.cfgCurrentlimitA)/100;//sBattInfo.sRAWBattInfo.fCCLVal* sBattInfo.sRAWBattInfo.fCapTotal;
  uint16_t temp =(uint16_t)(sBattInfo.sRAWBattInfo.fCapCCLVal*100);
  DKD51_set_command(&g_setInfo[0],DKD51_CHARGING_FACTOR,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set temp compensation -----------------------------------------------------------------------
void DKD51_set_temp_compensation(){
  sBattInfo.sRAWBattInfo.fTempCompVal = (float)private_mib_base_struct.configGroup.cfgTempCompValue;
  uint16_t temp =(uint16_t)(sBattInfo.sRAWBattInfo.fTempCompVal*100);
  DKD51_set_command(&g_setInfo[0],DKD51_TEMP_COMPENSATION,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set batt capacity 1--------------------------------------------------------------------------
void DKD51_set_batt_capacity_1(){
  sBattInfo.sRAWBattInfo.fCapTotal = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal;
  uint16_t temp =(uint16_t)(sBattInfo.sRAWBattInfo.fCapTotal*100);
  DKD51_set_command(&g_setInfo[0],DKD51_BATT_CAP1,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set batt capacity 2--------------------------------------------------------------------------
void DKD51_set_batt_capacity_2(){
  sBattInfo.sRAWBattInfo.fCapTotal2 = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal2;
  uint16_t temp =(uint16_t) (sBattInfo.sRAWBattInfo.fCapTotal2*100);
  DKD51_set_command(&g_setInfo[0],DKD51_BATT_CAP2,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set batt capacity 3--------------------------------------------------------------------------
void DKD51_set_batt_capacity_3(){
  sBattInfo.sRAWBattInfo.fCapTotal3 = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal3;
  uint16_t temp =(uint16_t)(sBattInfo.sRAWBattInfo.fCapTotal3*100);
  DKD51_set_command(&g_setInfo[0],DKD51_BATT_CAP3,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
}
//set batt capacity 4--------------------------------------------------------------------------
void DKD51_set_batt_capacity_4(){
  sBattInfo.sRAWBattInfo.fCapTotal4 = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal4;
  uint16_t temp =(uint16_t)(sBattInfo.sRAWBattInfo.fCapTotal4*100);
  DKD51_set_command(&g_setInfo[0],DKD51_BATT_CAP4,temp,DKD51_DC);
  
  DIR_MODBUS_AB_DC_CABINET_ON;
  vTaskDelay(5);
  for (uint8_t i = 0; i < 24; i++)
  {
     LPUART_WriteBlocking(LPUART4,&g_setInfo[i],1);
  }
  vTaskDelay(5);
  DIR_MODBUS_AB_DC_CABINET_OFF;
  
}
//set ac input current limit-------------------------------------------------------------------
void DKD51_set_ac_input_curr_limit(){
  //sAcInfo.fAcInputCurrLimit = 0;//(float)private_mib_base_struct.configGroup.
  
}
//set fan-------------------------------------------------------------------------------------
//set heating start point------------------------------------------------------------------
void DKD51_set_fan_heating_start_point(int16_t temp){
  DKD51_set_command_1(&g_setInfo[0],DKD51_FAN_SET_HEATING_START_POINT,temp,DKD51_FAN);
  
  DIR_MODBUS_AB_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART3,g_setInfo,24);
  DKD51_delay();
  DIR_MODBUS_AB_OFF
}
//set heating sensitive--------------------------------------------------------------------
void DKD51_set_fan_heating_sensitive_point(int16_t temp){
  DKD51_set_command_1(&g_setInfo[0],DKD51_FAN_SET_HEATING_SENSITIVE_POINT,temp,DKD51_FAN);
  
  DIR_MODBUS_AB_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART3,g_setInfo,24);
  DKD51_delay();
  DIR_MODBUS_AB_OFF
}
//set starting point-------------------------------------------------------------------------
void DKD51_set_fan_starting_point(uint16_t temp){
  DKD51_set_command(&g_setInfo[0],DKD51_FAN_SET_STARTING_POINT,temp,DKD51_FAN);
  
  DIR_MODBUS_AB_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART3,g_setInfo,24);
  DKD51_delay();
  DIR_MODBUS_AB_OFF
}

//set sensitive point------------------------------------------------------------------------
void DKD51_set_fan_sensitive_point(uint16_t temp){
  DKD51_set_command(&g_setInfo[0],DKD51_FAN_SET_SENSITIVE_POINT,temp,DKD51_FAN);
  
  DIR_MODBUS_AB_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART3,g_setInfo,24);
  DKD51_delay();
  DIR_MODBUS_AB_OFF
}
//set high temp warning point ----------------------------------------------------------------
void DKD51_set_fan_H_temp_W_point(uint16_t temp){
  DKD51_set_command(&g_setInfo[0],DKD51_FAN_SET_H_TEMP_W_POINT,temp,DKD51_FAN);
  
  DIR_MODBUS_AB_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART3,g_setInfo,24);
  DKD51_delay();
  DIR_MODBUS_AB_OFF
}
//set low temp warning point ----------------------------------------------------------------
void DKD51_set_fan_L_temp_W_point(int16_t temp){
  DKD51_set_command_1(&g_setInfo[0],DKD51_FAN_SET_L_TEMP_W_POINT,temp,DKD51_FAN);
  
  DIR_MODBUS_AB_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART3,g_setInfo,24);
  DKD51_delay();
  DIR_MODBUS_AB_OFF
}
//send request fan----------------------------------------------------------------------------
void DKD51_send_message_resquest_fan(uint8_t * buff_request,uint8_t len_buff_request){
  
  DIR_MODBUS_AB_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART3,buff_request,len_buff_request);
  DKD51_delay();
  DIR_MODBUS_AB_OFF
    
}
//send request---------------------------------------------------------------------------------
void DKD51_send_message_resquest(uint8_t * buff_request,uint8_t len_buff_request){
  
  DIR_MODBUS_AB_DC_CABINET_ON
    vTaskDelay(5);
  LPUART_WriteBlocking(LPUART4,buff_request,len_buff_request);
  DKD51_delay();
  DIR_MODBUS_AB_DC_CABINET_OFF
    
}

//check sum message------------------------------------------------------------------------------------
uint8_t DKD51_check_sum(DPC_T* p){
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
//check return----------------------------------------------------------------------------------------
uint8_t DKD51_check_RTN(DPC_T* p){
  if(hex2byte(&p->rx.buff_485[0],7)== 0) return NO_ERR;
  else return ERR;
}
//clear-----------------------------------------------------------------------------------------------
void DKD51_clearn_buff(DPC_T* p){
  for(uint16_t i=0; i<500;i++){
    p->uart_rx.buff_485[i]=0;
  }
  p->uart_rx.cnt =0;
}
//delay-----------------------------------------------------------------------------------------------
void DKD51_delay(void)
{
  volatile uint32_t i = 0;
  for (i = 0; i < 300; ++i)//2500
  {
    __NOP(); /* delay */
  }
}
//





