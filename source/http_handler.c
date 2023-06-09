/*
 * http_handler.c
 *
 *  Created on: Mar 18, 2020
 *      Author: 84965
 */

#include "core/net.h"
//#include "drivers/mac/mimxrt1050_eth_driver.h"
//#include "drivers/phy/lan8720_driver.h"
#include "http/http_server.h"
#include "http/mime.h"
#include "path.h"
#include "resource_manager.h"

#include "daq_v3_private_mib_module.h"
#include "header_files.h"
#define APP_HTTP_MAX_CONNECTIONS 4
#define HTTP_BUFF 16000
char sBuffHttp[HTTP_BUFF];
uint16_t text_sBuffHttp_len;
const char *ucdata;
SYSTEM_CONFIG_STRUCT sIpConfig;
bool bConfirmed[12];
SYSTEM_CONFIG_STRUCT sConfigParams1;

HttpServerSettings httpServerSettings;
HttpServerContext httpServerContext;
HttpConnection httpConnections[APP_HTTP_MAX_CONNECTIONS];

error_t httpServerCgiCallback(HttpConnection *connection, const char_t *param) {
   //Not implemented
   return ERROR_INVALID_TAG;
}

/**
 * @brief namht4
 **/
error_t httpServeruriResponse(char_t *pBuff, uint32_t len, HttpConnection *connection) {
  error_t error;
  //Format HTTP response header
  connection->response.version = connection->request.version;
  connection->response.statusCode = 200;
  connection->response.keepAlive = connection->request.keepAlive;
  connection->response.noCache = TRUE;
  connection->response.contentType = mimeGetType(".html");
  connection->response.chunkedEncoding = FALSE;
  connection->response.contentLength = len;
  
  //Send the header to the client
  error = httpWriteHeader(connection);
  //Any error to report?
  if(error) return error;
  
  //Send response body
  error = httpWriteStream(connection, pBuff, len);
  //Any error to report?
  if (error) return error;
  
  //Properly close output stream
  error = httpCloseStream(connection);
  //Return status code
  return error;  
}

error_t httpServeruriCallback(HttpConnection *connection, const char_t *uri) {
  uint32_t n = 0;
  const char *data;
  
//device information============================================================  
  if(!strcasecmp(uri, "/dev_status")) {
    n = sprintf(sBuffHttp,"%s;%s;%s;%s;%s-V3.02;%s;%s;%d",
                private_mib_base_struct.siteGroup.siteDevModel,
                "Power System Monitor",
                private_mib_base_struct.siteGroup.siteSerialNumber,
                "HWV1R3",
                private_mib_base_struct.siteGroup.siteCRCStr,
                working_default_parameter_2.u8CompileInfo,
                private_mib_base_struct.siteGroup.siteMACInfo,
                system_flag_struct.eth_type
                );
  }
  
//serial port information=======================================================
  else if(!strcasecmp(uri, "/sp_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%d",
                9600,
                8,
                0,
                1
                 );
  }
//snmp information==============================================================
  else if(!strcasecmp(uri, "/snmp_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%s",
                private_mib_base_struct.cfgNetworkGroup.u8TrapMode,
                private_mib_base_struct.cfgNetworkGroup.u32TrapInterval,
                private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP
                );
  }
  // config trap enable---------------------------------------------------------
  else if(strncmp(uri, "/trapmode?value=", 9) == 0)
  {
    // Trap Interval Value
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&sConfigParams1.ip_parameter.trap_mode,sizeof("trapmode"));
    // Did parameters change?
    if  (parameter.ip_parameter.trap_mode != sConfigParams1.ip_parameter.trap_mode)
    {
      // Update the current parameters with the new settings.
      parameter.ip_parameter.trap_mode = sConfigParams1.ip_parameter.trap_mode;
      private_mib_base_struct.cfgNetworkGroup.u8TrapMode = parameter.ip_parameter.trap_mode;
      private_mib_base_struct.siteGroup.siteTrapEnable = sConfigParams1.ip_parameter.trap_mode;
      // Yes - save these settings as the defaults.
      working_default_parameter.ip_parameter.trap_mode = parameter.ip_parameter.trap_mode;
      flag_struct.save_config_flag |=1;
    }
  }
  //config trap interval--------------------------------------------------------
  
  else if(strncmp(uri, "/traptime?value=", 9) == 0)
  {
    // Trap Interval Value
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&sConfigParams1.ip_parameter.trap_period,sizeof("traptime"));
    // Did parameters change?
    if  (parameter.ip_parameter.trap_period != sConfigParams1.ip_parameter.trap_period)
    {
      // Update the current parameters with the new settings.
      parameter.ip_parameter.trap_period = sConfigParams1.ip_parameter.trap_period;
      private_mib_base_struct.cfgNetworkGroup.u32TrapInterval = parameter.ip_parameter.trap_period;
      // Yes - save these settings as the defaults.
      working_default_parameter.ip_parameter.trap_period = parameter.ip_parameter.trap_period;
      
      flag_struct.save_config_flag |=1;
    }        
  }
  //config snmp ip--------------------------------------------------------------
  else if(strncmp(uri, "/telnetip1?value=", 9) == 0)
  {
    uint32_t temp=0;
    // Trap Interval Value
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("telnetip1"));
    data = uri;
    switch (data[sizeof("telnetip1")-1]-0x30)
    {
    case 1:	sConfigParams1.ip_parameter.snmp_ip = (sConfigParams1.ip_parameter.snmp_ip &  0x00ffffff) | (temp << 24);
    break;
    case 2:	sConfigParams1.ip_parameter.snmp_ip = (sConfigParams1.ip_parameter.snmp_ip &  0xff00ffff) | (temp << 16);
    break;
    case 3:	sConfigParams1.ip_parameter.snmp_ip = (sConfigParams1.ip_parameter.snmp_ip &  0xffff00ff) | (temp << 8);
    break;
    case 4:	sConfigParams1.ip_parameter.snmp_ip = (sConfigParams1.ip_parameter.snmp_ip &  0xffffff00) | (temp << 0);
    break;
    default:
      break;
    }
    // Did parameters change?
    if  (parameter.ip_parameter.snmp_ip != sConfigParams1.ip_parameter.snmp_ip)
    {
      // Update the current parameters with the new settings.
      parameter.ip_parameter.snmp_ip = sConfigParams1.ip_parameter.snmp_ip;
      private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SIP = revertIP(parameter.ip_parameter.snmp_ip);
      strcpy((char *)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP, ipv4AddrToString((Ipv4Addr)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SIP,0));//
      // Yes - save these settings as the defaults.
      working_default_parameter.ip_parameter.snmp_ip = parameter.ip_parameter.snmp_ip;
      
      flag_struct.save_config_flag |=1;  
    }
    
  }
//bts information===============================================================
  
  else if(!strcasecmp(uri, "/bts_status")) {
    n = sprintf(sBuffHttp,"%s;%s;%s;%s;%d;%d;%d",
                private_mib_base_struct.siteGroup.siteBTSCode,
                "Emerson",
                "Emerson Ctrler",
                "DC 48V",
                private_mib_base_struct.siteGroup.siteAmbientTemp,
                private_mib_base_struct.siteGroup.siteControllerModel[0],
                private_mib_base_struct.siteGroup.siteControllerModel[1]
                );
  }
  
  //VAC=============================================================================================
  //slave vac info-------------------------------------------------
  else if(!strcasecmp(uri,"/vacInfo_status")){
    n = sprintf(sBuffHttp,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%x;%d;%s;%d",
       /* 1*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_installed_fan,
       /* 2*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_active_fan,
       /* 3*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_type,
       /* 4*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_sys_mode,
       /* 5*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_fan_speed_mode,
       /* 6*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_in_temp,
       /* 7*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_ex_temp,
       /* 8*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_fan1_duty,
       /* 9*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_fan1_rpm,
       /*10*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_alarm_status.all,
       /*11*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_status,
       /*12*/   &private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_serial[0],
       /*13*/   private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_firm_version);
    
  }
  
  else if(!strcasecmp(uri, "/vac_status")){
    uint8_t i;
    n=0;
    for (i = 0; i < private_mib_base_struct.vac_group_struct.vac_installed_vac; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%s@",
                      private_mib_base_struct.vac_group_struct.vac_table_struct[i].vac_modbus_id,
                      private_mib_base_struct.vac_group_struct.vac_table_struct[i].vac_type,
                      "VAC"
                        );
      }
    }
    
  }
  //vac setting----------------------------------------------------
  else if(strncmp(uri, "/vacnum1i?value=", 8) == 0)
  {
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("vacnum1i"));
    data = uri;
    j = data[sizeof("vacnum1i")-2]-0x30;
    switch (j)
    {
    case 1:
    case 2:
      {
        if(strncmp(&data[sizeof("vacnum1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.vac_manager_struct[j-1].vac_type = temp;
          switch(modbus_manager_struct.vac_manager_struct[j-1].vac_type)
          {
          case 1:// VIETTEL
            {
              modbus_manager_struct.vac_manager_struct[j-1].vac_slave_offset    = 34;
              modbus_manager_struct.vac_manager_struct[j-1].vac_abs_slave_id    = 34+j;
              modbus_manager_struct.vac_manager_struct[j-1].vac_base_addr[0]    = 0;
              modbus_manager_struct.vac_manager_struct[j-1].vac_base_addr[1]    = 0;
              modbus_manager_struct.vac_manager_struct[j-1].vac_base_addr[2]    = 80;
              modbus_manager_struct.vac_manager_struct[j-1].vac_num_of_reg[0]   = 14;    
              modbus_manager_struct.vac_manager_struct[j-1].vac_num_of_reg[1]   = 39;   
              modbus_manager_struct.vac_manager_struct[j-1].vac_num_of_reg[2]   = 3;
              
            }
            break;
            
          case 2:// 
            {                                     
            }
            break;
          };

          // Did parameters change?------------------------------------------------------
          if  ( parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor != modbus_manager_struct.vac_manager_struct[j-1].vac_type)
          {
            // Update the current parameters with the new settings-----------------------
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor = modbus_manager_struct.vac_manager_struct[j-1].vac_type;
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].slave_id      = modbus_manager_struct.vac_manager_struct[j-1].vac_abs_slave_id;
            // Yes - save these settings as the defaults---------------------------------
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor;
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].slave_id      = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].slave_id;
            
            flag_struct.save_config_flag |=1;
          }                 
        }
      }
      break;
    default:
      break;
    };
  }
  else if(strncmp(uri, "/vacTotal?value=", 8) == 0)
  {
    uint32_t temp=0;
    // get vac  total-------------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("vacTotal"));
    if (temp != modbus_manager_struct.number_of_vac)
    {
      modbus_manager_struct.number_of_vac      = temp;
      parameter.device_number[TOTAL_VAC_INDEX] = modbus_manager_struct.number_of_vac;
      working_default_parameter.device_number[TOTAL_VAC_INDEX] = parameter.device_number[TOTAL_VAC_INDEX];
      flag_struct.save_config_flag |=1;
    }
  }
  //LIB==========================================================================================
  //slave lib info-------------------------------------------------
  else if(!strcasecmp(uri, "/libbatt_status"))//edit sBuffHttp 300 to 500
  {
    uint8_t i;
    n=0;
    for (i = 0; i < 16; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%d;%d;%d@",
                      i + 1,
                      private_mib_base_struct.liBattGroup.liBattTable[i].liBattPackVolt,
                      private_mib_base_struct.liBattGroup.liBattTable[i].liBattPackCurr,
                      private_mib_base_struct.liBattGroup.liBattTable[i].liBattAmbTemp,
                      private_mib_base_struct.liBattGroup.liBattTable[i].liBattSOC
                        );
      }
    }
  }
  else if(!strcasecmp(uri, "/liion_status"))//edit sBuffHttp 300 to 500
  {
    uint8_t i;
    n=0;
    for (i = 0; i < private_mib_base_struct.liBattGroup.liBattInstalledPack; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%s@",
                      private_mib_base_struct.liBattGroup.liBattTable[i].liBattModbusID,
                      private_mib_base_struct.liBattGroup.liBattTable[i].liBattType,
                      private_mib_base_struct.liBattGroup.liBattTable[i].liBattSerialNo
                      );
      }
    }
  }
  
  //slave lib setting----------------------------------------------------
  else if(strncmp(uri, "/liionpack1i?value=", 10) == 0)
  {
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("liionpack1i"));
    data = uri;
    j = data[sizeof("liionpack1i")-2]-0x30;
    switch (j)
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
      {
        if(strncmp(&data[sizeof("liionpack1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.lib_manager_struct[j-1].lib_type = temp;
          switch(modbus_manager_struct.lib_manager_struct[j-1].lib_type)
          {
          case 13:// COSLIGHT_CF4850T
          case 1:// COSLIGHT
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;      // FC 04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 58;      
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 128;    // FC 02
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 30; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 48;    // FC 03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 4;
            }
            break;
          case 2:// COSLIGHT_OLD_V1.1
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 30;      // FC 04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 24;      
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 128;    // FC 02
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 28; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 48;    // FC 03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 2;
            }
            break;
          case 12:// SHOTO_SDA10_48100
          case 3:// SHOTO_2019
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0x1000;      // FC 04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 10;      
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 0x2000;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 25; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 0x3040;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 56; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[3] = 0x800f;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[3] = 16; 
            }
            break;
          case 4:// HUAWEI
            {
              if(j == 9)
                modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 215;
              else
                modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 213;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset + j;
              //                    modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = 213 + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 50; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 67;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 8;  
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 257;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 7;  
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[3] = 529;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[3] = 10; 
            }
            break;
          case 5:// M1Viettel50
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0; // FC:0x04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 100;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 130;// FC:0x04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 22;  //20
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 0;// FC:0x03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 80; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[3] = 162;// FC:0x03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[3] = 3;//2                 
            }
            break;
          case 14:// HUAFU_HF48100C
          case 6:// ZTT_2020
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 42;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 150;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 10;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 170;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 10;
            }
            break;
          case 9:// SAFT
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 23764;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 16;
            }
            break;
          case 10:// Narada75
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 38;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = 38 + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 4095;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 9;
              
            }
            break;
          case 7:// ZTT50
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 38;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = 38 + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 4096;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 9;
            }
            break;
          case 8:// HUAWEI_A1
            {
              if(j == 9)
                modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 215;
              else
                modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 213;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 50; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 67;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 8;  
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 257;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 7;                  
            }
            break;
          case 11:// EVE
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 42;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 150;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 10;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 170;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 10;
            }
            break;
          };
          // Did parameters change?---------------------------------------------
          if  (parameter.modbus_parameter[j-1].device_vendor != modbus_manager_struct.lib_manager_struct[j-1].lib_type)
          {
            // Update the current parameters with the new settings--------------
            parameter.modbus_parameter[j-1].device_vendor = modbus_manager_struct.lib_manager_struct[j-1].lib_type;
            parameter.modbus_parameter[j-1].slave_id      = modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id;
            // Yes - save these settings as the defaults------------------------
            working_default_parameter.modbus_parameter[j-1].device_vendor = parameter.modbus_parameter[j-1].device_vendor;
            working_default_parameter.modbus_parameter[j-1].slave_id = parameter.modbus_parameter[j-1].slave_id;
            
            flag_struct.save_config_flag |=1;
          } 
          
        }
      }
      break;
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
      {
        j = j - 7;
        if(strncmp(&data[sizeof("liionpack1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.lib_manager_struct[j-1].lib_type = temp;
          switch(modbus_manager_struct.lib_manager_struct[j-1].lib_type)
          {
          case 13:// COSLIGHT_CF4850T
          case 1:// COSLIGHT
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;      // FC 04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 58;      
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 128;    // FC 02
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 30; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 48;    // FC 03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 4;
            }
            break;
          case 2:// COSLIGHT_OLD_V1.1
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 30;      // FC 04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 24;      
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 128;    // FC 02
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 28; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 48;    // FC 03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 2;
            }
            break;
          case 12:// SHOTO_SDA10_48100
          case 3:// SHOTO_2019
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0x1000;      // FC 04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 10;      
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 0x2000;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 25; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 0x3040;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 56; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[3] = 0x800f;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[3] = 16; 
            }
            break;
          case 4:// HUAWEI
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 215;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = 215 + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 50; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 67;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 8;  
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 257;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 7;  
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[3] = 529;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[3] = 10; 
            }
            break;
          case 5:// M1Viettel50
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0; // FC:0x04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 100;   
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 130;// FC:0x04
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 22;  //20;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 0;// FC:0x03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 80; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[3] = 162;// FC:0x03
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[3] = 3;//2                
            }
            break;
            
            //              case 8:// BYU75
            //              {
            //                  modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
            //                  modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
            //                  modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
            //                  modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 54;                  
            //              }
            //              break;
          case 14: // HUAFU_HF48100C
          case 6: // ZTT_2020
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 42;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 150;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 10;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 170;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 10;
            }
            break;
          case 9:// SAFT
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 23764;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 16;
            }
            break;
          case 10:// Narada75
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 38;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = 38 + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 4095;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 9;
              
            }
            break;
          case 7:// ZTT50
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 38;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = 38 + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 4096;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 9;
            }
            break;
          case 8:// HUAWEI_A1
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 215;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = 215 + j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 50; 
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 67;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 8;  
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 257;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 7;                  
            }
            break;
          case 11:// EVE
            {
              modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 42;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 150;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 10;
              modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 170;
              modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 10;
            }
            break;
          };
          // Did parameters change?--------------------------------------------
          if  (parameter.modbus_parameter[j-1].device_vendor != modbus_manager_struct.lib_manager_struct[j-1].lib_type)
          {
            // Update the current parameters with the new settings.-------------
            parameter.modbus_parameter[j-1].device_vendor = modbus_manager_struct.lib_manager_struct[j-1].lib_type;
            parameter.modbus_parameter[j-1].slave_id = modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id;
            // Yes - save these settings as the defaults.-----------------------
            working_default_parameter.modbus_parameter[j-1].device_vendor = parameter.modbus_parameter[j-1].device_vendor;
            working_default_parameter.modbus_parameter[j-1].slave_id = parameter.modbus_parameter[j-1].slave_id;
            
            flag_struct.save_config_flag |=1;
          } 
          
        }
      }
      break;
      
    default:
      break;
    };
  }
  else if(strncmp(uri, "/libTotal?value=", 8) == 0)
  {
    uint32_t temp=0;
    // lib batt total---------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("libTotal"));
    if (temp != modbus_manager_struct.number_of_lib)
    {
      modbus_manager_struct.number_of_lib = temp;
      parameter.device_number[TOTAL_LIB_INDEX] = modbus_manager_struct.number_of_lib;
      working_default_parameter.device_number[TOTAL_LIB_INDEX] = parameter.device_number[TOTAL_LIB_INDEX];
      flag_struct.save_config_flag |=1;
    }
  }
  //GEN=============================================================================================
  //slave gen info------------------------------------------------------
  else if(!strcasecmp(uri, "/geninfo_status"))
  {
    uint8_t i;
    n=0;
    for (i = 0; i < 2; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d@",
                      i + 1,
                      private_mib_base_struct.genGroup.genTable[i].genStatusStatus,
                      private_mib_base_struct.genGroup.genTable[i].genStatusLNVolt1,
                      private_mib_base_struct.genGroup.genTable[i].genStatusLNVolt2,
                      private_mib_base_struct.genGroup.genTable[i].genStatusLNVolt3,
                      private_mib_base_struct.genGroup.genTable[i].genStatusLNCurr1,
                      private_mib_base_struct.genGroup.genTable[i].genStatusLNCurr2,
                      private_mib_base_struct.genGroup.genTable[i].genStatusLNCurr3,
                      private_mib_base_struct.genGroup.genTable[i].genStatusOilPressure,
                      private_mib_base_struct.genGroup.genTable[i].genStatusFuelLevel,
                      private_mib_base_struct.genGroup.genTable[i].genStatusBattVolt,
                      private_mib_base_struct.genGroup.genTable[i].genStatusFrequency,
                      private_mib_base_struct.genGroup.genTable[i].genStatusCoolantTemp,
                      private_mib_base_struct.genGroup.genTable[i].genStatusWorkingHour,
                      private_mib_base_struct.genGroup.genTable[i].genStatusEngineSpeed
                        );
      }
    }
  } 
  else if(!strcasecmp(uri, "/generator_status"))
  {
    uint8_t i;
    n=0;
    for (i = 0; i < private_mib_base_struct.genGroup.genInstalledGen; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%s@",
                      private_mib_base_struct.genGroup.genTable[i].genStatusModbusID,
                      private_mib_base_struct.genGroup.genTable[i].genStatusType,
                      "Generator"
                        );
      }
    }
  }
  //slave gen setting---------------------------------------------------
  else if(strncmp(uri, "/gennum1i?value=", 7) == 0)
  {
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("gennum1i"));
    data = uri;
    j = data[sizeof("gennum1i")-2]-0x30;
    switch (j)
    {
    case 1:
    case 2:
      {
        if(strncmp(&data[sizeof("gennum1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.gen_manager_struct[j-1].gen_type = temp;
          switch(modbus_manager_struct.gen_manager_struct[j-1].gen_type)
          {
          case 1: // KUBOTA
            {
              modbus_manager_struct.gen_manager_struct[j-1].gen_slave_offset = 60;
              modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id = 60+j;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[0] = 0;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 235;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 85;    
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 1; 
            }
            break;
          case 2:// BE142
            {
              modbus_manager_struct.gen_manager_struct[j-1].gen_slave_offset = 60;
              modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id = 60+j;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[0] = 30010;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 30135;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[2] = 30079;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 38;    
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 4;     
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[2] = 27;
            }
            break;
          case 3:// DEEPSEA
            {
              modbus_manager_struct.gen_manager_struct[j-1].gen_slave_offset = 60;
              modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id = 60+j;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[0] = 0x400;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 0x700;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[2] = 0x800;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[3] = 0x304;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 34;    
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 8; 
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[2] = 8; 
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[3] = 1; 
            }
            break;
          case 4:// LR2057
            {
              modbus_manager_struct.gen_manager_struct[j-1].gen_slave_offset = 60;
              modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id = 60+j;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[0] = 6;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 19;  
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 42;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 13; 
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[2] = 12;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[2] = 66; 
              
            }
            break;
          case 5:// HIMOINSA
            {
              modbus_manager_struct.gen_manager_struct[j-1].gen_slave_offset = 86;
              modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id = 86+j;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[0] = 0;      // FC 0x02
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 23;  
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 7;      // FC 0x04
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 36; 
              
            }
            break;
          case 6:// QC315
            {
              modbus_manager_struct.gen_manager_struct[j-1].gen_slave_offset = 60;
              modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id = 60+j;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[0] = 20;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 708;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[2] = 800;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 61;  
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 6; 
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[2] = 7; 
            }
            break;
          case 7:// CUMMIN
            {
              modbus_manager_struct.gen_manager_struct[j-1].gen_slave_offset = 60;
              modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id = 60+j;
              modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[0] = 12;
              //                    modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 708;
              //                    modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[2] = 800;
              modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 60;  
              //                    modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 6; 
              //                    modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[2] = 7; 
            }
            break;
          };
          // Did parameters change?---------------------------------------------
          if  (parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor != modbus_manager_struct.gen_manager_struct[j-1].gen_type)
          {
            // Update the current parameters with the new settings--------------
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor = modbus_manager_struct.gen_manager_struct[j-1].gen_type;
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].slave_id = modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id;
            // Yes - save these settings as the defaults------------------------
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor;
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].slave_id;
            
            flag_struct.save_config_flag |=1;
          }                 
        }
      }
      break;
    default:
      break;
    };
    n = sprintf(sBuffHttp,"%d", (int)temp);
  }
  else if(strncmp(uri, "/genTotal?value=", 8) == 0)
  {
    uint32_t temp=0;
    //gen total-----------------------------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("genTotal"));
    if (temp != modbus_manager_struct.number_of_gen)
    {
      modbus_manager_struct.number_of_gen = temp;
      parameter.device_number[TOTAL_GEN_INDEX] = modbus_manager_struct.number_of_gen;
      working_default_parameter.device_number[TOTAL_GEN_INDEX] = parameter.device_number[TOTAL_GEN_INDEX];
      flag_struct.save_config_flag |=1;
    }
  }
  //PM==========================================================================
  //pm info-----------------------------------------------
  else if(!strcasecmp(uri, "/pmInfo_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
                private_mib_base_struct.pmGroup.pmTable[0].pmStatus,
                private_mib_base_struct.pmGroup.pmInstalledPM,
                private_mib_base_struct.pmGroup.pmActivePM,
                private_mib_base_struct.pmGroup.pmTable[0].pmType,
                &private_mib_base_struct.pmGroup.pmTable[0].pmSerial[0],
                private_mib_base_struct.pmGroup.pmTable[0].pmTotalCurrent,
                private_mib_base_struct.pmGroup.pmTable[0].pmFrequency,
                private_mib_base_struct.pmGroup.pmTable[0].pmPowerFactor,
                private_mib_base_struct.pmGroup.pmTable[0].pmActivePower,
                private_mib_base_struct.pmGroup.pmTable[0].pmReactivePower,
                private_mib_base_struct.pmGroup.pmTable[0].pmTotalActiveEnergy,
                private_mib_base_struct.pmGroup.pmTable[0].pmTotalReactiveEnergy,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase1Voltage,//1
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase1Current,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase1PowerFactor,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase1RealPower,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase1ReactivePower,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase2Voltage,//2
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase2Current,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase2PowerFactor,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase2RealPower,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase2ReactivePower,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase3Voltage,//3
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase3Current,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase3PowerFactor,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase3RealPower,
                private_mib_base_struct.pmGroup.pmTable[0].pmPhase3ReactivePower
                  );
  }
  else if(!strcasecmp(uri, "/powermeter_status"))
    {
        uint8_t i;
        n=0;
        for (i = 0; i < private_mib_base_struct.pmGroup.pmInstalledPM; i++)
        {
            if(n<HTTP_BUFF)
            {
                n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                              "%d;%d;%s@",
                              private_mib_base_struct.pmGroup.pmTable[i].pmModbusID,
                              private_mib_base_struct.pmGroup.pmTable[i].pmType,
                              "PowerMeter"
                             );
            }
        }
    }
  //pm setting----------------------------------------------
  else if(strncmp(uri, "/pmnum1i?value=", 6) == 0)
  {
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("pmnum1i"));
    data = uri;
    j = data[sizeof("pmnum1i")-2]-0x30;     
    
    switch (j)
    {
    case 1:
    case 2:
      {
        if(strncmp(&data[sizeof("pmnum1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.pm_manager_struct[j-1].pm_type = temp;
          switch(modbus_manager_struct.pm_manager_struct[j-1].pm_type)
          {
          case 1:// FINECO
            {
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 16;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1] = 50;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 0x700;    
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 2; 
            }
            break;
            
          case 2:// ASCENT
            {    
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 0;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1] = 100;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2] = 106;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[3] = 118;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[4] = 142;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[5] = 144;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 2;    
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 2;    
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2] = 2;   
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[3] = 2;   
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[4] = 2;   
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[5] = 2;                                 
            }
            break;
            
          case 3:// EASTRON
            {  
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 0;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 32;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1] = 70;//0x71
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 10;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2] = 342;//0x156
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2] = 4;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[3] = 64512;//0x156
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[3] = 2;
            }
            break;
            
          case 4:// CET1
            {  
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 52;
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 52+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 0;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1] = 40;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2] = 9800;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 14;    
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 18;    
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2] = 27;                                  
            }
            break;
            
          case 5:// PILOT
            {  
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;  
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 0;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 17;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1] = 19;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 6;                                  
            }
            break;
            //                case 6:// FORLONG
            //                {  
            //                    modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;  
            //                    modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
            //                    modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 0;//0x03
            //                    modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 20;
            //                }
            //                break;
          case 15:// PILOT_3PHASE
            {  
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;  
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 100;//0x03
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 43;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1] = 1000;//0x03
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 12;  
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2] = 9000;//0x03
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2] = 2; 
            }
            break;
          case 16://YADA_3PHASE_DPC
            {
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset  = 16;
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id   = 16+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0]  = 4240;//0x03 U
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0]   = 6;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1]  = 4288;//0x03 I
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1]   = 6;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2]  = 4496;//0x03 cosphi
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2]   = 4;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[3]  = 4528;//0x03 HZ
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[3]   = 1;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[4]  = 4192;//0x03 kvarh
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[4]   = 2;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[5]  = 4144;//0x03 kwh
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[5]   = 2;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[6]  = 4352;//0x03 P
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[6]   = 8;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[7]  = 4416;//0x03 Q
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[7]   = 8;
            }
            break;
          case 17: //Schneider 2022
            {
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset  = 16;
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id  = 16+j;
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0]  = 29;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 20;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1]  = 128;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 3;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2]  = 2699;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2] = 24;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[3]  = 2999;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[3] = 6;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[4]  = 3027;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[4]   = 6;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[5]  = 3053;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[5]   = 24;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[6]  = 3077;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[6]   = 8;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[7]  = 3109;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[7]   = 2;
            }
            
            break;
          case 18://EASTRON SMD72D 2022
            {
              modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset  = 16;
              modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id  = 16+j;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0]  = 0;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 30;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1]  = 30;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 30;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2]  = 60;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2] = 30;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[3]  = 342;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[3] = 30;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[4]  = 10;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[4]   = 30;
              
              modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[5]  = 64512;
              modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[5]   = 4;
            }
            break;
          };            
          // Did parameters change?--------------------------------------------
          if  (parameter.modbus_parameter[j-1+ MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor != modbus_manager_struct.pm_manager_struct[j-1].pm_type)
          {
            // Update the current parameters with the new settings.-------------
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor = modbus_manager_struct.pm_manager_struct[j-1].pm_type;
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].slave_id = modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id;
            // Yes - save these settings as the defaults.-----------------------
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor;
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].slave_id;
            
            flag_struct.save_config_flag |=1;
          }                 
        }
      }
      break;
    default:
      break;
    };

  }
  else if(strncmp(uri, "/pmTotal?value=", 7) == 0)
  {
    uint32_t temp=0;
    //pm total------------------------------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("pmTotal"));
    if (temp != modbus_manager_struct.number_of_pm)
    {
      modbus_manager_struct.number_of_pm = temp;
      parameter.device_number[TOTAL_PM_INDEX] = modbus_manager_struct.number_of_pm;
      working_default_parameter.device_number[TOTAL_PM_INDEX] = parameter.device_number[TOTAL_PM_INDEX];
      flag_struct.save_config_flag |=1;
    }
  }
  //SMBC======================================================================
  //smbc status---------------------------------------------
  else if(!strcasecmp(uri, "/smcb_status"))
  {
    uint8_t i;
    n=0;
    for (i = 0; i < private_mib_base_struct.smcbGroup.SmcbInstalledSMCB; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%d;%d@",
                      private_mib_base_struct.smcbGroup.SmcbTable[i].SmcbModbusID,
                      private_mib_base_struct.smcbGroup.SmcbTable[i].SmcbType,
                      private_mib_base_struct.smcbGroup.SmcbTable[i].SmcbStatus,
                      private_mib_base_struct.smcbGroup.SmcbTable[i].SmcbState
                        );
      }
    }
  }
  //smcb setting--------------------------------------------
  else if(strncmp(uri, "/smcbnum1v?value=", 8) == 0)
  {
    uint32_t j,temp = 0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("smcbnum1v"));
    data = uri;
    j = data[sizeof("smcbnum1v")-2]-0x30;
    switch (j)
    {
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
      {
        if(strncmp(&data[sizeof("smcbnum1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.smcb_manager_struct[j-1].smcb_type = temp;
          switch(modbus_manager_struct.smcb_manager_struct[j-1].smcb_type)
          {
          case 1:// OPEN
            {
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_slave_offset = 18;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_abs_slave_id = 18+j;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_base_addr[0] = 0;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_num_of_reg[0] = 1;    
            }
            break;
            
          case 2:// MATIS
            {
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_slave_offset = 18;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_abs_slave_id = 18+j;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_base_addr[0] = 16;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_num_of_reg[0] = 1;    
            }
            break;
          case 3:// GOL
            {
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_slave_offset = 18;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_abs_slave_id = 18+j;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_base_addr[0] = 32768;
              modbus_manager_struct.smcb_manager_struct[j-1].smcb_num_of_reg[0] = 1;   
            }
            break;
          
          // Did parameters change?---------------------------------------------
          if  (parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor != modbus_manager_struct.smcb_manager_struct[j-1].smcb_type)
          {
            // Update the current parameters with the new settings--------------
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor = modbus_manager_struct.smcb_manager_struct[j-1].smcb_type;
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].slave_id = modbus_manager_struct.smcb_manager_struct[j-1].smcb_abs_slave_id;
            // Yes - save these settings as the defaults------------------------
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor;
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].slave_id;
            
            flag_struct.save_config_flag |=1;
          }                 
        }
      }
      break;
    default:
      break;
    };
    
  }
  }
  else if(strncmp(uri, "/smcbTotal?value=", 9) == 0)
  {
    uint32_t temp=0;
    // smcb Total----------------------------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("smcbTotal"));
    if (temp != modbus_manager_struct.number_of_smcb)
    {
      modbus_manager_struct.number_of_smcb = temp;
      parameter.device_number[TOTAL_SMCB_INDEX] = modbus_manager_struct.number_of_smcb;
      working_default_parameter.device_number[TOTAL_SMCB_INDEX] = parameter.device_number[TOTAL_SMCB_INDEX];
      flag_struct.save_config_flag |=1;
    }
    
  }
  else if(strncmp(uri, "/smcbState1v?value=", 9) == 0)
  {
    // SMCB State--------------------------------------------------------------
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("smcbState1v"));
    data = uri;
    j = data[sizeof("smcbState1v")-2]-0x30;
    private_mib_base_struct.smcbGroup.SmcbTable[j-1].SmcbState = temp;
    private_mib_base_struct.smcbGroup.SmcbTable[j-1].SmcbStateWrite = temp;
    modbus_manager_struct.smcb_info_struct[j-1].u32State = private_mib_base_struct.smcbGroup.SmcbTable[j-1].SmcbState;
    private_mib_base_struct.smcbGroup.flag = j-1;
    modbus_manager_struct.setting_cmd = SET_SMCB_STATE;
    
  }
  //FUEL=======================================================================
  //fuel status---------------------------------------------------------
  else if(!strcasecmp(uri, "/fuelinfo_status"))
  {
    uint8_t i;
    n=0;
    for (i = 0; i < 2; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%d@",
                      i + 1,
                      private_mib_base_struct.fuelGroup.FuelTable[i].FuelStatus,
                      private_mib_base_struct.fuelGroup.FuelTable[i].FuelLevel
                        );
      }
    }
  }
  else if(!strcasecmp(uri, "/fuel_status"))
  {
    uint8_t i;
    n=0;
    for (i = 0; i < private_mib_base_struct.fuelGroup.FuelInstalledFUEL; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%s@",
                      private_mib_base_struct.fuelGroup.FuelTable[i].FuelModbusID,
                      private_mib_base_struct.fuelGroup.FuelTable[i].FuelType,
                      "FUEL"
                        );
      }
    }
  }
  //fuel setting--------------------------------------------------------
  else if(strncmp(uri, "/fuelnum1v?value=", 8) == 0)
  {
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("fuelnum1v"));
    data = uri;
    j = data[sizeof("fuelnum1v")-2]-0x30;
    switch (j)
    {
    case 1:
    case 2:
      {
        if(strncmp(&data[sizeof("fuelnum1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.fuel_manager_struct[j-1].fuel_type = temp;
          switch(modbus_manager_struct.fuel_manager_struct[j-1].fuel_type)
          {
          case 1:// HPT621
            {
              modbus_manager_struct.fuel_manager_struct[j-1].fuel_slave_offset = 23;
              modbus_manager_struct.fuel_manager_struct[j-1].fuel_abs_slave_id = 23+j;
              modbus_manager_struct.fuel_manager_struct[j-1].fuel_base_addr[0] = 0;
              modbus_manager_struct.fuel_manager_struct[j-1].fuel_num_of_reg[0] = 1;    
            }
            break;
          };
          // Did parameters change----------------------------------------------
          if  (parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor != modbus_manager_struct.fuel_manager_struct[j-1].fuel_type)
          {
            // Update the current parameters with the new setting---------------
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor = modbus_manager_struct.fuel_manager_struct[j-1].fuel_type;
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].slave_id = modbus_manager_struct.fuel_manager_struct[j-1].fuel_abs_slave_id;
            // Yes - save these settings as the defaults------------------------
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor;
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].slave_id;
            
            flag_struct.save_config_flag |=1;
          }                 
        }
      }
      break;
    default:
      break;
    };
    
  }
  else if(strncmp(uri, "/fuelTotal?value=", 9) == 0)
  {
    uint32_t temp=0;
    // fuel Total---------------------------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("fuelTotal"));
    if (temp != modbus_manager_struct.number_of_fuel)
    {
      modbus_manager_struct.number_of_fuel = temp;
      parameter.device_number[TOTAL_FUEL_INDEX] = modbus_manager_struct.number_of_fuel;
      working_default_parameter.device_number[TOTAL_FUEL_INDEX] = parameter.device_number[TOTAL_FUEL_INDEX];
      flag_struct.save_config_flag |=1;
    }
    
  }
  //ISENSE=====================================================================
  
  //isense status-----------------------------------------------------------
  else if(!strcasecmp(uri, "/i_senseInfo_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%s;%s;%d;%d",
                private_mib_base_struct.isenseGroup.isenseTable.isenseStatus,
                private_mib_base_struct.isenseGroup.isenseInstalledISENSE,
                private_mib_base_struct.isenseGroup.isenseActiveISENSE,
                &private_mib_base_struct.isenseGroup.isenseTable.isenseModel[0],
                &private_mib_base_struct.isenseGroup.isenseTable.isenseSerial[0],
                private_mib_base_struct.isenseGroup.isenseTable.isenseVoltage,
                private_mib_base_struct.isenseGroup.isenseTable.isenseFrequency   
                  );
  }
  else if(!strcasecmp(uri, "/i_sense_status"))
  {
    n=0;
    if(private_mib_base_struct.isenseGroup.isenseInstalledISENSE){
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%s@",
                      private_mib_base_struct.isenseGroup.isenseTable.isenseModbusID,
                      private_mib_base_struct.isenseGroup.isenseTable.isenseType,
                      "PowerMeter"
                        );
      }
    }
  }
  
  //isense setting----------------------------------------------------------
  
  else if(strncmp(uri, "/i_sensenum1i?value=", 11) == 0)
  {
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("i_sensenum1i"));
    data = uri;
    j = data[sizeof("i_sensenum1i")-2]-0x30;     
    
    switch (j)
    {
    case 1:
      {
        if(strncmp(&data[sizeof("i_sensenum1i")-1],"v",1) == 0)
        {
          modbus_manager_struct.isense_manager_struct[j-1].isense_type = temp;
          switch(modbus_manager_struct.isense_manager_struct[j-1].isense_type)
          {
          case 1:// FORLONG
            {  
              modbus_manager_struct.isense_manager_struct[j-1].isense_slave_offset = 26;  
              modbus_manager_struct.isense_manager_struct[j-1].isense_abs_slave_id = 26+j;
              modbus_manager_struct.isense_manager_struct[j-1].isense_base_addr[0] = 0;//0x04
              modbus_manager_struct.isense_manager_struct[j-1].isense_num_of_reg[0] = 22;
            }
            break;
          };
          // Did parameters change?---------------------------------------------
          if  (parameter.modbus_parameter[j-1+ MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor != modbus_manager_struct.isense_manager_struct[j-1].isense_type)
          {
            // Update the current parameters with the new settings--------------
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor = modbus_manager_struct.isense_manager_struct[j-1].isense_type;
            parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].slave_id = modbus_manager_struct.isense_manager_struct[j-1].isense_abs_slave_id;
            // Yes - save these settings as the defaults------------------------
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor;
            working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].slave_id;
            
            flag_struct.save_config_flag |=1;
          }                 
        }
      }
      break;
    default:
      break;
    };
    
  }
  else if(strncmp(uri, "/i_senseTotal?value=", 11) == 0)
  {
    uint32_t temp=0;
    // i sense total------------------------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("i_senseTotal"));
    if (temp != modbus_manager_struct.number_of_isense)
    {
      modbus_manager_struct.number_of_isense = temp;
      parameter.device_number[TOTAL_ISENSE_INDEX] = modbus_manager_struct.number_of_isense;
      working_default_parameter.device_number[TOTAL_ISENSE_INDEX] = parameter.device_number[TOTAL_ISENSE_INDEX];
      flag_struct.save_config_flag |=1;
    }
  }
  //PMDC========================================================================
  //pmdc status---------------------------------------------------------------
  else if(!strcasecmp(uri, "/pm_dcInfo_status")){
    n = sprintf(sBuffHttp,"%d;%d;%d;%s;%d;%d;%d;%d",
                private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_status,          //0
                private_mib_base_struct.pm_dc_group.pm_dc_installed_PM_DC,             //1
                private_mib_base_struct.pm_dc_group.pm_dc_active_PM_DC,                //2
                &private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_model_[0],      //3
                private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_voltage,         //4
                private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_current,         //5
                private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_active_power,    //6
                private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_active_energy    //7
                  );
  }
  else if(!strcasecmp(uri, "/pm_dc_status")){
    n=0;
    if(private_mib_base_struct.pm_dc_group.pm_dc_installed_PM_DC){
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%d;%s@",
                      private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_modbus_ID,
                      private_mib_base_struct.pm_dc_group.pm_dc_table.pm_dc_type,
                      "PowerMeter"
                        );
      }
    }
  }
  
  
  //pmdc setting--------------------------------------------------------------
  else if(strncmp(uri, "/pm_dcnum1i?value=", 9) == 0){
    uint32_t j,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("pm_dcnum1i"));
    data = uri;
    j = data[sizeof("pm_dcnum1i")-2]-0x30;
    
    switch(j){
    case 1:
      if(strncmp(&data[sizeof("pm_dcnum1i")-1],"v",1) == 0){
        modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_type = temp;
        switch(modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_type){
        case 1:// YADA_DC
          {
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_slave_offset    = 30;
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_abs_slave_id     = 30 +j;
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_base_addr[0]    = 0;       //0x03
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_num_of_reg[0]    = 2;
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_base_addr[1]    = 12;       //0x03
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_num_of_reg[1]    = 2;
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_base_addr[2]    = 26;       //0x03
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_num_of_reg[2]    = 2;
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_base_addr[3]    = 40;       //0x03
            modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_num_of_reg[3]    = 2;
            
          }
          break;
        default:
          break; 
        }

        // Did parameters change?-----------------------------------------------
        if  (parameter.modbus_parameter[j-1+ MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor != modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_type)
        {
          // Update the current parameters with the new settings.---------------
          parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor = modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_type;
          parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].slave_id = modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_abs_slave_id;
          // Yes - save these settings as the defaults.-------------------------
          working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor;
          working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].slave_id;
          
          flag_struct.save_config_flag |=1;
        }
      }
      break;
    default:
      break;
    }

  }
  else if(strncmp(uri, "/pm_dcTotal?value=", 10) == 0){
    uint32_t temp=0;
    //pm dc total---------------------------------------------------------------
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("pm_dcTotal"));
    if (temp != modbus_manager_struct.number_of_pmdc)
    {
      modbus_manager_struct.number_of_pmdc = temp;
      parameter.device_number[TOTAL_PMDC_INDEX] = modbus_manager_struct.number_of_pmdc;
      working_default_parameter.device_number[TOTAL_PMDC_INDEX] = parameter.device_number[TOTAL_PMDC_INDEX];
      flag_struct.save_config_flag |=1;
    }
  }
//system status================================================================================================
  else if(!strcasecmp(uri, "/battconfig_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%s;%s;%s;%s;%d;%d;%s;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
                private_mib_base_struct.configGroup.cfgCurrentLimit,//0
                private_mib_base_struct.configGroup.cfgFloatVolt,//1
                private_mib_base_struct.configGroup.cfgBoostVolt,//2
                private_mib_base_struct.configGroup.cfgDCLowVolt,//3
                private_mib_base_struct.configGroup.cfgLLVDVolt,//4
                private_mib_base_struct.configGroup.cfgBLVDVolt,//5
                private_mib_base_struct.configGroup.cfgTempCompValue,//6
                private_mib_base_struct.configGroup.cfgBattCapacityTotal,//7
                private_mib_base_struct.configGroup.cfgHighMajorTempLevel,//8
                private_mib_base_struct.configGroup.cfgWalkInTimeEn,//9
                private_mib_base_struct.configGroup.cfgWalkInTimeDuration,//10
                private_mib_base_struct.configGroup.cfgAcLowLevel,//11
                private_mib_base_struct.configGroup.cfgAcUnderLevel,//12
                private_mib_base_struct.configGroup.cfgAcHighLevel,//13
                private_mib_base_struct.configGroup.cfgOverMajorTempLevel,//14
                private_mib_base_struct.configGroup.cfgBattTestVolt,//15
                private_mib_base_struct.configGroup.cfgBattCapacityTotal2,//16
                private_mib_base_struct.cfgBTGroup.cfgBTEndVolt,//17
                private_mib_base_struct.cfgBTGroup.cfgBTEndCap,//18
                private_mib_base_struct.cfgBTGroup.cfgBTEndTime,//19
                private_mib_base_struct.cfgBTGroup.cfgBTPlanTestEn,//20
                0,//21
                private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestString,//22
                private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestString,//23
                private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestString,//24
                private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestString,//25
                private_mib_base_struct.configGroup.cfgDCUnderVolt,//26
                private_mib_base_struct.configGroup.cfgDCOverVolt,//27
                private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUString,//28
                private_mib_base_struct.configGroup.cfgLowTempLevel,//29
                private_mib_base_struct.cfgBTGroup.cfgTestVoltCfg,//30
                private_mib_base_struct.cfgBTGroup.cfgAutoTestDay,//31
                private_mib_base_struct.cfgBTGroup.cfgTestStartTime,//32
                private_mib_base_struct.siteGroup.siteControllerModel[0],//33
                private_mib_base_struct.configGroup.cfgLLVDEn,//34
                private_mib_base_struct.configGroup.cfgBLVDEn,//35
                private_mib_base_struct.configGroup.cfgBattCapacityTotal3,//36 thanhcm3 add for dpc 
                private_mib_base_struct.configGroup.cfgBattCapacityTotal4, //37 thanhcm3 add for dpc
                private_mib_base_struct.fan_dpc_info.mib.para_dc_starting_point,//38 thanhcm3 add for dpc
                private_mib_base_struct.fan_dpc_info.mib.para_dc_sensivive_point,//39 thanhcm3 add for dpc
                private_mib_base_struct.fan_dpc_info.mib.para_alarm_high_temp,//40 thanhcm3 add for dpc
                private_mib_base_struct.fan_dpc_info.mib.para_alarm_low_temp, //41 thanhcm3 add for dpc
                private_mib_base_struct.configGroup.cfgCurrentlimitA,      //42 thanhcm3 add for dpc
                private_mib_base_struct.fan_dpc_info.mib.para_dc_heating_start_point,//43 thanhcm3 add for dpc
                private_mib_base_struct.fan_dpc_info.mib.para_dc_heating_sensivive    //44 thanhcm3 add for dpc
                  );
  }
  //rect status---------------------------------------------
  else if(!strcasecmp(uri, "/rect_status"))
  {
    uint8_t i;
    n=0;
    for (i = 0; i < private_mib_base_struct.rectGroup.rectInstalledRect; i++)
    {
      if(n<HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%s;%d;%d;%d@",
                      i + 1,
                      private_mib_base_struct.rectGroup.rectTable[i].rectSerialNo,
                      private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage,
                      private_mib_base_struct.rectGroup.rectTable[i].rectOutputCurrent,
                      private_mib_base_struct.rectGroup.rectTable[i].rectTemp
                      );
      }
    }
  }
  //ac status-----------------------------------------------
  else if(!strcasecmp(uri, "/ac_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d",
                private_mib_base_struct.acPhaseGroup.acPhaseTable[0].acPhaseVolt,
                private_mib_base_struct.acPhaseGroup.acPhaseTable[1].acPhaseVolt,
                private_mib_base_struct.acPhaseGroup.acPhaseTable[2].acPhaseVolt
                );
  }
  //load status---------------------------------------------
  else if(!strcasecmp(uri, "/load_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
                private_mib_base_struct.loadGroup.loadCurrent,
                private_mib_base_struct.loadGroup.loadStatus[0],
                private_mib_base_struct.loadGroup.loadStatus[1],
                private_mib_base_struct.loadGroup.loadStatus[2],
                private_mib_base_struct.loadGroup.loadStatus[3],
                private_mib_base_struct.loadGroup.loadStatus[4],
                private_mib_base_struct.loadGroup.loadStatus[5],
                private_mib_base_struct.loadGroup.loadStatus[6],
                private_mib_base_struct.loadGroup.loadStatus[7],
                private_mib_base_struct.loadGroup.loadStatus[8],
                private_mib_base_struct.loadGroup.loadStatus[9]
                );
  }
  // batt status----------------------------------------------
  else if(!strcasecmp(uri, "/batt_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
                private_mib_base_struct.batteryGroup.battVolt,
                private_mib_base_struct.batteryGroup.battCurr,
                private_mib_base_struct.batteryGroup.battTemp,
                private_mib_base_struct.batteryGroup.battBanksNumofBanks,
                private_mib_base_struct.batteryGroup.battBrkStatus[0],
                private_mib_base_struct.batteryGroup.battBrkStatus[1],
                private_mib_base_struct.batteryGroup.battBrkStatus[2],
                private_mib_base_struct.batteryGroup.battBrkStatus[3],
                private_mib_base_struct.batteryGroup.battCapLeft1,
                private_mib_base_struct.batteryGroup.battCapLeft2,
                private_mib_base_struct.batteryGroup.battCapLeft3,
                private_mib_base_struct.batteryGroup.battCapLeft4
                );
  }
  //device ip status-----------------------------------------
  else if(!strcasecmp(uri, "/deviceip_status"))
  {
    
    n = sprintf(sBuffHttp,"%s;%s;%s;%s;%d;%d;%s;%d",
                private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucIP,
                private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucGW,
                private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSN,
                private_mib_base_struct.siteGroup.siteBTSCode,
                private_mib_base_struct.siteGroup.siteControllerModel[0],
                private_mib_base_struct.siteGroup.siteControllerModel[1],
                private_mib_base_struct.cfgNetworkGroup.cFTPServer,
                private_mib_base_struct.cfgNetworkGroup.ftp_port
                  
                );
  }
  //firmware update------------------------------------------
  else if(!strcasecmp(uri, "/fwupdate_status")){
    n = sprintf(sBuffHttp,"%d",
                config_handler_struct.state_update
               );
  
  }
  else if(strncmp(uri, "/fwUpdateDev?value=", 12) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&config_handler_struct.state_update,sizeof("fwUpdateDev"));
  }
  else if(strncmp(uri, "/update", 7) == 0)
    {
      if(config_handler_struct.u8FwUpdateFlag == 0)
      {
        //save Vac firm version--------------------------------------------------------------------
        if((config_handler_struct.state_update==_VAC_FW_)||(config_handler_struct.state_update==_VAC_V2_FW_))
        {
          config_handler_struct.Check_vac_firm_version = private_mib_base_struct.vac_group_struct.vac_table_struct[0].vac_firm_version;
        }else
        {
          config_handler_struct.Check_vac_firm_version=0;
        }
        //flag update-------------------------------------------------------------------------------
        if(config_handler_struct.state_update==_NO_FW_UPDATE){
        }
        else{
          config_handler_struct.u8FwUpdateFlag=1;
        }
        //------------------------------------------------------------------------------------------
        
      }
    }
//ip address setup==============================================================
  
  //config ip static------------------------------------------------------------
  else if(strncmp(uri, "/devsip1?value=", 7) == 0)//(!strcasecmp(request, "/devsip"))
  {
    uint32_t i,temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("devsip1"));
    ucdata = uri;
    switch (ucdata[sizeof("devsip1")-1]-0x30)
    {
    case 1:
      {
        sIpConfig.ip_parameter.static_ip = (sIpConfig.ip_parameter.static_ip &  0x00ffffff) | (temp << 24);
        bConfirmed[0] = 1;
      }
      break;
    case 2:
      {
        sIpConfig.ip_parameter.static_ip = (sIpConfig.ip_parameter.static_ip &  0xff00ffff) | (temp << 16);
        bConfirmed[1] = 1;
      }
      break;
    case 3:
      {
        sIpConfig.ip_parameter.static_ip = (sIpConfig.ip_parameter.static_ip &  0xffff00ff) | (temp << 8);
        bConfirmed[2] = 1;
      }
      break;
    case 4:
      {
        sIpConfig.ip_parameter.static_ip = (sIpConfig.ip_parameter.static_ip &  0xffffff00) | (temp << 0);
        bConfirmed[3] = 1;
      }
      break;
      
    case 5:
      {
        sIpConfig.ip_parameter.gateway_ip = (sIpConfig.ip_parameter.gateway_ip &  0x00ffffff) | (temp << 24);
        bConfirmed[4] = 1;
      }
      break;
    case 6:
      {
        sIpConfig.ip_parameter.gateway_ip = (sIpConfig.ip_parameter.gateway_ip &  0xff00ffff) | (temp << 16);
        bConfirmed[5] = 1;
      }
      break;
    case 7:
      {
        sIpConfig.ip_parameter.gateway_ip = (sIpConfig.ip_parameter.gateway_ip &  0xffff00ff) | (temp << 8);
        bConfirmed[6] = 1;
      }
      break;
    case 8:
      {
        sIpConfig.ip_parameter.gateway_ip = (sIpConfig.ip_parameter.gateway_ip &  0xffffff00) | (temp << 0);
        bConfirmed[7] = 1;
      }
      break;
      
    case 9:
      {
        sIpConfig.ip_parameter.subnet_mask = (sIpConfig.ip_parameter.subnet_mask &  0x00ffffff) | (temp << 24);
        bConfirmed[8] = 1;
      }
      break;
    case 17:
      {
        sIpConfig.ip_parameter.subnet_mask = (sIpConfig.ip_parameter.subnet_mask &  0xff00ffff) | (temp << 16);
        bConfirmed[9] = 1;
      }
      break;
    case 18:
      {
        sIpConfig.ip_parameter.subnet_mask = (sIpConfig.ip_parameter.subnet_mask &  0xffff00ff) | (temp << 8);
        bConfirmed[10] = 1;
      }
      break;
    case 19:
      {
        sIpConfig.ip_parameter.subnet_mask = (sIpConfig.ip_parameter.subnet_mask &  0xffffff00) | (temp << 0);
        bConfirmed[11] = 1;
      }
      break;
    default:
      break;
    };
    
    if ((bConfirmed[0] == 1)&&(bConfirmed[1] == 1)&&(bConfirmed[2] == 1)&&(bConfirmed[3] == 1)&&
        (bConfirmed[4] == 1)&&(bConfirmed[5] == 1)&&(bConfirmed[6] == 1)&&(bConfirmed[7] == 1)&&
          (bConfirmed[8] == 1)&&(bConfirmed[9] == 1)&&(bConfirmed[10] == 1)&&(bConfirmed[11] == 1))
    {
      for (i = 0; i < 12; i++)
      {
        bConfirmed[i] = 0;
      }
      // Did any of the telnet parameters change?-------------------------------
      if  (parameter.ip_parameter.static_ip != sIpConfig.ip_parameter.static_ip)
      {
        // Update the current parameters with the new settings.-----------------
        parameter.ip_parameter.static_ip = sIpConfig.ip_parameter.static_ip;
      }
      // Did any of the telnet parameters change?-------------------------------
      if  (parameter.ip_parameter.gateway_ip != sIpConfig.ip_parameter.gateway_ip)
      {
        // Update the current parameters with the new settings.-----------------
        parameter.ip_parameter.gateway_ip = sIpConfig.ip_parameter.gateway_ip;
      }
      // Did any of the telnet parameters change?-------------------------------
      if  (parameter.ip_parameter.subnet_mask != sIpConfig.ip_parameter.subnet_mask)
      {
        // Update the current parameters with the new settings.-----------------
        parameter.ip_parameter.subnet_mask = sIpConfig.ip_parameter.subnet_mask;
      }
      // Yes - save these settings as the defaults.-----------------------------
      working_default_parameter.ip_parameter.static_ip = parameter.ip_parameter.static_ip;
      working_default_parameter.ip_parameter.gateway_ip = parameter.ip_parameter.gateway_ip;
      working_default_parameter.ip_parameter.subnet_mask = parameter.ip_parameter.subnet_mask;
      flag_struct.save_config_flag |=1;
      system_flag_struct.u8IsRebootFlag = 1;
    };
  }
  //config bts id --------------------------------------------------------------
  else if(strncmp(uri, "/btsid?value=", 6) == 0)
  {
    
    const char *pucBTSName;
    pucBTSName = uri;
    pucBTSName += (sizeof("btsid") + 7);
    //
    // Charge BTS Name
    //
    private_mib_base_struct.siteGroup.siteBTSCodeLen = htmlDecodeFormString(pucBTSName,(char *)&private_mib_base_struct.siteGroup.siteBTSCode[0],40);
    
    strncpy((char*)&working_default_parameter.ip_parameter.site_name[0],(char*)&private_mib_base_struct.siteGroup.siteBTSCode[0],private_mib_base_struct.siteGroup.siteBTSCodeLen);
    strncpy((char*)&parameter.ip_parameter.site_name[0],(char*)&working_default_parameter.ip_parameter.site_name[0],private_mib_base_struct.siteGroup.siteBTSCodeLen);
    private_mib_base_struct.siteGroup.siteBTSCode[private_mib_base_struct.siteGroup.siteBTSCodeLen] ='\0';
    parameter.ip_parameter.site_name[private_mib_base_struct.siteGroup.siteBTSCodeLen] ='\0';
    working_default_parameter.ip_parameter.site_name[private_mib_base_struct.siteGroup.siteBTSCodeLen] ='\0';
    
    flag_struct.save_config_flag |=1;
  }
  //config cabinet 1 type------------------------------------------------------
  else if(strncmp(uri, "/cabCtrl1Type?value=", 8) == 0)
  {
    //
    // Battery End Time
    //
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.siteGroup.siteControllerModel[0],sizeof("cabCtrl1Type"));
    if (parameter.u8ControllerType != private_mib_base_struct.siteGroup.siteControllerModel[0])
    {
      //0: Emerson
      //1: ZTE
      //2: Agisson
      //3: Emerson_v21
      //4: DKD51_BDP
      parameter.u8ControllerType = private_mib_base_struct.siteGroup.siteControllerModel[0];
      working_default_parameter.u8ControllerType = parameter.u8ControllerType;  
      flag_struct.save_config_flag |= 1;  
      system_flag_struct.u8IsRebootFlag = 1;          
    }
  }
  //config ftp sever name-------------------------------------------------------
  else if(strncmp(uri, "/ftpname?value=", 6) == 0)
  {
    
    const char *pucFTPServer;
    pucFTPServer = uri;
    pucFTPServer += (sizeof("ftpname") + 7);
    // Charge BTS Name
    private_mib_base_struct.cfgNetworkGroup.cFTPServerLen = htmlDecodeFormString(pucFTPServer,(char *)&private_mib_base_struct.cfgNetworkGroup.cFTPServer[0],40);
    
    strncpy((char*)&working_default_parameter.ip_parameter.uc_ftp_server[0],(char*)&private_mib_base_struct.cfgNetworkGroup.cFTPServer[0],private_mib_base_struct.cfgNetworkGroup.cFTPServerLen);
    strncpy((char*)&parameter.ip_parameter.uc_ftp_server[0],(char*)&working_default_parameter.ip_parameter.uc_ftp_server[0],private_mib_base_struct.cfgNetworkGroup.cFTPServerLen);
    private_mib_base_struct.cfgNetworkGroup.cFTPServer[private_mib_base_struct.cfgNetworkGroup.cFTPServerLen] ='\0';
    parameter.ip_parameter.uc_ftp_server[private_mib_base_struct.cfgNetworkGroup.cFTPServerLen] ='\0';
    working_default_parameter.ip_parameter.uc_ftp_server[private_mib_base_struct.cfgNetworkGroup.cFTPServerLen] ='\0';
    
    flag_struct.save_config_flag |= 1; 
  }
  else if(strncmp(uri, "/ftp_port?value=", 8) == 0)    //"/battTempComp?value=", 9) == 0
  {
    
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgNetworkGroup.ftp_port,sizeof("ftp_port"));
    if(private_mib_base_struct.cfgNetworkGroup.ftp_port != parameter.ip_parameter.ftp_port){
      parameter.ip_parameter.ftp_port                     = private_mib_base_struct.cfgNetworkGroup.ftp_port;
      working_default_parameter.ip_parameter.ftp_port     = private_mib_base_struct.cfgNetworkGroup.ftp_port;
      flag_struct.save_config_flag |= 1;
    }
    
    
  }
//config factory reset==========================================================
  else if(strncmp(uri, "/restore", 7) == 0)
  {
    ConfigLoadFactory(); 
    flag_struct.save_config_flag |= 1;
    system_flag_struct.u8IsRebootFlag = 1;
    system_flag_struct.u8IsRewriteSN = 1;
  }
//config update=================================================================
  //code here
//config reboot=================================================================
  else if(strncmp(uri, "/reboot", 7) == 0)
  {
    system_flag_struct.u8IsRebootFlag = 1;
  }
//config power m500d============================================================
  //Rect Walk-In-Time Enable:---------------------------------------------------
  else if(strncmp(uri, "/rectWITE?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgWalkInTimeEn,sizeof("rectWITE"));
    setCmd_flag = 1;
    setCmd_mask = SET_WIT_EN;
  }
  //Rect Walk-In-Time Interval:-------------------------------------------------
  else if(strncmp(uri, "/rectWITI?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgWalkInTimeDuration,sizeof("rectWITI"));
    setCmd_flag = 1;
    setCmd_mask = SET_WIT_VAL;
  }
  //Battery Charge Current Limit:-----------------------------------------------
  else if(strncmp(uri, "/battCCL?value=", 8) == 0)
  {
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] != 4)
    {
      text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgCurrentLimit,sizeof("battCCL"));
      setCmd_flag = 1;
      setCmd_mask = SET_CCL;
    }
    
  }
  //Battery Float Voltage:------------------------------------------------------
  else if(strncmp(uri, "/battFV?value=", 7) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgFloatVolt,sizeof("battFV"));
    setCmd_flag = 1;
    setCmd_mask = SET_FLTVOL;
  }
  
  //Battery Boost Voltage:------------------------------------------------------
  else if(strncmp(uri, "/battBV?value=", 7) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgBoostVolt,sizeof("battBV"));
    setCmd_flag = 1;
    setCmd_mask = SET_BSTVOL;
  }
  //Battery DC Low Voltage:-----------------------------------------------------
  else if(strncmp(uri, "/battDCLow?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgDCLowVolt,sizeof("battDCLow"));
    setCmd_flag = 1;
    setCmd_mask = SET_DCLOW;
  }
  
  //Battery DC Under Voltage:---------------------------------------------------
  else if(strncmp(uri, "/battDCUnder?value=", 12) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgDCUnderVolt,sizeof("battDCUnder"));
    setCmd_flag = 1;
    setCmd_mask = SET_DCUNDER;
  }
  //Battery LLVD Enable:--------------------------------------------------------
  else if(strncmp(uri, "/battEnLLVD?value=", 10) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgLLVDEn,sizeof("battEnLLVD"));
    setCmd_flag = 1;
    setCmd_mask = SET_LLVD_EN;
  }
  
  //Battery BLVD Enable:--------------------------------------------------------
  else if(strncmp(uri, "/battEnBLVD?value=", 10) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgBLVDEn,sizeof("battEnBLVD"));
    setCmd_flag = 1;
    setCmd_mask = SET_BLVD_EN;
  }
  //Battery LLVD Voltage--------------------------------------------------------
  else if(strncmp(uri, "/battLLVD?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgLLVDVolt,sizeof("battLLVD"));
    setCmd_flag = 1;
    setCmd_mask = SET_LLVD;
  }
  
  //Battery BLVD Voltage:-------------------------------------------------------
  else if(strncmp(uri, "/battBLVD?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgBLVDVolt,sizeof("battBLVD"));
    setCmd_flag = 1;
    setCmd_mask = SET_BLVD;
  }
  //Battery Temperature Compensation:-------------------------------------------
  else if(strncmp(uri, "/battTempComp?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgTempCompValue,sizeof("battTempComp"));
    setCmd_flag = 1;
    setCmd_mask = SET_TEMPCOMP_VAL;
  }
  //Battery Capacity:-----------------------------------------------------------
  else if(strncmp(uri, "/battCap1?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgBattCapacityTotal,sizeof("battCap1"));
    setCmd_flag = 1;
    setCmd_mask = SET_BATTSTDCAP;
  }
  //Battery High Temperature Level:---------------------------------------------
  else if(strncmp(uri, "/battHiTemp?value=", 9) == 0)
  {
    text_sBuffHttp_len = htmlTextExtract2(uri,(int32_t*)&private_mib_base_struct.configGroup.cfgHighMajorTempLevel,sizeof("battHiTemp"));
    setCmd_flag = 1;
    setCmd_mask = SET_OVERTEMP;
  }
  //AC Low Level:---------------------------------------------------------------
  else if(strncmp(uri, "/acLowThres?value=", 11) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgAcLowLevel,sizeof("acLowThres"));
    setCmd_flag = 1;
    setCmd_mask = SET_AC_THRES;
  }
  //Battery Test Setting:------------------------------------------------------
  //End Voltage:------------------------------
  else if(strncmp(uri, "/btEndVolt?value=", 10) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgBTGroup.cfgBTEndVolt,sizeof("btEndVolt"));
    setCmd_flag = 1;
    setCmd_mask = SET_BT_ENDVOLT;
  }
  //End Capacity:-----------------------------
  else if(strncmp(uri, "/btEndCap?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgBTGroup.cfgBTEndCap,sizeof("btEndCap"));
    setCmd_flag = 1;
    setCmd_mask = SET_BT_ENDCAP;
  }
  
  //End Time:---------------------------------
  else if(strncmp(uri, "/btEndTime?value=", 10) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgBTGroup.cfgBTEndTime,sizeof("btEndTime"));
    setCmd_flag = 1;
    setCmd_mask = SET_BT_ENDTIME;
  }
  //Planned Test Option:----------------------
  else if(strncmp(uri, "/btPlanEn?value=", 9) == 0)
  {
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgBTGroup.cfgBTPlanTestEn,sizeof("btPlanEn"));
    setCmd_flag = 1;
    setCmd_mask = SET_BT_PLEN;
  }
  
  //Test Option:---------------------------------------------------------------
  else if(strncmp(uri, "/plantest1m?value=", 9) == 0)
  {
    uint32_t temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("plantest1m"));
    data = uri;
    switch (data[sizeof("plantest1m")-2]-0x30)
    {
    case 1:
      {
        if(strncmp(&data[sizeof("plantest1m")-1],"m",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestMonth = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"d",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestDate = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"h",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestHour = temp;
        }
        sBattTestInfo.u8SetPlan = 1;
        setCmd_flag = 1;
        setCmd_mask = SET_BT_PLANTEST;
      }
      break;
    case 2:
      {
        if(strncmp(&data[sizeof("plantest1m")-1],"m",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestMonth = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"d",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestDate = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"h",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestHour = temp;
        }
        sBattTestInfo.u8SetPlan = 2;
        setCmd_flag = 1;
        setCmd_mask = SET_BT_PLANTEST;
      }
      break;
    case 3:
      {
        if(strncmp(&data[sizeof("plantest1m")-1],"m",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestMonth = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"d",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestDate = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"h",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestHour = temp;
        }
        sBattTestInfo.u8SetPlan = 3;
        setCmd_flag = 1;
        setCmd_mask = SET_BT_PLANTEST;
      }
      break;
    case 4:
      {
        if(strncmp(&data[sizeof("plantest1m")-1],"m",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestMonth = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"d",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestDate = temp;
        }
        else if(strncmp(&data[sizeof("plantest1m")-1],"h",1) == 0)
        {
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestHour = temp;
        }
        sBattTestInfo.u8SetPlan = 4;
        setCmd_flag = 1;
        setCmd_mask = SET_BT_PLANTEST;
      }
      break;
    default:
      break;
    };
    sBattTestInfo.u8CheckSentAll = 1;    
  }
  //Planned Test 1 Set:-----------------------
  //Planned Test 2 Set:-----------------------
  //Planned Test 3 Set:-----------------------
  //Planned Test 4 Set:-----------------------
  
  //SCU Time:------------------------------------------------------------------
  //SCU Time Set:-----------------------------
  else if(strncmp(uri, "/scutimem?value=", 8) == 0)
  {
    uint32_t temp=0;
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&temp,sizeof("scutimem"));
    data = uri;
    if(strncmp(&data[sizeof("scutimem")-1],"y",1) == 0)
    {
      private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUYear = temp;
    }
    else if(strncmp(&data[sizeof("scutimem")-1],"m",1) == 0)
    {
      private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMonth = temp;
    }
    else if(strncmp(&data[sizeof("scutimem")-1],"d",1) == 0)
    {
      private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUDate = temp;
    }
    else if(strncmp(&data[sizeof("scutimem")-1],"h",1) == 0)
    {
      private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUHour = temp;
    }
    else if(strncmp(&data[sizeof("scutimem")-1],"p",1) == 0)
    {
      private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMinute = temp;
    }
    else if(strncmp(&data[sizeof("scutimem")-1],"s",1) == 0)
    {
      private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUSecond = temp;
    }
    
    sBattTestInfo.u8SetPlan = 5;
    setCmd_flag = 1;
    setCmd_mask = SET_BT_SCUTIME;
    
    sBattTestInfo.u8CheckSentAll = 1;
  }
//config power aggsion======================================================================
  //Battery 2 Capacity:---------------------------------------------------------
  else if(strncmp(uri, "/battCap2?value=", 9) == 0)
  {
    // Battery Capacity 2
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgBattCapacityTotal2,sizeof("battCap2"));
    setCmd_flag = 1;
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 1)
    {
      setCmd_mask = SET_BATTSTDCAP2;
    }else if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 2)
    {
      setCmd_mask = SET_BATTSTDCAP;
    }else if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      setCmd_mask = SET_BATTSTDCAP2;
    }
    
  }
  //Planned Test Start:---------------------------------------------------------
  else if(strncmp(uri, "/btplstart?id=", 9) == 0)
  {
    // Battery Test Plan Start
    setCmd_flag = 1;
    setCmd_mask = START_BT_TEST;
  }
  //Planned Test Stop:---------------------------------------------------------
  else if(strncmp(uri, "/btplstop?id=", 9) == 0)
  {
    setCmd_flag = 1;
    setCmd_mask = STOP_BT_TEST;
  }

//config power m221============================================================
  //batt dc over volt----------------------------------------------------------
  else if(strncmp(uri, "/battDCOver?value=", 10) == 0)
  {
    //Battery DC Over voltage
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgDCOverVolt,sizeof("battDCOver"));
    setCmd_flag = 1;
    setCmd_mask = SET_DCOVER;
  }
  //ac high voltage-------------------------------------------------------------
  else if(strncmp(uri, "/acHiThres?value=", 10) == 0)
  {
    // AC High Threshold
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgAcHighLevel,sizeof("acHiThres"));
    setCmd_flag = 1;
    setCmd_mask = SET_AC_HIGH_THRES;
  }
  //test volt-------------------------------------------------------------------
  else if(strncmp(uri, "/btZTETestVolt?value=", 13) == 0)
  {
    // Battery Test Volt
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgBTGroup.cfgTestVoltCfg,sizeof("btZTETestVolt"));
    setCmd_flag = 1;
    setCmd_mask = SET_TEST_VOLT;
  }
//config power zte=============================================================
  else if(strncmp(uri, "/btZTETestDay?value=", 13) == 0)
  {
    // Battery Test Day
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgBTGroup.cfgAutoTestDay,sizeof("btZTETestDay"));
    setCmd_flag = 1;
    setCmd_mask = SET_AUTO_TEST_DAY;
    
  }
  else if(strncmp(uri, "/btZTETestTime?value=", 13) == 0)
  {
    // Battery Test Time
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.cfgBTGroup.cfgTestStartTime,sizeof("btZTETestTime"));
    setCmd_flag = 1;
    setCmd_mask = SET_TEST_START_TIME;
  }
//config power dpc==============================================================
  //Battery Charge Current Limit------------------------------------------------
  else if (strncmp(uri, "/battdpcCCL?value=", 11) == 0)
  {
    // charge current limit (A)
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4)
    {
      text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgCurrentlimitA,sizeof("battdpcCCL"));
      setCmd_flag = 1;
      setCmd_mask = SET_CCL;
    }
  }
  //Battery capacity 3---------------------------------------------------------
  else if(strncmp(uri, "/battCap3?value=", 9) == 0)
  {
    // Battery Capacity 3
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgBattCapacityTotal3,sizeof("battCap3"));
    setCmd_flag = 1;
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      setCmd_mask = SET_BATTSTDCAP3;
    }
  }
  //battery capacity 4----------------------------------------------------------
  else if(strncmp(uri, "/battCap4?value=", 9) == 0)
  {
    // Battery Capacity 4
    text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.configGroup.cfgBattCapacityTotal4,sizeof("battCap4"));
    setCmd_flag = 1;
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      setCmd_mask = SET_BATTSTDCAP4;
    }
  }
  // fan dc startting point-----------------------------------------------------
  else if(strncmp(uri, "/fanstartpoint?value=", 14) == 0)
  {
    // fan dc startting point
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      
      text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.fan_dpc_info.mib.temp_set,sizeof("fanstartpoint"));
      private_mib_base_struct.fan_dpc_info.mib.para_dc_starting_point = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.temp_set;
      fan_info_dpc_t.para_dc_starting_point                  = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.temp_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_STARTING_POINT;
    }
  }
  // fan dc sensivive point-----------------------------------------------------
  else if(strncmp(uri, "/fansensitpoint?value=", 15) == 0)
  {
    // fan dc sensivive point
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      
      text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.fan_dpc_info.mib.temp_set,sizeof("fansensitpoint"));
      private_mib_base_struct.fan_dpc_info.mib.para_dc_sensivive_point = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.temp_set;
      fan_info_dpc_t.para_dc_sensivive_point                  = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.temp_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_SENSITIVE_POINT;
    }
  }
  // fan alarm low temp--------------------------------------------------------
  else if(strncmp(uri, "/fanalarmHtemp?value=", 14) == 0)
  {
    // fan alarm high temp
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      
      text_sBuffHttp_len = html_text_extract(uri,(uint32_t*)&private_mib_base_struct.fan_dpc_info.mib.temp_set,sizeof("fanalarmHtemp"));
      private_mib_base_struct.fan_dpc_info.mib.para_alarm_high_temp = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.temp_set;
      fan_info_dpc_t.para_alarm_high_temp                  = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.temp_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_H_TEMP_W_POINT;
    }
  }
  // fan alarm low temp---------------------------------------------------------
  else if(strncmp(uri, "/fanalarmLtemp?value=", 14) == 0)
  {
    // fan alarm low temp
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      
      text_sBuffHttp_len = htmlTextExtract2(uri,(int32_t*)&private_mib_base_struct.fan_dpc_info.mib.i_tem_set,sizeof("fanalarmLtemp"));
      private_mib_base_struct.fan_dpc_info.mib.para_alarm_low_temp = (int16_t) private_mib_base_struct.fan_dpc_info.mib.i_tem_set;
      fan_info_dpc_t.para_alarm_low_temp                  = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.i_tem_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_L_TEMP_W_POINT;
    }
  }
  // fan alarm heat start temp--------------------------------------------------
  else if (strncmp(uri, "/fanHeatSpoint?value=", 14) == 0)
  {
    // fan alarm heat start temp
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      
      text_sBuffHttp_len = htmlTextExtract2(uri,(int32_t*)&private_mib_base_struct.fan_dpc_info.mib.i_tem_set,sizeof("fanHeatSpoint"));
      private_mib_base_struct.fan_dpc_info.mib.para_dc_heating_start_point = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.i_tem_set;
      fan_info_dpc_t.para_dc_heating_start_point                 = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.i_tem_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_HEATING_START_POINT;
    }
  }
  // fan alarm heat sensivive temp----------------------------------------------
  else if (strncmp(uri, "/fanHeatSI?value=", 10) == 0)
  {
    // fan alarm heat sensivive temp
    if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
      
      text_sBuffHttp_len = htmlTextExtract2(uri,(int32_t*)&private_mib_base_struct.fan_dpc_info.mib.i_tem_set,sizeof("fanHeatSI"));
      private_mib_base_struct.fan_dpc_info.mib.para_dc_heating_sensivive = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.i_tem_set;
      fan_info_dpc_t.para_dc_heating_sensivive                  = (uint16_t) private_mib_base_struct.fan_dpc_info.mib.i_tem_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_HEATING_SENSITIVE_POINT;
    }
  }
  
//alarm=========================================================================
  //alarm general---------------------------------------------------------------
  else if(!strcasecmp(uri, "/alarmGen_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d;%d",
                private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp,
                private_mib_base_struct.main_alarm_group_struct.alarm_llvd,
                private_mib_base_struct.main_alarm_group_struct.alarm_blvd,
                private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low,
                private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains,
                private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open,
                private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open,
                private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault,
                private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp,
                private_mib_base_struct.main_alarm_group_struct.alarm_manual_mode,
                private_mib_base_struct.main_alarm_group_struct.alarm_ac_low,
                private_mib_base_struct.main_alarm_group_struct.alarm_smoke,
                private_mib_base_struct.main_alarm_group_struct.alarm_env_door
                  );
  }
  //alarm digital input---------------------------------------------------------
  else if(!strcasecmp(uri, "/alarmDI_status"))
  {
    n = sprintf(sBuffHttp,"%d;%d;%d;%d;%d;%d;%d;%d",
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0],
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[1],
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[2],
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[3],
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[4],
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[5],
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[6],
                private_mib_base_struct.diAlarmGroup.alarmDigitalInput[7]
                  );
  }
  //history alarm---------------------------------------------------------------
  else if(!strcasecmp(uri, "/alarmHis_status"))
  {
    //uint16_t n = 0,i;
    for (uint16_t i = 0; i < 200; i++)
    {
      if(n < HTTP_BUFF)
      {
        n += snprintf(sBuffHttp + n, HTTP_BUFF - n,
                      "%d;%s;%s;%s;%s@",
                      i,
                      sHistoryInfo.sHistoryParam[i].sAlarmContent.ucName,
                      sHistoryInfo.sHistoryParam[i].sAlarmContent.cRectID,
                      sHistoryInfo.sHistoryParam[i].sTimeFrame[0].cTimeFrame,
                      sHistoryInfo.sHistoryParam[i].sTimeFrame[1].cTimeFrame
                        );
      }
    }
  }
  else if(!strcasecmp(uri, "/alrmget"))
  {
    sHistoryInfo.ucSequenceNum = 0;
    if (TYPE == EMERV21) {MESGState_v21 = HISTORYMESG_REQ;}
    else MESGState = HISTORYMESG_REQ;
    
  }
  
  
  /*
  else if(strncmp(uri, "/startupdate?value=", 12) == 0)
  {
    // namht Test update FW
    uint32_t temp = 0;
    html_text_extract(uri,(uint32_t*)&temp,sizeof("startupdate"));
    if (temp == 1) config_handler_struct.u8FwUpdateFlag = temp;
    n = sprintf(sBuffHttp,"%d", (int)temp);
  }*/
  /* =========================================================================
  * Finish.
  * ========================================================================= */
  else return ERROR_NOT_FOUND;
  return httpServeruriResponse(&sBuffHttp[0], n, connection);
}

/**
 * @brief URI not found callback
 * @param[in] connection Handle referencing a client connection
 * @param[in] uri NULL-terminated string containing the path to the uried resource
 * @return Error code
 **/

error_t httpServerUriNotFoundCallback(HttpConnection *connection, const char_t *uri) {
   //Not implemented
   return ERROR_NOT_FOUND;
}

void http_handler_init(void) {
  error_t error;
  
  //Get default settings
  httpServerGetDefaultSettings(&httpServerSettings);
  //Bind HTTP server to the desired interface
  httpServerSettings.interface = &netInterface[0];
  //Listen to port 80
  httpServerSettings.port = HTTP_PORT;
  //Client connections
  httpServerSettings.maxConnections = APP_HTTP_MAX_CONNECTIONS;
  httpServerSettings.connections = httpConnections;
  //Specify the server's root directory
  strcpy(httpServerSettings.rootDirectory, "/www/");
  //Set default home page
  strcpy(httpServerSettings.defaultDocument, "login.html");
  //Callback functions
  httpServerSettings.cgiCallback = httpServerCgiCallback;
  httpServerSettings.requestCallback = httpServeruriCallback;
  httpServerSettings.uriNotFoundCallback = httpServerUriNotFoundCallback;
  
  //HTTP server initialization
  error = httpServerInit(&httpServerContext, &httpServerSettings);
  //Failed to initialize HTTP server?
  if(error)
  {
    //Debug message
//    TRACE_ERROR("Failed to initialize HTTP server!\r\n");
  }
  
  //Start HTTP server
  error = httpServerStart(&httpServerContext);
  //Failed to start HTTP server?
  if(error)
  {
    //Debug message
//    TRACE_ERROR("Failed to start HTTP server!\r\n");
  }
}