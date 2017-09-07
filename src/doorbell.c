/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    doorbell.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

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

#include "doorbell.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/

DOORBELL_DATA doorbellData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void DOORBELL_Initialize ( void )

  Remarks:
    See prototype in doorbell.h.
 */

void DOORBELL_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    doorbellData.state = DOORBELL_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
}

DOORBELL_TIMER_TICK_T led_1Data;
SYS_TMR_HANDLE led_1Handle;
extern void ledTimerCallback(uintptr_t context, uint32_t currTick);

SYS_MODULE_OBJ temp1_Obj;
DRV_HANDLE temp1_Handle;
SYS_TMR_HANDLE delay_Handle;
/******************************************************************************
  Function:
    void DOORBELL_Tasks ( void )

  Remarks:
    See prototype in doorbell.h.
 */

void DOORBELL_Tasks ( void )
{

    /* Check the application's current state. */
    switch ( doorbellData.state )
    {
        /* Application's initial state. */
        case DOORBELL_STATE_INIT:
        {
            bool appInitialized = true;
        
            {
                temp1_Obj = NULL;
                temp1_Handle = NULL;
                DRV_TMR_INIT init;
                init.moduleInit.value = SYS_MODULE_POWER_RUN_FULL;
                init.tmrId              = TMR_ID_5;
                init.clockSource      = DRV_TMR_CLKSOURCE_EXTERNAL_SYNCHRONOUS;
                init.prescale           = TMR_PRESCALE_VALUE_1;
                init.interruptSource  = INT_SOURCE_TIMER_2;
                init.mode                = DRV_TMR_OPERATION_MODE_16_BIT;
                init.asyncWriteEnable = false;
                temp1_Obj = DRV_TMR_Initialize ( DRV_TMR_INDEX_2, (SYS_MODULE_INIT*)&init );
            }
            LATBbits.LATB3 = 0;
            if (appInitialized)
            {
                doorbellData.state = DOORBELL_STATE_SERVICE_TASKS;
     
            }
            break;
        }

        case DOORBELL_STATE_SERVICE_TASKS:
        {
            led_1Data.num = 1;
            led_1Data.toggle_status = false;
            led_1Data.userdata = NULL;
            led_1Handle = SYS_TMR_ObjectCreate ( 300, (uintptr_t)(&led_1Data), ledTimerCallback, SYS_TMR_FLAG_PERIODIC );
            
            doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
            break;
        }
        
        case DOORBELL_STATE_REQ_TEMP1: // Using LMT01
        {
            if(temp1_Obj == NULL) {
                doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
                break;
            }
            if(temp1_Handle != NULL) break;
            temp1_Handle = DRV_TMR_Open ( DRV_TMR_INDEX_2, DRV_IO_INTENT_EXCLUSIVE);
            if(temp1_Handle != NULL) {
                DRV_TMR_CounterClear(temp1_Handle);
                LATAbits.LATA2 = 1;
                //doorbellData.state = DOORBELL_STATE_READING_TEMP1;
                doorbellData.state = DOORBELL_STATE_DONE_TEMP1;
                delay_Handle = SYS_TMR_DelayMS(54 + 50);
            }
            break;
        }
        case DOORBELL_STATE_DONE_TEMP1: // Using LMT01
        {
            LATAbits.LATA2 = 0;
            // Record time_temp1, date_temp1;
            doorbellData.realdata.recent_temp1 = DRV_TMR_CounterValueGet(temp1_Handle);
            DRV_TMR_Close(temp1_Handle);
            temp1_Handle = NULL;
            // Calc MD5
            doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
        }
        /* TODO: implement your application state machine.*/
        case DOORBELL_STATE_WAIT_COMMAND:
        {
            asm("WAIT");
            break;
        }
        case DOORBELL_STATE_EXIT_SYSTEM: //Maybe Dummy.
        {
            SYS_TMR_ObjectDelete(led_1Handle);
            break;
        }
        /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

 

/*******************************************************************************
 End of File
 */
