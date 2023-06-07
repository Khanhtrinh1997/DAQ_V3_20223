//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
uint16_t g_testchecksum=0xFFFF,g_checksum=0;
uint8_t u8InterruptCode = 0;
uint32_t RecvCount =0;
uint32_t u32IPAddr;
uint32_t u32GWAddr;
uint32_t u32SNAddr;
uint32_t u32SIPAddr;
void emersion_m500d_task(void* pv_parameters){
  private_mib_base_struct.cntGroup.task.emerson_m500d =0;
  
  
  uint32_t i,j;
  uint32_t fuse_flag;
  uint32_t temp[2];
  MESGState = SYSINFO_REQ;
  
  do
  {
    private_mib_base_struct.cntGroup.task.emerson_m500d++;
    
    if (sInterruptMesg.u8Checkbit == 1)
    {
      g_testchecksum = hex2byte2(&g_UARTRxBuf2[0],RecvCntStart2-5); 
      g_checksum = Checksum16(&g_UARTRxBuf2[0],RecvCntStart2);
      if (g_checksum == g_testchecksum)
      {
        u8InterruptCode = hex2byte(&g_UARTRxBuf2[0],3);
        switch(u8InterruptCode)
        {
        case 0x01:
          {
            MESGState = CALIB_MESG_1;
            
          } break;
        case 0x02:
          {
            u32IPAddr = hex2byte4(&g_UARTRxBuf2[0],13);
            u32SNAddr = hex2byte4(&g_UARTRxBuf2[0],21);
            u32GWAddr = hex2byte4(&g_UARTRxBuf2[0],29);
            u32SIPAddr = hex2byte4(&g_UARTRxBuf2[0],37);
            
            //
            // If we are now using static IP, check for modifications to the IP
            // addresses and mask.
            //
            
            if((parameter.ip_parameter.static_ip != u32IPAddr) ||
               (parameter.ip_parameter.gateway_ip != u32GWAddr) ||
                 (parameter.ip_parameter.subnet_mask != u32SNAddr) ||
                   (parameter.ip_parameter.snmp_ip != u32SIPAddr ))
            {
              parameter.ip_parameter.static_ip = u32IPAddr;
              parameter.ip_parameter.gateway_ip = u32GWAddr;
              parameter.ip_parameter.subnet_mask = u32SNAddr;
              parameter.ip_parameter.snmp_ip = u32SIPAddr;
              //
              // Yes - save these settings as the defaults.
              //
              working_default_parameter.ip_parameter.static_ip = parameter.ip_parameter.static_ip;
              working_default_parameter.ip_parameter.gateway_ip = parameter.ip_parameter.gateway_ip;
              working_default_parameter.ip_parameter.subnet_mask = parameter.ip_parameter.subnet_mask;
              working_default_parameter.ip_parameter.snmp_ip = parameter.ip_parameter.snmp_ip;
              config_save();  
            }
            MESGState = CALIB_MESG_2;
          } break;
        case 0x03:
          {
            parameter2.u8IsWritten = 0x53;
            parameter2.u8UserMAC[0] = hex2byte(&g_UARTRxBuf2[0],13);
            parameter2.u8UserMAC[1] = hex2byte(&g_UARTRxBuf2[0],15);
            parameter2.u8UserMAC[2] = hex2byte(&g_UARTRxBuf2[0],17);
            parameter2.u8UserMAC[3] = hex2byte(&g_UARTRxBuf2[0],19);
            parameter2.u8UserMAC[4] = hex2byte(&g_UARTRxBuf2[0],21);
            parameter2.u8UserMAC[5] = hex2byte(&g_UARTRxBuf2[0],23);
            snprintf((char*)parameter2.u8UserSerialNo,20,"%s",&g_UARTRxBuf2[25]);                
            parameter2.u32FirmwareCRC_old = hex2byte4(&g_UARTRxBuf2[0],45);
            parameter2.u32FirmwareCRC = parameter2.u32FirmwareCRC_old;
            sFLASH_EraseSector(FLASH_CFG_START_SN);
            sFLASH_WritePage((uint8_t *)ptr_default_parameter_2, FLASH_CFG_START_SN, FLASH_PB_SIZE);
            
            
            sFLASH_ReadBuffer((uint8_t *)&g_u8ReadBackBuffer[0],FLASH_CFG_START_SN, FLASH_PB_SIZE);
            
            if (g_u8ReadBackBuffer[0] == 0x53)
            {
              working_default_parameter_2 = *(SYSTEM_CONFIG_STRUCT_2 *)g_u8ReadBackBuffer;
              MESGState = CALIB_MESG_3;                  
            }
            
          } break;
        case 0x04:
          {
            sTestRS485.requesttestRS485 =1;
            if(sTestRS485.InitUart==0)
            {
              
              init_rs485_dc_cabinet(BAUD_RATE_9600,NONE_PARITY);
              DIR_MODBUS_AB_DC_CABINET_OFF
              sTestRS485.InitUart = 0xAA;
            } 
            sTestRS485.Cnt=0;
            sTestRS485.RS485CabRecv =0;
            sTestRS485.RS485GenRecv =0;
            sTestRS485.CabRecvCnt=0;
            sTestRS485.GenRecvCnt=0;
            for(i=0;i<15;i++)
            {
              sTestRS485.CabBuff[i]=0;
            }
            for(i=0;i<15;i++)
            {
              sTestRS485.GenBuff[i]=0;
            }
            MESGState = CALIB_MESG_4;
            
          } break;
        default:
          break;
        }
        
      }
      sInterruptMesg.u8Checkbit = 0;
      
    }
    else if (sInterruptMesg.u8Checkbit == 0)
    {
      if (setCmd_flag == 1)
      {
        settingCommand |= setCmd_mask;
        switch(settingCommand)
        {
        case SET_FLTVOL:
          sBattInfo.sRAWBattInfo.fFltVoltCfg = (float)private_mib_base_struct.configGroup.cfgFloatVolt/100;
          setCommand(&g_setInfo[0],&cmdCodeTable[0][0], sBattInfo.sRAWBattInfo.fFltVoltCfg,0);
          for (i = 0; i < 32; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BSTVOL:
          sBattInfo.sRAWBattInfo.fBotVoltCfg = (float)private_mib_base_struct.configGroup.cfgBoostVolt/100;
          setCommand(&g_setInfo[0],&cmdCodeTable[1][0], sBattInfo.sRAWBattInfo.fBotVoltCfg,0);
          for (i = 0; i < 32; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_TEMPCOMP_VAL:
          sBattInfo.sRAWBattInfo.fTempCompVal = (float)private_mib_base_struct.configGroup.cfgTempCompValue;
          setCommand(&g_setInfo[0],&cmdCodeTable[2][0], sBattInfo.sRAWBattInfo.fTempCompVal,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_LLVD:
          sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = (float)private_mib_base_struct.configGroup.cfgLLVDVolt/100;
          setCommand(&g_setInfo[0],&cmdCodeTable[3][0], sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BLVD:
          sBattInfo.sRAWBattInfo.fLVDDV = (float)private_mib_base_struct.configGroup.cfgBLVDVolt/100;
          setCommand(&g_setInfo[0],&cmdCodeTable[4][0], sBattInfo.sRAWBattInfo.fLVDDV,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_DCLOW:
          sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = (float)private_mib_base_struct.configGroup.cfgDCLowVolt/100;
          setCommand(&g_setInfo[0],&cmdCodeTable[5][0], sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_DCUNDER:
          sBattInfo.sRAWBattInfo.fDCUnderCfg = (float)private_mib_base_struct.configGroup.cfgDCUnderVolt/100;
          setCommand(&g_setInfo[0],&cmdCodeTable[6][0], sBattInfo.sRAWBattInfo.fDCUnderCfg,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_LLVD_EN:
          sBattInfo.sRAWBattInfo.u8LLVDE = private_mib_base_struct.configGroup.cfgLLVDEn;
          setCommand(&g_setInfo[0],&cmdCodeTable[72][0], sBattInfo.sRAWBattInfo.u8LLVDE,1);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_BLVD_EN:
          sBattInfo.sRAWBattInfo.u8BLVDE = private_mib_base_struct.configGroup.cfgBLVDEn;
          setCommand(&g_setInfo[0],&cmdCodeTable[73][0], sBattInfo.sRAWBattInfo.u8BLVDE,1);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_BATTSTDCAP:
          sBattInfo.sRAWBattInfo.fCapTotal = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal;
          setCommand(&g_setInfo[0],&cmdCodeTable[15][0], sBattInfo.sRAWBattInfo.fCapTotal,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_CCL:
          sBattInfo.sRAWBattInfo.fCCLVal = (float)private_mib_base_struct.configGroup.cfgCurrentLimit/1000;
          setCommand(&g_setInfo[0],&cmdCodeTable[17][0], sBattInfo.sRAWBattInfo.fCCLVal,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_OVERTEMP:
          sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = (float)private_mib_base_struct.configGroup.cfgHighMajorTempLevel/100;
          setCommand(&g_setInfo[0],&cmdCodeTable[30][0], sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_WIT_VAL:
          sRectInfo.u8WITI = private_mib_base_struct.configGroup.cfgWalkInTimeDuration;
          setCommand(&g_setInfo[0],&cmdCodeTable[36][0], sRectInfo.u8WITI,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_WIT_EN:
          sRectInfo.u8WITE = private_mib_base_struct.configGroup.cfgWalkInTimeEn;
          setCommand(&g_setInfo[0],&cmdCodeTable[37][0], sRectInfo.u8WITE,1);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_AC_THRES:
          sAcInfo.fAcLowThres = (float)private_mib_base_struct.configGroup.cfgAcLowLevel;
          setCommand(&g_setInfo[0],&cmdCodeTable[45][0], sAcInfo.fAcLowThres,0);
          for (i = 0; i < 32; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          //======================================== BATTERY TEST SETTING ============================================//
          
        case SET_BT_ENDVOLT:
          sBattTestInfo.fBattTestVolt = (float) private_mib_base_struct.cfgBTGroup.cfgBTEndVolt/10;
          setCommand(&g_setInfo[0],&cmdCodeTable[48][0], sBattTestInfo.fBattTestVolt,0);
          for (i = 0; i < 32; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BT_ENDCAP:
          sBattTestInfo.fTestEndCap = (float) private_mib_base_struct.cfgBTGroup.cfgBTEndCap/1000;
          setCommand(&g_setInfo[0],&cmdCodeTable[50][0], sBattTestInfo.fTestEndCap,0);
          for (i = 0; i < 32; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BT_ENDTIME:
          sBattTestInfo.fBattTestDur = (float) private_mib_base_struct.cfgBTGroup.cfgBTEndTime;
          setCommand(&g_setInfo[0],&cmdCodeTable[49][0], sBattTestInfo.fBattTestDur,0);
          for (i = 0; i < 32; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BT_PLEN:
          setCommand(&g_setInfo[0],&cmdCodeTable[51][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestEn,1);
          for (i = 0; i < 32; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BT_PLANTEST:
          if (sBattTestInfo.u8SetPlan == 1)
          {
            switch(sBattTestInfo.u8CheckSentAll)
            {
            case 1:
              {
                sBattTestInfo.u8CheckValidAll = 0;
                sBattTestInfo.u8CheckSentAll = 2;
              }
              break;
            case 2:
              {
                switch(private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestMonth)
                {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                  {
                    sBattTestInfo.u8CheckValidAll = 1;
                  }
                  break;
                case 4:
                case 6:
                case 9:
                case 11:
                  {
                    if (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestDate == 31)
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                case 2:
                  {
                    if ((private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestDate == 30) || (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestDate == 31))
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                }
                
                if (sBattTestInfo.u8CheckValidAll == 1)
                {
                  setCommand(&g_setInfo[0],&cmdCodeTable[60][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestMonth,1);
                  for (i = 0; i < 32; i++)
                  {
                    LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                  }
                  sBattTestInfo.u8CheckSentAll = 3;
                }
                else
                {
                  
                  sBattTestInfo.u8CheckSentAll = 0;
                  sBattTestInfo.u8SetPlan = 0;
                }
                
              }
              break;
            case 3:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[61][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestDate,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 4;
              }
              break;
            case 4:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[62][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestHour,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 0;
                sBattTestInfo.u8SetPlan = 0;
              }
              break;
            };
          }
          else if (sBattTestInfo.u8SetPlan == 2)
          {
            switch(sBattTestInfo.u8CheckSentAll)
            {
            case 1:
              {
                sBattTestInfo.u8CheckValidAll = 0;
                sBattTestInfo.u8CheckSentAll = 2;
              }
              break;
            case 2:
              {
                switch(private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestMonth)
                {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                  {
                    sBattTestInfo.u8CheckValidAll = 1;
                  }
                  break;
                case 4:
                case 6:
                case 9:
                case 11:
                  {
                    if (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestDate == 31)
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                case 2:
                  {
                    if ((private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestDate == 30) || (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestDate == 31))
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                }
                
                if (sBattTestInfo.u8CheckValidAll == 1)
                {
                  setCommand(&g_setInfo[0],&cmdCodeTable[63][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestMonth,1);
                  for (i = 0; i < 32; i++)
                  {
                    LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                  }
                  sBattTestInfo.u8CheckSentAll = 3;
                }
                else
                {
                  sBattTestInfo.u8CheckSentAll = 0;
                  sBattTestInfo.u8SetPlan = 0;
                }
                
              }
              break;
            case 3:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[64][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestDate,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 4;
              }
              break;
            case 4:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[65][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestHour,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 0;
                sBattTestInfo.u8SetPlan = 0;
              }
              break;
            };
          }
          else if (sBattTestInfo.u8SetPlan == 3)
          {
            switch(sBattTestInfo.u8CheckSentAll)
            {
            case 1:
              {
                sBattTestInfo.u8CheckValidAll = 0;
                sBattTestInfo.u8CheckSentAll = 2;
              }
              break;
            case 2:
              {
                switch(private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestMonth)
                {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                  {
                    sBattTestInfo.u8CheckValidAll = 1;
                  }
                  break;
                case 4:
                case 6:
                case 9:
                case 11:
                  {
                    if (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestDate == 31)
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                case 2:
                  {
                    if ((private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestDate == 30) || (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestDate == 31))
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                }
                
                if (sBattTestInfo.u8CheckValidAll == 1)
                {
                  setCommand(&g_setInfo[0],&cmdCodeTable[66][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestMonth,1);
                  for (i = 0; i < 32; i++)
                  {
                    LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                  }
                  sBattTestInfo.u8CheckSentAll = 3;
                }
                else
                {
                  sBattTestInfo.u8CheckSentAll = 0;
                  sBattTestInfo.u8SetPlan = 0;
                }
                
              }
              break;
            case 3:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[67][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestDate,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 4;
              }
              break;
            case 4:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[68][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestHour,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 0;
                sBattTestInfo.u8SetPlan = 0;
              }
              break;
            };
          }
          else if (sBattTestInfo.u8SetPlan == 4)
          {
            switch(sBattTestInfo.u8CheckSentAll)
            {
            case 1:
              {
                sBattTestInfo.u8CheckValidAll = 0;
                sBattTestInfo.u8CheckSentAll = 2;
              }
              break;
            case 2:
              {
                switch(private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestMonth)
                {
                case 1:
                case 3:
                case 5:
                case 7:
                case 8:
                case 10:
                case 12:
                  {
                    sBattTestInfo.u8CheckValidAll = 1;
                  }
                  break;
                case 4:
                case 6:
                case 9:
                case 11:
                  {
                    if (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestDate == 31)
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                case 2:
                  {
                    if ((private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestDate == 30) || (private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestDate == 31))
                    {
                      sBattTestInfo.u8CheckValidAll = 0;
                    }
                    else
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                  }
                  break;
                }
                
                if (sBattTestInfo.u8CheckValidAll == 1)
                {
                  setCommand(&g_setInfo[0],&cmdCodeTable[69][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestMonth,1);
                  for (i = 0; i < 32; i++)
                  {
                    LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                  }
                  sBattTestInfo.u8CheckSentAll = 3;
                }
                else
                {
                  sBattTestInfo.u8CheckSentAll = 0;
                  sBattTestInfo.u8SetPlan = 0;
                }
                
              }
              break;
            case 3:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[70][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestDate,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 4;
              }
              break;
            case 4:
              {
                setCommand(&g_setInfo[0],&cmdCodeTable[71][0], private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestHour,1);
                for (i = 0; i < 32; i++)
                {
                  LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
                }
                sBattTestInfo.u8CheckSentAll = 0;
                sBattTestInfo.u8SetPlan = 0;
              }
              break;
            };
          }
          break;
          //======================================== BATTERY TEST SETTING ============================================//
        case SET_BT_SCUTIME:
          {
            if (sBattTestInfo.u8SetPlan == 5)
            {
              switch(sBattTestInfo.u8CheckSentAll)
              {
              case 1:
                {
                  sBattTestInfo.u8CheckValidAll = 0;
                  sBattTestInfo.u8CheckSentAll = 2;
                }
                break;
              case 2:
                {
                  switch(private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMonth)
                  {
                  case 1:
                  case 3:
                  case 5:
                  case 7:
                  case 8:
                  case 10:
                  case 12:
                    {
                      sBattTestInfo.u8CheckValidAll = 1;
                    }
                    break;
                  case 4:
                  case 6:
                  case 9:
                  case 11:
                    {
                      if (private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUDate == 31)
                      {
                        sBattTestInfo.u8CheckValidAll = 0;
                      }
                      else
                      {
                        sBattTestInfo.u8CheckValidAll = 1;
                      }
                    }
                    break;
                  case 2:
                    {
                      if ((private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUDate == 30) || (private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUDate == 31))
                      {
                        sBattTestInfo.u8CheckValidAll = 0;
                      }
                      else
                      {
                        sBattTestInfo.u8CheckValidAll = 1;
                      }
                    }
                    break;
                  }
                  
                  if (sBattTestInfo.u8CheckValidAll == 1)
                  {
                    u82char(&scuTimeSet[13], (uint8_t)(private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUYear/100), 2); 
                    u82char(&scuTimeSet[15], (uint8_t)(private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUYear%100), 2);
                    u82char(&scuTimeSet[17], private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMonth, 2);
                    u82char(&scuTimeSet[19], private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUDate, 2);
                    u82char(&scuTimeSet[21], private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUHour, 2);
                    u82char(&scuTimeSet[23], private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMinute, 2);
                    u82char(&scuTimeSet[25], private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUSecond, 2);
                    
                    u162char(&scuTimeSet[27],Checksum16(&scuTimeSet[0],32),4);
                    
                    for (i = 0; i < 32; i++)
                    {
                      LPUART_WriteBlocking(LPUART5,&scuTimeSet[i],1);;
                    }
                  }
                  sBattTestInfo.u8CheckSentAll = 0;
                  sBattTestInfo.u8SetPlan = 0;
                  
                }
                break;
              };
            }
          }
          break;
          
        default:
          break;
        }
        if (sBattTestInfo.u8SetPlan == 0)
        {
          setCmd_mask = 0;
          setCmd_flag = 0;
          settingCommand = 0;
        }
      }
      else if (setCmd_flag == 0)
      {
        switch(MESGState)
        {
          //====================================== HISTORYINFO MESSAGE ======================================//
          
        case HISTORYMESG_REQ:
          
          u82char(&GetHistoryInfo[17], sHistoryInfo.ucSequenceNum, 2);
          u162char(&GetHistoryInfo[19],Checksum16(&GetHistoryInfo[0],24),4);
          
          for (i = 0; i < GetHistoryInfoLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetHistoryInfo[i],1);
          }
          
          MESGState = HISTORYMESG_RES;
          break;
          
        case HISTORYMESG_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            if((sHistoryInfo.ucSequenceNum == hex2byte(&RecvBuff[0],17)) && (sHistoryInfo.ucSequenceNum >= 0) && (sHistoryInfo.ucSequenceNum < 201))
            {
              sHistoryInfo.ucSequenceNum++;
              sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.u8ID = hex2byte(&RecvBuff[0],25);
              if (sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.u8ID != 0)
              {                          
                snprintf(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[0].cTimeFrame[0],18,"%02d-%02d-%02d,%02d:%02d:%02d",
                         hex2byte(&RecvBuff[0],27),
                         hex2byte(&RecvBuff[0],29),
                         hex2byte(&RecvBuff[0],31),
                         hex2byte(&RecvBuff[0],33),
                         hex2byte(&RecvBuff[0],35),
                         hex2byte(&RecvBuff[0],37)
                           );
                sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[0].cTimeFrame[18] = '\0';
                
                
                snprintf(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[1].cTimeFrame[0],18,"%02d-%02d-%02d,%02d:%02d:%02d",
                         hex2byte(&RecvBuff[0],39),
                         hex2byte(&RecvBuff[0],41),
                         hex2byte(&RecvBuff[0],43),
                         hex2byte(&RecvBuff[0],45),
                         hex2byte(&RecvBuff[0],47),
                         hex2byte(&RecvBuff[0],49)
                           );
                
                sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[1].cTimeFrame[18] = '\0';
                RectID_Calculation(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.cRectID[0], &RecvBuff[51]);
              }
              //                        else
              //                        {
              //                          memset(sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[1].cTimeFrame[0],0,19);
              //                          memset(sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[1].cTimeFrame[0],0,19);
              //                          memset(sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[1].cTimeFrame[0],0,19);
              //                        }
              
              //                        snprintf(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[0].cTimeFrame[0],18,"%02d-%02d-%02d,%02d:%02d:%02d",
              //                                        hex2byte(&RecvBuff[0],27),
              //                                        hex2byte(&RecvBuff[0],29),
              //                                        hex2byte(&RecvBuff[0],31),
              //                                        hex2byte(&RecvBuff[0],33),
              //                                        hex2byte(&RecvBuff[0],35),
              //                                        hex2byte(&RecvBuff[0],37)
              //                                        );
              //                        sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[0].cTimeFrame[18] = '\0';
              //
              //
              //                        snprintf(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[1].cTimeFrame[0],18,"%02d-%02d-%02d,%02d:%02d:%02d",
              //                                        hex2byte(&RecvBuff[0],39),
              //                                        hex2byte(&RecvBuff[0],41),
              //                                        hex2byte(&RecvBuff[0],43),
              //                                        hex2byte(&RecvBuff[0],45),
              //                                        hex2byte(&RecvBuff[0],47),
              //                                        hex2byte(&RecvBuff[0],49)
              //                                        );
              //
              //                        sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sTimeFrame[1].cTimeFrame[18] = '\0';
              //                        RectID_Calculation(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.cRectID[0], &RecvBuff[51]);
              switch (sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.u8ID)
              {
              case 0x00:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"               ",UCNAMESIZE+2);
                break;
              case 0x01:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse1BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x02:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse2BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x03:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse3BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x04:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse4BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x05:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse5BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x06:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse6BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x07://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse7BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x08://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse8BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x09://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Fuse9BreakAlarm",UCNAMESIZE+2);
                break;
              case 0x0A:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"AuxLoadAlarm",UCNAMESIZE+2);
                break;
              case 0x0B:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital1Alarm",UCNAMESIZE+2);
                break;
              case 0x0C://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital2Alarm",UCNAMESIZE+2);
                break;
              case 0x0D://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital3Alarm",UCNAMESIZE+2);
                break;
              case 0x0E:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital4Alarm",UCNAMESIZE+2);
                break;
              case 0x0F://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital5Alarm",UCNAMESIZE+2);
                break;
              case 0x10://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital6Alarm",UCNAMESIZE+2);
                break;
              case 0x11://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital7Alarm",UCNAMESIZE+2);
                break;
              case 0x12://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Digital8Alarm",UCNAMESIZE+2);
                break;
              case 0x13:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"DCVoltAlarm",UCNAMESIZE+2);
                break;
              case 0x14:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"AC1/UaVoltAlarm",UCNAMESIZE+2);
                break;
              case 0x15:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"AC1/UbVoltAlarm",UCNAMESIZE+2);
                break;
              case 0x16:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"AC1/UcVoltAlarm",UCNAMESIZE+2);
                break;
              case 0x1B:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Batt1OCAlarm",UCNAMESIZE+2);
                break;
              case 0x1C://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Batt2OCAlarm",UCNAMESIZE+2);
                break;
              case 0x1D://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"BattFuse1Alarm",UCNAMESIZE+2);
                break;
              case 0x1E://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"BattFuse2Alarm",UCNAMESIZE+2);
                break;
              case 0x1F://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"BattFuse3Alarm",UCNAMESIZE+2);
                break;
              case 0x20://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"BattFuse4Alarm",UCNAMESIZE+2);
                break;
              case 0x21:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"TemperatureAlarm",UCNAMESIZE+2);
                break;
              case 0x22:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"TemperatureAlarm",UCNAMESIZE+2);
                break;
              case 0x25:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"ManualAlarm",UCNAMESIZE+2);
                break;
              case 0x27:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"BattDischargeAlarm",UCNAMESIZE+2);
                break;
              case 0x28:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"CurrImbalanceAlarm",UCNAMESIZE+2);
                break;
              case 0x29:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect2FanFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x2B:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"LVD1",UCNAMESIZE+2);
                break;
              case 0x2C:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"LVD2",UCNAMESIZE+2);
                break;
              case 0x2D:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"VoltDiscrepancy",UCNAMESIZE+2);
                break;
              case 0x2E:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"ACFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x2F:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"MultiRectFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x38:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect1CommFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x39:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect2CommFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x3A://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect3CommFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x58:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect1DeratedAlarm",UCNAMESIZE+2);
                break;
              case 0x59://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect2DeratedAlarm",UCNAMESIZE+2);
                break;
              case 0x5A://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect3DeratedAlarm",UCNAMESIZE+2);
                break;
              case 0x68:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect1ACFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x69:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect2ACFailsAlarm",UCNAMESIZE+2);
                break;
              case 0x6A://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect3ACFailsAlarm",UCNAMESIZE+2);
                break;
              case 0xAB:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"RectLostAlarm",UCNAMESIZE+2);
                break;
              case 0xC8:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect1FailsAlarm",UCNAMESIZE+2);
                break;
              case 0xC9://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect2FailsAlarm",UCNAMESIZE+2);
                break;
              case 0xCA://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect3FailsAlarm",UCNAMESIZE+2);
                break;
              case 0xF8:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect1ProtectAlarm",UCNAMESIZE+2);
                break;
              case 0xF9://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect2ProtectAlarm",UCNAMESIZE+2);
                break;
              case 0xFA://************ Interpolating ***********//
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"Rect3ProtectAlarm",UCNAMESIZE+2);
                break;
              default:
                strncpy(&sHistoryInfo.sHistoryParam[sHistoryInfo.ucSequenceNum - 1].sAlarmContent.ucName[0],"ObservationAlarm",UCNAMESIZE+2);
                break;
              };                        
            }
          }
          
          RecvCntStart = 0;
          if (sHistoryInfo.ucSequenceNum < 200)
          {
            MESGState = HISTORYMESG_REQ;
          } 
          else
          {
            sHistoryInfo.ucSequenceNum = 0;
            MESGState = SYSINFO_REQ;
          }
          
          break;
          //====================================== HISTORYINFO MESSAGE ======================================//
          //====================================== SYSINFO MESSAGE ======================================//
          
        case  SYSINFO_REQ:
          for (i = 0; i < GetSysInfoLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetSysInfo[i],1);
          }
          MESGState = SYSINFO_RES;
          
          vTaskDelay(300);
          break;
          
        case  SYSINFO_RES:
          
          //                    memset(&RecvBuff[0],0,300);
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          
          //                    memset(&g_UARTRxBuf[0],0,300);
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sDcInfo.u32DCNoResponse = 0;
            private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 0;
            sDcInfo.sINFODcInfo.fVoltage = hex2float(&RecvBuff[0],17);
            sDcInfo.sINFODcInfo.fCurrent = hex2float(&RecvBuff[0],25);
            sDcInfo.sINFODcInfo.fBatt1Curr = hex2float(&RecvBuff[0],35);
            sDcInfo.sINFODcInfo.fBatt2Curr = hex2float(&RecvBuff[0],43);
            if((RecvBuff[43]==0x20)&&(RecvBuff[44]==0x20)&&(RecvBuff[45]==0x20)&&(RecvBuff[46]==0x20))
            {
              sDcInfo.sINFODcInfo.fBatt2Curr = 0;
            }
            
            sDcInfo.sINFODcInfo.fBatt1Volt = hex2float(&RecvBuff[0],55);
            sDcInfo.sINFODcInfo.fBatt2Volt = hex2float(&RecvBuff[0],63);
            sDcInfo.sINFODcInfo.fBatt1RmnCap = hex2float(&RecvBuff[0],71);
            sDcInfo.sINFODcInfo.fBatt2RmnCap = hex2float(&RecvBuff[0],79);
            sDcInfo.sINFODcInfo.fSen1BattTemp = hex2float(&RecvBuff[0],87);
            sDcInfo.sINFODcInfo.fSen2BattTemp = hex2float(&RecvBuff[0],95);
            sDcInfo.sINFODcInfo.fSen1AmbTemp = hex2float(&RecvBuff[0],103);
            sDcInfo.sINFODcInfo.fSen2AmbTemp = hex2float(&RecvBuff[0],111);
          }
          else
          {
            sDcInfo.u32DCNoResponse++;
            if (sDcInfo.u32DCNoResponse > 3)
            {
              private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 1;
              sDcInfo.u32DCNoResponse = 10;
              // xoa khi checksum sai
              sDcInfo.sINFODcInfo.fVoltage = 0;
              sDcInfo.sINFODcInfo.fCurrent = 0;
              sDcInfo.sINFODcInfo.fBatt1Curr = 0;
              sDcInfo.sINFODcInfo.fBatt2Curr = 0;
              sDcInfo.sINFODcInfo.fBatt2Curr = 0;
              sDcInfo.sINFODcInfo.fBatt1Volt = 0;
              sDcInfo.sINFODcInfo.fBatt2Volt = 0;
              sDcInfo.sINFODcInfo.fBatt1RmnCap = 0;
              sDcInfo.sINFODcInfo.fBatt2RmnCap = 0;
              sDcInfo.sINFODcInfo.fSen1BattTemp = 0;
              sDcInfo.sINFODcInfo.fSen2BattTemp = 0;
              sDcInfo.sINFODcInfo.fSen1AmbTemp = 0;
              sDcInfo.sINFODcInfo.fSen2AmbTemp = 0; 
              for (j = 24; j > 0; j--)
                sRectInfo.sRAWRectParam[j-1].fRect_DcCurr= 0;
              sRectInfo.sRAWRectParam[j-1].fRect_Temp = 0;
              sRectInfo.sRAWRectParam[j-1].fRect_LimCurr = 0;
              sRectInfo.sRAWRectParam[j-1].fRect_DcVolt = 0;
              sRectInfo.sRAWRectParam[j-1].fRect_AcVolt = 0;
              sAcInfo.facVolt[0] = 0;
              sAcInfo.facVolt[1] = 0;
              sAcInfo.facVolt[2] = 0;
              sBattInfo.sRAWBattInfo.fFltVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fBotVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fTempCompVal = 0;
              sBattInfo.sRAWBattInfo.u8LLVDE = 0;
              sBattInfo.sRAWBattInfo.u8BLVDE = 0;
              sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fLVDDV = 0;
              sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fDCUnderCfg = 0;  
              sBattInfo.sRAWBattInfo.fDCOverCfg = 0;
              sBattInfo.sRAWBattInfo.u8BankNo = 0;
              sBattInfo.sRAWBattInfo.fCapTotal = 0;
              sBattInfo.sRAWBattInfo.fCCLVal = 0; 
              sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = 0;
              sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel = 0;
              sRectInfo.u8WITI = 0;
              sRectInfo.u8WITE = 0;
              sAcInfo.fAcLowThres = 0;
              sAcInfo.fAcUnderThres = 0;
              sAcInfo.fAcHighThres = 0;
              sBattTestInfo.fBattTestVolt = 0;
              sBattTestInfo.fBattTestDur = 0;
              sBattTestInfo.fBattTestDur = 0;
              sBattTestInfo.u8TimeTestEn = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestMonth = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestDate = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestHour = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestMonth = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestDate = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestHour = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestMonth = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestDate = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestHour = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestMonth = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestDate = 0;
              private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestHour = 0;
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = RECTINFO_REQ;
          
          break;
          //====================================== SYSINFO MESSAGE ======================================//
          
          //====================================== RECTINFO MESSAGE ======================================//
        case  RECTINFO_REQ:
          for (i = 0; i < GetRecInfoLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetRecInfo[i],1);
          }
          MESGState = RECTINFO_RES;
          
          vTaskDelay(300);
          break;
        case  RECTINFO_RES:
          //                    memset(&RecvBuff[0],0,300);
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          
          //                    memset(&g_UARTRxBuf[0],0,300);
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sRectInfo.fAllRectDcVolt = hex2float(&RecvBuff[0],15);
            sRectInfo.u8Rect_Num = hex2byte(&RecvBuff[0],23);   
            
            j = sRectInfo.u8Rect_Num;
            if ((j <= 24) && (j > 0))
            {
              sRectInfo.u8Rect_InfoNum = hex2byte(&RecvBuff[0],33);                          
              
              while (j > 0)
              {  
                switch(sRectInfo.u8Rect_InfoNum)
                {
                case 0x03:
                  {
                    sRectInfo.sRAWRectParam[j-1].fRect_DcCurr= hex2float(&RecvBuff[0],25 + 34*(j-1));
                    sRectInfo.sRAWRectParam[j-1].fRect_Temp = 0;
                    sRectInfo.sRAWRectParam[j-1].fRect_LimCurr = hex2float(&RecvBuff[0],35 + 34*(j-1));
                    sRectInfo.sRAWRectParam[j-1].fRect_DcVolt = hex2float(&RecvBuff[0],43 + 34*(j-1));
                    sRectInfo.sRAWRectParam[j-1].fRect_AcVolt = hex2float(&RecvBuff[0],51 + 34*(j-1));
                    
                    j--;
                  }break;
                case 0x04:
                  {
                    sRectInfo.sRAWRectParam[j-1].fRect_DcCurr= hex2float(&RecvBuff[0],25 + 42*(j-1));
                    sRectInfo.sRAWRectParam[j-1].fRect_Temp = hex2float(&RecvBuff[0],35 + 42*(j-1));
                    sRectInfo.sRAWRectParam[j-1].fRect_LimCurr = hex2float(&RecvBuff[0],43 + 42*(j-1));
                    sRectInfo.sRAWRectParam[j-1].fRect_DcVolt = hex2float(&RecvBuff[0],51 + 42*(j-1));
                    sRectInfo.sRAWRectParam[j-1].fRect_AcVolt = hex2float(&RecvBuff[0],59 + 42*(j-1));
                    
                    j--;
                  }break;
                default:
                  break;
                }; 
              }
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = ACINFO_REQ;
          break;
          //====================================== RECTINFO MESSAGE ======================================//
          
          //====================================== ACINFO MESSAGE ======================================//
        case  ACINFO_REQ:
          for (i = 0; i < GetACInfoLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetACInfo[i],1);
          }
          MESGState = ACINFO_RES;
          break;
        case  ACINFO_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sAcInfo.facVolt[0] = hex2float(&RecvBuff[0],19);
            sAcInfo.facVolt[1] = hex2float(&RecvBuff[0],27);
            sAcInfo.facVolt[2] = hex2float(&RecvBuff[0],35);
          }
          
          RecvCntStart = 0;
          MESGState = ALARM_DC_REQ;
          break;
          //====================================== ACINFO MESSAGE ======================================//
          
          //====================================== ALARM DC MESSAGE ======================================//
        case  ALARM_DC_REQ:
          for (i = 0; i < GetAlarmDCLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetAlarmDC[i],1);
          }
          MESGState = ALARM_DC_RES;
          break;
        case  ALARM_DC_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sDcInfo.sALARMDcInfo.u8DC = hex2byte(&RecvBuff[0],17);
            sDcInfo.sALARMDcInfo.u8DC_NoFuse = hex2byte(&RecvBuff[0],19);
            sDcInfo.sALARMDcInfo.u8NumOfUsrDefInfo = hex2byte(&RecvBuff[0],41);
            
            for (i = 0; i < 10; i++)
            {
              sDcInfo.sALARMDcInfo.u8LoadFuse[i] = hex2byte(&RecvBuff[0],21 + 2*i);
            }
            
            for (i = 0; i < 4; i++)
            {
              sDcInfo.sALARMDcInfo.u8BattFuse[i] = hex2byte(&RecvBuff[0],43 + 2*i);
            }
            
            switch(sDcInfo.sALARMDcInfo.u8NumOfUsrDefInfo)
            {
            case 0x1A:
              {
                for (i = 0; i < 8; i++)
                {
                  sDcInfo.sALARMDcInfo.u8DIFuse[i] = hex2byte(&RecvBuff[0],79 + 2*i);
                }
                sDcInfo.sALARMDcInfo.u8Batt1_OC = hex2byte(&RecvBuff[0],51);
                sDcInfo.sALARMDcInfo.u8Batt2_OC = hex2byte(&RecvBuff[0],53);
                sDcInfo.sALARMDcInfo.u8BLVD = hex2byte(&RecvBuff[0],55);
                sDcInfo.sALARMDcInfo.u8LLVD = hex2byte(&RecvBuff[0],57);
                sDcInfo.sALARMDcInfo.u8Sen1_BattTemp = hex2byte(&RecvBuff[0],59);
                sDcInfo.sALARMDcInfo.u8Sen2_BattTemp = hex2byte(&RecvBuff[0],61);
                sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp = hex2byte(&RecvBuff[0],63);
                sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp = hex2byte(&RecvBuff[0],65);
                sDcInfo.sALARMDcInfo.u8Batt_Discharge = hex2byte(&RecvBuff[0],67);
                sDcInfo.sALARMDcInfo.u8OutVoltFault = hex2byte(&RecvBuff[0],77);                            
              }break;
            case 0x1F:
              {
                for (i = 0; i < 8; i++)
                {
                  sDcInfo.sALARMDcInfo.u8DIFuse[i] = hex2byte(&RecvBuff[0],89 + 2*i);
                }
                sDcInfo.sALARMDcInfo.u8Batt1_OC = hex2byte(&RecvBuff[0],51);
                sDcInfo.sALARMDcInfo.u8Batt2_OC = hex2byte(&RecvBuff[0],53);
                sDcInfo.sALARMDcInfo.u8BLVD = hex2byte(&RecvBuff[0],55);
                sDcInfo.sALARMDcInfo.u8LLVD = hex2byte(&RecvBuff[0],59);
                sDcInfo.sALARMDcInfo.u8Sen1_BattTemp = hex2byte(&RecvBuff[0],63);
                sDcInfo.sALARMDcInfo.u8Sen2_BattTemp = hex2byte(&RecvBuff[0],65);
                sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp = hex2byte(&RecvBuff[0],67);
                sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp = hex2byte(&RecvBuff[0],69);
                sDcInfo.sALARMDcInfo.u8Batt_Discharge = hex2byte(&RecvBuff[0],79);
                sDcInfo.sALARMDcInfo.u8OutVoltFault = hex2byte(&RecvBuff[0],87);  
                
              }break;
            default:
              break;
            };        
          }
          
          RecvCntStart = 0;
          MESGState = ALARM_RECT_REQ;
          break;
          //====================================== ALARM DC MESSAGE ======================================//
          
          //====================================== ALARM RECT MESSAGE ======================================//
        case  ALARM_RECT_REQ:
          for (i = 0; i < GetAlarmRectLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetAlarmRect[i],1);
          }
          MESGState = ALARM_RECT_RES;
          break;
        case  ALARM_RECT_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            //							sRectInfo.u8Rect_Num = hex2byte(&RecvBuff[0],15);
            j = sRectInfo.u8Rect_Num;
            if (j <= 24)
            {
              while (j > 0)
              {
                switch(sRectInfo.u8Rect_InfoNum)
                {
                case 0x03:
                  {
                    //                                  sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault = hex2byte(&RecvBuff[0],29 + 16*(j-1));//RecvBuff[30 + 16*(j-1)];
                    sRectInfo.sRAWRectParam[j-1].u8Rect_Fail = hex2byte(&RecvBuff[0],23 + 16*(j-1));
                    sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp = hex2byte(&RecvBuff[0],27 + 16*(j-1));
                    j--;
                  }break;
                case 0x04:
                  {
                    sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault = hex2byte(&RecvBuff[0],29 + 16*(j-1));//RecvBuff[30 + 16*(j-1)];
                    sRectInfo.sRAWRectParam[j-1].u8Rect_Fail = hex2byte(&RecvBuff[0],23 + 16*(j-1));
                    sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp = hex2byte(&RecvBuff[0],31 + 16*(j-1));
                    j--;
                  }break;
                default:
                  j--;
                  break;
                }; 
                
              }
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = ALARM_AC_REQ;
          break;
          //====================================== ALARM RECT MESSAGE ======================================//
          
          //====================================== ALARM AC MESSAGE ======================================//
        case  ALARM_AC_REQ:
          for (i = 0; i < GetAlarmACLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetAlarmAC[i],1);
          }
          MESGState = ALARM_AC_RES;
          break;
        case  ALARM_AC_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sAcInfo.u8Thres[0] = hex2byte(&RecvBuff[0],19);
            sAcInfo.u8Thres[1] = hex2byte(&RecvBuff[0],21);
            sAcInfo.u8Thres[2] = hex2byte(&RecvBuff[0],23);
            sAcInfo.u8MainFail = hex2byte(&RecvBuff[0],37);
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_FLOATVOLTAGE_REQ;//
          break;
          //====================================== ALARM AC MESSAGE ======================================//
          
          //====================================== ALARM MAN MESSAGE ======================================//
        case  ALARM_MAN_REQ:
          for (i = 0; i < GetAlarmManLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetAlarmMan[i],1);
          }
          MESGState = ALARM_MAN_RES;
          break;
        case  ALARM_MAN_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            g_AlrmMan = hex2byte(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = SYSINFO_REQ;//BATT_FLOATVOLTAGE_REQ;
          break;
          //====================================== ALARM MAN MESSAGE ======================================//
          
          
          //====================================== BatteryFloatVoltage MESSAGE ======================================//
        case BATT_FLOATVOLTAGE_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[0][0]);
          
          for (i = 0; i < 24; i++)
          {
            //                        athao[i]= g_getInfo[i];
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_FLOATVOLTAGE_RES;
          break;
          
        case BATT_FLOATVOLTAGE_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fFltVoltCfg = roundf(hex2float(&RecvBuff[0],13)*10)/10; 
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_BOOSTVOLTAGE_REQ;
          break;
          //====================================== BatteryFloatVoltage MESSAGE ======================================//
          
          //====================================== BatteryBoostVoltage MESSAGE ======================================//
        case BATT_BOOSTVOLTAGE_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[1][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_BOOSTVOLTAGE_RES;
          break;
          
        case BATT_BOOSTVOLTAGE_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fBotVoltCfg = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_TEMPCOMPEN_REQ;
          break;
          //====================================== BatteryBoostVoltage MESSAGE ======================================//
          
          //====================================== BatteryTemperatureCompensation MESSAGE ======================================//
        case BATT_TEMPCOMPEN_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[2][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_TEMPCOMPEN_RES;
          break;
          
        case BATT_TEMPCOMPEN_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fTempCompVal = hex2float(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_LLVDEN_REQ;
          break;
          //====================================== BatteryTemperatureCompensation MESSAGE ======================================//
          //====================================== LLVDEnable MESSAGE ======================================//
        case BATT_LLVDEN_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[72][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_LLVDEN_RES;
          break;
          
        case BATT_LLVDEN_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.u8LLVDE = hex2byte(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_BLVDEN_REQ;
          break;
          //====================================== LLVDEnable MESSAGE ======================================//
          //====================================== BLVDEnable MESSAGE ======================================//
        case BATT_BLVDEN_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[73][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_BLVDEN_RES;
          break;
          
        case BATT_BLVDEN_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.u8BLVDE = hex2byte(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_LLVD_REQ;
          break;
          //====================================== BLVDEnable MESSAGE ======================================//
          //   ====================================== BatteryLLVD MESSAGE ======================================//
        case BATT_LLVD_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[3][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_LLVD_RES;
          break;
          
        case BATT_LLVD_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_BLVD_REQ;
          break;
          //====================================== BatteryLLVD MESSAGE ======================================//
          
          //====================================== BatteryBLVD MESSAGE ======================================//
        case BATT_BLVD_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[4][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_BLVD_RES;
          break;
          
        case BATT_BLVD_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fLVDDV = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_DCLOW_REQ;
          break;
          //====================================== BatteryBLVD MESSAGE ======================================//
          //====================================== BatteryDCLOW MESSAGE ======================================//
        case BATT_DCLOW_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[5][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_DCLOW_RES;
          break;
          
        case BATT_DCLOW_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_DCUNDER_REQ;
          break;
          //====================================== BatteryDCLOW MESSAGE ======================================//
          //====================================== BatteryDCUNDER MESSAGE ======================================//
        case BATT_DCUNDER_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[6][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_DCUNDER_RES;
          break;
          
        case BATT_DCUNDER_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fDCUnderCfg = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_DCOVER_REQ;
          break;
          //====================================== BatteryDCUNDER MESSAGE ======================================//
          //====================================== BatteryDCOVER MESSAGE ======================================//
        case BATT_DCOVER_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[7][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_DCOVER_RES;
          break;
          
        case BATT_DCOVER_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fDCOverCfg = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_STRING_REQ;
          break;
          //====================================== BatteryDCOVER MESSAGE ======================================//
          //====================================== BatteryStringNo MESSAGE ======================================//
        case BATT_STRING_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[9][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_STRING_RES;
          break;
          
        case BATT_STRING_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.u8BankNo = hex2byte(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_STDCAP_REQ;
          break;
          //====================================== BatteryStringNo MESSAGE ======================================//
          
          //====================================== BatteryStandardCapacity MESSAGE ======================================//
        case BATT_STDCAP_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[15][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_STDCAP_RES;
          break;
          
        case BATT_STDCAP_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fCapTotal = hex2float(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_CURRLIMIT_REQ;
          break;
          //====================================== BatteryStandardCapacity MESSAGE ======================================//
          
          //====================================== BatteryCurrentLimitPoint MESSAGE ======================================//
        case BATT_CURRLIMIT_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[17][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_CURRLIMIT_RES;
          break;
          
        case BATT_CURRLIMIT_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fCCLVal = roundf(hex2float(&RecvBuff[0],13)*1000)/1000;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_HIGHTEMPALARM_REQ;
          break;
          //====================================== BatteryCurrentLimitPoint MESSAGE ======================================//
          
          //====================================== BatteryHighTempAlarm MESSAGE ======================================//
        case BATT_HIGHTEMPALARM_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[30][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_HIGHTEMPALARM_RES;
          break;
          
        case BATT_HIGHTEMPALARM_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_OVERTEMPALARM_REQ;
          break;
          //====================================== BatteryHighTempAlarm MESSAGE ======================================//
          //====================================== BatteryOverTemp MESSAGE ======================================//
        case BATT_OVERTEMPALARM_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[31][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_OVERTEMPALARM_RES;
          break;
          
        case BATT_OVERTEMPALARM_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          
          RecvCntStart = 0;
          MESGState = SYS_WALKINTIMEDUR_REQ;
          break;
          //====================================== BatteryOverTemp MESSAGE ======================================//
          //====================================== SystemWalkInTimeDuration MESSAGE ======================================//
        case SYS_WALKINTIMEDUR_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[36][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = SYS_WALKINTIMEDUR_RES;
          break;
          
        case SYS_WALKINTIMEDUR_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sRectInfo.u8WITI = (uint8_t)hex2float(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = SYS_WALKINTIMEEN_REQ;
          break;
          //====================================== SystemWalkInTimeDuration MESSAGE ======================================//
          
          //====================================== SystemWalkInTimeEnable MESSAGE ======================================//
        case SYS_WALKINTIMEEN_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[37][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = SYS_WALKINTIMEEN_RES;
          break;
          
        case SYS_WALKINTIMEEN_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sRectInfo.u8WITE = hex2byte(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = AC_LOWTHRES_REQ;//ATSINFO_REQ;
          break;
          
          //====================================== AC Low Thres MESSAGE ======================================//
        case AC_LOWTHRES_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[45][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = AC_LOWTHRES_RES;
          break;
          
        case AC_LOWTHRES_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sAcInfo.fAcLowThres = hex2float(&RecvBuff[0],13);
          }
          
          
          RecvCntStart = 0;
          MESGState = AC_UNDERTHRES_REQ;//ATSINFO_REQ;
          break;
          //====================================== AC Low Thres MESSAGE ======================================//
          //====================================== AC Under Thres MESSAGE ======================================//
        case AC_UNDERTHRES_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[46][0]);
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = AC_UNDERTHRES_RES;
          break;
          
        case AC_UNDERTHRES_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            //								sAcInfo.fAcHighThres = hex2float(&RecvBuff[0],13);
            sAcInfo.fAcUnderThres = hex2float(&RecvBuff[0],13);
            //								sAcInfo.fAcLowThres = hex2float(&RecvBuff[0],2);
          }
          
          
          RecvCntStart = 0;
          MESGState = AC_HITHRES_REQ;//ATSINFO_REQ;
          break;
          //====================================== AC Under Thres MESSAGE ======================================//
          //====================================== AC High Thres MESSAGE ======================================//
        case AC_HITHRES_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[47][0]);
          
          for (i = 0; i < 24; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = AC_HITHRES_RES;
          break;
          
        case AC_HITHRES_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sAcInfo.fAcHighThres = hex2float(&RecvBuff[0],13);
            //								sAcInfo.fAcHighThres = 280;
          }
          
          
          RecvCntStart = 0;
          MESGState = BATT_TESTVOLT_REQ ;//ATSINFO_REQ;
          break;
          //====================================== AC High Thres MESSAGE ======================================//
          //====================================== Battery Test Voltage MESSAGE ======================================//
        case BATT_TESTVOLT_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[48][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_TESTVOLT_RES;
          break;
          
        case BATT_TESTVOLT_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattTestInfo.fBattTestVolt = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          RecvCntStart = 0;
          MESGState = BATT_TESTDUR_REQ;//ATSINFO_REQ;
          break;
          //====================================== Battery Test Voltage MESSAGE ======================================//
          //====================================== Battery Test Duration MESSAGE ======================================//
        case BATT_TESTDUR_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[49][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = BATT_TESTDUR_RES;
          break;
          
        case BATT_TESTDUR_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattTestInfo.fBattTestDur = roundf(hex2float(&RecvBuff[0],13)*10)/10;
          }
          
          RecvCntStart = 0;
          MESGState = TEST_ENDCAP_REQ;//ATSINFO_REQ;
          break;
          //====================================== Battery Test Duration MESSAGE ======================================//
          //====================================== Test End Capacity ======================================//
        case TEST_ENDCAP_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[50][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST_ENDCAP_RES;
          break;
          
        case TEST_ENDCAP_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattTestInfo.fTestEndCap = roundf(hex2float(&RecvBuff[0],13)*1000)/1000;
          }
          
          RecvCntStart = 0;
          MESGState = TIME_TESTEN_REQ;//ATSINFO_REQ;
          break;
          //====================================== Test End Capacity ======================================//
          //====================================== Time Test Enable ======================================//
        case TIME_TESTEN_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[51][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TIME_TESTEN_RES;
          break;
          
        case TIME_TESTEN_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            sBattTestInfo.u8TimeTestEn = hex2byte(&RecvBuff[0],13);
            //								sAcInfo.fAcHighThres = 280;
          }
          
          RecvCntStart = 0;
          MESGState = TEST1_MONTH_REQ;//ATSINFO_REQ;
          break;
          //====================================== Time Test Enable ======================================//
          //====================================== Test Time 1 Month ======================================//
        case TEST1_MONTH_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[60][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST1_MONTH_RES;
          break;
          
        case TEST1_MONTH_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestMonth = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST1_DAY_REQ;
          break;
          //====================================== Test Time 1 Month ======================================//
          //====================================== Test Time 1 Day ======================================//
        case TEST1_DAY_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[61][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST1_DAY_RES;
          break;
          
        case TEST1_DAY_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestDate = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST1_HOUR_REQ;
          break;
          //====================================== Test Time 1 Day ======================================//
          //====================================== Test Time 1 Hour ======================================//
        case TEST1_HOUR_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[62][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST1_HOUR_RES;
          break;
          
        case TEST1_HOUR_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[0].cfgBTPlanTestHour = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST2_MONTH_REQ;
          break;
          //====================================== Test Time 1 Hour ======================================//
          //====================================== Test Time 2 Month ======================================//
        case TEST2_MONTH_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[63][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST2_MONTH_RES;
          break;
          
        case TEST2_MONTH_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestMonth = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST2_DAY_REQ;
          break;
          //====================================== Test Time 2 Month ======================================//
          //====================================== Test Time 2 Day ======================================//
        case TEST2_DAY_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[64][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST2_DAY_RES;
          break;
          
        case TEST2_DAY_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestDate = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST2_HOUR_REQ;
          break;
          //====================================== Test Time 2 Day ======================================//
          //====================================== Test Time 2 Hour ======================================//
        case TEST2_HOUR_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[65][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST2_HOUR_RES;
          break;
          
        case TEST2_HOUR_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[1].cfgBTPlanTestHour = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST3_MONTH_REQ;
          break;
          //====================================== Test Time 2 Hour ======================================//
          //====================================== Test Time 3 Month ======================================//
        case TEST3_MONTH_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[66][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST3_MONTH_RES;
          break;
          
        case TEST3_MONTH_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestMonth = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST3_DAY_REQ;
          break;
          //====================================== Test Time 3 Month ======================================//
          //====================================== Test Time 3 Day ======================================//
        case TEST3_DAY_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[67][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST3_DAY_RES;
          break;
          
        case TEST3_DAY_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestDate = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST3_HOUR_REQ;
          break;
          //====================================== Test Time 3 Day ======================================//
          //====================================== Test Time 3 Hour ======================================//
        case TEST3_HOUR_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[68][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST3_HOUR_RES;
          break;
          
        case TEST3_HOUR_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[2].cfgBTPlanTestHour = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST4_MONTH_REQ;
          break;
          //====================================== Test Time 3 Hour ======================================//
          //====================================== Test Time 4 Month ======================================//
        case TEST4_MONTH_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[69][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST4_MONTH_RES;
          break;
          
        case TEST4_MONTH_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestMonth = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST4_DAY_REQ;
          break;
          //====================================== Test Time 4 Month ======================================//
          //====================================== Test Time 4 Day ======================================//
        case TEST4_DAY_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[70][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST4_DAY_RES;
          break;
          
        case TEST4_DAY_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestDate = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = TEST4_HOUR_REQ;
          break;
          //====================================== Test Time 4 Day ======================================//
          //====================================== Test Time 4 Hour ======================================//
        case TEST4_HOUR_REQ:
          
          getCommand(&g_getInfo[0],&cmdCodeTable[71][0]);
          
          for (i = 0; i < 24; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_getInfo[i],1);
          }
          
          MESGState = TEST4_HOUR_RES;
          break;
          
        case TEST4_HOUR_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[3].cfgBTPlanTestHour = hex2byte(&RecvBuff[0],13);
          }
          
          RecvCntStart = 0;
          MESGState = SYS_TIME_REQ;
          break;
          //====================================== Test Time 4 Hour ======================================//
          //====================================== Sys Time  ======================================//
        case SYS_TIME_REQ:
          for (i = 0; i < GetTimeInfoLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetTimeInfo[i],1);
          }
          
          MESGState = SYS_TIME_RES;
          break;
          
        case SYS_TIME_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            temp[0] = hex2byte(&RecvBuff[0],13);
            temp[1] = hex2byte(&RecvBuff[0],15);
            
            private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUYear = temp[0] * 100 + temp[1];
            private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMonth = hex2byte(&RecvBuff[0],17);
            private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUDate = hex2byte(&RecvBuff[0],19);
            private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUHour = hex2byte(&RecvBuff[0],21);
            private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMinute = hex2byte(&RecvBuff[0],23);
            private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUSecond = hex2byte(&RecvBuff[0],25);
            temp[0] = 0;
            temp[1] = 0;
          }
          
          RecvCntStart = 0;
          MESGState = UPDATE_OK;
          break;
          //====================================== Sys Time ======================================//
          //#if	IPS_VERSION
          //                //====================================== ETHERNET INFO MESSAGE ======================================//
          //                case  ETHSYNC_REQ:
          //
          //                        u322char(&EthInfoSync[13], parameter.ulStaticIP, 8);
          //                        u322char(&EthInfoSync[21], parameter.ulSubnetMask, 8);
          //                        u322char(&EthInfoSync[29], parameter.ulGatewayIP, 8);
          //                        u322char(&EthInfoSync[37], parameter.sPort[1].ulTelnetIPAddr, 8);
          //                        hex2char(&EthInfoSync[45],checksum(&EthInfoSync[0],50),4);
          //
          //                        for (i = 0; i < 50; i++)
          //                        {
          //                                SerialSend(EthInfoSync[i]);
          //                        }
          //                        MESGState = ETHSYNC_RES;
          //                break;
          //                case  ETHSYNC_RES:
          //                        RingBufWrite(&g_sRxBuf, g_UARTRxBuf,RecvCntStart);
          //                        RecvCount = RingBufUsed(&g_sRxBuf);
          //                        RingBufRead(&g_sRxBuf,RecvBuff,RecvCount);
          //
          //                        g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          //                        g_checksum = checksum(&RecvBuff[0],RecvCount);
          //                        if (g_checksum == g_testchecksum)
          //                        {
          //
          //                        }
          //
          //
          //                        RecvCntStart = 0;
          //                        MESGState = UPDATE_OK;
          //                break;
          //                //====================================== ETHERNET INFO MESSAGE ======================================//
          //#endif
          //====================================== SystemWalkInTimeEnable MESSAGE ======================================//
          
        case UPDATE_OK:
          
          private_mib_base_struct.batteryGroup.battVolt = (int32_t) (sDcInfo.sINFODcInfo.fBatt1Volt * 100);//private_mib_base_struct.batteryGroup.battVolt
          private_mib_base_struct.batteryGroup.battCurr = (int32_t) ((sDcInfo.sINFODcInfo.fBatt1Curr + sDcInfo.sINFODcInfo.fBatt2Curr) * 100);//private_mib_base_struct.batteryGroup.battCurr
          private_mib_base_struct.batteryGroup.battCapLeft1 = (uint32_t) (sDcInfo.sINFODcInfo.fBatt1RmnCap * 100);//sBattInfosSNMPBattInfo.u32CapLeft[0]
          private_mib_base_struct.batteryGroup.battCapLeft2 = (uint32_t) (sDcInfo.sINFODcInfo.fBatt2RmnCap * 100);//sBattInfo.sSNMPBattInfo.u32CapLeft[1]
          // ======================== Update rectifier information to SNMP resource ==========================//
          j = sRectInfo.u8Rect_Num;
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
          
          private_mib_base_struct.rectGroup.rectInstalledRect = sRectInfo.u8Rect_Num;
          private_mib_base_struct.rectGroup.rectActiveRect = sRectInfo.u8Rect_Num;
          if (j <= 24)
          {
            while (j > 0)
            {
              //                            private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0;
              //
              //                            private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault << ((j-1)*8);
              //                            private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= sRectInfo.sRAWRectParam[j-1].u8Rect_Fail << ((j-1)*8);
              //                            private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp |= sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp << ((j-1)*8);
              //
              //                            if (sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp == 0)
              //                            {
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectFault = sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault;
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectNoResp = sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp;
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputCurrent = (uint32_t) (sRectInfo.sRAWRectParam[j-1].fRect_DcCurr * 100);
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectTemp = (uint32_t) (sRectInfo.sRAWRectParam[j-1].fRect_Temp * 100);
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputVoltage = (uint32_t) (sRectInfo.sRAWRectParam[j-1].fRect_DcVolt * 100);
              //
              //                                if(sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault==1)
              //                                {
              //                                    private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xA5; // Rect fail
              //                                }
              //                                else
              //                                {
              //                                    private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xff;
              //                                }
              ////                                                usnprintf(&sRectInfo.sREPORTRectParam[j-1].cRect_SN[0],15,"%u",
              ////                                                                sRectInfo.sRAWRectParam[j-1].u32Rect_SN
              ////                                                );
              ////                                sRectInfo.sREPORTRectParam[j-1].cRect_SN[15] = '\0';
              //
              //                            }
              //                            else if (sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp == 0xe2)
              //                            {
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectFault = sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault;
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectNoResp = sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp;
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xaa;
              ////                                                usnprintf(&sRectInfo.sREPORTRectParam[j-1].cRect_SN[0],15,"%u",
              ////                                                                sRectInfo.sRAWRectParam[j-1].u32Rect_SN
              ////                                                );
              ////                                sRectInfo.sREPORTRectParam[j-1].cRect_SN[15] = '\0';
              //
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputCurrent = 0;
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectTemp = 0;
              //                                private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputVoltage = 0;
              //                            }
              
              private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0;
              
              switch(sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp)
              {
              case 0:
                {
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectFault = sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault;
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectNoResp = sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp;
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputCurrent = (uint32_t) (sRectInfo.sRAWRectParam[j-1].fRect_DcCurr * 100);
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectTemp = (uint32_t) (sRectInfo.sRAWRectParam[j-1].fRect_Temp * 100);
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputVoltage = (uint32_t) (sRectInfo.sRAWRectParam[j-1].fRect_DcVolt * 100);
                  
                  if(sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault==1)
                  {
                    private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xA5; // Rect fail
                    private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= (1 << (j-1));
                  }
                  else
                  {
                    private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xff;
                    private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= (0 << (j-1));
                  }
                  if(sRectInfo.sRAWRectParam[j-1].u8Rect_Fail==1)
                  {
                    private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xA5; // Rect fail
                    private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= (1 << (j-1));
                  }
                  else
                  {
                    private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xff;
                    private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= (0 << (j-1));
                  }
                  private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp |= (0 << (j-1));
                }break;
              case 0xe2:
                {
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectFault = sRectInfo.sRAWRectParam[j-1].u8Rect_ACFault;
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectNoResp = sRectInfo.sRAWRectParam[j-1].u8Rect_NoResp;
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectStatus = 0xaa;
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputCurrent = 0;
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectTemp = 0;
                  private_mib_base_struct.rectGroup.rectTable[j-1].rectOutputVoltage = 0;
                  private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp |= (1 << (j-1));
                  private_mib_base_struct.rectGroup.rectActiveRect--;
                  if (private_mib_base_struct.rectGroup.rectActiveRect < 0)
                  {
                    private_mib_base_struct.rectGroup.rectActiveRect = 0;
                  }
                }break;
              };
              j--;
            }
          }
          if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
          if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
          private_mib_base_struct.rectGroup.rectTotalCurrent = 0;
          
          for (i = 0; i < sRectInfo.u8Rect_Num; i++)
          {
            private_mib_base_struct.rectGroup.rectTotalCurrent += (uint32_t) (private_mib_base_struct.rectGroup.rectTable[i].rectOutputCurrent);
          }
          
          private_mib_base_struct.loadGroup.loadCurrent = (uint32_t) (sDcInfo.sINFODcInfo.fCurrent * 100);
          //							private_mib_base_struct.loadGroup.loadCurrent = private_mib_base_struct.rectGroup.rectTotalCurrent - private_mib_base_struct.batteryGroup.battCurr;
          //                        if (private_mib_base_struct.loadGroup.loadCurrent >= 0) private_mib_base_struct.loadGroup.loadCurrent = private_mib_base_struct.loadGroup.loadCurrent;
          //                        else if (private_mib_base_struct.loadGroup.loadCurrent < 0) private_mib_base_struct.loadGroup.loadCurrent = -private_mib_base_struct.loadGroup.loadCurrent;
          
          // ======================== Update rectifier information to SNMP resource ==========================//
          // ======================== Update ac information to SNMP resource ==========================//
          private_mib_base_struct.acPhaseGroup.acPhaseTable[0].acPhaseVolt = (int32_t) (sAcInfo.facVolt[0] * 100);
          private_mib_base_struct.acPhaseGroup.acPhaseTable[1].acPhaseVolt = (int32_t) (sAcInfo.facVolt[1] * 100);
          private_mib_base_struct.acPhaseGroup.acPhaseTable[2].acPhaseVolt = (int32_t) (sAcInfo.facVolt[2] * 100);
          // ======================== Update ac information to SNMP resource ==========================//
          // ======================== Update alarm information to SNMP resource ==========================//
          if(g_AlrmMan == 0xe1)private_mib_base_struct.main_alarm_group_struct.alarm_manual_mode =1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_manual_mode =0;
          //			private_mib_base_struct.main_alarm_group_struct.alarm_manual_mode = g_AlrmMan;
          if(sAcInfo.u8MainFail !=0)private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains =1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains =0;
          //			private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains = sAcInfo.u8MainFail;
          private_mib_base_struct.main_alarm_group_struct.alarm_llvd =0;
          if(sDcInfo.sALARMDcInfo.u8LLVD == 0xE3)private_mib_base_struct.main_alarm_group_struct.alarm_llvd =1;
          //			private_mib_base_struct.main_alarm_group_struct.alarm_llvd = sDcInfo.sALARMDcInfo.u8LLVD;
          private_mib_base_struct.main_alarm_group_struct.alarm_blvd = 0;
          if(sDcInfo.sALARMDcInfo.u8BLVD == 0xE4)private_mib_base_struct.main_alarm_group_struct.alarm_blvd =1;
          //			private_mib_base_struct.main_alarm_group_struct.alarm_blvd = sDcInfo.sALARMDcInfo.u8BLVD;
          //			sAlarmInfo.u32DCopen = sDcInfo.sALARMDcInfo.u8DC;
          //                    if(sDcInfo.sINFODcInfo.fBatt1Volt<sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg)
          //                     sAlarmInfo.u32DCopen = 0x01;
          //                    else sAlarmInfo.u32DCopen = 0x00;
          if(sDcInfo.sALARMDcInfo.u8DC == 1) private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 0;
          
          if (sAcInfo.u8Thres[0] || sAcInfo.u8Thres[1] || sAcInfo.u8Thres[2])
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_low = 1;
          }
          else
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_low = 0;
          }
          
          private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open = 0;
          for (i = 0; i < 10; i++)
          {
            if(sDcInfo.sALARMDcInfo.u8LoadFuse[i] == 0x03)
            {
              fuse_flag = 1;
              private_mib_base_struct.loadGroup.loadStatus[i] = 1;
            }
            else
            {
              fuse_flag = 0;
              private_mib_base_struct.loadGroup.loadStatus[i] = 0;
            }
            private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open |= (fuse_flag << i);
          }
          if (private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open != 0)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open = 1;
          }
          else
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open = 0;
          }
          
          private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 0;
          for (i = 0; i < 4; i++)
          {
            if(sDcInfo.sALARMDcInfo.u8BattFuse[i] == 0x03)
            {
              fuse_flag = 1;
              private_mib_base_struct.batteryGroup.battBrkStatus[i] = 1;
            }
            else
            {
              fuse_flag = 0;
              private_mib_base_struct.batteryGroup.battBrkStatus[i] = 0;
            }
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open |= (fuse_flag << i);
          }
          if (private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open != 0)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 1;
          }
          else
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 0;
          }
          
          for (i = 0; i < 8; i++)
          {
            if(sDcInfo.sALARMDcInfo.u8DIFuse[i] == 0xEA)
            {
              fuse_flag = 1;
              private_mib_base_struct.diAlarmGroup.alarmDigitalInput[i] = 1;
            }
            else
            {
              fuse_flag = 0;
              private_mib_base_struct.diAlarmGroup.alarmDigitalInput[i] = 0;
            }
          }
          
          switch(sDcInfo.sALARMDcInfo.u8Sen1_BattTemp)
          {
          case 0x20:
            if ((sDcInfo.sALARMDcInfo.u8Sen2_BattTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen2_BattTemp == 0x05))
            {
              private_mib_base_struct.batteryGroup.battTemp = 0;
              if (sDcInfo.sALARMDcInfo.u8Sen2_BattTemp == 0x20)
              {
                private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x20;
              }
              else if (sDcInfo.sALARMDcInfo.u8Sen2_BattTemp == 0x05)
              {
                private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x05;
              }
            }
            break;
          case 0x00:
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen1BattTemp * 10);
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x00;
            break;
          case 0x05:
            if ((sDcInfo.sALARMDcInfo.u8Sen2_BattTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen2_BattTemp == 0x05))
            {
              private_mib_base_struct.batteryGroup.battTemp = 0;
              private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x05;
            }
            break;
          case 0x02:
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen1BattTemp * 10);
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x02;
            break;
          case 0x01:
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen1BattTemp * 10);
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x01;
            break;
          }
          
          switch(sDcInfo.sALARMDcInfo.u8Sen2_BattTemp)
          {
          case 0x20:
            if ((sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x05))
            {
              private_mib_base_struct.batteryGroup.battTemp = 0;
              if (sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x20)
              {
                private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x20;
              }
              else if (sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x05)
              {
                private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x05;
              }
            }
            break;
          case 0x00:
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen2BattTemp * 10);
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x00;
            break;
          case 0x05:
            if ((sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x05))
            {
              private_mib_base_struct.batteryGroup.battTemp = 0;
              private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x05;
            }
            break;
          case 0x02:
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen2BattTemp * 10);
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x02;
            break;
          case 0x01:
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen2BattTemp * 10);
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x01;
            break;
          }
          
          switch(sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp)
          {
          case 0x20:
            if ((sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp == 0x05))
            {
              private_mib_base_struct.siteGroup.siteAmbientTemp = 0;
            }
            break;
          case 0x00:
            private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen1AmbTemp * 10);
            break;
          case 0x05:
            if ((sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp == 0x05))
            {
              private_mib_base_struct.siteGroup.siteAmbientTemp = 0;
            }
            break;
          case 0x02:
            private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen1AmbTemp * 10);
            break;
          case 0x01:
            private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen1AmbTemp * 10);
            break;
          }
          
          switch(sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp)
          {
          case 0x20:
            if ((sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp == 0x05))
            {
              private_mib_base_struct.siteGroup.siteAmbientTemp = 0;
            }
            break;
          case 0x00:
            private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen2AmbTemp * 10);
            break;
          case 0x05:
            if ((sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp == 0x20 ) || (sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp == 0x05))
            {
              private_mib_base_struct.siteGroup.siteAmbientTemp = 0;
            }
            break;
          case 0x02:
            private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen2AmbTemp * 10);
            break;
          case 0x01:
            private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t)(sDcInfo.sINFODcInfo.fSen2AmbTemp * 10);
            break;
          }
          // ======================== Update alarm information to SNMP resource ==========================//
          // ======================== Update Config information to SNMP resource ==========================//
          private_mib_base_struct.configGroup.cfgFloatVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fFltVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgBoostVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fBotVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgTempCompValue = (uint32_t) (sBattInfo.sRAWBattInfo.fTempCompVal);
          private_mib_base_struct.configGroup.cfgLLVDVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgBLVDVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLVDDV * 100);
          private_mib_base_struct.configGroup.cfgDCLowVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgDCUnderVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fDCUnderCfg * 100);
          private_mib_base_struct.configGroup.cfgDCOverVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fDCOverCfg * 100);                
          private_mib_base_struct.configGroup.cfgLLVDEn = sBattInfo.sRAWBattInfo.u8LLVDE;
          private_mib_base_struct.configGroup.cfgBLVDEn = sBattInfo.sRAWBattInfo.u8BLVDE;
          
          
          private_mib_base_struct.batteryGroup.battBanksNumofBanks = sBattInfo.sRAWBattInfo.u8BankNo;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal;
          private_mib_base_struct.configGroup.cfgCurrentLimit = (uint32_t) (sBattInfo.sRAWBattInfo.fCCLVal * 1000);
          
          private_mib_base_struct.configGroup.cfgOverMajorTempLevel = (uint32_t)(sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel * 100);
          private_mib_base_struct.configGroup.cfgHighMajorTempLevel = (uint32_t) (sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel *100);
          
          private_mib_base_struct.configGroup.cfgWalkInTimeDuration = sRectInfo.u8WITI;
          private_mib_base_struct.configGroup.cfgWalkInTimeEn = sRectInfo.u8WITE;
          private_mib_base_struct.configGroup.cfgAcLowLevel = (uint32_t) sAcInfo.fAcLowThres;
          private_mib_base_struct.configGroup.cfgAcUnderLevel = (uint32_t) sAcInfo.fAcUnderThres;
          private_mib_base_struct.configGroup.cfgAcHighLevel = (uint32_t) sAcInfo.fAcHighThres;
          
          // ======================== Update Config information to SNMP resource ==========================//
          //======== Update Planned Test ============//
          private_mib_base_struct.cfgBTGroup.cfgBTPlanTestEn = sBattTestInfo.u8TimeTestEn;
          private_mib_base_struct.cfgBTGroup.cfgBTEndCap = (uint32_t) (sBattTestInfo.fTestEndCap * 1000);
          private_mib_base_struct.cfgBTGroup.cfgBTEndTime = (uint32_t) sBattTestInfo.fBattTestDur;
          private_mib_base_struct.cfgBTGroup.cfgBTEndVolt = (uint32_t) (sBattTestInfo.fBattTestVolt * 10);
          for (i = 0; i < 4; i++)
          {
            snprintf(private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[i].cfgBTPlanTestString,14,"%02d/%02d - %02d:00 ",
                     private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[i].cfgBTPlanTestMonth,
                     private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[i].cfgBTPlanTestDate,
                     private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[i].cfgBTPlanTestHour
                       );
            private_mib_base_struct.cfgBTGroup.cfgBTPlanTestTable[i].cfgBTPlanTestString[14] = '\0';
          }
          
          snprintf(private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUString,20,"%04d-%02d-%02d@%02d:%02d:%02d",
                   private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUYear,
                   private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMonth,
                   private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUDate,
                   private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUHour,
                   private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUMinute,
                   private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUSecond
                     );
          private_mib_base_struct.cfgBTGroup.cfgBTSCUTable[0].cfgBTSCUString[20] = '\0';
          
          
          //======== Update Planned Test ============//
          
          MESGState = SYSINFO_REQ;
          break;
        case  TEST_MODE_REQ:
          
          for (i = 0; i < GetMACAddrLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetMACAddr[i],1);
          }
          
          MESGState = TEST_MODE_RES;
          break;
          
        case  TEST_MODE_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
          g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          if (g_checksum == g_testchecksum)
          {
            ui8User[0] = hex2byte(&RecvBuff[0],13);
            ui8User[1] = hex2byte(&RecvBuff[0],15);
            ui8User[2] = hex2byte(&RecvBuff[0],17);
            ui8User[3] = hex2byte(&RecvBuff[0],19);
            ui8User[4] = hex2byte(&RecvBuff[0],21);
            ui8User[5] = hex2byte(&RecvBuff[0],23);
            
            //                                eeprom_write_byte(MAC_ALREADY_WRITTEN,0x53);
            //
            //                                eeprom_write_byte(EEPROM_MACADDR1,ui8User[0]);
            //                                eeprom_write_byte(EEPROM_MACADDR2,ui8User[1]);
            //                                eeprom_write_byte(EEPROM_MACADDR3,ui8User[2]);
            //                                eeprom_write_byte(EEPROM_MACADDR4,ui8User[3]);
            //                                eeprom_write_byte(EEPROM_MACADDR5,ui8User[4]);
            //                                eeprom_write_byte(EEPROM_MACADDR6,ui8User[5]);
            
            sSiteInfo.u32RemoteReboot = 0x53;
          }
          else
          {
            MESGState = TEST_MODE_REQ;
            //					test_mode = 1;
          }
          
          RecvCntStart = 0;
          
          break;
        case  CALIB_MESG_1:    
          {
            u82char(&calibMesg_1[3], 0x01, 2);
            u322char(&calibMesg_1[13], parameter.ip_parameter.static_ip, 8);
            u322char(&calibMesg_1[21], parameter.ip_parameter.subnet_mask, 8);
            u322char(&calibMesg_1[29], parameter.ip_parameter.gateway_ip, 8);
            u322char(&calibMesg_1[37], parameter.ip_parameter.snmp_ip, 8);
            
            u82char(&calibMesg_1[45], working_default_parameter_2.u8IsWritten, 2); 
            u82char(&calibMesg_1[47], working_default_parameter_2.u8UserMAC[0], 2);
            u82char(&calibMesg_1[49], working_default_parameter_2.u8UserMAC[1], 2);
            u82char(&calibMesg_1[51], working_default_parameter_2.u8UserMAC[2], 2);
            u82char(&calibMesg_1[53], working_default_parameter_2.u8UserMAC[3], 2);
            u82char(&calibMesg_1[55], working_default_parameter_2.u8UserMAC[4], 2);
            u82char(&calibMesg_1[57], working_default_parameter_2.u8UserMAC[5], 2);
            u322char(&calibMesg_1[59], parameter2.u32FirmwareCRC, 8);
            
            u82char(&calibMesg_1[67], sTestRS485.Test485LoopOk, 2);
            
            
            //                        u82char(&calibMesg_1[45], eeprom_read_byte(MAC_ALREADY_WRITTEN), 2);
            //                        u82char(&calibMesg_1[47], eeprom_read_byte(EEPROM_MACADDR1), 2);
            //                        u82char(&calibMesg_1[49], eeprom_read_byte(EEPROM_MACADDR2), 2);
            //                        u82char(&calibMesg_1[51], eeprom_read_byte(EEPROM_MACADDR3), 2);
            //                        u82char(&calibMesg_1[53], eeprom_read_byte(EEPROM_MACADDR4), 2);
            //                        u82char(&calibMesg_1[55], eeprom_read_byte(EEPROM_MACADDR5), 2);
            //                        u82char(&calibMesg_1[57], eeprom_read_byte(EEPROM_MACADDR6), 2);
            u162char(&calibMesg_1[69],Checksum16(&calibMesg_1[0],74),4);
            
            for (i = 0; i < 74; i++)
            {
              
              LPUART_WriteBlocking(LPUART5,&calibMesg_1[i],1);
              
            }
            //                        u8IsRebootFlag = 1;
            MESGState = SYSINFO_REQ;
          }
          
          break;
        case  CALIB_MESG_2:
          {
            u82char(&calibMesg_1[3], 0x02, 2);
            u322char(&calibMesg_1[13], parameter.ip_parameter.static_ip, 8);
            u322char(&calibMesg_1[21], parameter.ip_parameter.subnet_mask, 8);
            u322char(&calibMesg_1[29], parameter.ip_parameter.gateway_ip, 8);
            u322char(&calibMesg_1[37], parameter.ip_parameter.snmp_ip, 8);
            
            u82char(&calibMesg_1[45], working_default_parameter_2.u8IsWritten, 2);
            u82char(&calibMesg_1[47], working_default_parameter_2.u8UserMAC[0], 2);
            u82char(&calibMesg_1[49], working_default_parameter_2.u8UserMAC[1], 2);
            u82char(&calibMesg_1[51], working_default_parameter_2.u8UserMAC[2], 2);
            u82char(&calibMesg_1[53], working_default_parameter_2.u8UserMAC[3], 2);
            u82char(&calibMesg_1[55], working_default_parameter_2.u8UserMAC[4], 2);
            u82char(&calibMesg_1[57], working_default_parameter_2.u8UserMAC[5], 2);
            u322char(&calibMesg_1[59], parameter2.u32FirmwareCRC, 8);
            
            u82char(&calibMesg_1[67], sTestRS485.Test485LoopOk, 2);
            
            
            //                        u82char(&calibMesg_1[45], eeprom_read_byte(MAC_ALREADY_WRITTEN), 2);
            //                        u82char(&calibMesg_1[47], eeprom_read_byte(EEPROM_MACADDR1), 2);
            //                        u82char(&calibMesg_1[49], eeprom_read_byte(EEPROM_MACADDR2), 2);
            //                        u82char(&calibMesg_1[51], eeprom_read_byte(EEPROM_MACADDR3), 2);
            //                        u82char(&calibMesg_1[53], eeprom_read_byte(EEPROM_MACADDR4), 2);
            //                        u82char(&calibMesg_1[55], eeprom_read_byte(EEPROM_MACADDR5), 2);
            //                        u82char(&calibMesg_1[57], eeprom_read_byte(EEPROM_MACADDR6), 2);
            u162char(&calibMesg_1[69],Checksum16(&calibMesg_1[0],74),4);
            
            for (i = 0; i < 74; i++)
            {
              
              LPUART_WriteBlocking(LPUART5,&calibMesg_1[i],1);
              
            }
            system_flag_struct.u8IsRebootFlag = 1;
            MESGState = SYSINFO_REQ;
          }
          
          break;
        case  CALIB_MESG_3:
          {
            u82char(&calibMesg_1[3], 0x03, 2);
            u322char(&calibMesg_1[13], parameter.ip_parameter.static_ip, 8);
            u322char(&calibMesg_1[21], parameter.ip_parameter.subnet_mask, 8);
            u322char(&calibMesg_1[29], parameter.ip_parameter.gateway_ip, 8);
            u322char(&calibMesg_1[37], parameter.ip_parameter.snmp_ip, 8);
            
            u82char(&calibMesg_1[45], working_default_parameter_2.u8IsWritten, 2);
            u82char(&calibMesg_1[47], working_default_parameter_2.u8UserMAC[0], 2);
            u82char(&calibMesg_1[49], working_default_parameter_2.u8UserMAC[1], 2);
            u82char(&calibMesg_1[51], working_default_parameter_2.u8UserMAC[2], 2);
            u82char(&calibMesg_1[53], working_default_parameter_2.u8UserMAC[3], 2);
            u82char(&calibMesg_1[55], working_default_parameter_2.u8UserMAC[4], 2);
            u82char(&calibMesg_1[57], working_default_parameter_2.u8UserMAC[5], 2);
            u322char(&calibMesg_1[59], parameter2.u32FirmwareCRC, 8);
            
            u82char(&calibMesg_1[67], sTestRS485.Test485LoopOk, 2);
            
            
            //                        u82char(&calibMesg_1[45], eeprom_read_byte(MAC_ALREADY_WRITTEN), 2);
            //                        u82char(&calibMesg_1[47], eeprom_read_byte(EEPROM_MACADDR1), 2);
            //                        u82char(&calibMesg_1[49], eeprom_read_byte(EEPROM_MACADDR2), 2);
            //                        u82char(&calibMesg_1[51], eeprom_read_byte(EEPROM_MACADDR3), 2);
            //                        u82char(&calibMesg_1[53], eeprom_read_byte(EEPROM_MACADDR4), 2);
            //                        u82char(&calibMesg_1[55], eeprom_read_byte(EEPROM_MACADDR5), 2);
            //                        u82char(&calibMesg_1[57], eeprom_read_byte(EEPROM_MACADDR6), 2);
            u162char(&calibMesg_1[69],Checksum16(&calibMesg_1[0],74),4);
            
            for (i = 0; i < 74; i++)
            {
              
              LPUART_WriteBlocking(LPUART5,&calibMesg_1[i],1);
              
            }
            system_flag_struct.u8IsRebootFlag = 1;
            MESGState = SYSINFO_REQ;
          }
          
          break;
        case  CALIB_MESG_4:
          {
            if(sTestRS485.requesttestRS485 == 0)
            {
              u82char(&calibMesg_1[3], 0x04, 2);
              u322char(&calibMesg_1[13], parameter.ip_parameter.static_ip, 8);
              u322char(&calibMesg_1[21], parameter.ip_parameter.subnet_mask, 8);
              u322char(&calibMesg_1[29], parameter.ip_parameter.gateway_ip, 8);
              u322char(&calibMesg_1[37], parameter.ip_parameter.snmp_ip, 8);
              
              u82char(&calibMesg_1[45], working_default_parameter_2.u8IsWritten, 2);
              u82char(&calibMesg_1[47], working_default_parameter_2.u8UserMAC[0], 2);
              u82char(&calibMesg_1[49], working_default_parameter_2.u8UserMAC[1], 2);
              u82char(&calibMesg_1[51], working_default_parameter_2.u8UserMAC[2], 2);
              u82char(&calibMesg_1[53], working_default_parameter_2.u8UserMAC[3], 2);
              u82char(&calibMesg_1[55], working_default_parameter_2.u8UserMAC[4], 2);
              u82char(&calibMesg_1[57], working_default_parameter_2.u8UserMAC[5], 2);
              u322char(&calibMesg_1[59], parameter2.u32FirmwareCRC, 8);
              
              u82char(&calibMesg_1[67], sTestRS485.Test485LoopOk, 2);
              
              
              //                        u82char(&calibMesg_1[45], eeprom_read_byte(MAC_ALREADY_WRITTEN), 2);
              //                        u82char(&calibMesg_1[47], eeprom_read_byte(EEPROM_MACADDR1), 2);
              //                        u82char(&calibMesg_1[49], eeprom_read_byte(EEPROM_MACADDR2), 2);
              //                        u82char(&calibMesg_1[51], eeprom_read_byte(EEPROM_MACADDR3), 2);
              //                        u82char(&calibMesg_1[53], eeprom_read_byte(EEPROM_MACADDR4), 2);
              //                        u82char(&calibMesg_1[55], eeprom_read_byte(EEPROM_MACADDR5), 2);
              //                        u82char(&calibMesg_1[57], eeprom_read_byte(EEPROM_MACADDR6), 2);
              u162char(&calibMesg_1[69],Checksum16(&calibMesg_1[0],74),4);
              
              for (i = 0; i < 74; i++)
              {
                
                LPUART_WriteBlocking(LPUART5,&calibMesg_1[i],1);
                
              }
              //                        u8IsRebootFlag = 1;
              MESGState = SYSINFO_REQ;
            }
          }
          break;
        default:
          break;
        }
        
      }
    }
    
    
    vTaskDelay(200);
  }
  while (1);
  
  

}