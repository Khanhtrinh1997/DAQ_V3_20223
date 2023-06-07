//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
void zte_task(void* pv_parameters){
  private_mib_base_struct.cntGroup.task.zte =0;
  
  uint32_t i;
  uint32_t fuse_flag;
  MESGState = ACINPUTINFO_REQ;
  
  do
  {
    private_mib_base_struct.cntGroup.task.zte++;

    if (sInterruptMesg.u8Checkbit == 1)
    {
      //
      //        g_testchecksum = hex2byte2(&g_UARTRxBuf2[0],RecvCntStart2-5);
      //        g_checksum = Checksum16(&g_UARTRxBuf2[0],RecvCntStart2);
      //        if (g_checksum == g_testchecksum)
      //        {
      //          u8InterruptCode = hex2byte(&g_UARTRxBuf2[0],3);
      //          switch(u8InterruptCode)
      //          {
      //            case 0x01:
      //            {
      //              MESGState = CALIB_MESG_1;
      //
      //            } break;
      //            case 0x02:
      //            {
      //              u32IPAddr = hex2byte4(&g_UARTRxBuf2[0],13);
      //              u32SNAddr = hex2byte4(&g_UARTRxBuf2[0],21);
      //              u32GWAddr = hex2byte4(&g_UARTRxBuf2[0],29);
      //              u32SIPAddr = hex2byte4(&g_UARTRxBuf2[0],37);
      //
      //              //
      //              // If we are now using static IP, check for modifications to the IP
      //              // addresses and mask.
      //              //
      //
      //              if((g_sParameters.ulStaticIP != u32IPAddr) ||
      //                 (g_sParameters.ulGatewayIP != u32GWAddr) ||
      //                 (g_sParameters.ulSubnetMask != u32SNAddr) ||
      //                 (g_sParameters.sPort[1].ulTelnetIPAddr != u32SIPAddr))
      //              {
      //                      //
      //                      // Something changed so update the parameter block.
      //                      //
      //                      g_sParameters.ulStaticIP = u32IPAddr;
      //                      g_sParameters.ulGatewayIP = u32GWAddr;
      //                      g_sParameters.ulSubnetMask = u32SNAddr;
      //                      g_sParameters.sPort[1].ulTelnetIPAddr = u32SIPAddr;
      //
      //                      //
      //                      // Apply all the changes to the working parameter set.
      //                      //
      //                      ConfigUpdatePortParameters(1, 0, 1);
      //
      //                      //
      //                      // Update the working default set and save the parameter block.
      //                      //
      //                      g_sWorkingDefaultParameters = g_sParameters;
      //                      u8SaveConfigFlag |= 1;
      //
      //                      //
      //                      // Tell the main loop that a IP address update has been requested.
      //                      //
      //                      g_cUpdateRequired |= UPDATE_IP_ADDR;
      //              }
      //              MESGState = CALIB_MESG_2;
      //            } break;
      //            case 0x03:
      //            {
      //              ui8User[0] = hex2byte(&g_UARTRxBuf2[0],13);
      //              ui8User[1] = hex2byte(&g_UARTRxBuf2[0],15);
      //              ui8User[2] = hex2byte(&g_UARTRxBuf2[0],17);
      //              ui8User[3] = hex2byte(&g_UARTRxBuf2[0],19);
      //              ui8User[4] = hex2byte(&g_UARTRxBuf2[0],21);
      //              ui8User[5] = hex2byte(&g_UARTRxBuf2[0],23);
      //
      //              eeprom_write_byte(MAC_ALREADY_WRITTEN,0x53);
      //
      //              eeprom_write_byte(EEPROM_MACADDR1,ui8User[0]);
      //              eeprom_write_byte(EEPROM_MACADDR2,ui8User[1]);
      //              eeprom_write_byte(EEPROM_MACADDR3,ui8User[2]);
      //              eeprom_write_byte(EEPROM_MACADDR4,ui8User[3]);
      //              eeprom_write_byte(EEPROM_MACADDR5,ui8User[4]);
      //              eeprom_write_byte(EEPROM_MACADDR6,ui8User[5]);
      //
      //              if (eeprom_read_byte(MAC_ALREADY_WRITTEN) == 0x53)
      //              {
      //                      MESGState = CALIB_MESG_3;
      //              }
      //
      //
      //
      //            } break;
      //            default:
      //             break;
      //          }
      //
      //        }
      sInterruptMesg.u8Checkbit = 0;
      
    }
    else if (sInterruptMesg.u8Checkbit == 0)
    {
      if((private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 ==0)&& (private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect !=0))
      {
        if(ZTETime.year!=0) 
        {
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =29;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Year  =   ZTETime.year-2000;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Month =   ZTETime.month;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Day   =   ZTETime.date;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hour  =   ZTETime.hour;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Minute=   ZTETime.min;
          NextHis();
          private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 = private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect;
          //                HisCount_old[ih] = sHisFlashLog[ih].Count;
        }
        
      }
      if((private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 !=0)&& (private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect ==0))
      {
        if(ZTETime.year!=0)
        {
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =30;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Year  =   ZTETime.year-2000;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Month =   ZTETime.month;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Day   =   ZTETime.date;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hour  =   ZTETime.hour;
          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Minute=   ZTETime.min;
          NextHis();
          private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 = private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect;
          
        }
      }
      if (setCmd_flag == 1)
      {
        settingCommand |= setCmd_mask;
        
        switch(settingCommand)
        {
        case SET_FLTVOL:
          sBattInfo.sRAWBattInfo.fFltVoltCfg = (float)private_mib_base_struct.configGroup.cfgFloatVolt/100;
          //                    sBattInfo.sRAWBattInfo.fFltVoltCfg = a;
          ZTE_SetCommand(&g_setInfo[0],FLOATVOLT,sBattInfo.sRAWBattInfo.fFltVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BSTVOL:
          sBattInfo.sRAWBattInfo.fBotVoltCfg = (float)private_mib_base_struct.configGroup.cfgBoostVolt/100;
          ZTE_SetCommand(&g_setInfo[0],BOOSTVOLT,sBattInfo.sRAWBattInfo.fBotVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_TEMPCOMP_VAL:
          sBattInfo.sRAWBattInfo.fTempCompVal = (float)private_mib_base_struct.configGroup.cfgTempCompValue;
          ZTE_SetCommand(&g_setInfo[0],TEMP_COMPENSATION,sBattInfo.sRAWBattInfo.fTempCompVal,DC);
          for (i = 0; i < 28; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_LLVD:
          sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = (float)private_mib_base_struct.configGroup.cfgLLVDVolt/100;
          ZTE_SetCommand(&g_setInfo[0],LLVD,sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BLVD:
          sBattInfo.sRAWBattInfo.fLVDDV = (float)private_mib_base_struct.configGroup.cfgBLVDVolt/100;
          ZTE_SetCommand(&g_setInfo[0],BLVD,sBattInfo.sRAWBattInfo.fLVDDV,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_DCLOW:
          sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = (float)private_mib_base_struct.configGroup.cfgDCLowVolt/100;
          ZTE_SetCommand(&g_setInfo[0],DCLOW,sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_BATTSTDCAP:
          sBattInfo.sRAWBattInfo.fCapTotal = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal;
          ZTE_SetCommand(&g_setInfo[0],BATT_CAP1,sBattInfo.sRAWBattInfo.fCapTotal,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BATTSTDCAP2:
          sBattInfo.sRAWBattInfo.fCapTotal2 = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal2;
          ZTE_SetCommand(&g_setInfo[0],BATT_CAP2,sBattInfo.sRAWBattInfo.fCapTotal2,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_CCL:
          sBattInfo.sRAWBattInfo.fCCLVal = (float)private_mib_base_struct.configGroup.cfgCurrentLimit/1000;
          ZTE_SetCommand(&g_setInfo[0],CHARGINGFACTOR,sBattInfo.sRAWBattInfo.fCCLVal,DC);
          
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_OVERTEMP:
          sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = (float)private_mib_base_struct.configGroup.cfgHighMajorTempLevel/100;
          ZTE_SetCommand(&g_setInfo[0],BATT_TEMP_OVER,sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
            
          }
          break;
          
          //				case SET_WIT_VAL:
          ////					g_sParameters.ucReserved2[11] = private_mib_base_struct.configGroup.cfgWalkInTimeDuration;
          //					sRectInfo.u8WITI = (float)private_mib_base_struct.configGroup.cfgWalkInTimeDuration;
          //					setCommand(&g_setInfo[0],&cmdCodeTable[36][0], sRectInfo.u8WITI,0);
          //					for (i = 0; i < 28; i++)
          //					{
          //
          //                                                LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          //					}
          //				break;
          
          //				case SET_WIT_EN:
          //					g_sParameters.ucReserved2[12] = private_mib_base_struct.configGroup.cfgWalkInTimeEn;
          //					sRectInfo.u8WITE = (float)private_mib_base_struct.configGroup.cfgWalkInTimeEn;
          //					setCommand(&g_setInfo[0],&cmdCodeTable[37][0], sRectInfo.u8WITE,1);
          //					for (i = 0; i < 28; i++)
          //					{
          //
          //                                                 LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          //					}
          //				break;
          
        case SET_AC_LOW_THRES:
          sAcInfo.fAcLowThres = (float)private_mib_base_struct.configGroup.cfgAcLowLevel;
          ZTE_SetCommand(&g_setInfo[0],ACVOLT_LOWERLIMIT,sAcInfo.fAcLowThres,AC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_AC_HIGH_THRES:
          sAcInfo.fAcHighThres = (float)private_mib_base_struct.configGroup.cfgAcHighLevel;
          ZTE_SetCommand(&g_setInfo[0],ACVOLT_UPPERLIMIT,sAcInfo.fAcHighThres,AC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_AC_INPUT_CURR_LIMIT:
          //					sAcInfo.fAcHighThres = (float)private_mib_base_struct.configGroup.cfgAcLowLevel;
          ZTE_SetCommand(&g_setInfo[0],ACINPUTCUR_UPPERLIMIT,sAcInfo.fAcInputCurrLimit,AC);
          for (i = 0; i < 28; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_TEST_VOLT:
          sBattInfo.sRAWBattInfo.fTestVoltCfg = (float)private_mib_base_struct.cfgBTGroup.cfgTestVoltCfg/10;
          ZTE_SetCommand(&g_setInfo[0],TESTVOLT,sBattInfo.sRAWBattInfo.fTestVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
            
          }
          break;
        case SET_AUTO_TEST_DAY:
          sBattInfo.sRAWBattInfo.fAutoTestDay = (float)private_mib_base_struct.cfgBTGroup.cfgAutoTestDay;
          ZTE_SetCommand(&g_setInfo[0],AUTOTESTDAY,sBattInfo.sRAWBattInfo.fAutoTestDay,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
            
          }
          break;
        case SET_TEST_START_TIME:
          sBattInfo.sRAWBattInfo.fTestStartTime = (float)private_mib_base_struct.cfgBTGroup.cfgTestStartTime;
          ZTE_SetCommand(&g_setInfo[0],TESTSTARTTIME,sBattInfo.sRAWBattInfo.fTestStartTime,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
            
          }
          break;
          
        default:
          break;
        }
        setCmd_mask = 0;
        setCmd_flag = 0;
        settingCommand = 0;
        
      }
      else if (setCmd_flag == 0)
      {
        if(PMUConnectCount++>30)
        {
          PMUConnectCount =50;
          private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 1;
          g_testchecksum=0xFFFF;g_checksum=0;
        }
        switch(MESGState)
        {
        case HISTORYMESG_REQ:
          //                  thao++;
          switch(ModuleHis)
          {
          case ACHis:
            if(ZTEHIS.ZTEHisCommand==3)
            {
              for (i = 0; i < 22; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQachistory03[i],1);
              }
              //                          thao =1;
            }
            if(ZTEHIS.ZTEHisCommand==1)
            {
              for (i = 0; i < 22; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQachistory01[i],1);
              }
              //                           thao =2;
            }
            if(ZTEHIS.ZTEHisCommand==0)
            {
              for (i = 0; i < 22; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQachistory00[i],1);
                
              }
              //                               thao++;
              //                             thao =3;
            }
            break;
          case RectHis:
            if(ZTEHIS.ZTEHisCommand==3)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQrecthistory03[i],1);
              }
              //                           thao =4;
            }
            if(ZTEHIS.ZTEHisCommand==1)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQrecthistory01[i],1);
              }
              //                           thao =5;
            }
            if(ZTEHIS.ZTEHisCommand==0)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQrecthistory00[i],1);
                
              }
              //                             thao++;
              //                            ZTEHIS.ZTEHisCommand =1;
              //                             thao =6;
            }
            
            break;
          case DCHis:
            if(ZTEHIS.ZTEHisCommand==3)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQdchistory03[i],1);
              }
              //                           thao =7;
            }
            if(ZTEHIS.ZTEHisCommand==1)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQdchistory01[i],1);
              }
              //                           thao =8;
            }
            if(ZTEHIS.ZTEHisCommand==0)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQdchistory00[i],1);
                
              }
              //                            ZTEHIS.ZTEHisCommand =1;
              //                             thao =9;
            }
            
            break;
          case EnvHis:
            if(ZTEHIS.ZTEHisCommand==3)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQenvhistory03[i],1);
              }
              //                           thao =10;
            }
            if(ZTEHIS.ZTEHisCommand==1)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQenvhistory01[i],1);
              }
              //                           thao =11;
            }
            if(ZTEHIS.ZTEHisCommand==0)
            {
              for (i = 0; i < 20; i++)
              {
                LPUART_WriteBlocking(LPUART5,&REQenvhistory00[i],1);
                
              }
              //                             thao =12;
              //                            ZTEHIS.ZTEHisCommand =1;
            }
            break;
          default:
            break;
          }
          
          MESGState = HISTORYMESG_RES;
          break;
          
        case HISTORYMESG_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            //                        athao[i] =   g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            ZTEHIS.ZTEHisStatus = hex2byte(&RecvBuff[0],13);
            if(ZTEHIS.ZTEHisStatus<2)
            {
              switch(ModuleHis)
              {
              case ACHis:
                GetACDataLog();
                break;
              case RectHis:
                GetRectDataLog();
                break;
              case DCHis:
                GetDCDataLog();
                break;
              case EnvHis:
                GetEnvDataLog();
                break;
              default:
                break;
              } 
              if(ZTEHIS.ZTEHisStatus==1)
              {  
                //                        MESGState = HISTORYMESG_REQ;
                ZTEHIS.ZTEHisCommand =3;
              }
              if((ZTEHIS.ZTEHisCommand ==0)&& (ZTEHIS.ZTEHisStatus==0) )
              {  
                ZTEHIS.ZTEHisCommand =1;
                //                        MESGState = HISTORYMESG_REQ;
                //                         ZTEHIS.ZTEHisCommand =3;
              }
              
              //                       ZTEHIS.ZTEHisCommand =1;
              
            }
            
          }
          
          
          RecvCntStart = 0;
          MESGState = HISTORYMESG_REQ;
          if(ZTEHIS.ZTEHisStatus==2)
          {
            
            if(ModuleHis!=DCHis)
            {
              switch(ModuleHis)
              {
              case ACHis:
                ModuleHis = RectHis;
                break;
              case RectHis:
                ModuleHis = DCHis;
                break;
              case DCHis:
                ModuleHis = ACHis;
                break;
              case EnvHis:
                ModuleHis = ACHis;
                break;
              }
              
              
            }
            else 
            {
              MESGState = ACINPUTINFO_REQ;
              ModuleHis=ACHis;
            }
            ZTEHIS.ZTEHisCommand =0;
            ZTEHIS.ZTEHisStatus =0;     
            
          }    
          break;
          
          
          //====================================== ACINFO MESSAGE ======================================//
        case  ACINPUTINFO_REQ:
          //						thao++;
          for (i = 0; i < REQacinputinfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQacinputinfo[i],1);
            
          }
          MESGState = ACINPUTINFO_RES;
          break;
        case  ACINPUTINFO_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          
          if (g_checksum == g_testchecksum)
          {
            sDcInfo.u32DCNoResponse = 0;
            private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 0;
            sAcInfo.facVolt[0] = hex2float(&RecvBuff[0],19);
            sAcInfo.facVolt[1] = hex2float(&RecvBuff[0],27);
            sAcInfo.facVolt[2] = hex2float(&RecvBuff[0],35);
          }
          else
          {
            sDcInfo.u32DCNoResponse++;
            if (sDcInfo.u32DCNoResponse > 3)
            {
              private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 1;
              g_testchecksum=0xFFFF;g_checksum=0;
              sDcInfo.u32DCNoResponse = 10;
              
              // xoa khi checksum sai
              sAcInfo.facVolt[0] = 0;
              sAcInfo.facVolt[1] = 0;
              sAcInfo.facVolt[2] = 0;
              sAcInfo.fAcHighThres = 0;
              sAcInfo.fAcLowThres = 0;
              sAcInfo.fAcInputCurrLimit = 0;
              sRectInfo.fAllRectDcVolt = 0;
              for(i=0; i<10; i++)
              {
                sRectInfo.sRAWRectParam[i].fRect_DcCurr = 0;
              }
              sDcInfo.sINFODcInfo.fVoltage = 0;
              sDcInfo.sINFODcInfo.fCurrent = 0;
              sDcInfo.sINFODcInfo.u8BatNo = 0;                          
              sDcInfo.sINFODcInfo.fBatt1Curr = 0;
              sDcInfo.sINFODcInfo.fBatt2Curr = 0;
              sDcInfo.sINFODcInfo.fBatt3Curr = 0;
              sDcInfo.sINFODcInfo.fSen1BattTemp = 0;
              sDcInfo.sINFODcInfo.fBatt1Volt = 0;
              sDcInfo.sINFODcInfo.fSen2BattTemp = 0;
              sDcInfo.sINFODcInfo.fBatt2Volt = 0;
              sDcInfo.sINFODcInfo.fSen3BattTemp = 0;
              sDcInfo.sINFODcInfo.fBatt3Volt = 0;
              sDcInfo.sINFODcInfo.fBatt1RmnCap = 0;
              sBattInfo.sRAWBattInfo.fFltVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fBotVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fTestVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fLVDDV = 0;
              sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fCCLVal = 0;
              sBattInfo.sRAWBattInfo.fTempCompVal = 0;
              sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = 0;
              sBattInfo.sRAWBattInfo.fCapTotal = 0;
              sBattInfo.sRAWBattInfo.fCapTotal2 = 0;
              sBattInfo.sRAWBattInfo.fAutoTestDay = 0;
              sBattInfo.sRAWBattInfo.fTestStartTime =  0;
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = ACPARAMETER_REQ;
          
          break;
        case  ACPARAMETER_REQ:
          for (i = 0; i < REQacinputinfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQacparameter[i],1);
          }
          MESGState = ACPARAMETER_RES;
          break;
        case  ACPARAMETER_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            sAcInfo.fAcHighThres = hex2float(&RecvBuff[0],13);
            sAcInfo.fAcLowThres = hex2float(&RecvBuff[0],21);
            sAcInfo.fAcInputCurrLimit = hex2float(&RecvBuff[0],29);
          }
          
          RecvCntStart = 0;
          MESGState = ACALARM_REQ;
          break;
        case  ACALARM_REQ:
          for (i = 0; i < REQacalarmLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQacalarm[i],1);
          }
          MESGState = ACALARM_RES;
          break;
        case  ACALARM_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            
            sAcInfo.u8Thres[0] = hex2byte(&RecvBuff[0],19);
            sAcInfo.u8Thres[1] = hex2byte(&RecvBuff[0],21);
            sAcInfo.u8Thres[2] = hex2byte(&RecvBuff[0],23);
            
            sAcInfo.u8_I_Thres[0] = hex2byte(&RecvBuff[0],39);
            sAcInfo.u8_I_Thres[1] = hex2byte(&RecvBuff[0],41);
            sAcInfo.u8_I_Thres[2] = hex2byte(&RecvBuff[0],43);
            sAcInfo.u8MainFail = hex2byte(&RecvBuff[0],31);
            sAcInfo.u8PowerOff = hex2byte(&RecvBuff[0],33);
            sAcInfo.u8ACSPD  =  hex2byte(&RecvBuff[0],35);
            // Chen lenh bao alarm ac tai day
          }
          
          
          RecvCntStart = 0;
          MESGState = RECTSTSZTE_REQ;
          break;
          //====================================== RECTINFO MESSAGE ======================================//
        case RECTSTSZTE_REQ:
          for (i = 0; i < REQrectstsLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQrectstszte[i],1);
            
          }
          MESGState = RECTSTSZTE_RES;
          break;
        case RECTSTSZTE_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            RectNumber = 0;
            for(i=0; i<10; i++)
            {
              Rectlosedtest = sRectInfo.sRAWRectParam[i].u8Rect_Sts; 
              sRectInfo.sRAWRectParam[i].u8Rect_OnPosition = hex2byte(&RecvBuff[0],25+10*i);
              if(sRectInfo.sRAWRectParam[i].u8Rect_OnPosition==1)
              {
                sRectInfo.sRAWRectParam[i].u8Rect_Sts = hex2byte(&RecvBuff[0],17+10*i);
                
                RectNumber++;
              }
              else
                sRectInfo.sRAWRectParam[i].u8Rect_Sts =0;
              
              if((Rectlosedtest ==1)&&(sRectInfo.sRAWRectParam[i].u8Rect_Sts==0))
              {
                Rectlosedtest =0;
                sRectInfo.sRAWRectParam[i].u8Rect_NoResp =1;
              }
              if((sRectInfo.sRAWRectParam[i].u8Rect_NoResp ==1)&&(sRectInfo.sRAWRectParam[i].u8Rect_Sts==1))
                sRectInfo.sRAWRectParam[i].u8Rect_NoResp =0;
            }
            sRectInfo.u8Rect_Num =RectNumber;
          }
          
          RecvCntStart = 0;
          MESGState = RECTINFOZTE_REQ;
          break;
        case  RECTINFOZTE_REQ:
          for (i = 0; i < REQrectinfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQrectinfo[i],1);
          }
          MESGState = RECTINFOZTE_RES;
          break;
        case  RECTINFOZTE_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            sRectInfo.fAllRectDcVolt = hex2float(&RecvBuff[0],15);
            for(i=0; i<10; i++)
            {
              if(sRectInfo.sRAWRectParam[i].u8Rect_Sts==1)
              {
                sRectInfo.sRAWRectParam[i].fRect_DcCurr = hex2float(&RecvBuff[0],25+10*i);
                if(sRectInfo.sRAWRectParam[i].fRect_DcCurr == 0x20202020)sRectInfo.sRAWRectParam[i].fRect_DcCurr =0;
              }
              else
              {
                sRectInfo.sRAWRectParam[i].fRect_DcCurr = 0;
              }
              
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = RECTALARM_REQ;
          break;
        case  RECTALARM_REQ:
          for (i = 0; i < REQrectalarmLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQrectalarm[i],1);
          }
          MESGState = RECTALARM_RES;
          break;
        case  RECTALARM_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            if(RecvBuff[2] == 0x30) // version 2.0
            {
              for(i=1; i<10; i++)
              {
                sRectInfo.sRAWRectParam[i-1].u8Rect_Fail = hex2byte(&RecvBuff[0],((i-1)*4+17)); // 00H normal, 01H Fail
                sRectInfo.sRAWRectParam[i-1].u8Rect_Out = 0; // 00H normal, 01H Fail
              }
            }
            else // version 2.1
            {
              for(i=1; i<10; i++)
              {
                sRectInfo.sRAWRectParam[i-1].u8Rect_Fail = hex2byte(&RecvBuff[0],((i-1)*6+17)); // 00H normal, 01H Fail
                sRectInfo.sRAWRectParam[i-1].u8Rect_Out = hex2byte(&RecvBuff[0],((i-1)*6+17+4)); // 00H normal, 01H Fail
              }
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = DCOUTPUT_REQ;
          break;
          //====================================== DCINFO MESSAGE ======================================//
          
        case  DCOUTPUT_REQ:
          for (i = 0; i < REQdcoutputLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQdcoutput[i],1);
          }
          MESGState = DCOUTPUT_RES;
          break;
        case  DCOUTPUT_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            sDcInfo.sINFODcInfo.fVoltage = hex2float(&RecvBuff[0],17);
            sDcInfo.sINFODcInfo.fCurrent = hex2float(&RecvBuff[0],25);
            sDcInfo.sINFODcInfo.u8BatNo = 2;                          //hex2float(&RecvBuff[0],33);
            sDcInfo.sINFODcInfo.fBatt1Curr = hex2float(&RecvBuff[0],35);
            sDcInfo.sINFODcInfo.fBatt2Curr = hex2float(&RecvBuff[0],43);
            sDcInfo.sINFODcInfo.fBatt3Curr = hex2float(&RecvBuff[0],51);
            sDcInfo.sINFODcInfo.fSen1BattTemp = hex2float(&RecvBuff[0],63);
            sDcInfo.sINFODcInfo.fBatt1Volt = hex2float(&RecvBuff[0],71);
            
            sDcInfo.sINFODcInfo.fSen2BattTemp = hex2float(&RecvBuff[0],79);
            sDcInfo.sINFODcInfo.fBatt2Volt = hex2float(&RecvBuff[0],87);
            sDcInfo.sINFODcInfo.fSen3BattTemp = hex2float(&RecvBuff[0],95);
            sDcInfo.sINFODcInfo.fBatt3Volt = hex2float(&RecvBuff[0],103);
            
            if(hex2byte(&RecvBuff[0],101)==1)
            {
              sDcInfo.sINFODcInfo.fBatt1RmnCap = hex2float(&RecvBuff[0],125);
            }
            
            //							sDcInfo.sINFODcInfo.fBatt2RmnCap = hex2float(&RecvBuff[0],80);
            //							sDcInfo.sINFODcInfo.fSen1AmbTemp = hex2float(&RecvBuff[0],104);
            //							sDcInfo.sINFODcInfo.fSen2AmbTemp = hex2float(&RecvBuff[0],112);
          }
          
          RecvCntStart = 0;
          MESGState = DCPARAMETER_REQ;
          break;
          
        case  DCPARAMETER_REQ:
          for (i = 0; i < REQdcparameterLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&REQdcparameter[i],1);
            //                        athao[i]=REQdcparameter[i];
          }
          MESGState = DCPARAMETER_RES;
          break;
        case  DCPARAMETER_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            if(RecvBuff[2] == 0x30) // version 2.0
            {
              sBattInfo.sRAWBattInfo.fFltVoltCfg = roundf(hex2float(&RecvBuff[0],31)*10)/10;
              sBattInfo.sRAWBattInfo.fBotVoltCfg = roundf(hex2float(&RecvBuff[0],39)*10)/10;
              sBattInfo.sRAWBattInfo.fTestVoltCfg = roundf(hex2float(&RecvBuff[0],47)*10)/10;
              sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],55)*10)/10;
              sBattInfo.sRAWBattInfo.fLVDDV = roundf(hex2float(&RecvBuff[0],71)*10)/10;
              sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],63)*10)/10;
              sBattInfo.sRAWBattInfo.fCCLVal = roundf(hex2float(&RecvBuff[0],79)*1000)/1000;
              sBattInfo.sRAWBattInfo.fTempCompVal = hex2float(&RecvBuff[0],95);
              sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = roundf(hex2float(&RecvBuff[0],103)*10)/10;
              sBattInfo.sRAWBattInfo.fCapTotal = hex2float(&RecvBuff[0],111);
              sBattInfo.sRAWBattInfo.fCapTotal2 = hex2float(&RecvBuff[0],119);
              sBattInfo.sRAWBattInfo.fAutoTestDay = 0;
              sBattInfo.sRAWBattInfo.fTestStartTime =  0;
            }
            else // version 2.1
            {
              sBattInfo.sRAWBattInfo.fFltVoltCfg = roundf(hex2float(&RecvBuff[0],31)*10)/10;
              sBattInfo.sRAWBattInfo.fBotVoltCfg = roundf(hex2float(&RecvBuff[0],39)*10)/10;
              sBattInfo.sRAWBattInfo.fTestVoltCfg = roundf(hex2float(&RecvBuff[0],47)*10)/10;
              sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],55)*10)/10;
              sBattInfo.sRAWBattInfo.fLVDDV = roundf(hex2float(&RecvBuff[0],71)*10)/10;
              sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],63)*10)/10;
              sBattInfo.sRAWBattInfo.fCCLVal = roundf(hex2float(&RecvBuff[0],79)*1000)/1000;
              sBattInfo.sRAWBattInfo.fTempCompVal = hex2float(&RecvBuff[0],95);
              sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = roundf(hex2float(&RecvBuff[0],103)*10)/10;
              sBattInfo.sRAWBattInfo.fCapTotal = hex2float(&RecvBuff[0],111);
              sBattInfo.sRAWBattInfo.fCapTotal2 = hex2float(&RecvBuff[0],119);
              sBattInfo.sRAWBattInfo.fAutoTestDay = hex2float(&RecvBuff[0],183);
              sBattInfo.sRAWBattInfo.fTestStartTime =  hex2float(&RecvBuff[0],199);
            }
          }
          
          RecvCntStart = 0;
          MESGState = DCALARM_REQ;
          //                        MESGState = DCPARAMETER_REQ;
          break;
        case  DCALARM_REQ:
          for (i = 0; i < REQdcalarmLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQdcalarm[i],1);
          }
          MESGState = DCALARM_RES;
          break;
        case  DCALARM_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            if(RecvBuff[2] == 0x30) // version 2.0
            {
              sDcInfo.sALARMDcInfo.u8DC = hex2byte(&RecvBuff[0],17);
              sDcInfo.sALARMDcInfo.u8DC_NoFuse = hex2byte(&RecvBuff[0],19); // So switch trong tu
              val = sDcInfo.sALARMDcInfo.u8DC_NoFuse;
              for (i = 0; i < val; i++)
              {
                sDcInfo.sALARMDcInfo.u8LoadFuse[i] = hex2byte(&RecvBuff[0],9 + 2*i+12);
                if((i<10)&&(sDcInfo.sALARMDcInfo.u8LoadFuse[i] == 0x03))private_mib_base_struct.loadGroup.loadStatus[i]=1;
              }
              for (i = 0; i < 3; i++)
              {
                sDcInfo.sALARMDcInfo.u8BattFuse[i] = hex2byte(&RecvBuff[0],((val+7)*2+1+i*6+12));
                if(sDcInfo.sALARMDcInfo.u8BattFuse[i] == 0x03)
                  private_mib_base_struct.batteryGroup.battBrkStatus[i] = 1;
              }
              sDcInfo.sALARMDcInfo.u8LLVD = hex2byte(&RecvBuff[0],(val+5)*2+13);
              sDcInfo.sALARMDcInfo.u8BLVD = hex2byte(&RecvBuff[0],(val+6)*2+13);
              sDcInfo.sALARMDcInfo.u8BatVol[0] = hex2byte(&RecvBuff[0],(val+8)*2+13);   // Canh bao bat 1 low voltage
              sDcInfo.sALARMDcInfo.u8Sen1_BattTemp = hex2byte(&RecvBuff[0],(val+9)*2+13);  // Canh bao nhiet do acquy1 cao
              sDcInfo.sALARMDcInfo.u8Sen1_BattInvalid = 0; // Canh bao batt temp invalid
              sDcInfo.sALARMDcInfo.u8BatVol[1] = hex2byte(&RecvBuff[0],(val+11)*2+13);   // Canh bao bat 2 low voltage
              sDcInfo.sALARMDcInfo.u8Sen2_BattTemp = hex2byte(&RecvBuff[0],(val+12)*2+13); // Canh bao nhiet do acquy2 cao
              sDcInfo.sALARMDcInfo.u8Sen2_BattInvalid = 0; // Canh bao batt temp invalid
              sDcInfo.sALARMDcInfo.u8Sen3_BattTemp = hex2byte(&RecvBuff[0],(val+15)*2+13); // Canh bao nhiet do acquy3 cao
              sDcInfo.sALARMDcInfo.u8Sen3_BattInvalid = 0; // Canh bao batt temp invalid
              sDcInfo.sALARMDcInfo.u8DCSPD = hex2byte(&RecvBuff[0],(val+16)*2+13);
            }
            else // version 2.1
            {
              sDcInfo.sALARMDcInfo.u8DC = hex2byte(&RecvBuff[0],17);
              sDcInfo.sALARMDcInfo.u8DC_NoFuse = hex2byte(&RecvBuff[0],19); // So switch trong tu
              val = sDcInfo.sALARMDcInfo.u8DC_NoFuse;
              for (i = 0; i < val; i++)
              {
                sDcInfo.sALARMDcInfo.u8LoadFuse[i] = hex2byte(&RecvBuff[0],9 + 2*i+12);
                if((i<10)&&(sDcInfo.sALARMDcInfo.u8LoadFuse[i] == 0x03))private_mib_base_struct.loadGroup.loadStatus[i]=1;
              }
              //
              for (i = 0; i < 3; i++)
              {
                sDcInfo.sALARMDcInfo.u8BattFuse[i] = hex2byte(&RecvBuff[0],((val+7)*2+1+i*10+12));
                if(sDcInfo.sALARMDcInfo.u8BattFuse[i] == 0x03)
                  private_mib_base_struct.batteryGroup.battBrkStatus[i] = 1;
              }
              //									for (i = 0; i < 8; i++)
              //									{
              //										sDcInfo.sALARMDcInfo.u8DIFuse[i] = hex2byte(&RecvBuff[0],79 + 2*i);
              //									}
              //
              //									sDcInfo.sALARMDcInfo.u8Batt1_OC = hex2byte(&RecvBuff[0],51);
              //									sDcInfo.sALARMDcInfo.u8Batt2_OC = hex2byte(&RecvBuff[0],53);
              //							sDcInfo.sALARMDcInfo.u8BLVD = hex2byte(&RecvBuff[0],(val+8)*2+13);
              sDcInfo.sALARMDcInfo.u8LLVD = hex2byte(&RecvBuff[0],(val+5)*2+13);
              sDcInfo.sALARMDcInfo.u8BLVD = hex2byte(&RecvBuff[0],(val+6)*2+13);
              sDcInfo.sALARMDcInfo.u8Sen1_BattTemp = hex2byte(&RecvBuff[0],(val+9)*2+13);  // Canh bao nhiet do acquy1 cao
              sDcInfo.sALARMDcInfo.u8BatVol[0] = hex2byte(&RecvBuff[0],(val+8)*2+13);   // Canh bao bat 1 low voltage
              sDcInfo.sALARMDcInfo.u8Sen1_BattInvalid = hex2byte(&RecvBuff[0],(val+11)*2+13); // Canh bao batt temp invalid
              sDcInfo.sALARMDcInfo.u8Sen2_BattTemp = hex2byte(&RecvBuff[0],(val+14)*2+13); // Canh bao nhiet do acquy2 cao
              sDcInfo.sALARMDcInfo.u8BatVol[1] = hex2byte(&RecvBuff[0],(val+13)*2+13);   // Canh bao bat 2 low voltage
              sDcInfo.sALARMDcInfo.u8Sen2_BattInvalid = hex2byte(&RecvBuff[0],(val+16)*2+13); // Canh bao batt temp invalid
              sDcInfo.sALARMDcInfo.u8Sen3_BattTemp = hex2byte(&RecvBuff[0],(val+19)*2+13); // Canh bao nhiet do acquy3 cao
              sDcInfo.sALARMDcInfo.u8Sen3_BattInvalid = hex2byte(&RecvBuff[0],(val+21)*2+13); // Canh bao batt temp invalid
              sDcInfo.sALARMDcInfo.u8DCSPD = hex2byte(&RecvBuff[0],(val+22)*2+13);
              //									sDcInfo.sALARMDcInfo.u8Sen1_AmbTemp = hex2byte(&RecvBuff[0],63);
              //									sDcInfo.sALARMDcInfo.u8Sen2_AmbTemp = hex2byte(&RecvBuff[0],65);
              sDcInfo.sALARMDcInfo.u8Batt_Discharge = hex2byte(&RecvBuff[0],(val+26)*2+13);
              //									sDcInfo.sALARMDcInfo.u8OutVoltFault = hex2byte(&RecvBuff[0],77);
              //							sDcInfo.sALARMDcInfo.u8BatVol[0]=hex2byte(&RecvBuff[0],17);
              
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = ENVALARM_REQ;
          break;
          
          
        case  ENVALARM_REQ:
          for (i = 0; i < REQenvalarmLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQenvalarm[i],1);
          }
          MESGState = ENVALARM_RES;
          break;
        case  ENVALARM_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            if(RecvBuff[2] == 0x30) // version 2.0
            {
              sAlarmEnvInfo.u8EnvHumidity = hex2byte(&RecvBuff[0],21);
              sAlarmEnvInfo.u8EnvSmoke = hex2byte(&RecvBuff[0],25);
              sAlarmEnvInfo.u8EnvWater = hex2byte(&RecvBuff[0],29);
              sAlarmEnvInfo.u8EnvInfra = hex2byte(&RecvBuff[0],33);
              sAlarmEnvInfo.u8EnvDoor = hex2byte(&RecvBuff[0],37);
              sAlarmEnvInfo.u8EnvGlass = hex2byte(&RecvBuff[0],41);
              sAlarmEnvInfo.u8HeatExch = hex2byte(&RecvBuff[0],47);
            }
            else // version 2.1
            {
              sAlarmEnvInfo.u8EnvTemp = hex2byte(&RecvBuff[0],49);
              sAlarmEnvInfo.u8EnvHumidity = hex2byte(&RecvBuff[0],21);
              sAlarmEnvInfo.u8EnvSmoke = hex2byte(&RecvBuff[0],25);
              sAlarmEnvInfo.u8EnvWater = hex2byte(&RecvBuff[0],29);
              sAlarmEnvInfo.u8EnvInfra = hex2byte(&RecvBuff[0],33);
              sAlarmEnvInfo.u8EnvDoor = hex2byte(&RecvBuff[0],37);
              sAlarmEnvInfo.u8EnvGlass = hex2byte(&RecvBuff[0],41);
              sAlarmEnvInfo.u8HeatExch = hex2byte(&RecvBuff[0],47);
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = ENVDATA_REQ;
          break;
        case  ENVDATA_REQ:
          for (i = 0; i < REQenvdataLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQenvdata[i],1);
          }
          MESGState = ENVDATA_RES;
          break;
        case  ENVDATA_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            if((sAlarmEnvInfo.u8EnvTemp!=0x05)&&(sAlarmEnvInfo.u8EnvTemp!=0xF5))
            {
              sDcInfo.sINFODcInfo.fSen1AmbTemp = hex2float(&RecvBuff[0],17);
              
            }
            else
            {
              sDcInfo.sINFODcInfo.fSen1AmbTemp=0;
              sAlarmInfo.u32AmbTemp = 1;
            }
            
          }
          
          RecvCntStart = 0;
          MESGState = TIMEINFO_REQ;
          break;
        case  TIMEINFO_REQ:
          for (i = 0; i < REQtimeinfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&REQtimeinfo[i],1);
          }
          MESGState = TIMEINFO_RES;
          break;
        case  TIMEINFO_RES:
          for(i=0; i < RecvCntStart; i++)
          {
            RecvBuff[i]=g_UARTRxBuf[i];
            g_UARTRxBuf[i] =0;
          }
          RecvCount = RecvCntStart;
          
          if(RecvCount>6)
          {
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-6);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount-1);
          }
          if (g_checksum == g_testchecksum)
          {
            // nhan thong so thoi gian
            if(( RecvBuff[7]==0x34)&&( RecvBuff[8]==0x44))
            {
              ZTETime.year= hex2byte2(&RecvBuff[0],13);
              ZTETime.month = hex2byte(&RecvBuff[0],17);
              ZTETime.date = hex2byte(&RecvBuff[0],19);
              ZTETime.hour = hex2byte(&RecvBuff[0],21);
              ZTETime.min = hex2byte(&RecvBuff[0],23);
              ZTETime.sec = hex2byte(&RecvBuff[0],25);
            }               
          }
          
          RecvCntStart = 0;
          MESGState = UPDATE_OK;
          break;
        case UPDATE_OK:
          // ======================== Update dc information to SNMP resource ==========================//
          private_mib_base_struct.batteryGroup.battVolt = (int32_t) (sDcInfo.sINFODcInfo.fBatt1Volt * 100);
          private_mib_base_struct.batteryGroup.battCurr = (int32_t) ((sDcInfo.sINFODcInfo.fBatt1Curr + sDcInfo.sINFODcInfo.fBatt2Curr) * 100);
          private_mib_base_struct.batteryGroup.battCapLeft1 = (uint32_t) (sDcInfo.sINFODcInfo.fBatt1RmnCap * 100);
          private_mib_base_struct.batteryGroup.battTemp = (uint32_t) (sDcInfo.sINFODcInfo.fSen1BattTemp*10);
          if(sDcInfo.sALARMDcInfo.u8Sen1_BattInvalid==0xF5)
          {
            //							private_mib_base_struct.batteryGroup.battTemp = sDcInfo.sINFODcInfo.fSen2BattTemp*10;
            private_mib_base_struct.batteryGroup.battTemp =0;
          }
          
          private_mib_base_struct.batteryGroup.battBanksNumofBanks = sDcInfo.sINFODcInfo.u8BatNo;
          
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
          if((sAcInfo.u8MainFail==0x05)||(sAcInfo.u8PowerOff==0xE0))
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains =1;
          }
          else private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains =0;
          if(sAcInfo.u8ACSPD==0x05)
          {
            private_mib_base_struct.main_alarm_group_struct.alarmACSPD =1;
          }
          else private_mib_base_struct.main_alarm_group_struct.alarmACSPD =0;
          if(sDcInfo.sALARMDcInfo.u8DCSPD==0x05)
          {
            private_mib_base_struct.main_alarm_group_struct.alarmDCSPD =1;
          }
          else private_mib_base_struct.main_alarm_group_struct.alarmDCSPD =0;
          if(sDcInfo.sALARMDcInfo.u8LLVD==0xF0)
            private_mib_base_struct.main_alarm_group_struct.alarm_llvd = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_llvd = 0;
          //						private_mib_base_struct.main_alarm_group_struct.alarmLLVD = sDcInfo.sALARMDcInfo.u8LLVD;
          if(sDcInfo.sALARMDcInfo.u8BLVD == 0xF1)private_mib_base_struct.main_alarm_group_struct.alarm_blvd = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_blvd = 0;
          //						private_mib_base_struct.main_alarm_group_struct.alarm_blvd = sDcInfo.sALARMDcInfo.u8BLVD;
          if((sDcInfo.sALARMDcInfo.u8BatVol[0]==1)||(sDcInfo.sALARMDcInfo.u8BatVol[1]==1))
            private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 0x01;
          else private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 0x00;
          private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0;
          if(sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x02)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x02;
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t) (sDcInfo.sINFODcInfo.fSen1BattTemp * 10);
          }
          //						else if((sDcInfo.sALARMDcInfo.u8Sen1_BattInvalid!=0xF5)&&(sDcInfo.sALARMDcInfo.u8Sen1_BattInvalid!=0x05))
          //						{
          //							private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x00;
          //							private_mib_base_struct.batteryGroup.battTemp = sDcInfo.sINFODcInfo.fSen1BattTemp * 10;
          //						}
          //						if(sDcInfo.sALARMDcInfo.u8Sen2_BattTemp == 0x02)
          //						{
          //							private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x02;
          //							private_mib_base_struct.batteryGroup.battTemp = sDcInfo.sINFODcInfo.fSen2BattTemp * 10;
          //						}
          //						else if((sDcInfo.sALARMDcInfo.u8Sen2_BattInvalid!=0xF5)&&(sDcInfo.sALARMDcInfo.u8Sen2_BattInvalid!=0x05))
          //						{
          //							private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x00;
          //							private_mib_base_struct.batteryGroup.battTemp = sDcInfo.sINFODcInfo.fSen2BattTemp * 10;
          //						}
          //						if((sDcInfo.sALARMDcInfo.u8Sen1_BattInvalid==0xF5)&&(sDcInfo.sALARMDcInfo.u8Sen2_BattInvalid==0xF5))
          //						{
          //							private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x05;
          //							private_mib_base_struct.batteryGroup.battTemp = 0;
          //						}
          
          
          // ======================== Update dc information to SNMP resource ==========================//
          // ======================== Update rect information to SNMP resource ==========================//
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
          
          //                    sRectInfo.sSNMPRectParam[0].u32Rect_State = 0;
          for(i=0; i<10; i++)
          {
            private_mib_base_struct.rectGroup.rectTable[i].rectStatus = 0;
            private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp |= sRectInfo.sRAWRectParam[i].u8Rect_NoResp << i;
            private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= sRectInfo.sRAWRectParam[i].u8Rect_Fail << i;
            //							sRectInfo.sSNMPRectParam[i].u32Rect_Fail = sRectInfo.sRAWRectParam[i].u8Rect_Fail;
            //                        sRectInfo.sSNMPRectParam[i].u32Rect_Out = sRectInfo.sRAWRectParam[i].u8Rect_Out;
            if(sRectInfo.sRAWRectParam[i].u8Rect_Sts==1)
            {
              private_mib_base_struct.rectGroup.rectTable[i].rectStatus = 0xFF; // Rect dang hoat dong
            }
            if(sRectInfo.sRAWRectParam[i].u8Rect_NoResp==1)
            {
              private_mib_base_struct.rectGroup.rectTable[i].rectStatus = 0xAA; // Rect no respond
            }
            if(sRectInfo.sRAWRectParam[i].u8Rect_Out==1)
            {
              private_mib_base_struct.rectGroup.rectTable[i].rectStatus = 0x0A; // Rect dc off
            }
            if(sRectInfo.sRAWRectParam[i].u8Rect_Fail==1)
            {
              private_mib_base_struct.rectGroup.rectTable[i].rectStatus = 0xA5; // Rect fail
            }
            
            private_mib_base_struct.rectGroup.rectTable[i].rectOutputCurrent = (uint32_t) (sRectInfo.sRAWRectParam[i].fRect_DcCurr * 100);
            //							sRectInfo.sSNMPRectParam[i].u32Rect_Temp = sRectInfo.sRAWRectParam[i].fRect_Temp * 100;
            //							if(private_mib_base_struct.rectGroup.rectTable[i].rectOutputCurrent >0)
            if(sRectInfo.sRAWRectParam[i].u8Rect_Sts==1)
              private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =(uint32_t) (sRectInfo.fAllRectDcVolt * 100);
            else private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =0;
            //
          }
          if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
          if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
          private_mib_base_struct.rectGroup.rectTotalCurrent = 0;
          for (i = 0; i < 10; i++)
          {
            private_mib_base_struct.rectGroup.rectTotalCurrent += (uint32_t) (private_mib_base_struct.rectGroup.rectTable[i].rectOutputCurrent);
          }
          private_mib_base_struct.rectGroup.rectInstalledRect = 4;//10;
          private_mib_base_struct.rectGroup.rectActiveRect = sRectInfo.u8Rect_Num;
          
          private_mib_base_struct.loadGroup.loadCurrent = (uint32_t) (sDcInfo.sINFODcInfo.fCurrent*100);
          //						private_mib_base_struct.loadGroup.loadCurrent = private_mib_base_struct.rectGroup.rectTotalCurrent - private_mib_base_struct.batteryGroup.battCurr;
          //						if (private_mib_base_struct.loadGroup.loadCurrent >= 0) private_mib_base_struct.loadGroup.loadCurrent = private_mib_base_struct.loadGroup.loadCurrent;
          //						else if (private_mib_base_struct.loadGroup.loadCurrent < 0) private_mib_base_struct.loadGroup.loadCurrent = -private_mib_base_struct.loadGroup.loadCurrent;
          //						private_mib_base_struct.main_alarm_group_struct.alarmManualMode = g_AlrmMan;
          
          // ======================== Update rect information to SNMP resource ==========================//
          // ======================== Update ac information to SNMP resource ==========================//
          private_mib_base_struct.acPhaseGroup.acPhaseTable[0].acPhaseVolt = (int32_t) (sAcInfo.facVolt[0] * 100);
          private_mib_base_struct.acPhaseGroup.acPhaseTable[1].acPhaseVolt = (int32_t) (sAcInfo.facVolt[1] * 100);
          private_mib_base_struct.acPhaseGroup.acPhaseTable[2].acPhaseVolt = (int32_t) (sAcInfo.facVolt[2] * 100);
          if (sAcInfo.u8Thres[0] || sAcInfo.u8Thres[1] || sAcInfo.u8Thres[2])
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_low = 1;
          }
          else
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_low = 0;
          }
          
          // ======================== Update ac information to SNMP resource ==========================//
          
          // ======================== Update Config information to SNMP resource ==========================//
          private_mib_base_struct.configGroup.cfgFloatVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fFltVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgBoostVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fBotVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgTempCompValue = (uint32_t) sBattInfo.sRAWBattInfo.fTempCompVal;
          private_mib_base_struct.configGroup.cfgLLVDVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgBLVDVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLVDDV * 100);
          private_mib_base_struct.configGroup.cfgDCLowVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgBattTestVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fTestVoltCfg * 100);
          
          private_mib_base_struct.cfgBTGroup.cfgTestVoltCfg = (uint32_t)(sBattInfo.sRAWBattInfo.fTestVoltCfg*10);
          private_mib_base_struct.cfgBTGroup.cfgAutoTestDay = (uint32_t)sBattInfo.sRAWBattInfo.fAutoTestDay;
          private_mib_base_struct.cfgBTGroup.cfgTestStartTime = (uint32_t)sBattInfo.sRAWBattInfo.fTestStartTime;
          //						private_mib_base_struct.batteryGroup.battBanksNumofBanks = sBattInfo.sRAWBattInfo.u8BankNo;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal2 = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal2;
          private_mib_base_struct.configGroup.cfgCurrentLimit = (uint32_t) (sBattInfo.sRAWBattInfo.fCCLVal * 1000);
          
          sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel = 100;
          private_mib_base_struct.configGroup.cfgOverMajorTempLevel = (uint32_t) (sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel * 100);
          private_mib_base_struct.configGroup.cfgHighMajorTempLevel = (uint32_t) (sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel *100);
          
          //						private_mib_base_struct.configGroup.cfgWalkInTimeDuration = sRectInfo.u8WITI;
          //						private_mib_base_struct.configGroup.cfgWalkInTimeEn = sRectInfo.u8WITE;
          private_mib_base_struct.configGroup.cfgAcLowLevel = (uint32_t) sAcInfo.fAcLowThres;
          
          // ======================== Update Config information to SNMP resource ==========================//
          // ======================== Update Env information to SNMP resource ==========================//
          private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t) (sDcInfo.sINFODcInfo.fSen1AmbTemp*10);
          sAlarmInfo.u32AmbTemp = 0;
          if(sAlarmEnvInfo.u8EnvTemp == 0x04)
          {
            sAlarmInfo.u32AmbTemp = 0x02;
          }
          if((sAlarmEnvInfo.u8EnvTemp == 0x05)||(sAlarmEnvInfo.u8EnvTemp == 0xF5))
          {
            sAlarmInfo.u32AmbTemp = 0x05;
          }
          private_mib_base_struct.main_alarm_group_struct.alarm_smoke = 0;
          if(sAlarmEnvInfo.u8EnvSmoke==0x04)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_smoke = 0x01;
          }
          //						if(sAlarmEnvInfo.u8EnvSmoke==0x05)
          //						{
          //							sAlarmInfo.u32Smoke = 0x05;
          //						}
          
          sAlarmInfo.u32EnvDoor = 0;
          if(sAlarmEnvInfo.u8EnvDoor==0x04)
          {
            sAlarmInfo.u32EnvDoor = 0x01;
          }
          //						if(sAlarmEnvInfo.u8EnvDoor==0x05)
          //						{
          //							sAlarmInfo.u32EnvDoor = 0x05;
          //						}
          sAlarmInfo.u32HeatExch = 0;
          if(sAlarmEnvInfo.u8HeatExch==0x04)
          {
            sAlarmInfo.u32HeatExch = 0x01;
          }
          //						if(sAlarmEnvInfo.u8HeatExch==0x05)
          //						{
          //							sAlarmInfo.u32HeatExch = 0x05;
          //						}
          private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0] = sAlarmInfo.u32HeatExch;
          
          // ======================== Update Env information to SNMP resource ==========================//                            
          MESGState = ACINPUTINFO_REQ;
          break;
        default:
          break;
        }
        if(g_testchecksum != g_checksum)
        {
          reverror++;
          MESGStatetest[k] =  MESGState;  
          k++;
          if(k>19) k =0;
        }
        
        
        
      }
    }
    
    
    vTaskDelay(400);
  }
  while (1); 
}