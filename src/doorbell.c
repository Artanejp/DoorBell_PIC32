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
#define APP_READ_BUFFER_SIZE                    256
#define APP_WRITE_BUFFER_SIZE                   256
char UsbRdBuf[APP_READ_BUFFER_SIZE + 128];
char UsbWrBuf[APP_WRITE_BUFFER_SIZE + 128];

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
    doorbellData.bytesUartRead = 0;
    doorbellData.bytesUsbRead = 0;
    doorbellData.wrUartComplete = true;
    doorbellData.wrUsbComplete = true;
    doorbellData.rdUartComplete = true;
    doorbellData.rdUsbComplete = true;

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
}

// ToDo: Will Move
const uint16_t sound_level_table[32] = {
    0, 5, 10, 15, 20, 25, 30, 35,
    41, 47, 53, 60, 67, 74, 81, 88,
    95, 103, 111, 119, 127, 136, 145,
    154, 164, 174, 185, 196, 208, 220, 235
};
uint16_t sample_buffer[SOUND_LENGTH]; // 0.2sec

void UART_ReadComplete(void *handle)
{
    size_t *readSize = handle;
    doorbellData.bytesUartRead = *readSize;
    doorbellData.rdUartComplete = true;
}

void UART_WriteComplete(void *handle)
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

void USB_ReadComplete(void *handle)
{
    size_t *readSize = handle;
    doorbellData.bytesUsbRead = *readSize;
    doorbellData.rdUsbComplete = true;
}

void USB_WriteComplete(void *handle)
{
    if ((handle != UsbWrBuf) && (doorbellData.state == DOORBELL_STATE_USB_WRITE_TEST_1_WFC))
    {
          return;
    }
       else
    {
        doorbellData.wrUsbComplete = true;
    }
}

DOORBELL_TIMER_TICK_T led_1Data;
SYS_TMR_HANDLE led_1Handle;
SYS_TMR_HANDLE poll_Handle;

DRV_TEMP_LM01_T temp_lm01;

extern void ledTimerCallback(uintptr_t context, uint32_t currTick);
extern void pollCallback(uintptr_t context, uint32_t currTick);

void SLEEP_Periferals(void)
{
	// ToDo: Unlock PMDxbits.
    // A/D OFF
    PMD1bits.AD1MD = 1;
    PMD1bits.CTMUMD = 1;

    // Internal Comparator off
    PMD2bits.CMP1MD = 1;
    PMD2bits.CMP2MD = 1;
    PMD2bits.CMP3MD = 1;

    // Input Capture OFF
    PMD3bits.IC1MD = 1;
    PMD3bits.IC2MD = 1;
    PMD3bits.IC3MD = 1;
    PMD3bits.IC4MD = 1;
    PMD3bits.IC5MD = 1;

    // Output Compare 1, 3-5 OFF
    PMD3bits.OC1MD = 1;
    //PMD3bits.IC2MD = 1;
    PMD3bits.OC3MD = 1;
    PMD3bits.OC4MD = 1;
    PMD3bits.OC5MD = 1;

    // Timer4 OFF
    PMD4bits.T4MD = 1;

    // UART2 OFF
    PMD5bits.U2MD = 1;
    // I2C-2 OFF
    PMD5bits.I2C2MD = 1;
    // SPI OFF
    PMD5bits.SPI1MD = 1;
    PMD5bits.SPI2MD = 1;
    // PMP OFF
    PMD6bits.PMPMD = 1;
	// ToDo: Lock PMDxbits.
}

/******************************************************************************
  Function:
    void DOORBELL_Tasks ( void )

  Remarks:
    See prototype in doorbell.h.
 */

void DOORBELL_Tasks(void)
{
    volatile SYS_STATUS uartConsoleStatus;
    volatile SYS_STATUS usbConsoleStatus;
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
		if(doorbellData.bootparam_passthrough /* || debug mode */) {
			if (usbConsoleStatus == SYS_STATUS_READY) {
				SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, USB_ReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);
				SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, USB_WriteComplete, SYS_CONSOLE_EVENT_WRITE_COMPLETE);
			} else {
				appInitialized = false;
			}
		} else {
			// Disable USB and CONSOLE #1 (To Save power).
		}
        //poll_Handle = SYS_TMR_CallbackPeriodic(2000, (uintptr_t) 0, pollCallback);
        led_1Data.num = 1;
        led_1Data.toggle_status = false;
        led_1Data.userdata = NULL;
        doorbellData.wrUsbComplete = false;
        doorbellData.rdUsbComplete = false;
        doorbellData.wrUartComplete = false;
        doorbellData.rdUartComplete = false;

        if (appInitialized) {
			//if debug-mode.(RING + PASSTHROUGH)
			// Enable normal feature and accepting command from USB.
			// elseif ...
			if(doorbellData.bootparam_passthrough) {
				doorbellData.state = DOORBELL_STATE_READ_FROM_USB; // passthrough mode.
				led_1Handle = SYS_TMR_CallbackPeriodic(300, (uintptr_t) (&led_1Data), ledTimerCallback);
				
			} else {
				doorbellData.state = DOORBELL_STATE_SERVICE_TASKS;
			}
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
			doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
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
		if(doorbellData.bytesUartRead < APP_WRITE_BUFFER_SIZE) {
			_len = APP_WRITE_BUFFER_SIZE - doorbellData.bytesUartRead;
			_len = (_len >= 16) ? 16 : _len;
			doorbellData.bytesUartRead += SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, STDIN_FILENO,
														  &(UsbWrBuf[doorbellData.bytesUartRead]), _len);
		}
		if(doorbellData.bytesUartRead >= APP_WRITE_BUFFER_SIZE) { // Buffer FULL
			doorbellData.state = DOORBELL_STATE_WRITE_TO_USB;
		} else if(doorbellData.rdUartComplete) { // NO remain data exists.
			doorbellData.state = DOORBELL_STATE_WRITE_TO_USB;
			doorbellData.rdUartCompelte = false;
		} else { // Data remains
			doorbellData.state = DOORBELL_STATE_READ_FROM_USB;
		}
	break;
	// PASSTHROUGH
	case DOORBELL_STATE_READ_FROM_USB:
		if(doorbellData.bytesUsbRead < APP_READ_BUFFER_SIZE) {
			_len = APP_READ_BUFFER_SIZE - doorbellData.bytesUsbRead;
			_len = (_len >= 16) ? 16 : _len;
			doorbellData.bytesUsbRead += SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_1, STDIN_FILENO,
														  &(UsbRdBuf[doorbellData.bytesUsbRead]), _len);
		}
		if(doorbellData.bytesUsbRead >= APP_READ_BUFFER_SIZE) { // Buffer FULL
			doorbellData.state = DOORBELL_STATE_WRITE_TO_UART;
		} else if(doorbellData.rdUsbComplete) { // NO remain data exists.
			doorbellData.state = DOORBELL_STATE_WRITE_TO_UART;
			doorbellData.rdUsbCompelte = false;
		} else {  // Data remains
			doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
		}
	break;
    case DOORBELL_STATE_WRITE_TO_USB:
		if(doorbellData.wrUsbComplete) { // Ready to write.
			doorbellData.wrUsbComplete = false;
			_len = (doorbellData.bytesUartRead >= 16) ? 16 : doorbellData.bytesUartRead;
			if(_len > 0) {
				if(doorbellData.bytesUsbWrite < APP_WRITE_BUFFER_SIZE) {
					_len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_1, STDOUT_FILENO,
											 &(UsbWrBuf[doorbellData.bytesUsbWrite]), _len);
					doorbellData.bytesUsbWrite += _len;
					if(doorbellData.bytesUartRead >= _len) { // Check even size_t is not signed. 
						doorbellData.bytesUartRead -= _len;
						if(doorbellData.bytesUareRead == 0) { // No More Data.
							doorbellData.bytesUsbWrite = 0;  // reset write pointer.
							doorbellData.state = DOORBELL_STATE_READ_FROM_UART;  // End
						} else {
							//doorbellData.state = DOORBELL_STATE_READ_FROM_UART;  // Prefer to read from UART.
						}
					} else { // Buffer Underrun
						doorbellData.bytesUartRead = 0;
						doorbellData.bytesUsbWrite = 0;
						doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
					}
				} else { // Buffer Overrun : DISCARD DATA
					doorbellData.bytesUsbWrite = 0;
					doorbellData.bytesUartRead = 0;
					doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
				}
			} else {
				// No More Data.
				doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
			}
		} else {
			// Writing progress...
			//doorbellData.state = DOORBELL_STATE_READ_FROM_UART;
		}
		break;
    case DOORBELL_STATE_WRITE_TO_UART:
		if(doorbellData.wrUartComplete) { // Ready to write.
			doorbellData.wrUartComplete = false;
			_len = (doorbellData.bytesUsbRead >= 16) ? 16 : doorbellData.bytesUsbRead;
			if(_len > 0) {
				if(doorbellData.bytesUartWrite < APP_READ_BUFFER_SIZE) {
					_len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO,
											 &(UsbRdBuf[doorbellData.bytesUartWrite]), _len);
					doorbellData.bytesUartWrite += _len;
					if(doorbellData.bytesUsbRead >= _len) { // Check even size_t is not signed. 
						doorbellData.bytesUsbRead -= _len;
						if(doorbellData.bytesUsbRead == 0) { // No More Data.
							doorbellData.bytesUartWrite = 0;  // reset write pointer.
							doorbellData.state = DOORBELL_STATE_READ_FROM_USB;  // End
						} else {
							//doorbellData.state = DOORBELL_STATE_READ_FROM_USB;  // Prefer to read from UART.
						}
					} else { // Buffer Underrun
						doorbellData.bytesUsbRead = 0;
						doorbellData.bytesUartWrite = 0;
						doorbellData.state = DOORBELL_STATE_READ_FROM_USB;
					}
				} else { // Buffer Overrun : DISCARD DATA
					doorbellData.bytesUartWrite = 0;
					doorbellData.bytesUsbRead = 0;
					doorbellData.state = DOORBELL_STATE_READ_FROM_USB;
				}
			} else {
				// No More Data.
				doorbellData.state = DOORBELL_STATE_READ_FROM_USB;
			}
		} else {
			// Writing progress...
			//doorbellData.state = DOORBELL_STATE_READ_FROM_USB;
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
            doorbellData.state = DOORBELL_STATE_WAIT_COMMAND;
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
