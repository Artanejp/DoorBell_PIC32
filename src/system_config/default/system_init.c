/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, defines the configuration bits,
    and allocates any necessary global system resources, such as the
    sysObj structure that contains the object handles to all the MPLAB Harmony
    module objects in the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************
// <editor-fold defaultstate="collapsed" desc="Configuration Bits">

/*** DEVCFG0 ***/

#pragma config DEBUG =      ON
#pragma config JTAGEN =     OFF
#pragma config ICESEL =     ICS_PGx1
#pragma config PWP =        OFF
#pragma config BWP =        OFF
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      PRIPLL
#pragma config FSOSCEN =    ON
#pragma config IESO =       OFF
#pragma config POSCMOD =    XT
#pragma config OSCIOFNC =   OFF
#pragma config FPBDIV =     DIV_2
#pragma config FCKSM =      CSDCMD
#pragma config WDTPS =      PS1048576
#pragma config FWDTEN =     OFF
#pragma config WINDIS =     OFF
#pragma config FWDTWINSZ =  WINSZ_50
/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_1
#pragma config FPLLMUL =    MUL_20
#pragma config FPLLODIV =   DIV_4
#pragma config UPLLIDIV =   DIV_1
#pragma config UPLLEN =     ON
/*** DEVCFG3 ***/

#pragma config USERID =     0xffff
#pragma config PMDL1WAY =   ON
#pragma config IOL1WAY =    ON
#pragma config FUSBIDIO =   ON
#pragma config FVBUSONIO =  ON
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************
// <editor-fold defaultstate="collapsed" desc="DRV_I2C Initialization Data">
// *****************************************************************************
/* I2C Driver Initialization Data
*/

const DRV_I2C_INIT drvI2C0InitData =
{
    .i2cId = DRV_I2C_PERIPHERAL_ID_IDX0,
    .i2cMode = DRV_I2C_OPERATION_MODE_IDX0,
    .portSCL = DRV_SCL_PORT_IDX0,
	.pinSCL  = DRV_SCL_PIN_POSITION_IDX0,
	.portSDA = DRV_SDA_PORT_IDX0,
	.pinSDA  = DRV_SDA_PIN_POSITION_IDX0,
    .baudRate = DRV_I2C_BAUD_RATE_IDX0,
    .busspeed = DRV_I2C_SLEW_RATE_CONTROL_IDX0,
    .buslevel = DRV_I2C_SMBus_SPECIFICATION_IDX0,
    .mstrInterruptSource = DRV_I2C_MASTER_INT_SRC_IDX0,
    .errInterruptSource = DRV_I2C_ERR_MX_INT_SRC_IDX0,
};




// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_Timer Initialization Data">
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="DRV_USART Initialization Data">

const DRV_USART_INIT drvUsart0InitData =
{
    .moduleInit.value = DRV_USART_POWER_STATE_IDX0,
    .usartID = DRV_USART_PERIPHERAL_ID_IDX0, 
    .mode = DRV_USART_OPER_MODE_IDX0,
    .flags = DRV_USART_INIT_FLAGS_IDX0,
    .brgClock = DRV_USART_BRG_CLOCK_IDX0,
    .lineControl = DRV_USART_LINE_CNTRL_IDX0,
    .baud = DRV_USART_BAUD_RATE_IDX0,
    .handshake = DRV_USART_HANDSHAKE_MODE_IDX0,
    .interruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,
    .interruptReceive = DRV_USART_RCV_INT_SRC_IDX0,
    .interruptError = DRV_USART_ERR_INT_SRC_IDX0,
    .queueSizeTransmit = DRV_USART_XMIT_QUEUE_SIZE_IDX0,
    .queueSizeReceive = DRV_USART_RCV_QUEUE_SIZE_IDX0,
    .dmaChannelTransmit = DMA_CHANNEL_NONE,
    .dmaInterruptTransmit = DRV_USART_XMIT_INT_SRC_IDX0,    
    .dmaChannelReceive = DMA_CHANNEL_NONE,
    .dmaInterruptReceive = DRV_USART_RCV_INT_SRC_IDX0,    
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************
//<editor-fold defaultstate="collapsed" desc="SYS_DMA Initialization Data">
/*** System DMA Initialization Data ***/

const SYS_DMA_INIT sysDmaInit =
{
	.sidl = SYS_DMA_SIDL_DISABLE,

};
// </editor-fold>
// <editor-fold defaultstate="collapsed" desc="SYS_MSG Initialization Data">
/*** Message System Initialization Data ***/

static uint16_t queuePriorities0[2] = { 64, 32 };
SYS_MSG_INIT msg0Init =
{
    .nMaxMsgsDelivered = 1,
    .nMessagePriorities = 1,
};
// </editor-fold>

// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( void *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)NULL);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());
    SYS_PORTS_Initialize();

    /* Initialize Drivers */
    sysObj.drvI2C0 = DRV_I2C_Initialize(DRV_I2C_INDEX_0, (SYS_MODULE_INIT *)&drvI2C0InitData);


    SYS_INT_VectorPrioritySet(INT_VECTOR_I2C1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_I2C1, INT_SUBPRIORITY_LEVEL0);


    sysObj.sysDma = SYS_DMA_Initialize((SYS_MODULE_INIT *)&sysDmaInit);
    SYS_INT_VectorPrioritySet(INT_VECTOR_DMA0, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA0, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_VectorPrioritySet(INT_VECTOR_DMA1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_DMA1, INT_SUBPRIORITY_LEVEL0);

    SYS_INT_SourceEnable(INT_SOURCE_DMA_0);
    SYS_INT_SourceEnable(INT_SOURCE_DMA_1);


    /* Initialize the OC Driver */
    DRV_OC0_Initialize();
    /*Initialize TMR0 */
    DRV_TMR0_Initialize();
    /*Initialize TMR1 */
    DRV_TMR1_Initialize();
    /*Initialize TMR2 */
    DRV_TMR2_Initialize();
 
     sysObj.drvUsart0 = DRV_USART_Initialize(DRV_USART_INDEX_0, (SYS_MODULE_INIT *)&drvUsart0InitData);
    SYS_INT_VectorPrioritySet(INT_VECTOR_UART1, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_UART1, INT_SUBPRIORITY_LEVEL0);
    /* RTCC System Service Initialization Call */
    sysObj.sysRtcc = SYS_RTCC_Initialize( );

    /* Initialize System Services */

    /*** Interrupt Service Initialization Code ***/
    SYS_INT_Initialize();

    /*Setup the INT_SOURCE_EXTERNAL_0 and Enable it*/
    SYS_INT_VectorPrioritySet(INT_VECTOR_INT0, INT_PRIORITY_LEVEL1);
    SYS_INT_VectorSubprioritySet(INT_VECTOR_INT0, INT_SUBPRIORITY_LEVEL0);
    SYS_INT_ExternalInterruptTriggerSet(INT_EXTERNAL_INT_SOURCE0,INT_EDGE_TRIGGER_FALLING);
    SYS_INT_SourceEnable(INT_SOURCE_EXTERNAL_0);






    /*** Message Service Initialization Code ***/
    msg0Init.nQSizes = queuePriorities0;
    sysObj.sysMsg0 = SYS_MSG_Initialize(SYS_MSG_0, (SYS_OBJ_HANDLE)&msg0Init);

    /*** Random Service Initialization Code ***/
    SYS_RANDOM_Initialize(0, 0);

    /* Initialize Middleware */

    /* Initialize the Application */
    DOORBELL_Initialize();
}


/*******************************************************************************
 End of File
*/

