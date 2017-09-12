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

#include "lm01_drv.h"
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

/* Fulfill USB DMA transfer criteria */
#define APP_READ_BUFFER_SIZE                    64
#define APP_WRITE_BUFFER_SIZE                   64
char UartRdBuf[DRV_USART_RCV_QUEUE_SIZE_IDX0];
char UartWrBuf[DRV_USART_XMIT_QUEUE_SIZE_IDX0];
char UsbRdBuf[APP_READ_BUFFER_SIZE];
char UsbWrBuf[APP_WRITE_BUFFER_SIZE];

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

void DOORBELL_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    doorbellData.state = DOORBELL_STATE_INIT;


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    
}

void UART_ReadComplete (void *handle)
{
    size_t *readSize = handle;
    doorbellData.bytesUartRead = *readSize;
    doorbellData.rdUartComplete = true;
}

void UART_WriteComplete (void *handle)
{
//    if ((handle != UartWrBuf) && (appData.state == APP_STATE_WRITE_TEST_2_WFC))
//    {
//        return;
//    }
//    else
    {
        doorbellData.wrUartComplete = true;
    }
}

void USB_ReadComplete (void *handle)
{
    size_t *readSize = handle;
    doorbellData.bytesUsbRead = *readSize;
    doorbellData.rdUsbComplete = true;
}

void USB_WriteComplete (void *handle)
{
//    if ((handle != UsbWrBuf) && (appData.state == APP_STATE_WRITE_TEST_2_WFC))
//    {
//        return;
//    }
//   else
    {
        doorbellData.wrUsbComplete = true;
    }
}

DOORBELL_TIMER_TICK_T led_1Data;
SYS_TMR_HANDLE led_1Handle;

DRV_TEMP_LM01_T temp_lm01;

extern void ledTimerCallback(uintptr_t context, uint32_t currTick);

/******************************************************************************
  Function:
    void DOORBELL_Tasks ( void )

  Remarks:
    See prototype in doorbell.h.
 */

void DOORBELL_Tasks(void)
{
    SYS_STATUS uartConsoleStatus;
    SYS_STATUS usbConsoleStatus;
    uartConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole0); 
    usbConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole1); 
#if 0
     //Do not proceed in the current app state unless the console is ready
    if (uartConsoleStatus != SYS_STATUS_READY)
    {
        if (uartConsoleStatus == SYS_STATUS_ERROR)
        {            
            //if (doorbellData.state == APP_STATE_WRITE_TEST_3)       
            //{
            //    DoorBell_Reset();                                       
            //    SYS_CONSOLE_Flush(SYS_CONSOLE_INDEX_0);            
            //    SYS_MESSAGE("\n\r\n\rWrite Queue Overflowed! Flushed console.\n\r\n\r");
            //    doorbellData.state = APP_STATE_READ_TEST_1;
            //}
        }
        return;
    }
    if (usbConsoleStatus != SYS_STATUS_READY)
    {
        if (usbConsoleStatus == SYS_STATUS_ERROR)
        {            
            //if (doorbellData.state == APP_STATE_WRITE_TEST_3)       
            //{
            //    DoorBell_Reset();                                       
            //    SYS_CONSOLE_Flush(SYS_CONSOLE_INDEX_0);            
            //    SYS_MESSAGE("\n\r\n\rWrite Queue Overflowed! Flushed console.\n\r\n\r");
            //    doorbellData.state = APP_STATE_READ_TEST_1;
            //}
        }
        return;
    }
#endif

    /* Check the application's current state. */
    switch (doorbellData.state) {
        /* Application's initial state. */
    case DOORBELL_STATE_INIT:
    {
        bool appInitialized = true;
        LATBbits.LATB3 = 0;
        SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, UART_ReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);
        SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, UART_WriteComplete, SYS_CONSOLE_EVENT_WRITE_COMPLETE);
        SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, USB_ReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);
        SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, USB_WriteComplete, SYS_CONSOLE_EVENT_WRITE_COMPLETE);

        if (appInitialized) {
            doorbellData.state = DOORBELL_STATE_SERVICE_TASKS;

        }
        break;
    }
    break;
    case DOORBELL_STATE_SERVICE_TASKS:
    {
        led_1Data.num = 1;
        led_1Data.toggle_status = false;
        led_1Data.userdata = NULL;
        led_1Handle = SYS_TMR_CallbackPeriodic(300, (uintptr_t) (&led_1Data), ledTimerCallback);

        //DRV_TEMP_LM01_Init(&temp_lm01, NULL);

        doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
        break;
    }
        break;
    case DOORBELL_STATE_REQ_TEMP1: // Using LMT01
    {
        DRV_TEMP_LM01_StartConversion(temp_lm01);
        doorbellData.state = DOORBELL_STATE_DONE_TEMP1;
        break;
    }
        break;
    case DOORBELL_STATE_DONE_TEMP1: // Using LMT01
    {
        doorbellData.realdata.recent_temp1 = DRV_TEMP_LM01_EndConversion(temp_lm01);
        doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
        // Calc MD5

        break;
    }
    break;
        /* TODO: implement your application state machine.*/
    case DOORBELL_STATE_WAIT_COMMAND:
    {
        asm("WAIT");
        break;
    }
    break;
    case DOORBELL_STATE_USBTOUART:
    {
        ssize_t _len, _len2;
        char *p = UsbRdBuf;
        _len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_1, STDIN_FILENO, UsbRdBuf, APP_READ_BUFFER_SIZE);
        // ToDo: TAP to internal.
        for(;_len > 0;) {
            _len2 = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, p, APP_READ_BUFFER_SIZE);
            if(_len2 >= APP_READ_BUFFER_SIZE) _len = -1;
            if(_len2 >= 0) _len = _len - _len2;
        }
        doorbellData.state = DOORBELL_STATE_UARTTOUSB;
        break;
    }
    break;
    case DOORBELL_STATE_UARTTOUSB:
    {
        ssize_t _len, _len2;
        char *p = UsbWrBuf;
        _len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, STDIN_FILENO, UsbRdBuf, APP_WRITE_BUFFER_SIZE);
        // ToDo: TAP to internal.
        for(;_len > 0;) {
            _len2 = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_1, STDOUT_FILENO, p, APP_WRITE_BUFFER_SIZE);
            if(_len2 >= APP_WRITE_BUFFER_SIZE) _len = -1;
            if(_len2 >= 0) _len = _len - _len2;
        }
        //ToDo:  Break reason
        doorbellData.state = DOORBELL_STATE_USBTOUART;
        break;
    }
    break;
    case DOORBELL_STATE_EXIT_SYSTEM: //Maybe Dummy.
    {
        SYS_TMR_ObjectDelete(led_1Handle);
        break;
    }
    break;
        /* The default state should never be executed. */
    default:
    {
        /* TODO: Handle error in application's state machine. */
        break;
    }
    break;
    }
}



/*******************************************************************************
 End of File
 */
