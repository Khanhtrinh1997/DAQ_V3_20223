/**
 * @file private_mib_module.h
 * @brief Private MIB module
 *
 * @section License
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Copyright (C) 2010-2022 Oryx Embedded SARL. All rights reserved.
 *
 * This file is part of CycloneTCP Open.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * @author Oryx Embedded SARL (www.oryx-embedded.com)
 * @version 2.1.4
 **/

#ifndef _PRIVATE_MIB_MODULE_H
#define _PRIVATE_MIB_MODULE_H

//Dependencies
#include "mibs/mib_common.h"
//#include "daq_v3_variables.h"   

//Size of testString object
#define PRIVATE_MIB_TEST_STRING_SIZE 32
//Number of LEDs
#define PRIVATE_MIB_LED_COUNT 3
//Size of ledColor object
#define PRIVATE_MIB_LED_COLOR_SIZE 8


/**
 * @brief LED table entry
 **/
//vac group=====================================================================

//vac table entry---------------------------------------------------------------
typedef struct{
  uint32_t                                                vac_id;
  uint32_t                                                vac_status;
  int32_t                                                 vac_in_temp;
  int32_t                                                 vac_ex_temp;
  int32_t                                                 vac_frost1_temp;
  int32_t                                                 vac_frost2_temp;
  uint32_t                                                vac_humid;
  uint32_t                                                vac_fan1_duty;
  uint32_t                                                vac_fan1_rpm;
  uint32_t                                                vac_fan2_duty;
  uint32_t                                                vac_fan2_rpm;  
  union
  {
    struct
    {            
      uint32_t                                            alarm_frost2_under      :1;		//0x00010000: Frost2Under
      uint32_t                                            alarm_air_con_over      :1;		//0x00020000: AirConOver
      uint32_t                                            alarm_air_con_under     :1;		//0x00040000: AirConUnder
      uint32_t                                            alarm_hi_humid          :1;		//0x00080000: HiHumid
      uint32_t                                            alarm_filter_stuck      :1;		//0x00100000: FilterStuck
      uint32_t                                                                    :11;    
    
    
      uint32_t                                            alarm_air_con1_status   :1;           //0x00000001:AirCon1Status ON=1/OFF=0 
      uint32_t                                            alarm_air_con2_status   :1;  		//0x00000002:AirCon2Status ON=1/OFF=0  
      uint32_t                                            alarm_air_con1_fail     :1; 		//0x00000004: AirCon1Fail
      uint32_t                                            alarm_air_con2_fail     :1;		//0x00000008: AirCon2Fail
      uint32_t                                            alarm_fan_fail          :1;		//0x00000010: FanFail
      uint32_t                                            alarm_di1               :1;   	//0x00000020: DI1 Alarm
      uint32_t                                            alarm_di2               :1;  		//0x00000040: DI2 Alarm
      uint32_t                                            alarm_do1               :1; 	        //0x00000080: DO1 Status CLOSE=0/OPEN=1
      uint32_t                                            alarm_do2               :1;		//0x00000100: DO1 Status CLOSE=0/OPEN=1
      uint32_t                                            alarm_in_temp_over      :1;   	//0x00000200: InTempOver
      uint32_t                                            alarm_in_temp_under     :1;  		//0x00000400: InTempUnder
      uint32_t                                            alarm_ex_temp_over      :1; 		//0x00000800: ExTempOver
      uint32_t                                            alarm_ex_temp_under     :1;		//0x00001000: ExTempUnder
      uint32_t                                            alarm_frost1_over       :1;		//0x00002000: Frost1Over
      uint32_t                                            alarm_frost1_under      :1;		//0x00004000: Frost1Under
      uint32_t                                            alarm_frost2_over       :1;		//0x00008000: Frost2Over
    }bit;
    uint32_t                                              all;
  }vac_alarm_status;
  
  uint32_t                                                vac_sys_mode;
  uint32_t                                                vac_fan_speed_mode;
  int32_t                                                 vac_fan_start_temp;
  int32_t                                                 vac_set_temp;
  int32_t                                                 vac_pid_offset_temp;
  uint32_t                                                vac_fan_min_speed;
  uint32_t                                                vac_fan_max_speed;
  int32_t                                                 vac_filter_stuck_temp;
  uint32_t                                                vac_night_mode_en;
  uint32_t                                                vac_night_mode_start;
  uint32_t                                                vac_night_mode_end;
  uint32_t                                                vac_night_max_speed;
  uint32_t                                                vac_manual_mode;
  uint32_t                                                vac_manual_max_speed;
  int32_t                                                 vac_in_max_temp;
  int32_t                                                 vac_ex_max_temp;
  int32_t                                                 vac_frost_max_temp;
  int32_t                                                 vac_in_min_temp;
  int32_t                                                 vac_ex_min_temp;
  int32_t                                                 vac_frost_min_temp;
  int32_t                                                 vac_min_out_temp;
  int32_t                                                 vac_delta_temp;
  int32_t                                                 vac_panic_temp;
  int32_t                                                 vac_acu1_on_temp;
  int32_t                                                 vac_acu2_on_temp;
  uint32_t                                                vac_acu2_en;
  uint32_t                                                vac_air_con1_model;
  uint32_t                                                vac_air_con1_type;
  uint32_t                                                vac_air_con2_model;
  uint32_t                                                vac_air_con2_type;
  uint32_t                                                vac_air_con_on_off;
  uint32_t                                                vac_air_con_mode;
  int32_t                                                 vac_air_con_temp;
  uint32_t                                                vac_air_con_speed;
  uint32_t                                                vac_air_cond_dir;
  uint32_t                                                vac_air_con_volt;
  uint32_t                                                vac_air_con_current;
  uint32_t                                                vac_air_con_power;
  uint32_t                                                vac_air_con_frequency;
  uint32_t                                                vac_air_con_pf;
  uint32_t                                                vac_real_time_sync;
  uint32_t                                                vac_type;
  char_t                                                  vac_serial[20];
  size_t                                                  vac_serial_len;
  uint32_t                                                vac_modbus_id;
  uint32_t                                                vac_update_flag;
  uint32_t                                                vac_firm_version;
  char_t                                                  c_sync_time[20];
  size_t                                                  c_sync_time_len;
  uint32_t                                                vac_w_enable;
  uint32_t                                                vac_w_serial1;
  uint32_t                                                vac_w_serial2;
  uint32_t                                                vac_enable_reset;
  // write
  uint32_t                                                vac_sys_mode_write;
  uint32_t                                                vac_fan_speed_mode_write;
  int32_t                                                 vac_fan_start_temp_write;
  int32_t                                                 vac_set_temp_write;
  int32_t                                                 vac_pid_offset_temp_write;
  uint32_t                                                vac_fan_min_speed_write;
  uint32_t                                                vac_fan_max_speed_write;
  int32_t                                                 vac_filter_stuck_temp_write;
  uint32_t                                                vac_night_mode_en_write;
  uint32_t                                                vac_night_mode_start_write;
  uint32_t                                                vac_night_mode_end_write;
  uint32_t                                                vac_night_max_speed_write;
  uint32_t                                                vac_manual_mode_write;
  uint32_t                                                vac_manual_max_speed_write;
  int32_t                                                 vac_in_max_temp_write;
  int32_t                                                 vac_ex_max_temp_write;
  int32_t                                                 vac_frost_max_temp_write;
  int32_t                                                 vac_in_min_temp_write;
  int32_t                                                 vac_ex_min_temp_write;
  int32_t                                                 vac_frost_min_temp_write;
  int32_t                                                 vac_min_out_temp_write;
  int32_t                                                 vac_delta_temp_write;
  int32_t                                                 vac_panic_temp_write;
  //  int32_t vacACU1OnTemp;
  //  int32_t vacACU2OnTemp;
  //  uint32_t vacACU2En;
  uint32_t                                                vac_air_con1_model_write;
  uint32_t                                                vac_air_con1_type_write;
  uint32_t                                                vac_air_con2_model_write;
  uint32_t                                                vac_air_con2_type_write;
  uint32_t                                                vac_air_con_on_off_write;
  uint32_t                                                vac_air_con_mode_write;
  int32_t                                                 vac_air_con_temp_write;
  uint32_t                                                vac_air_con_speed_write;
  uint32_t                                                vac_air_cond_dir_write;  
  uint32_t                                                vac_w_enable_write;
  uint32_t                                                vac_w_serial1_write;
  uint32_t                                                vac_w_serial2_write;
  // write
  uint32_t                                                vac_active_fan;
  uint32_t                                                vac_installed_fan;
  uint32_t                                                vac_installed_air_con;
  uint32_t                                                vac_update_step;

}PRIVATE_MIB_VAC_INFO_ENTRY_STRUCT;

//vac group-----------------------------------------------------------------------------
typedef struct{
  uint32_t                                                vac_installed_vac;
  uint32_t                                                vac_active_vac; 
  PRIVATE_MIB_VAC_INFO_ENTRY_STRUCT                       vac_table_struct[1];
}PRIVATE_MIB_VAC_GROUP_STRUCT;

//lib group=====================================================================
typedef struct
{
	int32_t liBattIndex;
	uint32_t liBattStatus;
	int32_t liBattPackVolt;
	int32_t liBattPackCurr;
	uint32_t liBattRemainCap;
	int32_t liBattAvrCellTemp;
	int32_t liBattAmbTemp;
	uint32_t liBattWarningFlag;
	uint32_t liBattProtectFlag;
	uint32_t liBattFaultStat;
	uint32_t liBattSOC;
        uint32_t liBattType;
        uint16_t liBattModbusID;
        uint32_t liBattCellVolt[16];
        int32_t liBattCellTemp[16];
	uint32_t liBattSOH;
        uint32_t liBattPeriod;
        uint32_t liBattStatusCCL;
        uint32_t liBattMode;
	char liBattModel[20];
        size_t liBattModelLen;
	
	char liBattSWVer[20];
        size_t liBattSWVerLen;
	
	char liBattHWVer[20];
        size_t liBattHWVerLen;
	
	char liBattSerialNo[31];
        size_t liBattSerialNoLen;
        union
        {
                struct
                {
                        uint32_t  alarm_CellOverVolt:1;   		//0x00000001:
                        uint32_t  alarm_CellLowVolt:1;  			//0x00000002:
                        uint32_t  alarm_PackOverVolt:1;					//0x00000004:
                        uint32_t  alarm_PackLowVolt:1;			//0x00000008:
                        uint32_t  alarm_CharOverCurrent:1;   		//0x00000010:
                        uint32_t  alarm_DisCharOverCurrent:1;  		//0x00000020:
                        uint32_t  alarm_BattHighTemp:1; 	//0x00000040:
                        uint32_t  alarm_BattLowTemp:1;			//0x00000080:
                        uint32_t  alarm_EnvHighTemp:1;   		//0x00000100:
                        uint32_t  alarm_EnvLowTemp:1;  			//0x00000200:
                        uint32_t  alarm_PCBHighTemp:1; 		//0x00000400:
                        uint32_t  alarm_LowCapacity:1;		//0x00000800:
                        uint32_t  alarm_VoltDiff:1;		//0x00001000:
                        uint32_t  reserved1:3;
                        
                        uint32_t  protect_CellOverVolt:1;   		//0x00000001:
                        uint32_t  protect_CellLowVolt:1;  			//0x00000002:
                        uint32_t  protect_PackOverVolt:1; 			//0x00000004:
                        uint32_t  protect_PackLowVolt:1;			//0x00000008:
                        uint32_t  protect_ShortCircuit:1;			//0x00000010:
                        uint32_t  protect_OverCurr:1;   		//0x00000020:
                        uint32_t  protect_CharHighTemp:1;  		//0x00000040:
                        uint32_t  protect_CharLowTemp:1; 	//0x00000080:
                        uint32_t  protect_DisCharHighTemp:1;			//0x00000100:
                        uint32_t  protect_DisCharLowTemp:1;   		//0x00000200:
                        uint32_t  protect_lost_sensor:1;      		//0x00000400:
                        uint32_t  protect_reverse:1;			
                        uint32_t  reserved2:4;		
                        
                }bit;
                uint32_t all;
        }liBattAlarmStatus;
        
//----------------------M1---------------------------------//
        uint32_t u32ConverterState;
        uint32_t u32ErrCode;
        uint32_t u32BalanceStatus;
        uint32_t u32MosfetMode;
        uint32_t u32Mcu2McuErr;
        uint32_t u32CHGCapacity;
        uint32_t u32DSGCapacity;
        uint32_t u32Efficiency;
        uint32_t u32NumberOfCan;
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
        uint32_t u32SystemByte;
        uint32_t u32KeyTime;
        uint32_t u32IniMaxim;
        uint32_t u32EnableKCS;
        uint32_t u32EnableLock;
        //===Write
        //===Alarm
        int32_t i32PackHighVolt_AWrite;
        int32_t i32BattHighVolt_AWrite;
        int32_t i32CellHighVolt_AWrite;
        int32_t i32BattLowVolt_AWrite;
        int32_t i32CellLowVolt_AWrite;
        int32_t i32CharHighCurr_AWrite;
        int32_t i32DisCharHighCurr_AWrite;
        int32_t i32CharHighTemp_AWrite;
        int32_t i32DisCharHighTemp_AWrite;
        int32_t i32CharLowTemp_AWrite;
        int32_t i32DisCharLowtemp_AWrite;
        int32_t i32LowCap_AWrite;
        int32_t i32BMSHighTemp_AWrite;
        int32_t i32BMSLowTemp_AWrite;
        //===Protect
        int32_t i32PackOverVolt_PWrite;
        int32_t i32BattOverVolt_PWrite;
        int32_t i32CellOverVolt_PWrite;
        int32_t i32BattUnderVolt_PWrite;
        int32_t i32CellUnderVolt_PWrite;
        int32_t i32CharOverCurr_PWrite;
        int32_t i32DisCharOverCurr_PWrite;
        int32_t i32CharOverTemp_PWrite;
        int32_t i32DisCharOverTemp_PWrite;
        int32_t i32CharUnderTemp_PWrite;
        int32_t i32DisCharUnderTemp_PWrite;
        int32_t i32UnderCap_PWrite;
        int32_t i32BMSOverTemp_PWrite;
        int32_t i32BMSUnderTemp_PWrite;
        int32_t i32DifferentVolt_PWrite;
        //===
        uint32_t u32VoltBalanceWrite;
        uint32_t u32DeltaVoltBalanceWrite;
        uint32_t u32DisCharCurrLimitWrite;
        uint32_t u32CharCurrLimitWrite;
        uint32_t u32VoltDisCharRefWrite;
        uint32_t u32VoltDisCharCMDWrite;
        uint32_t u32SystemByteWrite;
        uint32_t u32KeyTimeWrite;
        uint32_t u32IniMaximWrite;
        uint32_t u32EnableKCSWrite;
        uint32_t u32EnableLockWrite;
        uint16_t u16_liionCapInitWrite;
          
        uint32_t u32ADCPackVolt;
        uint32_t u32ADCBattVolt;
        int32_t i32ADCPackCurr;
        int32_t i32ADCBattCurr;
        
        uint32_t u32SOCMaxim;
        uint32_t u32SOHMaxim;
        uint32_t u32FullCapRepMaxim;
        uint32_t u32VoltMaxim;
        uint32_t u32CurrMaxim;
        
        uint32_t u32IKalamn;
        uint32_t u32SOCKalamn;
        uint32_t u32VpriKalamn;
        uint32_t u32VmesKalamn;
        uint32_t u32CapKalamn;
        uint32_t u32InternalR0Kalamn;
        uint16_t u16_liionCapInit;
//----------------------M1---------------------------------//
} PrivateMibLIBattInfoEntry;

/**
 * @brief LIBattInfo group
 **/

typedef struct
{
	uint32_t liBattInstalledPack;
	uint32_t liBattActivePack;
	uint32_t liBattTotalCurrent;
	uint32_t liBattUtilization;
	PrivateMibLIBattInfoEntry liBattTable[16];
} PrivateMibLIBattInfoGroup;

//gen group=====================================================================
typedef struct
{
  	uint32_t genStatusID;
	uint32_t genStatusStatus;
	uint32_t genStatusLNVolt1;
	uint32_t genStatusLNVolt2;
	uint32_t genStatusLNVolt3;
	uint32_t genStatusLNCurr1;
	uint32_t genStatusLNCurr2;
	uint32_t genStatusLNCurr3;
	uint32_t genStatusLNPower1;
	uint32_t genStatusLNPower2;
	uint32_t genStatusLNPower3;
	int32_t genStatusLNSPower1;
	int32_t genStatusLNSPower2;
	int32_t genStatusLNSPower3;
	uint32_t genStatusFrequency;
	uint32_t genStatusBattVolt;
	uint32_t genStatusCoolantTemp;
	uint32_t genStatusOilPressure;
	uint32_t genStatusFuelLevel;
	uint32_t genStatusRunTime;
	uint32_t genStatusOilTemp;
	uint32_t genStatusMaintenanceTime;
	uint32_t genStatusEngineSpeed;
        uint32_t genStatusWorkingHour;
        uint32_t genStatusWorkingMin;
        uint32_t genStatusActiveEnergyLow;
        uint32_t genStatusActiveEnergyHigh;   
        
        union
        {
          struct
          {
                  uint32_t  err_LowFuel:1;   		        //0x00000001:   uint32_t  err_ChargeAlterFail:1; 
                  uint32_t  err_UnderFreq:1;  			//0x00000002:*
                  uint32_t  err_OverFreq:1; 			//0x00000004:*
                  uint32_t  err_HiCoolantTemp:1;		//0x00000008:* 
                  uint32_t  err_UnderVolt:1;			//0x00000010:*
                  uint32_t  err_OverVolt:1;   		        //0x00000020:*
                  uint32_t  err_LowOilPressure:1;  		//0x00000040:*
                  uint32_t  err_HiBattVol:1; 	                //0x00000080:   uint32_t  err_FailToStart:1; 	
                  uint32_t  err_LowBattVol:1;			//0x00000100:   uint32_t  err_AI2high:1;	
                  uint32_t  err_GenPhaseSeqFail:1;   		//0x00000200:
                  uint32_t  err_UnderPower:1;  			//0x00000400:
                  uint32_t  err_OverPower:1; 		        //0x00000800:
                  uint32_t  err_OilPressureSensor:1;		//0x00001000:
                  uint32_t  err_Tempsensor:1;                   //0x00002000:
                  uint32_t  err_AI1sensor:1;                    //0x00004000:
                  uint32_t  err_AI2sensor:1;                    //0x00008000:
                  
                  uint32_t  err_EmergencyStop:1;   		//0x00000001:
                  uint32_t  err_GenStop:1;  			//0x00000002:
                  uint32_t  err_ReversePower:1; 		//0x00000004:
                  uint32_t  err_CabinTempLow:1;			//0x00000008:
                  uint32_t  err_CabinTempHigh:1;		//0x00000010:
                  uint32_t  err_PickupSignalLoss:1;   		//0x00000020:
                  uint32_t  err_Al1low:1;  		        //0x00000040:
                  uint32_t  err_OverSpeed:1; 	                //0x00000080:
                  uint32_t  err_UnderSpeed:1;			//0x00000100:
                  uint32_t  err_AI2low:1;   		        //0x00000200:
                  uint32_t  err_AI1high:1;  			//0x00000400:
                  uint32_t  err_OverCurr:1; 		        //0x00000800:*
                  uint32_t  err_ShortCircuit:1;		        //0x00001000:
                  uint32_t  err_EarthFault:1;		        //0x00002000:
                  uint32_t  err_UnderCurrent:1;  		//0x00004000:
                  uint32_t  err_Maintenance:1;                  //0x00008000:
          }bit;
          uint32_t all;
        }genStatusAlarmStatus;
//	uint32_t genStatusAlarmStatus;
        
        char_t genSerial[20];
        size_t genSerialLen;
        uint16_t genStatusModbusID;
        uint8_t genStatusType;
        uint8_t genStopMode;
        uint8_t genManualMode;
        uint8_t genAutoMode;
        uint8_t genStartMode;

} PrivateMibGenInfoEntry;

/**
 * @brief GenInfo group
 **/

typedef struct
{
	uint32_t genInstalledGen;
	uint32_t genActiveGen;
        uint32_t genflag;
	PrivateMibGenInfoEntry genTable[2];
} PrivateMibGenInfoGroup;


//pm group=====================================================================
typedef struct
{
  uint32_t pmID;	
  uint32_t pmStatus;	
  uint32_t pmImportActiveEnergy;	
  uint32_t pmExportActiveEnergy;
  uint32_t pmImportReactiveEnergy;	
  uint32_t pmExportReactiveEnergy;	
  uint32_t pmTotalActiveEnergy;	
  uint32_t pmTotalReactiveEnergy;	
  uint32_t pmActivePower;	
  int32_t  pmReactivePower;	
  uint32_t pmPowerFactor;
  uint32_t pmFrequency;	
  uint32_t pmTotalCurrent;	
  
  uint32_t pmPhase1Current;	
  uint32_t pmPhase1Voltage;	
  uint32_t pmPhase1RealPower;
  uint32_t pmPhase1ApparentPower;	
  int32_t pmPhase1ReactivePower;	
  uint32_t pmPhase1PowerFactor;
  uint32_t pmPhase2Current;	
  uint32_t pmPhase2Voltage;	
  uint32_t pmPhase2RealPower;
  uint32_t pmPhase2ApparentPower;	
  uint32_t pmPhase2ReactivePower;	
  uint32_t pmPhase2PowerFactor;
  uint32_t pmPhase3Current;	
  uint32_t pmPhase3Voltage;	
  uint32_t pmPhase3RealPower;
  uint32_t pmPhase3ApparentPower;	
  uint32_t pmPhase3ReactivePower;
  uint32_t pmPhase3PowerFactor;  
  
  uint32_t pmOutOfRangeFreq;  
  uint32_t pmDeltaFreqDisConnect;  
  uint32_t pmDeltaFreqReConnect;  
  uint32_t pmEnableFreqTrap; 
  
  char_t pmSerial[20];
  char_t pmModel[20];
  size_t pmSerialLen;
  size_t pmModelLen;
  uint32_t pmType;
  uint32_t pmModbusID;
  
} PrivateMibPMInfoEntry;

/**
* @brief LIBattInfo group
**/

typedef struct
{
  uint32_t pmInstalledPM;
  uint32_t pmActivePM;
  PrivateMibPMInfoEntry pmTable[2];
} PrivateMibPMGroup;

//smbc group===================================================================
typedef struct // smcb
{
  int32_t SmcbStatusID;
  uint32_t SmcbStatus; // connect, 0:no 1:yes
  uint32_t SmcbState;  // 0:off 1:on
  uint32_t SmcbStateWrite;
  uint32_t SmcbType;    
  uint16_t SmcbModbusID;
} PrivateMibSMCBInfoEntry;

/**
 * @brief SMCBInfo group
 **/
typedef struct
{
	uint32_t SmcbInstalledSMCB;
	uint32_t SmcbActiveSMCB;
        uint32_t flag; 
	PrivateMibSMCBInfoEntry SmcbTable[5];
} PrivateMibSMCBInfoGroup;

//fuel group===========================================================================
typedef struct 
{
  int32_t FuelStatusID;
  uint32_t FuelStatus; // connect, 0:no 1:yes
  uint32_t FuelType;    
  uint16_t FuelModbusID;
  uint32_t FuelLevel;
} PrivateMibFUELInfoEntry;

/**
 * @brief FUELInfo group
 **/
typedef struct
{
	uint32_t FuelInstalledFUEL;
	uint32_t FuelActiveFUEL; 
	PrivateMibFUELInfoEntry FuelTable[2];
} PrivateMibFUELInfoGroup;

// isense group==============================================================
typedef struct
{
  uint32_t isenseID;	
  uint32_t isenseStatus;	
  uint32_t isenseImportActiveEnergy;	
  uint32_t isenseExportActiveEnergy;
  uint32_t isenseImportReactiveEnergy;	
  uint32_t isenseExportReactiveEnergy;	
  uint32_t isenseTotalActiveEnergy;	
  uint32_t isenseTotalReactiveEnergy;	
  uint32_t isenseActivePower;	
  int32_t  isenseReactivePower;	
  int32_t  isenseApparentPower; 
  uint32_t isensePowerFactor;
  uint32_t isenseFrequency;	
  uint32_t isenseCurrent;	
  uint32_t isenseVoltage;
  
  uint32_t isenseOutOfRangeFreq;  
  uint32_t isenseDeltaFreqDisConnect;  
  uint32_t isenseDeltaFreqReConnect;  
  uint32_t isenseEnableFreqTrap; 
  
  char_t isenseSerial[20];
  char_t isenseModel[20];
  size_t isenseSerialLen;
  size_t isenseModelLen;
  uint32_t isenseType;
  uint32_t isenseModbusID;
	
} PrivateMibISENSEInfoEntry;
/**
 * @brief ISenseInfo group
 **/
typedef struct
{
	uint32_t isenseInstalledISENSE;
	uint32_t isenseActiveISENSE;
	PrivateMibISENSEInfoEntry isenseTable;
} PrivateMibISENSEGroup;

//pm_dc group===================================================================
typedef struct{
  uint32_t pm_dc_id;
  uint32_t pm_dc_status;
  uint32_t pm_dc_active_power;
  uint32_t pm_dc_active_energy;
  uint32_t pm_dc_current;
  uint32_t pm_dc_voltage;
  char_t   pm_dc_model_[20];
  size_t   pm_dc_model_len;
  uint32_t pm_dc_type;
  uint32_t pm_dc_modbus_ID; 
}PrivateMibPMDCInfoEntry;
//pribate mib pm dc froup----------------------------------------------------------
typedef struct{
  uint32_t                 pm_dc_installed_PM_DC;
  uint32_t                 pm_dc_active_PM_DC;
  PrivateMibPMDCInfoEntry  pm_dc_table;
}PrivateMibPMDCGroup;
//
//bm group======================================================================
typedef struct
{	 
  uint32_t bmID;
  uint32_t bmStatus;
  uint32_t bmBattVolt;
  uint32_t bmPackVolt;
  uint32_t bmPackCurr;
  uint32_t bmCellVolt1;
  uint32_t bmCellVolt2;
  uint32_t bmCellVolt3;
  uint32_t bmCellVolt4;
  uint32_t bmCellTemp1;
  uint32_t bmCellTemp2;
  uint32_t bmCellTemp3;
  uint32_t bmCellTemp4;
  uint32_t bmCellTemp5;
  uint32_t bmCellTemp6;
  union
  {
    struct
    {
      uint32_t  protect_PackOverVolt:1;   		//0x00000001:Pack over voltage alarm
      uint32_t  protect_BattOverVolt:1;  			//0x00000002:Pack over voltage alarm
      uint32_t  protect_CellOverVolt:1; 			//0x00000004:Pack over voltage alarm
      uint32_t  protect_HalfVoltProtect:1;			//0x00000008:Pack over voltage alarm
      uint32_t  protect_BattUnderVolt:1;			//0x00000010:Pack over voltage alarm
      uint32_t  protect_CellUnderVolt:1;   		//0x00000020:Pack over voltage alarm
      uint32_t  protect_CharOverCurrent:1;  		//0x00000040:Pack over voltage alarm
      uint32_t  protect_DisCharOverCurrent:1; 	//0x00000080:Pack over voltage alarm
      uint32_t  protect_CharHighTemp:1;			//0x00000100:Pack over voltage alarm
      uint32_t  protect_MosfetError:1;   		//0x00000200:Pack over voltage alarm
      uint32_t  protect_CharLowTemp:1;  			//0x00000400:Pack over voltage alarm
      uint32_t  protect_Unbalancing:1; 		//0x00000800:Pack over voltage alarm
      uint32_t  protect_LowCapacity:1;		//0x00001000:Pack over voltage alarm                  
      uint32_t  protect_ShortCircuit:1;  			//0x00004000:Pack over voltage alarm
      uint32_t  reserved2:2;		//0x00002000:Pack over voltage alarm
      
      uint32_t  alarm_PackOverVolt:1;   		//0x00000001:Pack over voltage alarm (V= 59V)
      uint32_t  alarm_BattOverVolt:1;  			//0x00000002:Battery over voltage alarmV (V= 58V)
      uint32_t  alarm_CellOverVolt:1; 			//0x00000004:Cell over voltage alarm (V = 3,6V)
      uint32_t  alarm_SocUnderVolt:1;					//0x00000008: SocUnderVolt
      uint32_t  alarm_BattUnderVolt:1;			//0x00000010:Battery under voltage alarm(V = 43.2V)
      uint32_t  alarm_CellUnderVolt:1;   		//0x00000020:Cell under voltage alarm(V = 2.8V)
      uint32_t  alarm_CharOverCurrent:1;  		//0x00000040:Charge over current alarm(A=  45A)
      uint32_t  alarm_DisCharOverCurrent:1; 	//0x00000080:Discharge over current alarm(A=  50A)
      uint32_t  alarm_CharHighTemp:1;			//0x00000100:Charge high temperature alarm(T=  65?)
      uint32_t  alarm_DisCharHighTemp:1;   		//0x00000200:Discharge high temperature alarm(T=  65?)
      uint32_t  alarm_CharLowTemp:1;  			//0x00000400:Charge low temperature alarm(T=  5?)
      uint32_t  alarm_DisCharLowTemp:1; 		//0x00000800:Discharge low temperature alarm(T=-10?)
      uint32_t  alarm_LowCapacity:1;		//0x00001000:Low capacity alarm (SOC=10%)
      uint32_t  reserved1:3;
      
      
      
    }bit;
    uint32_t all;
  }bmAlarmStatus;
  
  //  uint32_t bmAlarmStatus;
  uint32_t bmBalanceStatus;
  uint32_t bmBattStatus;
  uint32_t bmSOC;
  uint32_t bmSOH;
  uint32_t bmDischargeTime;
  uint32_t bmTotalRunTime;
  uint32_t bmVoltDiff;
  uint32_t bmMaxTemp;
  uint32_t bmVoltThres;
  uint32_t bmCurrThres;
  uint32_t bmTimeThres;
  uint32_t bmSOCThres;  
  uint32_t bmMinTemp;
  uint32_t bmLowCapTime;
  uint32_t bmTotalDisAH;
  uint32_t bmHalfVoltAlarm;
  uint32_t bmRechargeVolt;
  uint32_t bmCurrBalanceThres;
  uint32_t bmType;
  uint32_t bmModbusID;
  
} PrivateMibBMInfoEntry;

/**
 * @brief Batt Monitor group
 **/

typedef struct
{
	uint32_t bmInstalledBM;
	uint32_t bmActiveBM;
	PrivateMibBMInfoEntry bmTable[16];
} PrivateMibBMGroup;

//net work group================================================================
typedef struct
{
	uint32_t	u32IP;
	uint32_t	u32SN;
	uint32_t	u32GW;
	uint32_t	u32SIP;     
    	uint8_t		ucIP[16];
    	uint8_t		ucSN[16];
    	uint8_t		ucGW[16];
    	uint8_t		ucSIP[16];
//        uint8_t u8TrapMode;
//        uint32_t u32TrapInterval;
}tEthernet_Setting_Struct;

typedef struct
{
  uint8_t u8TrapMode;
  uint32_t u32TrapInterval;
  uint32_t u32TrapTick;
  char_t cFTPServer[40];
  size_t cFTPServerLen;
  char_t cSyncTime[20];
  size_t cSyncTimeLen;
  tEthernet_Setting_Struct siteNetworkInfo;	
  uint8_t u8NewFWFlag;
  uint16_t ftp_port;
} PrivateMibCfgNetworkGroup;
//load group====================================================================
typedef struct
{
  uint32_t loadCurrent;
  uint8_t loadStatus[10];
  
}PRIVATE_MIB_LOAD_GROUP_STRUCT;

//batt group====================================================================
typedef struct
{
  uint32_t battVolt;
  uint32_t battCurr;
  uint32_t battTemp;
  uint32_t battBanksNumofBanks;
  uint32_t battCapLeft1;
  uint32_t battCapLeft2;
  uint32_t battCapLeft3;
  uint32_t battCapLeft4;
  uint32_t battBrkStatus[4];
  
} PrivateMibBatteryGroup;

//ac group=====================================================================
typedef struct
{
  int32_t acPhaseIndex;
  uint32_t acPhaseVolt;
  
} PrivateMibAcPhaseEntry;

/**
* @brief AcInfo group
**/

typedef struct
{
  int32_t acPhaseNumber;
  PrivateMibAcPhaseEntry acPhaseTable[3];
} PrivateMibAcPhaseGroup;

//recr group=======================================================================================
typedef struct
{
  int32_t rectIndex;
  uint32_t rectStatus;
  uint32_t rectOutputCurrent;
  uint32_t rectOutputVoltage;
  uint32_t rectTemp;
  char rectType[16];
  size_t rectTypeLen;
  uint32_t rectAlarmStatus;
  uint32_t rectFault;
  uint32_t rectNoResp;
  char rectSerialNo[16];
  size_t rectSerialNoLen;
  uint32_t rectRevisionLevel;
  
} PrivateMibRectInfoEntry;

typedef struct
{
  uint32_t rectInstalledRect;
  uint32_t rectActiveRect;
  uint32_t rectTotalCurrent;
  uint32_t rectUtilization;
  PrivateMibRectInfoEntry rectTable[24];
} PrivateMibRectInfoGroup;

//config group====================================================================
typedef struct
{
  uint32_t cfgWalkInTimeEn;
  uint32_t cfgWalkInTimeDuration;
  uint32_t cfgCurrentLimit;
  uint32_t cfgCurrentlimitA; //thanhcm3 add for dpc
  uint32_t cfgFloatVolt;
  uint32_t cfgBoostVolt;
  uint32_t cfgLLVDVolt;
  uint32_t cfgDCLowVolt;
  uint32_t cfgStartManualTest;
  uint32_t cfgTempCompValue;
  uint32_t cfgBLVDVolt;
  uint32_t cfgBattCapacityTotal;
  int32_t cfgHighMajorTempLevel;
  int32_t cfgLowTempLevel;
  uint32_t cfgAcLowLevel;
  uint32_t cfgBattCapacityTotal2;
  uint32_t cfgBattCapacityTotal3; //thanhcm3 add for dpc
  uint32_t cfgBattCapacityTotal4; //thanhcm3 add for dpc
  //uint16_t fan_para_dc_starting_point; //thanhcm3 add for dpc
  //uint16_t fan_para_dc_sensivive_point;//thanhcm3 add for dpc
  //uint16_t fan_para_alarm_high_temp; //thanhcm3 add for dpc
  //uint16_t fan_para_alarm_low_temp;//thanhcm3 add for dpc
  
  uint32_t cfgDCOverVolt;
  uint32_t cfgDCUnderVolt;
  uint32_t cfgAcUnderLevel;
  uint32_t cfgAcHighLevel;
  uint32_t cfgBattTestVolt;
  uint32_t cfgOverMajorTempLevel;
  uint32_t cfgLLVDEn;
  uint32_t cfgBLVDEn;
  //        uint32_t cfgTestVoltCfg;
  //        uint32_t cfgAutoTestDay;
  //        uint32_t cfgTestStartTime;
} PrivateMibConfigGroup;

//cofig BT group============================================================
typedef struct
{
  int32_t cfgBTPlanTestIndex;
  uint8_t cfgBTPlanTestMonth;
  uint8_t cfgBTPlanTestDate;
  uint8_t cfgBTPlanTestHour;
  char cfgBTPlanTestString[16];
  size_t cfgBTPlanTestStringLen;
} PrivateMibCfgBTPlanTestEntry;

/**
* @brief cfgBTSCU table entry
**/

typedef struct
{
  int32_t cfgBTSCUIndex;
  uint16_t cfgBTSCUYear;
  uint8_t cfgBTSCUMonth;
  uint8_t cfgBTSCUDate;
  uint8_t cfgBTSCUHour;
  uint8_t cfgBTSCUMinute;
  uint8_t cfgBTSCUSecond;        
  char cfgBTSCUString[21];
  size_t cfgBTSCUStringLen;
} PrivateMibCfgBTSCUEntry;

/**
* @brief configAcc group
**/

typedef struct
{
  uint32_t cfgBTEndVolt;
  uint32_t cfgBTEndCap;
  uint32_t cfgBTEndTime;
  uint8_t cfgBTPlanTestNumber;
  uint8_t cfgBTPlanTestEn;
  uint8_t cfgBTSCUNumber;
  uint8_t cfgBTCellNo;
  uint32_t cfgTestVoltCfg;
  uint32_t cfgAutoTestDay;
  uint32_t cfgTestStartTime;
  uint32_t cfgAgissonBTRespond;
  PrivateMibCfgBTPlanTestEntry cfgBTPlanTestTable[4];
  PrivateMibCfgBTSCUEntry cfgBTSCUTable[1];
} PrivateMibCfgBTGroup;

//DI============================================================================

typedef struct
{
  uint32_t alarmDigitalInput[8];
  uint32_t alarmDigitalInput_old[8];
  uint32_t alarmDigitalInput_old2[8];
} PrivateMibDIAlarmGroup;

//site group====================================================================
typedef struct
{
  char_t siteBTSCode[40];
  size_t siteBTSCodeLen;
  char_t siteDCsystemVendor[16];
  size_t siteDCsystemVendorLen;
  uint32_t siteControllerModel[2];                //????????
  uint32_t siteBatteryType;
  uint32_t siteAmbientTemp;
  uint32_t siteTrapCounter;
  uint32_t siteSystemReset;
  char_t siteMACInfo[17];
  size_t siteMACInfoLen;
  char_t siteSerialNumber[20];
  size_t siteSerialNumberLen;
  uint32_t siteFirmwareCRC;
  char_t siteCRCStr[20];
  size_t siteCRCStrLen;        
  uint32_t siteFirmwareUpdate;
  uint32_t siteFWType;
  char_t siteDevModel[17];
  size_t siteDevModelLen;
  uint32_t siteTrapEnable;
} PrivateMibSiteInfoGroup;

//private main alarm group=============================================================
typedef struct{
  uint32_t alarm_batt_high_temp;
  uint32_t alarm_llvd;
  uint32_t alarm_blvd;
  uint32_t alarm_dc_Low;
  uint32_t alarm_ac_mains;
  uint32_t alarm_batt_breaker_open;
  uint32_t alarm_load_breaker_open;
  uint32_t alarm_rect_ac_fault;
  uint32_t alarm_rect_no_resp;
  uint32_t alarm_manual_mode;
  uint32_t alarm_ac_low;        
  uint32_t alarm_smoke;
  uint32_t alarm_env_door;
  uint32_t alarm_lib;
  uint32_t alarm_gen;
  uint32_t alarm_pm;
  uint32_t alarm_vac;
  uint32_t alarm_bm;
  uint32_t alarm_smcb;
  uint32_t alarm_out_of_range_freq;
  uint32_t alarm_fuel;
  uint32_t alarm_isense;
  uint32_t alarm_pm_dc;
  //uint32_t alarm_dpc_fan;
  uint32_t alarm_out_of_range_freq2;
  
  uint32_t alarm_lib_pack[16];        
  uint32_t alarm_gen_pack[2];
  uint32_t alarm_bm_pack[16];
  uint32_t alarm_vac_pack[1];
  uint32_t alarm_smcb_pack[5];
  uint32_t alarm_out_of_range_freq_pack[2];
  uint32_t alarm_fuel_pack[2];
  uint32_t alarm_out_of_range_freq_pack2[1];
  uint32_t alarm_isense_pack[1];
  uint32_t alarm_pm_dc_pack[1];
  uint32_t alarm_dpc_fan_pack[1];
  //code here
  
  uint32_t alarmBattHighTemp_old;
  uint32_t alarmLLVD_old;
  uint32_t alarmBLVD_old;
  uint32_t alarmDCLow_old;
  uint32_t alarmACmains_old;
  uint32_t alarmBattBreakerOpen_old;
  uint32_t alarmLoadBreakerOpen_old;
  uint32_t alarmRectACFault_old;
  uint32_t alarmRectNoResp_old;
  uint32_t alarmManualMode_old;
  uint32_t alarmACLow_old; 
  uint32_t alarmSmoke_old;
  uint32_t alarmEnvDoor_old;        
  uint32_t alarmLIB_old;
  uint32_t alarmGen_old;     
  uint32_t alarmPM_old;
  uint32_t alarmVAC_old;
  uint32_t alarmBM_old;
  uint32_t alarmSMCB_old;
  uint32_t alarmOutOfRangeFreq_Old;
  uint32_t alarmFUEL_old;
  uint32_t alarmISENSE_old;
  uint32_t alarmOutOfRangeFreq2_old;
  
  uint32_t alarmLIBPack_old[16];
  uint32_t alarmGENPack_old[2];
  uint32_t alarmBMPack_old[16];
  uint32_t alarmVACPack_old[1];
  uint32_t alarmSMCBPack_old[5];
  uint32_t alarmOutOfRangeFreqPack_Old[2];
  uint32_t alarmFUELPack_old[2];
  uint32_t alarmOutOfRangeFreqPack2_old[1];
  
  uint32_t alarmBattHighTemp_old2;
  uint32_t alarmLLVD_old2;
  uint32_t alarmBLVD_old2;
  uint32_t alarmDCLow_old2;
  uint32_t alarmACmains_old2;
  uint32_t alarmBattBreakerOpen_old2;
  uint32_t alarmLoadBreakerOpen_old2;
  uint32_t alarmRectACFault_old2;
  uint32_t alarmRectNoResp_old2;
  uint32_t alarmManualMode_old2;
  uint32_t alarmACLow_old2; 
  uint32_t alarmSmoke_old2;
  uint32_t alarmEnvDoor_old2;        
  uint32_t alarmLIB_old2;
  uint32_t alarmGen_old2;
  uint32_t alarmPM_old2;
  uint32_t alarmVAC_old2;
  uint32_t alarmBM_old2;
  uint32_t alarmOutOfRangeFreq_Old2;
  uint32_t alarmFUEL_old2;
  uint32_t alarmISENSE_old2;
  uint32_t alarmOutOfRangeFreq2_old2;
  
  uint32_t alarmLIBPack_old2[16];        
  uint32_t alarmGENPack_old2[2];
  uint32_t alarmBMPack_old2[16];
  uint32_t alarmVACPack_old2[1];
  uint32_t alarmSMCBPack_old2[5];
  uint32_t alarmOutOfRangeFreqPack_Old2[2];
  uint32_t alarmFUELPack_old2[2];
  uint32_t alarmOutOfRangeFreqPack2_Old2[1];
  uint32_t alarmISENSEPack_old2[1];
  uint32_t alarm_pm_dc_pack_old2[1];
  
  uint32_t alarmACSPD;
  uint32_t alarmDCSPD;
  
  uint32_t alarmACSPD_old;
  uint32_t alarmDCSPD_old;
  
}PRIVATE_MIB_MAIN_ALARM_GROUP_STRUCT;

//private mib connect alarm group======================================================

typedef struct{
  uint32_t alarm_lib_connect;
  uint32_t alarm_pmu_connect;
  uint32_t alarm_gen_connect;  
  uint32_t alarm_bm_connect; 
  uint32_t alarm_pm_connect;
  uint32_t alarm_vac_connect;    
  uint32_t alarm_smcb_connect;   
  uint32_t alarm_fuel_connect; 
  uint32_t alarm_isense_connect;
  uint32_t alarm_pmdc_connect;
  //code here 
  uint32_t alarm_dpc_fan_connect;
  
  uint32_t alarmLIBConnect_old;
  uint32_t alarmPMUConnect_old;
  uint32_t alarmGenConnect_old; 
  uint32_t alarmBMConnect_old; 
  uint32_t alarmPMConnect_old;
  uint32_t alarmVACConnect_old;     
  uint32_t alarmSMCBConnect_old;    
  uint32_t alarmFUELConnect_old;    
  uint32_t alarmISENSEConnect_old;  
  
  uint32_t alarmLIBConnect_old2;
  uint32_t alarmPMUConnect_old2;
  uint32_t alarmGenConnect_old2;
  uint32_t alarmBMConnect_old2; 
  uint32_t alarmPMConnect_old2;
  uint32_t alarmVACConnect_old2;  
  uint32_t alarmSMCBConnect_old2;  
  uint32_t alarmFUELConnect_old2;  
  uint32_t alarmISENSEConnect_old2;
  uint32_t alarm_dpc_fan_connect_old2; 
  
  uint32_t alarmPMUConnect_old3; //fix loi ko bao trap SNMP cu tu nguon Agisson.
}PRIVATE__MIB_CONNECT_ALARM_GROUP_STRUCT;
//fan dpc info-------------------------------------------------------- 
typedef struct{
  uint16_t env_temp;
  uint16_t airflow_temp;
  uint16_t voltage;
  uint16_t working_current;
  //state--------------------------
  uint8_t state_fan;
  uint8_t state_device;
  //alarm--------------------------
  uint8_t alarm_fail_refrigeration;
  uint8_t alarm_high_temp;
  uint8_t alarm_low_temp;
  uint8_t alarm_fail_sensor_temp;
  uint8_t alarm_low_voltage;
  uint8_t alarm_high_voltage;
  //parameter----------------------
  uint16_t para_dc_starting_point;
  uint16_t para_dc_sensivive_point;
  int16_t  para_dc_heating_start_point;
  int16_t para_dc_heating_sensivive;
  uint16_t para_alarm_high_temp;
  int16_t  para_alarm_low_temp;
  //parameter set-----------------
  uint32_t temp_set;
  int32_t  i_tem_set;
  //cnt response------------------
  uint8_t  no_response;
}FAN_INFO_DPC_T;
extern FAN_INFO_DPC_T fan_info_dpc_t;

typedef struct {
  FAN_INFO_DPC_T mib;
  
}PrivateMibFANDPCInfo;

//cnt===========================================================================
typedef struct{
  uint32_t control;
  uint32_t modbus_rtu;
  uint32_t agisson;
  uint32_t emerson_m500d;
  uint32_t emerson_m221;
  uint32_t zte;
  uint32_t trap_send;
  uint32_t dpc;
}CNT_TASK;
//extern CNT_TASK  cnt_task;

typedef struct
{
  CNT_TASK    task;
  uint32_t    EMERState;
  uint32_t    EMERV21State;
  uint32_t    ZTEState;
  uint32_t    HWState;
  uint32_t    HWRs485Cmd;
  
  uint32_t RS232ReInitEnable;
} PrivateMibCntGroup;

/**
 * @brief Private MIB base
 **/

//private mib base======================================================================
typedef struct{
  //slave lib batt group------------------------------------------------------
  PrivateMibLIBattInfoGroup                            liBattGroup;
  //slave pm group------------------------------------------------------
  PrivateMibPMGroup                                    pmGroup;
  //slave isense group--------------------------------------------------
  PrivateMibISENSEGroup                                isenseGroup;
  //slave pm dc group---------------------------------------------------
  PrivateMibPMDCGroup                                  pm_dc_group;
  //slave vac group-----------------------------------------------------
  PRIVATE_MIB_VAC_GROUP_STRUCT                         vac_group_struct;
  //slave smcb group----------------------------------------------------
  PrivateMibSMCBInfoGroup                              smcbGroup;
  //slave fuel group----------------------------------------------------
  PrivateMibFUELInfoGroup                              fuelGroup;
  //slave bm group--------------------------------------------------
  PrivateMibBMGroup                                    bmGroup;
  //slave gen group-----------------------------------------------------
  PrivateMibGenInfoGroup                               genGroup;
  //alarm main group----------------------------------------------------
  PRIVATE_MIB_MAIN_ALARM_GROUP_STRUCT                  main_alarm_group_struct;
  //alarm connect group-------------------------------------------------
  PRIVATE__MIB_CONNECT_ALARM_GROUP_STRUCT              connect_alarm_group_struct;
  //load group----------------------------------------------------------
  PRIVATE_MIB_LOAD_GROUP_STRUCT                        loadGroup;
  //fan dpc group-------------------------------------------------------
  PrivateMibFANDPCInfo                                 fan_dpc_info;
  
  
  //batt group----------------------------------------------------------
  PrivateMibBatteryGroup                               batteryGroup;
  //ac group------------------------------------------------------------
  PrivateMibAcPhaseGroup                               acPhaseGroup;
  //rect group----------------------------------------------------------
  PrivateMibRectInfoGroup                              rectGroup; 
  //config group--------------------------------------------------------
  PrivateMibConfigGroup                                configGroup;
  //cfg BT group--------------------------------------------------------
  PrivateMibCfgBTGroup                                 cfgBTGroup;
  //di alarm group------------------------------------------------------
  PrivateMibDIAlarmGroup                               diAlarmGroup;
  //site group----------------------------------------------------------
  PrivateMibSiteInfoGroup                              siteGroup;
  //net work group------------------------------------------------------
  PrivateMibCfgNetworkGroup                            cfgNetworkGroup;
  //cnt group-----------------------------------------------------------
  PrivateMibCntGroup                                   cntGroup;

}PRIVATE_MIB_BASE_STRUCT;

extern PRIVATE_MIB_BASE_STRUCT private_mib_base_struct;
//Private MIB related constants
extern const MibObject privateMibObjects[];
extern const MibModule privateMibModule;

#endif /*_PRIVATE_MIB_MODULE_H*/
