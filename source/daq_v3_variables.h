//--thanhcm3--07--04--22--
#ifndef _DAQ_V3_VARIABLES_H
#define _DAQ_V3_VARIABLES_H
//include-----------------------------------------------------------------------
//define------------------------------------------------------------------------
#define DIR_MODBUS_AB_OFF                         GPIO_PinWrite(GPIO2, 6U,0);GPIO_PinWrite(GPIO3, 26U,0);
#define DIR_MODBUS_AB_ON                          GPIO_PinWrite(GPIO2, 6U,1);GPIO_PinWrite(GPIO3, 26U,1);    

#define DIR_MODBUS_AB_DC_CABINET_OFF              GPIO_PinWrite(GPIO2,7U,0);GPIO_PinWrite(GPIO2,11U,0);
#define DIR_MODBUS_AB_DC_CABINET_ON               GPIO_PinWrite(GPIO2,7U,1);GPIO_PinWrite(GPIO2,11U,1);


//
#define UCNAMESIZE 19
//eth
typedef enum{
DP83825,   
LAN8720   
}ETH_TYPE;
//logic------------------------------------------
#define ERR    1
#define NO_ERR 0
//typedef enum-----------------------------------------------------------------
typedef enum
{
LIB_COSLIGHT_INFO_1,
LIB_COSLIGHT_INFO_2,
LIB_COSLIGHT_INFO_3,
LIB_COSLIGHT_OLD_INFO_1,
LIB_COSLIGHT_OLD_INFO_2,
LIB_COSLIGHT_OLD_INFO_3,
LIB_SAFT_INFO_1,
LIB_SHOTO_INFO_1,
LIB_SHOTO_INFO_2,
LIB_SHOTO_INFO_3,
LIB_SHOTO_INFO_4,
LIB_HUAWEI_INFO_1,
LIB_HUAWEI_INFO_2,
LIB_HUAWEI_INFO_3,
LIB_HUAWEI_INFO_4,
LIB_HUAWEI_A1_INFO_1,
LIB_HUAWEI_A1_INFO_2,
LIB_HUAWEI_A1_INFO_3,
LIB_HUAWEI_A1_INFO_4,
LIB_VIETTEL_INFO_1,
LIB_VIETTEL_INFO_2,
LIB_VIETTEL_INFO_3,
LIB_VIETTEL_INFO_4,
LIB_VIETTEL_INFO_5,
LIB_VIETTEL_INFO_6,
LIB_NARADA50_INFO_1,
LIB_ZTT_INFO_1,
LIB_ZTT_INFO_2,
LIB_BYD_INFO_1,
LIB_VISION_INFO_1,
LIB_NARADA75_INFO_1,
LIB_NARADA75_INFO_2,
LIB_EVE_INFO_1,
LIB_EVE_INFO_2,
LIB_EVE_INFO_3,
LIB_ZTT_2020_INFO_1,
LIB_ZTT_2020_INFO_2,
LIB_ZTT_2020_INFO_3,

GEN_KUBOTA_INFO_1,
GEN_KUBOTA_INFO_2,
GEN_BE142_INFO_1,
GEN_BE142_INFO_2,
GEN_BE142_INFO_3,
GEN_DEEPSEA_INFO_1,
GEN_DEEPSEA_INFO_2,
GEN_DEEPSEA_INFO_3,
GEN_DEEPSEA_INFO_4,
GEN_LR2057_INFO_1,
GEN_LR2057_INFO_2,
GEN_LR2057_INFO_3,
GEN_HIMOINSA_INFO_1,
GEN_HIMOINSA_INFO_2,
GEN_QC315_INFO_1,
GEN_QC315_INFO_2,
GEN_QC315_INFO_3,
GEN_CUMMIN_INFO_1,

PM_FINECO_INFO_1,
PM_FINECO_INFO_2,
PM_ASCENT_INFO_1,
PM_ASCENT_INFO_2,
PM_ASCENT_INFO_3,
PM_ASCENT_INFO_4,
PM_ASCENT_INFO_5,
PM_ASCENT_INFO_6,
PM_EASTRON_INFO_1,
PM_EASTRON_INFO_2,
PM_EASTRON_INFO_3,
PM_EASTRON_INFO_4,
PM_CET1_INFO_1,
PM_CET1_INFO_2,
PM_CET1_INFO_3,
PM_PILOT_INFO_1,
PM_PILOT_INFO_2,
PM_PILOT_3PHASE_INFO_1,
PM_PILOT_3PHASE_INFO_2,
PM_PILOT_3PHASE_INFO_3,
ISENSE_FORLONG_INFO_1,

VAC_VIETTEL_INFO_1,
VAC_VIETTEL_INFO_2,
VAC_VIETTEL_INFO_3,
BM_VIETTEL_INFO_1,
BM_VIETTEL_INFO_2,

SMCB_OPEN_INFO_1,
SMCB_MATIS_INFO_1,

FUEL_HPT621_INFO_1,

PM_YADA_3PHASE_DPC_INFO_1,
PM_YADA_3PHASE_DPC_INFO_2,
PM_YADA_3PHASE_DPC_INFO_3,
PM_YADA_3PHASE_DPC_INFO_4,
PM_YADA_3PHASE_DPC_INFO_5,
PM_YADA_3PHASE_DPC_INFO_6,
PM_YADA_3PHASE_DPC_INFO_7,
PM_YADA_3PHASE_DPC_INFO_8,
PM_DC_YADA_INFO_1,
PM_DC_YADA_INFO_2,
PM_DC_YADA_INFO_3,
PM_DC_YADA_INFO_4,
}
RTU_REQ_STATE;
//--------------------------------------------------
typedef enum{
  SET_DAQ_NOOP,
  SET_DAQ_WALKINTIMEEN,
  SET_DAQ_WALKINTIMEDURATION,
  SET_DAQ_CURRENTLIMIT,
  SET_DAQ_FLOATVOLT,
  SET_DAQ_BOOSTVOLT,
  SET_DAQ_LLVDVOLT,
  SET_DAQ_DCLOWVOLT,
  SET_DAQ_TEMPCOMPVALUE,
  SET_DAQ_BLVDVOLT,
  SET_DAQ_BATTCAPACITYTOTAL,
  SET_DAQ_HIGHMAJORTEMPLEVEL,
  SET_DAQ_LOWTEMPLEVEL,
  SET_DAQ_ACLOWLEVEL,
  SET_DAQ_BATTCAPACITYTOTAL2,
  SET_DAQ_DCOVERVOLT,
  SET_DAQ_DCUNDERVOLT,
  SET_DAQ_ACUNDERLEVEL,
  SET_DAQ_ACHIGHLEVEL,
  SET_DAQ_BATTTESTVOLT,
  SET_DAQ_OVERMAJORTEMPLEVEL,
  SET_DAQ_TESTVOLTCFG,
  SET_DAQ_AUTOTESTDAY,
  SET_DAQ_TESTSTARTTIME,
  SET_DAQ_BTENDVOLT,
  SET_DAQ_BTENDCAP,
  SET_DAQ_BTENDTIME,
  SET_ACM_SYSMODE,
  SET_ACM_PHASENUM,
  SET_ACM_MININPUTVOLT,
  SET_ACM_AUTORESETMODE,
  SET_ACM_AUTORESETTIME,
  SET_ACM_WARMUPTIME,
  SET_ACM_COOLDOWNTIME,
  SET_ACM_DCLOWDETECTMODE,
  SET_ACM_DCLOWVOLT,
  SET_ACM_NIGHTMODEEN,
  SET_ACM_NIGHTMODESTART,
  SET_ACM_NIGHTMODEEND,
  SET_ACM_MAXRUNTIME,
  SET_ACM_GRIDSTABLETIME,
  SET_ACM_BLOCKNUM,
  SET_ACM_STARTBLOCK1,
  SET_ACM_GENRUNTIME,
  SET_ACM_GENEXTENDTIME,
  SET_ACM_DEFAULTSETTING,
  SET_ACM_STSGRIDRUNHOUR,
  SET_ACM_STSGRIDRUNTIME,
  SET_ACM_STSGENRUNHOUR,
  SET_ACM_STSGENRUNTIME,
  SET_ACM_DATE,
  SET_ACM_MONTH,
  SET_ACM_YEAR,
  SET_ACM_HOUR,
  SET_ACM_MINUTE,
  SET_ACM_SECOND,
  SET_ACM_CFGACLOWLEVEL,
  SET_ACM_CFGACHILEVEL,
  SET_ACM_CFGFREQLOWLEVEL,
  SET_ACM_CFGFREQHILEVEL,
  SET_ACM_CFGGENPMAX,
  SET_ACM_CFGABNORMALPOWER,
  SET_ACM_CFGFUELLOWLEVEL,
  SET_VAC_SYSMODE,
  SET_VAC_FANSPEEDMODE,
  SET_VAC_FANSTARTTEMP,
  SET_VAC_SETTEMP,
  SET_VAC_PIDOFFSETTEMP,
  SET_VAC_FANMINSPEED,
  SET_VAC_FANMAXSPEED,
  SET_VAC_FILTERSTUCKTEMP,
  SET_VAC_NIGHTMODEEN,
  SET_VAC_NIGHTMODESTART,
  SET_VAC_NIGHTMODEEND,
  SET_VAC_NIGHTMAXSPEED,
  SET_VAC_MANUALMODE,
  SET_VAC_MANUALMAXSPEED,
  SET_VAC_INMAXTEMP,
  SET_VAC_EXMAXTEMP,
  SET_VAC_FROSTMAXTEMP,
  SET_VAC_INMINTEMP,
  SET_VAC_EXMINTEMP,
  SET_VAC_FROSTMINTEMP,
  SET_VAC_MINOUTTEMP,
  SET_VAC_DELTATEMP,
  SET_VAC_PANICTEMP,
  SET_VAC_ACU1ONTEMP,
  SET_VAC_ACU2ONTEMP,
  SET_VAC_ACU2EN,
  SET_VAC_SYNCTIME,
  SET_VAC_AIRCON1MODEL,
  SET_VAC_AIRCON1TYPE,
  SET_VAC_AIRCON2MODEL,
  SET_VAC_AIRCON2TYPE,
  SET_VAC_AIRCONONOFF,
  SET_VAC_AIRCONMODE,
  SET_VAC_AIRCONTEMP,
  SET_VAC_AIRCONSPEED,
  SET_VAC_AIRCONDDIR,
  SET_VAC_W_ENABLE,
  SET_VAC_W_SERIAL1,
  SET_VAC_W_SERIAL2,
  SET_VAC_RESET,
  SET_BM_VOLTDIFF,
  SET_BM_MAXTEMP,
  SET_BM_VOLTTHRES,
  SET_BM_CURRTHRES,
  SET_BM_TIMETHRES,
  SET_BM_SOCTHRES,
  SET_BM_MINTEMP,
  SET_BM_LOWCAPTIME,
  SET_BM_HALFVOLTALARM,
  SET_BM_RECHARGEVOLT,
  SET_BM_CURRBALANCETHRES,
  SET_SMU_CFGACLOWLEVEL,
  SET_SMU_CFGACHILEVEL,
  SET_SMU_CFGFREQLOWLEVEL,
  SET_SMU_CFGFREQHILEVEL,
  SET_SMU_CFGOVERPOWERDELTA,
  SET_SMU_CFGPOWERDIFF,
  SET_SMU_CFGFUELLOWLEVEL,
  SET_SMCB_STATE,
  SET_GEN_STOPMODE,
  SET_GEN_STARTMODE,
  SET_GEN_MANUALMODE,
  SET_GEN_AUTOMODE,
  SET_LIB_PACKHIGHVOLT,
  SET_LIB_BATTHIGHVOLT,
  SET_LIB_CELLHIGHVOLT,
  SET_LIB_BATTLOWVOLT,
  SET_LIB_CELLLOWVOLT,
  SET_LIB_CHARHIGHCURR,
  SET_LIB_DISCHARHIGHCURR,
  SET_LIB_CHARHIGHTEMP,
  SET_LIB_DISCHARHIGHTEMP,
  SET_LIB_CHARLOWTEMP,
  SET_LIB_DISCHARLOWTEMP,
  SET_LIB_LOWCAP,
  SET_LIB_BMSHIGHTEMP,
  SET_LIB_BMSLOWTEMP,
  SET_LIB_PACKOVERVOLT,
  SET_LIB_BATTOVERVOLT,
  SET_LIB_CELLOVERVOLT,
  SET_LIB_BATTUNDERVOLT,
  SET_LIB_CELLUNDERVOLT,
  SET_LIB_CHAROVERCURR,
  SET_LIB_DISCHAROVERCURR,
  SET_LIB_CHAROVERTEMP,
  SET_LIB_DISCHAROVERTEMP,
  SET_LIB_CHARUNDERTEMP,
  SET_LIB_DISCHARUNDERTEMP,
  SET_LIB_UNDERCAP,
  SET_LIB_BMSOVERTEMP,
  SET_LIB_BMSUNDERTEMP,
  SET_LIB_DIFFERENTVOLT,
  SET_LIB_VOLTBALANCE,
  SET_LIB_DELTAVOLTBALANCE,
  SET_LIB_DISCHARCURRLIMIT,
  SET_LIB_CHARCURRLIMIT,
  SET_LIB_VOLTDISCHARREF,
  SET_LIB_VOLTDISCHARCMD,
  SET_LIB_SYSTEMBYTE,
  SET_LIB_KEYTIME,
  SET_LIB_INIMAXIM,
  SET_LIB_ENABLEKCS,
  SET_LIB_ENABLELOCK,
  SET_LIB_CAPINIT,
  SET_FAN_DPC_STARTING_POINT,
  SET_FAN_DPC_SENSITIVE_POINT,
  SET_FAN_DPC_H_TEMP_W_POINT,
  SET_FAN_DPC_L_TEMP_W_POINT,
  SET_FAN_DPC_HEATING_START_POINT,
  SET_FAN_DPC_HEATING_SENSITIVE_POINT
  
}RTU_CMD_SET_STATE;
//typeedef struct---------------------------------------------------------------
//alarm---------------------------------------
typedef struct
{
  uint8_t   u8AlarmValue;
  uint8_t   u8NormalValue;
}tAlarmValue;
extern tAlarmValue sAlarmValue[128];

// system flag---------------------------------
typedef struct{
  uint8_t   u8IsRewriteSN;
  uint8_t   led_user_blink;
  uint8_t   u8IsRebootFlag;
  uint32_t  cnt_reboot;
  uint16_t  cnt_time_bnt_default;
  uint8_t   flag_bnt;
  ETH_TYPE  eth_type;
}SYSTEM_FLAG_STRUCT;
extern SYSTEM_FLAG_STRUCT  system_flag_struct;
//cnt struct------------------------------------


//flag struct-----------------------------------
typedef struct {
  uint8_t                      save_config_flag;
}FLAG_STRUCT;

extern FLAG_STRUCT flag_struct;
//time struct----------------------------------- 
typedef struct{
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
  unsigned char day;
  unsigned char date;
  unsigned char month;
  unsigned char year;
}TIME_FORMAT_STRUCT;
extern TIME_FORMAT_STRUCT sync_time,EMERV21Time,ZTETime;

//bat test info---------------------------------
typedef struct
{
  float fBattTestVolt;
  float fBattTestDur;
  float fTestEndCap;
  
  float fStdTestEndVolt;
  uint8_t u8StdCellNo;
  float fStdTestTime;
  float fShortTestEndVolt;
  uint8_t u8ShortTestTime;
  
  uint32_t u32BattTestVolt;
  uint32_t u32BattTestDur;
  uint32_t u32TestEndCap;
  uint8_t u8TimeTestEn;
  TIME_FORMAT_STRUCT sPlanTest[4];
  TIME_FORMAT_STRUCT sDCTime;
  
  char cPlanTest1[15];
  char cPlanTest2[15];
  char cPlanTest3[15];
  char cPlanTest4[15];
  
  char cDCTime[21];
  
  uint8_t u8PlanTest1[3];
  uint8_t u8PlanTest2[3];
  uint8_t u8PlanTest3[3];
  uint8_t u8PlanTest4[3];
  uint8_t u8SetPlan;
  uint8_t u8SetSCU;
  
  uint8_t u8CheckSentAll;
  uint8_t u8CheckValidAll;
  
  uint32_t	u32STNoIdx;
  uint32_t	*pui32STNoIdx;
  uint32_t	u32STTableIdx;
  
  uint32_t	u32PLNoIdx;
  uint32_t	*pui32PLNoIdx;
  uint32_t	u32PLTableIdx;
  
  uint32_t *pui32BattTestVolt,
  *pui32BattTestDur,
  *pui32TestEndCap;
  uint8_t	 *pui8TimeTestEn;
}
tBattTestInfo;
extern tBattTestInfo sBattTestInfo;

// gen manager struct---------------------------
typedef struct{
  uint8_t                                  gen_array_index;
  uint8_t                                  gen_no_response;
  uint32_t                                 gen_slave_offset;
  uint32_t                                 gen_abs_slave_id;
  uint8_t                                  gen_type;
  RTU_REQ_STATE                            gen_running_step;
  uint32_t                                 gen_base_addr[4];
  uint8_t                                  gen_num_of_reg[4];
}GEN_MANAGER_STRUCT;

// pm manager struct----------------------------
typedef struct{
  uint8_t                                  pm_array_index;
  uint8_t                                  pm_no_response;
  uint32_t                                 pm_slave_offset;
  uint32_t                                 pm_abs_slave_id;
  uint8_t                                  pm_type;
  RTU_REQ_STATE                            pm_running_step;
  uint32_t                                 pm_base_addr[10];
  uint8_t                                  pm_num_of_reg[10];
}PM_MANAGER_STRUCT;
// smcb manager struct--------------------------
typedef struct{
  uint8_t                                  smcb_array_index;
  uint8_t                                  smcb_no_response;
  uint32_t                                 smcb_slave_offset;
  uint32_t                                 smcb_abs_slave_id;
  uint8_t                                  smcb_type;
  RTU_REQ_STATE                            smcb_running_step;
  uint32_t                                 smcb_base_addr[2];
  uint8_t                                  smcb_num_of_reg[2];
}SMCB_MANAGER_STRUCT;

// fuel manager struct--------------------------
typedef struct{
  uint8_t                                  fuel_array_index;
  uint8_t                                  fuel_no_response;
  uint32_t                                 fuel_slave_offset;
  uint32_t                                 fuel_abs_slave_id;
  uint8_t                                  fuel_type;
  RTU_REQ_STATE                            fuel_running_step;
  uint32_t                                 fuel_base_addr[2];
  uint8_t                                  fuel_num_of_reg[2];
}FUEL_MANAGER_STRUCT;

// isense manager struct--------------------------
typedef struct{
  uint8_t                                  isense_array_index;
  uint8_t                                  isense_no_response;
  uint32_t                                 isense_slave_offset;
  uint32_t                                 isense_abs_slave_id;
  uint8_t                                  isense_type;
  RTU_REQ_STATE                            isense_running_step;
  uint32_t                                 isense_base_addr[2];
  uint8_t                                  isense_num_of_reg[2];
}ISENSE_MANAGER_STRUCT;

// isense manager struct--------------------------
typedef struct{
  uint8_t                                  pmdc_array_index;
  uint8_t                                  pmdc_no_response;
  uint32_t                                 pmdc_slave_offset;
  uint32_t                                 pmdc_abs_slave_id;
  uint8_t                                  pmdc_type;
  RTU_REQ_STATE                            pmdc_running_step;
  uint32_t                                 pmdc_base_addr[6];
  uint8_t                                  pmdc_num_of_reg[6];
}PMDC_MANAGER_STRUCT;

// vac manager struct---------------------------
typedef struct{
  uint8_t                                  vac_array_index;
  uint8_t                                  vac_no_response;
  uint32_t                                 vac_slave_offset;
  uint32_t                                 vac_abs_slave_id;
  uint8_t                                  vac_type;
  RTU_REQ_STATE                            vac_running_step;
  uint32_t                                 vac_base_addr[4];
  uint8_t                                  vac_num_of_reg[4]; 
}VAC_MANAGER_STRUCT;

// lib manager struct---------------------------
typedef struct{
  uint8_t                                  lib_array_index;
  uint8_t                                  lib_no_response;
  uint32_t                                 lib_slave_offset;
  uint32_t                                 lib_abs_slave_id;
  uint8_t                                  lib_type;
  RTU_REQ_STATE                            lib_running_step;
  uint32_t                                 lib_base_addr[4];
  uint8_t                                  lib_num_of_reg[4]; 
}LIB_MANAGER_STRUCT;

// bm manager struct---------------------------
typedef struct{
  uint8_t                                  bm_array_index;
  uint8_t                                  bm_no_response;
  uint32_t                                 bm_slave_offset;
  uint32_t                                 bm_abs_slave_id;
  uint8_t                                  bm_type;
  RTU_REQ_STATE                            bm_running_step;
  uint32_t                                 bm_base_addr[4];
  uint8_t                                  bm_num_of_reg[4]; 
}BM_MANAGER_STRUCT;

//vac info struct-----------------------------
typedef struct
{    
  uint8_t                                  no_response_cnt;
  uint8_t                                  error_flag;
  
  /* Status Variables */
  int16_t                                  in_temp;
  int16_t                                  ex_temp;
  int16_t                                  frost1_temp;
  int16_t                                  frost2_temp;
  uint32_t                                 humid;
  uint32_t                                 fan1_duty;
  uint32_t                                 fan1_rpm;
  uint32_t                                 fan2_duty;
  uint32_t                                 fan2_rpm;
  uint32_t                                 alarm_status;
  uint16_t                                 firm_version;

  uint16_t                                 sn_lo;
  uint16_t                                 sn_hi;
  /* Setting Variables */
  uint32_t                                 sys_mode;
  uint32_t                                 fan_speed_mode;
  int16_t                                  fan_start_temp;
  int16_t                                  set_temp;
  int16_t                                  pid_offset_temp;
  uint32_t                                 fan_min_speed;
  uint32_t                                 fan_max_speed;
  int16_t                                  filter_stuck_temp;
  uint32_t                                 night_mode_en;
  uint32_t                                 night_mode_start;
  uint32_t                                 night_mode_end;
  uint32_t                                 night_max_speed;
  uint32_t                                 manual_mode;
  uint32_t                                 manual_max_speed;
  int16_t                                  in_max_temp;
  int16_t                                  ex_max_temp;
  int16_t                                  frost_max_temp;
  int16_t                                  in_min_temp;
  int16_t                                  ex_min_temp;
  int16_t                                  frost_min_temp;
  int16_t                                  min_out_temp;
  int16_t                                  delta_temp;
  int16_t                                  panic_temp;
  int16_t                                  acu1_on_temp;
  int16_t                                  acu2_on_temp;
  uint32_t                                 acu2_en;
  uint32_t                                 air_con1_model;
  uint32_t                                 air_con1_type;
  uint32_t                                 air_con2_model;
  uint32_t                                 air_con2_type;
  uint32_t                                 air_con_on_off;
  uint32_t                                 air_con_mode;
  int16_t                                  air_con_temp;
  uint32_t                                 air_con_speed;
  uint32_t                                 air_cond_dir;
  uint32_t                                 air_con_volt;
  uint32_t                                 air_con_current;
  uint32_t                                 air_con_power;
  uint32_t                                 air_con_frequency;
  uint32_t                                 air_con_pf;
  uint32_t                                 real_time_sync;
  uint32_t                                 type;
  uint32_t                                 serial;
  uint32_t                                 modbusID;
  
  uint16_t                                 active_fan;
  uint16_t                                 installed_fan;
  uint16_t                                 installed_air_con;
  
  uint32_t                                 w_enable;
  uint32_t                                 w_serial1;
  uint32_t                                 w_serial2;
  
  uint8_t                                  vac_update_step;
  uint8_t                                  vac_reset_ok;
}VAC_INFO_STRUCT;

//lib info struct------------------------------
typedef struct{
  uint32_t u32PackVolt;
  uint32_t u32BattVolt;
  uint16_t u16CellVolt[20];
  int32_t u32PackCurr;
  int16_t u16PackCurr;
  uint16_t u16CellTemp[20];
  
  uint16_t u16MinorAlarm;
  uint16_t u16MajorAlarm;
  
  uint16_t u16Protect1;
  uint16_t u16Protect2;
  
  uint16_t u16ModuleMode;
  uint16_t u16ModuleAlarm;
  uint16_t u16SoftwareVersion;
  uint16_t u16HardwareVersion;
  
  uint32_t u32AlarmStatus;
  uint32_t u32ProtectStatus;
  uint32_t u32SOC;
  uint32_t u32SOH;
  uint32_t u32DischargeTime; 
  
  uint32_t u32CapRemain;
  uint16_t u16AverTempCell;
  uint16_t u16EnvTemp;
  uint32_t u32FaultStatus;
  uint32_t u32Serial;
  
  uint8_t u8hwversion[40];
  uint8_t u8swversion[40];
  uint8_t u8mode[61];
  uint16_t sw_ver;    //thanhcm3 add fix bms Vietel 2 version 1.10, 1.06
  uint32_t reg_152;    //thanhcm3 add fix bms Vietel 2 version 1.10, 1.06  
  
  float fPackVolt;
  float fPackCurr;
  float fCapRemain;
  float fAverTempCell;
  float fEnvTemp;
  float fSOC;
  uint8_t u8NoResponseCnt;
  uint8_t u8ErrorFlag;
  uint16_t u16BattType;  
  
  //----------------------M1---------------------------------//
  uint16_t u16ConverterState;
  uint16_t u16ErrCode;
  uint16_t u16BalanceStatus;
  uint16_t u16MosfetMode;
  uint32_t u32Mcu2McuErr;
  uint32_t u32CHGCapacity;
  uint32_t u32DSGCapacity;
  uint32_t u32Efficiency;
  uint16_t u16NumberOfCan;
  //===Alarm
  int32_t i32PackHighVolt_A;
  int32_t i32BattHighVolt_A;
  int32_t i32CellHighVolt_A;
  int32_t i32BattLowVolt_A;
  int32_t i32CellLowVolt_A;
  int32_t i32CharHighCurr_A;
  int32_t i32DisCharHighCurr_A;
  int32_t i32CharHighTemp_A;
  int32_t i32DisCharHighTemp_A;
  int32_t i32CharLowTemp_A;
  int32_t i32DisCharLowtemp_A;
  int32_t i32LowCap_A;
  int32_t i32BMSHighTemp_A;
  int32_t i32BMSLowTemp_A;
  //===Protect
  int32_t i32PackOverVolt_P;
  int32_t i32BattOverVolt_P;
  int32_t i32CellOverVolt_P;
  int32_t i32BattUnderVolt_P;
  int32_t i32CellUnderVolt_P;
  int32_t i32CharOverCurr_P;
  int32_t i32DisCharOverCurr_P;
  int32_t i32CharOverTemp_P;
  int32_t i32DisCharOverTemp_P;
  int32_t i32CharUnderTemp_P;
  int32_t i32DisCharUnderTemp_P;
  int32_t i32UnderCap_P;
  int32_t i32BMSOverTemp_P;
  int32_t i32BMSUnderTemp_P;
  int32_t i32DifferentVolt_P;
  //===
  uint32_t u32VoltBalance;
  uint32_t u32DeltaVoltBalance;
  uint32_t u32DisCharCurrLimit;
  uint32_t u32CharCurrLimit;
  uint32_t u32VoltDisCharRef;
  uint32_t u32VoltDisCharCMD;
  uint16_t u16SystemByte;
  uint16_t u16KeyTime;
  uint16_t u16IniMaxim;
  uint16_t u16EnableKCS;
  uint16_t u16EnableLock;
  
  uint32_t u32ADCPackVolt;
  uint32_t u32ADCBattVolt;
  int32_t i32ADCPackCurr;
  int32_t i32ADCBattCurr;
  
  uint16_t u16SOCMaxim;
  uint16_t u16SOHMaxim;
  uint16_t u16FullCapRepMaxim;
  uint16_t u16VoltMaxim;
  uint16_t u16CurrMaxim;
  
  uint32_t u32IKalamn;
  uint32_t u32SOCKalamn;
  uint32_t u32VpriKalamn;
  uint32_t u32VmesKalamn;
  uint32_t u32CapKalamn;
  uint32_t u32InternalR0Kalamn;
  uint16_t u16_liionCapInit;
  //----------------------M1---------------------------------//
  
}LIB_INFO_STRUCT;

//gen info struct---------------------------------------------
typedef struct
{    
  uint8_t u8NoResponseCnt;
  uint8_t u8ErrorFlag;
  
  uint32_t u32ID;
  uint32_t u32Status;
  uint32_t u32LNVolt1;
  uint32_t u32LNVolt2;
  uint32_t u32LNVolt3;
  uint32_t u32LNCurr1;
  uint32_t u32LNCurr2;
  uint32_t u32LNCurr3;
  uint32_t u32LNPower1; // KW
  uint32_t u32LNPower2;
  uint32_t u32LNPower3;
  int32_t u32LNSPower1; //KVA
  int32_t u32LNSPower2;
  int32_t u32LNSPower3;
  uint32_t u32MainFrequency;
  uint32_t u32GenFrequency;
  uint32_t u32BattVolt;
  uint32_t u32CoolantTemp;
  uint32_t u32OilPressure;
  uint32_t u32FuelLevel;
  uint32_t u32RunTime;
  uint32_t u32MaintenanceTime;
  uint32_t u32EngineSpeed;
  uint32_t u32EngineWorkingHour;
  uint32_t u32EngineWorkingMin;
  uint32_t u32ActiveEnergyLow;
  uint32_t u32ActiveEnergyHigh;  
  float fPF1;
  float fPF2;
  float fPF3;  
  uint32_t u32AlarmStatus;
  uint16_t u16Error1Status;
  uint16_t u16Error2Status;
  uint16_t u16ErrorBattVolt;
  uint16_t u16StatusBits1;
  uint16_t u16StatusBits2;
  uint16_t oil_temperature;

}
GEN_INFO_STRUCT;

//phase info struct------------------------------
typedef struct
{
  float fVoltage;
  float fCurrent;
  float fActivePower;
  float fReactivePower;
  float fApparentPower;
  float fPowerFactor; 
  float fFrequency; 
}
tPhaseInfo;

//pm info struct-----------------------------
typedef struct
{
  uint8_t u8NoResponseCnt;
  uint8_t u8ErrorFlag;
  
  float fRealEnergy;
  float fApparentEnergy;
  float fReactiveEnergy;
  float fRealPower;
  float fApparentPower;
  float fReactivePower;
  float fPowerFactor;
  float fFrequency;
  
  float fNeutralCurrent;
  float fTotalCurrent;
  
  int32_t u32ImportActiveE;
  int32_t u32ExportActiveE;
  int32_t u32NetActiveE;
  int32_t u32TotalActiveE;
  
  int32_t u32ImportReActiveE;
  int32_t u32ExportReActiveE;
  int32_t u32NetReActiveE;
  int32_t u32TotalReActiveE;
  
  uint8_t u8Model[20];
  uint32_t u32SerialNumber;
  tPhaseInfo sPhaseInfo[3];
}
PM_INFO_STRUCT;

//SMBC--------------------------------------
typedef struct
{    
  uint8_t u8NoResponseCnt;
  uint8_t u8ErrorFlag;  
  
  uint32_t u32State;
}
SMBC_INFO_STRUCT;

//FUEL -------------------------------------
typedef struct
{    
  uint8_t u8NoResponseCnt;
  uint8_t u8ErrorFlag;  
  
  uint32_t u32FuelLevel;
}
FUEL_INFO_STRUCT;
//ISENSE------------------------------------
typedef struct
{
  uint8_t u8NoResponseCnt;
  uint8_t u8ErrorFlag;
  
  float fVoltage;
  float fCurrent;
  float fRealEnergy;
  float fApparentEnergy;
  float fReactiveEnergy;
  float fApparentPower;
  float fReactivePower;
  float fActivePower;
  float fPowerFactor;
  float fFrequency;
  
  int32_t u32TotalActiveE;
  int32_t u32TotalReActiveE;
  
  uint8_t u8Model[20];
  uint32_t u32SerialNumber;
}
ISENSE_INFO_STRUCT;
//BM------------------------------------------
typedef struct
{    
  uint8_t u8NoResponseCnt;
  uint8_t u8ErrorFlag;  
  
  uint32_t u32BattVolt;
  uint32_t u32PackVolt;
  uint16_t u16CellVolt[4];
  int32_t i32PackCurr;
  uint16_t u16CellTemp[6];
  
  uint32_t u32AlarmStatus;
  uint32_t u32BalanceStatus;
  uint16_t u16BattStatus;
  
  uint16_t u16SOC;
  uint32_t u32SOH;
  uint32_t u32DischargeTime; 
  uint32_t u32TotalRunTime;
  
  uint32_t u32VoltDiff;
  uint32_t u32MaxTemp;
  uint32_t u32VoltThres;
  uint32_t u32CurrThres;
  uint32_t u32TimeThres;
  uint32_t u32SOCThres; 
  uint32_t u32MinTemp;
  uint32_t u32LowCapTime;
  uint32_t u32TotalDisAH;
  uint32_t u32HalfVoltAlarm;
  uint32_t u32RechargeVolt;
  uint32_t u32CurrBalanceThres;
  
  uint8_t cSerialNo[8];
}
BM_INFO_STRUCT;
//PMDC---------------------------------------
typedef struct{
  uint8_t   u8_no_response_cnt;
  uint8_t   u8_error_flag;
  float     f_voltage;
  float     f_current;
  float     f_active_power;
  float     f_active_energy;
  
  uint8_t   u8_model[20];
  
}PMDC_INFO_STRUCT;
// modbus manager struct----------------------
typedef struct{
  //cmd setting slave------------------------------------------
  uint32_t                                    setting_cmd;
  //slave gen--------------------------------------------------
  uint8_t                                     number_of_gen;
  uint8_t                                     gen_current_index;
  GEN_MANAGER_STRUCT                          gen_manager_struct[2];
  GEN_INFO_STRUCT                             gen_info_struct[2];
  uint32_t                                    cnt_dis_connect_gen[2];
  //slave lib--------------------------------------------------
  uint8_t                                     number_of_lib;
  uint8_t                                     lib_current_index;
  LIB_MANAGER_STRUCT                          lib_manager_struct[16];
  LIB_INFO_STRUCT                             lib_info_struct[16];
  uint32_t                                    cnt_dis_connect_lib[16];
  //slave bm---------------------------------------------------
  uint8_t                                     number_of_bm;
  uint8_t                                     bm_current_index;
  BM_MANAGER_STRUCT                           bm_manager_struct[16];
  BM_INFO_STRUCT                              bm_info_struct[16];
  uint32_t                                    cnt_dis_connect_bm[16];
  //slave vac--------------------------------------------------
  uint8_t                                     number_of_vac;
  uint8_t                                     vac_current_index;
  VAC_MANAGER_STRUCT                          vac_manager_struct[2];
  VAC_INFO_STRUCT                             vac_info_struct[2];
  uint32_t                                    cnt_dis_connect_vac[2];
  //slave pm ------------------------------------------------
  uint8_t                                     number_of_pm;
  uint8_t                                     pm_current_index;
  PM_MANAGER_STRUCT                           pm_manager_struct[2];
  uint8_t                                     u8PMBootPassFlag;
  PM_INFO_STRUCT                              pm_info_struct[2];
  uint32_t                                    cnt_dis_connect_pm[2];
  //slave smcb-------------------------------------------------
  uint8_t                                     number_of_smcb;
  uint8_t                                     smcb_current_index;
  SMCB_MANAGER_STRUCT                         smcb_manager_struct[5];
  SMBC_INFO_STRUCT                            smcb_info_struct[5];
  uint32_t                                    cnt_dis_connect_smbc[5];
  //slave fuel-------------------------------------------------
  uint8_t                                     number_of_fuel;
  uint8_t                                     fuel_current_index;
  FUEL_MANAGER_STRUCT                         fuel_manager_struct[2];
  FUEL_INFO_STRUCT                            fuel_info_struct[2];
  uint32_t                                    cnt_dis_connect_fuel[2];
  //slave isense-----------------------------------------------
  uint8_t                                     number_of_isense;
  uint8_t                                     isense_current_index;
  ISENSE_MANAGER_STRUCT                       isense_manager_struct[1];
  ISENSE_INFO_STRUCT                          isense_info_struct[1];
  uint32_t                                    cnt_dis_connect_isense[1];
  //slave pm dc------------------------------------------------
  uint8_t                                     number_of_pmdc;
  uint8_t                                     pmdc_current_index;
  PMDC_MANAGER_STRUCT                         pmdc_manager_struct[1];
  PMDC_INFO_STRUCT                            pmdc_info_struct[1];
  uint32_t                                    cnt_dis_connect_pmdc[1];
  //slave future expansion-------------------------------------
  
}MODBUS_MANAGER_STRUCT;
extern MODBUS_MANAGER_STRUCT modbus_manager_struct;
extern uint8_t LibM1Addr;



//agisson======================================================================
typedef struct
{
  uint8_t InitUart;
  uint8_t Cnt;
  uint8_t requesttestRS485;
  uint8_t Numberofbyterecvok;
  uint8_t RS485CabRecv;
  uint8_t RS485GenRecv;
  uint8_t CabRecvCnt;
  uint8_t GenRecvCnt; 
  uint8_t CabBuff[15];
  uint8_t GenBuff[15];
  uint8_t Test485LoopOk;
}
ttestloop;
extern ttestloop sTestRS485;

extern uint32_t iSet;
extern uint8_t Program_Status;
extern uint8_t Program_Status_old;
extern uint32_t RS485Timer;
extern uint32_t RS485Command;
extern uint32_t RecvCntEnd485;
extern uint32_t RecvOK;
extern uint32_t RecvCodeOK;      
extern uint8_t u8HisBuffer[256];
extern unsigned char g_UARTRxBuf485[300];
extern uint32_t RecvCntStart485;
extern uint32_t setuprepeat;

extern uint32_t iCounter;

extern uint8_t Recv03;
extern uint32_t RX485Count;
extern uint32_t settingCommandmask;
extern uint32_t settingCommand;
extern uint8_t setCmd_flag;
extern uint32_t setCmd_mask;
extern uint8_t setCmd_Ready;
extern uint8_t RecvComplete;
extern uint8_t  writeFlash;
extern uint8_t TYPE;

//DC-----------------------------------------------------------
typedef struct
{
  uint8_t u8DC;		// DC Alarm
  uint8_t u8BatVol[2]; // Batt low voltage
  uint8_t u8DC_NoFuse; // Number of Load Fuse
  uint8_t u8Batt1_OC; // Batt1 Over Current Alarm
  uint8_t u8Batt2_OC; // Batt2 Over Current Alarm
  uint8_t u8BLVD; // BLVD Alarm
  uint8_t u8LLVD; // LLVD Alarm
  uint8_t u8LLVD2; // LLVD Alarm
  uint8_t u8Sen1_BattTemp; // Battery-Configured TempSensor Alarm
  uint8_t u8Sen1_BattInvalid; // Battery TempSensor Invalid Alarm
  uint8_t u8Sen2_BattTemp; // Battery-Configured TempSensor Alarm
  uint8_t u8Sen2_BattInvalid; // Battery TempSensor Invalid Alarm
  uint8_t u8Sen3_BattTemp;
  uint8_t u8Sen3_BattInvalid; // Battery TempSensor Invalid Alarm
  uint8_t u8Sen1_AmbTemp; // Ambient-Configured TempSensor Alarm
  uint8_t u8Sen2_AmbTemp; // Ambient-Configured TempSensor Alarm
  uint8_t u8Batt_Discharge; // Battery discharge Alarm
  uint8_t u8OutVoltFault; // Output Voltage Fault Alarm
  uint8_t u8LoadFuse[100],
  u8BattFuse[4],
  u8DIFuse[8];
  uint8_t u8NumOfUsrDefInfo;
  uint8_t u8DCSPD;		// DC Alarm
  uint8_t batt_low_volt_alarm;
}
tALARMDcInfo;
typedef struct
{
  float fVoltage;
  float fCurrent;
  uint8_t u8BatNo;
  float fBatt1Curr;
  float fBatt2Curr;
  float fBatt3Curr;
  float fBatt4Curr;     //thanhcm3 add for dpc
  float fBatt1Volt;
  float fBatt2Volt;
  float fBatt3Volt;
  float fBatt4Volt;     //thanhcm3 add for dpc
  float fBatt1RmnCap;
  float fBatt2RmnCap;
  float fBatt3RmnCap;   //thanhcm3 add for dpc
  float fBatt4RmnCap;   //thanhcm3 add for dpc
  float fSen1BattTemp;
  float fSen2BattTemp;
  float fSen3BattTemp;
  float fSen4BattTemp;  //thanhcm3 add for dpc
  float fSen1AmbTemp;
  float fSen2AmbTemp;
  float fHumidity;     //thanhcm3 add for dpc
  float fcustomized;   //thanhcm3 add for dpc
}
tINFODcInfo;
typedef struct
{
  tALARMDcInfo sALARMDcInfo;
  tINFODcInfo sINFODcInfo;
  uint32_t u32DCNoResponse;
}
tDcInfo;
extern tDcInfo sDcInfo;
//AC--------------------------------------------------------------------
typedef struct
{
  int32_t	s32acVolt[3];
  float	facVolt[3];
  float   f_hz_in;             //thanhcm3 add for dpc
  float   f_ac_curr[3];        //thanhcm3 add for dpc
  uint8_t u8Thres[3];          
  uint8_t u8_I_Thres[3];
  uint8_t u8_hz;               //thanhcm3 add for dpc
  uint8_t u8MainFail;
  uint8_t u8PowerOff;
  uint8_t u8ACSPD;
  float fAcLowThres;
  float fAcHighThres;
  float fAcInputCurrLimit;
  float fAcUnderThres;
  float f_ac_high_hz;         //thanhcm3 add for dpc
  float f_ac_low_hz;          //thanhcm3 add for dpc
  
}
tAcInfo;
extern tAcInfo sAcInfo;
//BATT--------------------------------------------------------------
typedef struct
{
  float fFltVoltCfg; //Float Voltage
  float fBotVoltCfg; //Boost Voltage
  float fTestVoltCfg; //Test Voltage
  float fTempCompVal; //TempComp Value
  float fLoMjAlrmVoltCfg; //LLVD
  float fLVDDV; //BLVD
  float fLoMnAlrmVoltCfg; //DC Low
  float fDCUnderCfg; //DC Under
  float fDCOverCfg; //DC Over
  
  uint8_t u8BankNo; //Battery String
  float fCapTotal; //Battery Standard Capacity
  float fCapTotal2; //Battery Standard Capacity
  float fCapTotal3; // thanhcm3 add for dpc
  float fCapTotal4; // thanhcm3 add for dpc
  float fCapTotal_all;//thanhcm3 add for dpc
  float fCCLVal;
  float fCapCCLVal;
  float fHiMjTempAlrmLevel;
  float fOvMjTempAlrmLevel;
  float fAutoTestDay;
  float fTestStartTime;
  uint8_t u8LLVDE;
  uint8_t u8BLVDE;
}
tRAWBattInfo;
typedef struct
{
  tRAWBattInfo	sRAWBattInfo;
  //	tSNMPBattInfo	sSNMPBattInfo;
}
tBattInfo;
extern tBattInfo sBattInfo;
//RECT-----------------------------------------------------------------
typedef struct
{            
  float fRect_DcCurr;
  float fRect_Temp;
  float fRect_LimCurr;
  float fRect_DcVolt;
  float fRect_AcVolt;
  uint32_t u32Rect_SN;
  
  uint8_t u8Rect_ACFault;
  uint8_t u8Rect_NoResp;
  uint8_t u8Rect_Fail;
  
  uint8_t u8Rect_Out;
  uint8_t u8Rect_Sts;
  uint8_t u8Rect_OnPosition;
  uint8_t rect_sts_current_limit;  //thanhcm3 add for dpc
  uint8_t rect_sts_fl_eq_testing;  //thanhcm3 add for dpc 
  uint8_t rect_dc_over_volt;
  uint8_t rect_ac_over_volt;
  uint8_t rect_fan_fault;
  uint8_t rect_over_temp;
  uint8_t rect_dc_over_curr;   
}
tRAWRectifierParameters;

typedef struct
{
  uint8_t u8Rect_Num;
  float fAllRectDcVolt;
  uint8_t u8Rect_InfoNum;
  
  uint32_t	u32rectNoIdx;
  uint32_t*	pui32rectNoIdx;
  
  uint32_t 	u32rectInstRect,
  u32rectActRect,
  u32rectTotCurr,
  u32rectUtlz;
  int8_t  i8NumRectChanged;
  uint8_t u8NumRectlist;
  
  
  uint8_t u8rectInfolen;
  
  uint8_t* pui8RectType_len;
  uint8_t* pui8RectSN_len;
  uint8_t* pui8RectRev_len;
  
  uint8_t u8WITE;
  uint8_t u8WITI;
  tRAWRectifierParameters sRAWRectParam[24];
  //	tSNMPRectifierParameters sSNMPRectParam[6];
  //	tREPORTRectifierParameters sREPORTRectParam[6];
}
tRectifierInfo;
extern tRectifierInfo sRectInfo;
//-------------------------------------------------------------
#define EMER 0
#define ZTE 1
#define HW 2
#define EMERV21 3
#define DKD51_BDP 4

#define SET_LLVD				0x00000001
#define SET_BLVD				0x00000002
#define SET_DCLOW				0x00000004
#define SET_BATTSTDCAP			        0x00000008
#define SET_CCL					0x00000010
#define SET_WIT_EN				0x00000020
#define SET_WIT_VAL				0x00000040
#define SET_FLTVOL				0x00000080
#define SET_BSTVOL				0x00000100
#define SET_OVERTEMP			        0x00000200
#define SET_TEMPCOMP_VAL		        0x00000400
#define SET_AC_THRES 			        0x00000800
#define SET_AC_LOW_THRES 		        0x00000800
#define SET_AC_HIGH_THRES 		        0x00001000
#define SET_AC_INPUT_CURR_LIMIT                 0x00002000
#define SET_BATTSTDCAP2 		        0x00004000

#define SET_BT_PLANTEST 		        0x00008000
#define SET_BT_ENDCAP			        0x00010000
#define SET_BT_ENDTIME			        0x00020000
#define SET_BT_ENDVOLT			        0x00040000
#define SET_BT_PLEN				0x00080000
#define SET_BT_SCUTIME			        0x00100000

#define SET_BT_CELLNO                           0x00200000
#define START_BT_TEST                           0x00400000
#define STOP_BT_TEST                            0x00800000

#define SET_TEST_VOLT                           0x01000000
#define SET_AUTO_TEST_DAY                       0x02000000
#define SET_TEST_START_TIME                     0x04000000

#define SET_DCUNDER				0x08000000
#define SET_LLVD_EN				0x10000000
#define SET_BLVD_EN				0x40000000
#define SET_DCOVER				0x10000000
#define SET_BATTSTDCAP3                         0x20000000  //thanhcm add for dpc 
#define SET_BATTSTDCAP4                         0x80000000  //thanhcm3 add for dpc

//---------------------------------------------------------------------------------
typedef enum
{
// phan danh cho tu ZTE
TIMEINFO_REQ,
TIMEINFO_RES,
MANUFACTURERINFO_REQ,
MANUFACTURERINFO_RES,
COMMONPARA_REQ,
COMMONPARA_RES,
ALARMLEVEL_REQ,
ALARMLEVEL_RES,

ACINPUTINFO_REQ,
ACINPUTINFO_RES,
ACALARM_REQ,
ACALARM_RES,
ACPARAMETER_REQ,
ACPARAMETER_RES,

RECTSTSZTE_REQ,
RECTSTSZTE_RES,
RECTINFOZTE_REQ,
RECTINFOZTE_RES,
RECTALARM_REQ,
RECTALARM_RES,
RECTREMOTECTL_REQ,
RECTREMOTECTL_RES,

DCOUTPUT_REQ,
DCOUTPUT_RES,
DCALARM_REQ,
DCALARM_RES,
DCPARAMETER_REQ,
DCPARAMETER_RES,
ENVDATA_REQ,
ENVDATA_RES,
ENVALARM_REQ,
ENVALARM_RES,
// ket thuc phan danh cho tu ZTE
SYSINFO_REQ,
SYSINFO_RES,
RECTINFO_REQ,
RECTINFO_RES,
ACINFO_REQ,
ACINFO_RES,
ALARM_DC_REQ,
ALARM_DC_RES,
ALARM_RECT_REQ,
ALARM_RECT_RES,
ALARM_AC_REQ,
ALARM_AC_RES,
ALARM_MAN_REQ,
ALARM_MAN_RES,
BATT_FLOATVOLTAGE_REQ,
BATT_FLOATVOLTAGE_RES,
BATT_BOOSTVOLTAGE_REQ,
BATT_BOOSTVOLTAGE_RES,
BATT_TEMPCOMPEN_REQ,
BATT_TEMPCOMPEN_RES,
BATT_LLVDEN_REQ,
BATT_LLVDEN_RES,
BATT_BLVDEN_REQ,
BATT_BLVDEN_RES,
BATT_LLVD_REQ,
BATT_LLVD_RES,
BATT_BLVD_REQ,
BATT_BLVD_RES,
BATT_DCLOW_REQ,
BATT_DCLOW_RES,
BATT_DCUNDER_REQ,
BATT_DCUNDER_RES,
BATT_DCOVER_REQ,
BATT_DCOVER_RES,
BATT_STRING_REQ,
BATT_STRING_RES,
BATT_STDCAP_REQ,
BATT_STDCAP_RES,
BATT_CURRLIMIT_REQ,
BATT_CURRLIMIT_RES,
BATT_HIGHTEMPALARM_REQ,
BATT_HIGHTEMPALARM_RES,
BATT_OVERTEMPALARM_REQ,
BATT_OVERTEMPALARM_RES,
SYS_WALKINTIMEDUR_REQ,
SYS_WALKINTIMEDUR_RES,
SYS_WALKINTIMEEN_REQ,
SYS_WALKINTIMEEN_RES,
ATSINFO_REQ,
ATSINFO_RES,
CHARGERINFO_REQ,
CHARGERINFO_RES,
ALARM_CHARGER_REQ,
ALARM_CHARGER_RES,
ETHSYNC_REQ,
ETHSYNC_RES,
UPDATE_OK,
TEST_MODE_REQ,
TEST_MODE_RES,
HISTORYMESG_REQ,
HISTORYMESG_RES,
AC_LOWTHRES_REQ,
AC_LOWTHRES_RES,
AC_UNDERTHRES_REQ,
AC_UNDERTHRES_RES,
AC_HITHRES_REQ,
AC_HITHRES_RES,
BATT_TESTVOLT_REQ,
BATT_TESTVOLT_RES,
BATT_TESTDUR_REQ,
BATT_TESTDUR_RES,
TEST_ENDCAP_REQ,
TEST_ENDCAP_RES,
TIME_TESTEN_REQ,
TIME_TESTEN_RES,

TEST1_MONTH_REQ,
TEST1_MONTH_RES,
TEST1_DAY_REQ,
TEST1_DAY_RES,
TEST1_HOUR_REQ,
TEST1_HOUR_RES,

TEST2_MONTH_REQ,
TEST2_MONTH_RES,
TEST2_DAY_REQ,
TEST2_DAY_RES,
TEST2_HOUR_REQ,
TEST2_HOUR_RES,

TEST3_MONTH_REQ,
TEST3_MONTH_RES,
TEST3_DAY_REQ,
TEST3_DAY_RES,
TEST3_HOUR_REQ,
TEST3_HOUR_RES,

TEST4_MONTH_REQ,
TEST4_MONTH_RES,
TEST4_DAY_REQ,
TEST4_DAY_RES,
TEST4_HOUR_REQ,
TEST4_HOUR_RES,
SYS_TIME_REQ,
SYS_TIME_RES,
CALIB_MESG_1,
CALIB_MESG_2,
CALIB_MESG_3,
CALIB_MESG_4,
WRITE_FLASH,
//for DKD51_FAN
DKD51_FAN_SYS_REQ,
DKD51_FAN_SYS_RES,
DKD51_FAN_STATE_REQ,
DKD51_FAN_STATE_RES,
DKD51_FAN_WARNING_REQ,
DKD51_FAN_WARNING_RES,
DKD51_FAN_REMOTE_REQ,
DKD51_FAN_REMOTE_RES,
DKD51_FAN_PARAMETER_REQ,
DKD51_FAN_PARAMETER_RES,
DKD51_FAN_EQUIPMENT_REQ,
DKD51_FAN_EQUIPMENT_RES,
}
tMESGState;

extern tMESGState MESGState_v21;   
extern tMESGState MESGState;
//agisson======================================================================
//emerson m500d=================================================================
typedef struct InterruptMesg_Struct{
  
  uint8_t u8RecvByte[3];
  uint8_t u8Checkbit;
  
} tInterruptMesg_Struct;
extern tInterruptMesg_Struct sInterruptMesg;

typedef enum
{
CHECKBYTE1,
CHECKBYTE2,
CHECKBYTE3
}
tInterruptMesgState;
extern tInterruptMesgState InterruptMesgState;
extern uint32_t PMUConnectCount;

extern unsigned char g_UARTRxBuf[300];
extern unsigned char g_UARTRxBuf2[300];
extern uint32_t RecvCntStart;
extern uint32_t RecvCntStart2;
extern uint8_t g_u8ReadBackBuffer[256];
extern unsigned char g_setInfo[32];

static unsigned char scuTimeSet[32] = {0x7E,0x32,0x30,0x30,0x31,0x45,0x31,0x45,0x32,0x32,0x30,0x30,0x45,
								0x30,0x30,0x30,0x30,// Year
								0x30,0x30,// Month
								0x30,0x30,// Date
								0x30,0x30,// Hour
								0x30,0x30,// Minute
								0x30,0x30,// Second
									0x30,0x30,0x30,0x30,0x0D};
extern unsigned char scuTimeSet[32];
typedef struct
{
  char cTimeFrame[19];
}
tAlarmTimeParameters;

typedef struct
{
  //   unsigned char ucContent[11];
  char ucName[21];
  char cRectID[20];
  uint8_t u8SequenceNumber;
  uint8_t u8ID;
}
tAlarmContentParameters;

typedef struct
{
  tAlarmTimeParameters sTimeFrame[2];
  
  tAlarmContentParameters sAlarmContent;
}
tHistoryParameters;

typedef struct
{
  unsigned char ucSequenceNum;
  unsigned char ucSequenceNum_old;
  
  tHistoryParameters sHistoryParam[200];
}
tHistoryInfo;

typedef struct
{
  uint8_t	cBtsNameDB[40];
  uint8_t	cBtsName[40];
  uint8_t	cBtsDCVendor[16];
  uint8_t	cBtsControllerModel[36];
  uint8_t	cBtsBattType[16];
  
  uint8_t	cBtsName_len;
  uint8_t	cBtsDCVendor_len;
  uint8_t	cBtsControllerModel_len;
  uint8_t	cBtsBattType_len;
  
  uint8_t*	pcBtsName_len;
  uint8_t*	pcBtsDCVendor_len;
  uint8_t*	pcBtsControllerModel_len;
  uint8_t*	pcBtsBattType_len;
  
  uint8_t*	pcBtsName;
  uint8_t*	pcBtsDCVendor;
  uint8_t*	pcBtsControllerMode;
  uint8_t*	pcBtsBattType;
  
  unsigned char ucMACAddress[6];
  
  uint32_t	u32BtsControllerModel[2];
  uint32_t	u32BtsAmbientTemp;
  uint32_t	u32RemoteReboot;
  uint32_t*	pui32RemoteReboot;
  uint32_t   u32SysTrapCnt;
  
}
tSiteInfo;
extern tSiteInfo sSiteInfo;

static unsigned char calibMesg_1[74] = {0x7E,0x7B,0x7C,
									0x30,0x31,
									0x20,0x20,0x20,0x20,0x20,0x20,
									0x20,0x20, // Confirm Message
									0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30, // Current IP
									0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30, // Current SN
									0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30, // Current GW
									0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30, // Current SIP
									0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30, // Current MAC                                                                        
									0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30, // FirmwareCRC
									0x20,0x20, // Test RS485 Response
									0x30,0x30,0x30,0x30,0x0D};
extern unsigned char calibMesg_1[74];

extern tHistoryInfo sHistoryInfo;
extern uint8_t RecvBuff[300];
extern unsigned char g_getInfo[24];
extern uint8_t g_AlrmMan;
extern uint8_t ui8User[6];
//emerson m500d=================================================================

//emerson m221==================================================================
extern uint32_t RecvCount;
extern uint16_t g_testchecksum,g_checksum;
extern uint8_t val;
typedef struct
{
  uint32_t 	u32MjHiBattVolt,
  u32MnHiBattVolt,
  u32MjLoBattVolt,
  u32MnLoBattVolt,
  u32MjBattHiTemp,
  u32MnBattHiTemp,
  u32BattDiscOpen,
  u32LLVDopen,
  u32BLVDopen,
  u32DCopen,
  u32ACmains,
  u32BattBrkOpen,
  u32DistBrkOpen,
  u32RectACFault,
  u32RectNoResp,
  u32RectFail,
  u32MjRect,
  u32MnRect,
  u32ManMode,
  u32LoadFuse,
  u32BattFuse,
  u32DIFuse[8],
  u32DIRelay,
  u32DigitalInput,
  u32ACThres,
  u32AmbTemp,
  u32Smoke,
  u32HeatExch,
  u32EnvDoor;
}
tAlarmGroupInfo;
extern tAlarmGroupInfo sAlarmInfo;

typedef struct
{
	uint8_t u8EnvTemp;			// Nhiet do moi truong
	uint8_t u8EnvHumidity; 		// Do am moi truong
	uint8_t u8EnvSmoke; 		// Cam bien khoi
	uint8_t u8EnvWater; 		// Cam bien nuoc
	uint8_t u8EnvInfra;
	uint8_t u8EnvDoor; 			// Cam bien mo cua
	uint8_t u8EnvGlass;
	uint8_t u8HeatExch;
}
tALARMEnvInfo;
extern tALARMEnvInfo sAlarmEnvInfo;
//emerson m221==================================================================
//zte===========================================================================
extern uint8_t RectNumber;
extern uint8_t Rectlosedtest;
extern uint32_t reverror;
extern tMESGState MESGStatetest[20];
extern uint8_t k;
//zte===========================================================================
//
//Trap mode=====================================================================
//
extern uint8_t trap_flag[200];
extern uint32_t trapStatus_TimePeriod;
//dpc===========================================================================
typedef struct{
  uint8_t    buff_485[500];
  uint16_t   cnt;
}RS_485_T;

typedef struct{
  RS_485_T uart_rx;
  RS_485_T rx;
  
  uint16_t check_sum_read;
  uint16_t check_sum_calculator;
  uint8_t  rect_number;
  uint8_t  var;
  uint32_t  fuse_flag;
}DPC_T;

extern DPC_T dpc_t;
//dpc===========================================================================
//for fan dpc===================================================================
///
typedef enum
{
_DKD51_FAN_SYS_REQ,
_DKD51_FAN_SYS_RES,
_DKD51_FAN_STATE_REQ,
_DKD51_FAN_STATE_RES,
_DKD51_FAN_WARNING_REQ,
_DKD51_FAN_WARNING_RES,
_DKD51_FAN_REMOTE_REQ,
_DKD51_FAN_REMOTE_RES,
_DKD51_FAN_PARAMETER_REQ,
_DKD51_FAN_PARAMETER_RES,
_DKD51_FAN_EQUIPMENT_REQ,
_DKD51_FAN_EQUIPMENT_RES,
}FAN_DPC_STATE_E;
///
typedef struct{
  uint8_t    buff_485[500];
  uint16_t   cnt;
}RS_485_CN_T;

///
typedef struct 
{
  uint8_t            rx_enable;
  RS_485_CN_T        rx;
  RS_485_CN_T        uart_rx;
  uint16_t           check_sum_read;
  uint16_t           check_sum_calculator;
  FAN_DPC_STATE_E    fan_state_e;
}FAN_DPC_SWITCH_UART_T;

extern FAN_DPC_SWITCH_UART_T fan_dpc_switch_uart_t;
///
//for fan dpc===================================================================

//function----------------------------------------------------------------------

//code here


#endif /* _DAQ_V3_VARIABLES_H */