//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//declaration-------------------------------------------------------------------
//CNT_TASK                   cnt_task;
TIME_FORMAT_STRUCT         sync_time,EMERV21Time,ZTETime;
FLAG_STRUCT                flag_struct;
MODBUS_MANAGER_STRUCT      modbus_manager_struct;
SYSTEM_FLAG_STRUCT         system_flag_struct;
tAlarmValue                sAlarmValue[128];
uint8_t LibM1Addr = 0;

//agisson=======================================================================
ttestloop sTestRS485;
uint32_t iSet =0;
uint8_t Program_Status =0;
uint8_t Program_Status_old =0;
uint32_t RS485Timer =0;
uint32_t RS485Command =1;
unsigned char g_UARTRxBuf485[300];
uint32_t RecvCntStart485=0;
uint32_t RecvCntEnd485=0;
uint32_t RecvOK=0;
uint32_t RecvCodeOK =0;      
uint8_t u8HisBuffer[256];
uint32_t setuprepeat =0;

uint32_t RX485Count=0;
uint32_t settingCommandmask= 0;
uint32_t settingCommand = 0;
uint8_t setCmd_flag = 0;
uint32_t setCmd_mask = 0;
uint8_t setCmd_Ready = 0;
uint8_t  writeFlash=0;
uint8_t TYPE = 0;


tDcInfo sDcInfo;
tAcInfo sAcInfo;
tBattInfo sBattInfo;
tRectifierInfo sRectInfo;
tBattTestInfo sBattTestInfo;
tMESGState MESGState;
tMESGState MESGState_v21;
//agisson=======================================================================
//emerson m500d=================================================================
tInterruptMesg_Struct sInterruptMesg;
unsigned char g_UARTRxBuf[300];
unsigned char g_UARTRxBuf2[300];
uint32_t RecvCntStart =0;
uint32_t RecvCntStart2 =0;
tInterruptMesgState InterruptMesgState = CHECKBYTE1;
uint32_t PMUConnectCount =0;
uint8_t g_u8ReadBackBuffer[256];
unsigned char g_setInfo[32];
tHistoryInfo sHistoryInfo;
uint8_t RecvBuff[300];
unsigned char g_getInfo[24];
uint8_t g_AlrmMan;
uint8_t ui8User[6];
tSiteInfo sSiteInfo; 
//emerson m500d=================================================================
//trap mode=====================================================================
uint8_t trap_flag[200]={0};
uint32_t  trapStatus_TimePeriod;
//trap mode=====================================================================
//dpc===========================================================================
DPC_T dpc_t;
//dpc===========================================================================
//for fan dpc===================================================================
FAN_DPC_SWITCH_UART_T fan_dpc_switch_uart_t;
FAN_INFO_DPC_T fan_info_dpc_t; 
//for fan dp====================================================================
tAlarmGroupInfo sAlarmInfo; 
tALARMEnvInfo sAlarmEnvInfo;




//VAC_INFO_STRUCT vac_info_struct[1];
//funciton----------------------------------------------------------------------