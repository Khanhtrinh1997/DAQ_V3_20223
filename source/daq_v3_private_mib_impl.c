/**
 * @file private_mib_impl.c
 * @brief Private MIB module implementation
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

//Dependencies
#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "board.h"
#include "core/net.h"
#include "daq_v3_private_mib_module.h"
#include "daq_v3_private_mib_impl.h"
#include "core/crypto.h"
#include "encoding/asn1.h"
#include "encoding/oid.h"
#include "debug.h"
#include "header_files.h"

//Mutex preventing simultaneous access to the private MIB base
static OsMutex privateMibMutex;


/**
 * @brief Private MIB module initialization
 * @return Error code
 **/

error_t privateMibInit(void) {
  
  uint8_t i;
  //Clear memory----------------------------------------------------------------
  memset(&modbus_manager_struct,0,sizeof(modbus_manager_struct));
  //Clear private MIB base------------------------------------------------------
  memset(&private_mib_base_struct, 0, sizeof(private_mib_base_struct));
  
  private_mib_base_struct.cfgNetworkGroup.cSyncTimeLen = 20;
  sprintf(&private_mib_base_struct.cfgNetworkGroup.cSyncTime[0], "09:00:00-30/05/22");
  
  private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32IP = revertIP(parameter.ip_parameter.static_ip);
  private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32GW = revertIP(parameter.ip_parameter.gateway_ip);
  private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SN = revertIP(parameter.ip_parameter.subnet_mask);
  private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SIP = revertIP(parameter.ip_parameter.snmp_ip);
  private_mib_base_struct.cfgNetworkGroup.u8TrapMode = parameter.ip_parameter.trap_mode;
  private_mib_base_struct.cfgNetworkGroup.u32TrapInterval = parameter.ip_parameter.trap_period;
  private_mib_base_struct.siteGroup.siteControllerModel[0] = parameter.u8ControllerType;
  private_mib_base_struct.siteGroup.siteFirmwareCRC = working_default_parameter_2.u32FirmwareCRC;
  private_mib_base_struct.cfgNetworkGroup.ftp_port  = parameter.ip_parameter.ftp_port;
  
  private_mib_base_struct.pmGroup.pmTable[0].pmDeltaFreqDisConnect = (uint32_t)parameter.u8DeltaFreqDisConnect[0];
  private_mib_base_struct.pmGroup.pmTable[1].pmDeltaFreqDisConnect = (uint32_t)parameter.u8DeltaFreqDisConnect[1];
  private_mib_base_struct.pmGroup.pmTable[0].pmDeltaFreqReConnect = (uint32_t)parameter.u8DeltaFreqReConnect[0];
  private_mib_base_struct.pmGroup.pmTable[1].pmDeltaFreqReConnect = (uint32_t)parameter.u8DeltaFreqReConnect[1];
  private_mib_base_struct.pmGroup.pmTable[0].pmEnableFreqTrap = (uint32_t)parameter.u8EnableFreqTrap;
  
  private_mib_base_struct.isenseGroup.isenseTable.isenseDeltaFreqDisConnect = (uint32_t)parameter.u8DeltaFreqDisConnect2;
  private_mib_base_struct.isenseGroup.isenseTable.isenseDeltaFreqReConnect = (uint32_t)parameter.u8DeltaFreqReConnect2;
  private_mib_base_struct.isenseGroup.isenseTable.isenseEnableFreqTrap = (uint32_t)parameter.u8EnableFreqTrap2;
  
  switch(private_mib_base_struct.siteGroup.siteControllerModel[0])
  {
  case 0:
    {
      strcpy(private_mib_base_struct.siteGroup.siteDCsystemVendor, "Emerson");
      private_mib_base_struct.siteGroup.siteDCsystemVendorLen = sizeof(private_mib_base_struct.siteGroup.siteDCsystemVendor);
    }break;
  case 1:
    {
      strcpy(private_mib_base_struct.siteGroup.siteDCsystemVendor, "ZTE");
      private_mib_base_struct.siteGroup.siteDCsystemVendorLen = sizeof(private_mib_base_struct.siteGroup.siteDCsystemVendor);
    }break;
  case 2:
    {
      strcpy(private_mib_base_struct.siteGroup.siteDCsystemVendor, "Agisson");
      private_mib_base_struct.siteGroup.siteDCsystemVendorLen = sizeof(private_mib_base_struct.siteGroup.siteDCsystemVendor);
    }break;
  case 3:
    {
      strcpy(private_mib_base_struct.siteGroup.siteDCsystemVendor, "Emerson");
      private_mib_base_struct.siteGroup.siteDCsystemVendorLen = sizeof(private_mib_base_struct.siteGroup.siteDCsystemVendor);
    }break;
  case 4:
    {
      strcpy(private_mib_base_struct.siteGroup.siteDCsystemVendor, "DPC");
      private_mib_base_struct.siteGroup.siteDCsystemVendorLen = sizeof(private_mib_base_struct.siteGroup.siteDCsystemVendor);
    }
    break;
  default:
    break;
  };
  
  strcpy(private_mib_base_struct.siteGroup.siteDevModel, "DAQ-V3");
  private_mib_base_struct.siteGroup.siteDevModelLen = 7;
  
  strcpy(private_mib_base_struct.genGroup.genTable[0].genSerial, "   ");
  private_mib_base_struct.genGroup.genTable[0].genSerialLen = 3;
  strcpy(private_mib_base_struct.genGroup.genTable[1].genSerial, "   ");
  private_mib_base_struct.genGroup.genTable[1].genSerialLen = 3;
  
  strcpy((char*)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucIP,ipv4AddrToString((Ipv4Addr)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32IP,0));
  strcpy((char*)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucGW,ipv4AddrToString((Ipv4Addr)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32GW,0));
  strcpy((char*)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSN,ipv4AddrToString((Ipv4Addr)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SN,0));
  strcpy((char*)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP,ipv4AddrToString((Ipv4Addr)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SIP,0));
  
  sAlarmValue[0].u8AlarmValue = 2;
  sAlarmValue[0].u8NormalValue = 0;
  for (i=1; i < 14;i++)
  {
    sAlarmValue[i].u8AlarmValue = 1;
    sAlarmValue[i].u8NormalValue = 0;      
  }
  sAlarmValue[9].u8AlarmValue = 0xff;
  sAlarmValue[9].u8NormalValue = 0;
  for (i=50; i < 55;i++)
  {
    sAlarmValue[i].u8AlarmValue = 1;
    sAlarmValue[i].u8NormalValue = 0;      
  }
  for (i=100; i < 115;i++)
  {
    sAlarmValue[i].u8AlarmValue = 1;
    sAlarmValue[i].u8NormalValue = 0;      
  }
  
  private_mib_base_struct.siteGroup.siteTrapCounter = 0;
  strcpy(private_mib_base_struct.siteGroup.siteSerialNumber, (const char*)working_default_parameter_2.u8UserSerialNo);
  private_mib_base_struct.siteGroup.siteSerialNumberLen = strlen(private_mib_base_struct.siteGroup.siteSerialNumber);    
  
  strcpy(private_mib_base_struct.cfgNetworkGroup.cFTPServer, (const char*)parameter.ip_parameter.uc_ftp_server);
  private_mib_base_struct.cfgNetworkGroup.cFTPServerLen = sizeof(private_mib_base_struct.cfgNetworkGroup.cFTPServer);
  
  snprintf(private_mib_base_struct.siteGroup.siteMACInfo,18,"%02X:%02X:%02X:%02X:%02X:%02X",
           working_default_parameter_2.u8UserMAC[0],
           working_default_parameter_2.u8UserMAC[1],
           working_default_parameter_2.u8UserMAC[2],
           working_default_parameter_2.u8UserMAC[3],
           working_default_parameter_2.u8UserMAC[4],
           working_default_parameter_2.u8UserMAC[5]
             );
  if (working_default_parameter_2.u32FirmwareCRC_old != working_default_parameter_2.u32FirmwareCRC)
  {
    private_mib_base_struct.cfgNetworkGroup.u8NewFWFlag = 1;
    working_default_parameter_2.u32FirmwareCRC_old = working_default_parameter_2.u32FirmwareCRC;      
    system_flag_struct.u8IsRewriteSN = 1;
  }
  //    strcpy(private_mib_base_struct.siteGroup.siteMACInfo, "00:1a:b6:22:03:91");
  private_mib_base_struct.siteGroup.siteMACInfoLen = strlen(private_mib_base_struct.siteGroup.siteMACInfo);
  strcpy(private_mib_base_struct.siteGroup.siteBTSCode,(const char*) parameter.ip_parameter.site_name);
  private_mib_base_struct.siteGroup.siteBTSCodeLen = sizeof(private_mib_base_struct.siteGroup.siteBTSCode);
  snprintf(private_mib_base_struct.siteGroup.siteCRCStr,11,"0x%x",
           private_mib_base_struct.siteGroup.siteFirmwareCRC);
  private_mib_base_struct.siteGroup.siteCRCStrLen = 11;
  
  private_mib_base_struct.acPhaseGroup.acPhaseNumber = 3;
  for (i = 0; i < 4; i++)
  {
    strcpy(private_mib_base_struct.rectGroup.rectTable[i].rectType, "DC48V");
    private_mib_base_struct.rectGroup.rectTable[i].rectTypeLen = strlen(private_mib_base_struct.rectGroup.rectTable[i].rectType);     
  }
  //    private_mib_base_struct.acPhaseGroup.acPhaseTable[0].acPhaseIndex = 3;
  //    private_mib_base_struct.acPhaseGroup.acPhaseTable[0].acPhaseVolt = 222;
//slave libbat--------------------------------------------------------------------------------------------         
  if(parameter.device_number[TOTAL_LIB_INDEX] <= 16)
  {
    modbus_manager_struct.number_of_lib = parameter.device_number[TOTAL_LIB_INDEX];                
    private_mib_base_struct.liBattGroup.liBattInstalledPack = modbus_manager_struct.number_of_lib;
    for (i = 0; i < private_mib_base_struct.liBattGroup.liBattInstalledPack; i++)
    {
      modbus_manager_struct.lib_manager_struct[i].lib_type = parameter.modbus_parameter[i].device_vendor;
      modbus_manager_struct.lib_manager_struct[i].lib_abs_slave_id = parameter.modbus_parameter[i].slave_id;
      switch(modbus_manager_struct.lib_manager_struct[i].lib_type)
      {
      case 13:// COSLIGHT_CF4850T
      case 1:// COSLIGHT
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0;      // FC 04
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 58;      
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 128;    // FC 02
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 30;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 48;    // FC 03
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 4;
        }	
        break;
      case 2:// COSLIGHT_OLD_V1.1
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 30;      // FC 04
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 24;      
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 128;    // FC 02
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 28;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 48;    // FC 03
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 2;
        }	
        break;
      case 12:// SHOTO_SDA10_48100 
      case 3:// SHOTO_2019
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0x1000;      // FC 04
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 10;      
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 0x2000;    
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 25;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 0x3040;    
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 56;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[3] = 0x800f;    
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[3] = 11;
        }	
        break;
      case 4:// HUAWEI
        {
          if(i<8)
            modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 213;
          else
            modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 215;
          //              modbus_manager_struct.lib_manager_struct[i].u32LIBAbsSlaveID = modbus_manager_struct.lib_manager_struct[i].lib_slave_offset + i + 1;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 50;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 67;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 8;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 257;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 7;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[3] = 529;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[3] = 10;
        }
        break;  
      case 8:// HUAWEI_A1
        {
          if(i<8)
            modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 213;
          else
            modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 215;
          //              modbus_manager_struct.lib_manager_struct[i].u32LIBAbsSlaveID = modbus_manager_struct.lib_manager_struct[i].lib_slave_offset + i + 1;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 50;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 67;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 8;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 257;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 7;
        }
        break;  
      case 5:// M1Viettel50
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0; // FC:0x04
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 100;   
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 130;// FC:0x04
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 22; //20
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 0;// FC:0x03
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 80; 
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[3] = 162;// FC:0x03
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[3] = 3;//2   
        }
        break;          
        //          case 8:// BYU75
        //          {
        //              modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
        //              modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0;
        //              modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 54;
        //          }
        //          break;
        case 14:// HUAFU_HF48100C
      case 6:// ZTT_2020   
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 42;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 150;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 10;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 170;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 10;
        }
        break;       
      case 9:// SAFT
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 23764;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 16;
        }
        break;
      case 10:// Narada75
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 38;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 4095;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 9;
        }
        break;
      case 7:// ZTT50
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 38;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 4096;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 9;
        }
        break;
      case 11:// EVE
        {
          modbus_manager_struct.lib_manager_struct[i].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[0] = 0;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[0] = 42;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[1] = 150;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[1] = 10;
          modbus_manager_struct.lib_manager_struct[i].lib_base_addr[2] = 170;
          modbus_manager_struct.lib_manager_struct[i].lib_num_of_reg[2] = 10;
        }
        break;
      };
    }      
  }
  
  //slave gen ------------------------------------------------------------------      
  if(parameter.device_number[TOTAL_GEN_INDEX] <= 2)
  {                
    modbus_manager_struct.number_of_gen = parameter.device_number[TOTAL_GEN_INDEX];
    private_mib_base_struct.genGroup.genInstalledGen = modbus_manager_struct.number_of_gen;
    for (i = 0; i < private_mib_base_struct.genGroup.genInstalledGen; i++)
    {
      modbus_manager_struct.gen_manager_struct[i].gen_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB].device_vendor;
      modbus_manager_struct.gen_manager_struct[i].gen_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB].slave_id;;
      switch(modbus_manager_struct.gen_manager_struct[i].gen_type)
      {
      case 1:// KUBOTA
        {
          modbus_manager_struct.gen_manager_struct[i].gen_slave_offset = 60;
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[0] = 0;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[0] = 85;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 235;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 1;
          modbus_manager_struct.gen_manager_struct[i].gen_running_step = GEN_KUBOTA_INFO_1;
        }
        break;  
      case 2:// BE142
        {
          modbus_manager_struct.gen_manager_struct[i].gen_slave_offset = 60;
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[0] = 30010;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[0] = 38;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 30135;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 4;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 30079;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 27;
          
          modbus_manager_struct.gen_manager_struct[i].gen_running_step = GEN_BE142_INFO_1;
        }
        break; 
      case 3:// DEEPSEA
        {
          modbus_manager_struct.gen_manager_struct[i].gen_slave_offset = 60;
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[0] = 0x400;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[0] = 34;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 0x700;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 8;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[2] = 0x800;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[2] = 8;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[3] = 0x304;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[3] = 1;
          modbus_manager_struct.gen_manager_struct[i].gen_running_step = GEN_DEEPSEA_INFO_1;
        }
        break; 
      case 4:// LR2057
        {
          modbus_manager_struct.gen_manager_struct[i].gen_slave_offset = 60;
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[0] = 6;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[0] = 19;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 42;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 13;
          //              
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[2] = 12;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[2] = 66;
          modbus_manager_struct.gen_manager_struct[i].gen_running_step = GEN_LR2057_INFO_1;
        }
        break; 
      case 5:// HIMOINSA
        {
          modbus_manager_struct.gen_manager_struct[i].gen_slave_offset = 86;
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[0] = 0;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[0] = 23;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 7;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 36;
          
          modbus_manager_struct.gen_manager_struct[i].gen_running_step = GEN_HIMOINSA_INFO_1;
        }
        break; 
      case 6:// QC315
        {
          modbus_manager_struct.gen_manager_struct[i].gen_slave_offset = 60;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[0] = 20;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[0] = 61;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 708;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 6;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[2] = 800;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[2] = 7;
          modbus_manager_struct.gen_manager_struct[i].gen_running_step = GEN_QC315_INFO_1;
        }
        break; 
      case 7:// CUMMIN
        {
          modbus_manager_struct.gen_manager_struct[i].gen_slave_offset = 60;
          
          modbus_manager_struct.gen_manager_struct[i].gen_base_addr[0] = 12;
          modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[0] = 60;
          //              
          //              modbus_manager_struct.gen_manager_struct[i].gen_base_addr[1] = 708;
          //              modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[1] = 6;
          //              
          //              modbus_manager_struct.gen_manager_struct[i].gen_base_addr[2] = 800;
          //              modbus_manager_struct.gen_manager_struct[i].gen_num_of_reg[2] = 7;
          modbus_manager_struct.gen_manager_struct[i].gen_running_step = GEN_CUMMIN_INFO_1;
        }
        break; 
      };
    }
  }
  
//slave bm=================================================================================  
  //========================== RS485 Batt Monitor Initialization ===========================//
  if(parameter.device_number[TOTAL_BM_INDEX] <= 16)
  {    
    modbus_manager_struct.number_of_bm = parameter.device_number[TOTAL_BM_INDEX];                
    private_mib_base_struct.bmGroup.bmInstalledBM = modbus_manager_struct.number_of_bm;
    for (i = 0; i < private_mib_base_struct.bmGroup.bmInstalledBM; i++)
    {
      modbus_manager_struct.bm_manager_struct[i].bm_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN].device_vendor;//1
      modbus_manager_struct.bm_manager_struct[i].bm_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN].slave_id;//i+70
      switch(modbus_manager_struct.bm_manager_struct[i].bm_type)
      {
      case 1:// VIETTEL
        {
          modbus_manager_struct.bm_manager_struct[i].bm_slave_offset = 69;
          modbus_manager_struct.bm_manager_struct[i].bm_base_addr[0] = 0;
          modbus_manager_struct.bm_manager_struct[i].bm_num_of_reg[0] = 56;
          modbus_manager_struct.bm_manager_struct[i].bm_running_step = BM_VIETTEL_INFO_1;
        }
        break;     
      };
    } 
  }
  
//slave pm ---------------------------------------------------------------------
  
  private_mib_base_struct.pmGroup.pmTable[0].pmSerialLen = 1;
  sprintf(&private_mib_base_struct.pmGroup.pmTable[0].pmSerial[0], "");
  modbus_manager_struct.u8PMBootPassFlag = 0;
  
  if(parameter.device_number[TOTAL_PM_INDEX] <= 2)
  {                
    modbus_manager_struct.number_of_pm = parameter.device_number[TOTAL_PM_INDEX];
    private_mib_base_struct.pmGroup.pmInstalledPM = modbus_manager_struct.number_of_pm;
    for (i = 0; i < private_mib_base_struct.pmGroup.pmInstalledPM; i++)
    {
      modbus_manager_struct.pm_manager_struct[i].pm_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM].device_vendor;  
      modbus_manager_struct.pm_manager_struct[i].pm_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM].slave_id;;     
      switch(modbus_manager_struct.pm_manager_struct[i].pm_type)   //switch(modbus_manager_struct.pm_manager_struct[i].pm_type)
      {
      case 1:// FINECO
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset = 16;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0] = 16;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0] = 50;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1] = 0x700;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_running_step = PM_FINECO_INFO_1;
        }
        break;  
      case 2:// ASCENT
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset = 16;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0] = 0;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1] = 100;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[2] = 106;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[2] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[3] = 118;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[3] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[4] = 142;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[4] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[5] = 144;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[5] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_running_step = PM_ASCENT_INFO_1;
        }
        break;    
      case 3:// EASTRON
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset = 16;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0] = 0;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0] = 32;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1] = 70;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1] = 10;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[2] = 342;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[2] = 4;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[3] = 64512;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[3] = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_running_step = PM_EASTRON_INFO_1;
        }
        break;    
      case 4:// CET1
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset = 52;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0] = 0;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0] = 14;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1] = 40;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1] = 18;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[2] = 9800;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[2] = 27;
          modbus_manager_struct.pm_manager_struct[i].pm_running_step = PM_CET1_INFO_1;
        }
        break;  
      case 5://PILOT
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset = 16;   
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0] = 0;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0] = 17;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1] = 19;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1] = 6;              
          modbus_manager_struct.pm_manager_struct[i].pm_running_step = PM_PILOT_INFO_1;
        }
        break; 
        //        case 6://FORLONG
        //          {
        //              modbus_manager_struct.pm_manager_struct[i].pm_slave_offset = 16;   
        //              modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0] = 0;
        //              modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0] = 20;             
        //              modbus_manager_struct.pm_manager_struct[i].pm_running_step = PM_FORLONG_INFO_1;
        //          }
        //          break; 
      case 15://PILOT_3PHASE
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset = 16;   
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0] = 100;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0] = 43;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1] = 1000;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1] = 12;   
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[2] = 9000;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[2] = 2;            
          modbus_manager_struct.pm_manager_struct[i].pm_running_step = PM_PILOT_3PHASE_INFO_1;
        }
        break;  
      case 16://YADA_3PHASE_DPC
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset  = 16;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0]  = 4240;//0x03 U
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0]   = 6;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1]  = 4288;//0x03 I
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1]   = 6;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[2]  = 4496;//0x03 cosphi
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[2]   = 4;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[3]  = 4528;//0x03 HZ
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[3]   = 1;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[4]  = 4192;//0x03 kvarh
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[4]   = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[5]  = 4144;//0x03 kwh
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[5]   = 2;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[6]  = 4352;//0x03 P
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[6]   = 8;
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[7]  = 4416;//0x03 Q
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[7]   = 8;
          modbus_manager_struct.pm_manager_struct[i].pm_running_step   = PM_YADA_3PHASE_DPC_INFO_1;
        }
        break;
      case 17: //Schneider 2022
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset  = 16;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0]  = 29;  
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0]   = 20;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1]  = 128;      
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1]   = 3;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[2]  = 2699;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[2]   = 24;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[3]  = 2999;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[3]   = 6;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[4]  = 3027; 
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[4]   = 6;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[5]  = 3053;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[5]   = 24;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[6]  = 3077;         
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[6]   = 8;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[7]  = 3109;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[7]   = 2;
          
          modbus_manager_struct.pm_manager_struct[i].pm_running_step   = PM_SCHNEDER_3PHASE_INFO_1;
        }
        break;
      case 18:  //EASTRON SMD72D 2022
        {
          modbus_manager_struct.pm_manager_struct[i].pm_slave_offset    = 16;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[0]    = 0;  
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[0]   = 30;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[1]    = 30;      
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[1]   = 30;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[2]    = 60;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[2]   = 30;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[3]    = 342;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[3]   = 30;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[4]    = 11; 
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[4]   = 30;
          
          modbus_manager_struct.pm_manager_struct[i].pm_base_addr[5]    = 64512;
          modbus_manager_struct.pm_manager_struct[i].pm_num_of_reg[5]   = 4;
          
          modbus_manager_struct.pm_manager_struct[i].pm_running_step   = PM_EASTRON_INFO_1;
          
        }
        break;
      };
    }
  }
//slave VAC-------------------------------------------------------------------------------------
  
  if(parameter.device_number[TOTAL_VAC_INDEX] <= 2)
  {                
    modbus_manager_struct.number_of_vac = parameter.device_number[TOTAL_VAC_INDEX];         
    private_mib_base_struct.vac_group_struct.vac_installed_vac = modbus_manager_struct.number_of_vac;
    for (i = 0; i < private_mib_base_struct.vac_group_struct.vac_installed_vac; i++)
    {
      modbus_manager_struct.vac_manager_struct[i].vac_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor;
      modbus_manager_struct.vac_manager_struct[i].vac_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].slave_id;
      switch(modbus_manager_struct.vac_manager_struct[i].vac_type)
      {
      case 1:// VIETTEL
        {
          modbus_manager_struct.vac_manager_struct[i].vac_slave_offset = 34;
          modbus_manager_struct.vac_manager_struct[i].vac_base_addr[0] = 0;
          modbus_manager_struct.vac_manager_struct[i].vac_num_of_reg[0] = 14;//22
          
          modbus_manager_struct.vac_manager_struct[i].vac_base_addr[1] = 0;
          modbus_manager_struct.vac_manager_struct[i].vac_num_of_reg[1] = 39;//29;//94;
          
          modbus_manager_struct.vac_manager_struct[i].vac_base_addr[2] = 80;
          modbus_manager_struct.vac_manager_struct[i].vac_num_of_reg[2] = 3;//29;//94;
          modbus_manager_struct.vac_manager_struct[i].vac_running_step = VAC_VIETTEL_INFO_1;
        }
        break;     
      };
    } 
  }    
//slave fuel ------------------------------------------------------------------
  
  if(parameter.device_number[TOTAL_FUEL_INDEX] <= 2)
  {                
    modbus_manager_struct.number_of_fuel = parameter.device_number[TOTAL_FUEL_INDEX];         
    private_mib_base_struct.fuelGroup.FuelInstalledFUEL = modbus_manager_struct.number_of_fuel;
    for (i = 0; i < private_mib_base_struct.fuelGroup.FuelInstalledFUEL; i++)
    {
      modbus_manager_struct.fuel_manager_struct[i].fuel_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor;
      modbus_manager_struct.fuel_manager_struct[i].fuel_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].slave_id;
      switch(modbus_manager_struct.fuel_manager_struct[i].fuel_type)
      {
      case 1:// HPT621
        {
          modbus_manager_struct.fuel_manager_struct[i].fuel_slave_offset = 23;
          modbus_manager_struct.fuel_manager_struct[i].fuel_base_addr[0] = 0;
          modbus_manager_struct.fuel_manager_struct[i].fuel_num_of_reg[0] = 1;
          
          modbus_manager_struct.fuel_manager_struct[i].fuel_running_step = FUEL_HPT621_INFO_1;
        }
        break;     
      };
    } 
  }       
 
//slave SMCB--------------------------------------------------------------------    
  if(parameter.device_number[TOTAL_SMCB_INDEX] <= 5)
  {                
    modbus_manager_struct.number_of_smcb = parameter.device_number[TOTAL_SMCB_INDEX];         
    private_mib_base_struct.smcbGroup.SmcbInstalledSMCB = modbus_manager_struct.number_of_smcb;
    for (i = 0; i < private_mib_base_struct.smcbGroup.SmcbInstalledSMCB; i++)
    {
      modbus_manager_struct.smcb_manager_struct[i].smcb_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor;
      modbus_manager_struct.smcb_manager_struct[i].smcb_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].slave_id;
      switch(modbus_manager_struct.smcb_manager_struct[i].smcb_type)
      {
      case 1:// OPEN
        {
          modbus_manager_struct.smcb_manager_struct[i].smcb_slave_offset = 18;
          modbus_manager_struct.smcb_manager_struct[i].smcb_base_addr[0] = 0;
          modbus_manager_struct.smcb_manager_struct[i].smcb_num_of_reg[0] = 1;
          
          modbus_manager_struct.smcb_manager_struct[i].smcb_running_step = SMCB_OPEN_INFO_1;
        }
        break; 
      case 2:// MATIS
        {
          modbus_manager_struct.smcb_manager_struct[i].smcb_slave_offset = 18;
          modbus_manager_struct.smcb_manager_struct[i].smcb_base_addr[0] = 16;
          modbus_manager_struct.smcb_manager_struct[i].smcb_num_of_reg[0] = 1;
          
          modbus_manager_struct.smcb_manager_struct[i].smcb_running_step = SMCB_MATIS_INFO_1;
        }
        break; 
        case 3:// GOL
        {
         modbus_manager_struct.smcb_manager_struct[i].smcb_slave_offset = 18;
         modbus_manager_struct.smcb_manager_struct[i].smcb_base_addr[0] = 32768;
         modbus_manager_struct.smcb_manager_struct[i].smcb_num_of_reg[0] = 1;
          
         modbus_manager_struct.smcb_manager_struct[i].smcb_running_step = SMCB_GOL_INFO_1;
        }
        break;
      };
    } 
  }    
    

//slave isense----------------------------------------------------------------- 
  
  private_mib_base_struct.isenseGroup.isenseTable.isenseSerialLen = 1;
  sprintf(&private_mib_base_struct.isenseGroup.isenseTable.isenseSerial[0], "");
  
  if(parameter.device_number[TOTAL_ISENSE_INDEX] <= 1)
  {                
    modbus_manager_struct.number_of_isense = parameter.device_number[TOTAL_ISENSE_INDEX];
    private_mib_base_struct.isenseGroup.isenseInstalledISENSE = modbus_manager_struct.number_of_isense;
    for (i = 0; i < private_mib_base_struct.isenseGroup.isenseInstalledISENSE; i++)
    {
      modbus_manager_struct.isense_manager_struct[i].isense_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB+MAX_NUM_OF_FUEL].device_vendor;;  
      modbus_manager_struct.isense_manager_struct[i].isense_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB+MAX_NUM_OF_FUEL].slave_id;;     
      switch(modbus_manager_struct.isense_manager_struct[i].isense_type) 
      {
      case 1://FORLONG
        {
          modbus_manager_struct.isense_manager_struct[i].isense_slave_offset = 26;   
          modbus_manager_struct.isense_manager_struct[i].isense_base_addr[0] = 0;
          modbus_manager_struct.isense_manager_struct[i].isense_num_of_reg[0] = 22;             
          modbus_manager_struct.isense_manager_struct[i].isense_running_step = ISENSE_FORLONG_INFO_1;
        }
        break; 
      };
    }
  }

//slave pm dc-------------------------------------------------------------------
  if(parameter.device_number[TOTAL_PMDC_INDEX] <= 1){
    modbus_manager_struct.number_of_pmdc = parameter.device_number[TOTAL_PMDC_INDEX];
    private_mib_base_struct.pm_dc_group.pm_dc_installed_PM_DC = modbus_manager_struct.number_of_pmdc;
    for (i = 0; i < private_mib_base_struct.pm_dc_group.pm_dc_installed_PM_DC; i++)
    {
      modbus_manager_struct.pmdc_manager_struct[i].pmdc_type = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB+MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor;  
      modbus_manager_struct.pmdc_manager_struct[i].pmdc_abs_slave_id = parameter.modbus_parameter[i+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB+MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].slave_id;     
      switch(modbus_manager_struct.pmdc_manager_struct[i].pmdc_type) 
      {
      case 1://YADA_DC
        {
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_slave_offset    = 30;
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_base_addr[0]    = 0;       //0x03
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_num_of_reg[0]   = 2;
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_base_addr[1]    = 12;       //0x03
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_num_of_reg[1]   = 2;
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_base_addr[2]    = 26;       //0x03
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_num_of_reg[2]   = 2;
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_base_addr[3]    = 40;       //0x03
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_num_of_reg[3]   = 2;
          modbus_manager_struct.pmdc_manager_struct[i].pmdc_running_step    = PM_DC_YADA_INFO_1;
        }
        break; 
      };
    }
  }
  
  private_mib_base_struct.cfgBTGroup.cfgBTPlanTestNumber = 4;
  private_mib_base_struct.cfgBTGroup.cfgBTSCUNumber = 1;  
  
   //Create a mutex to prevent simultaneous access to the private MIB base
   if(!osCreateMutex(&privateMibMutex))
   {
      //Failed to create mutex
      return ERROR_OUT_OF_RESOURCES;
   }

   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Load private MIB module
 * @param[in] context Pointer to the SNMP agent context
 * @return Error code
 **/

error_t privateMibLoad(void *context)
{
   //Successful processing
   return NO_ERROR;
}


/**
 * @brief Unload private MIB module
 * @param[in] context Pointer to the SNMP agent context
 **/

void privateMibUnload(void *context)
{
}


/**
 * @brief Lock private MIB base
 **/

void privateMibLock(void)
{
   //Enter critical section
   osAcquireMutex(&privateMibMutex);
}


/**
 * @brief Unlock private MIB base
 **/

void privateMibUnlock(void)
{
   //Leave critical section
   osReleaseMutex(&privateMibMutex);
}


/**
 * @brief Get currentTime object value
 * @param[in] object Pointer to the MIB object descriptor
 * @param[in] oid Object identifier (object name and instance identifier)
 * @param[in] oidLen Length of the OID, in bytes
 * @param[out] value Object value
 * @param[in,out] valueLen Length of the object value, in bytes
 * @return Error code
 **/

error_t privateMibGetCurrentTime(const MibObject *object, const uint8_t *oid,
   size_t oidLen, MibVariant *value, size_t *valueLen)
{
   //Get object value
   value->timeTicks = osGetSystemTime64() / 10;
   //Successful processing
   return NO_ERROR;
}


//SiteInfo Function ===========================================================
error_t privateMibSetSiteInfoGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{


  PrivateMibSiteInfoGroup *entry;
  Ipv4Addr tempIP;
  
  memset(&tempIP,0,sizeof(Ipv4Addr));
  
  //Point to the end of the OID prefix
  
  //	Point to the siteInfoGroup entry
  entry = &private_mib_base_struct.siteGroup;
  //siteSystemReset object?
  if(!strcmp(object->name, "siteSystemReset"))
  {
    if(value->integer==0x53)
    {
      //Get object value
      entry->siteSystemReset= value->integer;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //siteSNMPServer object?
  else if(!strcmp(object->name, "siteSNMPServer"))
  {
    
    //Make sure the buffer is large enough to hold the entire object
    if(valueLen > 15)
      //      return ERROR_BUFFER_OVERFLOW;
      valueLen = 15;
    //===========xu ly dau cach nhanh ve===============
    uint32_t cntSIP;
    uint8_t tempSIP[16];
    strncpy((char*)tempSIP,(const char*)value->octetString,valueLen); 
    for(cntSIP = 0; cntSIP < 16 ; cntSIP++)
    {
      if(tempSIP[cntSIP] == ' ') tempSIP[cntSIP] = '\0'; 
    }
    tempSIP[15] = '\0';
    //===========xu ly dau cach nhanh ve===============
    //Copy object value
    memset(private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP,0,16);
    strncpy((char*)private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP,(const char*)tempSIP,valueLen); 
    ipv4StringToAddr((const char*)&private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP[0],&tempIP);
    
    if  (parameter.ip_parameter.snmp_ip != revertIP(tempIP))
    {
      // Update the current parameters with the new settings.
      parameter.ip_parameter.snmp_ip = revertIP(tempIP);
      private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SIP = revertIP(parameter.ip_parameter.snmp_ip);
      
      // Yes - save these settings as the defaults.
      working_default_parameter.ip_parameter.snmp_ip = parameter.ip_parameter.snmp_ip;
      
      flag_struct.save_config_flag |=1;  
    }
    
    //      private_mib_base_struct.cfgNetworkGroup.cFTPServerLen = valueLen;
  }
  //siteFTPServer object?
  else if(!strcmp(object->name, "siteFTPServer"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(valueLen > 40)
      return ERROR_BUFFER_OVERFLOW;
    
    //Copy object value
    memset(private_mib_base_struct.cfgNetworkGroup.cFTPServer,0,40);
    strncpy(private_mib_base_struct.cfgNetworkGroup.cFTPServer,(const char*)value->octetString,valueLen);    
    private_mib_base_struct.cfgNetworkGroup.cFTPServerLen = valueLen;
    
    strncpy((char*)&working_default_parameter.ip_parameter.uc_ftp_server[0],(char*)&private_mib_base_struct.cfgNetworkGroup.cFTPServer[0],private_mib_base_struct.cfgNetworkGroup.cFTPServerLen);
    strncpy((char*)&parameter.ip_parameter.uc_ftp_server[0],(char*)&working_default_parameter.ip_parameter.uc_ftp_server[0],private_mib_base_struct.cfgNetworkGroup.cFTPServerLen);
    private_mib_base_struct.cfgNetworkGroup.cFTPServer[private_mib_base_struct.cfgNetworkGroup.cFTPServerLen] ='\0';
    parameter.ip_parameter.uc_ftp_server[private_mib_base_struct.cfgNetworkGroup.cFTPServerLen] ='\0';
    working_default_parameter.ip_parameter.uc_ftp_server[private_mib_base_struct.cfgNetworkGroup.cFTPServerLen] ='\0';
    
    flag_struct.save_config_flag |=1;
  }
  //siteFirmwareUpdate object?
  else if(!strcmp(object->name, "siteFirmwareUpdate"))
  {
    if((value->integer == 0x53)||(value->integer==0x00))
    {
      //Get object value
      entry->siteFirmwareUpdate= value->integer;
      if((config_handler_struct.u8FwUpdateFlag == 0) && (entry->siteFirmwareUpdate == 0x53))
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
        }else{
          config_handler_struct.u8FwUpdateFlag=1;
        }
        
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //siteFWType object?
  else if(!strcmp(object->name, "siteFWType"))
  {
    if((value->integer >= 0) && (value->integer <= 3))
    {
      //Get object value
      private_mib_base_struct.siteGroup.siteFWType = value->integer;
      config_handler_struct.state_update = (STATE_UPDATE_ENUM)private_mib_base_struct.siteGroup.siteFWType;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //siteTrapEnable object?
  else if(!strcmp(object->name, "siteTrapEnable"))
  {
    if((value->integer >= 0) && (value->integer <= 2))
    {
      SYSTEM_CONFIG_STRUCT sConfigParams1;
      
      //Get object value
      private_mib_base_struct.siteGroup.siteTrapEnable = value->integer;
      
      sConfigParams1.ip_parameter.trap_mode = private_mib_base_struct.siteGroup.siteTrapEnable;
      //
      // Did parameters change?
      //
      if  (parameter.ip_parameter.trap_mode != sConfigParams1.ip_parameter.trap_mode)
      {
        //
        // Update the current parameters with the new settings.
        //
        parameter.ip_parameter.trap_mode = sConfigParams1.ip_parameter.trap_mode;
        private_mib_base_struct.cfgNetworkGroup.u8TrapMode = parameter.ip_parameter.trap_mode;
        //
        // Yes - save these settings as the defaults.
        //
        working_default_parameter.ip_parameter.trap_mode = parameter.ip_parameter.trap_mode;
        
        flag_struct.save_config_flag |=1;
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //siteSyncTime object?
  else if(!strcmp(object->name, "siteSyncTime"))
  {
    //        //Make sure the buffer is large enough to hold the entire object
    //        if(40 >= valueLen)
    //        {
    //          pui8Char = &value->octetString[0];        
    //          charIdx = 0;
    //          for (uint8_t i = 0; i < valueLen; i++)
    //          {
    //            setChar = *pui8Char++;
    //            switch (setChar)
    //            {
    //            case '/':
    //            case ':':
    //            case '-':
    //              {
    //                token++;
    //                charIdx = 0;
    //              }
    //              break;
    //            case ' ':
    //              break;
    //            default:
    //              {
    //                buff[token][charIdx]= setChar;
    //                charIdx++;
    //              }
    //              break;
    //            };
    //          }
    //       
    //          SyncTime.hour = atol(&buff[0][0]);
    //          SyncTime.min = atol(&buff[1][0]);
    //          SyncTime.sec = atol(&buff[2][0]);
    //          SyncTime.date = atol(&buff[3][0]);
    //          SyncTime.month = atol(&buff[4][0]);
    //          SyncTime.year = atol(&buff[5][0]);
    //          if((SyncTime.sec > 59)||
    //             (SyncTime.min > 59) || 
    //             (SyncTime.hour > 23) || 
    //             (SyncTime.year > 99)  || 
    //             (SyncTime.month > 12) || 
    //             (SyncTime.date > 31))
    //          {
    //            return ERROR_PARAMETER_OUT_OF_RANGE;
    //          }
    //          token = 0;
    //          charIdx = 0;
    //          memset(&buff, 0, sizeof(buff));
    //          
    //          sprintf(&private_mib_base_struct.cfgNetworkGroup.cSyncTime[0], "%02d:%02d:%02d-%02d/%02d/%02d",
    //                  SyncTime.hour, 
    //                  SyncTime.min, 
    //                  SyncTime.sec, 
    //                  SyncTime.date, 
    //                  SyncTime.month, 
    //                  SyncTime.year);
    //         
    //        
    //            //Copy object value
    //  //          memcpy(entry->siteDCsystemVendor, value->octetString, valueLen);
    //            //Return object length
    //            private_mib_base_struct.cfgNetworkGroup.cSyncTimeLen = 20;
    //            config_handler_struct.isSyncTime = 1;
    //            modbus_manager_struct.SettingCommand = SET_VAC_SYNCTIME;
    //        }
    //        else
    //        {
    //            //Report an error
    //            error = ERROR_BUFFER_OVERFLOW;
    //        }
    return ERROR_OBJECT_NOT_FOUND;
  }
  //set fpt port----------------------------------------------------------------
  else if(!strcmp(object->name, "ftp_port"))
  {
    if((value->integer>=0)&&(value->integer<=65535))
    {
      private_mib_base_struct.cfgNetworkGroup.ftp_port = value->integer;
      
      if(private_mib_base_struct.cfgNetworkGroup.ftp_port != parameter.ip_parameter.ftp_port){
        parameter.ip_parameter.ftp_port                     = private_mib_base_struct.cfgNetworkGroup.ftp_port;
        working_default_parameter.ip_parameter.ftp_port     = private_mib_base_struct.cfgNetworkGroup.ftp_port;
        flag_struct.save_config_flag |= 1;
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //Unknown object?
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;
}

// private mib get site info group----------------------------------------------
error_t privateMibGetSiteInfoGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PrivateMibSiteInfoGroup *entry;
  
  //	Point to the siteInfoGroup entry
  entry = &private_mib_base_struct.siteGroup;
  
  //siteInfoBTSCode object?
  if(!strcmp(object->name, "siteBTSCode"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->siteBTSCodeLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->siteBTSCode, entry->siteBTSCodeLen);
      //Return object length
      *valueLen = entry->siteBTSCodeLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteDCsystemVendor object?
  else if(!strcmp(object->name, "siteDCsystemVendor"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->siteDCsystemVendorLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->siteDCsystemVendor, entry->siteDCsystemVendorLen);
      //Return object length
      *valueLen = entry->siteDCsystemVendorLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteControllerModel object?
  else if(!strcmp(object->name, "siteControllerModel"))
  {
    //Get object value
    value->integer = private_mib_base_struct.siteGroup.siteControllerModel[0];
  }
  //siteCRCStr object?
  else if(!strcmp(object->name, "siteCRCStr"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->siteCRCStrLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->siteCRCStr, entry->siteCRCStrLen);
      //Return object length
      *valueLen = entry->siteCRCStrLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteAmbientTemp object?
  else if(!strcmp(object->name, "siteAmbientTemp"))
  {
    //Get object value
    value->integer = private_mib_base_struct.siteGroup.siteAmbientTemp;
  }
  //siteInfoTrapCounter object?
  else if(!strcmp(object->name, "siteTrapCounter"))
  {
    //Get object value
    value->integer = private_mib_base_struct.siteGroup.siteTrapCounter;
  }
  //siteSystemReset object?
  else if(!strcmp(object->name, "siteSystemReset"))
  {
    //Get object value
    value->integer = private_mib_base_struct.siteGroup.siteSystemReset;
  }
  //siteMACInfo object?
  else if(!strcmp(object->name, "siteMACInfo"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->siteMACInfoLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->siteMACInfo, entry->siteMACInfoLen);
      //Return object length
      *valueLen = entry->siteMACInfoLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteSerialNumber object?
  else if(!strcmp(object->name, "siteSerialNumber"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->siteSerialNumberLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->siteSerialNumber, entry->siteSerialNumberLen);
      //Return object length
      *valueLen = entry->siteSerialNumberLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteSNMPServer object?
  else if(!strcmp(object->name, "siteSNMPServer"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= 16)
    {
      //Copy object value
      memcpy(value->octetString, &private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.ucSIP[0], 16);            
      //Return object length
      *valueLen = 16;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteFTPServer object?
  else if(!strcmp(object->name, "siteFTPServer"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= private_mib_base_struct.cfgNetworkGroup.cFTPServerLen)
    {
      //Copy object value
      memcpy(value->octetString, &private_mib_base_struct.cfgNetworkGroup.cFTPServer[0], private_mib_base_struct.cfgNetworkGroup.cFTPServerLen);
      //Return object length
      *valueLen = private_mib_base_struct.cfgNetworkGroup.cFTPServerLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteFirmwareUpdate object?
  else if(!strcmp(object->name, "siteFirmwareUpdate"))
  {
    //Get object value
    value->integer = private_mib_base_struct.siteGroup.siteFirmwareUpdate;
  }
  //siteFWType object?
  else if(!strcmp(object->name, "siteFWType"))
  {
    private_mib_base_struct.siteGroup.siteFWType = config_handler_struct.state_update;
    //Get object value
    value->integer = private_mib_base_struct.siteGroup.siteFWType;
  }
  //siteDeviceModel object?
  else if(!strcmp(object->name, "siteDeviceModel"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= private_mib_base_struct.siteGroup.siteDevModelLen)
    {
      //Copy object value
      memcpy(value->octetString, private_mib_base_struct.siteGroup.siteDevModel, private_mib_base_struct.siteGroup.siteDevModelLen);
      //Return object length
      *valueLen = private_mib_base_struct.siteGroup.siteDevModelLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //siteTrapEnable object?
  else if(!strcmp(object->name, "siteTrapEnable"))
  {
    //Get object value
    value->integer = private_mib_base_struct.cfgNetworkGroup.u8TrapMode;
  }
  //ftp_port
  else if(!strcmp(object->name, "ftp_port"))
  {
    //Get object value
    value->integer = private_mib_base_struct.cfgNetworkGroup.ftp_port;
  }
  else if(!strcmp(object->name, "eth_type"))
  {
    //Get object value
    value->integer = system_flag_struct.eth_type;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}
//power system=================================================================

//alarm info function==========================================================

//private mib get main alarm group----------------------------------------------
error_t privateMibGetMainAlarmGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PRIVATE_MIB_MAIN_ALARM_GROUP_STRUCT *entry;
  entry = &private_mib_base_struct.main_alarm_group_struct;
  //alarmBattHighTemp object?
  if(!strcmp(object->name, "alarmBattHighTemp"))
  {
    //Get object value
    value->integer = entry->alarm_batt_high_temp;
  }
  //alarmLLVD object?
  else if(!strcmp(object->name, "alarmLLVD"))
  {
    //Get object value
    value->integer = entry->alarm_llvd;
  }
  //alarmBLVD object?
  else if(!strcmp(object->name, "alarmBLVD"))
  {
    //Get object value
    value->integer = entry->alarm_blvd;
  }
  //alarmDCLow object?
  else if(!strcmp(object->name, "alarmDCLow"))
  {
    //Get object value
    value->integer = entry->alarm_dc_Low;
  }
  //alarmACmains object?
  else if(!strcmp(object->name, "alarmACmains"))
  {
    //Get object value
    value->integer = entry->alarm_ac_mains;
  }
  //alarmBattBreakerOpen object?
  else if(!strcmp(object->name, "alarmBattBreakerOpen"))
  {
    //Get object value
    value->integer = entry->alarm_batt_breaker_open;
  }
  //alarmLoadBreakerOpen object?
  else if(!strcmp(object->name, "alarmLoadBreakerOpen"))
  {
    //Get object value
    value->integer = entry->alarm_load_breaker_open;
  }
  //alarmRectACFault object?
  else if(!strcmp(object->name, "alarmRectACFault"))
  {
    //Get object value
    value->integer = entry->alarm_rect_ac_fault;
  }
  //alarmRectNoResp object?
  else if(!strcmp(object->name, "alarmRectNoResp"))
  {
    //Get object value
    value->integer = entry->alarm_rect_no_resp;
  }
  //alarmManualMode object?
  else if(!strcmp(object->name, "alarmManualMode"))
  {
    //Get object value
    value->integer = entry->alarm_manual_mode;
  }
  //alarmACLow object?
  else if(!strcmp(object->name, "alarmACLow"))
  {
    //Get object value
    value->integer = entry->alarm_ac_low;
  }
  //alarmSmoke object?
  else if(!strcmp(object->name, "alarmSmoke"))
  {
    //Get object value
    value->integer = entry->alarm_smoke;
  }
  //alarmEnvDoor object?
  else if(!strcmp(object->name, "alarmEnvDoor"))
  {
    //Get object value
    value->integer = entry->alarm_env_door;
  }
  //alarmLIB object?
  else if(!strcmp(object->name, "alarmLIB"))
  {
    //Get object value
    value->integer = entry->alarm_lib;
  }
  //alarmGen object?
  else if(!strcmp(object->name, "alarmGen"))
  {
    //Get object value
    value->integer = entry->alarm_gen;
  }
  //alarmBM object?
  else if(!strcmp(object->name, "alarmBM"))
  {
    //Get object value
    value->integer = entry->alarm_bm;
  }
  //alarmVAC object?
  else if(!strcmp(object->name, "alarmVAC"))
  {
    //Get object value
    value->integer = entry->alarm_vac;
  }
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get di alarm group-----------------------------------------------
error_t privateMibGetDIAlarmGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PrivateMibDIAlarmGroup *entry;
  entry = &private_mib_base_struct.diAlarmGroup;
  //alarmDI1 object?
  if(!strcmp(object->name, "alarmDI1"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[0];
  }
  //alarmDI2 object?
  else if(!strcmp(object->name, "alarmDI2"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[1];
  }
  //alarmDI3 object?
  else if(!strcmp(object->name, "alarmDI3"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[2];
  }
  //alarmDI4 object?
  else if(!strcmp(object->name, "alarmDI4"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[3];
  }
  //alarmDI5 object?
  else if(!strcmp(object->name, "alarmDI5"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[4];
  }
  //alarmDI6 object?
  else if(!strcmp(object->name, "alarmDI6"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[5];
  }
  //alarmDI7 object?
  else if(!strcmp(object->name, "alarmDI7"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[6];
  }
  //alarmDI8 object?
  else if(!strcmp(object->name, "alarmDI8"))
  {
    //Get object value
    value->integer = entry->alarmDigitalInput[7];
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get conn alarm group----------------------------------------------
error_t privateMibGetConnAlarmGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PRIVATE__MIB_CONNECT_ALARM_GROUP_STRUCT *entry;
  entry = &private_mib_base_struct.connect_alarm_group_struct;
  //alarmLIBConnect object?
  if(!strcmp(object->name, "alarmLIBConnect"))
  {
    //Get object value
    value->integer = entry->alarm_lib_connect;
  }
  //alarmPMUConnect object?
  else if(!strcmp(object->name, "alarmPMUConnect"))
  {
    //Get object value
    value->integer = entry->alarm_pmu_connect;
  }
  //alarmGenConnect object?
  else if(!strcmp(object->name, "alarmGenConnect"))
  {
    //Get object value
    value->integer = entry->alarm_gen_connect;
  }
  //alarmBMConnect object?
  else if(!strcmp(object->name, "alarmBMConnect"))
  {
    //Get object value
    value->integer = entry->alarm_bm_connect;
  }
  //alarmPMConnect object?
  else if(!strcmp(object->name, "alarmPMConnect"))
  {
    //Get object value
    value->integer = entry->alarm_pm_connect;
  }
  //alarmVACConnect object?
  else if(!strcmp(object->name, "alarmVACConnect"))
  {
    //Get object value
    value->integer = entry->alarm_vac_connect;
  }
  //alarmSMCBConnect object?
  else if(!strcmp(object->name, "alarmSMCBConnect"))
  {
    //Get object value
    value->integer = entry->alarm_smcb_connect;
  }
  //alarmFUELConnect object?
  else if(!strcmp(object->name, "alarmFUELConnect"))
  {
    //Get object value
    value->integer = entry->alarm_fuel_connect;
  }
  //alarmIsenseConnect object?
  else if(!strcmp(object->name, "alarmISENSEConnect"))
  {
    //Get object value
    value->integer = entry->alarm_isense_connect;
  }
  //alarm_pm_dc_Connect object?
  else if(!strcmp(object->name, "alarm_pm_dc_Connect")){
    //Get object value
    value->integer = entry->alarm_pmdc_connect;
  }
  //alarm_dpc_fan_Connect object?
  else if(!strcmp(object->name, "alarm_dpc_fan_Connect")){
    //Get object value
    value->integer = entry->alarm_dpc_fan_connect;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//rect info function=====================================================================================

//private mib get rect info entry-----------------------------------------------
error_t privateMibGetRectInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibRectInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 24)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &private_mib_base_struct.rectGroup.rectTable[index - 1];
  //rectIndex object?
  if(!strcmp(object->name, "rectIndex"))
  {
    //Get object value
    value->integer = entry->rectIndex;
  }
  //rectStatus object?
  else if(!strcmp(object->name, "rectStatus"))
  {
    //Get object value
    value->integer = entry->rectStatus;
  }
  //rectOutputCurrent object?
  else if(!strcmp(object->name, "rectOutputCurrent"))
  {
    //Get object value
    value->integer = entry->rectOutputCurrent;
  }
  //rectOutputVoltage object?
  else if(!strcmp(object->name, "rectOutputVoltage"))
  {
    //Get object value
    value->integer = entry->rectOutputVoltage;
  }
  //rectTemp object?
  else if(!strcmp(object->name, "rectTemp"))
  {
    //Get object value
    value->integer = entry->rectTemp;
  }
  //rectType object?
  else if(!strcmp(object->name, "rectType"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->rectTypeLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->rectType, entry->rectTypeLen);
      //Return object length
      *valueLen = entry->rectTypeLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //rectAlarmStatus object?
  else if(!strcmp(object->name, "rectAlarmStatus"))
  {
    //Get object value
    value->integer = entry->rectAlarmStatus;
  }
  //rectSerialNo object?
  else if(!strcmp(object->name, "rectSerialNo"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->rectSerialNoLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->rectSerialNo, entry->rectSerialNoLen);
      //Return object length
      *valueLen = entry->rectSerialNoLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //rectRevisionLevel object?
  else if(!strcmp(object->name, "rectRevisionLevel"))
  {
    //Get object value
    value->integer = entry->rectRevisionLevel;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get next rect info entry------------------------------------------
error_t privateMibGetNextRectInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.rectGroup.rectInstalledRect; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

//ac info function==============================================================

//private mib get ac phase entry-----------------------
error_t privateMibGetAcPhaseEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibAcPhaseEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 3)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &private_mib_base_struct.acPhaseGroup.acPhaseTable[index - 1];
  
  //ifIndex object?
  if(!strcmp(object->name, "acPhaseIndex"))
  {
    //Get object value
    value->integer = entry->acPhaseIndex;
  }
  //acPhaseVolt object?
  else if(!strcmp(object->name, "acPhaseVolt"))
  {
    //Get object value
    value->integer = entry->acPhaseVolt;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get ac phase entry----------------------
error_t privateMibGetNextAcPhaseEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.acPhaseGroup.acPhaseNumber; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

//battery info group===========================================================

// private mib get battery group---------------------------------
error_t privateMibGetBatteryGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PrivateMibBatteryGroup *entry;
  entry = &private_mib_base_struct.batteryGroup;
  
  //battVolt object?
  if(!strcmp(object->name, "battVolt"))
  {
    //Get object value
    value->integer = entry->battVolt;
  }
  //battCurr object?
  else if(!strcmp(object->name, "battCurr"))
  {
    //Get object value
    value->integer = entry->battCurr;
  }
  //battTemp object?
  else if(!strcmp(object->name, "battTemp"))
  {
    //Get object value
    value->integer = entry->battTemp;
  }
  //battBanksNumofBanks object?
  else if(!strcmp(object->name, "battBanksNumofBanks"))
  {
    //Get object value
    value->integer = entry->battBanksNumofBanks;
  }
  //battCapLeft1 object?
  else if(!strcmp(object->name, "battCapLeft1"))
  {
    //Get object value
    value->integer = entry->battCapLeft1;
  }
  //battCapLeft2 object?
  else if(!strcmp(object->name, "battCapLeft2"))
  {
    //Get object value
    value->integer = entry->battCapLeft2;
  }
  //battBrk1Status object?
  else if(!strcmp(object->name, "battBrk1Status"))
  {
    //Get object value
    value->integer = entry->battBrkStatus[0];
  }
  //battBrk2Status object?
  else if(!strcmp(object->name, "battBrk2Status"))
  {
    //Get object value
    value->integer = entry->battBrkStatus[1];
  }
  //battBrk3Status object?
  else if(!strcmp(object->name, "battBrk3Status"))
  {
    //Get object value
    value->integer = entry->battBrkStatus[2];
  }
  //battBrk4Status object?
  else if(!strcmp(object->name, "battBrk4Status"))
  {
    //Get object value
    value->integer = entry->battBrkStatus[3];
  }
  //battCapLeft3 object?
  else if(!strcmp(object->name, "battCapLeft3"))
  {
    //Get object value
    value->integer = entry->battCapLeft3;
  }
  //battCapLeft4 object?
  else if(!strcmp(object->name, "battCapLeft4"))
  {
    //Get object value
    value->integer = entry->battCapLeft4;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}
//load info====================================================================

error_t privateMibGetLoadGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PRIVATE_MIB_LOAD_GROUP_STRUCT *entry;
  
  entry = &private_mib_base_struct.loadGroup;
  //loadCurrent object?
  if(!strcmp(object->name, "loadCurrent"))
  {
    //Get object value
    value->integer = entry->loadCurrent;
  }
  //load1Status object?
  else if(!strcmp(object->name, "load1Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[0];
  }
  //load2Status object?
  else if(!strcmp(object->name, "load2Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[1];
  }
  //load3Status object?
  else if(!strcmp(object->name, "load3Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[2];
  }
  //load4Status object?
  else if(!strcmp(object->name, "load4Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[3];
  }
  //load5Status object?
  else if(!strcmp(object->name, "load5Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[4];
  }
  //load6Status object?
  else if(!strcmp(object->name, "load6Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[5];
  }
  //load7Status object?
  else if(!strcmp(object->name, "load7Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[6];
  }
  //load8Status object?
  else if(!strcmp(object->name, "load8Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[7];
  }
  //load9Status object?
  else if(!strcmp(object->name, "load9Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[8];
  }
  //load10Status object?
  else if(!strcmp(object->name, "load10Status"))
  {
    //Get object value
    value->integer = entry->loadStatus[9];
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//config info function==========================================================

//private mib set cfg bt scu entry------------------------------
error_t privateMibSetCfgBTSCUEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibCfgBTSCUEntry *entry;
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 5)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the cfgBTPlanTestTable entry
  entry = &private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[index - 1];
  
  //cfgBTSCUYear object?
  if(!strcmp(object->name, "cfgBTSCUYear"))
  {      
    if((value->integer >= 2000) && (value->integer <= 2099))
    {
      //Get object value
      entry->cfgBTSCUYear = value->integer;       
      sBattTestInfo.u8SetPlan = 5;       
      setCmd_flag = 1;
      setCmd_mask = SET_BT_SCUTIME;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    } 
  }
  //cfgBTSCUMonth object?
  else if(!strcmp(object->name, "cfgBTSCUMonth"))
  {      
    if((value->integer >= 1) && (value->integer <= 12))
    {
      //Get object value
      entry->cfgBTSCUMonth = value->integer;       
      sBattTestInfo.u8SetPlan = 5;       
      setCmd_flag = 1;
      setCmd_mask = SET_BT_SCUTIME;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }        
  }
  //cfgBTSCUDate object?
  else if(!strcmp(object->name, "cfgBTSCUDate"))
  {      
    if((value->integer >= 1) && (value->integer <= 31))
    {
      //Get object value
      entry->cfgBTSCUDate = value->integer;       
      sBattTestInfo.u8SetPlan = 5;       
      setCmd_flag = 1;
      setCmd_mask = SET_BT_SCUTIME;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //cfgBTSCUHour object?
  else if(!strcmp(object->name, "cfgBTSCUHour"))
  {      
    if(value->integer <= 23)
    {
      //Get object value
      entry->cfgBTSCUHour = value->integer;       
      sBattTestInfo.u8SetPlan = 5;       
      setCmd_flag = 1;
      setCmd_mask = SET_BT_SCUTIME;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //cfgBTSCUMinute object?
  else if(!strcmp(object->name, "cfgBTSCUMinute"))
  {      
    if(value->integer <= 59)
    {
      //Get object value
      entry->cfgBTSCUMinute = value->integer;       
      sBattTestInfo.u8SetPlan = 5;       
      setCmd_flag = 1;
      setCmd_mask = SET_BT_SCUTIME;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //cfgBTSCUSecond object?
  else if(!strcmp(object->name, "cfgBTSCUSecond"))
  {      
    if(value->integer <= 59)
    {
      //Get object value
      entry->cfgBTSCUSecond = value->integer;       
      sBattTestInfo.u8SetPlan = 5;       
      setCmd_flag = 1;
      setCmd_mask = SET_BT_SCUTIME;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  
  //    //ifIndex object?
  //    if(!strcmp(object->name, "configAccessIdCard"))
  //    {
  //        //Make sure the buffer is large enough to hold the entire object
  //        if(valueLen >= sizeof(entry->configAccessIdCard))
  //            return ERROR_BUFFER_OVERFLOW;
  //
  //        //Copy object value
  //        memset(entry->configAccessIdCard,0,sizeof(entry->configAccessIdCard));
  //        memcpy(entry->configAccessIdCard, value->octetString, valueLen);
  //        entry->configAccessIdCardLen = 8;
  ////            for(j=0;j<8;j++)
  ////            {
  ////              sMenu_Variable.u8UserID[index-1][j] = entry->configAccessIdCard[j];
  ////            }
  ////            Save_User_ID(sMenu_Variable.u8UserIDAddr[index-1],entry->configAccessIdCard);
  //    }
  
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get cfg bt scu entry------------------------------
error_t privateMibGetCfgBTSCUEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibCfgBTSCUEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 5)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the cfgBTPlanTestTable entry
  entry = &private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[index - 1];
  
  //cfgBTSCUIndex object?
  if(!strcmp(object->name, "cfgBTSCUIndex"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUIndex;
  }
  //cfgBTSCUYear object?
  else if(!strcmp(object->name, "cfgBTSCUYear"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUYear;
  }
  //cfgBTSCUMonth object?
  else if(!strcmp(object->name, "cfgBTSCUMonth"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUMonth;
  }
  //cfgBTSCUDate object?
  else if(!strcmp(object->name, "cfgBTSCUDate"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUDate;
  }
  //cfgBTSCUHour object?
  else if(!strcmp(object->name, "cfgBTSCUHour"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUHour;
  }
  //cfgBTSCUMinute object?
  else if(!strcmp(object->name, "cfgBTSCUMinute"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUMinute;
  }
  //cfgBTSCUSecond object?
  else if(!strcmp(object->name, "cfgBTSCUSecond"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUSecond;
  }
  //    //configAccessIdCard object?
  //    else if(!strcmp(object->name, "configAccessIdCard"))
  //    {
  //        //Make sure the buffer is large enough to hold the entire object
  //        if(*valueLen >= entry->configAccessIdCardLen)
  //        {
  //            //Copy object value
  //            memcpy(value->octetString, entry->configAccessIdCard, entry->configAccessIdCardLen);
  //            //Return object length
  //            *valueLen = entry->configAccessIdCardLen;
  //        }
  //        else
  //        {
  //            //Report an error
  //            error = ERROR_BUFFER_OVERFLOW;
  //        }
  //    }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get next cfg bt scu entry-------------------------
error_t privateMibGetNextCfgBTSCUEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.cfgBTGroup.cfgBTSCUNumber; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

// private mib set cfg plan test entry------------------------
error_t privateMibSetCfgBTPlanTestEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibCfgBTPlanTestEntry *entry;
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 5)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the cfgBTPlanTestTable entry
  entry = &private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[index - 1];
  
  //cfgBTPlanTestMonth object?
  if(!strcmp(object->name, "cfgBTPlanTestMonth"))
  {      
    if((value->integer >= 1) && (value->integer <= 12))
    {
      //Get object value
      entry->cfgBTPlanTestMonth= value->integer;       
      sBattTestInfo.u8SetPlan = index;       
      setCmd_flag = 1;
      setCmd_mask = SET_BT_PLANTEST;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //cfgBTPlanTestDate object?
  else if(!strcmp(object->name, "cfgBTPlanTestDate"))
  {      
    if((value->integer >= 1) && (value->integer <= 31))
    {
      //Get object value
      entry->cfgBTPlanTestDate= value->integer;
      sBattTestInfo.u8SetPlan = index;
      setCmd_flag = 1;
      setCmd_mask = SET_BT_PLANTEST;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //cfgBTPlanTestHour object?
  else if(!strcmp(object->name, "cfgBTPlanTestHour"))
  {      
    if((value->integer >= 0) && (value->integer <= 23))
    {
      //Get object value
      entry->cfgBTPlanTestHour= value->integer;
      sBattTestInfo.u8SetPlan = index;
      setCmd_flag = 1;
      setCmd_mask = SET_BT_PLANTEST;
      sBattTestInfo.u8CheckSentAll = 1;
    } 
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  
  //    //ifIndex object?
  //    if(!strcmp(object->name, "configAccessIdCard"))
  //    {
  //        //Make sure the buffer is large enough to hold the entire object
  //        if(valueLen >= sizeof(entry->configAccessIdCard))
  //            return ERROR_BUFFER_OVERFLOW;
  //
  //        //Copy object value
  //        memset(entry->configAccessIdCard,0,sizeof(entry->configAccessIdCard));
  //        memcpy(entry->configAccessIdCard, value->octetString, valueLen);
  //        entry->configAccessIdCardLen = 8;
  ////            for(j=0;j<8;j++)
  ////            {
  ////              sMenu_Variable.u8UserID[index-1][j] = entry->configAccessIdCard[j];
  ////            }
  ////            Save_User_ID(sMenu_Variable.u8UserIDAddr[index-1],entry->configAccessIdCard);
  //    }
  
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get cfg bt plan test entry
error_t privateMibGetCfgBTPlanTestEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibCfgBTPlanTestEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 4)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the cfgBTPlanTestTable entry
  entry = &private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[index - 1];
  
  //cfgBTPlanTestIndex object?
  if(!strcmp(object->name, "cfgBTPlanTestIndex"))
  {
    //Get object value
    value->integer = entry->cfgBTPlanTestIndex;
  }
  //cfgBTPlanTestMonth object?
  else if(!strcmp(object->name, "cfgBTPlanTestMonth"))
  {
    //Get object value
    value->integer = entry->cfgBTPlanTestMonth;
  }
  //cfgBTPlanTestDate object?
  else if(!strcmp(object->name, "cfgBTPlanTestDate"))
  {
    //Get object value
    value->integer = entry->cfgBTPlanTestDate;
  }
  //cfgBTPlanTestHour object?
  else if(!strcmp(object->name, "cfgBTPlanTestHour"))
  {
    //Get object value
    value->integer = entry->cfgBTPlanTestHour;
  }
  //    //configAccessIdCard object?
  //    else if(!strcmp(object->name, "configAccessIdCard"))
  //    {
  //        //Make sure the buffer is large enough to hold the entire object
  //        if(*valueLen >= entry->configAccessIdCardLen)
  //        {
  //            //Copy object value
  //            memcpy(value->octetString, entry->configAccessIdCard, entry->configAccessIdCardLen);
  //            //Return object length
  //            *valueLen = entry->configAccessIdCardLen;
  //        }
  //        else
  //        {
  //            //Report an error
  //            error = ERROR_BUFFER_OVERFLOW;
  //        }
  //    }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//private mib get next cfg bt plan test entry----------------------------------
error_t privateMibGetNextCfgBTPlanTestEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.cfgBTGroup.cfgBTPlanTestNumber; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

//private mib set cfg bt group----------------------
error_t privateMibSetCfgBTGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{

  PrivateMibCfgBTGroup *entry;
  
  //	Point to the siteInfoGroup entry
  entry = &private_mib_base_struct.cfgBTGroup;
  switch(TYPE)
  {
  case EMER:
    {
      //cfgBTEndVolt object?
      if(!strcmp(object->name, "cfgBTEndVolt"))
      {      
        if((value->integer >= 431) && (value->integer <= 579))
        {
          //Get object value
          entry->cfgBTEndVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_ENDVOLT;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBTEndCap object?
      else if(!strcmp(object->name, "cfgBTEndCap"))
      {      
        if((value->integer >= 0) && (value->integer <= 1000))
        {
          //Get object value
          entry->cfgBTEndCap= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_ENDCAP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBTEndTime object?
      else if(!strcmp(object->name, "cfgBTEndTime"))
      {      
        if((value->integer >= 5) && (value->integer <= 1440))
        {
          //Get object value
          entry->cfgBTEndTime= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_ENDTIME;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBTPlanTestEn object?
      else if(!strcmp(object->name, "cfgBTPlanTestEn"))
      {      
        if((value->integer == 0) || (value->integer == 1))
        {
          //Get object value
          entry->cfgBTPlanTestEn= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_PLEN;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgTestVoltCfg object?
      else if(!strcmp(object->name, "cfgTestVoltCfg"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgAutoTestDay object?
      else if(!strcmp(object->name, "cfgAutoTestDay"))
      {       
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestStartTime object?
      else if(!strcmp(object->name, "cfgTestStartTime"))
      {      
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestAgissonStartTest object?
      else if(!strcmp(object->name, "cfgTestAgissonStartTest"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestAgissonStopTest object?
      else if(!strcmp(object->name, "cfgTestAgissonStopTest"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
      
    }break;
  case ZTE:
    {
      //cfgBTEndVolt object?
      if(!strcmp(object->name, "cfgBTEndVolt"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgBTEndCap object?
      else if(!strcmp(object->name, "cfgBTEndCap"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgBTEndTime object?
      else if(!strcmp(object->name, "cfgBTEndTime"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgBTPlanTestEn object?
      else if(!strcmp(object->name, "cfgBTPlanTestEn"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestVoltCfg object?
      else if(!strcmp(object->name, "cfgTestVoltCfg"))
      {      
        if((value->integer >= (uint32_t)(private_mib_base_struct.configGroup.cfgLLVDVolt/10) + 10) && (value->integer <= 500))
        {
          //Get object value
          entry->cfgTestVoltCfg= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEST_VOLT;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgAutoTestDay object?
      else if(!strcmp(object->name, "cfgAutoTestDay"))
      {      
        if((value->integer >= 15) && (value->integer <= 365))
        {
          //Get object value
          entry->cfgAutoTestDay= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_AUTO_TEST_DAY;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgTestStartTime object?
      else if(!strcmp(object->name, "cfgTestStartTime"))
      {      
        if((value->integer >= 0) && (value->integer <= 23))
        {
          //Get object value
          entry->cfgTestStartTime= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEST_START_TIME;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgTestAgissonStartTest object?
      else if(!strcmp(object->name, "cfgTestAgissonStartTest"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestAgissonStopTest object?
      else if(!strcmp(object->name, "cfgTestAgissonStopTest"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
    }break;
  case HW:
    {
      //cfgBTEndVolt object?
      if(!strcmp(object->name, "cfgBTEndVolt"))
      {      
        if((value->integer >= 170) && (value->integer <= 220))
        {
          //Get object value
          entry->cfgBTEndVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_ENDVOLT;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBTEndCap object?
      else if(!strcmp(object->name, "cfgBTEndCap"))
      { 
        return ERROR_PARAMETER_OUT_OF_RANGE;          
      }
      //cfgBTEndTime object?
      else if(!strcmp(object->name, "cfgBTEndTime"))
      {      
        if((value->integer >= 1) && (value->integer <= 100))
        {
          //Get object value
          entry->cfgBTEndTime= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_ENDTIME;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBTPlanTestEn object?
      else if(!strcmp(object->name, "cfgBTPlanTestEn"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestVoltCfg object?
      else if(!strcmp(object->name, "cfgTestVoltCfg"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgAutoTestDay object?
      else if(!strcmp(object->name, "cfgAutoTestDay"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestStartTime object?
      else if(!strcmp(object->name, "cfgTestStartTime"))
      {     
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgTestAgissonStartTest object?
      else if(!strcmp(object->name, "cfgTestAgissonStartTest"))
      {      
        if((value->integer == 1))
        {
          setCmd_flag = 1;
          setCmd_mask = START_BT_TEST;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgTestAgissonStopTest object?
      else if(!strcmp(object->name, "cfgTestAgissonStopTest"))
      {      
        if((value->integer == 1))
        {
          setCmd_flag = 1;
          setCmd_mask = STOP_BT_TEST;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
    }break;
  case EMERV21:
    {
      //cfgTestVoltCfg object?
      if(!strcmp(object->name, "cfgTestVoltCfg"))
      {      
        if((value->integer >= 431) && (value->integer <= 579))
        {
          //Get object value
          entry->cfgTestVoltCfg= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEST_VOLT;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }        
      //cfgBTEndCap object?
      else if(!strcmp(object->name, "cfgBTEndCap"))
      {      
        if((value->integer >= 0) && (value->integer <= 1000))
        {
          //Get object value
          entry->cfgBTEndCap= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_ENDCAP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBTEndTime object?
      else if(!strcmp(object->name, "cfgBTEndTime"))
      {      
        if((value->integer >= 5) && (value->integer <= 1440))
        {
          //Get object value
          entry->cfgBTEndTime= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BT_ENDTIME;
        } 
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
    }break;
  case DKD51_BDP:
    {
      //code DKD51 heare
    }break;
    
  default:
    break;
  };
  
  
  //Successful processing
  return NO_ERROR;
}

//private mib get cfg bt group--------------------------------------------------
error_t privateMibGetCfgBTGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PrivateMibCfgBTGroup *entry;
  entry = &private_mib_base_struct.cfgBTGroup;
  
  //cfgBTEndVolt object?
  if(!strcmp(object->name, "cfgBTEndVolt"))
  {
    //Get object value
    value->integer = entry->cfgBTEndVolt;
  }
  //cfgBTEndCap object?
  else if(!strcmp(object->name, "cfgBTEndCap"))
  {
    //Get object value
    value->integer = entry->cfgBTEndCap;
  }
  //cfgBTEndTime object?
  else if(!strcmp(object->name, "cfgBTEndTime"))
  {
    //Get object value
    value->integer = entry->cfgBTEndTime;
  }
  //cfgBTPlanTestNumber object?
  else if(!strcmp(object->name, "cfgBTPlanTestNumber"))
  {
    //Get object value
    value->integer = entry->cfgBTPlanTestNumber;
  }
  //cfgBTPlanTestEn object?
  else if(!strcmp(object->name, "cfgBTPlanTestEn"))
  {
    //Get object value
    value->integer = entry->cfgBTPlanTestEn;
  }
  //cfgBTSCUNumber object?
  else if(!strcmp(object->name, "cfgBTSCUNumber"))
  {
    //Get object value
    value->integer = entry->cfgBTSCUNumber;
  }
  //cfgTestVoltCfg object?
  else if(!strcmp(object->name, "cfgTestVoltCfg"))
  {
    //Get object value
    value->integer = entry->cfgTestVoltCfg;
  }
  //cfgAutoTestDay object?
  else if(!strcmp(object->name, "cfgAutoTestDay"))
  {
    //Get object value
    value->integer = entry->cfgAutoTestDay;
  }
  //cfgTestStartTime object?
  else if(!strcmp(object->name, "cfgTestStartTime"))
  {
    //Get object value
    value->integer = entry->cfgTestStartTime;
  }
  //cfgTestAgissonStartTest object?
  else if(!strcmp(object->name, "cfgTestAgissonStartTest"))
  {
    //Get object value
    value->integer = entry->cfgAgissonBTRespond;
  }
  //cfgTestAgissonStopTest object?
  else if(!strcmp(object->name, "cfgTestAgissonStopTest"))
  {
    //Get object value
    value->integer = entry->cfgAgissonBTRespond;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

// Configinfo function =========================================================

//private mib set config group---------------------------------------
error_t privateMibSetConfigGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit)
{

  PrivateMibConfigGroup *entry;
  
  //	Point to the siteInfoGroup entry
  entry = &private_mib_base_struct.configGroup;
  
  switch(TYPE)
  {
  case EMER:
    {
      //cfgWalkInTimeEn object?
      if(!strcmp(object->name, "cfgWalkInTimeEn"))
      {
        if((value->integer == 0) || (value->integer == 1))
        {
          //Get object value
          entry->cfgWalkInTimeEn = value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_WIT_EN;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgWalkInTimeDuration object?
      else if(!strcmp(object->name, "cfgWalkInTimeDuration"))
      {      
        if((value->integer >= 8) && (value->integer <= 128))
        {
          //Get object value
          entry->cfgWalkInTimeDuration= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_WIT_VAL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgCurrentLimit object?
      else if(!strcmp(object->name, "cfgCurrentLimit"))
      {       
        if((value->integer >= 100) && (value->integer <= 250))
        {
          //Get object value
          entry->cfgCurrentLimit= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_CCL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
        
      }
      //cfgFloatVolt object?
      else if(!strcmp(object->name, "cfgFloatVolt"))
      {      
        if((value->integer >= 4200) && (value->integer <= private_mib_base_struct.configGroup.cfgBoostVolt))
        {
          //Get object value
          entry->cfgFloatVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_FLTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBoostVolt object?
      else if(!strcmp(object->name, "cfgBoostVolt"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgFloatVolt) && (value->integer <= 5800))
        {
          //Get object value
          entry->cfgBoostVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BSTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgLLVDVolt object?
      else if(!strcmp(object->name, "cfgLLVDVolt"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgBLVDVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgDCUnderVolt))
        {
          //Get object value
          entry->cfgLLVDVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_LLVD;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgDCLowVolt object?
      else if(!strcmp(object->name, "cfgDCLowVolt"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgDCUnderVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgDCOverVolt))
        {
          //Get object value
          entry->cfgDCLowVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_DCLOW;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgStartManualTest object?
      else if(!strcmp(object->name, "cfgStartManualTest"))
      {
        //Get object value
        entry->cfgStartManualTest= value->integer;
      }
      //cfgTempCompValue object?
      else if(!strcmp(object->name, "cfgTempCompValue"))
      {      
        if((value->integer >= 0) && (value->integer <= 500))
        {
          //Get object value
          entry->cfgTempCompValue= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEMPCOMP_VAL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBLVDVolt object?
      else if(!strcmp(object->name, "cfgBLVDVolt"))
      {      
        if((value->integer >= 4000) && (value->integer <= private_mib_base_struct.configGroup.cfgLLVDVolt))
        {
          //Get object value
          entry->cfgBLVDVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BLVD;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal"))
      {      
        if((value->integer >= 50) && (value->integer <= 5000))
        {
          //Get object value
          entry->cfgBattCapacityTotal= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgHighMajorTempLevel object?
      else if(!strcmp(object->name, "cfgHighMajorTempLevel"))
      {      
        if((value->integer >= 1000) && (value->integer <= private_mib_base_struct.configGroup.cfgOverMajorTempLevel))
        {
          //Get object value
          entry->cfgHighMajorTempLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_OVERTEMP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgAcLowLevel object?
      else if(!strcmp(object->name, "cfgAcLowLevel"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgAcUnderLevel) && (value->integer <= private_mib_base_struct.configGroup.cfgAcHighLevel))
        {
          //Get object value
          entry->cfgAcLowLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_AC_THRES;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal2 object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal2"))
      {      
        if((value->integer >= 50) && (value->integer <= 5000))
        {
          //Get object value
          entry->cfgBattCapacityTotal2= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP2;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgDCUnderVolt object?
      else if(!strcmp(object->name, "cfgDCUnderVolt"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgLLVDVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgDCLowVolt))
        {
          //Get object value
          entry->cfgDCUnderVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_DCUNDER;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgLLVDEn object?
      else if(!strcmp(object->name, "cfgLLVDEn"))
      {
        if((value->integer == 0) || (value->integer == 1))
        {
          //Get object value
          entry->cfgLLVDEn = value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_LLVD_EN;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBLVDEn object?
      else if(!strcmp(object->name, "cfgBLVDEn"))
      {
        if((value->integer == 0) || (value->integer == 1))
        {
          //Get object value
          entry->cfgBLVDEn = value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BLVD_EN;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
      
      
    }break;
  case ZTE:
    {
      //cfgWalkInTimeEn object?
      if(!strcmp(object->name, "cfgWalkInTimeEn"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;          
      }
      //cfgWalkInTimeDuration object?
      else if(!strcmp(object->name, "cfgWalkInTimeDuration"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgCurrentLimit object?
      else if(!strcmp(object->name, "cfgCurrentLimit"))
      {      
        if((value->integer >= 10) && (value->integer <= 400))
        {
          //Get object value
          entry->cfgCurrentLimit= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_CCL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgFloatVolt object?
      else if(!strcmp(object->name, "cfgFloatVolt"))
      {
        if((value->integer >= 4200) && (value->integer <= 5800))
        {  
          if((value->integer >= private_mib_base_struct.configGroup.cfgDCLowVolt + 100) && (value->integer <= private_mib_base_struct.configGroup.cfgBoostVolt))
          {           
            //Get object value
            entry->cfgFloatVolt= value->integer;
            setCmd_flag = 1;
            setCmd_mask = SET_FLTVOL;                        
          } 
          else
          {
            return ERROR_PARAMETER_OUT_OF_RANGE;
          }          
        }
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBoostVolt object?
      else if(!strcmp(object->name, "cfgBoostVolt"))
      {
        if((value->integer >= 4200) && (value->integer <= 5800))
        {      
          if(value->integer >= private_mib_base_struct.configGroup.cfgFloatVolt)
          {
            //Get object value
            entry->cfgBoostVolt= value->integer;
            setCmd_flag = 1;
            setCmd_mask = SET_BSTVOL;
          } 
          else
          {
            return ERROR_PARAMETER_OUT_OF_RANGE;
          }         
        }
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgLLVDVolt object?
      else if(!strcmp(object->name, "cfgLLVDVolt"))
      {
        if((value->integer >= 3800) && (value->integer <= 4900))
        {
          if (private_mib_base_struct.configGroup.cfgDCLowVolt <= private_mib_base_struct.configGroup.cfgBattTestVolt)
          {
            if((value->integer >= private_mib_base_struct.configGroup.cfgBLVDVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgDCLowVolt - 100))
            {
              //Get object value
              entry->cfgLLVDVolt= value->integer;
              setCmd_flag = 1;
              setCmd_mask = SET_LLVD;            
            } 
            else
            {
              return ERROR_PARAMETER_OUT_OF_RANGE;
            }
          }
          else
          {
            if((value->integer >= private_mib_base_struct.configGroup.cfgBLVDVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgBattTestVolt - 100))
            {               
              //Get object value
              entry->cfgLLVDVolt= value->integer;
              setCmd_flag = 1;
              setCmd_mask = SET_LLVD;               
            } 
            else
            {
              return ERROR_PARAMETER_OUT_OF_RANGE;
            }
          }
        }
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }     
      }
      //cfgDCLowVolt object?
      else if(!strcmp(object->name, "cfgDCLowVolt"))
      {
        if (private_mib_base_struct.configGroup.cfgFloatVolt <= 5300)
        {            
          if((value->integer >= private_mib_base_struct.configGroup.cfgLLVDVolt + 100) && (value->integer <= private_mib_base_struct.configGroup.cfgFloatVolt - 100))
          {
            //Get object value
            entry->cfgDCLowVolt= value->integer;
            setCmd_flag = 1;
            setCmd_mask = SET_DCLOW;
          } 
          else
          {
            return ERROR_PARAMETER_OUT_OF_RANGE;
          }
        }
        else 
        {            
          if((value->integer >= private_mib_base_struct.configGroup.cfgLLVDVolt + 100) && (value->integer <= 5200))
          {
            //Get object value
            entry->cfgDCLowVolt= value->integer;
            setCmd_flag = 1;
            setCmd_mask = SET_DCLOW;
          } 
          else
          {
            return ERROR_PARAMETER_OUT_OF_RANGE;
          }
        }
      }
      //cfgStartManualTest object?
      else if(!strcmp(object->name, "cfgStartManualTest"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
        //Get object value
        //            entry->cfgStartManualTest= value->integer;
      }
      //cfgTempCompValue object?
      else if(!strcmp(object->name, "cfgTempCompValue"))
      {      
        if((value->integer >= 0) && (value->integer <= 8))
        {
          //Get object value
          entry->cfgTempCompValue= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEMPCOMP_VAL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBLVDVolt object?
      else if(!strcmp(object->name, "cfgBLVDVolt"))
      {      
        if((value->integer >= 3800) && (value->integer <= private_mib_base_struct.configGroup.cfgLLVDVolt))
        {
          //Get object value
          entry->cfgBLVDVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BLVD;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal"))
      {      
        if((value->integer >= 0) && (value->integer <= 9990))
        {
          //Get object value
          entry->cfgBattCapacityTotal= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgHighMajorTempLevel object?
      else if(!strcmp(object->name, "cfgHighMajorTempLevel"))
      {      
        if((value->integer >= 3000) && (value->integer <= 6000))
        {
          //Get object value
          entry->cfgHighMajorTempLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_OVERTEMP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgAcLowLevel object?
      else if(!strcmp(object->name, "cfgAcLowLevel"))
      {      
        if((value->integer >= 80) && (value->integer <= 200))
        {
          //Get object value
          entry->cfgAcLowLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_AC_THRES;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal2 object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal2"))
      {      
        if((value->integer >= 0) && (value->integer <= 9990))
        {
          //Get object value
          entry->cfgBattCapacityTotal2= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP2;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgDCUnderVolt object?
      else if(!strcmp(object->name, "cfgDCUnderVolt"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgLLVDEn object?
      else if(!strcmp(object->name, "cfgLLVDEn"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgBLVDEn object?
      else if(!strcmp(object->name, "cfgBLVDEn"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
    }break;
  case HW:
    {
      //cfgWalkInTimeEn object?
      if(!strcmp(object->name, "cfgWalkInTimeEn"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgWalkInTimeDuration object?
      else if(!strcmp(object->name, "cfgWalkInTimeDuration"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgCurrentLimit object?
      else if(!strcmp(object->name, "cfgCurrentLimit"))
      {      
        if((value->integer >= 50) && (value->integer <= 250))
        {
          //Get object value
          entry->cfgCurrentLimit= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_CCL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgFloatVolt object?
      else if(!strcmp(object->name, "cfgFloatVolt"))
      {      
        if((value->integer > private_mib_base_struct.configGroup.cfgDCLowVolt + 200) && (value->integer < private_mib_base_struct.configGroup.cfgBoostVolt))
        {
          //Get object value
          entry->cfgFloatVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_FLTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBoostVolt object?
      else if(!strcmp(object->name, "cfgBoostVolt"))
      {      
        if((value->integer > private_mib_base_struct.configGroup.cfgFloatVolt) && (value->integer <= 5760))
        {
          //Get object value
          entry->cfgBoostVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BSTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgLLVDVolt object?
      else if(!strcmp(object->name, "cfgLLVDVolt"))
      {      
        if((value->integer > private_mib_base_struct.configGroup.cfgBLVDVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgDCLowVolt))
        {
          //Get object value
          entry->cfgLLVDVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_LLVD;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgDCLowVolt object?
      else if(!strcmp(object->name, "cfgDCLowVolt"))
      {      
        if((value->integer > private_mib_base_struct.configGroup.cfgLLVDVolt) && (value->integer < private_mib_base_struct.configGroup.cfgFloatVolt - 200))
        {
          //Get object value
          entry->cfgDCLowVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_DCLOW;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgStartManualTest object?
      else if(!strcmp(object->name, "cfgStartManualTest"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
        //Get object value
        //            entry->cfgStartManualTest= value->integer;
      }
      //cfgTempCompValue object?
      else if(!strcmp(object->name, "cfgTempCompValue"))
      {      
        if((value->integer >= 0) && (value->integer <= 500))
        {
          //Get object value
          entry->cfgTempCompValue= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEMPCOMP_VAL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBLVDVolt object?
      else if(!strcmp(object->name, "cfgBLVDVolt"))
      {      
        if((value->integer >= 3500) && (value->integer < private_mib_base_struct.configGroup.cfgLLVDVolt))
        {
          //Get object value
          entry->cfgBLVDVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BLVD;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal"))
      {      
        if((value->integer >= 5) && (value->integer <= 1000))
        {
          //Get object value
          entry->cfgBattCapacityTotal= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgHighMajorTempLevel object?
      else if(!strcmp(object->name, "cfgHighMajorTempLevel"))
      {      
        if((value->integer > entry->cfgLowTempLevel) && (value->integer <= 10000))
        {
          //Get object value
          entry->cfgHighMajorTempLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_OVERTEMP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgAcLowLevel object?
      else if(!strcmp(object->name, "cfgAcLowLevel"))
      {      
        if((value->integer >= 60) && (value->integer <= 299))
        {
          //Get object value
          entry->cfgAcLowLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_AC_THRES;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal2 object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal2"))
      {      
        if((value->integer >= 5) && (value->integer <= 1000))
        {
          //Get object value
          entry->cfgBattCapacityTotal2= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP2;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgDCUnderVolt object?
      else if(!strcmp(object->name, "cfgDCUnderVolt"))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      //cfgLLVDEn object?
      else if(!strcmp(object->name, "cfgLLVDEn"))
      {
        if((value->integer == 0) || (value->integer == 1))
        {
          //Get object value
          entry->cfgLLVDEn = value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_LLVD_EN;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBLVDEn object?
      else if(!strcmp(object->name, "cfgBLVDEn"))
      {
        if((value->integer == 0) || (value->integer == 1))
        {
          //Get object value
          entry->cfgBLVDEn = value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BLVD_EN;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
    }break;
  case EMERV21:
    {     
      if(!strcmp(object->name, "cfgCurrentLimit"))
      {      
        if((value->integer >= 100) && (value->integer <= 250))
        {
          //Get object value
          entry->cfgCurrentLimit= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_CCL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgFloatVolt object?
      else if(!strcmp(object->name, "cfgFloatVolt"))
      {      
        if((value->integer >= 4200) && (value->integer <= private_mib_base_struct.configGroup.cfgBoostVolt))
        {
          //Get object value
          entry->cfgFloatVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_FLTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBoostVolt object?
      else if(!strcmp(object->name, "cfgBoostVolt"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgFloatVolt) && (value->integer <= 5800))
        {
          //Get object value
          entry->cfgBoostVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BSTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgLLVDVolt object?
      else if(!strcmp(object->name, "cfgLLVDVolt"))
      {      
        if((value->integer > private_mib_base_struct.configGroup.cfgBLVDVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgDCLowVolt))
        {
          //Get object value
          entry->cfgLLVDVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_LLVD;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgDCLowVolt object?
      else if(!strcmp(object->name, "cfgDCLowVolt"))
      {      
        if((value->integer > private_mib_base_struct.configGroup.cfgLLVDVolt) && (value->integer < private_mib_base_struct.configGroup.cfgDCOverVolt))
        {
          //Get object value
          entry->cfgDCLowVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_DCLOW;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }//cfgTempCompValue object?
      else if(!strcmp(object->name, "cfgTempCompValue"))
      {      
        if((value->integer >= 0) && (value->integer <= 500))
        {
          //Get object value
          entry->cfgTempCompValue= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEMPCOMP_VAL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBLVDVolt object?
      else if(!strcmp(object->name, "cfgBLVDVolt"))
      {      
        if((value->integer >= 4000) && (value->integer < private_mib_base_struct.configGroup.cfgLLVDVolt))
        {
          //Get object value
          entry->cfgBLVDVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BLVD;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal"))
      {      
        if((value->integer >= 20) && (value->integer <= 5000))
        {
          //Get object value
          entry->cfgBattCapacityTotal= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgHighMajorTempLevel object?
      else if(!strcmp(object->name, "cfgHighMajorTempLevel"))
      {      
        if((value->integer > 1000) && (value->integer <= 5000))
        {
          //Get object value
          entry->cfgHighMajorTempLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_OVERTEMP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgAcLowLevel object?
      else if(!strcmp(object->name, "cfgAcLowLevel"))
      {      
        if((value->integer >= 80) && (value->integer <= private_mib_base_struct.configGroup.cfgAcHighLevel))
        {
          //Get object value
          entry->cfgAcLowLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_AC_THRES;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
    }break;
  case DKD51_BDP:
    {
      //cfgCurrentLimit dpc A object?
      if (!strcmp(object->name, "cfgCurrentLimit_dpc_A")){
        if(private_mib_base_struct.siteGroup.siteControllerModel[0] == 4){
          if((value->integer >= 1000) && (value->integer <= 99900))
          {
            entry->cfgCurrentlimitA = value->integer;
            setCmd_flag = 1;
            setCmd_mask = SET_CCL;
          }
          else
          {
            return ERROR_PARAMETER_OUT_OF_RANGE;
          }
          
        }
        
      }
      //cfgFloatVolt object?
      else if(!strcmp(object->name, "cfgFloatVolt"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgDCLowVolt ) && (value->integer <= private_mib_base_struct.configGroup.cfgBoostVolt))
        {
          //Get object value
          entry->cfgFloatVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_FLTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBoostVolt object?
      else if(!strcmp(object->name, "cfgBoostVolt"))
      {      
        if((value->integer >= private_mib_base_struct.configGroup.cfgFloatVolt) && (value->integer <= private_mib_base_struct.configGroup.cfgDCOverVolt))
        {
          //Get object value
          entry->cfgBoostVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BSTVOL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgDCLowVolt object?
      else if(!strcmp(object->name, "cfgDCLowVolt"))
      {      
        if((value->integer >= 5000) && (value->integer <= private_mib_base_struct.configGroup.cfgFloatVolt))
        {
          //Get object value
          entry->cfgDCLowVolt= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_DCLOW;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }//cfgTempCompValue object?
      else if(!strcmp(object->name, "cfgTempCompValue"))
      {      
        if((value->integer >= 1) && (value->integer <= 120))
        {
          //Get object value
          entry->cfgTempCompValue= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_TEMPCOMP_VAL;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal"))
      {      
        if((value->integer >= 0) && (value->integer <= 600))
        {
          //Get object value
          entry->cfgBattCapacityTotal= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal2 object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal2"))
      {      
        if((value->integer >= 0) && (value->integer <= 600))
        {
          //Get object value
          entry->cfgBattCapacityTotal2= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP2;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal3 object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal3"))
      {      
        if((value->integer >= 0) && (value->integer <= 600))
        {
          //Get object value
          entry->cfgBattCapacityTotal3= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP3;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgBattCapacityTotal4 object?
      else if(!strcmp(object->name, "cfgBattCapacityTotal4"))
      {      
        if((value->integer >= 0) && (value->integer <= 600))
        {
          //Get object value
          entry->cfgBattCapacityTotal4= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_BATTSTDCAP4;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //cfgAcLowLevel object?
      else if(!strcmp(object->name, "cfgAcLowLevel"))
      {      
        if((value->integer >= 154) && (value->integer <= 230))
        {
          //Get object value
          entry->cfgAcLowLevel= value->integer;
          setCmd_flag = 1;
          setCmd_mask = SET_AC_THRES;
        } 
        else
        {
          return ERROR_PARAMETER_OUT_OF_RANGE;
        }
      }
      //Unknown object?
      else
      {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
      }
      
    }break;
    
    
  default:
    break;
  };    
  
  //Successful processing
  return NO_ERROR;
}

//private mib get config group---------------------------------------
error_t privateMibGetConfigGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PrivateMibConfigGroup *entry;
  entry = &private_mib_base_struct.configGroup;
  
  //cfgWalkInTimeEn object?
  if(!strcmp(object->name, "cfgWalkInTimeEn"))
  {
    //Get object value
    value->integer = entry->cfgWalkInTimeEn;
  }
  //cfgWalkInTimeDuration object?
  else if(!strcmp(object->name, "cfgWalkInTimeDuration"))
  {
    //Get object value
    value->integer = entry->cfgWalkInTimeDuration;
  }
  //cfgCurrentLimit object?
  else if(!strcmp(object->name, "cfgCurrentLimit"))
  {
    //Get object value
    value->integer = entry->cfgCurrentLimit;
  }
  //cfgFloatVolt object?
  else if(!strcmp(object->name, "cfgFloatVolt"))
  {
    //Get object value
    value->integer = entry->cfgFloatVolt;
  }
  //cfgBoostVolt object?
  else if(!strcmp(object->name, "cfgBoostVolt"))
  {
    //Get object value
    value->integer = entry->cfgBoostVolt;
  }
  //cfgLLVDVolt object?
  else if(!strcmp(object->name, "cfgLLVDVolt"))
  {
    //Get object value
    value->integer = entry->cfgLLVDVolt;
  }
  //cfgDCLowVolt object?
  else if(!strcmp(object->name, "cfgDCLowVolt"))
  {
    //Get object value
    value->integer = entry->cfgDCLowVolt;
  }
  //cfgStartManualTest object?
  else if(!strcmp(object->name, "cfgStartManualTest"))
  {
    //Get object value
    value->integer = entry->cfgStartManualTest;
  }
  //cfgTempCompValue object?
  else if(!strcmp(object->name, "cfgTempCompValue"))
  {
    //Get object value
    value->integer = entry->cfgTempCompValue;
  }
  //cfgBLVDVolt object?
  else if(!strcmp(object->name, "cfgBLVDVolt"))
  {
    //Get object value
    value->integer = entry->cfgBLVDVolt;
  }
  //cfgBattCapacityTotal object?
  else if(!strcmp(object->name, "cfgBattCapacityTotal"))
  {
    //Get object value
    value->integer = entry->cfgBattCapacityTotal;
  }
  //cfgHighMajorTempLevel object?
  else if(!strcmp(object->name, "cfgHighMajorTempLevel"))
  {
    //Get object value
    value->integer = entry->cfgHighMajorTempLevel;
  }
  //cfgAcLowLevel object?
  else if(!strcmp(object->name, "cfgAcLowLevel"))
  {
    //Get object value
    value->integer = entry->cfgAcLowLevel;
  }
  //cfgBattCapacityTotal2 object?
  else if(!strcmp(object->name, "cfgBattCapacityTotal2"))
  {
    //Get object value
    value->integer = entry->cfgBattCapacityTotal2;
  }
  //cfgDCUnderVolt object?
  else if(!strcmp(object->name, "cfgDCUnderVolt"))
  {
    //Get object value
    value->integer = entry->cfgDCUnderVolt;
  }
  //cfgLLVDEn object?
  else if(!strcmp(object->name, "cfgLLVDEn"))
  {
    //Get object value
    value->integer = entry->cfgLLVDEn;
  }
  //cfgBLVDEn object?
  else if(!strcmp(object->name, "cfgBLVDEn"))
  {
    //Get object value
    value->integer = entry->cfgBLVDEn;
  }
  //current limit dpc A object?
  else if(!strcmp(object->name, "cfgCurrentLimit_dpc_A"))
  {
    //Get object value
    value->integer = entry->cfgCurrentlimitA;
  }
  //cfgBattCapacityTotal3 object?
  else if(!strcmp(object->name, "cfgBattCapacityTotal3"))
  {
    //Get object value
    value->integer = entry->cfgBattCapacityTotal3;
  }
  //cfgBattCapacityTotal4 object?
  else if(!strcmp(object->name, "cfgBattCapacityTotal4"))
  {
    //Get object value
    value->integer = entry->cfgBattCapacityTotal4;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}


//vac info fuction=======================================================================================

//private mib set vac group----------------------------------------------------
error_t private_mib_set_vac_group(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit){
  error_t error = NO_ERROR;
  
  PRIVATE_MIB_VAC_GROUP_STRUCT *entry;
  entry = &private_mib_base_struct.vac_group_struct;
  if(!strcmp(object->name, "vac_installed_vac"))
  {
    if((value->integer >= 0) && (value->integer <= 2))
    {
      //Get object value
      entry->vac_installed_vac  = value->integer; 
      if (entry->vac_installed_vac != modbus_manager_struct.number_of_vac)
      {
        modbus_manager_struct.number_of_vac                      = entry->vac_installed_vac;
        parameter.device_number[TOTAL_VAC_INDEX]                 = modbus_manager_struct.number_of_vac;
        working_default_parameter.device_number[TOTAL_VAC_INDEX] =  parameter.device_number[TOTAL_VAC_INDEX];
        flag_struct.save_config_flag |=1;
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return error;
}

//private mib set vac info entry-----------------------------------------------
error_t private_mib_set_vac_info_entry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen, bool_t commit){
  error_t error = NO_ERROR;
  size_t  n;
  uint_t  index;
  uint_t  j;
  MODBUS_PARAMETER_STRUCT modbus_dev_struct[3];
  uint8_t char_idx      = 0;
  uint8_t set_char      = 0;
  uint8_t token         = 0;
  uint8_t buff[6][4];
  uint8_t const *pui8_char;
  
  PRIVATE_MIB_VAC_INFO_ENTRY_STRUCT *entry;
  
  n = object->oidLen;
  
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  
  if(error) return error;
  
  if(n != oidLen) return ERROR_INSTANCE_NOT_FOUND;
  
  if(index < 1 || index > 2)return ERROR_INSTANCE_NOT_FOUND;
  
  entry = &private_mib_base_struct.vac_group_struct.vac_table_struct[index - 1];
  
  //vac_sys_mode object---------------------------------------
  if(!strcmp(object->name, "vac_sys_mode"))
  {
    if((value->integer >= 0) && (value->integer <= 2))
    {
      //Get object value
      entry->vac_sys_mode                  = value->integer;
      entry->vac_sys_mode_write            = value->integer;
      modbus_manager_struct.setting_cmd    = SET_VAC_SYSMODE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_fan_speed_mode object-------------------------------
  else if(!strcmp(object->name, "vac_fan_speed_mode"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->vac_fan_speed_mode           = value->integer;
      entry->vac_fan_speed_mode_write     = value->integer;
      modbus_manager_struct.setting_cmd   = SET_VAC_FANSPEEDMODE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_fan_start_temp object--------------------------------
  else if(!strcmp(object->name, "vac_fan_start_temp"))
  {
    if((value->integer >= 200) && (value->integer <= 300))
    {
      //Get object value
      entry->vac_fan_start_temp         = value->integer;
      entry->vac_fan_start_temp_write   = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_FANSTARTTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_set_temp object------------------------------------
  else if(!strcmp(object->name, "vac_set_temp"))
  {
    if((value->integer >= 200) && (value->integer <= 400))
    {
      //Get object value
      entry->vac_set_temp               = value->integer;
      entry->vac_set_temp_write         = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_SETTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_pid_offset_temp object----------------------------
  else if(!strcmp(object->name, "vac_pid_offset_temp"))
  {
    if((value->integer >= 10) && (value->integer <= 100))
    {
      //Get object value
      entry->vac_pid_offset_temp        = value->integer;
      entry->vac_pid_offset_temp_write  = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_PIDOFFSETTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_fan_min_speed object-----------------------------
  else if(!strcmp(object->name, "vac_fan_min_speed"))
  {
    if((value->integer >= 0) && (value->integer <= 50))
    {
      //Get object value
      entry->vac_fan_min_speed          = value->integer;
      entry->vac_fan_min_speed_write    = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_FANMINSPEED;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_fan_max_speed object------------------------------
  else if(!strcmp(object->name, "vac_fan_max_speed"))
  {
    if((value->integer >= 70) && (value->integer <= 100))
    {
      //Get object value
      entry->vac_fan_max_speed           = value->integer;
      entry->vac_fan_max_speed_write     = value->integer;
      modbus_manager_struct.setting_cmd  = SET_VAC_FANMAXSPEED;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_filter_stuck_temp object------------------------
  else if(!strcmp(object->name, "vac_filter_stuck_temp"))
  {
    if((value->integer >= 10) && (value->integer <= 200))
    {
      //Get object value
      entry->vac_filter_stuck_temp       = value->integer;
      entry->vac_filter_stuck_temp_write = value->integer;
      modbus_manager_struct.setting_cmd  = SET_VAC_FILTERSTUCKTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_night_mode_en object--------------------------
  else if(!strcmp(object->name, "vac_night_mode_en"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->vac_night_mode_en           = value->integer;
      entry->vac_night_mode_en_write    = value->integer;
      modbus_manager_struct.setting_cmd  = SET_VAC_NIGHTMODEEN;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_night_mode_start object-----------------------
  else if(!strcmp(object->name, "vac_night_mode_start"))
  {
    if((value->integer >= 0) && (value->integer <= 23))
    {
      //Get object value
      entry->vac_night_mode_start        = value->integer;
      entry->vac_night_mode_start_write  = value->integer;
      modbus_manager_struct.setting_cmd  = SET_VAC_NIGHTMODESTART;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_night_mode_end object-------------------------
  else if(!strcmp(object->name, "vac_night_mode_end"))
  {
    if((value->integer >= 0) && (value->integer <= 23))
    {
      //Get object value
      entry->vac_night_mode_end          = value->integer;
      entry->vac_night_mode_end_write    = value->integer;
      modbus_manager_struct.setting_cmd  = SET_VAC_NIGHTMODEEND;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_night_max_speed object-----------------------
  else if(!strcmp(object->name, "vac_night_max_speed"))
  {
    if((value->integer >= 50) && (value->integer <= 100))
    {
      //Get object value
      entry->vac_night_max_speed        = value->integer;
      entry->vac_night_max_speed_write  = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_NIGHTMAXSPEED;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_manual_mode object---------------------------
  else if(!strcmp(object->name, "vac_manual_mode"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->vac_manual_mode            = value->integer;
      entry->vac_manual_mode_write      = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_MANUALMODE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_manual_max_speed object--------------------------
  else if(!strcmp(object->name, "vac_manual_max_speed"))
  {
    if((value->integer >= 0) && (value->integer <= 100))
    {
      //Get object value
      entry->vac_manual_max_speed          = value->integer;
      entry->vac_manual_max_speed_write    = value->integer;
      modbus_manager_struct.setting_cmd    = SET_VAC_MANUALMAXSPEED;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_in_max_temp object--------------------------------
  else if(!strcmp(object->name, "vac_in_max_temp"))
  {
    if((value->integer >= 300) && (value->integer <= 600))
    {
      //Get object value
      entry->vac_in_max_temp             = value->integer;
      entry->vac_in_max_temp_write       = value->integer;
      modbus_manager_struct.setting_cmd  = SET_VAC_INMAXTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_ex_max_temp object-------------------------------
  else if(!strcmp(object->name, "vac_ex_max_temp"))
  {
    if((value->integer >= 300) && (value->integer <= 600))
    {
      //Get object value
      entry->vac_ex_max_temp            = value->integer;
      entry->vac_ex_max_temp_write      = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_EXMAXTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_frost_max_temp object------------------------------------
  else if(!strcmp(object->name, "vac_frost_max_temp"))
  {
    if((value->integer >= 200) && (value->integer <= 500))
    {
      //Get object value
      entry->vac_frost_max_temp          = value->integer;
      entry->vac_frost_max_temp_write    = value->integer;
      modbus_manager_struct.setting_cmd  = SET_VAC_FROSTMAXTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_in_min_temp object------------------------------------
  else if(!strcmp(object->name, "vac_in_min_temp"))
  {
    if((value->integer >= -100) && (value->integer <= 200))
    {
      //Get object value
      entry->vac_in_min_temp            = value->integer;
      entry->vac_in_min_temp_write      = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_INMINTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_ex_min_temp object---------------------------------
  else if(!strcmp(object->name, "vac_ex_min_temp"))
  {
    if((value->integer >= -100) && (value->integer <= 200))
    {
      //Get object value
      entry->vac_ex_min_temp= value->integer;
      entry->vac_ex_min_temp_write= value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_EXMINTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_frost_min_temp object--------------------------------
  else if(!strcmp(object->name, "vac_frost_min_temp"))
  {
    if((value->integer >= -100) && (value->integer <= 200))
    {
      //Get object value
      entry->vac_frost_min_temp         = value->integer;
      entry->vac_frost_min_temp_write   = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_FROSTMINTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_min_out_temp object-------------------------------
  else if(!strcmp(object->name, "vac_min_out_temp"))
  {
    if((value->integer >= -100) && (value->integer <= 100))
    {
      //Get object value
      entry->vac_min_out_temp           = value->integer;
      entry->vac_min_out_temp_write     = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_MINOUTTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_delta_temp object--------------------------------
  else if(!strcmp(object->name, "vac_delta_temp"))
  {
    if((value->integer >= 0) && (value->integer <= 100))
    {
      //Get object value
      entry->vac_delta_temp             = value->integer;
      entry->vac_delta_temp             = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_DELTATEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_panic_temp object--------------------------------
  else if(!strcmp(object->name, "vac_panic_temp"))
  {
    if((value->integer >= 300) && (value->integer <= 500))
    {
      //Get object value
      entry->vac_panic_temp             = value->integer;
      entry->vac_panic_temp_write       = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_PANICTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //    //vacACU1OnTemp object?
  //    else if(!strcmp(object->name, "vacACU1OnTemp"))
  //    {
  //      if((value->integer >= 300) && (value->integer <= 400))
  //      {
  //        //Get object value
  //        entry->vacACU1OnTemp= value->integer;
  //        modbus_manager_struct.setting_cmd = SET_VAC_ACU1ONTEMP;
  //      }
  //      else
  //      {
  //        return ERROR_PARAMETER_OUT_OF_RANGE;
  //      }
  //    }
  //    //vacACU2OnTemp object?
  //    else if(!strcmp(object->name, "vacACU2OnTemp"))
  //    {
  //      if((value->integer >= 300) && (value->integer <= 400))
  //      {
  //        //Get object value
  //        entry->vacACU2OnTemp= value->integer;
  //        modbus_manager_struct.setting_cmd = SET_VAC_ACU2ONTEMP;
  //      }
  //      else
  //      {
  //        return ERROR_PARAMETER_OUT_OF_RANGE;
  //      }
  //    }
  //    //vacACU2En object?
  //    else if(!strcmp(object->name, "vacACU2En"))
  //    {
  //      if((value->integer >= 1) && (value->integer <= 2))
  //      {
  //        //Get object value
  //        entry->vacACU2En= value->integer;
  //        modbus_manager_struct.setting_cmd = SET_VAC_ACU2EN;
  //      }
  //      else
  //      {
  //        return ERROR_PARAMETER_OUT_OF_RANGE;
  //      }
  //    }
  //vac_air_con1_model object-----------------------------------
  else if(!strcmp(object->name, "vac_air_con1_model"))
  {
    if((value->integer >= 0) && (value->integer <= 10))
    {
      //Get object value
      entry->vac_air_con1_model= value->integer;
      entry->vac_air_con1_model_write= value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCON1MODEL;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_con1_type object----------------------------------
  else if(!strcmp(object->name, "vac_air_con1_type"))
  {
    if((value->integer >= 0) && (value->integer <= 99))
    {
      //Get object value
      entry->vac_air_con1_type          = value->integer;
      entry->vac_air_con1_model_write   = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCON1TYPE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_con2_model object----------------------------------
  else if(!strcmp(object->name, "vac_air_con2_model"))
  {
    if((value->integer >= 0) && (value->integer <= 10))
    {
      //Get object value
      entry->vac_air_con2_model         = value->integer;
      entry->vac_air_con2_model_write   = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCON2MODEL;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_con2_type object---------------------------------
  else if(!strcmp(object->name, "vac_air_con2_type"))
  {
    if((value->integer >= 0) && (value->integer <= 99))
    {
      //Get object value
      entry->vac_air_con2_type          = value->integer;
      entry->vac_air_con2_type_write    = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCON2TYPE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_con_on_off object-------------------------------
  else if(!strcmp(object->name, "vac_air_con_on_off"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->vac_air_con_on_off         = value->integer;
      entry->vac_air_con_on_off_write   = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCONONOFF;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_con_mode object-------------------------------
  else if(!strcmp(object->name, "vac_air_con_mode"))
  {
    if((value->integer >= 1) && (value->integer <= 5))
    {
      //Get object value
      entry->vac_air_con_mode           = value->integer;
      entry->vac_air_con_mode_write     = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCONMODE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_con_temp object---------------------------------
  else if(!strcmp(object->name, "vac_air_con_temp"))
  {
    if((value->integer >= 16) && (value->integer <= 30))
    {
      //Get object value
      entry->vac_air_con_temp           = value->integer;
      entry->vac_air_con_temp_write     = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCONTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_con_speed object------------------------------
  else if(!strcmp(object->name, "vac_air_con_speed"))
  {
    if((value->integer >= 1) && (value->integer <= 4))
    {
      //Get object value
      entry->vac_air_con_speed          = value->integer;
      entry->vac_air_con_speed_write    = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCONSPEED;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_air_cond_dir object---------------------------------------
  else if(!strcmp(object->name, "vac_air_cond_dir"))
  {
    if((value->integer >= 0) && (value->integer <= 3))
    {
      //Get object value
      entry->vac_air_cond_dir           = value->integer;
      entry->vac_air_cond_dir_write     = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_AIRCONDDIR;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vacType object?
  else if(!strcmp(object->name, "vac_type"))
  {
    if((value->integer >= 0) && (value->integer <= 2))
    {
      //Get object value
      entry->vac_type= value->integer;
      j = index;
      modbus_manager_struct.vac_manager_struct[j-1].vac_type = entry->vac_type;
      switch(modbus_manager_struct.vac_manager_struct[j-1].vac_type)
      {
      case 1:// VIETTEL
        {
          modbus_manager_struct.vac_manager_struct[j-1].vac_slave_offset   = 34;
          modbus_manager_struct.vac_manager_struct[j-1].vac_abs_slave_id   = 34+j;
          modbus_manager_struct.vac_manager_struct[j-1].vac_base_addr[0]   = 0;
          modbus_manager_struct.vac_manager_struct[j-1].vac_base_addr[1]   = 0;
          modbus_manager_struct.vac_manager_struct[j-1].vac_base_addr[2]   = 80;
          modbus_manager_struct.vac_manager_struct[j-1].vac_num_of_reg[0]  = 14;    
          modbus_manager_struct.vac_manager_struct[j-1].vac_num_of_reg[1]  = 39;   
          modbus_manager_struct.vac_manager_struct[j-1].vac_num_of_reg[2]  = 3;
        }
        break;
        
      case 2:// 
        {                                     
        }
        break;
      };
      modbus_dev_struct[j-1].device_vendor = modbus_manager_struct.vac_manager_struct[j-1].vac_type;
      modbus_dev_struct[j-1].slave_id      = modbus_manager_struct.vac_manager_struct[j-1].vac_abs_slave_id;  
      //
      // Did parameters change?
      //
      if  (parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor != modbus_dev_struct[j-1].device_vendor)
      {
        //
        // Update the current parameters with the new settings.
        //
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor = modbus_dev_struct[j-1].device_vendor;
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].slave_id      = modbus_dev_struct[j-1].slave_id;
        //
        // Yes - save these settings as the defaults.
        //
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].device_vendor;
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].slave_id      = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM].slave_id;
        
        flag_struct.save_config_flag |=1;
      }   
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vacRealTimeSync object?
  else if(!strcmp(object->name, "vac_real_time_sync"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(40 >= valueLen)
    {
      pui8_char = &value->octetString[0];        
      char_idx = 0;
      for (uint8_t i = 0; i < valueLen; i++)
      {
        set_char = *pui8_char++;
        switch (set_char)
        {
        case '/':
        case ':':
        case '-':
          {
            token++;
            char_idx = 0;
          }
          break;
        case ' ':
          break;
        default:
          {
            buff[token][char_idx]= set_char;
            char_idx++;
          }
          break;
        };
      }
      
      sync_time.hour     = atol((const char*)&buff[0][0]);
      sync_time.min      = atol((const char*)&buff[1][0]);
      sync_time.sec      = atol((const char*)&buff[2][0]);
      sync_time.date     = atol((const char*)&buff[3][0]);
      sync_time.month    = atol((const char*)&buff[4][0]);
      sync_time.year     = atol((const char*)&buff[5][0]);
      if((sync_time.sec > 59)   ||
         (sync_time.min > 59)   || 
         (sync_time.hour > 23)  || 
         (sync_time.year > 99)  || 
         (sync_time.month > 12) || 
         (sync_time.date > 31))
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
      token = 0;
      char_idx = 0;
      memset(&buff, 0, sizeof(buff));
      
      sprintf(&entry->c_sync_time[0], "%02d:%02d:%02d-%02d/%02d/%02d",
              sync_time.hour, 
              sync_time.min, 
              sync_time.sec, 
              sync_time.date, 
              sync_time.month, 
              sync_time.year);
      
      
      //Copy object value
      //          memcpy(entry->siteDCsystemVendor, value->octetString, valueLen);
      //Return object length
      entry->c_sync_time_len               = 20;
      config_handler_struct.is_sync_time   = 1;
      modbus_manager_struct.setting_cmd    = SET_VAC_SYNCTIME;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //vac_w_enable object-----------------------------------------------
  else if(!strcmp(object->name, "vac_w_enable"))
  {
    if((value->integer == 0)||((value->integer == 1)&&(entry->vac_w_serial2 == 1)))
    {
      //Get object value
      entry->vac_w_enable               = value->integer;
      entry->vac_w_enable_write         = value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_W_ENABLE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_w_serial1 object-------------------------------------------
  else if(!strcmp(object->name, "vac_w_serial1"))
  {
    if((value->integer >= 0) && (value->integer <= 9999))
    {
      //Get object value
      entry->vac_w_serial1= value->integer;
      entry->vac_w_serial1_write= value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_W_SERIAL1;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_w_serial2 object------------------------------------------
  else if(!strcmp(object->name, "vac_w_serial2"))
  {
    if((value->integer >= 0) && (value->integer <= 99999))
    {
      //Get object value
      entry->vac_w_serial2= value->integer;
      entry->vac_w_serial2_write= value->integer;
      modbus_manager_struct.setting_cmd = SET_VAC_W_SERIAL2;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //vac_enable_reset object-------------------------------------
  else if(!strcmp(object->name, "vac_enable_reset"))
  {
    if(value->integer == 1)
    {
      //Get object value
      modbus_manager_struct.setting_cmd = SET_VAC_RESET;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  
  return error;
}

// private mib get info entry-------------------------------------------------
error_t private_mib_get_vac_info_entry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen){
  error_t error = NO_ERROR;
  size_t  n;
  uint_t  index;
  PRIVATE_MIB_VAC_INFO_ENTRY_STRUCT *entry;
  
  n = object->oidLen;
  
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  if(error) return error;
  
  if(n != oidLen) return ERROR_INSTANCE_NOT_FOUND;
  
  if((index<1) || (index>10)) return ERROR_INSTANCE_NOT_FOUND;
  
  entry = &private_mib_base_struct.vac_group_struct.vac_table_struct[index - 1];
  //vac_id object----------------------------------
  if(!strcmp(object->name, "vac_id"))
  {
    value->integer = entry->vac_id;
  }
  //vac_status object------------------------------
  else if(!strcmp(object->name, "vac_status"))//vac_status
  {
    value->integer = entry->vac_status;
  }
  //vac_in_temp object------------------------------
  else if(!strcmp(object->name, "vac_in_temp"))
  {
    value->integer = entry->vac_in_temp;
  }  
  //vac_ex_temp object------------------------------
  else if(!strcmp(object->name, "vac_ex_temp"))
  {
    value->integer = entry->vac_ex_temp;
  }  
  //vac_frost1_temp object------------------------------
  else if(!strcmp(object->name, "vac_frost1_temp"))
  {
    value->integer = entry->vac_frost1_temp;
  }
  //vac_frost2_temp object------------------------------
  else if(!strcmp(object->name, "vac_frost2_temp"))
  {
    value->integer = entry->vac_frost2_temp;
  }
  //vac_humid object------------------------------
  else if(!strcmp(object->name, "vac_humid"))
  {
    value->integer = entry->vac_humid;
  }
  //vac_fan1_duty object------------------------------
  else if(!strcmp(object->name, "vac_fan1_duty"))
  {
    value->integer = entry->vac_fan1_duty;
  }
  //vac_fan1_rpm object------------------------------
  else if(!strcmp(object->name, "vac_fan1_rpm"))
  {
    value->integer = entry->vac_fan1_rpm;
  }
  //vac_fan2_duty object------------------------------
  else if(!strcmp(object->name, "vac_fan2_duty"))
  {
    value->integer = entry->vac_fan2_duty;
  }
  //vac_fan2_rpm object------------------------------
  else if(!strcmp(object->name, "vac_fan2_rpm"))
  {
    value->integer = entry->vac_fan2_rpm;
  }
  //vac_alarm_status object------------------------------
  else if(!strcmp(object->name, "vac_alarm_status"))
  {
    value->integer = entry->vac_alarm_status.all;
  }
  //vac_sys_mode object------------------------------
  else if(!strcmp(object->name, "vac_sys_mode"))
  {
    value->integer = entry->vac_sys_mode;
  }
  //vac_fan_speed_mode object------------------------------
  else if(!strcmp(object->name, "vac_fan_speed_mode"))
  {
    value->integer = entry->vac_fan_speed_mode;
  }
  //vac_fan_start_temp object------------------------------
  else if(!strcmp(object->name, "vac_fan_start_temp"))
  {
    value->integer = entry->vac_fan_start_temp;
  }  
  //vac_set_temp object------------------------------------
  else if(!strcmp(object->name, "vac_set_temp"))
  {
    value->integer = entry->vac_set_temp;
  }
  //vac_pid_offset_temp object-----------------------------
  else if(!strcmp(object->name, "vac_pid_offset_temp"))
  {
    value->integer = entry->vac_pid_offset_temp;
  }
  //vac_fan_min_speed object-------------------------------
  else if(!strcmp(object->name, "vac_fan_min_speed"))
  {
    value->integer = entry->vac_fan_min_speed;
  }
  //vac_fan_max_speed object------------------------------
  else if(!strcmp(object->name, "vac_fan_max_speed"))
  {
    value->integer = entry->vac_fan_max_speed;
  }
  //vac_filter_stuck_temp object-------------------------
  else if(!strcmp(object->name, "vac_filter_stuck_temp"))
  {
    value->integer = entry->vac_filter_stuck_temp;
  }
  //vac_night_mode_en object-----------------------------
  else if(!strcmp(object->name, "vac_night_mode_en"))
  {
    value->integer = entry->vac_night_mode_en;
  }
  //vac_night_mode_start object--------------------------
  else if(!strcmp(object->name, "vac_night_mode_start"))
  {
    value->integer = entry->vac_night_mode_start;
  }
  //vac_night_mode_end object---------------------------
  else if(!strcmp(object->name, "vac_night_mode_end"))
  {
    value->integer = entry->vac_night_mode_end;
  }
  //vac_night_max_speed object-------------------------
  else if(!strcmp(object->name, "vac_night_max_speed"))
  {
    value->integer = entry->vac_night_max_speed;
  }
  //vac_manual_mode object-----------------------------
  else if(!strcmp(object->name, "vac_manual_mode"))
  {
    value->integer = entry->vac_manual_mode;
  }
  //vac_manual_max_speed object------------------------
  else if(!strcmp(object->name, "vac_manual_max_speed"))
  {
    value->integer = entry->vac_manual_max_speed;
  }
  //vac_in_max_temp object---------------------------
  else if(!strcmp(object->name, "vac_in_max_temp"))
  {
    value->integer = entry->vac_in_max_temp;
  }
  //vac_ex_max_temp object---------------------------
  else if(!strcmp(object->name, "vac_ex_max_temp"))
  {
    value->integer = entry->vac_ex_max_temp;
  }
  //vac_frost_max_temp object------------------------
  else if(!strcmp(object->name, "vac_frost_max_temp"))
  {
    value->integer = entry->vac_frost_max_temp;
  }
  //vac_in_min_temp object--------------------------
  else if(!strcmp(object->name, "vac_in_min_temp"))
  {
    value->integer = entry->vac_in_min_temp;
  }
  //vac_ex_min_temp object--------------------------
  else if(!strcmp(object->name, "vac_ex_min_temp"))
  {
    value->integer = entry->vac_ex_min_temp;
  }
  //vac_frost_min_temp object----------------------
  else if(!strcmp(object->name, "vac_frost_min_temp"))
  {
    value->integer = entry->vac_frost_min_temp;
  }
  //vac_min_out_temp object------------------------
  else if(!strcmp(object->name, "vac_min_out_temp"))
  {
    value->integer = entry->vac_min_out_temp;
  }
  //vac_delta_temp object-------------------------
  else if(!strcmp(object->name, "vac_delta_temp"))
  {
    value->integer = entry->vac_delta_temp;
  }
  //vac_panic_temp object------------------------
  else if(!strcmp(object->name, "vac_panic_temp"))
  {
    value->integer = entry->vac_panic_temp;
  }
  //vac_active_fan object------------------------
  else if(!strcmp(object->name, "vac_active_fan"))
  {
    value->integer = entry->vac_active_fan;
  }
  //vac_installed_fan object-----------------------
  else if(!strcmp(object->name, "vac_installed_fan"))
  {
    value->integer = entry->vac_installed_fan;
  }
  //vac_installed_air_con object-------------------
  else if(!strcmp(object->name, "vac_installed_air_con"))
  {
    value->integer = entry->vac_installed_air_con;
  }
  //vac_air_con1_model object---------------------
  else if(!strcmp(object->name, "vac_air_con1_model"))
  {
    value->integer = entry->vac_air_con1_model;
  }
  //vac_air_con1_type object-----------------------
  else if(!strcmp(object->name, "vac_air_con1_type"))
  {
    value->integer = entry->vac_air_con1_type;
  }
  //vac_air_con2_model object----------------------
  else if(!strcmp(object->name, "vac_air_con2_model"))
  {
    value->integer = entry->vac_air_con2_model;
  }
  //vac_air_con2_type object----------------------
  else if(!strcmp(object->name, "vac_air_con2_type"))
  {
    value->integer = entry->vac_air_con2_type;
  }
  //vac_air_con_on_off object----------------------
  else if(!strcmp(object->name, "vac_air_con_on_off"))
  {
    value->integer = entry->vac_air_con_on_off;
  }
  //vac_air_con_mode object------------------------
  else if(!strcmp(object->name, "vac_air_con_mode"))
  {
    value->integer = entry->vac_air_con_mode;
  }
  //vac_air_con_temp object-----------------------
  else if(!strcmp(object->name, "vac_air_con_temp"))
  {
    value->integer = entry->vac_air_con_temp;
  }
  //vac_air_con_speed object----------------------
  else if(!strcmp(object->name, "vac_air_con_speed"))
  {
    value->integer = entry->vac_air_con_speed;
  }
  //vac_air_cond_dir object----------------------
  else if(!strcmp(object->name, "vac_air_cond_dir"))
  {
    value->integer = entry->vac_air_cond_dir;
  }
  //vac_air_con_volt object----------------------
  else if(!strcmp(object->name, "vac_air_con_volt"))
  {
    value->integer = entry->vac_air_con_volt;
  }
  //vac_air_con_current object----------------------
  else if(!strcmp(object->name, "vac_air_con_current"))
  {
    value->integer = entry->vac_air_con_current;
  }
  //vac_air_con_power object---------------------
  else if(!strcmp(object->name, "vac_air_con_power"))
  {
    value->integer = entry->vac_air_con_power;
  }
  //vac_air_con_frequency object-----------------------
  else if(!strcmp(object->name, "vac_air_con_frequency"))
  {
    value->integer = entry->vac_air_con_frequency;
  }
  //vac_air_con_pf object------------------------------
  else if(!strcmp(object->name, "vac_air_con_pf"))
  {
    value->integer = entry->vac_air_con_pf;
  }
  //vac_real_time_sync object?
  else if(!strcmp(object->name, "vac_real_time_sync"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->c_sync_time_len)
    {
      //Copy object value
      memcpy(value->octetString, entry->c_sync_time, entry->c_sync_time_len);
      //Return object length
      *valueLen = entry->c_sync_time_len;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //vac_type object--------------------------
  else if(!strcmp(object->name, "vac_type"))
  {
    value->integer = entry->vac_type;
  }
  //vac_serial object----------------------------
  else if(!strcmp(object->name, "vac_serial"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->vac_serial_len)
    {
      //Copy object value
      memcpy(value->octetString, entry->vac_serial, entry->vac_serial_len);
      //Return object length
      *valueLen = entry->vac_serial_len;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //vac_modbus_id object------------------------
  else if(!strcmp(object->name, "vac_modbus_id"))
  {
    value->integer = entry->vac_modbus_id;
  }
  //vac_firm_version object--------------------
  else if(!strcmp(object->name, "vac_firm_version"))
  {
    value->integer = entry->vac_firm_version;
  }
  //vac_w_enable object------------------------
  else if(!strcmp(object->name, "vac_w_enable"))
  {
    value->integer = entry->vac_w_enable;
  }
  //vac_w_serial1 object------------------------
  else if(!strcmp(object->name, "vac_w_serial1"))
  {
    value->integer = entry->vac_w_serial1;
  }
  //vac_w_serial2 object-------------------------
  else if(!strcmp(object->name, "vac_w_serial2"))
  {
    value->integer = entry->vac_w_serial2;
  }
  //vac_enable_reset object---------------------
  else if(!strcmp(object->name, "vac_enable_reset"))
  {
    value->integer = entry->vac_enable_reset;
  }
  //vac_update_step object------------------------
  else if(!strcmp(object->name, "vac_update_step"))
  {
    value->integer = entry->vac_update_step;
  }
  return error;  
}

//private mib get next info entry--------------------------------------------
error_t private_mib_get_next_vac_info_entry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen){
  error_t error = NO_ERROR;
  
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.vac_group_struct.vac_installed_vac; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

//lib info function=======================================================================================

//private mib set lib batt group--------------------------------------------
error_t privateMibSetLIBattGroup(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  
  
  
  PrivateMibLIBattInfoGroup *entry;
  
  //	Point to the liBattGroup entry
  entry = &private_mib_base_struct.liBattGroup;
  
  if(!strcmp(object->name, "liBattInstalledPack"))
  {
    if((value->integer >= 0) && (value->integer <= 16))
    {
      //Get object value
      entry->liBattInstalledPack = value->integer; 
      if (entry->liBattInstalledPack != modbus_manager_struct.number_of_lib)
      {
        modbus_manager_struct.number_of_lib = entry->liBattInstalledPack;
        parameter.device_number[TOTAL_LIB_INDEX] = modbus_manager_struct.number_of_lib;
        working_default_parameter.device_number[TOTAL_LIB_INDEX] = parameter.device_number[TOTAL_LIB_INDEX];
        flag_struct.save_config_flag |=1;
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR;    
}

//private mib set lib batt info entry---------------------------------------
error_t privateMibSetLIBattInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  uint_t j;
  MODBUS_PARAMETER_STRUCT sModbusDev[20];
  PrivateMibLIBattInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 16)
    return ERROR_INSTANCE_NOT_FOUND;
  
  LibM1Addr = index;
  //Point to the libTable entry
  entry = &private_mib_base_struct.liBattGroup.liBattTable[index - 1];
  //liionBattStatusType object?
  if(!strcmp(object->name, "liionBattStatusType"))
  {
    if((value->integer >= 0) && (value->integer <= 11))
    {
      //Set object value
      entry->liBattType= value->integer;
      j = index;
      modbus_manager_struct.lib_manager_struct[j-1].lib_type = entry->liBattType;
      
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
          if(j<9)
            modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 213;
          else
            modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 215;
          modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset + j;
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
      case 8:// HUAWEI_A1
        {
          if(j<9)
            modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 213;
          else
            modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 215;
          modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset + j;
          modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
          modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 50; 
          modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 67;
          modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 8;  
          modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 257;
          modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 7;  
        }
        break;
      case 5:// M1Viettel50
        {
          modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
          modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
          modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0; // FC:0x04
          modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 100;   
          modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[1] = 130;// FC:0x04
          modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[1] = 22; //20
          modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[2] = 0;// FC:0x03
          modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[2] = 80; 
          modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[3] = 162;// FC:0x03
          modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[3] = 3;//2                 
        }
        break;
        
        //        case 8:// BYU75
        //        {
        //            modbus_manager_struct.lib_manager_struct[j-1].lib_slave_offset = 0;
        //            modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id = j;
        //            modbus_manager_struct.lib_manager_struct[j-1].lib_base_addr[0] = 0;
        //            modbus_manager_struct.lib_manager_struct[j-1].lib_num_of_reg[0] = 54;                  
        //        }
        //        break;
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
      sModbusDev[j-1].device_vendor = modbus_manager_struct.lib_manager_struct[j-1].lib_type;
      sModbusDev[j-1].slave_id = modbus_manager_struct.lib_manager_struct[j-1].lib_abs_slave_id;
      
      //
      // Did parameters change?
      //
      if  (parameter.modbus_parameter[j-1].device_vendor != sModbusDev[j-1].device_vendor)
      {
        //
        // Update the current parameters with the new settings.
        //
        parameter.modbus_parameter[j-1].device_vendor = sModbusDev[j-1].device_vendor;
        parameter.modbus_parameter[j-1].slave_id = sModbusDev[j-1].slave_id;
        //
        // Yes - save these settings as the defaults.
        //
        working_default_parameter.modbus_parameter[j-1].device_vendor = parameter.modbus_parameter[j-1].device_vendor;
        working_default_parameter.modbus_parameter[j-1].slave_id = parameter.modbus_parameter[j-1].slave_id;
        
        flag_struct.save_config_flag |=1;
      }        
      
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32PackHighVolt_A object?
  else if(!strcmp(object->name, "i32PackHighVolt_A"))
  {
    if((value->integer >= 4100) && (value->integer <= 6000))
    {
      //Get object value
      entry->i32PackHighVolt_A= value->integer;
      entry->i32PackHighVolt_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_PACKHIGHVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BattHighVolt_A object?
  else if(!strcmp(object->name, "i32BattHighVolt_A"))
  {
    if((value->integer >= 4100) && (value->integer <= 6000))
    {
      //Get object value
      entry->i32BattHighVolt_A= value->integer;
      entry->i32BattHighVolt_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BATTHIGHVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CellHighVolt_A object?
  else if(!strcmp(object->name, "i32CellHighVolt_A"))
  {
    if((value->integer >= 3200) && (value->integer <= 4550))
    {
      //Get object value
      entry->i32CellHighVolt_A= value->integer;
      entry->i32CellHighVolt_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CELLHIGHVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BattLowVolt_A object?
  else if(!strcmp(object->name, "i32BattLowVolt_A"))
  {
    if((value->integer >= 3300) && (value->integer <= 5100))
    {
      //Get object value
      entry->i32BattLowVolt_A= value->integer;
      entry->i32BattLowVolt_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BATTLOWVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CellLowVolt_A object?
  else if(!strcmp(object->name, "i32CellLowVolt_A"))
  {
    if((value->integer >= 2500) && (value->integer <= 3400))
    {
      //Get object value
      entry->i32CellLowVolt_A= value->integer;
      entry->i32CellLowVolt_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CELLLOWVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CharHighCurr_A object?
  else if(!strcmp(object->name, "i32CharHighCurr_A"))
  {
    if((value->integer >= 50) && (value->integer <= 1000))
    {
      //Get object value
      entry->i32CharHighCurr_A= value->integer;
      entry->i32CharHighCurr_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CHARHIGHCURR;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32DisCharHighCurr_A object?
  else if(!strcmp(object->name, "i32DisCharHighCurr_A"))
  {
    if((value->integer >= 50) && (value->integer <= 1500))
    {
      //Get object value
      entry->i32DisCharHighCurr_A= value->integer;
      entry->i32DisCharHighCurr_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DISCHARHIGHCURR;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CharHighTemp_A object?
  else if(!strcmp(object->name, "i32CharHighTemp_A"))
  {
    if((value->integer >= 2000) && (value->integer <= 6000))
    {
      //Get object value
      entry->i32CharHighTemp_A= value->integer;
      entry->i32CharHighTemp_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CHARHIGHTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32DisCharHighTemp_A object?
  else if(!strcmp(object->name, "i32DisCharHighTemp_A"))
  {
    if((value->integer >= 2000) && (value->integer <= 6000))
    {
      //Get object value
      entry->i32DisCharHighTemp_A= value->integer;
      entry->i32DisCharHighTemp_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DISCHARHIGHTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CharLowTemp_A object?
  else if(!strcmp(object->name, "i32CharLowTemp_A"))
  {
    if((value->integer >= -2000) && (value->integer <= 4000))
    {
      //Get object value
      entry->i32CharLowTemp_A= value->integer;
      entry->i32CharLowTemp_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CHARLOWTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32DisCharLowtemp_A object?
  else if(!strcmp(object->name, "i32DisCharLowtemp_A"))
  {
    if((value->integer >= -2000) && (value->integer <= 4000))
    {
      //Get object value
      entry->i32DisCharLowtemp_A= value->integer;
      entry->i32DisCharLowtemp_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DISCHARLOWTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32LowCap_A object?
  else if(!strcmp(object->name, "i32LowCap_A"))
  {
    if((value->integer >= 0) && (value->integer <= 100))
    {
      //Get object value
      entry->i32LowCap_A= value->integer;
      entry->i32LowCap_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_LOWCAP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BMSHighTemp_A object?
  else if(!strcmp(object->name, "i32BMSHighTemp_A"))
  {
    if((value->integer >= 6000) && (value->integer <= 12000))
    {
      //Get object value
      entry->i32BMSHighTemp_A= value->integer;
      entry->i32BMSHighTemp_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BMSHIGHTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BMSLowTemp_A object?
  else if(!strcmp(object->name, "i32BMSLowTemp_A"))
  {
    if((value->integer >= 0) && (value->integer <= 4000))
    {
      //Get object value
      entry->i32BMSLowTemp_A= value->integer;
      entry->i32BMSLowTemp_AWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BMSLOWTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32PackOverVolt_P object?
  else if(!strcmp(object->name, "i32PackOverVolt_P"))
  {
    if((value->integer >= 4000) && (value->integer <= 6000))
    {
      //Get object value
      entry->i32PackOverVolt_P= value->integer;
      entry->i32PackOverVolt_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_PACKOVERVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BattOverVolt_P object?
  else if(!strcmp(object->name, "i32BattOverVolt_P"))
  {
    if((value->integer >= 4000) && (value->integer <= 6000))
    {
      //Get object value
      entry->i32BattOverVolt_P= value->integer;
      entry->i32BattOverVolt_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BATTOVERVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CellOverVolt_P object?
  else if(!strcmp(object->name, "i32CellOverVolt_P"))
  {
    if((value->integer >= 3200) && (value->integer <= 4550))
    {
      //Get object value
      entry->i32CellOverVolt_P= value->integer;
      entry->i32CellOverVolt_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CELLOVERVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BattUnderVolt_P object?
  else if(!strcmp(object->name, "i32BattUnderVolt_P"))
  {
    if((value->integer >= 3300) && (value->integer <= 5000))
    {
      //Get object value
      entry->i32BattUnderVolt_P= value->integer;
      entry->i32BattUnderVolt_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BATTUNDERVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CellUnderVolt_P object?
  else if(!strcmp(object->name, "i32CellUnderVolt_P"))
  {
    if((value->integer >= 2500) && (value->integer <= 3400))
    {
      //Get object value
      entry->i32CellUnderVolt_P= value->integer;
      entry->i32CellUnderVolt_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CELLUNDERVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CharOverCurr_P object?
  else if(!strcmp(object->name, "i32CharOverCurr_P"))
  {
    if((value->integer >= 50) && (value->integer <= 1000))
    {
      //Get object value
      entry->i32CharOverCurr_P= value->integer;
      entry->i32CharOverCurr_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CHAROVERCURR;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32DisCharOverCurr_P object?
  else if(!strcmp(object->name, "i32DisCharOverCurr_P"))
  {
    if((value->integer >= 50) && (value->integer <= 1500))
    {
      //Get object value
      entry->i32DisCharOverCurr_P= value->integer;
      entry->i32DisCharOverCurr_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DISCHAROVERCURR;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CharOverTemp_P object?
  else if(!strcmp(object->name, "i32CharOverTemp_P"))
  {
    if((value->integer >= 2000) && (value->integer <= 8000))
    {
      //Get object value
      entry->i32CharOverTemp_P= value->integer;
      entry->i32CharOverTemp_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CHAROVERTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32DisCharOverTemp_P object?
  else if(!strcmp(object->name, "i32DisCharOverTemp_P"))
  {
    if((value->integer >= 2000) && (value->integer <= 8000))
    {
      //Get object value
      entry->i32DisCharOverTemp_P= value->integer;
      entry->i32DisCharOverTemp_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DISCHAROVERTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32CharUnderTemp_P object?
  else if(!strcmp(object->name, "i32CharUnderTemp_P"))
  {
    if((value->integer >= -3000) && (value->integer <= 4000))
    {
      //Get object value
      entry->i32CharUnderTemp_P= value->integer;
      entry->i32CharUnderTemp_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CHARUNDERTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32DisCharUnderTemp_P object?
  else if(!strcmp(object->name, "i32DisCharUnderTemp_P"))
  {
    if((value->integer >= -3000) && (value->integer <= 4000))
    {
      //Get object value
      entry->i32DisCharUnderTemp_P= value->integer;
      entry->i32DisCharUnderTemp_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DISCHARUNDERTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32UnderCap_P object?
  else if(!strcmp(object->name, "i32UnderCap_P"))
  {
    if((value->integer >= 0) && (value->integer <= 100))
    {
      //Get object value
      entry->i32UnderCap_P= value->integer;
      entry->i32UnderCap_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_UNDERCAP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BMSOverTemp_P object?
  else if(!strcmp(object->name, "i32BMSOverTemp_P"))
  {
    if((value->integer >= 6000) && (value->integer <= 15000))
    {
      //Get object value
      entry->i32BMSOverTemp_P= value->integer;
      entry->i32BMSOverTemp_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BMSOVERTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32BMSUnderTemp_P object?
  else if(!strcmp(object->name, "i32BMSUnderTemp_P"))
  {
    if((value->integer >= -2000) && (value->integer <= 4000))
    {
      //Get object value
      entry->i32BMSUnderTemp_P= value->integer;
      entry->i32BMSUnderTemp_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_BMSUNDERTEMP;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //i32DifferentVolt_P object?
  else if(!strcmp(object->name, "i32DifferentVolt_P"))
  {
    if((value->integer >= 50) && (value->integer <= 700))
    {
      //Get object value
      entry->i32DifferentVolt_P= value->integer;
      entry->i32DifferentVolt_PWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DIFFERENTVOLT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32VoltBalance object?
  else if(!strcmp(object->name, "u32VoltBalance"))
  {
    if((value->integer >= 4000) && (value->integer <= 5800))
    {
      //Get object value
      entry->u32VoltBalance= value->integer;
      entry->u32VoltBalanceWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_VOLTBALANCE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32DeltaVoltBalance object?
  else if(!strcmp(object->name, "u32DeltaVoltBalance"))
  {
    if((value->integer >= 0) && (value->integer <= 500))
    {
      //Get object value
      entry->u32DeltaVoltBalance= value->integer;
      entry->u32DeltaVoltBalanceWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DELTAVOLTBALANCE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32DisCharCurrLimit object?
  else if(!strcmp(object->name, "u32DisCharCurrLimit"))
  {
    if((value->integer >= 0) && (value->integer <= 99))
    {
      //Get object value
      entry->u32DisCharCurrLimit= value->integer;
      entry->u32DisCharCurrLimitWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_DISCHARCURRLIMIT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32CharCurrLimit object?
  else if(!strcmp(object->name, "u32CharCurrLimit"))
  {
    if((value->integer >= 0) && (value->integer <= 60))
    {
      //Get object value
      entry->u32CharCurrLimit= value->integer;
      entry->u32CharCurrLimitWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CHARCURRLIMIT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32VoltDisCharRef object?
  else if(!strcmp(object->name, "u32VoltDisCharRef"))
  {
    if((value->integer >= 4000) && (value->integer <= 6000))
    {
      //Get object value
      entry->u32VoltDisCharRef= value->integer;
      entry->u32VoltDisCharRefWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_VOLTDISCHARREF;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32VoltDisCharCMD object?
  else if(!strcmp(object->name, "u32VoltDisCharCMD"))
  {
    if((value->integer >= 0) && (value->integer <= 4))
    {
      //Get object value
      entry->u32VoltDisCharCMD= value->integer;
      entry->u32VoltDisCharCMDWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_VOLTDISCHARCMD;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32SystemByte object?
  else if(!strcmp(object->name, "u32SystemByte"))
  {
    //      if((value->integer >= 0) && (value->integer <= 300))
    //      {
    //Get object value
    entry->u32SystemByte= value->integer;
    entry->u32SystemByteWrite= value->integer;
    modbus_manager_struct.setting_cmd = SET_LIB_SYSTEMBYTE;
    //      }
    //      else
    //      {
    //        return ERROR_PARAMETER_OUT_OF_RANGE;
    //      }
  }
  //u32KeyTime object?
  else if(!strcmp(object->name, "u32KeyTime"))
  {
    if((value->integer >= 0) && (value->integer <= 64))
    {
      //Get object value
      entry->u32KeyTime= value->integer;
      entry->u32KeyTimeWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_KEYTIME;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32IniMaxim object?
  else if(!strcmp(object->name, "u32IniMaxim"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->u32IniMaxim= value->integer;
      entry->u32IniMaximWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_INIMAXIM;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32EnableKCS object?
  else if(!strcmp(object->name, "u32EnableKCS"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->u32EnableKCS= value->integer;
      entry->u32EnableKCSWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_ENABLEKCS;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //u32EnableLock object?
  else if(!strcmp(object->name, "u32EnableLock"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->u32EnableLock= value->integer;
      entry->u32EnableLockWrite= value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_ENABLELOCK;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //liionCapInit object?
  else if(!strcmp(object->name, "liionCapInit"))
  {
    if((value->integer >= 30) && (value->integer <= 150))
    {
      //Get object value
      entry->u16_liionCapInit       = value->integer;
      entry->u16_liionCapInitWrite  = value->integer;
      modbus_manager_struct.setting_cmd = SET_LIB_CAPINIT;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //Successful processing
  return NO_ERROR;
}

//private mib get lib batt info entry---------------------------------------
error_t privateMibGetLIBattInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibLIBattInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 16)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &private_mib_base_struct.liBattGroup.liBattTable[index - 1];
  //liBattIndex object?
  if(!strcmp(object->name, "liBattIndex"))
  {
    //Get object value
    value->integer = entry->liBattIndex;
  }
  //liBattStatus object?
  else if(!strcmp(object->name, "liBattStatus"))
  {
    //Get object value
    value->integer = entry->liBattStatus;
  }
  //liBattPackVolt object?
  else if(!strcmp(object->name, "liBattPackVolt"))
  {
    //Get object value
    value->integer = entry->liBattPackVolt;
  }
  //liBattPackCurr object?
  else if(!strcmp(object->name, "liBattPackCurr"))
  {
    //Get object value
    value->integer = entry->liBattPackCurr;
  }
  //liBattRemainCap object?
  else if(!strcmp(object->name, "liBattRemainCap"))
  {
    //Get object value
    value->integer = entry->liBattRemainCap;
  }
  //liBattAvrCellTemp object?
  else if(!strcmp(object->name, "liBattAvrCellTemp"))
  {
    //Get object value
    value->integer = entry->liBattAvrCellTemp;
  }
  //liBattAmbTemp object?
  else if(!strcmp(object->name, "liBattAmbTemp"))
  {
    //Get object value
    value->integer = entry->liBattAmbTemp;
  }
  //liBattWarningFlag object?
  else if(!strcmp(object->name, "liBattWarningFlag"))
  {
    //Get object value
    value->integer = entry->liBattWarningFlag;
  }
  //liBattProtectFlag object?
  else if(!strcmp(object->name, "liBattProtectFlag"))
  {
    //Get object value
    value->integer = entry->liBattProtectFlag;
  }
  //liBattFaultStat object?
  else if(!strcmp(object->name, "liBattFaultStat"))
  {
    //Get object value
    value->integer = entry->liBattFaultStat;
  }
  //u32SOCMaxim object?
  else if(!strcmp(object->name, "u32SOCMaxim"))
  {
    //Get object value
    value->integer = entry->u32SOCMaxim;
  }
  //liBattSOH object?
  else if(!strcmp(object->name, "liBattSOH"))
  {
    //Get object value
    value->integer = entry->liBattSOH;
  }
  //liBattPeriod object?
  else if(!strcmp(object->name, "liBattPeriod"))
  {
    //Get object value
    value->integer = entry->liBattPeriod;
  }
  //liBattHWVer object?
  else if(!strcmp(object->name, "liBattHWVer"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->liBattHWVerLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->liBattHWVer, entry->liBattHWVerLen);
      //Return object length
      *valueLen = entry->liBattHWVerLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //liBattSWVer object?
  else if(!strcmp(object->name, "liBattSWVer"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->liBattSWVerLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->liBattSWVer, entry->liBattSWVerLen);
      //Return object length
      *valueLen = entry->liBattSWVerLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //liBattSerialNo object?
  else if(!strcmp(object->name, "liBattSerialNo"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->liBattSerialNoLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->liBattSerialNo, entry->liBattSerialNoLen);
      //Return object length
      *valueLen = entry->liBattSerialNoLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //liBattModel object?        // tmt
  else if(!strcmp(object->name, "liBattModel"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->liBattModelLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->liBattModel, entry->liBattModelLen);
      //Return object length
      *valueLen = entry->liBattModelLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //liBattCellVolt1 object?
  else if(!strcmp(object->name, "liBattCellVolt1"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[0];
  }
  //liBattCellVolt2 object?
  else if(!strcmp(object->name, "liBattCellVolt2"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[1];
  }
  //liBattCellVolt3 object?
  else if(!strcmp(object->name, "liBattCellVolt3"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[2];
  }
  //liBattCellVolt4 object?
  else if(!strcmp(object->name, "liBattCellVolt4"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[3];
  }
  //liBattCellVolt5 object?
  else if(!strcmp(object->name, "liBattCellVolt5"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[4];
  }
  //liBattCellVolt6 object?
  else if(!strcmp(object->name, "liBattCellVolt6"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[5];
  }
  //liBattCellVolt7 object?
  else if(!strcmp(object->name, "liBattCellVolt7"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[6];
  }
  //liBattCellVolt8 object?
  else if(!strcmp(object->name, "liBattCellVolt8"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[7];
  }
  //liBattCellVolt9 object?
  else if(!strcmp(object->name, "liBattCellVolt9"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[8];
  }
  //liBattCellVolt10 object?
  else if(!strcmp(object->name, "liBattCellVolt10"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[9];
  }
  //liBattCellVolt11 object?
  else if(!strcmp(object->name, "liBattCellVolt11"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[10];
  }
  //liBattCellVolt12 object?
  else if(!strcmp(object->name, "liBattCellVolt12"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[11];
  }
  //liBattCellVolt13 object?
  else if(!strcmp(object->name, "liBattCellVolt13"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[12];
  }
  //liBattCellVolt14 object?
  else if(!strcmp(object->name, "liBattCellVolt14"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[13];
  }
  //liBattCellVolt15 object?
  else if(!strcmp(object->name, "liBattCellVolt15"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[14];
  }
  //liBattCellVolt16 object?
  else if(!strcmp(object->name, "liBattCellVolt16"))
  {
    //Get object value
    value->integer = entry->liBattCellVolt[15];
  }
  //liBattCellTemp1 object?
  else if(!strcmp(object->name, "liBattCellTemp1"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[0];
  }
  //liBattCellTemp2 object?
  else if(!strcmp(object->name, "liBattCellTemp2"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[1];
  }
  //liBattCellTemp3 object?
  else if(!strcmp(object->name, "liBattCellTemp3"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[2];
  }
  //liBattCellTemp4 object?
  else if(!strcmp(object->name, "liBattCellTemp4"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[3];
  }
  //liBattCellTemp5 object?
  else if(!strcmp(object->name, "liBattCellTemp5"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[4];
  }
  //liBattCellTemp6 object?
  else if(!strcmp(object->name, "liBattCellTemp6"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[5];
  }
  //liBattCellTemp7 object?
  else if(!strcmp(object->name, "liBattCellTemp7"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[6];
  }
  //liBattCellTemp8 object?
  else if(!strcmp(object->name, "liBattCellTemp8"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[7];
  }
  //liBattCellTemp9 object?
  else if(!strcmp(object->name, "liBattCellTemp9"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[8];
  }
  //liBattCellTemp10 object?
  else if(!strcmp(object->name, "liBattCellTemp10"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[9];
  }
  //liBattCellTemp11 object?
  else if(!strcmp(object->name, "liBattCellTemp11"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[10];
  }
  //liBattCellTemp12 object?
  else if(!strcmp(object->name, "liBattCellTemp12"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[11];
  }
  //liBattCellTemp13 object?
  else if(!strcmp(object->name, "liBattCellTemp13"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[12];
  }
  //liBattCellTemp14 object?
  else if(!strcmp(object->name, "liBattCellTemp14"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[13];
  }
  //liBattCellTemp15 object?
  else if(!strcmp(object->name, "liBattCellTemp15"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[14];
  }
  //liBattCellTemp16 object?
  else if(!strcmp(object->name, "liBattCellTemp16"))
  {
    //Get object value
    value->integer = entry->liBattCellTemp[15];
  }
  //liBattModbusID object?
  else if(!strcmp(object->name, "liBattModbusID"))
  {
    //Get object value
    value->integer = entry->liBattModbusID;
  }
  //liBattAlarmStatus object?
  else if(!strcmp(object->name, "liBattAlarmStatus"))
  {
    //Get object value
    value->counter32 = entry->liBattAlarmStatus.all;
  }
  //liionBattStatusType object?
  else if(!strcmp(object->name, "liionBattStatusType"))
  {
    //Get object value
    value->integer = entry->liBattType;
  }
  //liionBattStatusCCL object?
  else if(!strcmp(object->name, "liionBattStatusCCL"))
  {
    //Get object value
    value->integer = entry->liBattStatusCCL;
  }
  //liBattMode object?
  else if(!strcmp(object->name, "liBattMode"))
  {
    //Get object value
    value->integer = entry->liBattMode;
  }
  //u32ConverterState object?
  else if(!strcmp(object->name, "u32ConverterState"))
  {
    //Get object value
    value->integer = entry->u32ConverterState;
  }
  //u32ErrCode object?
  else if(!strcmp(object->name, "u32ErrCode"))
  {
    //Get object value
    value->integer = entry->u32ErrCode;
  }
  //u32BalanceStatus object?
  else if(!strcmp(object->name, "u32BalanceStatus"))
  {
    //Get object value
    value->integer = entry->u32BalanceStatus;
  }
  //u32MosfetMode object?
  else if(!strcmp(object->name, "u32MosfetMode"))
  {
    //Get object value
    value->integer = entry->u32MosfetMode;
  }
  //u32Mcu2McuErr object?
  else if(!strcmp(object->name, "u32Mcu2McuErr"))
  {
    //Get object value
    value->integer = entry->u32Mcu2McuErr;
  }
  //u32CHGCapacity object?
  else if(!strcmp(object->name, "u32CHGCapacity"))
  {
    //Get object value
    value->integer = entry->u32CHGCapacity;
  }
  //u32DSGCapacity object?
  else if(!strcmp(object->name, "u32DSGCapacity"))
  {
    //Get object value
    value->integer = entry->u32DSGCapacity;
  }
  //u32Efficiency object?
  else if(!strcmp(object->name, "u32Efficiency"))
  {
    //Get object value
    value->integer = entry->u32Efficiency;
  }
  //u32NumberOfCan object?
  else if(!strcmp(object->name, "u32NumberOfCan"))
  {
    //Get object value
    value->integer = entry->u32NumberOfCan;
  }
  //i32PackHighVolt_A
  else if(!strcmp(object->name, "i32PackHighVolt_A"))
  {
    //Get object value
    value->integer = entry->i32PackHighVolt_A;
  }
  
  //i32BattHighVolt_A object?
  else if(!strcmp(object->name, "i32BattHighVolt_A"))
  {
    //Get object value
    value->integer = entry->i32BattHighVolt_A;
  }
  //i32CellHighVolt_A object?
  else if(!strcmp(object->name, "i32CellHighVolt_A"))
  {
    //Get object value
    value->integer = entry->i32CellHighVolt_A;
  }
  //i32BattLowVolt_A object?
  else if(!strcmp(object->name, "i32BattLowVolt_A"))
  {
    //Get object value
    value->integer = entry->i32BattLowVolt_A;
  }
  //i32CellLowVolt_A object?
  else if(!strcmp(object->name, "i32CellLowVolt_A"))
  {
    //Get object value
    value->integer = entry->i32CellLowVolt_A;
  }
  //i32CharHighCurr_A object?
  else if(!strcmp(object->name, "i32CharHighCurr_A"))
  {
    //Get object value
    value->integer = entry->i32CharHighCurr_A;
  }
  //i32DisCharHighCurr_A object?
  else if(!strcmp(object->name, "i32DisCharHighCurr_A"))
  {
    //Get object value
    value->integer = entry->i32DisCharHighCurr_A;
  }
  //i32CharHighTemp_A object?
  else if(!strcmp(object->name, "i32CharHighTemp_A"))
  {
    //Get object value
    value->integer = entry->i32CharHighTemp_A;
  }
  //i32DisCharHighTemp_A object?
  else if(!strcmp(object->name, "i32DisCharHighTemp_A"))
  {
    //Get object value
    value->integer = entry->i32DisCharHighTemp_A;
  }
  //i32CharLowTemp_A object?
  else if(!strcmp(object->name, "i32CharLowTemp_A"))
  {
    //Get object value
    value->integer = entry->i32CharLowTemp_A;
  }
  //i32DisCharLowtemp_A object?
  else if(!strcmp(object->name, "i32DisCharLowtemp_A"))
  {
    //Get object value
    value->integer = entry->i32DisCharLowtemp_A;
  }
  //i32LowCap_A object?
  else if(!strcmp(object->name, "i32LowCap_A"))
  {
    //Get object value
    value->integer = entry->i32LowCap_A;
  }
  //i32BMSHighTemp_A object?
  else if(!strcmp(object->name, "i32BMSHighTemp_A"))
  {
    //Get object value
    value->integer = entry->i32BMSHighTemp_A;
  }
  //i32BMSLowTemp_A object?
  else if(!strcmp(object->name, "i32BMSLowTemp_A"))
  {
    //Get object value
    value->integer = entry->i32BMSLowTemp_A;
  }
  //i32PackOverVolt_P object?
  else if(!strcmp(object->name, "i32PackOverVolt_P"))
  {
    //Get object value
    value->integer = entry->i32PackOverVolt_P;
  }
  //i32BattOverVolt_P object?
  else if(!strcmp(object->name, "i32BattOverVolt_P"))
  {
    //Get object value
    value->integer = entry->i32BattOverVolt_P;
  }
  //i32CellOverVolt_P object?
  else if(!strcmp(object->name, "i32CellOverVolt_P"))
  {
    //Get object value
    value->integer = entry->i32CellOverVolt_P;
  }
  //i32BattUnderVolt_P object?
  else if(!strcmp(object->name, "i32BattUnderVolt_P"))
  {
    //Get object value
    value->integer = entry->i32BattUnderVolt_P;
  }
  //i32CellUnderVolt_P object?
  else if(!strcmp(object->name, "i32CellUnderVolt_P"))
  {
    //Get object value
    value->integer = entry->i32CellUnderVolt_P;
  }
  //i32CharOverCurr_P object?
  else if(!strcmp(object->name, "i32CharOverCurr_P"))
  {
    //Get object value
    value->integer = entry->i32CharOverCurr_P;
  }
  //i32DisCharOverCurr_P object?
  else if(!strcmp(object->name, "i32DisCharOverCurr_P"))
  {
    //Get object value
    value->integer = entry->i32DisCharOverCurr_P;
  }
  //i32CharOverTemp_P object?
  else if(!strcmp(object->name, "i32CharOverTemp_P"))
  {
    //Get object value
    value->integer = entry->i32CharOverTemp_P;
  }
  //i32DisCharOverTemp_P object?
  else if(!strcmp(object->name, "i32DisCharOverTemp_P"))
  {
    //Get object value
    value->integer = entry->i32DisCharOverTemp_P;
  }
  //i32CharUnderTemp_P object?
  else if(!strcmp(object->name, "i32CharUnderTemp_P"))
  {
    //Get object value
    value->integer = entry->i32CharUnderTemp_P;
  }
  //i32DisCharUnderTemp_P object?
  else if(!strcmp(object->name, "i32DisCharUnderTemp_P"))
  {
    //Get object value
    value->integer = entry->i32DisCharUnderTemp_P;
  }
  //i32UnderCap_P object?
  else if(!strcmp(object->name, "i32UnderCap_P"))
  {
    //Get object value
    value->integer = entry->i32UnderCap_P;
  }
  //i32BMSOverTemp_P object?
  else if(!strcmp(object->name, "i32BMSOverTemp_P"))
  {
    //Get object value
    value->integer = entry->i32BMSOverTemp_P;
  }
  //i32BMSUnderTemp_P object?
  else if(!strcmp(object->name, "i32BMSUnderTemp_P"))
  {
    //Get object value
    value->integer = entry->i32BMSUnderTemp_P;
  }
  //i32DifferentVolt_P object?
  else if(!strcmp(object->name, "i32DifferentVolt_P"))
  {
    //Get object value
    value->integer = entry->i32DifferentVolt_P;
  }
  //u32VoltBalance object?
  else if(!strcmp(object->name, "u32VoltBalance"))
  {
    //Get object value
    value->integer = entry->u32VoltBalance;
  }
  //u32DeltaVoltBalance object?
  else if(!strcmp(object->name, "u32DeltaVoltBalance"))
  {
    //Get object value
    value->integer = entry->u32DeltaVoltBalance;
  }
  //u32DisCharCurrLimit object?
  else if(!strcmp(object->name, "u32DisCharCurrLimit"))
  {
    //Get object value
    value->integer = entry->u32DisCharCurrLimit;
  }
  //u32CharCurrLimit object?
  else if(!strcmp(object->name, "u32CharCurrLimit"))
  {
    //Get object value
    value->integer = entry->u32CharCurrLimit;
  }
  //u32VoltDisCharRef object?
  else if(!strcmp(object->name, "u32VoltDisCharRef"))
  {
    //Get object value
    value->integer = entry->u32VoltDisCharRef;
  }
  //u32VoltDisCharCMD object?
  else if(!strcmp(object->name, "u32VoltDisCharCMD"))
  {
    //Get object value
    value->integer = entry->u32VoltDisCharCMD;
  }
  //u32SystemByte object?
  else if(!strcmp(object->name, "u32SystemByte"))
  {
    //Get object value
    value->integer = entry->u32SystemByte;
  }
  //u32KeyTime object?
  else if(!strcmp(object->name, "u32KeyTime"))
  {
    //Get object value
    value->integer = entry->u32KeyTime;
  }
  //u32IniMaxim object?
  else if(!strcmp(object->name, "u32IniMaxim"))
  {
    //Get object value
    value->integer = entry->u32IniMaxim;
  }
  //u32EnableKCS object?
  else if(!strcmp(object->name, "u32EnableKCS"))
  {
    //Get object value
    value->integer = entry->u32EnableKCS;
  }
  //u32EnableLock object?
  else if(!strcmp(object->name, "u32EnableLock"))
  {
    //Get object value
    value->integer = entry->u32EnableLock;
  }
  //u32ADCPackVolt object?
  else if(!strcmp(object->name, "u32ADCPackVolt"))
  {
    //Get object value
    value->integer = entry->u32ADCPackVolt;
  }
  //u32ADCBattVolt object?
  else if(!strcmp(object->name, "u32ADCBattVolt"))
  {
    //Get object value
    value->integer = entry->u32ADCBattVolt;
  }
  //i32ADCPackCurr object?
  else if(!strcmp(object->name, "i32ADCPackCurr"))
  {
    //Get object value
    value->integer = entry->i32ADCPackCurr;
  }
  //i32ADCBattCurr object?
  else if(!strcmp(object->name, "i32ADCBattCurr"))
  {
    //Get object value
    value->integer = entry->i32ADCBattCurr;
  }
  //liBattSOC object?
  else if(!strcmp(object->name, "liBattSOC"))
  {
    //Get object value
    value->integer = entry->liBattSOC;
  }
  //u32SOHMaxim object?
  else if(!strcmp(object->name, "u32SOHMaxim"))
  {
    //Get object value
    value->integer = entry->u32SOHMaxim;
  }
  //u32FullCapRepMaxim object?
  else if(!strcmp(object->name, "u32FullCapRepMaxim"))
  {
    //Get object value
    value->integer = entry->u32FullCapRepMaxim;
  }
  //u32VoltMaxim object?
  else if(!strcmp(object->name, "u32VoltMaxim"))
  {
    //Get object value
    value->integer = entry->u32VoltMaxim;
  }
  //u32CurrMaxim object?
  else if(!strcmp(object->name, "u32CurrMaxim"))
  {
    //Get object value
    value->integer = entry->u32CurrMaxim;
  }
  //u32IKalamn object?
  else if(!strcmp(object->name, "u32IKalamn"))
  {
    //Get object value
    value->integer = entry->u32IKalamn;
  }
  //u32SOCKalamn object?
  else if(!strcmp(object->name, "u32SOCKalamn"))
  {
    //Get object value
    value->integer = entry->u32SOCKalamn;
  }
  //u32VpriKalamn object?
  else if(!strcmp(object->name, "u32VpriKalamn"))
  {
    //Get object value
    value->integer = entry->u32VpriKalamn;
  }
  //u32VmesKalamn object?
  else if(!strcmp(object->name, "u32VmesKalamn"))
  {
    //Get object value
    value->integer = entry->u32VmesKalamn;
  }
  //u32CapKalamn object?
  else if(!strcmp(object->name, "u32CapKalamn"))
  {
    //Get object value
    value->integer = entry->u32CapKalamn;
  }
  //u32InternalR0Kalamn object?
  else if(!strcmp(object->name, "u32InternalR0Kalamn"))
  {
    //Get object value
    value->integer = entry->u32InternalR0Kalamn;
  }
  //liionCapInit object?
  else if(!strcmp(object->name, "liionCapInit"))
  {
    //Get object value
    value->integer = entry->u16_liionCapInit;
  }
  
  //Return status code
  return error;
}


//private mib get next lib batt info entry----------------------------------
error_t privateMibGetNextLIBattInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.liBattGroup.liBattInstalledPack; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

//gen info function=========================================================================================

//private mib set gen group----------------------------------------------------
error_t privateMibSetGenGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
    PrivateMibGenInfoGroup *entry;
    
    //	Point to the liBattGroup entry
    entry = &private_mib_base_struct.genGroup;
    
    if(!strcmp(object->name, "genInstalledGen"))
    {
      if((value->integer >= 0) && (value->integer <= 2))
      {
          //Get object value
          entry->genInstalledGen = value->integer; 
          if (entry->genInstalledGen != modbus_manager_struct.number_of_gen)
          {
              modbus_manager_struct.number_of_gen = entry->genInstalledGen;
              parameter.device_number[TOTAL_GEN_INDEX] = modbus_manager_struct.number_of_gen;
              working_default_parameter.device_number[TOTAL_GEN_INDEX] = parameter.device_number[TOTAL_GEN_INDEX];
              flag_struct.save_config_flag |=1;
          }
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    else
    {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
    }
    
    //Successful processing
    return NO_ERROR;    
}

//private mib set gen info entry------------------------------------------------
error_t privateMibSetGenInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
    error_t error;
    size_t n;
    uint_t index;
    uint_t j;
    MODBUS_PARAMETER_STRUCT sModbusDev[3];
    PrivateMibGenInfoEntry *entry;

    //Point to the instance identifier
    n = object->oidLen;
    //The ifIndex is used as instance identifier
    error = mibDecodeIndex(oid, oidLen, &n, &index);//1
    //error = mibDecodeIndex(oid, oidLen, &n, &index);//2
    //Invalid instance identifier?
    if(error) return error;

    //Sanity check
    if(n != oidLen)
        return ERROR_INSTANCE_NOT_FOUND;

    //Check index range
    if(index < 1 || index > 2)
        return ERROR_INSTANCE_NOT_FOUND;

    //Point to the libTable entry
    entry = &private_mib_base_struct.genGroup.genTable[index - 1];
    //genStatusType object?
    if(!strcmp(object->name, "genStatusType"))
    {
      if((value->integer >= 0) && (value->integer <= 5))
      {
        //Set object value
        entry->genStatusType= value->integer;
        j = index;
        modbus_manager_struct.gen_manager_struct[j-1].gen_type = entry->genStatusType;
        
        switch(modbus_manager_struct.gen_manager_struct[j-1].gen_type)
        {
        case 1:// KUBOTA
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
//            modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[1] = 708;
//            modbus_manager_struct.gen_manager_struct[j-1].gen_base_addr[2] = 800;
            modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[0] = 60;   
//            modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[1] = 6;   
//            modbus_manager_struct.gen_manager_struct[j-1].gen_num_of_reg[2] = 7; 
        }
        break;
        };
        sModbusDev[j-1].device_vendor = modbus_manager_struct.gen_manager_struct[j-1].gen_type;
        sModbusDev[j-1].slave_id = modbus_manager_struct.gen_manager_struct[j-1].gen_abs_slave_id;  
        //
        // Did parameters change?
        //
        if  (parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor != sModbusDev[j-1].device_vendor)
        {
          //
          // Update the current parameters with the new settings.
          //
          parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor = sModbusDev[j-1].device_vendor;
          parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].slave_id = sModbusDev[j-1].slave_id;
          //
          // Yes - save these settings as the defaults.
          //
          working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].device_vendor;
          working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB].slave_id;
           
          flag_struct.save_config_flag |=1;
        }      
        
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //genAutoMode
    else if(!strcmp(object->name, "genAutoMode"))
    {
      if(value->integer == 1)
      {
        //Set object value
        entry->genAutoMode = 1;
        private_mib_base_struct.genGroup.genflag = index - 1;
        modbus_manager_struct.setting_cmd = SET_GEN_AUTOMODE;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //genManualMode
    else if(!strcmp(object->name, "genManualMode"))
    {
      if(value->integer == 1)
      {
        //Set object value
        entry->genManualMode = 1;
        private_mib_base_struct.genGroup.genflag = index - 1;
        modbus_manager_struct.setting_cmd = SET_GEN_MANUALMODE;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //genStartMode
    else if(!strcmp(object->name, "genStartMode"))
    {
      if(value->integer == 1)
      {
        //Set object value
        entry->genStartMode = 1;
        private_mib_base_struct.genGroup.genflag = index - 1;
        modbus_manager_struct.setting_cmd = SET_GEN_STARTMODE;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //genStopMode
    else if(!strcmp(object->name, "genStopMode"))
    {
      if (value->integer == 1)
      {
        //Set object value
        entry->genStopMode = 1;
        private_mib_base_struct.genGroup.genflag = index - 1;
        modbus_manager_struct.setting_cmd = SET_GEN_STOPMODE;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    
    //Successful processing
    return NO_ERROR;
}

//private mib get gen info entry----------------------------------------------
error_t privateMibGetGenInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen)
{
    error_t error;
    size_t n;
    uint_t index;
    PrivateMibGenInfoEntry *entry;

    //Point to the instance identifier
    n = object->oidLen;
    //The ifIndex is used as instance identifier
    error = mibDecodeIndex(oid, oidLen, &n, &index);
    //Invalid instance identifier?
    if(error) return error;

    //Sanity check
    if(n != oidLen)
        return ERROR_INSTANCE_NOT_FOUND;

    //Check index range
    if(index < 1 || index > 10)
        return ERROR_INSTANCE_NOT_FOUND;

    //Point to the interface table entry
    entry = &private_mib_base_struct.genGroup.genTable[index - 1];
    //genStatusID object?
    if(!strcmp(object->name, "genStatusID"))
    {
        //Get object value
        value->integer = entry->genStatusID;
    }
    //genStatusStatus object?
    else if(!strcmp(object->name, "genStatusStatus"))
    {
        //Get object value
        value->integer = entry->genStatusStatus;
    }
    //genStatusLNVolt1 object?
    else if(!strcmp(object->name, "genStatusLNVolt1"))
    {
        //Get object value
        value->integer = entry->genStatusLNVolt1;
    }
    //genStatusLNVolt2 object?
    else if(!strcmp(object->name, "genStatusLNVolt2"))
    {
        //Get object value
        value->integer = entry->genStatusLNVolt2;
    }
    //genStatusLNVolt3 object?
    else if(!strcmp(object->name, "genStatusLNVolt3"))
    {
        //Get object value
        value->integer = entry->genStatusLNVolt3;
    }
    //genStatusLNCurr1 object?
    else if(!strcmp(object->name, "genStatusLNCurr1"))
    {
        //Get object value
        value->integer = entry->genStatusLNCurr1;
    }
    //genStatusLNCurr2 object?
    else if(!strcmp(object->name, "genStatusLNCurr2"))
    {
        //Get object value
        value->integer = entry->genStatusLNCurr2;
    }
    //genStatusLNCurr3 object?
    else if(!strcmp(object->name, "genStatusLNCurr3"))
    {
        //Get object value
        value->integer = entry->genStatusLNCurr3;
    }
    //genStatusLNPower1 object?
    else if(!strcmp(object->name, "genStatusLNPower1"))
    {
        //Get object value
        value->integer = entry->genStatusLNPower1;
    }
    //genStatusLNPower2 object?
    else if(!strcmp(object->name, "genStatusLNPower2"))
    {
        //Get object value
        value->integer = entry->genStatusLNPower2;
    }
    //genStatusLNPower3 object?
    else if(!strcmp(object->name, "genStatusLNPower3"))
    {
        //Get object value
        value->integer = entry->genStatusLNPower3;
    }
    //genStatusFrequency object?
    else if(!strcmp(object->name, "genStatusFrequency"))
    {
        //Get object value
        value->integer = entry->genStatusFrequency;
    }
    //genStatusBattVolt object?
    else if(!strcmp(object->name, "genStatusBattVolt"))
    {
        //Get object value
        value->integer = entry->genStatusBattVolt;
    }
    //genStatusCoolantTemp object?
    else if(!strcmp(object->name, "genStatusCoolantTemp"))
    {
        //Get object value
        value->integer = entry->genStatusCoolantTemp;
    }
    //genStatusOilPressure object?
    else if(!strcmp(object->name, "genStatusOilPressure"))
    {
        //Get object value
        value->integer = entry->genStatusOilPressure;
    }
    //genStatusFuelLevel object?
    else if(!strcmp(object->name, "genStatusFuelLevel"))
    {
        //Get object value
        value->integer = entry->genStatusFuelLevel;
    }
    //genStatusRunTime object?
    else if(!strcmp(object->name, "genStatusRunTime"))
    {
        //Get object value
        value->integer = entry->genStatusRunTime;
    }
    //genStatusOilTemp object?
    else if(!strcmp(object->name, "genStatusOilTemp"))
    {
        //Get object value
        value->integer = entry->genStatusOilTemp;
    }
    //genStatusMaintenanceTime object?
    else if(!strcmp(object->name, "genStatusMaintenanceTime"))
    {
        //Get object value
        value->integer = entry->genStatusMaintenanceTime;
    }
    //genStatusEngineSpeed object?
    else if(!strcmp(object->name, "genStatusEngineSpeed"))
    {
        //Get object value
        value->integer = entry->genStatusEngineSpeed;
    }
    //genStatusWorkingHour object?
    else if(!strcmp(object->name, "genStatusWorkingHour"))
    {
        //Get object value
        value->integer = entry->genStatusWorkingHour;
    }
    //genStatusWorkingMin object?
    else if(!strcmp(object->name, "genStatusWorkingMin"))
    {
        //Get object value
        value->integer = entry->genStatusWorkingMin;
    }
    //genStatusActiveEnergyLow object?
    else if(!strcmp(object->name, "genStatusActiveEnergyLow"))
    {
        //Get object value
        value->integer = entry->genStatusActiveEnergyLow;
    }
    //genStatusActiveEnergyHigh object?
    else if(!strcmp(object->name, "genStatusActiveEnergyHigh"))
    {
        //Get object value
        value->integer = entry->genStatusActiveEnergyHigh;
    }
    //genStatusAlarmStatus object?
    else if(!strcmp(object->name, "genStatusAlarmStatus"))
    {
        //Get object value
        value->counter32 = entry->genStatusAlarmStatus.all;
    } 
    //genStatusType object?
    else if(!strcmp(object->name, "genStatusType"))
    {
        //Get object value
        value->integer = entry->genStatusType;
    }   
    //genStatusSerial object?
    else if(!strcmp(object->name, "genStatusSerial"))
    {   
      //Make sure the buffer is large enough to hold the entire object
      if(*valueLen >= private_mib_base_struct.genGroup.genTable[0].genSerialLen)
      {
          //Copy object value
          memcpy(value->octetString, private_mib_base_struct.genGroup.genTable[0].genSerial, private_mib_base_struct.genGroup.genTable[0].genSerialLen);
          //Return object length
          *valueLen = private_mib_base_struct.genGroup.genTable[0].genSerialLen;
      }
      else
      {
          //Report an error
          error = ERROR_BUFFER_OVERFLOW;
      }
    }   
    //genStatusModbusID object?
    else if(!strcmp(object->name, "genStatusModbusID"))
    {
        //Get object value
        value->integer = entry->genStatusModbusID;
    }  
    //genStatusLNSPower1 object?
    else if(!strcmp(object->name, "genStatusLNSPower1"))
    {
        //Get object value
        value->integer = entry->genStatusLNSPower1;
    }  
    //genStatusLNSPower2 object?
    else if(!strcmp(object->name, "genStatusLNSPower2"))
    {
        //Get object value
        value->integer = entry->genStatusLNSPower2;
    }  
    //genStatusLNSPower3 object?
    else if(!strcmp(object->name, "genStatusLNSPower3"))
    {
        //Get object value
        value->integer = entry->genStatusLNSPower3;
    }  
    //genAutoMode object?
    else if(!strcmp(object->name, "genAutoMode"))
    {
        //Get object value
        value->integer = entry->genAutoMode;
    } 
    //genManualMode object?
    else if(!strcmp(object->name, "genManualMode"))
    {
        //Get object value
        value->integer = entry->genManualMode;
    }  
    //genStartMode object?
    else if(!strcmp(object->name, "genStartMode"))
    {
        //Get object value
        value->integer = entry->genStartMode;
    }  
    //genStopMode object?
    else if(!strcmp(object->name, "genStopMode"))
    {
        //Get object value
        value->integer = entry->genStopMode;
    }  
    //Return status code
    return error;
}


//private mib get next gen info entry------------------------------------------
error_t privateMibGetNextGenInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
    error_t error;
    size_t n;
    uint_t index;

    //Make sure the buffer is large enough to hold the OID prefix
    if(*nextOidLen < object->oidLen)
        return ERROR_BUFFER_OVERFLOW;

    //Copy OID prefix
    memcpy(nextOid, object->oid, object->oidLen);

    //Loop through network interfaces
    for(index = 1; index <= private_mib_base_struct.genGroup.genInstalledGen; index++)
    {
        //Append the instance identifier to the OID prefix
        n = object->oidLen;

        //The ifIndex is used as instance identifier
        error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
        //Any error to report?
        if(error) return error;

        //Check whether the resulting object identifier lexicographically
        //follows the specified OID
        if(oidComp(nextOid, n, oid, oidLen) > 0)
        {
            //Save the length of the resulting object identifier
            *nextOidLen = n;
            //Next object found
            return NO_ERROR;
        }
    }

    //The specified OID does not lexicographically precede the name
    //of some object
    return ERROR_OBJECT_NOT_FOUND;
}

//pm info function=============================================================================================

//private mib get next pm info group--------------------------------------------
error_t privateMibSetPMGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  PrivateMibPMGroup *entry;
  
  //	Point to the liBattGroup entry
  entry = &private_mib_base_struct.pmGroup;
  
  if(!strcmp(object->name, "pmInstalledPM"))
  {
    if((value->integer >= 0) && (value->integer <= 2))
    {
      //Get object value
      entry->pmInstalledPM = value->integer; 
      if (entry->pmInstalledPM != modbus_manager_struct.number_of_pm)
      {
        modbus_manager_struct.number_of_pm = entry->pmInstalledPM;
        parameter.device_number[TOTAL_PM_INDEX] = modbus_manager_struct.number_of_pm;
        working_default_parameter.device_number[TOTAL_PM_INDEX] = parameter.device_number[TOTAL_PM_INDEX];
        flag_struct.save_config_flag |=1;
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR; 
}

//private mib set pm info entry-------------------------------------------------
error_t privateMibSetPMInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  uint_t j;
  MODBUS_PARAMETER_STRUCT sModbusDev[3];
  PrivateMibPMInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 2)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the libTable entry
  entry = &private_mib_base_struct.pmGroup.pmTable[index - 1];
  //pmType object?
  if(!strcmp(object->name, "pmType"))
  {
    if((value->integer >= 0) && (value->integer <= 15))
    {
      //Set object value
      entry->pmType= value->integer;
      j = index;
      modbus_manager_struct.pm_manager_struct[j-1].pm_type = entry->pmType;
      
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
      case 6:// FORLONG
        {  
          modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;  
          modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 0;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 22;                                
        }
        break;
      case 15:// PILOT_3PHASE
        {  
          modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset = 16;  
          modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id = 16+j;
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0] = 100;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0] = 43;
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1] = 1000;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1] = 12;     
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2] = 9000;
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
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0]  = 4416;//0x03 Q
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0]   = 8;
        }
        break;
      case 17://Schneider 2022
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
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[4] = 6;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[5]  = 3053;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[5] = 24;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[6]  = 3077;         
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[6] = 8;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[7]  = 3109;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[7] = 2;
        }
        break;
      case 18://EASTRON SMD72D 2022
        {
          modbus_manager_struct.pm_manager_struct[j-1].pm_slave_offset    = 16;
          modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id    = 16+j;     
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[0]    = 0;  
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[0]   = 30;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[1]    = 30;      
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[1]   = 30;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[2]    = 60;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[2]   = 30;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[3]    = 342;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[3]   = 30;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[4]    = 11; 
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[4]   = 30;
          
          modbus_manager_struct.pm_manager_struct[j-1].pm_base_addr[5]    = 64512;
          modbus_manager_struct.pm_manager_struct[j-1].pm_num_of_reg[5]   = 4;
        }
        break;
      };
      sModbusDev[j-1].device_vendor = modbus_manager_struct.pm_manager_struct[j-1].pm_type;
      sModbusDev[j-1].slave_id = modbus_manager_struct.pm_manager_struct[j-1].pm_abs_slave_id;  
      //
      // Did parameters change?
      //
      if  (parameter.modbus_parameter[j-1+ MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor != sModbusDev[j-1].device_vendor)
      {
        //
        // Update the current parameters with the new settings.
        //
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor = sModbusDev[j-1].device_vendor;
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].slave_id = sModbusDev[j-1].slave_id;
        //
        // Yes - save these settings as the defaults.
        //
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].device_vendor;
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM].slave_id;
        
        flag_struct.save_config_flag |=1;
      }      
      
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  // pmDeltaFreqDisConnect
  else if(!strcmp(object->name, "pmDeltaFreqDisConnect"))
  {
    if((value->integer >= 1) && (value->integer <= 10) && (value->integer > entry->pmDeltaFreqReConnect ))
    {
      //Set object value
      entry->pmDeltaFreqDisConnect= value->integer;
      parameter.u8DeltaFreqDisConnect[index - 1] = (uint8_t)value->integer;
      working_default_parameter.u8DeltaFreqDisConnect [index - 1] = parameter.u8DeltaFreqDisConnect[index - 1];
      flag_struct.save_config_flag |=1;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  // pmDeltaFreqReConnect
  else if(!strcmp(object->name, "pmDeltaFreqReConnect"))
  {
    if((value->integer >= 1) && (value->integer <= 10) && (value->integer < entry->pmDeltaFreqDisConnect ))
    {
      //Set object value
      entry->pmDeltaFreqReConnect= value->integer;
      parameter.u8DeltaFreqReConnect[index - 1] = (uint8_t)value->integer;
      working_default_parameter.u8DeltaFreqReConnect [index - 1] = parameter.u8DeltaFreqReConnect[index - 1];
      flag_struct.save_config_flag |=1;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  // pmEnableFreqTrap
  else if(!strcmp(object->name, "pmEnableFreqTrap"))
  {
    if((value->integer == 0) || (value->integer == 1))
    {
      //Set object value
      private_mib_base_struct.pmGroup.pmTable[0].pmEnableFreqTrap = value->integer;
      private_mib_base_struct.pmGroup.pmTable[1].pmEnableFreqTrap = value->integer;
      parameter.u8EnableFreqTrap = (uint8_t)value->integer;
      working_default_parameter.u8EnableFreqTrap = parameter.u8EnableFreqTrap;
      flag_struct.save_config_flag |=1;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //Successful processing
  return NO_ERROR;
  
}

//private mib get pm info entry-------------------------------------------------
error_t privateMibGetPMInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibPMInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > private_mib_base_struct.pmGroup.pmInstalledPM)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &private_mib_base_struct.pmGroup.pmTable[index - 1];
  //pmID object?
  if(!strcmp(object->name, "pmID"))
  {
    //Get object value
    value->integer = entry->pmID;
  }
  //pmStatus object?
  else if(!strcmp(object->name, "pmStatus"))
  {
    //Get object value
    value->integer = entry->pmStatus;
  }
  //pmImportActiveEnergy object?
  else if(!strcmp(object->name, "pmImportActiveEnergy"))
  {
    //Get object value
    value->integer = entry->pmImportActiveEnergy;
  }
  //pmExportActiveEnergy object?
  else if(!strcmp(object->name, "pmExportActiveEnergy"))
  {
    //Get object value
    value->integer = entry->pmExportActiveEnergy;
  }
  //pmImportReactiveEnergy object?
  else if(!strcmp(object->name, "pmImportReactiveEnergy"))
  {
    //Get object value
    value->integer = entry->pmImportReactiveEnergy;
  }
  //pmExportReactiveEnergy object?
  else if(!strcmp(object->name, "pmExportReactiveEnergy"))
  {
    //Get object value
    value->integer = entry->pmExportReactiveEnergy;
  }
  //pmTotalActiveEnergy object?
  else if(!strcmp(object->name, "pmTotalActiveEnergy"))
  {
    //Get object value
    value->integer = entry->pmTotalActiveEnergy;
  }
  //pmTotalReactiveEnergy object?
  else if(!strcmp(object->name, "pmTotalReactiveEnergy"))
  {
    //Get object value
    value->integer = entry->pmTotalReactiveEnergy;
  }
  //pmActivePower object?
  else if(!strcmp(object->name, "pmActivePower"))
  {
    //Get object value
    value->integer = entry->pmActivePower;
  }
  //pmReactivePower object?
  else if(!strcmp(object->name, "pmReactivePower"))
  {
    //Get object value
    value->integer = entry->pmReactivePower;
  }
  //pmPowerFactor object?
  else if(!strcmp(object->name, "pmPowerFactor"))
  {
    //Get object value
    value->integer = entry->pmPowerFactor;
  }
  //pmFrequency object?
  else if(!strcmp(object->name, "pmFrequency"))
  {
    //Get object value
    value->integer = entry->pmFrequency;
  }
  //pmPhase1Current object?
  else if(!strcmp(object->name, "pmPhase1Current"))
  {
    //Get object value
    value->integer = entry->pmPhase1Current;
  }
  //pmPhase1Voltage object?
  else if(!strcmp(object->name, "pmPhase1Voltage"))
  {
    //Get object value
    value->integer = entry->pmPhase1Voltage;
  }
  //pmPhase1RealPower object?
  else if(!strcmp(object->name, "pmPhase1RealPower"))
  {
    //Get object value
    value->integer = entry->pmPhase1RealPower;
  }
  //pmPhase1ApparentPower object?
  else if(!strcmp(object->name, "pmPhase1ApparentPower"))
  {
    //Get object value
    value->integer = entry->pmPhase1ApparentPower;
  }
  //pmPhase1ReactivePower object?
  else if(!strcmp(object->name, "pmPhase1ReactivePower"))
  {
    //Get object value
    value->integer = entry->pmPhase1ReactivePower;
  }
  //pmPhase1PowerFactor object?
  else if(!strcmp(object->name, "pmPhase1PowerFactor"))
  {
    //Get object value
    value->integer = entry->pmPhase1PowerFactor;
  }
  //pmPhase2Current object?
  else if(!strcmp(object->name, "pmPhase2Current"))
  {
    //Get object value
    value->integer = entry->pmPhase2Current;
  }
  //pmPhase2Voltage object?
  else if(!strcmp(object->name, "pmPhase2Voltage"))
  {
    //Get object value
    value->integer = entry->pmPhase2Voltage;
  }
  //pmPhase2RealPower object?
  else if(!strcmp(object->name, "pmPhase2RealPower"))
  {
    //Get object value
    value->integer = entry->pmPhase2RealPower;
  }
  //pmPhase2ApparentPower object?
  else if(!strcmp(object->name, "pmPhase2ApparentPower"))
  {
    //Get object value
    value->integer = entry->pmPhase2ApparentPower;
  }
  //pmPhase2ReactivePower object?
  else if(!strcmp(object->name, "pmPhase2ReactivePower"))
  {
    //Get object value
    value->integer = entry->pmPhase2ReactivePower;
  }
  //pmPhase2PowerFactor object?
  else if(!strcmp(object->name, "pmPhase2PowerFactor"))
  {
    //Get object value
    value->integer = entry->pmPhase2PowerFactor;
  }
  //pmPhase3Current object?
  else if(!strcmp(object->name, "pmPhase3Current"))
  {
    //Get object value
    value->integer = entry->pmPhase3Current;
  }
  //pmPhase3Voltage object?
  else if(!strcmp(object->name, "pmPhase3Voltage"))
  {
    //Get object value
    value->integer = entry->pmPhase3Voltage;
  }
  //pmPhase3RealPower object?
  else if(!strcmp(object->name, "pmPhase3RealPower"))
  {
    //Get object value
    value->integer = entry->pmPhase3RealPower;
  }
  //pmPhase3ApparentPower object?
  else if(!strcmp(object->name, "pmPhase3ApparentPower"))
  {
    //Get object value
    value->integer = entry->pmPhase3ApparentPower;
  }
  //pmPhase3ReactivePower object?
  else if(!strcmp(object->name, "pmPhase3ReactivePower"))
  {
    //Get object value
    value->integer = entry->pmPhase3ReactivePower;
  }
  //pmPhase3PowerFactor object?
  else if(!strcmp(object->name, "pmPhase3PowerFactor"))
  {
    //Get object value
    value->integer = entry->pmPhase3PowerFactor;
  }
  //pmType object?
  else if(!strcmp(object->name, "pmType"))
  {
    //Get object value
    value->integer = entry->pmType;
  }
  //pmSerial object?
  else if(!strcmp(object->name, "pmSerial"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->pmSerialLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->pmSerial, entry->pmSerialLen);
      //Return object length
      *valueLen = entry->pmSerialLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //pmModbusID object?
  else if(!strcmp(object->name, "pmModbusID"))
  {
    //Get object value
    value->integer = entry->pmModbusID;
  }
  //pmModel object?
  else if(!strcmp(object->name, "pmModel"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->pmModelLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->pmModel, entry->pmModelLen);
      //Return object length
      *valueLen = entry->pmModelLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //pmOutOfRangeFreq object?
  else if(!strcmp(object->name, "pmOutOfRangeFreq"))
  {
    //Get object value
    value->integer = entry->pmOutOfRangeFreq;
  }
  //pmDeltaFreqDisConnect object?
  else if(!strcmp(object->name, "pmDeltaFreqDisConnect"))
  {
    //Get object value
    value->integer = entry->pmDeltaFreqDisConnect;
  }
  //pmDeltaFreqReConnect object?
  else if(!strcmp(object->name, "pmDeltaFreqReConnect"))
  {
    //Get object value
    value->integer = entry->pmDeltaFreqReConnect;
  }
  //pmEnableFreqTrap object?
  else if(!strcmp(object->name, "pmEnableFreqTrap"))
  {
    //Get object value
    value->integer = entry->pmEnableFreqTrap;
  }
  //pmTotalCurrent object?
  else if(!strcmp(object->name, "pmTotalCurrent"))
  {
    //Get object value
    value->integer = entry->pmTotalCurrent;
  }
  
  //Return status code
  return error;
}

//private mib get next pm info entry-------------------------------------------
error_t privateMibGetNextPMInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.pmGroup.pmInstalledPM; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}


//smcb info function============================================================================================

//private mib set smcb group---------------------------------------------------
error_t privateMibSetSMCBGroup(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
    PrivateMibSMCBInfoGroup *entry;
    
    //	Point to the liBattGroup entry
    entry = &private_mib_base_struct.smcbGroup;
    
    if(!strcmp(object->name, "SmcbInstalledSMCB"))
    {
      if((value->integer >= 0) && (value->integer <= 5))
      {
          //Get object value
          entry->SmcbInstalledSMCB = value->integer; 
          if (entry->SmcbInstalledSMCB != modbus_manager_struct.number_of_smcb)
          {
              modbus_manager_struct.number_of_smcb = entry->SmcbInstalledSMCB;
              parameter.device_number[TOTAL_SMCB_INDEX] = modbus_manager_struct.number_of_smcb;
              working_default_parameter.device_number[TOTAL_SMCB_INDEX] = parameter.device_number[TOTAL_SMCB_INDEX];
              flag_struct.save_config_flag |=1;
          }
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    else
    {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
    }
    
    //Successful processing
    return NO_ERROR;   
}

//private mib set smcb info entry-----------------------------------------------
error_t privateMibSetSmcbInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  uint_t j;
  PrivateMibSMCBInfoEntry *entry;
  MODBUS_PARAMETER_STRUCT sModbusDev[6];
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 5)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the smcbTable entry
  entry = &private_mib_base_struct.smcbGroup.SmcbTable[index - 1];
  //SmcbState object?
  if(!strcmp(object->name, "SmcbState"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->SmcbState= value->integer;
      entry->SmcbStateWrite= value->integer;
      private_mib_base_struct.smcbGroup.flag = index - 1;
      modbus_manager_struct.smcb_info_struct[index - 1].u32State = value->integer;
      modbus_manager_struct.setting_cmd = SET_SMCB_STATE;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //SmcbType object?
  else if (!strcmp(object->name, "SmcbType"))
  {
    //Set object value
    entry->SmcbType= value->integer;
    j = index;
    modbus_manager_struct.smcb_manager_struct[j-1].smcb_type = entry->SmcbType;
    
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
        modbus_manager_struct.smcb_manager_struct[j-1].smcb_base_addr[0] = 1;                                       
      }
      break;
      
    case 3:// GOL
      { 
        modbus_manager_struct.smcb_manager_struct[j-1].smcb_slave_offset = 18;
        modbus_manager_struct.smcb_manager_struct[j-1].smcb_abs_slave_id = 18+j;
        modbus_manager_struct.smcb_manager_struct[j-1].smcb_base_addr[0] = 32768;
        modbus_manager_struct.smcb_manager_struct[j-1].smcb_base_addr[0] = 1;                                       
      }
      break;
    };
    sModbusDev[j-1].device_vendor = modbus_manager_struct.smcb_manager_struct[j-1].smcb_type;
    sModbusDev[j-1].slave_id = modbus_manager_struct.smcb_manager_struct[j-1].smcb_abs_slave_id;  
    //
    // Did parameters change?
    // luu vendor vao flash 
    if  (parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor != sModbusDev[j-1].device_vendor)
    {
      //
      // Update the current parameters with the new settings.
      //
      parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor = sModbusDev[j-1].device_vendor;
      parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].slave_id = sModbusDev[j-1].slave_id;
      //
      // Yes - save these settings as the defaults.
      //
      working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].device_vendor;
      working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC].slave_id;
      
      flag_struct.save_config_flag |=1;      
    }      
    
  }
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  //Successful processing
  return NO_ERROR;
}


//private mib get smcb---------------------------------------------------------
error_t privateMibGetSmcbInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen)
{
    error_t error;
    size_t n;
    uint_t index;
    PrivateMibSMCBInfoEntry *entry;

    //Point to the instance identifier
    n = object->oidLen;

    //The ifIndex is used as instance identifier
    error = mibDecodeIndex(oid, oidLen, &n, &index);
    //Invalid instance identifier?
    if(error) return error;

    //Sanity check
    if(n != oidLen)
        return ERROR_INSTANCE_NOT_FOUND;

    //Check index range
    if(index < 1 || index > 10)
        return ERROR_INSTANCE_NOT_FOUND;

    //Point to the interface table entry
    entry = &private_mib_base_struct.smcbGroup.SmcbTable[index - 1];
    //SmcbStatusID object?
    if(!strcmp(object->name, "SmcbStatusID"))
    {
        //Get object value
        value->integer = entry->SmcbStatusID;
    }
    //SmcbStatus object?
    else if(!strcmp(object->name, "SmcbStatus"))
    {
        //Get object value
        value->integer = entry->SmcbStatus;
    }
    //SmcbState object?
    else if(!strcmp(object->name, "SmcbState"))
    {
        //Get object value
        value->integer = entry->SmcbState;
    }
    //SmcbType object?
    else if(!strcmp(object->name, "SmcbType"))
    {
        //Get object value
        value->integer = entry->SmcbType;
    }
    //SmcbModbusID object?
    else if(!strcmp(object->name, "SmcbModbusID"))
    {
        //Get object value
        value->integer = entry->SmcbModbusID;
    }
      

    //Return status code
    return error;
}


//private mid get next smcb info entry-----------------------------------------
error_t privateMibGetNextSmcbInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
    error_t error;
    size_t n;
    uint_t index;

    //Make sure the buffer is large enough to hold the OID prefix
    if(*nextOidLen < object->oidLen)
        return ERROR_BUFFER_OVERFLOW;

    //Copy OID prefix
    memcpy(nextOid, object->oid, object->oidLen);

    //Loop through network interfaces
    for(index = 1; index <= private_mib_base_struct.smcbGroup.SmcbInstalledSMCB; index++)
    {
        //Append the instance identifier to the OID prefix
        n = object->oidLen;

        //The ifIndex is used as instance identifier
        error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
        //Any error to report?
        if(error) return error;

        //Check whether the resulting object identifier lexicographically
        //follows the specified OID
        if(oidComp(nextOid, n, oid, oidLen) > 0)
        {
            //Save the length of the resulting object identifier
            *nextOidLen = n;
            //Next object found
            return NO_ERROR;
        }
    }

    //The specified OID does not lexicographically precede the name
    //of some object
    return ERROR_OBJECT_NOT_FOUND;
}



//fuel info function======================================================================

//private mib set fuel group----------------------------------------------------
error_t privateMibSetFUELGroup(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{

    PrivateMibFUELInfoGroup *entry;
    
    //	Point to the fuelGroup entry
    entry = &private_mib_base_struct.fuelGroup;
    
    if(!strcmp(object->name, "FuelInstalledFUEL"))
    {
      if((value->integer >= 0) && (value->integer <= 2))
      {
          //Get object value
          entry->FuelInstalledFUEL = value->integer; 
          if (entry->FuelInstalledFUEL != modbus_manager_struct.number_of_fuel)
          {
              modbus_manager_struct.number_of_fuel = entry->FuelInstalledFUEL;
              parameter.device_number[TOTAL_FUEL_INDEX] = modbus_manager_struct.number_of_fuel;
              working_default_parameter.device_number[TOTAL_FUEL_INDEX] = parameter.device_number[TOTAL_FUEL_INDEX];
              flag_struct.save_config_flag |=1;
          }
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    else
    {
        //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
        //The specified object does not exist
        return ERROR_OBJECT_NOT_FOUND;
    }
    
    //Successful processing
    return NO_ERROR;   
}

//private mib set fuel group----------------------------------------------------
error_t privateMibSetFuelInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  uint_t j;
  PrivateMibFUELInfoEntry *entry;
  MODBUS_PARAMETER_STRUCT sModbusDev[6];
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index < 1 || index > 2)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the FuelTable entry
  entry = &private_mib_base_struct.fuelGroup.FuelTable[index - 1];
  //FuelType object?
  if (!strcmp(object->name, "FuelType"))
  {
    //Set object value
    entry->FuelType= value->integer;
    j = index;
    modbus_manager_struct.fuel_manager_struct[j-1].fuel_type = entry->FuelType;
    
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
      
    case 2:// 
      {                                     
      }
      break;
    };
    sModbusDev[j-1].device_vendor = modbus_manager_struct.fuel_manager_struct[j-1].fuel_type;
    sModbusDev[j-1].slave_id = modbus_manager_struct.fuel_manager_struct[j-1].fuel_abs_slave_id;  
    //
    // Did parameters change?
    // luu vendor vao flash 
    if  (parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor != sModbusDev[j-1].device_vendor)
    {
      //
      // Update the current parameters with the new settings.
      //
      parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor = sModbusDev[j-1].device_vendor;
      parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].slave_id = sModbusDev[j-1].slave_id;
      //
      // Yes - save these settings as the defaults.
      //
      working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].device_vendor;
      working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB+MAX_NUM_OF_GEN+MAX_NUM_OF_BM+MAX_NUM_OF_PM+MAX_NUM_OF_VAC+MAX_NUM_OF_SMCB].slave_id;
      
      flag_struct.save_config_flag |=1;      
    }      
    
  }
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  //Successful processing
  return NO_ERROR;
}


//private mib get fuel info entry-----------------------------------------------
error_t privateMibGetFuelInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, MibVariant *value, size_t *valueLen)
{
    error_t error;
    size_t n;
    uint_t index;
    PrivateMibFUELInfoEntry *entry;

    //Point to the instance identifier
    n = object->oidLen;

    //The ifIndex is used as instance identifier
    error = mibDecodeIndex(oid, oidLen, &n, &index);
    //Invalid instance identifier?
    if(error) return error;

    //Sanity check
    if(n != oidLen)
        return ERROR_INSTANCE_NOT_FOUND;

    //Check index range
    if(index < 1 || index > 10)
        return ERROR_INSTANCE_NOT_FOUND;

    //Point to the interface table entry
    entry = &private_mib_base_struct.fuelGroup.FuelTable[index - 1];
    //FuelStatusID object?
    if(!strcmp(object->name, "FuelStatusID"))
    {
        //Get object value
        value->integer = entry->FuelStatusID;
    }
    //FuelStatus object?
    else if(!strcmp(object->name, "FuelStatus"))
    {
        //Get object value
        value->integer = entry->FuelStatus;
    }
    //FuelType object?
    else if(!strcmp(object->name, "FuelType"))
    {
        //Get object value
        value->integer = entry->FuelType;
    }
    //FuelLevel object?
    else if(!strcmp(object->name, "FuelLevel"))
    {
        //Get object value
        value->integer = entry->FuelLevel;
    }
    //FuelModbusID object?
    else if(!strcmp(object->name, "FuelModbusID"))
    {
        //Get object value
        value->integer = entry->FuelModbusID;
    }
      

    //Return status code
    return error;
}

//private mib get next fuel info entry------------------------------------------
error_t privateMibGetNextFuelInfoEntry(const MibObject *object, const uint8_t *oid,size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
    error_t error;
    size_t n;
    uint_t index;

    //Make sure the buffer is large enough to hold the OID prefix
    if(*nextOidLen < object->oidLen)
        return ERROR_BUFFER_OVERFLOW;

    //Copy OID prefix
    memcpy(nextOid, object->oid, object->oidLen);

    //Loop through network interfaces
    for(index = 1; index <= private_mib_base_struct.fuelGroup.FuelInstalledFUEL; index++)
    {
        //Append the instance identifier to the OID prefix
        n = object->oidLen;

        //The ifIndex is used as instance identifier
        error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
        //Any error to report?
        if(error) return error;

        //Check whether the resulting object identifier lexicographically
        //follows the specified OID
        if(oidComp(nextOid, n, oid, oidLen) > 0)
        {
            //Save the length of the resulting object identifier
            *nextOidLen = n;
            //Next object found
            return NO_ERROR;
        }
    }

    //The specified OID does not lexicographically precede the name
    //of some object
    return ERROR_OBJECT_NOT_FOUND;
}

//isense info function======================================================================================

//private mib set isense group-------------------------------------------------
error_t privateMibSetISENSEGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  PrivateMibISENSEGroup *entry;
  
  //	Point to the liBattGroup entry
  entry = &private_mib_base_struct.isenseGroup;
  
  if(!strcmp(object->name, "isenseInstalledISENSE"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->isenseInstalledISENSE = value->integer; 
      if (entry->isenseInstalledISENSE != modbus_manager_struct.number_of_isense)
      {
        modbus_manager_struct.number_of_isense = entry->isenseInstalledISENSE;
        parameter.device_number[TOTAL_ISENSE_INDEX] = modbus_manager_struct.number_of_isense;
        working_default_parameter.device_number[TOTAL_ISENSE_INDEX] = parameter.device_number[TOTAL_ISENSE_INDEX];
        flag_struct.save_config_flag |=1;
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR; 
}

//private mib set isense info entry--------------------------------------------
error_t privateMibSetISENSEInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  uint_t j;
  MODBUS_PARAMETER_STRUCT sModbusDev[3];
  PrivateMibISENSEInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index != 1)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the libTable entry
  entry = &private_mib_base_struct.isenseGroup.isenseTable;
  //isenseType object?
  if(!strcmp(object->name, "isenseType"))
  {
    if((value->integer >= 0) && (value->integer <= 15))
    {
      //Set object value
      entry->isenseType= value->integer;
      j = index;
      modbus_manager_struct.isense_manager_struct[j-1].isense_type = entry->isenseType;
      
      switch(modbus_manager_struct.isense_manager_struct[j-1].isense_type)
      {
      case 1:// FORLONG
        {  
          modbus_manager_struct.isense_manager_struct[j-1].isense_slave_offset = 26;  
          modbus_manager_struct.isense_manager_struct[j-1].isense_abs_slave_id = 26+j;
          modbus_manager_struct.isense_manager_struct[j-1].isense_base_addr[0] = 0;
          modbus_manager_struct.isense_manager_struct[j-1].isense_num_of_reg[0] = 22;                                
        }
        break;
      };
      sModbusDev[j-1].device_vendor = modbus_manager_struct.isense_manager_struct[j-1].isense_type;
      sModbusDev[j-1].slave_id = modbus_manager_struct.isense_manager_struct[j-1].isense_abs_slave_id;  
      //
      // Did parameters change?
      //
      if  (parameter.modbus_parameter[j-1+ MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor != sModbusDev[j-1].device_vendor)
      {
        //
        // Update the current parameters with the new settings.
        //
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor = sModbusDev[j-1].device_vendor;
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].slave_id = sModbusDev[j-1].slave_id;
        //
        // Yes - save these settings as the defaults.
        //
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].device_vendor;
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL].slave_id;
        
        flag_struct.save_config_flag |=1;
      }      
      
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  // isenseDeltaFreqDisConnect
  else if(!strcmp(object->name, "isenseDeltaFreqDisConnect"))
  {
    if((value->integer >= 1) && (value->integer <= 10) && (value->integer > entry->isenseDeltaFreqReConnect ))
    {
      //Set object value
      entry->isenseDeltaFreqDisConnect= value->integer;
      parameter.u8DeltaFreqDisConnect2 = (uint8_t)value->integer;
      working_default_parameter.u8DeltaFreqDisConnect2 = parameter.u8DeltaFreqDisConnect2;
      flag_struct.save_config_flag |=1;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  // isenseDeltaFreqReConnect
  else if(!strcmp(object->name, "isenseDeltaFreqReConnect"))
  {
    if((value->integer >= 1) && (value->integer <= 10) && (value->integer < entry->isenseDeltaFreqDisConnect ))
    {
      //Set object value
      entry->isenseDeltaFreqReConnect= value->integer;
      parameter.u8DeltaFreqReConnect2 = (uint8_t)value->integer;
      working_default_parameter.u8DeltaFreqReConnect2 = parameter.u8DeltaFreqReConnect2;
      flag_struct.save_config_flag |=1;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  // isenseEnableFreqTrap
  else if(!strcmp(object->name, "isenseEnableFreqTrap"))
  {
    if((value->integer == 0) || (value->integer == 1))
    {
      //Set object value
      private_mib_base_struct.isenseGroup.isenseTable.isenseEnableFreqTrap = value->integer;
      parameter.u8EnableFreqTrap2 = (uint8_t)value->integer;
      working_default_parameter.u8EnableFreqTrap2 = parameter.u8EnableFreqTrap2;
      flag_struct.save_config_flag |=1;
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //Successful processing
  return NO_ERROR;
  
}

//private mib get isense info entry---------------------------------------------
error_t privateMibGetISENSEInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibISENSEInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index != 1)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &private_mib_base_struct.isenseGroup.isenseTable;
  //isenseID object?
  if(!strcmp(object->name, "isenseID"))
  {
    //Get object value
    value->integer = entry->isenseID;
  }
  //isenseStatus object?
  else if(!strcmp(object->name, "isenseStatus"))
  {
    //Get object value
    value->integer = entry->isenseStatus;
  }
  //isenseImportActiveEnergy object?
  else if(!strcmp(object->name, "isenseImportActiveEnergy"))
  {
    //Get object value
    value->integer = entry->isenseImportActiveEnergy;
  }
  //isenseExportActiveEnergy object?
  else if(!strcmp(object->name, "isenseExportActiveEnergy"))
  {
    //Get object value
    value->integer = entry->isenseExportActiveEnergy;
  }
  //isenseImportReactiveEnergy object?
  else if(!strcmp(object->name, "isenseImportReactiveEnergy"))
  {
    //Get object value
    value->integer = entry->isenseImportReactiveEnergy;
  }
  //isenseExportReactiveEnergy object?
  else if(!strcmp(object->name, "isenseExportReactiveEnergy"))
  {
    //Get object value
    value->integer = entry->isenseExportReactiveEnergy;
  }
  //isenseTotalActiveEnergy object?
  else if(!strcmp(object->name, "isenseTotalActiveEnergy"))
  {
    //Get object value
    value->integer = entry->isenseTotalActiveEnergy;
  }
  //isenseTotalReactiveEnergy object?
  else if(!strcmp(object->name, "isenseTotalReactiveEnergy"))
  {
    //Get object value
    value->integer = entry->isenseTotalReactiveEnergy;
  }
  //isenseActivePower object?
  else if(!strcmp(object->name, "isenseActivePower"))
  {
    //Get object value
    value->integer = entry->isenseActivePower;
  }
  //isenseReactivePower object?
  else if(!strcmp(object->name, "isenseReactivePower"))
  {
    //Get object value
    value->integer = entry->isenseReactivePower;
  }
  //isensePowerFactor object?
  else if(!strcmp(object->name, "isensePowerFactor"))
  {
    //Get object value
    value->integer = entry->isensePowerFactor;
  }
  //isenseFrequency object?
  else if(!strcmp(object->name, "isenseFrequency"))
  {
    //Get object value
    value->integer = entry->isenseFrequency;
  }
  //isenseCurrent object?
  else if(!strcmp(object->name, "isenseCurrent"))
  {
    //Get object value
    value->integer = entry->isenseCurrent;
  }
  //isenseVoltage object?
  else if(!strcmp(object->name, "isenseVoltage"))
  {
    //Get object value
    value->integer = entry->isenseVoltage;
  }
  //isenseApparentPower object?
  else if(!strcmp(object->name, "isenseApparentPower"))
  {
    //Get object value
    value->integer = entry->isenseApparentPower;
  }
  //isenseType object?
  else if(!strcmp(object->name, "isenseType"))
  {
    //Get object value
    value->integer = entry->isenseType;
  }
  //isenseSerial object?
  else if(!strcmp(object->name, "isenseSerial"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->isenseSerialLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->isenseSerial, entry->isenseSerialLen);
      //Return object length
      *valueLen = entry->isenseSerialLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //isenseModbusID object?
  else if(!strcmp(object->name, "isenseModbusID"))
  {
    //Get object value
    value->integer = entry->isenseModbusID;
  }
  //isenseModel object?
  else if(!strcmp(object->name, "isenseModel"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->isenseModelLen)
    {
      //Copy object value
      memcpy(value->octetString, entry->isenseModel, entry->isenseModelLen);
      //Return object length
      *valueLen = entry->isenseModelLen;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //isenseOutOfRangeFreq object?
  else if(!strcmp(object->name, "isenseOutOfRangeFreq"))
  {
    //Get object value
    value->integer = entry->isenseOutOfRangeFreq;
  }
  //isenseDeltaFreqDisConnect object?
  else if(!strcmp(object->name, "isenseDeltaFreqDisConnect"))
  {
    //Get object value
    value->integer = entry->isenseDeltaFreqDisConnect;
  }
  //isenseDeltaFreqReConnect object?
  else if(!strcmp(object->name, "isenseDeltaFreqReConnect"))
  {
    //Get object value
    value->integer = entry->isenseDeltaFreqReConnect;
  }
  //isenseEnableFreqTrap object?
  else if(!strcmp(object->name, "isenseEnableFreqTrap"))
  {
    //Get object value
    value->integer = entry->isenseEnableFreqTrap;
  }
  
  //Return status code
  return error;
}

//private mib get next isense info entry----------------------------------------
error_t privateMibGetNextISENSEInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.isenseGroup.isenseInstalledISENSE; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

//pm dc info function======================================================================================

//private mib set pm dc group--------------------------------------------------
error_t privateMibSet_pm_dc_Group(const MibObject *object, const uint8_t *oid,size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  PrivateMibPMDCGroup *entry;
  
  //	Point to the liBattGroup entry
  entry = &private_mib_base_struct.pm_dc_group;
  
  if(!strcmp(object->name, "pm_dc_Installed_pm_dc"))
  {
    if((value->integer >= 0) && (value->integer <= 1))
    {
      //Get object value
      entry->pm_dc_installed_PM_DC = value->integer; 
      if (entry->pm_dc_installed_PM_DC != modbus_manager_struct.number_of_pmdc)
      {
        modbus_manager_struct.number_of_pmdc = entry->pm_dc_installed_PM_DC;
        parameter.device_number[TOTAL_PMDC_INDEX] = modbus_manager_struct.number_of_pmdc;
        working_default_parameter.device_number[TOTAL_PMDC_INDEX] = parameter.device_number[TOTAL_PMDC_INDEX];
        flag_struct.save_config_flag |=1;
      }
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  else
  {
    //TRACE_ERROR("ERROR_OBJECT_NOT_FOUND!\r\n");
    //The specified object does not exist
    return ERROR_OBJECT_NOT_FOUND;
  }
  
  //Successful processing
  return NO_ERROR; 
}

// private mib set pm_dc info entry--------------------------------------------------
error_t privateMibSet_pm_dc_InfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
  error_t error;
  size_t n;
  uint_t index;
  uint_t j;
  MODBUS_PARAMETER_STRUCT sModbusDev[3];
  PrivateMibPMDCInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index != 1)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the libTable entry
  entry = &private_mib_base_struct.pm_dc_group.pm_dc_table;
  //isenseType object?
  if(!strcmp(object->name, "pm_dc_Type"))
  {
    if((value->integer >= 0) && (value->integer <= 15))
    {
      //Set object value
      entry->pm_dc_type = value->integer;
      j = index;
      modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_type = entry->pm_dc_type;
      
      switch(modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_type)
      {
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
      };
      sModbusDev[j-1].device_vendor = modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_type;
      sModbusDev[j-1].slave_id = modbus_manager_struct.pmdc_manager_struct[j-1].pmdc_abs_slave_id;  
      //
      // Did parameters change?
      //
      if  (parameter.modbus_parameter[j-1+ MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor != sModbusDev[j-1].device_vendor)
      {
        //
        // Update the current parameters with the new settings.
        //
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor = sModbusDev[j-1].device_vendor;
        parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].slave_id = sModbusDev[j-1].slave_id;
        //
        // Yes - save these settings as the defaults.
        //
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].device_vendor;
        working_default_parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].slave_id = parameter.modbus_parameter[j-1+MAX_NUM_OF_LIB + MAX_NUM_OF_GEN + MAX_NUM_OF_BM + MAX_NUM_OF_PM + MAX_NUM_OF_VAC + MAX_NUM_OF_SMCB + MAX_NUM_OF_FUEL+MAX_NUM_OF_ISENSE].slave_id;
        
        flag_struct.save_config_flag |=1;
      }      
      
    }
    else
    {
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  //Successful processing
  return NO_ERROR;
  
}

// private mib get pm dc info entry---------------------------------------------------
error_t privateMibGet_pm_dc_InfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error;
  size_t n;
  uint_t index;
  PrivateMibPMDCInfoEntry *entry;
  
  //Point to the instance identifier
  n = object->oidLen;
  
  //The ifIndex is used as instance identifier
  error = mibDecodeIndex(oid, oidLen, &n, &index);
  //Invalid instance identifier?
  if(error) return error;
  
  //Sanity check
  if(n != oidLen)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Check index range
  if(index != 1)
    return ERROR_INSTANCE_NOT_FOUND;
  
  //Point to the interface table entry
  entry = &private_mib_base_struct.pm_dc_group.pm_dc_table;
  //pm_dc_ID object?
  if(!strcmp(object->name, "pm_dc_ID"))
  {
    //Get object value
    value->integer = entry->pm_dc_id;
  }
  //pm_dc_Status object?
  else if(!strcmp(object->name, "pm_dc_Status"))
  {
    //Get object value
    value->integer = entry->pm_dc_status;
  }
  //pm_dc_ActiveEnergy object?
  else if(!strcmp(object->name, "pm_dc_ActiveEnergy"))
  {
    //Get object value
    value->integer = entry->pm_dc_active_energy;
  }
  //pm_dc_ActivePower object?
  else if(!strcmp(object->name, "pm_dc_ActivePower"))
  {
    //Get object value
    value->integer = entry->pm_dc_active_power;
  }
  //pm_dc_Current object?
  else if(!strcmp(object->name, "pm_dc_Current"))
  {
    //Get object value
    value->integer = entry->pm_dc_current;
  }
  //pm_dc_Voltage object?
  else if(!strcmp(object->name, "pm_dc_Voltage"))
  {
    //Get object value
    value->integer = entry->pm_dc_voltage;
  }
  //pm_dc_Type object?
  else if(!strcmp(object->name, "pm_dc_Type"))
  {
    //Get object value
    value->integer = entry->pm_dc_type;
  }
  //pm_dc_ModbusID object?
  else if(!strcmp(object->name, "pm_dc_ModbusID"))
  {
    //Get object value
    value->integer = entry->pm_dc_modbus_ID;
  }
  //pm_dc_Model object?
  else if(!strcmp(object->name, "pm_dc_Model"))
  {
    //Make sure the buffer is large enough to hold the entire object
    if(*valueLen >= entry->pm_dc_model_len)
    {
      //Copy object value
      memcpy(value->octetString, entry->pm_dc_model_, entry->pm_dc_model_len);
      //Return object length
      *valueLen = entry->pm_dc_model_len;
    }
    else
    {
      //Report an error
      error = ERROR_BUFFER_OVERFLOW;
    }
  }
  //Return status code
  return error;
}

// private mib get next pm dc info entry------------------------------------------------------
error_t privateMibGetNext_pm_dc_InfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
  error_t error;
  size_t n;
  uint_t index;
  
  //Make sure the buffer is large enough to hold the OID prefix
  if(*nextOidLen < object->oidLen)
    return ERROR_BUFFER_OVERFLOW;
  
  //Copy OID prefix
  memcpy(nextOid, object->oid, object->oidLen);
  
  //Loop through network interfaces
  for(index = 1; index <= private_mib_base_struct.pm_dc_group.pm_dc_installed_PM_DC; index++)
  {
    //Append the instance identifier to the OID prefix
    n = object->oidLen;
    
    //The ifIndex is used as instance identifier
    error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
    //Any error to report?
    if(error) return error;
    
    //Check whether the resulting object identifier lexicographically
    //follows the specified OID
    if(oidComp(nextOid, n, oid, oidLen) > 0)
    {
      //Save the length of the resulting object identifier
      *nextOidLen = n;
      //Next object found
      return NO_ERROR;
    }
  }
  
  //The specified OID does not lexicographically precede the name
  //of some object
  return ERROR_OBJECT_NOT_FOUND;
}

//bm info group====================================================================================

//private mib set bm info entry-------------------------------------------------
error_t privateMibSetBMInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{
    error_t error;
    size_t n;
    uint_t index;
    PrivateMibBMInfoEntry *entry;

    //Point to the instance identifier
    n = object->oidLen;

    //The ifIndex is used as instance identifier
    error = mibDecodeIndex(oid, oidLen, &n, &index);
    //Invalid instance identifier?
    if(error) return error;

    //Sanity check
    if(n != oidLen)
        return ERROR_INSTANCE_NOT_FOUND;

    //Check index range
    if(index < 1 || index > 16)
        return ERROR_INSTANCE_NOT_FOUND;

    //Point to the bmTable entry
    entry = &private_mib_base_struct.bmGroup.bmTable[0];//[index - 1];
    
    //bmVoltDiff object?
    if(!strcmp(object->name, "bmVoltDiff"))
    {
      if((value->integer >= 1) && (value->integer <= 1000))
      {
        //Get object value
        entry->bmVoltDiff= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32VoltDiff = entry->bmVoltDiff;
        modbus_manager_struct.setting_cmd = SET_BM_VOLTDIFF;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmMaxTemp object?
    else if(!strcmp(object->name, "bmMaxTemp"))
    {
      if((value->integer >= 1000) && (value->integer <= 8000))
      {
        //Get object value
        entry->bmMaxTemp= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32MaxTemp = entry->bmMaxTemp;
        modbus_manager_struct.setting_cmd = SET_BM_MAXTEMP;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmVoltThres object?
    else if(!strcmp(object->name, "bmVoltThres"))
    {
      if((value->integer >= 5000) && (value->integer <= 5900))
      {
        //Get object value
        entry->bmVoltThres= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32VoltThres = entry->bmVoltThres;
        modbus_manager_struct.setting_cmd = SET_BM_VOLTTHRES;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmCurrThres object?
    else if(!strcmp(object->name, "bmCurrThres"))
    {
      if((value->integer >= 0) && (value->integer <= 1000))
      {
        //Get object value
        entry->bmCurrThres= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32CurrThres = entry->bmCurrThres;
        modbus_manager_struct.setting_cmd = SET_BM_CURRTHRES;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmTimeThres object?
    else if(!strcmp(object->name, "bmTimeThres"))
    {
      if((value->integer >= 1) && (value->integer <= 300))
      {
        //Get object value
        entry->bmTimeThres= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32TimeThres = entry->bmTimeThres;
        modbus_manager_struct.setting_cmd = SET_BM_TIMETHRES;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmSOCThres object?
    else if(!strcmp(object->name, "bmSOCThres"))
    {
      if((value->integer >= 0) && (value->integer <= 100))
      {
        //Get object value
        entry->bmSOCThres= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32SOCThres = entry->bmSOCThres;
        modbus_manager_struct.setting_cmd = SET_BM_SOCTHRES;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmMinTemp object?
    else if(!strcmp(object->name, "bmMinTemp"))
    {
      if((value->integer >= -3000) && (value->integer <= 3000))
      {
        //Get object value
        entry->bmMinTemp= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32MinTemp = entry->bmMinTemp;
        modbus_manager_struct.setting_cmd = SET_BM_MINTEMP;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmLowCapTime object?
    else if(!strcmp(object->name, "bmLowCapTime"))
    {
      if((value->integer >= 0) && (value->integer <= 1000))
      {
        //Get object value
        entry->bmLowCapTime= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32LowCapTime = entry->bmLowCapTime;
        modbus_manager_struct.setting_cmd = SET_BM_LOWCAPTIME;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmHalfVoltAlarm object?
    else if(!strcmp(object->name, "bmHalfVoltAlarm"))
    {
      if((value->integer >= 1000) && (value->integer <= 6000))
      {
        //Get object value
        entry->bmHalfVoltAlarm= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32HalfVoltAlarm = entry->bmHalfVoltAlarm;
        modbus_manager_struct.setting_cmd = SET_BM_HALFVOLTALARM;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmRechargeVolt object?
    else if(!strcmp(object->name, "bmRechargeVolt"))
    {
      if((value->integer >= 5200) && (value->integer <= 5800))
      {
        //Get object value
        entry->bmRechargeVolt= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32RechargeVolt = entry->bmRechargeVolt;
        modbus_manager_struct.setting_cmd = SET_BM_RECHARGEVOLT;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //bmCurrBalanceThres object?
    else if(!strcmp(object->name, "bmCurrBalanceThres"))
    {
      if((value->integer >= 0) && (value->integer <= 200))
      {
        //Get object value
        entry->bmCurrBalanceThres= value->integer;
        modbus_manager_struct.bm_info_struct[0].u32CurrBalanceThres = entry->bmCurrBalanceThres;
        modbus_manager_struct.setting_cmd = SET_BM_CURRBALANCETHRES;
        modbus_manager_struct.bm_current_index = 0;
      }
      else
      {
        return ERROR_PARAMETER_OUT_OF_RANGE;
      }
    }
    //Unknown object?
    else
    {
        //The specified object does not exist
        error = ERROR_OBJECT_NOT_FOUND;
    }    
   
    //Successful processing
    return NO_ERROR;
}


//private mib get bm info entry--------------------------------------------------
error_t privateMibGetBMInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
    error_t error;
    size_t n;
    uint_t index;
    PrivateMibBMInfoEntry *entry;

    //Point to the instance identifier
    n = object->oidLen;

    //The ifIndex is used as instance identifier
    error = mibDecodeIndex(oid, oidLen, &n, &index);
    //Invalid instance identifier?
    if(error) return error;

    //Sanity check
    if(n != oidLen)
        return ERROR_INSTANCE_NOT_FOUND;

    //Check index range
    if(index < 1 || index > 16)
        return ERROR_INSTANCE_NOT_FOUND;

    //Point to the interface table entry
    entry = &private_mib_base_struct.bmGroup.bmTable[index - 1];
    //bmID object?
    if(!strcmp(object->name, "bmID"))
    {
        //Get object value
        value->integer = entry->bmID;
    }
    //bmStatus object?
    else if(!strcmp(object->name, "bmStatus"))
    {
        //Get object value
        value->integer = entry->bmStatus;
    }
    //bmBattVolt object?
    else if(!strcmp(object->name, "bmBattVolt"))
    {
        //Get object value
        value->integer = entry->bmBattVolt;
    }
    //bmPackVolt object?
    else if(!strcmp(object->name, "bmPackVolt"))
    {
        //Get object value
        value->integer = entry->bmPackVolt;
    }
    //bmPackCurr object?
    else if(!strcmp(object->name, "bmPackCurr"))
    {
        //Get object value
        value->integer = entry->bmPackCurr;
    }
    //bmCellVolt1 object?
    else if(!strcmp(object->name, "bmCellVolt1"))
    {
        //Get object value
        value->integer = entry->bmCellVolt1;
    }
    //bmCellVolt2 object?
    else if(!strcmp(object->name, "bmCellVolt2"))
    {
        //Get object value
        value->integer = entry->bmCellVolt2;
    }
    //bmCellVolt3 object?
    else if(!strcmp(object->name, "bmCellVolt3"))
    {
        //Get object value
        value->integer = entry->bmCellVolt3;
    }
    //bmCellVolt4 object?
    else if(!strcmp(object->name, "bmCellVolt4"))
    {
        //Get object value
        value->integer = entry->bmCellVolt4;
    }
    //bmCellTemp1 object?
    else if(!strcmp(object->name, "bmCellTemp1"))
    {
        //Get object value
        value->integer = entry->bmCellTemp1;
    }
    //bmCellTemp2 object?
    else if(!strcmp(object->name, "bmCellTemp2"))
    {
        //Get object value
        value->integer = entry->bmCellTemp2;
    }
    //bmCellTemp3 object?
    else if(!strcmp(object->name, "bmCellTemp3"))
    {
        //Get object value
        value->integer = entry->bmCellTemp3;
    }
    //bmCellTemp4 object?
    else if(!strcmp(object->name, "bmCellTemp4"))
    {
        //Get object value
        value->integer = entry->bmCellTemp4;
    }
    //bmCellTemp5 object?
    else if(!strcmp(object->name, "bmCellTemp5"))
    {
        //Get object value
        value->integer = entry->bmCellTemp5;
    }
    //bmCellTemp6 object?
    else if(!strcmp(object->name, "bmCellTemp6"))
    {
        //Get object value
        value->integer = entry->bmCellTemp6;
    }
    //bmAlarmStatus object?
    else if(!strcmp(object->name, "bmAlarmStatus"))
    {
        //Get object value
        value->counter32 = entry->bmAlarmStatus.all;
    }
    //bmBalanceStatus object?
    else if(!strcmp(object->name, "bmBalanceStatus"))
    {
        //Get object value
        value->integer = entry->bmBalanceStatus;
    }
    //bmBattStatus object?
    else if(!strcmp(object->name, "bmBattStatus"))
    {
        //Get object value
        value->integer = entry->bmBattStatus;
    }
    //bmSOC object?
    else if(!strcmp(object->name, "bmSOC"))
    {
        //Get object value
        value->integer = entry->bmSOC;
    }
    //bmSOH object?
    else if(!strcmp(object->name, "bmSOH"))
    {
        //Get object value
        value->integer = entry->bmSOH;
    }
    //bmDischargeTime object?
    else if(!strcmp(object->name, "bmDischargeTime"))
    {
        //Get object value
        value->integer = entry->bmDischargeTime;
    }
    //bmTotalRunTime object?
    else if(!strcmp(object->name, "bmTotalRunTime"))
    {
        //Get object value
        value->integer = entry->bmTotalRunTime;
    }
    //bmVoltDiff object?
    else if(!strcmp(object->name, "bmVoltDiff"))
    {
        //Get object value
        value->integer = entry->bmVoltDiff;
    }
    //bmMaxTemp object?
    else if(!strcmp(object->name, "bmMaxTemp"))
    {
        //Get object value
        value->integer = entry->bmMaxTemp;
    }
    //bmVoltThres object?
    else if(!strcmp(object->name, "bmVoltThres"))
    {
        //Get object value
        value->integer = entry->bmVoltThres;
    }
    //bmCurrThres object?
    else if(!strcmp(object->name, "bmCurrThres"))
    {
        //Get object value
        value->integer = entry->bmCurrThres;
    }
    //bmTimeThres object?
    else if(!strcmp(object->name, "bmTimeThres"))
    {
        //Get object value
        value->integer = entry->bmTimeThres;
    }
    //bmSOCThres object?
    else if(!strcmp(object->name, "bmSOCThres"))
    {
        //Get object value
        value->integer = entry->bmSOCThres;
    }
    //bmMinTemp object?
    else if(!strcmp(object->name, "bmMinTemp"))
    {
        //Get object value
        value->integer = entry->bmMinTemp;
    }
    //bmLowCapTime object?
    else if(!strcmp(object->name, "bmLowCapTime"))
    {
        //Get object value
        value->integer = entry->bmLowCapTime;
    }
    //bmTotalDisAH object?
    else if(!strcmp(object->name, "bmTotalDisAH"))
    {
        //Get object value
        value->integer = entry->bmTotalDisAH;
    }
    //bmHalfVoltAlarm object?
    else if(!strcmp(object->name, "bmHalfVoltAlarm"))
    {
        //Get object value
        value->integer = entry->bmHalfVoltAlarm;
    }
    //bmRechargeVolt object?
    else if(!strcmp(object->name, "bmRechargeVolt"))
    {
        //Get object value
        value->integer = entry->bmRechargeVolt;
    }
    //bmCurrBalanceThres object?
    else if(!strcmp(object->name, "bmCurrBalanceThres"))
    {
        //Get object value
        value->integer = entry->bmCurrBalanceThres;
    }

    //Return status code
    return error;
}


//private mib get next bm info entry--------------------------------------------
error_t privateMibGetNextBMInfoEntry(const MibObject *object, const uint8_t *oid, size_t oidLen, uint8_t *nextOid, size_t *nextOidLen)
{
    error_t error;
    size_t n;
    uint_t index;

    //Make sure the buffer is large enough to hold the OID prefix
    if(*nextOidLen < object->oidLen)
        return ERROR_BUFFER_OVERFLOW;

    //Copy OID prefix
    memcpy(nextOid, object->oid, object->oidLen);

    //Loop through network interfaces
    for(index = 1; index <= private_mib_base_struct.bmGroup.bmInstalledBM; index++)
    {
        //Append the instance identifier to the OID prefix
        n = object->oidLen;

        //The ifIndex is used as instance identifier
        error = mibEncodeIndex(nextOid, *nextOidLen, &n, index);
        //Any error to report?
        if(error) return error;

        //Check whether the resulting object identifier lexicographically
        //follows the specified OID
        if(oidComp(nextOid, n, oid, oidLen) > 0)
        {
            //Save the length of the resulting object identifier
            *nextOidLen = n;
            //Next object found
            return NO_ERROR;
        }
    }

    //The specified OID does not lexicographically precede the name
    //of some object
    return ERROR_OBJECT_NOT_FOUND;
}
//dpc fan info==================================================================
//privateMibGet_fan_dpc_Group---------------------------------------------------
error_t privateMibGet_fan_dpc_Group(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PrivateMibFANDPCInfo *entry;
  
  entry = &private_mib_base_struct.fan_dpc_info;
  //dpc_fan_env_temp object?
  if(!strcmp(object->name, "dpc_fan_env_temp"))
  {
    //Get object value
    value->integer = entry->mib.env_temp;
  }
  // dpc_fan_airflow_temp object?
  else if(!strcmp(object->name, "dpc_fan_airflow_temp"))
  {
    //Get object value
    value->integer = entry->mib.airflow_temp;
  }
  // dpc_fan_voltage object?
  else if(!strcmp(object->name, "dpc_fan_voltage"))
  {
    //Get object value
    value->integer = entry->mib.voltage;
  }
  // dpc_fan_working_current object?
  else if(!strcmp(object->name, "dpc_fan_working_current"))
  {
    //Get object value
    value->integer = entry->mib.working_current;
  }
  // dpc_fan_state_device object?
  else if(!strcmp(object->name, "dpc_fan_state_device"))
  {
    //Get object value
    value->integer = entry->mib.state_device;
  }
  // dpc_fan_state_fan object?
  else if(!strcmp(object->name, "dpc_fan_state_fan"))
  {
    //Get object value
    value->integer = entry->mib.state_fan;
  }
  // dpc_fan_alarm_fail_refrigeration object?
  else if(!strcmp(object->name, "dpc_fan_alarm_fail_refrigeration"))
  {
    //Get object value
    value->integer = entry->mib.alarm_fail_refrigeration;
  }
  // dpc_fan_alarm_high_temp object?
  else if(!strcmp(object->name, "dpc_fan_alarm_high_temp"))
  {
    //Get object value
    value->integer = entry->mib.alarm_high_temp;
  }
  // dpc_fan_alarm_low_temp object?
  else if(!strcmp(object->name, "dpc_fan_alarm_low_temp"))
  {
    //Get object value
    value->integer = entry->mib.alarm_low_temp;
  }
  // dpc_fan_alarm_fail_sensor_temp object?
  else if(!strcmp(object->name, "dpc_fan_alarm_fail_sensor_temp"))
  {
    //Get object value
    value->integer = entry->mib.alarm_fail_sensor_temp;
  }
  //dpc_fan_alarm_high_voltage object?
  else if(!strcmp(object->name, "dpc_fan_alarm_high_voltage"))
  {
    //Get object value
    value->integer = entry->mib.alarm_high_voltage;
  }
  //dpc_fan_alarm_low_voltage object?
  else if(!strcmp(object->name, "dpc_fan_alarm_low_voltage"))
  {
    //Get object value
    value->integer = entry->mib.alarm_low_voltage;
  }
  
  //dpc_fan_para_dc_starting_point object?
  else if(!strcmp(object->name, "dpc_fan_para_dc_starting_point"))
  {
    //Get object value
    value->integer = entry->mib.para_dc_starting_point;
  }
  //dpc_fan_para_dc_sensivive_point object?
  else if(!strcmp(object->name, "dpc_fan_para_dc_sensivive_point"))
  {
    //Get object value
    value->integer = entry->mib.para_dc_sensivive_point;
  }
  //dpc_fan_para_alarm_high_temp object?
  else if(!strcmp(object->name, "dpc_fan_para_alarm_high_temp"))
  {
    //Get object value
    value->integer = entry->mib.para_alarm_high_temp;
  }
  //dpc_fan_para_alarm_low_temp object?
  else if(!strcmp(object->name, "dpc_fan_para_alarm_low_temp"))
  {
    //Get object value
    value->integer = entry->mib.para_alarm_low_temp;
  }
  //dpc_fan_para_dc_heating_start_point object?
  else if(!strcmp(object->name, "dpc_fan_para_dc_heating_start_point"))
  {
    //Get object value
    value->integer = entry->mib.para_dc_heating_start_point;
  }
  //dpc_fan_para_dc_heating_sensitive_point object?
  else if(!strcmp(object->name, "dpc_fan_para_dc_heating_sensitive_point"))
  {
    //Get object value
    value->integer = entry->mib.para_dc_heating_sensivive;
  }
  //Unknown object?    
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//privateMibSet_fan_dpc_Group---------------------------------------------------
error_t privateMibSet_fan_dpc_Group(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
{

  PrivateMibFANDPCInfo *entry;
  
  //	Point to the cntGroup entry
  entry = &private_mib_base_struct.fan_dpc_info;
  
  //RS232ReInitEnable object?
  if(!strcmp(object->name, "dpc_fan_para_dc_starting_point"))
  {
    if((value->integer >= 2000) && (value->integer <= 4000)){
      
      entry->mib.temp_set = value->integer;
      entry->mib.para_dc_starting_point = entry->mib.temp_set;
      fan_info_dpc_t.para_dc_starting_point = entry->mib.temp_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_STARTING_POINT;
    }
    else{
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  else if(!strcmp(object->name, "dpc_fan_para_dc_sensivive_point"))
  {
    if((value->integer >= 0) && (value->integer <= 2500)){
      entry->mib.temp_set = value->integer;
      entry->mib.para_dc_sensivive_point = entry->mib.temp_set;
      fan_info_dpc_t.para_dc_sensivive_point = entry->mib.temp_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_SENSITIVE_POINT; 
    }
    else{
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }else if(!strcmp(object->name, "dpc_fan_para_alarm_high_temp"))
  {
    if((value->integer >= 3000) && (value->integer <= 6000)){
      entry->mib.temp_set = value->integer;
      entry->mib.para_alarm_high_temp = entry->mib.temp_set;
      fan_info_dpc_t.para_alarm_high_temp = entry->mib.temp_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_H_TEMP_W_POINT;
    }
    else{
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }else if(!strcmp(object->name, "dpc_fan_para_alarm_low_temp"))
  {
    if((value->integer >= -4500) && (value->integer <= 1000)){
      entry->mib.i_tem_set = value->integer;
      entry->mib.para_alarm_low_temp = entry->mib.i_tem_set;
      fan_info_dpc_t.para_alarm_low_temp = entry->mib.i_tem_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_L_TEMP_W_POINT; 
    }
    else{
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }      
  }
  if(!strcmp(object->name, "dpc_fan_para_dc_heating_start_point"))
  {
    if((value->integer >= -1000) && (value->integer <= 1500)){
      entry->mib.i_tem_set = value->integer;
      entry->mib.para_dc_heating_start_point = entry->mib.i_tem_set;
      fan_info_dpc_t.para_dc_heating_start_point = entry->mib.i_tem_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_HEATING_START_POINT;
    }
    else{
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  if(!strcmp(object->name, "dpc_fan_para_dc_heating_sensitive_point"))
  {
    if((value->integer >= 0) && (value->integer <= 3000)){
      entry->mib.i_tem_set = value->integer;
      entry->mib.para_dc_heating_sensivive = entry->mib.i_tem_set;
      fan_info_dpc_t.para_dc_heating_sensivive = entry->mib.i_tem_set;
      modbus_manager_struct.setting_cmd = SET_FAN_DPC_HEATING_SENSITIVE_POINT;
    }
    else{
      return ERROR_PARAMETER_OUT_OF_RANGE;
    }
  }
  else
  {
    
  }
  
  
  return NO_ERROR; 
}
//cnt============================================================================
//privateMibGetCntGroup---------------------------------------------------------
error_t privateMibGetCntGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, MibVariant *value, size_t *valueLen)
{
  error_t error = NO_ERROR;
  PrivateMibCntGroup *entry;
  
  entry = &private_mib_base_struct.cntGroup;
  //ZTEcnt object?
  if(!strcmp(object->name, "ZTEcnt"))
  {
    //Get object value
    value->integer = entry->task.zte;
  }
  // EMERcnt object?
  else if(!strcmp(object->name, "EMERcnt"))
  {
    //Get object value
    value->integer = entry->task.emerson_m500d;
  }
  // EMERV21cnt object?
  else if(!strcmp(object->name, "EMERV21cnt"))
  {
    //Get object value
    value->integer = entry->task.emerson_m221;
  }
  // HWcnt object?
  else if(!strcmp(object->name, "HWcnt"))
  {
    //Get object value
    value->integer = entry->task.agisson;
  }
  // ModbusRTUcnt object?
  else if(!strcmp(object->name, "ModbusRTUcnt"))
  {
    //Get object value
    value->integer = entry->task.modbus_rtu;
  }
  // EMERState object?
  else if(!strcmp(object->name, "EMERState"))
  {
    //Get object value
    value->integer = entry->EMERState;
  }
  // EMERV21State object?
  else if(!strcmp(object->name, "EMERV21State"))
  {
    //Get object value
    value->integer = entry->EMERV21State;
  }
  // ZTEState object?
  else if(!strcmp(object->name, "ZTEState"))
  {
    //Get object value
    value->integer = entry->ZTEState;
  }
  // HWState object?
  else if(!strcmp(object->name, "HWState"))
  {
    //Get object value
    value->integer = entry->HWState;
  }
  // HWRs485Cmd object?
  else if(!strcmp(object->name, "HWRs485Cmd"))
  {
    //Get object value
    value->integer = entry->HWRs485Cmd;
  }
  else if(!strcmp(object->name, "RS232ReInitEnable"))
  {
    //Get object value
    value->integer = entry->RS232ReInitEnable;
  }
  else if(!strcmp(object->name, "control_task_cnt"))
  {
    //Get object value
    value->integer = entry->task.control;
  }
  else if(!strcmp(object->name, "trap_task_cnt"))
  {
    //Get object value
    value->integer = entry->task.trap_send;
  }
  else if(!strcmp(object->name, "dpc_task_cnt"))
  {
    //Get object value
    value->integer = entry->task.dpc;
  }
  //Unknown object?
  else
  {
    //The specified object does not exist
    error = ERROR_OBJECT_NOT_FOUND;
  }
  
  //Return status code
  return error;
}

//error_t privateMibSetCntGroup(const MibObject *object, const uint8_t *oid, size_t oidLen, const MibVariant *value, size_t valueLen,bool_t commit)
//{
//  error_t error;
//  size_t n;
//  uint_t index;
//  PrivateMibCntGroup *entry;
//  
//  //	Point to the cntGroup entry
//  entry = &privateMibBase.cntGroup;
//  
//  //RS232ReInitEnable object?
//  if(!strcmp(object->name, "RS232ReInitEnable"))
//  {
//    if((value->integer == 0)||(value->integer == 1))
//    {
//      //Get object value
//      entry->RS232ReInitEnable= value->integer;
//      //flag_ReInitRs232=1;
//    }
//    else
//    {
//      return ERROR_PARAMETER_OUT_OF_RANGE;
//    }
//  }
//  
//  
//  return NO_ERROR; 
//}













