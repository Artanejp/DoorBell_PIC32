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
#include "system_definitions.h"
#include "system_config.h"
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
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    uint8_t data_md5sum[MD5_DIGEST_SIZE];
    RESET_REASON reason;
    reason = SYS_RESET_ReasonGet();
    switch (reason) {
    case RESET_REASON_POWERON:
    case RESET_REASON_CONFIG_MISMATCH:
        memset(&doorbellData, 0x00, sizeof (doorbellData));
        break;
    default:
        // Check MD5SUM
        // If failed, clear.
        break;
    }
    doorbellData.ringed = false;
    doorbellData.uart_ready = true;
	doorbellData.usb_ready = false;

    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, false); // Set LED OFF
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, 0, false); // Turn temp-seosor off.
    if (!CHECK_MD5Sum()) {
        memset(&(doorbellData.realdata), 0x00, sizeof (doorbellData.realdata));
        // Re-Calc MD5.
        CALC_MD5Sum();
    }
#if 1
    if (!SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_B, 5)) {
        // IF S_MAINTAIN is LOW, PASSTHROUGH
        doorbellData.bootparam_passthrough = true;
    } else {
        doorbellData.bootparam_passthrough = false;
    }
#endif
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, true); // Set LED ON
    doorbellData.state = DOORBELL_STATE_INIT;
    U1RXRbits.U1RXR = 0x03; // RPB13 = U1RX
    RPB15Rbits.RPB15R = 0x01; //RPB15 = U1TX
}

// ToDo: Will Move
const uint16_t sound_level_table[32] = {
    0, 5, 10, 15, 20, 25, 30, 35,
    41, 47, 53, 60, 67, 74, 81, 88,
    95, 103, 111, 119, 127, 136, 145,
    154, 164, 174, 185, 196, 208, 220, 235
};
uint16_t sample_buffer[SOUND_LENGTH]; // 0.2sec


extern void ledTimerCallback(uintptr_t context, uint32_t currTick);
extern void pollCallback(uintptr_t context, uint32_t currTick);

void SLEEP_Periferals(bool onoff)
{
	int n = (onoff) ? 1 : 0; // OFF = TRUE
    // ToDo: Unlock PMDxbits.
    // A/D OFF
    PMD1bits.AD1MD = n;
    PMD1bits.CTMUMD = n;

    // Internal Comparator off
    PMD2bits.CMP1MD = n;
    PMD2bits.CMP2MD = n;
    PMD2bits.CMP3MD = n;

    // Input Capture OFF
    PMD3bits.IC1MD = n;
    PMD3bits.IC2MD = n;
    PMD3bits.IC3MD = n;
    PMD3bits.IC4MD = n;
    PMD3bits.IC5MD = n;

    // Output Compare 1, 3-5 OFF
    PMD3bits.OC1MD = n;
    //PMD3bits.IC2MD = n;
    PMD3bits.OC3MD = n;
    PMD3bits.OC4MD = n;
    PMD3bits.OC5MD = n;

    // Timer4 OFF
    PMD4bits.T4MD = n;

    // UART2 OFF
    PMD5bits.U2MD = n;
    // I2C-2 OFF
    PMD5bits.I2C2MD = n;
    // SPI OFF
    PMD5bits.SPI1MD = n;
    PMD5bits.SPI2MD = n;
    // PMP OFF
    PMD6bits.PMPMD = n;
    // ToDo: Lock PMDxbits.
}

void TWE_Wakeup(bool onoff)
{
	// Enter critical
	SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, onoff); // ON/OFF LED
	// Leave critical
	// ON/OFF TWE Module
	vTaskDelay(cTick100ms);
}
/******************************************************************************
  Function:
    void DOORBELL_Tasks ( void )

  Remarks:
    See prototype in doorbell.h.
 */

extern void prvHouseKeeping(void *pvParameters);
void DOORBELL_Tasks(void)
{
    prvHouseKeeping((void *)(&doorbellData));
#if 0
    volatile SYS_STATUS uartConsoleStatus;
    volatile SYS_STATUS usbConsoleStatus;
    size_t _len;
    uartConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole0);
    usbConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole1);
    /* Check the application's current state. */
    switch (doorbellData.state) {
        /* Application's initial state. */
    case DOORBELL_STATE_INIT:
    {
        bool appInitialized = true;

        LATBbits.LATB3 = 0;
        // ToDo: Turn ON TWE-LITE.
        if (uartConsoleStatus == SYS_STATUS_READY) {
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, UART_ReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);
            SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, UART_WriteComplete, SYS_CONSOLE_EVENT_WRITE_COMPLETE);
        } else {
            appInitialized = false;
        }
        //if(doorbellData.bootparam_passthrough /* || debug mode */) {
        //if (usbConsoleStatus == SYS_STATUS_READY) {
        SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, USB_ReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);
        SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, USB_WriteComplete, SYS_CONSOLE_EVENT_WRITE_COMPLETE);
        //} else {
        //	appInitialized = false;
        //}
        //} else {
        //	// Disable USB and CONSOLE #1 (To Save power).
        //}
        //poll_Handle = SYS_TMR_CallbackPeriodic(2000, (uintptr_t) 0, pollCallback);
        led_1Data.num = 1;
        led_1Data.toggle_status = false;
        led_1Data.userdata = NULL;
        doorbellData.wrUsbComplete = false;
        doorbellData.rdUsbComplete = true;
        doorbellData.wrUartComplete = false;
        doorbellData.rdUartComplete = true;

        if (appInitialized) {
            //if debug-mode.(RING + PASSTHROUGH)
            // Enable normal feature and accepting command from USB.
            // elseif ...
            //if(doorbellData.bootparam_passthrough) {
            //	doorbellData.state = DOORBELL_STATE_READ_FROM_USB; // passthrough mode.
            led_1Handle = SYS_TMR_CallbackPeriodic(300, (uintptr_t) (&led_1Data), ledTimerCallback);

            //} else {
            doorbellData.state = DOORBELL_STATE_SERVICE_TASKS;
            //}
        }
        SLEEP_Periferals();
        DRV_TEMP_LM01_Init(&temp_lm01, NULL);
    }
        break;
    case DOORBELL_STATE_SERVICE_TASKS:
    {
        // ToDo: Print starting message and initial logs.
        // ToDo: First measuring tempareture.
        // ToDo: Check and print RTC value.
        // ToDo: Waiting for Time data from host at 30 sec?.
        // ToDo: If passed 30sec ? without time-reply from host, continue.
        // ToDo: If received time from host, continue immediatry.
        // Set Wakeup Timer (and callback)
        // Set Interrupt callback and permissions.
        // Stop timer (until ringing or debug).
        // ToDo: Suspend TWE-LITE.
        // Go to wait.
        //doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
        doorbellData.state = DOORBELL_STATE_USB_WRITE_TEST_1;
        doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
    }
        break;
        // ToDo: Ring state.
        // ToDo: Rendering sound.(DMA driven).
        // ToDo: Sound end (DMA driven).
        // ToDo: Check clock drift.
        // ToDo: Adjust clock drift.
        // ToDo: Print time.
        // ToDo: Command : Set time via TWE-LITE.
        // ToDo: Command : Set time from USB.
        // ToDo: Command : Print log via TWE-LITE.
        // ToDo: Command : Manage log via TWE-LITE.
        // ToDo: Command : Upload MML.
        // ToDo: Command : Manage MMLs.
        // ToDo: Command : Test Sound.
        // ToDo: Command : Set housekeeping interval.
        // ToDo: Force wakeup by pressing debug button.
        // ToDo: Check power status (really?)
        // ToDo: Save buffered log to EEPROM via I2C (when housekeeping).
        // ToDo: Check saved log in EEPROM.
        // ToDo: Implement I/O expander via I2C.
        // ToDo: Implement RAM keeping mode (lower batteries).
        // ToDo: Dose clock mode.(With adjusting periferal clock).
        // ToDo: Boost clock mode.(With adjusting periferal clock).
        // ToDo: Standard clock mode.(With adjusting periferal clock).
        // ToDo: Implement CRC16-CCICT to protect LOG and URL.
    case DOORBELL_STATE_REQ_TEMP1: // Using LMT01
        DRV_TEMP_LM01_StartConversion(&temp_lm01);
        doorbellData.state = DOORBELL_STATE_DONE_TEMP1;
        break;
    case DOORBELL_STATE_DONE_TEMP1: // Using LMT01
        doorbellData.realdata.recent_temp1 = DRV_TEMP_LM01_EndConversion(&temp_lm01);
        doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
        // Calc MD5
        CALC_MD5Sum();
        break;
        /* TODO: implement your application state machine.*/
    case DOORBELL_STATE_READ_FROM_UART:
        if (doorbellData.rdUartComplete) {
            doorbellData.rdUartComplete = false;
            if (doorbellData.UartRdPtr < APP_WRITE_BUFFER_SIZE) {
                _len = 32;
                _len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, STDIN_FILENO,
                        &(UsbWrBuf[doorbellData.UartRdPtr]), _len);
            } else {
                // Discard Buffer
                doorbellData.bytesUartRead = 0;
                doorbellData.UartRdPtr = 0;
            } 
                doorbellData.state = DOORBELL_STATE_WRITE_TO_USB; // Next
        } else {
            doorbellData.state = DOORBELL_STATE_READ_FROM_USB; // Nex
        }
       doorbellData.state = DOORBELL_STATE_READ_FROM_UART; // Next
        break;
        // PASSTHROUGH
    case DOORBELL_STATE_READ_FROM_USB:
        if ((doorbellData.rdUsbComplete) && (usbConsoleStatus != SYS_CONSOLE_STATUS_NOT_CONFIGURED)) {
            doorbellData.rdUsbComplete = false;
            if (doorbellData.UsbRdPtr < APP_WRITE_BUFFER_SIZE) {
               _len = 1;
               _len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_1, STDIN_FILENO,
                        &(UsbRdBuf[doorbellData.UsbRdPtr]), _len);
            }
                            doorbellData.state = DOORBELL_STATE_WRITE_TO_UART; // Next
        } else {
           doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
        }
       doorbellData.state = DOORBELL_STATE_WRITE_TO_USB; // Next
        break;
    case DOORBELL_STATE_WRITE_TO_USB:
        if(usbConsoleStatus != SYS_CONSOLE_STATUS_NOT_CONFIGURED) {
        if(!doorbellData.wrUsbComplete) {
            _len = (doorbellData.bytesUartRead >= APP_READ_BUFFER_SIZE) ? APP_READ_BUFFER_SIZE : doorbellData.bytesUartRead;
            if((_len > 0) && ((doorbellData.UartRdPtr +1) > doorbellData.UsbWrPtr)) {
                _len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_1, STDOUT_FILENO, &(UsbWrBuf[doorbellData.UsbWrPtr]), _len);
                doorbellData.UsbWrPtr += _len;
            }
        } else {
            doorbellData.wrUsbComplete = false;
            doorbellData.UartRdPtr = 0;
            doorbellData.UsbWrPtr = 0;
            doorbellData.bytesUartRead = 0;
        }
        }
        doorbellData.state = DOORBELL_STATE_READ_FROM_UART; // Nex
        break;
    case DOORBELL_STATE_WRITE_TO_UART:
        if(!doorbellData.wrUartComplete) {
            _len = (doorbellData.bytesUsbRead >= APP_READ_BUFFER_SIZE) ? APP_READ_BUFFER_SIZE : doorbellData.bytesUsbRead;
            if((_len > 0) && ((doorbellData.UsbRdPtr + 1) > doorbellData.UartWrPtr)){
                _len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, &(UsbRdBuf[doorbellData.UartWrPtr]), _len);
                doorbellData.UartWrPtr += _len;
            }
        } else {
            doorbellData.wrUartComplete = false;
            doorbellData.UsbRdPtr = 0;
            doorbellData.UartWrPtr = 0;
            doorbellData.bytesUsbRead = 0;
        }
       doorbellData.state = DOORBELL_STATE_READ_FROM_UART; // Nex
        break;
    case DOORBELL_STATE_PASSTHROUGH_NEXT_TURN:
        if (doorbellData.bytesUartRead <= 0) {
            doorbellData.bytesUartRead = 0;
            doorbellData.UartRdPtr = 0;
            doorbellData.UsbWrPtr = 0;
        }
        //if (doorbellData.rdUsbComplete) {
        //    doorbellData.rdUsbComplete = false;
        //}
        if (doorbellData.bytesUsbRead <= 0) {
            doorbellData.bytesUsbRead = 0;
            doorbellData.UsbRdPtr = 0;
            doorbellData.UartWrPtr = 0;
        }
        if(doorbellData.rdUartComplete) {
            doorbellData.state = DOORBELL_STATE_WRITE_TO_USB;
        } else if(doorbellData.rdUsbComplete) {
            doorbellData.state = DOORBELL_STATE_WRITE_TO_UART;
        } else {
            doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
        }
        break;
    case DOORBELL_STATE_WAIT_COMMAND:
    {
        // Sleep command.
        //asm("WAIT");
    }
        break;
        // Bolows are  testing command. Deprecated.
    case DOORBELL_STATE_USB_WRITE_TEST_1:
        /* Demonstrates polling write method */
        //if (!doorbellData.wrUsbComplete) 
        // if (usbConsoleStatus == SYS_STATUS_READY) {
    {
        int i;
        for (i = 0; i < 5; i++) {
            UsbWrBuf[i] = '*';
        }
        UsbWrBuf[i] = '\r';
        UsbWrBuf[i + 1] = '\n';
        UsbWrBuf[i + 2] = '\0';
        SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_1, STDOUT_FILENO, UsbWrBuf, strlen(UsbWrBuf));
        doorbellData.state = DOORBELL_STATE_USB_WRITE_TEST_1_WFC;
    }
        break;
    case DOORBELL_STATE_USB_WRITE_TEST_1_WFC:
        if (SYS_CONSOLE_Status(sysObj.sysConsole1) == SYS_STATUS_READY) {
            //doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
            doorbellData.state = DOORBELL_STATE_USB_WRITE_TEST_1;
            SYS_TMR_DelayMS(1000);
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
#endif
}



/*******************************************************************************
 End of File
 */
