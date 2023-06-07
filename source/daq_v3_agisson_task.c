//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
uint32_t setuptimer =5000;
uint32_t SetupFlagComplete =1;
void agisson_task(void* pv_parameters){
  private_mib_base_struct.cntGroup.task.agisson = 0;
  uint32_t i;
  //uint32_t iSet =0;
  while(true){
    private_mib_base_struct.cntGroup.task.agisson++;
    //code here===========
    setuptimer++;
    RS485Timer++;
    if(setuptimer < 6000)
    {
      SetupFlagComplete =0;
    }
    else 
    {
      setuptimer =6500;
      if(setuptimer>10000)setuptimer=6500;
      SetupFlagComplete =1;
    }
    if (Program_Status != 0)
    {
      private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 0;
    } 
    else
    {
      private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect = 1;    
    }
    
    if((private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 ==0)&& (private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect !=0))
    {
      if(Uart_data_get.Year!=0)
      {
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =29;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Year  =   Uart_data_get.Year;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Month =   Uart_data_get.Month;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Day   =   Uart_data_get.Day;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hour  =   Uart_data_get.Hour;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Minute=   Uart_data_get.Minute;
        NextHis();
        private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 = private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect;
        HisCount_old[ih] = sHisFlashLog[ih].Count;
      }
      
    }
    if((private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 !=0)&& (private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect ==0))
    {
      if(Uart_data_get.Year!=0)
      {
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hiscode =30;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Year  =   Uart_data_get.Year;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Month =   Uart_data_get.Month;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Day   =   Uart_data_get.Day;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Hour  =   Uart_data_get.Hour;
        sHisFlashLog[ih].HisFlashEvent[sHisFlashLog[ih].Count].Minute=   Uart_data_get.Minute;
        NextHis();
        private_mib_base_struct.connect_alarm_group_struct.alarmPMUConnect_old3 = private_mib_base_struct.connect_alarm_group_struct.alarm_pmu_connect;
        
      }
    }
    if(MESGState == HISTORYMESG_REQ)             //
    {
      GetHistoryLog();//che tam 
      MESGState = HISTORYMESG_RES;
    }
    if(Program_Status ==0)
    {
      setCmd_Ready =0;
      Send_StartMesg0(RS485Timer);
      if(RS485Timer>1000)
      {
        RX485Count=0;
        RecvCntStart485=0;
        RS485Timer=0;
        RS485Command++;
        if(RS485Command>40) RS485Command =0;
        
      }    
    }
    if(Program_Status ==1)
    {
      setCmd_Ready =0;
      if(RS485Timer >1000)
      {
        RS485Command++;
        RS485Timer =0;
      }
      if(RS485Timer ==600)
      {
        Recv_Mesg();
      }
      
      switch(RS485Command)
      {
        
      case 1:
        Send_StartMesg1(RS485Timer);
        break;
      case 2:
        Send_StartMesg23(RS485Timer);
        break;
      case 3:
        Send_StartMesg23(RS485Timer);
        break;
      case 4:
        Send_StartMesg4(RS485Timer);
        break;
      case 5:
        if(CPMU01_Ver_IP ==1){Send_StartMesg10(RS485Timer);}
        else {Send_StartMesg56789(RS485Timer);}
        //          Send_StartMesg56789(RS485Timer);
        break;
      case 6:
        Send_StartMesg56789(RS485Timer);
        break;
      case 7:
        Send_StartMesg56789(RS485Timer);
        break;
      case 8:
        Send_StartMesg56789(RS485Timer);
        break;
      case 9:
        Send_StartMesg56789(RS485Timer);
        break;
      case 10:
        Send_StartMesg10(RS485Timer);
        break;
      default:
        break;
      }
      if(CPMU01_Ver_IP ==1){
        if(RS485Command >5)
        {
          Program_Status =2;
          RS485Command =0;
          RS485Timer =0;
        }
      }
      else {
        
        if(RS485Command >10)
        {
          Program_Status =2;
          RS485Command =0;
          RS485Timer =0;
        }
      }  
    }
    if(Program_Status ==2)
    { 
      //      if(  setCmd_flag == 1)
      //      {
      //        thao++;
      //      }
      
      if ((setCmd_flag == 1)&&(setCmd_Ready==2)&&(RecvComplete==1))            //&&(RS485Command <31)
      {
        setuptimer =0;
        settingCommandmask |= setCmd_mask;
        setCmd_mask=0;
        for(i=0;i<32;i++)
        {
          if(settingCommand !=0)
          {
            i=32;
          }
          else
          {
            settingCommand = settingCommandmask & (0x00000001<<i); 
          }
        }
        
        
        switch(settingCommand)
        {
        case SET_FLTVOL:
          SetFloatVolt();
          break;
        case SET_BSTVOL:
          SetBoostVolt();
          break;
        case SET_CCL:
          SetCCL();
          break;
        case SET_BATTSTDCAP:
          SetBattCap1();
          break;
        case SET_BATTSTDCAP2:
          SetBattCap2();
          break;
          
        case SET_LLVD_EN:
          SetLLVD_EN();
          break;
        case SET_BLVD_EN:
          SetBLVD_EN();
          break;
          
        case SET_LLVD:
          SetLLVD();
          break;
        case SET_BLVD:
          SetBLVD();
          break;
        case SET_TEMPCOMP_VAL:
          SetTempComVal();
          break;
        case SET_OVERTEMP:
          SetOverTemp();
          break;
        case SET_DCLOW:
          SetDClow();
          break;
        case SET_AC_THRES:
          SetACLow();
          break;
        case SET_BT_ENDVOLT:
          SetBattTestEndVolt();
          break;
        case SET_BT_CELLNO:
          SetBattStdCellNo();
          break;
        case SET_BT_ENDTIME:
          SetBattStdTestTime();
          break;
          //         case SET_BATT_SHORT_TEST_END_VOLT:
          //           SetBattShortTestEndVolt();
          //           break;
          //         case SET_BATT_SHORT_TEST_TIME:
          //           SetBattShortTestTime();
          //           break;  
        case START_BT_TEST:
          Start_Batt_Manual_Test();
          break;
          //         case START_BATT_SHORT_TEST:
          //           Start_Batt_Short_Test();
          //           break;
        case STOP_BT_TEST:
          Set_Batt_Test_Stop();
          break;
        default:
          break;
          
        }
        
      }
      else //if (((setCmd_flag == 0)||(setCmd_Ready!=2)))   //||((RS485Command >30)&&(setCmd_flag == 1))
      {
        if(RS485Timer > 200)
        {
          if(RecvOK ==1)
          {
            RS485Command++;
            RS485Timer =0;
          }
          else 
          {
            if(RS485Timer>300)
            {
              RS485Command++;
              RS485Timer =0;
            }
          }
          if(CPMU01_Ver_IP ==1)
          {
            if((RS485Command>3)&&(RS485Command<9))RS485Command=9;
          }
          
        }
        if(RS485Command > 40) 
        {
          RS485Command =0;
          setCmd_Ready++;
          if(setCmd_Ready>2)setCmd_Ready=2;
        }
        if(RS485Command < 31)
        {
          Send_RequestMesg(RS485Timer, RS485Command);
          if(RS485Timer ==150)
          {
            if(CPMU01_Ver_IP ==1)
            {
              if(SetupFlagComplete ==1)
              {
                Recv_Mesg();
              }
            }
            else
            {
              Recv_Mesg();
            }
          }
        }
        if(RS485Command==32)
        {
          if(writeFlash>10)
          {
            writeFlash=0;
            for(i=0;i<5;i++)
            {
              
              if(sHisFlashLog[i].Count_old< sHisFlashLog[i].Count)
              {  
                pHisFlashLog =    &sHisFlashLog[i];
                switch(i)
                {
                case 0:
                  sFLASH_EraseSector(FLASH_PB_LOG_START); //che tam
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB_LOG_START, 256);//che tam  
                  break;
                case 1:
                  sFLASH_EraseSector(FLASH_PB1_LOG_START);//che tam
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB1_LOG_START, 256);//che tam
                  break;
                case 2:
                  sFLASH_EraseSector(FLASH_PB2_LOG_START);//che tam
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB2_LOG_START, 256);//che tam
                  break;
                case 3:
                  sFLASH_EraseSector(FLASH_PB3_LOG_START);//che tam
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB3_LOG_START, 256);//che tam
                  break;
                case 4:
                 sFLASH_EraseSector(FLASH_PB4_LOG_START);//che tam
                  sFLASH_WriteBuffer((uint8_t *)pHisFlashLog, FLASH_PB4_LOG_START, 256);//che tam
                  break;
                default:
                  break;
                }
                sHisFlashLog[i].Count_old = sHisFlashLog[i].Count;
                
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
        }
      } 
    }
    vTaskDelay(3);
    //code here==========
    
  }
}