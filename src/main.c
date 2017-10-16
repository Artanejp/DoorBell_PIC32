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
#include "queue.h"
#include "timers.h"

/* Standard demo includes. */
//#include "partest.h"

/* Hardware specific includes. */
//#include "ConfigPerformance.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************


DOORBELL_DATA doorbellData;

RESET_REASON prvSetupHardware(void)
{
    uint32_t _time, _date;
    RESET_REASON reset_reason;
	
    PLIB_RTCC_Enable(RTCC_ID_0);
    reset_reason = PLIB_RESET_ReasonGet(RESET_ID_0);
    _time = PLIB_RTCC_RTCTimeGet(RTCC_ID_0);
    _date = PLIB_RTCC_RTCDateGet(RTCC_ID_0);
    SYS_Initialize(NULL);
	
    switch(reset_reason) {
    case RESET_REASON_POWERON:
    //case RESET_REASON_VBAT:
        break;
    default:
        PLIB_RTCC_RTCTimeSet(RTCC_ID_0, _time);
        PLIB_RTCC_RTCDateSet(RTCC_ID_0, _date);
        break;
    }
    // Reset RTC value if POWERON, restore values if not.
    //PLIB_RESET_ReasonClear(RESET_ID_0, reset_reason);
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, false); // Set LED OFF
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, 0, false); // Turn temp-seosor off.
	
    U1RXRbits.U1RXR = 0x03; // RPB13 = U1RX
    RPB15Rbits.RPB15R = 0x01; //RPB15 = U1TX
	return reset_reason;
}

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

    if (!CHECK_MD5Sum()) {
        memset(&(doorbellData.realdata), 0x00, sizeof (doorbellData.realdata));
        // Re-Calc MD5.
        CALC_MD5Sum();
    }
}

SYS_RTCC_ALARM_HANDLE *hAlarmTick;
TaskHandle_t xHandleHouseKeeping;

TaskHandle_t xHandleReadFromUART;
TaskHandle_t xHandleReadFromUSB;
TaskHandle_t xHandleWriteToUART;
TaskHandle_t xHandleWriteToUSB;
TaskHandle_t xHandleLED;
TaskHandle_t xHandleSoundRender;

QueueHandle_t xUartRecvQueue;
QueueHandle_t xUartSendQueue;
QueueHandle_t xUsbRecvQueue;
QueueHandle_t xUsbSendQueue;

extern void prvHouseKeeping(void *pvParameters);

uint32_t cTick100ms;
uint32_t cTick110ms;
uint32_t cTick200ms;
uint32_t cTick500ms;
uint32_t cTick1Sec;
uint32_t cTick5Sec;

extern void prvReadFromUsb(void *pvparameters);
extern void prvWriteToUsb(void *pvparameters);
extern void prvReadFromUart(void *pvparameters);
extern void prvWriteToUart(void *pvparameters);
extern void prvHouseKeeping(void *pvParameters);
extern void prvWriteToUart_HK(void *pvparameters);
extern void prvReadFromUart_HK(void *pvparameters);

void setupTicks(void)
{
	cTick100ms = (uint32_t)((10000 / portTICK_PERIOD_MS) / 100); 
	cTick110ms = (uint32_t)((11000 / portTICK_PERIOD_MS) / 100);
	cTick200ms = (uint32_t)((20000 / portTICK_PERIOD_MS) / 100); 
	cTick500ms = (uint32_t)((50000 / portTICK_PERIOD_MS) / 100); 
	cTick1Sec = (uint32_t)((10000 / portTICK_PERIOD_MS) / 10); 
	cTick5Sec = (uint32_t)((50000 / portTICK_PERIOD_MS) / 10);
	
}

int main ( void )
{
	TimerHandle_t xTimer = NULL;
	RESET_REASON reason = prvSetupHardware();
	bool passthrough;
#if 0
	vCreateBlockTimeTasks();
	vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
	vStartGenericQueueTasks( mainGEN_QUEUE_TASK_PRIORITY );
	vStartQueuePeekTasks();
	vStartInterruptQueueTasks();
#endif	
    if (!SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_B, 5)) {
        // IF S_MAINTAIN is LOW, PASSTHROUGH
        passthrough = true;
    } else {
        passthrough = false;
    }
	xUartRecvQueue = NULL;
	xUartSendQueue = NULL;
	xUsbRecvQueue = NULL;
	xUsbSendQueue = NULL;
	
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, true); // Set LED ON
	if(!passthrough) {
		// FULL
		setupTicks();
		xUartRecvQueue = xQueueCreate(16, sizeof(char));
		xUartSendQueue = xQueueCreate(16, sizeof(char));
		xTaskCreate( prvWriteToUart_HK,   "WriteToUart",  256, NULL, tskIDLE_PRIORITY + 2, &xHandleWriteToUART );
		xTaskCreate( prvReadFromUart_HK,   "ReadFromUart",  256, NULL, tskIDLE_PRIORITY + 4, &xHandleReadFromUART );
	
		//xTaskCreate( prvLEDs, "LEDs", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 0, &xHandleLED );
		//xTaskCreate( prvRenderThread, "Render&SOUND", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 4, &xHandleSoundRenderw );
		//xTaskCreate( prvHouseKeeping, "HouseKeeping", 512, NULL, tskIDLE_PRIORITY + 1, &xHandleHouseKeeping );
		//xTaskCreate( prvHouseKeeping, "HouseKeeping", 800, NULL, 1, &xHandleHouseKeeping );
                } else {
		setupTicks();
		xUsbRecvQueue = xQueueCreate(128, sizeof(char));
		xUsbSendQueue = xQueueCreate(256, sizeof(char));
		xTaskCreate( prvReadFromUsb,  "ReadFromUsb",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xHandleReadFromUSB );
		xTaskCreate( prvWriteToUsb,   "WriteToUsb",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &xHandleWriteToUSB );
		//xTaskCreate( prvLEDs, "LEDs", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 0, &xHandleLED );
	}		

	/* A software timer is also used to start the high frequency timer test.
	This is to ensure the test does not start before the kernel.  This time a
	one shot software timer is used. */
	//xTimer = xTimerCreate( "HighHzTimerSetup", 1, pdFALSE, ( void * ) 0, prvSetupHighFrequencyTimerTest );
	//if( xTimer != NULL )
	//{
	//	xTimerStart( xTimer, mainDONT_BLOCK );
	//}
	/* Finally start the scheduler. */
	//vTaskStartScheduler();
	setupTicks();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	SYS_Tasks();
	
	//for( ;; );
    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/
