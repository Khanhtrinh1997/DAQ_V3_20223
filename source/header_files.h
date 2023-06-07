/*System includes.*/
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
/* Kernel includes. */


/* Freescale includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


/*User include*/
#include "fsl_lpuart.h"
#include "board.h"
#include "fsl_lpspi.h"
//#include "fsl_gpio.h"

#include "imx_lpuart.h"
#include "ftp_client.h"

#include "daq_v3_modbus_rtu.h"
#include "daq_v3_variables.h"
#include "daq_v3_control_task.h"
#include "daq_v3_uart.h"
#include "daq_v3_rs485.h"
#include "daq_v3_config.h"
#include "daq_v3_ftp.h"
#include "daq_v3_private_mib_module.h"
#include "daq_v3_driver_function.h"
#include "daq_v3_agisson.h"
#include "daq_v3_agisson_task.h"
#include "daq_v3_flash_spi.h"
#include "daq_v3_emerson_m500d_task.h"
#include "daq_v3_emer_driver.h"
#include "daq_v3_emerson_m221_task.h"
#include "daq_v3_zte_driver.h"
#include "daq_v3_zte_task.h"
#include "daq_v3_zte_hislog.h"
#include "snmp_handler.h"
#include "daq_v3_dpc_driver.h"
#include "daq_v3_dpc_task.h"




/*Ethernet interface configuration*/
#define APP_IF_NAME "DAQ-V3"
#define APP_MAC_ADDR "00-AB-CD-EF-10-52"

#define APP_USE_DHCP_CLIENT ENABLED
#define APP_IPV4_HOST_ADDR "192.168.100.20"
#define APP_IPV4_SUBNET_MASK "255.255.255.0"
#define APP_IPV4_DEFAULT_GATEWAY "192.168.100.1"
#define APP_IPV4_PRIMARY_DNS "8.8.8.8"
#define APP_IPV4_SECONDARY_DNS "8.8.4.4"


