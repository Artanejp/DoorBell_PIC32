/*******************************************************************************
 System Interrupt Source File

  File Name:
    sys_interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file contains a definitions of the raw ISRs required to support the 
    interrupt sub-system.
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
/*
*********************************************************************************************************
*                                           INCLUDES
*********************************************************************************************************
*/
#include <xc.h>



#include "ISR_Support.h"


/* External Interrupt Instance 0 Interrupt */
   .extern  IntHandlerExternalInterruptInstance0

   .section	.vector_3,code, keep
   .equ     __vector_dispatch_3, IntVectorExternalInterruptInstance0
   .global  __vector_dispatch_3
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorExternalInterruptInstance0

IntVectorExternalInterruptInstance0:
    la    $26,  _IntHandlerExternalInterruptInstance0         
    jr    $26
    nop
	.end	IntVectorExternalInterruptInstance0

   .section	.ExternalInterruptInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerExternalInterruptInstance0

_IntHandlerExternalInterruptInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerExternalInterruptInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerExternalInterruptInstance0


 

/* TMR Instance 0 Interrupt */
   .extern  IntHandlerDrvTmrInstance0

   .section	.vector_16,code, keep
   .equ     __vector_dispatch_16, IntVectorDrvTmrInstance0
   .global  __vector_dispatch_16
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance0

IntVectorDrvTmrInstance0:
    la    $26,  _IntHandlerDrvTmrInstance0         
    jr    $26
    nop
	.end	IntVectorDrvTmrInstance0

   .section	.DrvTmrInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerDrvTmrInstance0

_IntHandlerDrvTmrInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerDrvTmrInstance0


/* TMR Instance 1 Interrupt */
   .extern  IntHandlerDrvTmrInstance1

   .section	.vector_12,code, keep
   .equ     __vector_dispatch_12, IntVectorDrvTmrInstance1
   .global  __vector_dispatch_12
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance1

IntVectorDrvTmrInstance1:
    la    $26,  _IntHandlerDrvTmrInstance1         
    jr    $26
    nop
	.end	IntVectorDrvTmrInstance1

   .section	.DrvTmrInstance1_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerDrvTmrInstance1

_IntHandlerDrvTmrInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerDrvTmrInstance1


/* TMR Instance 2 Interrupt */
   .extern  IntHandlerDrvTmrInstance2

   .section	.vector_8,code, keep
   .equ     __vector_dispatch_8, IntVectorDrvTmrInstance2
   .global  __vector_dispatch_8
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvTmrInstance2

IntVectorDrvTmrInstance2:
    la    $26,  _IntHandlerDrvTmrInstance2         
    jr    $26
    nop
	.end	IntVectorDrvTmrInstance2

   .section	.DrvTmrInstance2_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerDrvTmrInstance2

_IntHandlerDrvTmrInstance2:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvTmrInstance2
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerDrvTmrInstance2




/* USART Instance 0 Interrupt */

   .extern  IntHandlerDrvUsartInstance0

   .section	.vector_32,code, keep
   .equ     __vector_dispatch_32, IntVectorDrvUsartInstance0
   .global  __vector_dispatch_32
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvUsartInstance0

IntVectorDrvUsartInstance0:
    la    $26,  _IntHandlerDrvUsartInstance0         
    jr    $26
    nop
	.end	IntVectorDrvUsartInstance0

   .section	.DrvUsartInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerDrvUsartInstance0

_IntHandlerDrvUsartInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvUsartInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerDrvUsartInstance0


/* OC Instance 0 Interrupt */
   .extern  IntHandlerDrvOCInstance0

   .section	.vector_10,code, keep
   .equ     __vector_dispatch_10, IntVectorDrvOCInstance0
   .global  __vector_dispatch_10
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvOCInstance0

IntVectorDrvOCInstance0:
    la    $26,  _IntHandlerDrvOCInstance0         
    jr    $26
    nop
	.end	IntVectorDrvOCInstance0

   .section	.DrvOCInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerDrvOCInstance0

_IntHandlerDrvOCInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvOCInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerDrvOCInstance0



/* Sytem DMA Instance 0 Interrupt */
   .extern  IntHandlerSysDmaInstance0

   .section	.vector_40,code, keep
   .equ     __vector_dispatch_40, IntVectorSysDmaInstance0
   .global  __vector_dispatch_40
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSysDmaInstance0

IntVectorSysDmaInstance0:
    la    $26,  _IntHandlerSysDmaInstance0         
    jr    $26
    nop
	.end	IntVectorSysDmaInstance0

   .section	.SysDmaInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerSysDmaInstance0

_IntHandlerSysDmaInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSysDmaInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerSysDmaInstance0


/* Sytem DMA Instance 1 Interrupt */
   .extern  IntHandlerSysDmaInstance1

   .section	.vector_41,code, keep
   .equ     __vector_dispatch_41, IntVectorSysDmaInstance1
   .global  __vector_dispatch_41
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorSysDmaInstance1

IntVectorSysDmaInstance1:
    la    $26,  _IntHandlerSysDmaInstance1         
    jr    $26
    nop
	.end	IntVectorSysDmaInstance1

   .section	.SysDmaInstance1_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerSysDmaInstance1

_IntHandlerSysDmaInstance1:
    portSAVE_CONTEXT
    la    s6,  IntHandlerSysDmaInstance1
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerSysDmaInstance1



/* I2C Instance 0 Interrupt */

   .extern  IntHandlerDrvI2CInstance0

   .section	.vector_33,code, keep
   .equ     __vector_dispatch_33, IntVectorDrvI2CInstance0
   .global  __vector_dispatch_33
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorDrvI2CInstance0

IntVectorDrvI2CInstance0:
    la    $26,  _IntHandlerDrvI2CInstance0         
    jr    $26
    nop
	.end	IntVectorDrvI2CInstance0

   .section	.DrvI2CInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerDrvI2CInstance0

_IntHandlerDrvI2CInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerDrvI2CInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerDrvI2CInstance0



/* USB Device Interrupt */
   .extern  IntHandlerUSBInstance0

   .section	.vector_30,code, keep
   .equ     __vector_dispatch_30, IntVectorUSBInstance0
   .global  __vector_dispatch_30
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorUSBInstance0

IntVectorUSBInstance0:
    la    $26,  _IntHandlerUSBInstance0         
    jr    $26
    nop
	.end	IntVectorUSBInstance0

   .section	.USBInstance0_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _IntHandlerUSBInstance0

_IntHandlerUSBInstance0:
    portSAVE_CONTEXT
    la    s6,  IntHandlerUSBInstance0
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end	_IntHandlerUSBInstance0



/*******************************************************************************
 End of File
 */

