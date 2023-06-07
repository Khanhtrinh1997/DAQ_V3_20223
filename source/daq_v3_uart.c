//--thanhcm3--07--04--22--

//include-----------------------------------------------------------------------
#include "header_files.h"
//funciton----------------------------------------------------------------------
//LPUART3 IRQHandler-------------------------------
void Looptest2(uint8_t ucChar)
{
   if(modbus_rtu_struct.running_step ==_TEST_MODE)
     {
        sTestRS485.GenBuff[sTestRS485.GenRecvCnt++]=  ucChar;
     }
   if(sTestRS485.GenRecvCnt>14)   sTestRS485.GenRecvCnt=0;
}
void LPUART3_IRQHandler(void) {
  uint8_t char_lpuart3;
  if ((kLPUART_RxDataRegFullFlag|kLPUART_RxOverrunFlag)&LPUART_GetStatusFlags(LPUART3)){
    
    char_lpuart3 = LPUART_ReadByte(LPUART3);
    Looptest2(char_lpuart3);
    if((fan_dpc_switch_uart_t.rx_enable == STATE_ON)&&(TYPE == DKD51_BDP)){
      // for modbus china----------------------------------------------------
      //char_lpuart3 = LPUART_ReadByte(LPUART3);
      fan_dpc_switch_uart_t.uart_rx.buff_485[fan_dpc_switch_uart_t.uart_rx.cnt] = char_lpuart3;
      fan_dpc_switch_uart_t.uart_rx.cnt++;
      if((fan_dpc_switch_uart_t.uart_rx.cnt>=500)||(fan_dpc_switch_uart_t.uart_rx.cnt<0)) fan_dpc_switch_uart_t.uart_rx.cnt=0;
    }
    else
    {
      //for modbus standard------------------------------------------------------------------
      
      //reading-------------------------------
      if(modbus_rtu_struct.modbus_state == READING){
        modbus_rtu_struct.buff_read[modbus_rtu_struct.data_pointer++] = char_lpuart3;
        if(modbus_rtu_struct.data_pointer == modbus_rtu_struct.byte_count){
          modbus_rtu_struct.modbus_state = READING_END;
        }
      }
      //check function code-------------------
      if((modbus_rtu_struct.buff_read[1]>=1)&&(modbus_rtu_struct.buff_read[1]<=50)){
        switch(modbus_rtu_struct.buff_read[1]){
        case _READ_COIL_STATUS:
          modbus_rtu_struct.byte_count = modbus_rtu_struct.buff_read[2]+5;
          break;
        case _READ_INPUT_STATUS:
          modbus_rtu_struct.byte_count = modbus_rtu_struct.buff_read[2]+5;
          break;
        case _READ_HOLDING_REGS:
          modbus_rtu_struct.byte_count = modbus_rtu_struct.buff_read[2]+5;
          break;
        case _READ_INPUT_REG:
          modbus_rtu_struct.byte_count = modbus_rtu_struct.buff_read[2]+5;
          break;
        case _PRESET_SINGLE_REG:
          modbus_rtu_struct.byte_count = 8;
          break;
        case _PRESET_MULTIPLE_REGS:
          modbus_rtu_struct.byte_count = 8;
          break;
        case _SETUP_REGISTER:
          break;
        case _USER_REGISTER:
          modbus_rtu_struct.byte_count = 8;
          break;
        case _REPORT_SLAVE_ID:
          modbus_rtu_struct.byte_count = modbus_rtu_struct.buff_read[2]+5;
          break;
        default:
          modbus_rtu_struct.byte_count = 8; 
          break;
          
        }
        
      }else{
        modbus_rtu_struct.modbus_state = START;
        modbus_rtu_struct.data_pointer = 0;
      }
      //start---------------------------------
      if((char_lpuart3 == modbus_rtu_struct.slave_id)&&(modbus_rtu_struct.modbus_state ==START)){
        modbus_rtu_struct.modbus_state = READING;
        modbus_rtu_struct.data_pointer = 0;
        modbus_rtu_struct.buff_read[modbus_rtu_struct.data_pointer++] = char_lpuart3;
      }
      
    }
      
  }
  SDK_ISR_EXIT_BARRIER;
}
//LPUART4 IRQHander---------------------------------------------------------------------
uint8_t Recv03 =0;
uint8_t RecvComplete = 1;
unsigned char thaouchar;
void Looptest(uint8_t ucChar)
{
  if(modbus_rtu_struct.running_step ==_TEST_MODE)
  {
    sTestRS485.CabBuff[sTestRS485.CabRecvCnt++]=  ucChar;
  }
  if(sTestRS485.CabRecvCnt>14)   sTestRS485.CabRecvCnt=0;
}
//LPUART4 IRQHandler-------------------------------
void LPUART4_IRQHandler(void) {
  //uint8_t char_lpuart4;
  unsigned char ucChar,csum;
  if ((kLPUART_RxDataRegFullFlag|kLPUART_RxOverrunFlag)&LPUART_GetStatusFlags(LPUART4)){
    if((TYPE == HW)||(TYPE == EMER))
    {
      Cab_Connection = 0;
      ucChar         = LPUART_ReadByte(LPUART4);
      Looptest(ucChar);
      thaouchar = ucChar;
      g_UARTRxBuf485[RecvCntStart485]=  ucChar;
      RecvCntStart485++;
      RecvComplete = 1;
      if( (g_UARTRxBuf485[0] == 0x03)&&(RecvCntStart485>2) )
      {
        RecvComplete = 0;
        RecvCntEnd485 = g_UARTRxBuf485[2]+3;
        if(iSet!=0)RecvComplete =1;
      }
      else  RecvCntEnd485=0;
      if( RecvCntStart485>  RecvCntEnd485)
      {
        csum = ChecksumH(&g_UARTRxBuf485[0],RecvCntEnd485);
        if(csum==  g_UARTRxBuf485[RecvCntEnd485])
        {
          RecvOK =1; 
          RecvComplete = 1;
          if(g_UARTRxBuf485[4]== AcmdCodeTable[RS485Command][3])
          {
            RecvCodeOK=1;
          }
          else
          {
            RecvCodeOK=0;
          }
        }
        else RecvOK =0;
      }
      if(Program_Status ==0)        //&&(RecvCntStart485>4)
      {   
        //            if(g_UARTRxBuf485[4]==0x01)
        //              {
        if(( RecvOK==1)&&((g_UARTRxBuf485[4]== 0x01)||(g_UARTRxBuf485[2]== 0xEF) ))
        {
          Program_Status=1;
          RS485Command = 1;
          RS485Timer = 2;
          iCounter =1;
        }
        if(g_UARTRxBuf485[0]== 0x73)
        {
          Program_Status=1;
          RS485Command = 1;
          RS485Timer = 2;
          iCounter =1;
        }
        if(g_UARTRxBuf485[0]== 0x03)
        {
          Recv03=1;
        }
        //                  iCounter =1;
        //              }
      }
      
      if(RecvCntStart485>299)RecvCntStart485=0;
    }
    else if (TYPE ==DKD51_BDP){
      ucChar         = LPUART_ReadByte(LPUART4);
      dpc_t.uart_rx.buff_485[dpc_t.uart_rx.cnt] = ucChar;
      dpc_t.uart_rx.cnt++;
      if((dpc_t.uart_rx.cnt>=500)/*||(dpc_t.uart_rx.cnt<0)*/) dpc_t.uart_rx.cnt=0;
      PMUConnectCount =0;
    } 
  }
  SDK_ISR_EXIT_BARRIER;
}

//LPUART5 IRQHandler-------------------------------
void LPUART5_IRQHandler(void){
  unsigned char ucChar;
  if ((kLPUART_RxDataRegFullFlag|kLPUART_RxOverrunFlag)&LPUART_GetStatusFlags(LPUART5)){
    ucChar=LPUART_ReadByte(LPUART5);
    g_UARTRxBuf[RecvCntStart]=  ucChar;
    RecvCntStart++;
    
    if((RecvCntStart>=300)||(RecvCntStart<0))
    {
      RecvCntStart=0;
    }
    PMUConnectCount =0;
    //        RS2321Buff[RX2321Count] = ucChar;
    //        RX2321Count++;
    
    switch(InterruptMesgState) 
    {
    case CHECKBYTE1:
      if (ucChar == 0x7E)
      {
        sInterruptMesg.u8RecvByte[0] = ucChar;
        InterruptMesgState = CHECKBYTE2;
      } else 
      {
        sInterruptMesg.u8RecvByte[0] = 0;
        sInterruptMesg.u8RecvByte[1] = 0;
        sInterruptMesg.u8RecvByte[2] = 0;
      }
      break;
    case CHECKBYTE2:
      if (ucChar == 0x7B)
      {
        sInterruptMesg.u8RecvByte[1] = ucChar;
        InterruptMesgState = CHECKBYTE3;
      } else 
      {
        sInterruptMesg.u8RecvByte[0] = 0;
        sInterruptMesg.u8RecvByte[1] = 0;
        sInterruptMesg.u8RecvByte[2] = 0;
        InterruptMesgState = CHECKBYTE1;
      }
      break;
    case CHECKBYTE3:
      if (ucChar == 0x7C)
      {
        sInterruptMesg.u8RecvByte[2] = 0x7C;
      } else 
      {
        sInterruptMesg.u8RecvByte[0] = 0;
        sInterruptMesg.u8RecvByte[1] = 0;
        sInterruptMesg.u8RecvByte[2] = 0;
      }
      InterruptMesgState = CHECKBYTE1;         
      break;          
    }
    if ((sInterruptMesg.u8RecvByte[1] == 0x7B) && (sInterruptMesg.u8RecvByte[2] == 0x7C))
    {
      sInterruptMesg.u8Checkbit = 1;
      g_UARTRxBuf2[0] = 0x7E;
      g_UARTRxBuf2[1] = 0x7B;
      g_UARTRxBuf2[2] = 0x7C;
      RecvCntStart2 = 2;
      
      sInterruptMesg.u8RecvByte[0] = 0;
      sInterruptMesg.u8RecvByte[1] = 0;
      sInterruptMesg.u8RecvByte[2] = 0;
    }
    
    if (sInterruptMesg.u8Checkbit == 1)
    {
      g_UARTRxBuf2[RecvCntStart2] = ucChar;
      RecvCntStart2++;
    }
    
    
  }
  SDK_ISR_EXIT_BARRIER;
}