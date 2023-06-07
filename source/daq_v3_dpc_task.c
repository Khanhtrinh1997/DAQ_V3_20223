//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
void dpc_task(void* pv_parameters)
{
  private_mib_base_struct.cntGroup.task.dpc = 0;
  dpc_t.check_sum_calculator = 0;
  dpc_t.check_sum_read       = 0xFFFF;  
  MESGState                  = ACINPUTINFO_REQ;    
  while(1){
    //===========================================================
    //code here
    private_mib_base_struct.cntGroup.task.dpc++;
  
    //CMD set -----------------------------------------------------------
    if(setCmd_flag == 1){
      settingCommand |= setCmd_mask;
      
      switch(settingCommand){
        //---------------------------------------------------------------
        case SET_FLTVOL:
          DKD51_set_float_volt();
          break;
        //---------------------------------------------------------------
        case SET_BSTVOL:
          DKD51_set_boost_volt();
          break;
        //---------------------------------------------------------------
        case SET_TEMPCOMP_VAL:
          DKD51_set_temp_compensation();
          break;
        //---------------------------------------------------------------
        case SET_LLVD://space
          break;
        //---------------------------------------------------------------
        case SET_BLVD://space
          break;
        //---------------------------------------------------------------
        case SET_DCOVER:
          DKD51_set_dc_over();
          break;
        //---------------------------------------------------------------
        case SET_DCLOW:
          DKD51_set_dc_low();
          break;
        //---------------------------------------------------------------
        case SET_BATTSTDCAP:
          DKD51_set_batt_capacity_1();
          break;
        //---------------------------------------------------------------
        case SET_BATTSTDCAP2:
          DKD51_set_batt_capacity_2();
          break;
        //---------------------------------------------------------------
        case SET_BATTSTDCAP3:
          DKD51_set_batt_capacity_3();
          break;
        //---------------------------------------------------------------
        case SET_BATTSTDCAP4:
          DKD51_set_batt_capacity_4();
          break;
        //---------------------------------------------------------------
        case SET_CCL:
          DKD51_set_charge_current_limit();
          break;
        //---------------------------------------------------------------
        case SET_OVERTEMP://space
          break;
        //---------------------------------------------------------------
        case SET_AC_LOW_THRES:
          DKD51_set_ac_low();
          break;
        //---------------------------------------------------------------
        case SET_AC_HIGH_THRES:
          DKD51_set_ac_high();
          break;
        //---------------------------------------------------------------
        case SET_AC_INPUT_CURR_LIMIT://space
          break;
        //--------------------------------------------------------------
        default:
          break; 
      }
      setCmd_mask = 0;
      setCmd_flag = 0;
      settingCommand = 0;
    }else if(setCmd_flag == 0){
      //check disconnect-------------------------------------------------
      if(PMUConnectCount++>30){
        PMUConnectCount =50;
        private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 1;
      }
//      //thanhcm3 test====================================================
//      //1.------------------------------------------
//      if(flag_test == 20){
//        flag_test =0;
//        DKD51_clearn_buff(&dpc_t);
//      }
//      //1.------------------------------------------
//      if(flag_test==1){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_ac_input_info[0],(uint8_t)DKD51_REQ_ac_input_info_len);
//      }
//      //2.------------------------------------------
//      if(flag_test==2){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_ac_parameter[0],(uint8_t)DKD51_REQ_ac_parameter_len);
//      }
//      //3.-----------------------------------------
//      if(flag_test==3){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_ac_alarm[0],(uint8_t)DKD51_REQ_ac_alarm_len);
//      }
//      //4.----------------------------------------
//      if(flag_test==4){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_rect_sts[0],(uint8_t)DKD51_REQ_rect_sts_len);
//      }
//      //5.----------------------------------------
//      if(flag_test==5){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_rect_info[0],(uint8_t)DKD51_REQ_rect_info_len);
//      }
//      //6.-----------------------------------------
//      if(flag_test==6){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_rect_alarm[0],(uint8_t)DKD51_REQ_rect_alarm_len);
//      }
//      //7.-----------------------------------------
//      if(flag_test==7){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_dc_output[0],(uint8_t)DKD51_REQ_dc_output_len);
//      }
//      //8.-----------------------------------------
//      if(flag_test==8){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_dc_parameter[0],(uint8_t)DKD51_REQ_dc_parameter_len);
//      }
//      //9.-----------------------------------------
//      if(flag_test==9){
//        DKD51_clearn_buff(&dpc_t);
//        flag_test =0;
//        DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_dc_alarm[0],(uint8_t)DKD51_REQ_dc_alarm_len);
//      }
//      
//      //=================================================================
      //question DKD51---------------------------------------------------------------------------------------------
      switch(MESGState){
        // AC MESSAGE=====================================================================================
        case ACINPUTINFO_REQ://ac info request---------------------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_ac_input_info[0],(uint8_t)DKD51_REQ_ac_input_info_len);
          MESGState = ACINPUTINFO_RES;
          break;
          
        case ACINPUTINFO_RES://ac info response--------------------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            sDcInfo.u32DCNoResponse      = 0;
            private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 0;
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sAcInfo.facVolt[0]           = ((float)hex2byte2(&dpc_t.rx.buff_485[0],19))/100;
              sAcInfo.facVolt[1]           = ((float)hex2byte2(&dpc_t.rx.buff_485[0],23))/100;
              sAcInfo.facVolt[2]           = ((float)hex2byte2(&dpc_t.rx.buff_485[0],27))/100;
              sAcInfo.f_hz_in              = ((float)hex2byte2(&dpc_t.rx.buff_485[0],31))/100;
              sAcInfo.f_ac_curr[0]         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],79))/100;
              sAcInfo.f_ac_curr[1]         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],83))/100;
              sAcInfo.f_ac_curr[2]         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],87))/100;    
              //flag_test = 10;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            sDcInfo.u32DCNoResponse++;
            if(sDcInfo.u32DCNoResponse > 3){
              private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 1;
              sDcInfo.u32DCNoResponse = 10;
                      // xoa khi checksum sai
                      sAcInfo.facVolt[0] = 0;
                      sAcInfo.facVolt[1] = 0;
                      sAcInfo.facVolt[2] = 0;
                      sAcInfo.f_hz_in    = 0;
                      sAcInfo.f_ac_curr[0]= 0;
                      sAcInfo.f_ac_curr[1]= 0;
                      sAcInfo.f_ac_curr[2]= 0;
                      sAcInfo.fAcHighThres = 0;
                      sAcInfo.fAcLowThres = 0;
                      sAcInfo.fAcInputCurrLimit = 0;
                      sRectInfo.fAllRectDcVolt  = 0;
                      sAcInfo.f_ac_high_hz      = 0;  
                      sAcInfo.f_ac_low_hz       = 0;
                      for(uint8_t i=0; i<24; i++)
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
          dpc_t.uart_rx.cnt =0;
          MESGState = ACPARAMETER_REQ;
          break;
          
        case ACPARAMETER_REQ://ac parametter request------------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_ac_parameter[0],(uint8_t)DKD51_REQ_ac_parameter_len);
          MESGState = ACPARAMETER_RES;
          break;
          
        case ACPARAMETER_RES://ac parametter response------------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sAcInfo.fAcHighThres         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],13))/100;
              sAcInfo.fAcLowThres          = ((float)hex2byte2(&dpc_t.rx.buff_485[0],17))/100;
              sAcInfo.fAcInputCurrLimit    = ((float)hex2byte2(&dpc_t.rx.buff_485[0],21))/100;
              sAcInfo.f_ac_high_hz         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],25))/100;
              sAcInfo.f_ac_low_hz          = ((float)hex2byte2(&dpc_t.rx.buff_485[0],29))/100;
              //flag_test = 11;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = ACALARM_REQ;
          break;
          
        case ACALARM_REQ://ac alarm request--------------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_ac_alarm[0],(uint8_t)DKD51_REQ_ac_alarm_len);
          MESGState = ACALARM_RES;
          break;
          
        case ACALARM_RES://ac alarm response-------------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sAcInfo.u8Thres[0]           = hex2byte(&dpc_t.rx.buff_485[0],19);
              sAcInfo.u8Thres[1]           = hex2byte(&dpc_t.rx.buff_485[0],21);
              sAcInfo.u8Thres[2]           = hex2byte(&dpc_t.rx.buff_485[0],23);
              sAcInfo.u8_hz                = hex2byte(&dpc_t.rx.buff_485[0],25);  
              sAcInfo.u8_I_Thres[0]        = hex2byte(&dpc_t.rx.buff_485[0],55);
              sAcInfo.u8_I_Thres[1]        = hex2byte(&dpc_t.rx.buff_485[0],57);
              sAcInfo.u8_I_Thres[2]        = hex2byte(&dpc_t.rx.buff_485[0],59);
              sAcInfo.u8MainFail           = hex2byte(&dpc_t.rx.buff_485[0],31);
              sAcInfo.u8ACSPD              = hex2byte(&dpc_t.rx.buff_485[0],33);
              //flag_test = 12;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = RECTSTSZTE_REQ;
          break;
          
        //RECT MESSAGE==================================================================================
        case RECTSTSZTE_REQ://rect status request--------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_rect_sts[0],(uint8_t)DKD51_REQ_rect_sts_len);
          MESGState = RECTSTSZTE_RES;
          break;
          
        case RECTSTSZTE_RES://rect status response--------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              dpc_t.rect_number =0;
              for (uint8_t i=0; i<6;i++){
                sRectInfo.sRAWRectParam[i].u8Rect_OnPosition = hex2byte(&dpc_t.rx.buff_485[0],25+12*i);
                if(sRectInfo.sRAWRectParam[i].u8Rect_OnPosition == 0x00){  // 0x00H: rect installed; 0x80H: rect uninstalled
                  sRectInfo.sRAWRectParam[i].u8Rect_Sts = hex2byte(&dpc_t.rx.buff_485[0],17+12*i); // 0x00H: rect on; 0x01H: rect off
                  sRectInfo.sRAWRectParam[i].rect_sts_current_limit = hex2byte(&dpc_t.rx.buff_485[0],19+12*i);//0x00H:current limit; 0x01H: no current limit
                  sRectInfo.sRAWRectParam[i].rect_sts_fl_eq_testing = hex2byte(&dpc_t.rx.buff_485[0],21+12*i);//0x00H:FL; 0x01H: EQ ;0x02H: TESTING;
                  dpc_t.rect_number++;
                }
                else{
                  sRectInfo.sRAWRectParam[i].u8Rect_Sts =1;
                }
              }
               sRectInfo.u8Rect_Num = dpc_t.rect_number;
              //flag_test = 13;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = RECTINFOZTE_REQ;
          break;
          
        case RECTINFOZTE_REQ://rect info request----------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_rect_info[0],(uint8_t)DKD51_REQ_rect_info_len);
          MESGState = RECTINFOZTE_RES;
          break;
          
        case RECTINFOZTE_RES://rect info response----------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sRectInfo.fAllRectDcVolt = ((float)hex2byte2(&dpc_t.rx.buff_485[0],15))/100;
              dpc_t.var = hex2byte(&dpc_t.rx.buff_485[0],19);
              for(uint8_t i=0; i<dpc_t.var;i++){
                sRectInfo.sRAWRectParam[i].fRect_DcVolt = sRectInfo.fAllRectDcVolt; // dung tam DCall
                sRectInfo.sRAWRectParam[i].fRect_DcCurr = ((float)hex2byte2(&dpc_t.rx.buff_485[0],21+6*i))/100;
              }
              //flag_test = 14;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = RECTALARM_REQ;
          break;
          
        case RECTALARM_REQ://rect alarm request-------------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_rect_alarm[0],(uint8_t)DKD51_REQ_rect_alarm_len);
          MESGState = RECTALARM_RES;
          break;
          
        case RECTALARM_RES://rect alarm response-------------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              dpc_t.var = hex2byte(&dpc_t.rx.buff_485[0],15);
              for(uint8_t i=0; i<dpc_t.var;i++){
                sRectInfo.sRAWRectParam[i].u8Rect_Fail           = hex2byte(&dpc_t.rx.buff_485[0],17+18*i);//0x00H:OK; 0x01H:fault
                sRectInfo.sRAWRectParam[i].rect_dc_over_volt     = hex2byte(&dpc_t.rx.buff_485[0],21+18*i);//0x00H:OK; 0x80H:fault
                sRectInfo.sRAWRectParam[i].rect_ac_over_volt     = hex2byte(&dpc_t.rx.buff_485[0],23+18*i);//0x00H:OK; 0x81H:fault
                sRectInfo.sRAWRectParam[i].rect_fan_fault        = hex2byte(&dpc_t.rx.buff_485[0],25+18*i);//0x00H:OK; 0x82H:fault
                sRectInfo.sRAWRectParam[i].rect_over_temp        = hex2byte(&dpc_t.rx.buff_485[0],27+18*i);//0x00H:OK; 0x83H:fault
                sRectInfo.sRAWRectParam[i].rect_dc_over_curr     = hex2byte(&dpc_t.rx.buff_485[0],29+18*i);//0x00H:OK; 0x84H:fault
                sRectInfo.sRAWRectParam[i].u8Rect_NoResp         = hex2byte(&dpc_t.rx.buff_485[0],31+18*i);//0x00H:OK; 0x85H:fault
                if(sRectInfo.sRAWRectParam[i].u8Rect_NoResp == 0) sRectInfo.sRAWRectParam[i].u8Rect_NoResp = 0;
                else sRectInfo.sRAWRectParam[i].u8Rect_NoResp =1;
              }
              //flag_test = 15;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = DCOUTPUT_REQ;
          break;
          
        //DC MESSAGE===================================================================================
        case DCOUTPUT_REQ://dc ouput info request-------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_dc_output[0],(uint8_t)DKD51_REQ_dc_output_len);
          MESGState = DCOUTPUT_RES;
          break;
          
        case DCOUTPUT_RES://dc ouput info response------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sDcInfo.sINFODcInfo.fVoltage      = ((float)hex2byte2(&dpc_t.rx.buff_485[0],17))/100;
              sDcInfo.sINFODcInfo.fCurrent      = ((float)hex2byte2(&dpc_t.rx.buff_485[0],21))/100;
              
              sDcInfo.sINFODcInfo.fBatt1Curr    = ((float)iiihex2byte2(&dpc_t.rx.buff_485[0],27))/100;
              sDcInfo.sINFODcInfo.fBatt2Curr    = ((float)iiihex2byte2(&dpc_t.rx.buff_485[0],31))/100;
              sDcInfo.sINFODcInfo.fBatt3Curr    = ((float)iiihex2byte2(&dpc_t.rx.buff_485[0],35))/100;
              sDcInfo.sINFODcInfo.fBatt4Curr    = ((float)iiihex2byte2(&dpc_t.rx.buff_485[0],39))/100;
              
              sDcInfo.sINFODcInfo.fBatt1Volt    = ((float)hex2byte2(&dpc_t.rx.buff_485[0],207))/100;
              sDcInfo.sINFODcInfo.fBatt2Volt    = ((float)hex2byte2(&dpc_t.rx.buff_485[0],211))/100;
              sDcInfo.sINFODcInfo.fBatt3Volt    = ((float)hex2byte2(&dpc_t.rx.buff_485[0],215))/100;
              sDcInfo.sINFODcInfo.fBatt4Volt    = ((float)hex2byte2(&dpc_t.rx.buff_485[0],219))/100;
              
              sDcInfo.sINFODcInfo.fSen1BattTemp = ((float)hex2byte2(&dpc_t.rx.buff_485[0],151))/100;
              sDcInfo.sINFODcInfo.fSen2BattTemp = ((float)hex2byte2(&dpc_t.rx.buff_485[0],155))/100;
              sDcInfo.sINFODcInfo.fSen3BattTemp = ((float)hex2byte2(&dpc_t.rx.buff_485[0],159))/100;
              sDcInfo.sINFODcInfo.fSen4BattTemp = ((float)hex2byte2(&dpc_t.rx.buff_485[0],163))/100;
              
              sDcInfo.sINFODcInfo.fBatt1RmnCap  = ((float)hex2byte2(&dpc_t.rx.buff_485[0],131))/100;
              sDcInfo.sINFODcInfo.fBatt2RmnCap  = ((float)hex2byte2(&dpc_t.rx.buff_485[0],135))/100;
              sDcInfo.sINFODcInfo.fBatt3RmnCap  = ((float)hex2byte2(&dpc_t.rx.buff_485[0],139))/100;
              sDcInfo.sINFODcInfo.fBatt4RmnCap  = ((float)hex2byte2(&dpc_t.rx.buff_485[0],143))/100;
              
              
              //flag_test = 16;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = DCPARAMETER_REQ;
          break;
          
        case DCPARAMETER_REQ://dc parameter request-----------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_dc_parameter[0],(uint8_t)DKD51_REQ_dc_parameter_len);
          MESGState = DCPARAMETER_RES;
          break;
          
        case DCPARAMETER_RES://dc parameter response----------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              
              sBattInfo.sRAWBattInfo.fDCOverCfg         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],13))/100;
              sBattInfo.sRAWBattInfo.fFltVoltCfg        = ((float)hex2byte2(&dpc_t.rx.buff_485[0],31))/100;
              sBattInfo.sRAWBattInfo.fBotVoltCfg        = ((float)hex2byte2(&dpc_t.rx.buff_485[0],35))/100;
            //sBattInfo.sRAWBattInfo.fTestVoltCfg       = ((float)hex2byte2(&dpc_t.rx.buff_485[0],143))/100;
              sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg   = ((float)hex2byte2(&dpc_t.rx.buff_485[0],17))/100;
              sBattInfo.sRAWBattInfo.fCapCCLVal         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],23))/100;// current limit A
              sBattInfo.sRAWBattInfo.fTempCompVal       = ((float)hex2byte2(&dpc_t.rx.buff_485[0],51))/100;
              
              sBattInfo.sRAWBattInfo.fCapTotal          = ((float)hex2byte2(&dpc_t.rx.buff_485[0],59))/100;
              sBattInfo.sRAWBattInfo.fCapTotal2         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],63))/100;
              sBattInfo.sRAWBattInfo.fCapTotal3         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],67))/100;
              sBattInfo.sRAWBattInfo.fCapTotal4         = ((float)hex2byte2(&dpc_t.rx.buff_485[0],71))/100;
              sBattInfo.sRAWBattInfo.fCapTotal_all      = sBattInfo.sRAWBattInfo.fCapTotal+ sBattInfo.sRAWBattInfo.fCapTotal2
                                                          + sBattInfo.sRAWBattInfo.fCapTotal3+sBattInfo.sRAWBattInfo.fCapTotal4;
              //sBattInfo.sRAWBattInfo.fCCLVal = sBattInfo.sRAWBattInfo.fCapCCLVal/ sBattInfo.sRAWBattInfo.fCapTotal_all;// current limit C
              
              //flag_test = 17;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = DCALARM_REQ;
          break;
          
        case DCALARM_REQ://dc alarm request------------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_dc_alarm[0],(uint8_t)DKD51_REQ_dc_alarm_len);
          MESGState = DCALARM_RES;
          break;
          
        case DCALARM_RES://dc alarm response------------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sDcInfo.sALARMDcInfo.u8DC = hex2byte(&dpc_t.rx.buff_485[0],17); // 0x00H:OK; 0x01H: LOW; 0x02H: High;
              sDcInfo.sALARMDcInfo.u8DC_NoFuse = hex2byte(&dpc_t.rx.buff_485[0],19);
              dpc_t.var = sDcInfo.sALARMDcInfo.u8DC_NoFuse -2;
              for(uint_t i=0;i<dpc_t.var;i++){
                sDcInfo.sALARMDcInfo.u8LoadFuse[i] = hex2byte(&dpc_t.rx.buff_485[0],25+i*2);//0x03H:fulse off
                if((i<10)&&(sDcInfo.sALARMDcInfo.u8LoadFuse[i] == 0x03))private_mib_base_struct.loadGroup.loadStatus[i]=1;
              }
              sDcInfo.sALARMDcInfo.u8LLVD              = hex2byte(&dpc_t.rx.buff_485[0],187);//0x80
              sDcInfo.sALARMDcInfo.u8LLVD2             = hex2byte(&dpc_t.rx.buff_485[0],189);//0x81
              sDcInfo.sALARMDcInfo.batt_low_volt_alarm = hex2byte(&dpc_t.rx.buff_485[0],191);//0x82
              
              sDcInfo.sALARMDcInfo.u8BattFuse[0]       = hex2byte(&dpc_t.rx.buff_485[0],21); //0x03
              sDcInfo.sALARMDcInfo.u8BattFuse[1]       = hex2byte(&dpc_t.rx.buff_485[0],23); //0x03
              sDcInfo.sALARMDcInfo.u8BattFuse[2]       = hex2byte(&dpc_t.rx.buff_485[0],195);//0x03
              sDcInfo.sALARMDcInfo.u8BattFuse[3]       = hex2byte(&dpc_t.rx.buff_485[0],197);//0x03
              //flag_test = 18;
            }
          }else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
            
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = ENVDATA_REQ;
          break;
        //ENV MESSAGE==================================================================================
        case ENVDATA_REQ:// env data request------------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_env_data[0],(uint8_t)DKD51_REQ_env_data_len);
          MESGState = ENVDATA_RES;
          break;
        case ENVDATA_RES://env data response------------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sDcInfo.sINFODcInfo.fSen1AmbTemp = ((float)hex2byte2(&dpc_t.rx.buff_485[0],17))/100;
              sDcInfo.sINFODcInfo.fHumidity    = ((float)hex2byte2(&dpc_t.rx.buff_485[0],23))/100;
              sDcInfo.sINFODcInfo.fcustomized  = ((float)hex2byte2(&dpc_t.rx.buff_485[0],29))/100;
            } 
          }
          else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = ENVALARM_REQ;
          break;
        case ENVALARM_REQ://env alarm request----------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_env_alarm[0],(uint8_t)DKD51_REQ_env_alarm_len);
          MESGState = ENVALARM_RES;
          break;
        case ENVALARM_RES://env laarm response----------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              sAlarmEnvInfo.u8EnvTemp          = hex2byte(&dpc_t.rx.buff_485[0],15); //0x00H: OK;0x04H: ALARM;
              sAlarmEnvInfo.u8EnvHumidity      = hex2byte(&dpc_t.rx.buff_485[0],17); //0x00H: OK;0x04H: ALARM;
              sAlarmEnvInfo.u8EnvSmoke         = hex2byte(&dpc_t.rx.buff_485[0],19); //0x00H: OK;0x04H: ALARM;
              sAlarmEnvInfo.u8EnvWater         = hex2byte(&dpc_t.rx.buff_485[0],21); //0x00H: OK;0x04H: ALARM;
              sAlarmEnvInfo.u8EnvInfra         = hex2byte(&dpc_t.rx.buff_485[0],23); //0x00H: OK;0x04H: ALARM;
              sAlarmEnvInfo.u8EnvDoor          = hex2byte(&dpc_t.rx.buff_485[0],25); //0x00H: OK;0x04H: ALARM;
              sAlarmEnvInfo.u8EnvGlass         = hex2byte(&dpc_t.rx.buff_485[0],27); //0x00H: OK;0x04H: ALARM;
            } 
          }
          else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = TIMEINFO_REQ;
          break;
        //TIME MESSAGE=================================================================================
        case TIMEINFO_REQ://time info request-----------------------------------------------------------
          DKD51_send_message_resquest((uint8_t*)&DKD51_REQ_time_info[0],(uint8_t)DKD51_REQ_time_info_len);
          MESGState = TIMEINFO_RES;
          break;
          
        case TIMEINFO_RES://time info response---------------------------------------------------------
          if(DKD51_check_sum(&dpc_t) == NO_ERR){
            if(DKD51_check_RTN(&dpc_t) == NO_ERR){
              //code here
            } 
          }
          else{
            dpc_t.check_sum_calculator = 0;
            dpc_t.check_sum_read       = 0xFFFF;
          }
          dpc_t.uart_rx.cnt =0;
          MESGState = UPDATE_OK;
          break;
        //UPDATE data SNMP===================================================================================
        case UPDATE_OK:
          //batt======================
          if(sDcInfo.sINFODcInfo.fBatt1Volt >30){
            private_mib_base_struct.batteryGroup.battVolt = (int32_t) (sDcInfo.sINFODcInfo.fBatt1Volt * 100);
          }else if(sDcInfo.sINFODcInfo.fBatt2Volt>30){
            private_mib_base_struct.batteryGroup.battVolt = (int32_t) (sDcInfo.sINFODcInfo.fBatt2Volt * 100);
          }else if(sDcInfo.sINFODcInfo.fBatt3Volt>30){
            private_mib_base_struct.batteryGroup.battVolt = (int32_t) (sDcInfo.sINFODcInfo.fBatt3Volt * 100);
          }else if(sDcInfo.sINFODcInfo.fBatt4Volt>30){
            private_mib_base_struct.batteryGroup.battVolt = (int32_t) (sDcInfo.sINFODcInfo.fBatt4Volt * 100);
          }
          
          private_mib_base_struct.batteryGroup.battCurr = (int32_t) ((sDcInfo.sINFODcInfo.fBatt1Curr + sDcInfo.sINFODcInfo.fBatt2Curr 
                                                            +sDcInfo.sINFODcInfo.fBatt3Curr + sDcInfo.sINFODcInfo.fBatt4Curr) * 100);
          if(sBattInfo.sRAWBattInfo.fCapTotal !=0){
            private_mib_base_struct.batteryGroup.battCapLeft1 = (uint32_t) ((sDcInfo.sINFODcInfo.fBatt1RmnCap/sBattInfo.sRAWBattInfo.fCapTotal)* 10000);
          }else {
            private_mib_base_struct.batteryGroup.battCapLeft1 =0;
          }
          if(sBattInfo.sRAWBattInfo.fCapTotal2 !=0){
          private_mib_base_struct.batteryGroup.battCapLeft2 = (uint32_t) ((sDcInfo.sINFODcInfo.fBatt2RmnCap/sBattInfo.sRAWBattInfo.fCapTotal2) * 10000);
          }
          else{
            private_mib_base_struct.batteryGroup.battCapLeft2 = 0;
          }
          if(sBattInfo.sRAWBattInfo.fCapTotal3 !=0){
            private_mib_base_struct.batteryGroup.battCapLeft3 = (uint32_t) ((sDcInfo.sINFODcInfo.fBatt3RmnCap/sBattInfo.sRAWBattInfo.fCapTotal3) * 10000);
          }else{
            private_mib_base_struct.batteryGroup.battCapLeft3 =0;
          }
          if(sBattInfo.sRAWBattInfo.fCapTotal4 !=0)
          {
            private_mib_base_struct.batteryGroup.battCapLeft4 = (uint32_t) ((sDcInfo.sINFODcInfo.fBatt4RmnCap/sBattInfo.sRAWBattInfo.fCapTotal4) * 10000);
          }else {
            private_mib_base_struct.batteryGroup.battCapLeft4 = 0;
          }
          private_mib_base_struct.batteryGroup.battTemp = (uint32_t) (sDcInfo.sINFODcInfo.fSen1BattTemp*10); //0x55AA : Invalid
          if(private_mib_base_struct.batteryGroup.battTemp>= 2000){
            private_mib_base_struct.batteryGroup.battTemp = 0;
          }
          
          private_mib_base_struct.batteryGroup.battBanksNumofBanks = 4;
          //alarm load breaker open-----------------------------------------------------------------
          private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open = 0;
          for (uint8_t i = 0; i < 10; i++)
          {
              if(sDcInfo.sALARMDcInfo.u8LoadFuse[i] == 0x03)
              {
                  dpc_t.fuse_flag = 1;
                  private_mib_base_struct.loadGroup.loadStatus[i] = 1;
              }
              else
              {
                  dpc_t.fuse_flag = 0;
                  private_mib_base_struct.loadGroup.loadStatus[i] = 0;
              }
              private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open |= (dpc_t.fuse_flag << i);
          }
          
          if (private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open != 0)
          {
             private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open = 1;
          }
          else
          {
             private_mib_base_struct.main_alarm_group_struct.alarm_load_breaker_open = 0;
          }
          //alarm batt breaker open---------------------------------------------------------------------
          private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 0;
          for (uint8_t i = 0; i < 4; i++)
          {
               if(sDcInfo.sALARMDcInfo.u8BattFuse[i] == 0x03)
               {
                  dpc_t.fuse_flag = 1;
                  private_mib_base_struct.batteryGroup.battBrkStatus[i] = 1;
               }
               else
               {
                  dpc_t.fuse_flag = 0;
                  private_mib_base_struct.batteryGroup.battBrkStatus[i] = 0;
               }
               private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open |= (dpc_t.fuse_flag << i);
          }
          if (private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open != 0)
          {
              private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 1;
          }
          else
          {
              private_mib_base_struct.main_alarm_group_struct.alarm_batt_breaker_open = 0;
          }
          //ac main fail------------------------------------------------------------------------
          if(sAcInfo.u8MainFail==0x80)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains =1;
          }
          else private_mib_base_struct.main_alarm_group_struct.alarm_ac_mains =0;
          // ac spd-------------------------------------------------------------------------------
          if(sAcInfo.u8ACSPD==0x81)
          {
            private_mib_base_struct.main_alarm_group_struct.alarmACSPD =1;
          }
          else private_mib_base_struct.main_alarm_group_struct.alarmACSPD =0;
          //llvd---------------------------------------------------------------------------------
          if(sDcInfo.sALARMDcInfo.u8LLVD==0x00)
            private_mib_base_struct.main_alarm_group_struct.alarm_llvd = 0;
          else private_mib_base_struct.main_alarm_group_struct.alarm_llvd = 1;
          
          //blvd--------------------------------------------------------------------------------
          if(sDcInfo.sALARMDcInfo.batt_low_volt_alarm == 0x82)private_mib_base_struct.main_alarm_group_struct.alarm_blvd = 1;
          else private_mib_base_struct.main_alarm_group_struct.alarm_blvd = 0;
          //DC low-------------------------------------------------------------------------------
          if(sDcInfo.sALARMDcInfo.u8DC==0x01)
            private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 0x01;
          else private_mib_base_struct.main_alarm_group_struct.alarm_dc_Low = 0x00;
          //rect============
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
          private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
          for(uint8_t i=0; i<6; i++)  
          {
            private_mib_base_struct.rectGroup.rectTable[i].rectStatus = 0;
            private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp |= sRectInfo.sRAWRectParam[i].u8Rect_NoResp << i;    
            private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault |= sRectInfo.sRAWRectParam[i].u8Rect_Fail << i;    							
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
            if(sRectInfo.sRAWRectParam[i].u8Rect_Sts==0)
            {
               private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =(uint32_t) (sRectInfo.fAllRectDcVolt * 100);
               private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =(uint32_t) (sRectInfo.sRAWRectParam[i].fRect_DcVolt * 100);
               
           }
                           
           else
           {
               private_mib_base_struct.rectGroup.rectTable[i].rectOutputVoltage =0;
               
           }
         }
         if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 1;
         else private_mib_base_struct.main_alarm_group_struct.alarm_rect_no_resp = 0;
         if (private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault != 0) private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 1;
         else private_mib_base_struct.main_alarm_group_struct.alarm_rect_ac_fault = 0;
         private_mib_base_struct.rectGroup.rectTotalCurrent = 0;
         for (uint8_t i = 0; i < 6; i++)     
         {
            private_mib_base_struct.rectGroup.rectTotalCurrent += (uint32_t) (private_mib_base_struct.rectGroup.rectTable[i].rectOutputCurrent);
         }
         private_mib_base_struct.rectGroup.rectInstalledRect = sRectInfo.u8Rect_Num;    
         private_mib_base_struct.rectGroup.rectActiveRect = sRectInfo.u8Rect_Num;     
         private_mib_base_struct.loadGroup.loadCurrent = (uint32_t) (sDcInfo.sINFODcInfo.fCurrent*100);
         //AC====================================
          private_mib_base_struct.acPhaseGroup.acPhaseTable[0].acPhaseVolt = (int32_t) (sAcInfo.facVolt[0] * 100);
          private_mib_base_struct.acPhaseGroup.acPhaseTable[1].acPhaseVolt = (int32_t) (sAcInfo.facVolt[1] * 100);
          private_mib_base_struct.acPhaseGroup.acPhaseTable[2].acPhaseVolt = (int32_t) (sAcInfo.facVolt[2] * 100);
          //ac low-------------------------------------------
          if ((sAcInfo.u8Thres[0] == 0x01) || (sAcInfo.u8Thres[1] ==0x01) || (sAcInfo.u8Thres[2]==0x01))
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_low = 1;
          }
          else
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_ac_low = 0;
          }
          //
          private_mib_base_struct.configGroup.cfgFloatVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fFltVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgBoostVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fBotVoltCfg * 100);
          private_mib_base_struct.configGroup.cfgTempCompValue = (uint32_t) sBattInfo.sRAWBattInfo.fTempCompVal;
          //private_mib_base_struct.configGroup.cfgLLVDVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLoMjAlrmVoltCfg * 100);
          //private_mib_base_struct.configGroup.cfgBLVDVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLVDDV * 100);
          private_mib_base_struct.configGroup.cfgDCLowVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fLoMnAlrmVoltCfg * 100);
          //private_mib_base_struct.configGroup.cfgBattTestVolt = (uint32_t) (sBattInfo.sRAWBattInfo.fTestVoltCfg * 100);
          
          //private_mib_base_struct.cfgBTGroup.cfgBTEndTime = (uint32_t) (sBattTestInfo.fBattTestDur);
          //private_mib_base_struct.cfgBTGroup.cfgBTEndCap = (uint32_t) (sBattTestInfo.fTestEndCap * 1000);
          //private_mib_base_struct.cfgBTGroup.cfgTestVoltCfg = (uint32_t)(sBattInfo.sRAWBattInfo.fTestVoltCfg*10);
          private_mib_base_struct.configGroup.cfgBattCapacityTotal  = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal2 = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal2;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal3 = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal3;
          private_mib_base_struct.configGroup.cfgBattCapacityTotal4 = (uint32_t) sBattInfo.sRAWBattInfo.fCapTotal4;
          private_mib_base_struct.configGroup.cfgCurrentlimitA      = (uint32_t) ( sBattInfo.sRAWBattInfo.fCapCCLVal * 100);
          private_mib_base_struct.configGroup.cfgDCOverVolt         = (uint32_t)(sBattInfo.sRAWBattInfo.fDCOverCfg * 100);                    
          //private_mib_base_struct.configGroup.cfgOverMajorTempLevel = (uint32_t) (sBattInfo.sRAWBattInfo.fOvMjTempAlrmLevel * 100);
          //private_mib_base_struct.configGroup.cfgHighMajorTempLevel = (uint32_t) (sBattInfo.sRAWBattInfo.fHiMjTempAlrmLevel *100);
          private_mib_base_struct.configGroup.cfgAcLowLevel = (uint32_t) sAcInfo.fAcLowThres;
          private_mib_base_struct.configGroup.cfgAcHighLevel = (uint32_t) sAcInfo.fAcHighThres;
          //alarm env
          private_mib_base_struct.main_alarm_group_struct.alarm_smoke = 0;
          if(sAlarmEnvInfo.u8EnvSmoke==0x00)
          {
            private_mib_base_struct.main_alarm_group_struct.alarm_smoke = 0x00;
          }
          else{
            private_mib_base_struct.main_alarm_group_struct.alarm_smoke = 0x01;
          }
          
          private_mib_base_struct.main_alarm_group_struct.alarm_env_door = 0;
          if(sAlarmEnvInfo.u8EnvDoor == 0x00){
            private_mib_base_struct.main_alarm_group_struct.alarm_env_door = 0x00;
          }else{
            private_mib_base_struct.main_alarm_group_struct.alarm_env_door = 0x01;
          }
          
          
          
          
          
          MESGState = ACINPUTINFO_REQ;
          break;
          
        default: 
          MESGState = ACINPUTINFO_REQ;
          break;  
      } 
    }
    //-----------------------------------------------------------
    //===========================================================
    
    vTaskDelay(400);//500
  }
}