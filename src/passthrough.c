
/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

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

/* Standard demo includes. */
#include "partest.h"

/* Hardware specific includes. */
#include "ConfigPerformance.h"


void prvReadFromUsb(void)
{
    volatile SYS_STATUS usbConsoleStatus;
	char nc[32];
	ssize_t _len;
	while(1) {
		usbConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole0);
		if(usbConsoleStatus == SYS_STATUS_READY) {
			_len = 1;
			_len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, STDIN_FILENO, nc, _len);
			if(_len > 0) {
				_len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_1, STDOUT_FILENO, nc, _len);
			}
		}
	}
}

void prvWriteToUsb(void)
{
    volatile SYS_STATUS usbConsoleStatus;
	char nc[32];
	ssize_t _len;
	while(1) {
		_len = 1;
		_len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_1, STDIN_FILENO, nc, _len);
		if(_len > 0) {
			usbConsoleStatus = SYS_CONSOLE_Status(sysObj.sysConsole0);
			if(usbConsoleStatus == SYS_STATUS_READY) {
				_len = SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, nc, _len);
			}
		}
	}
}


