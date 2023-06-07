/*
 * snmp_handler.c
 *
 *  Created on: Mar 18, 2020
 *      Author: 84965
 */
//#include "snmp_handler.h"


#include "core/net.h"
#include "drivers/mac/mimxrt1050_eth_driver.h"
#include "drivers/phy/lan8720_driver.h"
#include "http/http_server.h"
#include "http/mime.h"
#include "path.h"
#include "date_time.h"
#include "resource_manager.h"
#include "debug.h"
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"

#include "snmp/snmp_agent.h"
#include "mibs/mib2_module.h"
#include "mibs/mib2_impl.h"
#include "hardware/mimxrt1050/mimxrt1050_crypto.h"
#include "hardware/mimxrt1050/mimxrt1050_crypto_trng.h"
#include "encoding/oid.h"
#include "rng/trng.h"
#include "daq_v3_private_mib_module.h"
#include "daq_v3_private_mib_impl.h"
#include "header_files.h"


SnmpAgentSettings snmpAgentSettings;
SnmpAgentContext snmpAgentContext;

#define APP_SNMP_ENTERPRISE_OID  "1.3.6.1.4.1.45797.14.20" //"1.3.6.1.4.1.45797.15.20"
#define APP_SNMP_CONTEXT_ENGINE "\x80\x00\x00\x00\x01\x02\x03\x04"

error_t snmpAgentRandCallback(uint8_t *data, size_t length) {
//   //Generate some random data
//   return trngGetRandomData(data, length);
  
   size_t i;

    //Generate some random data
    for(i = 0; i < length; i++)
        data[i] = rand();

    //No error to report
    return NO_ERROR;
}

void snmp_handler_init(void) {
  error_t error;
  size_t oidLen;
  uint8_t oid[SNMP_MAX_OID_SIZE];
  
   snmpAgentGetDefaultSettings(&snmpAgentSettings);
   snmpAgentSettings.interface = &netInterface[0];
   snmpAgentSettings.versionMin = SNMP_VERSION_1;
   snmpAgentSettings.versionMax = SNMP_VERSION_2C;
   
#if (SNMP_V3_SUPPORT == ENABLED)
   snmpAgentSettings.versionMax = SNMP_VERSION_3;
   snmpAgentSettings.randCallback = snmpAgentRandCallback;
#endif
   
   
   error = snmpAgentInit(&snmpAgentContext, &snmpAgentSettings);
   //Failed to initialize SNMP agent?
   if(error)
   {
      //Debug message
      // TRACE_ERROR("Failed to initialize SNMP agent!\r\n");
   }

   //Load standard MIB-II
   snmpAgentLoadMib(&snmpAgentContext, &mib2Module);
   //Load private MIB
   snmpAgentLoadMib(&snmpAgentContext, &privateMibModule);

   //Convert enterprise OID from string representation
   oidFromString(APP_SNMP_ENTERPRISE_OID, oid, sizeof(oid), &oidLen);
   //Set enterprise OID
   snmpAgentSetEnterpriseOid(&snmpAgentContext, oid, oidLen);

   //Set read-only community string
   snmpAgentCreateCommunity(&snmpAgentContext, "public",
      SNMP_ACCESS_READ_ONLY);

   //Set read-write community string
   snmpAgentCreateCommunity(&snmpAgentContext, "private",
      SNMP_ACCESS_READ_WRITE);

#if (SNMP_V3_SUPPORT == ENABLED)
   //Set context engine identifier
   snmpAgentSetContextEngine(&snmpAgentContext,
      APP_SNMP_CONTEXT_ENGINE, sizeof(APP_SNMP_CONTEXT_ENGINE) - 1);

   //Add a new user
   snmpAgentCreateUser(&snmpAgentContext, "usr-md5-none",
      SNMP_ACCESS_READ_WRITE, SNMP_KEY_FORMAT_TEXT,
      SNMP_AUTH_PROTOCOL_MD5, "authkey1",
      SNMP_PRIV_PROTOCOL_NONE, "");

   //Add a new user
   snmpAgentCreateUser(&snmpAgentContext, "usr-md5-des",
      SNMP_ACCESS_READ_WRITE, SNMP_KEY_FORMAT_TEXT,
      SNMP_AUTH_PROTOCOL_MD5, "authkey1",
      SNMP_PRIV_PROTOCOL_DES, "privkey1");
#endif

   //Start SNMP agent
   error = snmpAgentStart(&snmpAgentContext);
   //Failed to start SNMP agent?
   if(error)
   {
      //Debug message
      // TRACE_ERROR("Failed to start SNMP agent!\r\n");
   } 
   //creat task trap send-------------------------------------------------------
   osCreateTask("TrapSend", trapSendTask, NULL, 500, OS_TASK_PRIORITY_NORMAL);
}

//trap send type 3--------------------------------------------------------------
void Trap_Send_Type_3(SnmpAgentContext *context, const IpAddr *destIpAddr,
                      SnmpVersion version, const char_t *username, uint_t genericTrapType,
                      uint_t specificTrapCode, const SnmpTrapObject *objectList, uint_t objectListSize ,
                      uint32_t* pui32value_new, uint32_t* pui32value_old, uint16_t number, uint8_t normalVal)
{  
  //	trap_flag[number] = 0;
  if (*pui32value_new != *pui32value_old)
  {
    //Send a SNMP trap
    snmpAgentSendTrap(context, destIpAddr, version,
                      username,genericTrapType , specificTrapCode, objectList, objectListSize); 
    if (*pui32value_new == normalVal)
    {
      trap_flag[number] = 0;
    }
    else
    {
      trap_flag[number] = 1;
    }
    *pui32value_old = *pui32value_new;
  }
}


void Trap_Send_Type_1(SnmpAgentContext *context, const IpAddr *destIpAddr,
                      SnmpVersion version, const char_t *username, uint_t genericTrapType,
                      uint_t specificTrapCode, const SnmpTrapObject *objectList, uint_t objectListSize,
                      uint32_t* pui32value, uint16_t number, uint8_t alarmVal, uint8_t normalVal)
{
  if ((*pui32value == alarmVal) && (trap_flag[number] == 0))
  {
    trap_flag[number] = 1;
  }
  if (trap_flag[number] == 1)
  {
    if (/*(number >= 0) &&*/ (number <= 128))
    {
      //Send a SNMP trap
      snmpAgentSendTrap(context, destIpAddr, version,
                        username,genericTrapType , specificTrapCode, objectList, objectListSize); 
    }
    trap_flag[number] = 2;
    
  } else if ((trap_flag[number] == 2) && (*pui32value == normalVal))
  {
    if (/*(number >= 0) &&*/ (number <= 128))
    {
      //Send a SNMP trap
      snmpAgentSendTrap(context, destIpAddr, version,
                        username,genericTrapType , specificTrapCode, objectList, objectListSize); 
    }
    
    trap_flag[number] = 0;
  }
}

void Trap_Send_Type_2(SnmpAgentContext *context, const IpAddr *destIpAddr,
                      SnmpVersion version, const char_t *username, uint_t genericTrapType,
                      uint_t specificTrapCode, const SnmpTrapObject *objectList, uint_t objectListSize,
                      uint32_t* pui32value, uint16_t number, uint8_t alarmVal, uint8_t normalVal)
{
  //  if (*pui32value == alarmVal) 
  if (*pui32value != normalVal) 
  {
    trap_flag[number] = 1;
  }
  if (trap_flag[number] == 1)
  {
    if (/*(number >= 0) &&*/ (number <= 200))
    {
      //Send a SNMP trap
      snmpAgentSendTrap(context, destIpAddr, version,
                        username,genericTrapType , specificTrapCode, objectList, objectListSize); 
      if (*pui32value == normalVal) 
      {
        trap_flag[number] = 0;
      }
    }
  } 
}
uint8_t g_u8OidStr[28];
uint8_t g_u8OidStr1[35];//30
//uint32_t* ptr1 = &private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp;
//uint32_t* ptr2 = &private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0];
void snmp_TrapSend(void)
{
  IpAddr destIpAddr;
  SnmpTrapObject trapObjects[2];
  uint8_t i;
  
  uint32_t* ptr1 = &private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp;
  uint32_t* ptr2 = &private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0];
  uint32_t* ptr3 = &private_mib_base_struct.connect_alarm_group_struct.alarm_lib_connect;
  uint32_t* ptr4 = &private_mib_base_struct.main_alarm_group_struct.alarm_lib_pack[0];
  uint32_t* ptr5 = &private_mib_base_struct.main_alarm_group_struct.alarm_gen_pack[0];  
  uint32_t* ptr6 = &private_mib_base_struct.main_alarm_group_struct.alarm_bm_pack[0];  
  uint32_t* ptr7 = &private_mib_base_struct.main_alarm_group_struct.alarm_vac_pack[0]; 
  uint32_t* ptr8 = &private_mib_base_struct.main_alarm_group_struct.alarm_smcb_pack[0];
  uint32_t* ptr9 = &private_mib_base_struct.main_alarm_group_struct.alarm_out_of_range_freq_pack[0];
  uint32_t* ptr10 = &private_mib_base_struct.main_alarm_group_struct.alarm_out_of_range_freq_pack2[0];
  //uint32_t* ptr11 = &private_mib_base_struct.main_alarm_group_struct.alarm_isense_pack[0];
  uint32_t* ptr12 = &private_mib_base_struct.main_alarm_group_struct.alarm_pm_dc_pack[0];
  uint32_t* ptr13 = &private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect;
  
  uint32_t* ptr1_old = &private_mib_base_struct.main_alarm_group_struct.alarmBattHighTemp_old2;
  uint32_t* ptr2_old = &private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old2[0];
  uint32_t* ptr3_old = &private_mib_base_struct.connect_alarm_group_struct.alarmLIBConnect_old2;
  uint32_t* ptr4_old = &private_mib_base_struct.main_alarm_group_struct.alarmLIBPack_old2[0];
  uint32_t* ptr5_old = &private_mib_base_struct.main_alarm_group_struct.alarmGENPack_old2[0];       
  uint32_t* ptr6_old = &private_mib_base_struct.main_alarm_group_struct.alarmBMPack_old2[0]; 
  uint32_t* ptr7_old = &private_mib_base_struct.main_alarm_group_struct.alarmVACPack_old2[0]; 
  uint32_t* ptr8_old = &private_mib_base_struct.main_alarm_group_struct.alarmSMCBPack_old2[0]; 
  uint32_t* ptr9_old = &private_mib_base_struct.main_alarm_group_struct.alarmOutOfRangeFreqPack_Old2[0]; 
  uint32_t* ptr10_old = &private_mib_base_struct.main_alarm_group_struct.alarmOutOfRangeFreqPack2_Old2[0];
  //uint32_t* ptr11_old = &private_mib_base_struct.main_alarm_group_struct.alarmISENSEPack_old2[0];
  uint32_t* ptr12_old = &private_mib_base_struct.main_alarm_group_struct.alarm_pm_dc_pack_old2[0];
  uint32_t* ptr13_old = &private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect_old2;
  
  //Destination IP address
  ipStringToAddr((const char_t*)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP, &destIpAddr);
  
  switch(private_mib_base_struct.cfgNetworkGroup.u8TrapMode)
  {
  case 0:
    {
    }break;
  case 1:
    {            
      for (i = 1; i <= 16; i++)
      {
        sprintf((char*)g_u8OidStr,"1.3.6.1.4.1.45797.14.%d.%d.0",
                15,
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr, trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,i, trapObjects, 2,                             
                         ptr1 + i - 1, ptr1_old + i - 1,i - 1,sAlarmValue[i-1].u8NormalValue);         
        
        ptr1 = &private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp;
        ptr1_old = &private_mib_base_struct.main_alarm_group_struct.alarmBattHighTemp_old2;
      }            
      i = 0;
      for (i = 1; i <= 8; i++)
      {
        sprintf((char*)g_u8OidStr,"1.3.6.1.4.1.45797.14.%d.%d.0",
                16,
                i);
        
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr, trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,100+i, trapObjects, 2,                             
                         ptr2 + i - 1,ptr2_old + i - 1,100+i - 1,sAlarmValue[100+i-1].u8NormalValue);
        ptr2 = &private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0];
        ptr2_old = &private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old2[0];
      }
      i = 0;
      for (i = 1; i <= 9; i++)
      {
        sprintf((char*)g_u8OidStr,"1.3.6.1.4.1.45797.14.%d.%d.0",
                17,
                i);
        
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr, trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,150+i, trapObjects, 2,                             
                         ptr3 + i - 1,ptr3_old + i - 1,150+i - 1,sAlarmValue[50+i-1].u8NormalValue);
        ptr3 = &private_mib_base_struct.connect_alarm_group_struct.alarm_lib_connect;
        ptr3_old = &private_mib_base_struct.connect_alarm_group_struct.alarmLIBConnect_old2;
      }
      for (i = 1; i <= 16; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.9.5.1.49.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,14, trapObjects, 2,                             
                         ptr4 + i - 1, ptr4_old + i - 1,20+i - 1,sAlarmValue[20+i-1].u8NormalValue);         
        
        ptr4 = &private_mib_base_struct.main_alarm_group_struct.alarm_lib_pack[0];
        ptr4_old = &private_mib_base_struct.main_alarm_group_struct.alarmLIBPack_old2[0];
      } 
      for (i = 1; i <= 2; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.10.3.1.25.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,15, trapObjects, 2,                             
                         ptr5 + i - 1, ptr5_old + i - 1,36+i - 1,sAlarmValue[36+i-1].u8NormalValue);         
        
        ptr5 = &private_mib_base_struct.main_alarm_group_struct.alarm_gen_pack[0];
        ptr5_old = &private_mib_base_struct.main_alarm_group_struct.alarmGENPack_old2[0];
      } 
      
      for (i = 1; i <= 16; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.6.3.1.16.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,16, trapObjects, 2,                             
                         ptr6 + i - 1, ptr6_old + i - 1,38+i - 1,sAlarmValue[38+i-1].u8NormalValue);         
        
        ptr6 = &private_mib_base_struct.main_alarm_group_struct.alarm_bm_pack[0];
        ptr6_old = &private_mib_base_struct.main_alarm_group_struct.alarmBMPack_old2[0];
      }
      
      for (i = 1; i <= 1; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.12.3.1.12.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,17, trapObjects, 2,                             
                         ptr7 + i - 1, ptr7_old + i - 1,54+i - 1,sAlarmValue[54+i-1].u8NormalValue); 
        
        ptr7 = &private_mib_base_struct.main_alarm_group_struct.alarm_vac_pack[0];
        ptr7_old = &private_mib_base_struct.main_alarm_group_struct.alarmVACPack_old2[0];
      }
      
      for (i = 1; i <= 5; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.18.3.1.3.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,18, trapObjects, 2,                             
                         ptr8 + i - 1, ptr8_old + i - 1,55+i - 1,sAlarmValue[55+i-1].u8NormalValue);         
        
        ptr8 = &private_mib_base_struct.main_alarm_group_struct.alarm_smcb_pack[0];
        ptr8_old = &private_mib_base_struct.main_alarm_group_struct.alarmSMCBPack_old2[0];
      }
      
      for (i = 1; i <= 2; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.11.3.1.35.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,19, trapObjects, 2,                             
                         ptr9 + i - 1, ptr9_old + i - 1,60+i - 1,sAlarmValue[60+i-1].u8NormalValue);         
        
        ptr9 = &private_mib_base_struct.main_alarm_group_struct.alarm_out_of_range_freq_pack[0];
        ptr9_old = &private_mib_base_struct.main_alarm_group_struct.alarmOutOfRangeFreqPack_Old2[0];
      } 
      //=== ISENSE
      sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.21.3.1.20.1");
      //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
      oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
      //Add the siteInfoBTSCode.0 object to the variable binding list of the message
      oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
      Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                       "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,20, trapObjects, 2,                             
                       ptr10, ptr10_old,62,sAlarmValue[62].u8NormalValue); 
      
      ptr10 = &private_mib_base_struct.main_alarm_group_struct.alarm_out_of_range_freq_pack2[0];
      ptr10_old = &private_mib_base_struct.main_alarm_group_struct.alarmOutOfRangeFreqPack2_Old2[0];
      
      //----------them trap mat ket noi--------------
      //              sprintf(g_u8OidStr1,"1.3.6.1.4.1.45797.14.17.9.1");
      //              oidFromString(g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
      //              oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
      //              Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
      //                                  "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,159, trapObjects, 2,                           
      //                                  ptr11, ptr11_old,63,sAlarmValue[63].u8NormalValue);
      //              
      //              ptr11= &private_mib_base_struct.main_alarm_group_struct.alarmISENSEPack[0];
      //              ptr11_old= &private_mib_base_struct.main_alarm_group_struct.alarmISENSEPack_old2[0];
      //PM DC ========================================================================================================
      //no connect trap------------------------------
      sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.17.10.0");
      oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
      oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
      Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                       "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,160, trapObjects, 2,                             
                       ptr12, ptr12_old,64,sAlarmValue[64].u8NormalValue);
      ptr12= &private_mib_base_struct.main_alarm_group_struct.alarm_pm_dc_pack[0];
      ptr12_old= &private_mib_base_struct.main_alarm_group_struct.alarm_pm_dc_pack_old2[0];
      //FAN DPC ======================================================================================================
      //no connect trap-------------------------------
      sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.17.11.0");
      oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
      oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
      Trap_Send_Type_3(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                       "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,161, trapObjects, 2,                             
                       ptr13, ptr13_old,65,sAlarmValue[65].u8NormalValue);
      ptr13    = &private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect;
      ptr13_old= &private_mib_base_struct.connect_alarm_group_struct.alarm_dpc_fan_connect_old2;
      
      
      //==============================================================================================================
      if (private_mib_base_struct.cfgNetworkGroup.u8NewFWFlag == 1)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.1.4.0");
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);    
        //Send a SNMP trap
        snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C, "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 109, trapObjects, 2); 
        private_mib_base_struct.cfgNetworkGroup.u8NewFWFlag = 0;             
      }
      
    }break;
  case 2:
    {
      
      for (i = 1; i <= 16; i++)
      {
        sprintf((char*)g_u8OidStr,"1.3.6.1.4.1.45797.14.%d.%d.0",
                15,
                i);
        
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr, trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,i, trapObjects, 2,                             
                         ptr1 + i - 1,i - 1,sAlarmValue[i-1].u8AlarmValue,sAlarmValue[i-1].u8NormalValue);
        ptr1 = &private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp;
      }
      for (i = 1; i <= 8; i++)
      {
        sprintf((char*)g_u8OidStr,"1.3.6.1.4.1.45797.14.%d.%d.0",
                16,
                i);
        
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr, trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,100+i, trapObjects, 2,                             
                         ptr2 + i - 1,100+i - 1,sAlarmValue[100+i-1].u8AlarmValue,sAlarmValue[100+i-1].u8NormalValue);
        ptr2 = &private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0];
      }
      i = 0;
      for (i = 1; i <= 7; i++)
      {
        sprintf((char*)g_u8OidStr,"1.3.6.1.4.1.45797.14.%d.%d.0",
                17,
                i);
        
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr, trapObjects[0].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid,
                      SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,150+i, trapObjects, 2,                             
                         ptr3 + i - 1,150+i - 1,sAlarmValue[150+i-1].u8AlarmValue,sAlarmValue[50+i-1].u8NormalValue);
        ptr3 = &private_mib_base_struct.connect_alarm_group_struct.alarm_lib_connect;
      }
      for (i = 1; i <= 16; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.9.5.1.49.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,14, trapObjects, 2,                             
                         ptr4 + i - 1,20+i - 1,sAlarmValue[20+i-1].u8AlarmValue,sAlarmValue[20+i-1].u8NormalValue);         
        
        ptr4 = &private_mib_base_struct.main_alarm_group_struct.alarm_lib_pack[0];
      }  
      for (i = 1; i <= 2; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.10.3.1.25.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,15, trapObjects, 2,                             
                         ptr5 + i - 1,36+i - 1,sAlarmValue[36+i-1].u8AlarmValue,sAlarmValue[36+i-1].u8NormalValue);         
        
        ptr5 = &private_mib_base_struct.main_alarm_group_struct.alarm_gen_pack[0];
      }  
      for (i = 1; i <= 16; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.6.3.1.16.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,16, trapObjects, 2,                             
                         ptr6 + i - 1,38+i - 1,sAlarmValue[38+i-1].u8AlarmValue,sAlarmValue[38+i-1].u8NormalValue);         
        
        ptr6 = &private_mib_base_struct.main_alarm_group_struct.alarm_bm_pack[0];
      }
      for (i = 1; i <= 1; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.12.3.1.12.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,17, trapObjects, 2,                             
                         ptr7 + i - 1,54+i - 1,sAlarmValue[54+i-1].u8AlarmValue,sAlarmValue[54+i-1].u8NormalValue);         
        
        ptr7 = &private_mib_base_struct.main_alarm_group_struct.alarm_vac_pack[0];
      }
      for (i = 1; i <= 5; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.18.3.1.3.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,18, trapObjects, 2,                             
                         ptr8 + i - 1,55+i - 1,sAlarmValue[55+i-1].u8AlarmValue,sAlarmValue[55+i-1].u8NormalValue);         
        
        ptr8 = &private_mib_base_struct.main_alarm_group_struct.alarm_smcb_pack[0];
      }
      
      for (i = 1; i <= 2; i++)
      {
        sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.11.3.1.35.%d",
                i);
        //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
        oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
        //Add the siteInfoBTSCode.0 object to the variable binding list of the message
        oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);                 
        Trap_Send_Type_2(&snmpAgentContext, &destIpAddr,SNMP_VERSION_2C,
                         "public", SNMP_TRAP_ENTERPRISE_SPECIFIC,19, trapObjects, 2,                             
                         ptr9 + i - 1,60+i - 1,sAlarmValue[60+i-1].u8AlarmValue,sAlarmValue[60+i-1].u8NormalValue);         
        
        ptr9 = &private_mib_base_struct.main_alarm_group_struct.alarm_out_of_range_freq_pack[0];
      }
      //trap sen ISENSE
      for (i = 1; i <= 1; i++)
        
        
        if (private_mib_base_struct.cfgNetworkGroup.u8NewFWFlag == 1)
        {
          sprintf((char*)g_u8OidStr1,"1.3.6.1.4.1.45797.14.1.4.0");
          //Add the alarm_batt_high_temp.0 object to the variable binding list of the message
          oidFromString((const char_t*)g_u8OidStr1, trapObjects[0].oid, SNMP_MAX_OID_SIZE, &trapObjects[0].oidLen);
          //Add the siteInfoBTSCode.0 object to the variable binding list of the message
          oidFromString("1.3.6.1.4.1.45797.14.1.1.0", trapObjects[1].oid, SNMP_MAX_OID_SIZE, &trapObjects[1].oidLen);    
          //Send a SNMP trap
          snmpAgentSendTrap(&snmpAgentContext, &destIpAddr, SNMP_VERSION_2C, "public",SNMP_TRAP_ENTERPRISE_SPECIFIC , 109, trapObjects, 2); 
          private_mib_base_struct.cfgNetworkGroup.u8NewFWFlag = 0;
        }
    }break;
  };         
}

void trapSendTask(void *param)
{
  private_mib_base_struct.cntGroup.task.trap_send = 0;
  //	//Endless loop
  while(1)
  {
    private_mib_base_struct.cntGroup.task.trap_send++;
    if(trapStatus_TimePeriod >650 * private_mib_base_struct.cfgNetworkGroup.u32TrapInterval)
    {
      snmp_TrapSend();
      trapStatus_TimePeriod = 0; 
    }            
    osDelayTask(100);
  }
  //        while(1)
  //	{
  //          if(private_mib_base_struct.cfgNetworkGroup.u32TrapTick > 550 * private_mib_base_struct.cfgNetworkGroup.u32TrapInterval)
  //          {
  //            snmp_TrapSend();
  //            private_mib_base_struct.cfgNetworkGroup.u32TrapTick = 0;trapStatus_TimePeriod
  //          }            
  //          osDelayTask(100);
  //	}
}

