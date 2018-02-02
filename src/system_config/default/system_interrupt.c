/*******************************************************************************
 System Interrupts File

  File Name:
    system_interrupt.c

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the
    interrupt sub-system.

  Summary:
    This file contains source code for the interrupt vector functions in the
    system.

  Description:
    This file contains source code for the interrupt vector functions in the
    system.  It implements the system and part specific vector "stub" functions
    from which the individual "Tasks" functions are called for any modules
    executing interrupt-driven in the MPLAB Harmony system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    interrupt-driven in the system.  These handles are passed into the individual
    module "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2011-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
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

#include "system/common/sys_common.h"
#include "doorbell.h"
#include "read_uart.h"
#include "t_sounder.h"
#include "write_uart.h"
#include "DEBUG_TERM.h"
#include "system_definitions.h"

// *****************************************************************************
// *****************************************************************************
// Section: System Interrupt Vector Functions
// *****************************************************************************
// *****************************************************************************


void IntHandlerDrvI2CInstance0(void)
{
    DRV_I2C_Tasks(sysObj.drvI2C0);
 
}
     
 
   

 
 
 

 




 void IntHandlerDrvUsartInstance0(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart0);
    DRV_USART_TasksError(sysObj.drvUsart0);
    DRV_USART_TasksReceive(sysObj.drvUsart0);
}
 
 
 

void IntHandlerDrvUsartInstance1(void)
{
    DRV_USART_TasksTransmit(sysObj.drvUsart1);
    DRV_USART_TasksError(sysObj.drvUsart1);
    DRV_USART_TasksReceive(sysObj.drvUsart1);
}
void IntHandlerChangeNotification(void)
{
    /* TODO: Add code to process interrupt here */
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_CHANGE_NOTICE_B);
}


void IntHandlerSysDmaInstance0(void)
{          
    SYS_DMA_Tasks(sysObj.sysDma, DMA_CHANNEL_0);
}

void __ISR(_RTCC_VECTOR, ipl2AUTO) _IntHandlerSysRtcc (void)
{
    SYS_RTCC_Tasks(sysObj.sysRtcc);
}

extern   QueueHandle_t xPortInterruptQueue;

void IntHandlerExternalInterruptInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_3);
    volatile uint32_t _pb = 0x00010000; // RING
    xQueueSendFromISR(xPortInterruptQueue, (const void *)(&_pb), NULL);
}
void IntHandlerExternalInterruptInstance1(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0, INT_SOURCE_EXTERNAL_4);
    volatile uint32_t _pb = 0x00020000; // EXPANDER
    xQueueSendFromISR(xPortInterruptQueue, (const void *)(&_pb), NULL);
}
 

void IntHandlerDrvTmrInstance0(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_4);
}
void IntHandlerDrvTmrInstance1(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_3);
}
void IntHandlerDrvTmrInstance2(void)
{
    PLIB_INT_SourceFlagClear(INT_ID_0,INT_SOURCE_TIMER_2);
}
 
/*******************************************************************************
 End of File
*/
