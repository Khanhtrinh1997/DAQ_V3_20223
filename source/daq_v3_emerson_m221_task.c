//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
uint8_t RectNumber = 0;
uint8_t Rectlosedtest=0;
uint8_t val;
uint32_t reverror =0;
tMESGState MESGStatetest[20];
uint8_t k=0;
void emersion_m221_task(void* pv_parameters){
  private_mib_base_struct.cntGroup.task.emerson_m221 =0;
  
  uint32_t i;
  uint32_t fuse_flag;
  MESGState = ACINPUTINFO_REQ;        
  
  do
  {
    private_mib_base_struct.cntGroup.task.emerson_m221++;
    if (sInterruptMesg.u8Checkbit == 1)
    {
      sInterruptMesg.u8Checkbit = 0;
    }
    else if (sInterruptMesg.u8Checkbit == 0)
    {
      
      
      //====================================== HISTORY MESSAGE ======================================//                
      if(MESGState_v21 == HISTORYMESG_REQ)             
      {
        GetHistoryLog_v21();
        MESGState_v21 = HISTORYMESG_RES;
      }
      
      //====================================== HISTORY MESSAGE ======================================// 
      if (setCmd_flag == 1)
      {
        settingCommand |= setCmd_mask;
        
        switch(settingCommand)
        {
        case SET_FLTVOL:
          sBattInfo.sRAWBattInfo.fFltVoltCfg = (float)private_mib_base_struct.configGroup.cfgFloatVolt/100;
          //                    sBattInfo.sRAWBattInfo.fFltVoltCfg = a;
          EMERV21_SetCommand(&g_setInfo[0],EMER_FLOATVOLT,sBattInfo.sRAWBattInfo.fFltVoltCfg,DC);     
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1); 
          }
          break;
        case SET_BSTVOL:
          sBattInfo.sRAWBattInfo.fBotVoltCfg = (float)private_mib_base_struct.configGroup.cfgBoostVolt/100;
          EMERV21_SetCommand(&g_setInfo[0],EMER_BOOSTVOLT,sBattInfo.sRAWBattInfo.fBotVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_TEMPCOMP_VAL:
          sBattInfo.sRAWBattInfo.fTempCompVal = (float)private_mib_base_struct.configGroup.cfgTempCompValue;
          EMERV21_SetCommand(&g_setInfo[0],EMER_TEMP_COMPENSATION,sBattInfo.sRAWBattInfo.fTempCompVal,DC);
          for (i = 0; i < 28; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_LLVD:
          sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = (float)private_mib_base_struct.configGroup.cfgLLVDVolt/100;
          EMERV21_SetCommand(&g_setInfo[0],EMER_LLVD,sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_BLVD:
          sBattInfo.sRAWBattInfo.fLVDDV = (float)private_mib_base_struct.configGroup.cfgBLVDVolt/100;
          EMERV21_SetCommand(&g_setInfo[0],EMER_BLVD,sBattInfo.sRAWBattInfo.fLVDDV,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_DCOVER:
          sBattInfo.sRAWBattInfo.fDCOverCfg = (float)private_mib_base_struct.configGroup.cfgDCOverVolt/100;
          EMERV21_SetCommand(&g_setInfo[0],EMER_DCVOLT_UPPERLIMIT,sBattInfo.sRAWBattInfo.fDCOverCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_DCLOW:
          sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = (float)private_mib_base_struct.configGroup.cfgDCLowVolt/100;
          EMERV21_SetCommand(&g_setInfo[0],EMER_DCVOLT_LOWERLIMIT,sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_BATTSTDCAP:
          sBattInfo.sRAWBattInfo.fCapTotal = (float)private_mib_base_struct.configGroup.cfgBattCapacityTotal;
          EMERV21_SetCommand(&g_setInfo[0],EMER_BATT_CAP,sBattInfo.sRAWBattInfo.fCapTotal,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
          
        case SET_CCL:
          sBattInfo.sRAWBattInfo.fCCLVal = (float)private_mib_base_struct.configGroup.cfgCurrentLimit/1000;
          sBattInfo.sRAWBattInfo.fCapCCLVal  = sBattInfo.sRAWBattInfo.fCCLVal* sBattInfo.sRAWBattInfo.fCapTotal;
          EMERV21_SetCommand(&g_setInfo[0],EMER_CHARGINGFACTOR,sBattInfo.sRAWBattInfo.fCapCCLVal,DC);
          
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_OVERTEMP:
          sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = (float)private_mib_base_struct.configGroup.cfgHighMajorTempLevel/100;
          EMERV21_SetCommand(&g_setInfo[0],EMER_BATT_TEMP_OVER,sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel,DC);
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
          
        case SET_AC_THRES:
          sAcInfo.fAcLowThres = (float)private_mib_base_struct.configGroup.cfgAcLowLevel;
          EMERV21_SetCommand(&g_setInfo[0],EMER_ACVOLT_LOWERLIMIT,sAcInfo.fAcLowThres,AC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_AC_HIGH_THRES:
          sAcInfo.fAcHighThres = (float)private_mib_base_struct.configGroup.cfgAcHighLevel;
          EMERV21_SetCommand(&g_setInfo[0],EMER_ACVOLT_UPPERLIMIT,sAcInfo.fAcHighThres,AC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_AC_INPUT_CURR_LIMIT:           // ????????????????????
          //					sAcInfo.fAcHighThres = (float)private_mib_base_struct.configGroup.cfgAcLowLevel;
          EMERV21_SetCommand(&g_setInfo[0],EMER_ACINPUTCUR_UPPERLIMIT,sAcInfo.fAcInputCurrLimit,AC);
          for (i = 0; i < 28; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
          }
          break;
        case SET_TEST_VOLT:
          sBattInfo.sRAWBattInfo.fTestVoltCfg = (float)private_mib_base_struct.cfgBTGroup.cfgTestVoltCfg/10;
          EMERV21_SetCommand(&g_setInfo[0],EMER_TESTVOLT,sBattInfo.sRAWBattInfo.fTestVoltCfg,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
            
          }
          break;
        case SET_BT_ENDTIME:           
          sBattTestInfo.fBattTestDur = (float) private_mib_base_struct.cfgBTGroup.cfgBTEndTime;
          EMERV21_SetCommand(&g_setInfo[0],EMER_TESTTERMINATION,sBattTestInfo.fBattTestDur,DC);
          for (i = 0; i < 28; i++)
          {
            LPUART_WriteBlocking(LPUART5,&g_setInfo[i],1);
            
          }
          break;
        case SET_BT_ENDCAP:           
          sBattTestInfo.fTestEndCap = (float) private_mib_base_struct.cfgBTGroup.cfgBTEndCap/1000;
          EMERV21_SetCommand(&g_setInfo[0],EMER_AFTERTESTCAP,sBattTestInfo.fTestEndCap,DC);
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
        }               
        
        switch(MESGState)
        {
          //====================================== ACINFO MESSAGE ======================================//
        case  ACINPUTINFO_REQ:
          //						thao++;
          for (i = 0; i < GetACAnalogInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetACAnalogInfo[i],1);
            
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          
          if (g_checksum == g_testchecksum)
          {
            sDcInfo.u32DCNoResponse = 0;
            private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 0;
            sAcInfo.facVolt[0] = hex2float(&RecvBuff[0],17);
            sAcInfo.facVolt[1] = hex2float(&RecvBuff[0],25);
            sAcInfo.facVolt[2] = hex2float(&RecvBuff[0],33);
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
              for(i=0; i<24; i++)
              {
                sRectInfo.sRAWRectParam[i].fRect_DcVolt = 0;
                sRectInfo.sRAWRectParam[i].fRect_DcCurr = 0;
                sRectInfo.sRAWRectParam[i].fRect_Temp = 0;
              }
              sDcInfo.sINFODcInfo.fVoltage = 0;
              sDcInfo.sINFODcInfo.fCurrent = 0;
              sDcInfo.sINFODcInfo.fBatt1Curr = 0;
              sDcInfo.sINFODcInfo.fBatt2Curr = 0;
              sDcInfo.sINFODcInfo.fBatt1Volt = 0;
              sDcInfo.sINFODcInfo.fBatt2Volt = 0;                     
              sDcInfo.sINFODcInfo.fSen1BattTemp = 0;
              sDcInfo.sINFODcInfo.fSen2BattTemp = 0;
              sDcInfo.sINFODcInfo.fBatt1RmnCap = 0;
              sDcInfo.sINFODcInfo.fBatt2RmnCap = 0;
              sBattInfo.sRAWBattInfo.fDCOverCfg = 0;
              sBattInfo.sRAWBattInfo.fFltVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fBotVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fTestVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fLVDDV = 0;
              sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = 0;
              sBattInfo.sRAWBattInfo.fCapCCLVal= 0;
              sBattInfo.sRAWBattInfo.fTempCompVal = 0;
              sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = 0;
              sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel = 0;
              sBattInfo.sRAWBattInfo.fCapTotal = 0;
              sBattInfo.sRAWBattInfo.fCCLVal = 0;
              sBattTestInfo.fBattTestDur = 0;
              sBattTestInfo.fTestEndCap =  0;
              sDcInfo.sINFODcInfo.u8BatNo = 0;
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = ACPARAMETER_REQ;
          
          break;
        case  ACPARAMETER_REQ:
          for (i = 0; i < GetACParameterInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetACParameterInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5); 
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
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
          for (i = 0; i < GetACACAlarmInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetACAlarmInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            
            sAcInfo.u8Thres[0] = hex2byte(&RecvBuff[0],17);
            sAcInfo.u8Thres[1] = hex2byte(&RecvBuff[0],19);
            sAcInfo.u8Thres[2] = hex2byte(&RecvBuff[0],21);
            
            sAcInfo.u8_I_Thres[0] = hex2byte(&RecvBuff[0],57);
            sAcInfo.u8_I_Thres[1] = hex2byte(&RecvBuff[0],59);
            sAcInfo.u8_I_Thres[2] = hex2byte(&RecvBuff[0],61);
            sAcInfo.u8MainFail = hex2byte(&RecvBuff[0],29);
            sAcInfo.u8PowerOff = hex2byte(&RecvBuff[0],35);
            sAcInfo.u8ACSPD  =  hex2byte(&RecvBuff[0],33);
            // Chen lenh bao alarm ac tai day
          }
          
          
          RecvCntStart = 0;
          MESGState = RECTSTSZTE_REQ;
          break;
          //====================================== RECTINFO MESSAGE ======================================//
        case RECTSTSZTE_REQ:
          for (i = 0; i < GetRectStateInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetRectStateInfo[i],1);
            
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            RectNumber = 0;
            for(i=0; i<5; i++)     
            {
              Rectlosedtest = sRectInfo.sRAWRectParam[i].u8Rect_Sts;
              sRectInfo.sRAWRectParam[i].u8Rect_OnPosition = hex2byte(&RecvBuff[0],25+20*i); 
              if((sRectInfo.sRAWRectParam[i].u8Rect_OnPosition==0xE0)|(sRectInfo.sRAWRectParam[i].u8Rect_OnPosition==0xE1))
              {
                sRectInfo.sRAWRectParam[i].u8Rect_Sts = hex2byte(&RecvBuff[0],17+20*i); 
                
                RectNumber++;
              }
              else
                sRectInfo.sRAWRectParam[i].u8Rect_Sts =1;
              
              //                            if((Rectlosedtest ==1)&&(sRectInfo.sRAWRectParam[i].u8Rect_Sts==0))
              //                            {
              //                                Rectlosedtest =0;
              //                                sRectInfo.sRAWRectParam[i].u8Rect_NoResp =1;
              //                            }
              //                            if((sRectInfo.sRAWRectParam[i].u8Rect_NoResp ==1)&&(sRectInfo.sRAWRectParam[i].u8Rect_Sts==1))
              //                                sRectInfo.sRAWRectParam[i].u8Rect_NoResp =0;
            }
            sRectInfo.u8Rect_Num =RectNumber;
          }
          
          RecvCntStart = 0;
          MESGState = RECTINFOZTE_REQ;
          break;
        case  RECTINFOZTE_REQ:
          for (i = 0; i < GetRectAnalogInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetRectAnalogInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            sRectInfo.fAllRectDcVolt = hex2float(&RecvBuff[0],15);
            val =     hex2byte(&RecvBuff[0],23);
            for(i=0; i<val; i++)
            {
              //                          sRectInfo.sRAWRectParam[i].u8Rect_Sts=1;
              sRectInfo.sRAWRectParam[i].fRect_DcVolt = hex2float(&RecvBuff[0],43+i*66);
              sRectInfo.sRAWRectParam[i].fRect_DcCurr = hex2float(&RecvBuff[0],25+i*66);
              sRectInfo.sRAWRectParam[i].fRect_Temp = hex2float(&RecvBuff[0],59+i*66);
              
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = RECTALARM_REQ;
          break;
        case  RECTALARM_REQ:
          for (i = 0; i < GetRectAlarmInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetRectAlarmInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            val =        hex2byte(&RecvBuff[0],15);
            for(i=0; i<val; i++)
            {
              //                            sRectInfo.sRAWRectParam[i].u8Rect_Sts=1;
              sRectInfo.sRAWRectParam[i].u8Rect_Fail = hex2byte(&RecvBuff[0],i*20+17); // 00H normal, 01H Fail
              sRectInfo.sRAWRectParam[i].u8Rect_Out = hex2byte(&RecvBuff[0],i*20+35); // 00H normal, 01H Fail
              sRectInfo.sRAWRectParam[i].u8Rect_NoResp = hex2byte(&RecvBuff[0],i*20+27);
              if(sRectInfo.sRAWRectParam[i].u8Rect_NoResp == 0) sRectInfo.sRAWRectParam[i].u8Rect_NoResp = 0;
              else sRectInfo.sRAWRectParam[i].u8Rect_NoResp =1;
              
            }
          }
          
          
          RecvCntStart = 0;
          MESGState = DCOUTPUT_REQ;
          break;
          //====================================== DCINFO MESSAGE ======================================//
          
        case  DCOUTPUT_REQ:
          for (i = 0; i < GetDCAnalogInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetDCAnalogInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            sDcInfo.sINFODcInfo.fVoltage = hex2float(&RecvBuff[0],17);
            sDcInfo.sINFODcInfo.fCurrent = hex2float(&RecvBuff[0],25);
            //                        sDcInfo.sINFODcInfo.u8BatNo = hex2byte(&RecvBuff[0],33);
            sDcInfo.sINFODcInfo.fBatt1Curr = hex2float(&RecvBuff[0],35);
            sDcInfo.sINFODcInfo.fBatt2Curr = hex2float(&RecvBuff[0],43);
            sDcInfo.sINFODcInfo.fBatt1Volt = hex2float(&RecvBuff[0],55);
            sDcInfo.sINFODcInfo.fBatt2Volt = hex2float(&RecvBuff[0],63);                     
            sDcInfo.sINFODcInfo.fSen1BattTemp = hex2float(&RecvBuff[0],87);
            sDcInfo.sINFODcInfo.fSen2BattTemp = hex2float(&RecvBuff[0],95);
            sDcInfo.sINFODcInfo.fBatt1RmnCap = hex2float(&RecvBuff[0],71);
            sDcInfo.sINFODcInfo.fBatt2RmnCap = hex2float(&RecvBuff[0],79);
          }
          
          RecvCntStart = 0;
          MESGState = DCPARAMETER_REQ;
          break;
          
        case  DCPARAMETER_REQ:
          for (i = 0; i < GetDCParameterInfoLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetDCParameterInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            sBattInfo.sRAWBattInfo.fDCOverCfg = roundf(hex2float(&RecvBuff[0],13)*10)/10;
            sBattInfo.sRAWBattInfo.fFltVoltCfg = roundf(hex2float(&RecvBuff[0],127)*10)/10;
            sBattInfo.sRAWBattInfo.fBotVoltCfg = roundf(hex2float(&RecvBuff[0],135)*10)/10;
            sBattInfo.sRAWBattInfo.fTestVoltCfg = roundf(hex2float(&RecvBuff[0],191)*10)/10;
            sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],21)*10)/10;
            sBattInfo.sRAWBattInfo.fLVDDV = roundf(hex2float(&RecvBuff[0],183)*10)/10;
            sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg = roundf(hex2float(&RecvBuff[0],143)*10)/10;
            sBattInfo.sRAWBattInfo.fCapCCLVal= roundf(hex2float(&RecvBuff[0],159)*1000)/1000;
            sBattInfo.sRAWBattInfo.fTempCompVal = hex2float(&RecvBuff[0],175);
            sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel = roundf(hex2float(&RecvBuff[0],79)*10)/10;
            sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel = roundf(hex2float(&RecvBuff[0],87)*10)/10;
            sBattInfo.sRAWBattInfo.fCapTotal = hex2float(&RecvBuff[0],151);
            sBattInfo.sRAWBattInfo.fCCLVal = sBattInfo.sRAWBattInfo.fCapCCLVal/ sBattInfo.sRAWBattInfo.fCapTotal;
            sBattTestInfo.fBattTestDur = hex2float(&RecvBuff[0],199);
            sBattTestInfo.fTestEndCap =  hex2float(&RecvBuff[0],207);
            sDcInfo.sINFODcInfo.u8BatNo = (uint8_t)hex2float(&RecvBuff[0],215);
          }
          
          RecvCntStart = 0;
          MESGState = DCALARM_REQ;
          //                        MESGState = DCPARAMETER_REQ;
          break;
        case  DCALARM_REQ:
          for (i = 0; i < GetDCACAlarmInfoLen; i++)
          {
            LPUART_WriteBlocking(LPUART5,&GetDCAlarmInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            sDcInfo.sALARMDcInfo.u8DC = hex2byte(&RecvBuff[0],17);
            sDcInfo.sALARMDcInfo.u8DC_NoFuse = hex2byte(&RecvBuff[0],19); // So switch trong tu
            val = sDcInfo.sALARMDcInfo.u8DC_NoFuse;
            for (i = 0; i < val; i++)
            {
              sDcInfo.sALARMDcInfo.u8LoadFuse[i] = hex2byte(&RecvBuff[0],9 + 2*i+12);
              if((i<10)&&(sDcInfo.sALARMDcInfo.u8LoadFuse[i] == 0x03))private_mib_base_struct.loadGroup.loadStatus[i]=1;
            }
            for (i = 0; i < 4; i++)
            {
              sDcInfo.sALARMDcInfo.u8BattFuse[i] = hex2byte(&RecvBuff[0],43+i*2);
              if(sDcInfo.sALARMDcInfo.u8BattFuse[i] == 0x03)
                private_mib_base_struct.batteryGroup.battBrkStatus[i] = 1;
            }
            
            
            
            sDcInfo.sALARMDcInfo.u8LLVD = hex2byte(&RecvBuff[0],59);
            //sDcInfo.sALARMDcInfo.u8BLVD = hex2byte(&RecvBuff[0],61);
            sDcInfo.sALARMDcInfo.u8BLVD = hex2byte(&RecvBuff[0],55);//thanhcm3 fix
            sDcInfo.sALARMDcInfo.u8Sen1_BattTemp = hex2byte(&RecvBuff[0],63);  // Canh bao nhiet do acquy1 cao
            //                        sDcInfo.sALARMDcInfo.u8BatVol[0] = hex2byte(&RecvBuff[0],(val+8)*2+13);   // Canh bao bat 1 low voltage
            //                        sDcInfo.sALARMDcInfo.u8Sen1_BattInvalid = hex2byte(&RecvBuff[0],(val+11)*2+13); // Canh bao batt temp invalid
            //                        sDcInfo.sALARMDcInfo.u8Sen2_BattTemp = hex2byte(&RecvBuff[0],(val+14)*2+13); // Canh bao nhiet do acquy2 cao
            //                        sDcInfo.sALARMDcInfo.u8BatVol[1] = hex2byte(&RecvBuff[0],(val+13)*2+13);   // Canh bao bat 2 low voltage
            //                        sDcInfo.sALARMDcInfo.u8Sen2_BattInvalid = hex2byte(&RecvBuff[0],(val+16)*2+13); // Canh bao batt temp invalid
            //                        sDcInfo.sALARMDcInfo.u8Sen3_BattTemp = hex2byte(&RecvBuff[0],(val+19)*2+13); // Canh bao nhiet do acquy3 cao
            //                        sDcInfo.sALARMDcInfo.u8Sen3_BattInvalid = hex2byte(&RecvBuff[0],(val+21)*2+13); // Canh bao batt temp invalid
            for (i = 0; i < 8; i++)
            {
              sDcInfo.sALARMDcInfo.u8DIFuse[i] = hex2byte(&RecvBuff[0],89+i*2);
            }
            sDcInfo.sALARMDcInfo.u8DCSPD = hex2byte(&RecvBuff[0],215);
            //                                             
            //                        sDcInfo.sALARMDcInfo.u8Batt_Discharge = hex2byte(&RecvBuff[0],(val+26)*2+13);								
            
          }
          
          
          RecvCntStart = 0;
          MESGState = TIMEINFO_REQ;                          
          break;
        case  TIMEINFO_REQ:
          for (i = 0; i < GetEMERTimeInfoLen; i++)
          {
            
            LPUART_WriteBlocking(LPUART5,&GetEMERTimeInfo[i],1);
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
            g_testchecksum = hex2byte2(&RecvBuff[0],RecvCount-5);
            g_checksum = Checksum16(&RecvBuff[0],RecvCount);
          }
          if (g_checksum == g_testchecksum)
          {
            // nhan thong so thoi gian
            if(( RecvBuff[7]=0x34)&&( RecvBuff[8]=0x44))      // ko sua
            {
              EMERV21Time.year= hex2byte2(&RecvBuff[0],13);
              EMERV21Time.month = hex2byte(&RecvBuff[0],17);
              EMERV21Time.date = hex2byte(&RecvBuff[0],19);
              EMERV21Time.hour = hex2byte(&RecvBuff[0],21);
              EMERV21Time.min = hex2byte(&RecvBuff[0],23);
              EMERV21Time.sec = hex2byte(&RecvBuff[0],25);
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
            //			private_mib_base_struct.batteryGroup.battTemp = sDcInfo.sINFODcInfo.fSen2BattTemp*10;
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
          
          if (private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open != 0)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 1;
          }
          else
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 0;
          }
          if((sAcInfo.u8MainFail==0x05)||(sAcInfo.u8PowerOff==0xE1))
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
          if(sDcInfo.sALARMDcInfo.u8LLVD==0x00)
            private_mib_base_struct.main_alarm_group_struct.alarm_llvd = 0;
          else private_mib_base_struct.main_alarm_group_struct.alarm_llvd = 1;
          //						private_mib_base_struct.main_alarm_group_struct.alarm_llvd = sDcInfo.sALARMDcInfo.u8LLVD;
          if(sDcInfo.sALARMDcInfo.u8BLVD == 0x00)private_mib_base_struct.main_alarm_group_struct.alarm_blvd = 0;
          else private_mib_base_struct.main_alarm_group_struct.alarm_blvd = 1;
          //						private_mib_base_struct.main_alarm_group_struct.alarmBLVD = sDcInfo.sALARMDcInfo.u8BLVD;
          if(sDcInfo.sALARMDcInfo.u8DC==1)
            private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 0x01;
          else private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 0x00;
          private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0;
          if(sDcInfo.sALARMDcInfo.u8Sen1_BattTemp == 0x02)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp = 0x02;
            private_mib_base_struct.batteryGroup.battTemp = (uint32_t) (sDcInfo.sINFODcInfo.fSen1BattTemp * 10);
          }
          
          
          // ======================== Update dc information to SNMP resource ==========================//
          // ======================== Update rect information to SNMP resource ==========================//
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
          
          //                    sRectInfo.sSNMPRectParam[0].u32Rect_State = 0;
          for(i=0; i<6; i++)  
          {
            private_mib_base_struct.rectGroup.rectTable[i].rectStatus = 0;
            private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp |= sRectInfo.sRAWRectParam[i].u8Rect_NoResp << i;    
            private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= sRectInfo.sRAWRectParam[i].u8Rect_Fail << i;    
            //							sRectInfo.sSNMPRectParam[i].u32Rect_Fail = sRectInfo.sRAWRectParam[i].u8Rect_Fail;
            //                        sRectInfo.sSNMPRectParam[i].u32Rect_Out = sRectInfo.sRAWRectParam[i].u8Rect_Out;
            if(sRectInfo.sRAWRectParam[i].u8Rect_Sts==0)
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
            if(sRectInfo.sRAWRectParam[i].u8Rect_Sts==0)
            {
              //                           private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =(uint32_t) (sRectInfo.fAllRectDcVolt * 100);
              private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =(uint32_t) (sRectInfo.sRAWRectParam[i].fRect_DcVolt * 100);
              private_mib_base_struct.rectGroup.rectTable[i].rectTemp =(uint32_t) (sRectInfo.sRAWRectParam[i].fRect_Temp * 100);
            }
            
            else
            {
              private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =0;
              private_mib_base_struct.rectGroup.rectTable[i].rectTemp =0;
            }
            //
          }
          if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
          if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
          private_mib_base_struct.rectGroup.rectTotalCurrent = 0;
          for (i = 0; i < 6; i++)     
          {
            private_mib_base_struct.rectGroup.rectTotalCurrent += (uint32_t) (private_mib_base_struct.rectGroup.rectTable[i].rectOutputCurrent);
          }
          private_mib_base_struct.rectGroup.rectInstalledRect = 5;    
          private_mib_base_struct.rectGroup.rectActiveRect = sRectInfo.u8Rect_Num; 
          
          private_mib_base_struct.loadGroup.loadCurrent = (uint32_t) (sDcInfo.sINFODcInfo.fCurrent*100);
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
          
          private_mib_base_struct.cfgBTGroup.cfgBTEndTime = (uint32_t) (sBattTestInfo.fBattTestDur);
          private_mib_base_struct.cfgBTGroup.cfgBTEndCap = (uint32_t) (sBattTestInfo.fTestEndCap * 1000);
          private_mib_base_struct.cfgBTGroup.cfgTestVoltCfg = (uint32_t)(sBattInfo.sRAWBattInfo.fTestVoltCfg*10);
          //                    private_mib_base_struct.cfgBTGroup.cfgAutoTestDay = sBattInfo.sRAWBattInfo.fAutoTestDay;
          //                    private_mib_base_struct.cfgBTGroup.cfgTestStartTime = sBattInfo.sRAWBattInfo.fTestStartTime;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal2 = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal2;
          private_mib_base_struct.configGroup.cfgCurrentLimit = (uint32_t) ( sBattInfo.sRAWBattInfo.fCCLVal * 1000);
          private_mib_base_struct.configGroup.cfgDCOverVolt = (uint32_t)(sBattInfo.sRAWBattInfo.fDCOverCfg * 100);                    
          
          //                    sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel = 100;
          private_mib_base_struct.configGroup.cfgOverMajorTempLevel = (uint32_t) (sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel * 100);
          private_mib_base_struct.configGroup.cfgHighMajorTempLevel = (uint32_t) (sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel *100);
          private_mib_base_struct.configGroup.cfgAcLowLevel = (uint32_t) sAcInfo.fAcLowThres;
          private_mib_base_struct.configGroup.cfgAcHighLevel = (uint32_t) sAcInfo.fAcHighThres;
          
          // ======================== Update Config information to SNMP resource ==========================//
          // ======================== Update Env information to SNMP resource ==========================//
          private_mib_base_struct.siteGroup.siteAmbientTemp = (uint32_t) (sDcInfo.sINFODcInfo.fSen2BattTemp*10);
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
          sAlarmInfo.u32EnvDoor = 0;
          if(sAlarmEnvInfo.u8EnvDoor==0x04)
          {
            sAlarmInfo.u32EnvDoor = 0x01;
          }
          sAlarmInfo.u32HeatExch = 0;
          if(sAlarmEnvInfo.u8HeatExch==0x04)
          {
            sAlarmInfo.u32HeatExch = 0x01;
          }
          
          
          // ======================== Update alarm hitory ==========================//                    
          //                    if((t1 ==1)&&(t2==0))
          //                     {
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =9;
          //                          NextHis();
          //                          for(k=0;k<10;k++){NextHis();}
          //
          //
          //     
          //                     }
          //                    if((t1 ==0)&&(t2==1))
          //                    {     
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =10;
          //                          NextHis();
          //                          for(k=0;k<20;k++){NextHis();}
          //                    }
          //                     if((t3 ==1)&&(t4==0))
          //                     {
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =11;
          //                          NextHis();
          //                          for(k=0;k<170;k++){NextHis();}
          //                     }
          //                     if((t3 ==0)&&(t4==1))
          //                     { 
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =12;
          //                          NextHis();
          // 
          //                     }
          //                     
          //                     if((t5 ==1)&&(t6==0))
          //                     {    
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =13;
          //                          NextHis();
          //                     }  
          //                     if((t5==0)&&(t6==1))
          //                     {
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =14;
          //                          NextHis();
          //
          //                     }
          //                     if((t7 ==1)&&(t8==0))
          //                     {    
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =15;
          //                          NextHis();
          //
          //                     }  
          //                     if((t7==0)&&(t8==1))
          //                     {
          //                          sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =16;
          //                          NextHis();
          //
          //                     }
          //                    
          //                    t2=t1;
          //                    t4=t3;
          //                    t6=t5;
          //                    t8=t7;
          
          if((private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmACmains_old==0))
          {
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =1;
            NextHis();
            
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains ==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmACmains_old==1))
          {     
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =2;
            NextHis();
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_ac_low ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmACLow_old==0))
          {
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =3;
            NextHis();
            
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_ac_low ==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmACLow_old==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =4;
            NextHis();
            
          }
          
          if((private_mib_base_struct.main_alarm_group_struct.alarmACSPD ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmACSPD_old==0))
          {    
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =5;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarmACSPD==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmACSPD_old==1))
          {
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =6;
            NextHis();
            
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmDCLow_old==0))
          {    
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =7;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmDCLow_old==1))
          {
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =8;
            NextHis();
            
          }
          
          if((private_mib_base_struct.main_alarm_group_struct.alarmDCSPD ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmDCSPD_old==0))
          {    
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =9;
            NextHis();
            
          } 
          if((private_mib_base_struct.main_alarm_group_struct.alarmDCSPD==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmDCSPD_old==1))
          {
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =10;
            NextHis();
            
          }
          
          if((private_mib_base_struct.main_alarm_group_struct.alarm_llvd ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmLLVD_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =11;
            NextHis();
            
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_llvd==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmLLVD_old==1))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =12;
            NextHis();
            
          }
          
          if((private_mib_base_struct.main_alarm_group_struct.alarm_blvd ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmBLVD_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =13;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_blvd==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmBLVD_old==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =14;
            NextHis();
            
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmBattBreakerOpen_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =15;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmBattBreakerOpen_old==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =16;
            NextHis();
            
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp ==2)&&(private_mib_base_struct.main_alarm_group_struct.alarmBattHighTemp_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =17;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmBattHighTemp_old==2))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =18;
            NextHis();
            
          }
          
          if((private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmRectACFault_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =19;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmRectACFault_old==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =20;
            NextHis();
            
          }
          if((private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmRectNoResp_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =21;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmRectNoResp_old==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =22;
            NextHis();
            
          }
          
          if((private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open ==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmLoadBreakerOpen_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =23;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmLoadBreakerOpen_old==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =24;
            NextHis();
            
          }
          
          if((private_mib_base_struct.main_alarm_group_struct.alarm_smoke==1)&&(private_mib_base_struct.main_alarm_group_struct.alarmSmoke_old==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =25;
            NextHis();
            
          }  
          if((private_mib_base_struct.main_alarm_group_struct.alarm_smoke==0)&&(private_mib_base_struct.main_alarm_group_struct.alarmSmoke_old==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =26;
            NextHis();
            
          }
          
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[0]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =31;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[0]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[0]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =32;
            NextHis();
            
          }
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[1]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[1]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =33;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[1]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[1]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =34;
            NextHis();
            
          }
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[2]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[2]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =35;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[2]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[2]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =36;
            NextHis();
            
          }
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[3]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[3]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =37;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[3]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[3]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =38;
            NextHis();
            
          }
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[4]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[4]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =39;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[4]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[4]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =40;
            NextHis();
            
          }
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[5]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[5]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =41;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[5]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[5]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =42;
            NextHis();
            
          }
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[6]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[6]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =43;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[6]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[6]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =44;
            NextHis();
            
          }
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[7]==1)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[7]==0))
          {   
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =45;
            NextHis();
            
          }  
          if((private_mib_base_struct.diAlarmGroup.alarmDigitalInput[7]==0)&&(private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[7]==1))
          { 
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =46;
            NextHis();
            
          }
          if((private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old ==0)&& (private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect !=0))
          {
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =29;
            NextHis();
          }
          if((private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old !=0)&& (private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect ==0))
          {
            sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =30;
            NextHis();                          
          }
          
          private_mib_base_struct.main_alarm_group_struct.alarmACmains_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains;
          private_mib_base_struct.main_alarm_group_struct.alarmACLow_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_ac_low;
          private_mib_base_struct.main_alarm_group_struct.alarmACSPD_old  = private_mib_base_struct.main_alarm_group_struct.alarmACSPD;                                      
          private_mib_base_struct.main_alarm_group_struct.alarmDCLow_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low;
          private_mib_base_struct.main_alarm_group_struct.alarmDCSPD_old = private_mib_base_struct.main_alarm_group_struct.alarmDCSPD;
          private_mib_base_struct.main_alarm_group_struct.alarmLLVD_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_llvd;
          private_mib_base_struct.main_alarm_group_struct.alarmBLVD_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_blvd;
          private_mib_base_struct.main_alarm_group_struct.alarmBattBreakerOpen_old =  private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open;
          private_mib_base_struct.main_alarm_group_struct.alarmBattHighTemp_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_batt_high_temp;
          private_mib_base_struct.main_alarm_group_struct.alarmRectACFault_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault;
          private_mib_base_struct.main_alarm_group_struct.alarmRectNoResp_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp;
          private_mib_base_struct.main_alarm_group_struct.alarmLoadBreakerOpen_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open;
          private_mib_base_struct.main_alarm_group_struct.alarmSmoke_old  =  private_mib_base_struct.main_alarm_group_struct.alarm_smoke;
          private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old = private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect;
          for(i=0;i<8;i++)
          {
            private_mib_base_struct.diAlarmGroup.alarmDigitalInput_old[i]=  private_mib_base_struct.diAlarmGroup.alarmDigitalInput[i];
          }
          // ======================== Update alarm hitory ==========================//                   
          
          // ======================== Update Env information to SNMP resource ==========================//
          
          MESGState = WRITE_FLASH;
          break;
        case WRITE_FLASH:
          {if(writeFlash>10)
          {
            writeFlash=0;
            for(i=0;i<5;i++)
            {
              
              if(sHisFlashLog[i].Count_old != sHisFlashLog[i].Count)
              {  
                pHisFlashLog =    &sHisFlashLog[i];
                sHisFlashLog[i].Count_old = sHisFlashLog[i].Count;
                switch(i)
                {
                case 0:
                  sFLASH_EraseSector(FLASH_PB_LOG_START);
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB_LOG_START, 256);
                  break;
                case 1:
                  sFLASH_EraseSector(FLASH_PB1_LOG_START);
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB1_LOG_START, 256);
                  break;
                case 2:
                  sFLASH_EraseSector(FLASH_PB2_LOG_START);
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB2_LOG_START, 256);
                  break;
                case 3:
                  sFLASH_EraseSector(FLASH_PB3_LOG_START);
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB3_LOG_START, 256);
                  break;
                case 4:
                  sFLASH_EraseSector(FLASH_PB4_LOG_START);
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB4_LOG_START, 256);
                  break;
                default:
                  break;
                }
                
                
              }
            }
            
          }
          if( writeFlash==5)
          {
            //              sFLASH_ReadBuffer((uint8_t *)&u8HisBuffer[0],FLASH_PB_LOG_START, 256);
            //                 sFLASH_EraseSector(FLASH_PB_LOG_START);
            //                 sFLASH_EraseSector(FLASH_PB1_LOG_START);
            //                 sFLASH_EraseSector(FLASH_PB2_LOG_START);
            //                 sFLASH_EraseSector(FLASH_PB3_LOG_START);
            //                 sFLASH_EraseSector(FLASH_PB4_LOG_START);
          }
          writeFlash++;                  
          MESGState = ACINPUTINFO_REQ;
          break;
          }
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
        g_testchecksum = 0xFFFF;
        g_checksum = 0;
      }
      
    }
    vTaskDelay(450);
  }
  while (1);
}