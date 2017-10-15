
/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    passthrough.c

  Summary:
    Pass through via UART and USB task(s).

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
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

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"
#include "doorbell.h"   // SYS function prototypes


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Standard demo includes. */
#include "partest.h"

/* Hardware specific includes. */
#include "ConfigPerformance.h"

static char rdUsbBuf[128];
static char wrUsbBuf[128];
static char rdTmpUsbBuf[32];
static char wrTmpUsbBuf[32];
static char *rdUsbPtr;
static char *wrUsbPtr;
static ssize_t rdUsbSize;
static ssize_t wrUsbSize;
const ssize_t rdUsbSizeLimit = 128;
const ssize_t wrUsbSizeLimit = 128;

static SemaphoreHandle_t xUsbRdSemaphore;
static SemaphoreHandle_t xUsbWrSemaphore;

static consoleCallbackFunction cbUsbWriteReadComplete(void *handle)
{
	SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, false); // Set LED OFF
}

static consoleCallbackFunction cbUsbReadComplete(void *handle)
{
	char *p = rdTmpUsbBuf;
	ssize_t size = (ssize_t)handle;

	if(size >= 0) {
		rdUsbSize += size;
		if(rdUsbSize <= rdUsbSizeLimit) {
			if(rdUsbPtr != NULL) {
				memcpy(rdUsbPtr, p, size);
				rdUsbPtr += size;
			}
		}
	}
	if(xUsbRdSemaphore != NULL) xSemaphoreGive(xUsbRdSemaphore);
}

void prvReadFromUsb(void)
{
    volatile SYS_STATUS usbConsoleStatus;
	ssize_t _len;
	bool before_attached = true;
	
	rdUsbSize = 0;
	rdUsbPtr = NULL;
	xUsbRdSemaphore = xSemaphoreCreateBinary();
	if(xUsbRdSemaphore == NULL) return;

	SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, cbUsbReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);

	rdUsbSize = 0;
	rdUsbPtr = rdUsbBuf;

	while(1) {
		usbConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole0);
		if(usbConsoleStatus >= SYS_STATUS_READY) {
			if(rdUsbSize > 0) {
				if((rdUsbBuf[rdUsbSize - 1] == '\n') || (rdUsbSize >= rdUsbSizeLimit)) {
					_len = rdUsbSize;
					_len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_1, STDOUT_FILENO, rdUsbBuf, _len);
					rdUsbPtr = rdUsbBuf;
					rdUsbSize = 0;
				}
			}
			before_attached = true;
			_len = 1;
			_len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, STDIN_FILENO, rdTmpUsbBuf, _len);
			while((xSemaphoreTake(xUsbRdSemaphore, cDelay1Sec) != pdPASS) && (usbConsoleStatus >= SYS_STATUS_READY)) {
					usbConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole0);
			}
		} else {
			if(before_attached) {
				rdUsbSize = 0;
				rdUsbPtr = rdUsbBuf;
				xSemaphoreGive(xUsbRdSemaphore);
				before_attached = false;
			} else {
				vDelayTask(cDelay1Sec);
			}
		}
	}
}


static consoleCallbackFunction cbUartReadComplete(void *handle)
{
	char *p = wrTmpUsbBuf;
	ssize_t size = (ssize_t)handle;

	if(size >= 0) {
		wrUsbSize += size;
		if(wrUsbSize <= wrUsbSizeLimit) {
			if(wrUsbPtr != NULL) {
				memcpy(wrUsbPtr, p, size);
				wrUsbPtr += size;
			}
		}
	}
	if(xUsbWrSemaphore != NULL) xSemaphoreGive(xUsbWrSemaphore);
}


void prvWriteToUsb(void)
{
    volatile SYS_STATUS usbConsoleStatus;
	ssize_t _len;
	bool before_attached = true;
	
	wrUsbSize = 0;
	wrUsbPtr = NULL;
	xUsbWrSemaphore = xSemaphoreCreateBinary();
	if(xUsbWrSemaphore == NULL) return;

	SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, cbUartReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);
	SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_0, cbUsbWriteComplete, SYS_CONSOLE_EVENT_WRITE_COMPLETE);

	wrUsbSize = 0;
	wrUsbPtr = wrUsbBuf;
	SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, false); // Set LED OFF
	while(1) {
		_len = 1;
		_len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_1, STDIN_FILENO, wrTmpUsbPtr, _len);
		while(xSemaphoreTake(xUsbWrSemaphore, cDelay1Sec) != pdPASS) {}
		if(wrUsbSize > 0) {
			SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, true); // Set LED ON
			if((wrUsbBuf[wrUsbSize - 1] == '\n') || (wrUsbSize >= wrUsbSizeLimit)) {
				_len = wrUsbSize;
				usbConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole0);
				if(usbConsoleStatus >= SYS_STATUS_READY) {
					_len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, wrUsbBuf, _len);
				}
				wrUsbPtr = wrUsbBuf;
				wrUsbSize = 0;
			}
		} else {
			wrUsbPtr = wrUsbBuf;
			wrUsbSize = 0;
		}
	}
}



