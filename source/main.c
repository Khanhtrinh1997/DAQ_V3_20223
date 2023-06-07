/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 * ---> namht4
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "fsl_wdog.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "stdio.h"

#include "fsl_flexspi.h"
#include "fsl_cache.h"
#include "fsl_common.h"
   
#include "header_files.h"
//user
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"
#include "mibs/mib2_impl.h"
#include "hardware/mimxrt1050/mimxrt1050_crypto.h"
#include "daq_v3_private_mib_impl.h"
#include "drivers/phy/dp83825_driver.h"
#include "drivers/mac/mimxrt1050_eth_driver.h"
#include "drivers/phy/lan8720_driver.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define control_task_priority                         (configMAX_PRIORITIES - 1)
#define agisson_task_priority                         (configMAX_PRIORITIES - 3)
#define emerson_m500d_task_priority                   (configMAX_PRIORITIES - 3)
#define emerson_m221_task_priority                    (configMAX_PRIORITIES - 3)
#define zte_task_priority                             (configMAX_PRIORITIES - 3)  
#define modbus_rtu_task_priority                      (configMAX_PRIORITIES - 3)
#define dpc_task_priority                             (configMAX_PRIORITIES - 3)
   
   

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
extern status_t flexspi_nor_flash_erase_sector(FLEXSPI_Type *base, uint32_t address);
extern status_t flexspi_nor_flash_page_program(FLEXSPI_Type *base, uint32_t dstAddr, const uint32_t *src);
extern status_t flexspi_nor_get_vendor_id(FLEXSPI_Type *base, uint8_t *vendorId);
extern status_t flexspi_nor_enable_quad_mode(FLEXSPI_Type *base);
extern status_t flexspi_nor_erase_chip(FLEXSPI_Type *base);
extern void flexspi_nor_flash_init(FLEXSPI_Type *base);
extern void http_handler_init(void);
extern void snmp_handler_init(void);
/*******************************************************************************
 * Code
 ******************************************************************************/
static void ioInit(void) {
   gpio_pin_config_t pinConfig;

   //Enable IOMUXC clock
   CLOCK_EnableClock(kCLOCK_Iomuxc);
   //Enable IOMUXC SNVS clock
   CLOCK_EnableClock(kCLOCK_IomuxcSnvs);

   //Configure GPIO_AD_B0_09 pin as GPIO1_IO09
   IOMUXC_SetPinMux(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09, 0);

   //Set GPIO_AD_B0_09 pad properties
   IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_09_GPIO1_IO09,
      IOMUXC_SW_PAD_CTL_PAD_HYS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(0) |
      IOMUXC_SW_PAD_CTL_PAD_PKE(0) |
      IOMUXC_SW_PAD_CTL_PAD_ODE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SPEED(0) |
      IOMUXC_SW_PAD_CTL_PAD_DSE(7) |
      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

   //Configure WAKEUP pin as GPIO5_IO00
   IOMUXC_SetPinMux(IOMUXC_SNVS_WAKEUP_GPIO5_IO00, 0);

   //Set WAKEUP pad properties
   IOMUXC_SetPinConfig(IOMUXC_SNVS_WAKEUP_GPIO5_IO00,
      IOMUXC_SW_PAD_CTL_PAD_HYS(0) |
      IOMUXC_SW_PAD_CTL_PAD_PUS(3) |
      IOMUXC_SW_PAD_CTL_PAD_PUE(1) |
      IOMUXC_SW_PAD_CTL_PAD_PKE(1) |
      IOMUXC_SW_PAD_CTL_PAD_ODE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SPEED(0) |
      IOMUXC_SW_PAD_CTL_PAD_DSE(0) |
      IOMUXC_SW_PAD_CTL_PAD_SRE(0));

   //Configure user LED
   pinConfig.direction = kGPIO_DigitalOutput;
   pinConfig.outputLogic = 0;
   pinConfig.interruptMode = kGPIO_NoIntmode;
   GPIO_PinInit(BOARD_USER_LED_GPIO, BOARD_USER_LED_GPIO_PIN, &pinConfig);

   //Configure user button
   pinConfig.direction = kGPIO_DigitalInput;
   pinConfig.outputLogic = 0;
   pinConfig.interruptMode = kGPIO_NoIntmode;
   GPIO_PinInit(BOARD_USER_BUTTON_GPIO, BOARD_USER_BUTTON_GPIO_PIN, &pinConfig);
}

static void Hardwawre_Init(void) {
  BOARD_ConfigMPU();
  BOARD_InitBootPins();
  BOARD_InitBootClocks();
  
  SystemCoreClockUpdate();
  
  //----------------------------------------------------------
  CLOCK_InitSysPfd(kCLOCK_Pfd2, 29);
  /* Set semc clock to 163.86 MHz */
  CLOCK_SetMux(kCLOCK_SemcMux, 1);
  CLOCK_SetDiv(kCLOCK_SemcDiv, 1);
}


static void SNMP_Init() {
   error_t error;
   NetInterface *interface;
   MacAddr macAddr;
   Ipv4Addr ipv4Addr;

   osInitKernel();
   ioInit();
   //Initialize hardware cryptographic accelerator
   error = mimxrt1050CryptoInit();
   //Any error to report?
   if(error)
   {
      // TRACE_ERROR("Failed to initialize hardware crypto accelerator!\r\n");
   }

   //Standard MIB-II initialization
   error = mib2Init();
   //Any error to report?
   if(error)
   {
      // TRACE_ERROR("Failed to initialize MIB!\r\n");
   }

   //Private MIB initialization
   error = privateMibInit();
   //Any error to report?
   if(error)
   {
      // TRACE_ERROR("Failed to initialize MIB!\r\n");
   } 
   error = netInit();
   
   if(error)
   {
      // TRACE_ERROR("Failed to initialize TCP/IP stack!\r\n");
   }
   
   interface = &netInterface[0];
   //Set interface name
   netSetInterfaceName(interface, APP_IF_NAME);
   //Set host name
   netSetHostname(interface, "SiteMonitor");
   //Set host MAC address
   macStringToAddr(&private_mib_base_struct.siteGroup.siteMACInfo[0], &macAddr);
   netSetMacAddr(interface, &macAddr);
   //Select the relevant network adapter
   netSetDriver(interface, &mimxrt1050EthDriver);
   
   if(system_flag_struct.eth_type == DP83825)
   {
     netSetPhyDriver(interface, &dp83825PhyDriver);
   }
   else if(system_flag_struct.eth_type == LAN8720)
   {
     netSetPhyDriver(interface, &lan8720PhyDriver);
   }
   
   //Initialize network interface
   error = netConfigInterface(interface);
   //Any error to report?
   if(error)
   {
      // TRACE_ERROR("Failed to configure interface %s!\r\n", interface->name);
   }
   
   //Set IPv4 host address
   ipv4StringToAddr(APP_IPV4_HOST_ADDR, &ipv4Addr);
   ipv4SetHostAddr(interface, (Ipv4Addr) private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32IP);
    
   //Set subnet mask
   ipv4StringToAddr(APP_IPV4_SUBNET_MASK, &ipv4Addr);
   ipv4SetSubnetMask(interface, (Ipv4Addr) private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32SN);

   //Set default gateway
   ipv4StringToAddr(APP_IPV4_DEFAULT_GATEWAY, &ipv4Addr);
   ipv4SetDefaultGateway(interface, (Ipv4Addr) private_mib_base_struct.cfgNetworkGroup.siteNetworkInfo.u32GW);

   //Set primary and secondary DNS servers
   ipv4StringToAddr(APP_IPV4_PRIMARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 0, ipv4Addr);
   ipv4StringToAddr(APP_IPV4_SECONDARY_DNS, &ipv4Addr);
   ipv4SetDnsServer(interface, 1, ipv4Addr);

   snmp_handler_init();
   http_handler_init();
}

//init_dc_cabinet-------------------------------------------------
static void init_dc_cabinet(){
  
  TYPE = parameter.u8ControllerType;
  if ((TYPE == EMER) || (TYPE == ZTE) || (TYPE == EMERV21))
  {
    init_rs232_dc_cabinet(BAUD_RATE_9600,NONE_PARITY);
    
  } 
  else if (TYPE == HW)
  {
    DIR_MODBUS_AB_DC_CABINET_OFF
    init_rs485_dc_cabinet_9_bit(BAUD_RATE_9600,NONE_PARITY);
  }
  else if(TYPE == DKD51_BDP)
  {
    DIR_MODBUS_AB_DC_CABINET_OFF
    init_rs485_dc_cabinet(BAUD_RATE_9600,NONE_PARITY);
    
  }
  
}

//init modbus rtu-------------------------------------------------
static void init_modbus_ab(){
  DIR_MODBUS_AB_OFF
  init_rs485_modbus_rtu(BAUD_RATE_9600,NONE_PARITY);
}
//software init---------------------------------------------------
#define FLAG_ETH  0
                       
uint8_t eth_write[1];
uint8_t eth_read[1];
static void Softwawre_Init(void) {
  sFLASH_Init();
  config_init();
  
  //--------------------------------------------------------------
  //sFLASH_EraseSector(FLASH_CHECK_ETH);
  if(FLAG_ETH == 1){
    eth_write[0] = 0x99;
    sFLASH_EraseSector(FLASH_CHECK_ETH);
    sFLASH_WriteBuffer(eth_write, FLASH_CHECK_ETH, 1);
  }
  //--------------------------------------------------------------
  for(uint8_t i=0; i<3;i++){
    sFLASH_ReadBuffer(eth_read, FLASH_CHECK_ETH, 1);
    if(eth_read[0] == 0x99) break;
  }
  
  if(eth_read[0] == 0x99){
    system_flag_struct.eth_type = LAN8720; 
  }
  else{
    system_flag_struct.eth_type = DP83825;
  }
  //--------------------------------------------------------------
  if(system_flag_struct.eth_type == LAN8720)
  {
    /* GPIO configuration of DIGITAL_IN on GPIO_B0_05 (pin B8) */
    gpio_pin_config_t DIGITAL_IN_config = {
      .direction = kGPIO_DigitalInput,
      .outputLogic = 0U,
      .interruptMode = kGPIO_NoIntmode
    };
    /* Initialize GPIO functionality on GPIO_B0_05 (pin B8) */
    GPIO_PinInit(GPIO2, 5U, &DIGITAL_IN_config);
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_GPIO2_IO05, 0U); 
  }
    
  init_modbus_ab();
  init_dc_cabinet();
  
    
  
  SNMP_Init();
  
  // Init WDog.
  wdog_config_t configWDog;
  WDOG_GetDefaultConfig(&configWDog);
  configWDog.timeoutValue = 0xFFU; // Timeout value is (0xF + 1)/2 = 8 sec.
  WDOG_Init(WDOG1, &configWDog);
}

int main(void) {
  Hardwawre_Init();
  Softwawre_Init();
  
  
  
  xTaskCreate(control_task          , "control_task"             , configMINIMAL_STACK_SIZE + 512   , NULL, control_task_priority          , NULL);
  xTaskCreate(modbus_rtu_task       , "modbus_rtu_task"          , configMINIMAL_STACK_SIZE + 512   , NULL, modbus_rtu_task_priority       , NULL);
  switch(TYPE){
  case EMER://0.emerson m500d power monitor--------------------------------------------------------------------------------------------------------------
    {
      xTaskCreate(emersion_m500d_task   , "emersion_m500d_task"      , configMINIMAL_STACK_SIZE + 256   , NULL, emerson_m500d_task_priority     , NULL);
    }
    break;
  case EMERV21://3.emerson m221s power monitor-----------------------------------------------------------------------------------------------------------
    {
      xTaskCreate(emersion_m221_task   , "emersion_m221_task"        , configMINIMAL_STACK_SIZE + 256   , NULL, emerson_m221_task_priority      , NULL);
    }
    break;
  case HW://2.agisson power monitor----------------------------------------------------------------------------------------------------------------------
    {
      xTaskCreate(agisson_task         , "agisson_task"              , configMINIMAL_STACK_SIZE + 256   , NULL, agisson_task_priority           , NULL);
    }
    break;
  case ZTE://1.zte power monitor---------------------------------------------------------------------------------------------------------------------------
    {
      xTaskCreate(zte_task             , "zte_task"                  , configMINIMAL_STACK_SIZE + 256   , NULL, zte_task_priority               , NULL);
    }
    break;
  case DKD51_BDP://4. dpc power monitor-------------------------------------------------------------------------------------------------------------------- 
    {
      xTaskCreate(dpc_task             , "dpc_task"                  , configMINIMAL_STACK_SIZE + 256   , NULL, dpc_task_priority               , NULL);
    }
    break;
  default:
    break;
  }
  vTaskStartScheduler();
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
//static void hello_task(void *pvParameters) {
//  static uint32_t hello_task_Cnt = 0;
//  
//  char string[50] = {0};
//  for (;;) {
//    hello_task_Cnt++;
//    LPUART1_Send_Test();
//    sprintf(string, "Hello world 1 %d.\r\n", hello_task_Cnt);
//    LPUART1_Send((uint8_t *)&string, sizeof(string));
//    vTaskDelay(1000);
//  }
//}

void vApplicationTickHook(void) {
  static uint32_t tickhook_Cnt = 0;
  tickhook_Cnt++;
  trapStatus_TimePeriod++;
  
  if(private_mib_base_struct.siteGroup.siteSystemReset == 0x53){
    NVIC_SystemReset();
  }
  
  // WDog
  static uint32_t wdRefresh = 0;
  if (++wdRefresh > 3) {    //500
    wdRefresh = 0;
    WDOG_Refresh(WDOG1);
  }
  
  // Led toggle
  static uint32_t ledToggleCnt = 0;
  if (++ledToggleCnt >= 50) {
    ledToggleCnt = 0;
    blink_led_user();
  }
  
  
}