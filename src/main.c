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
    SYS_Initialize ( NULL );
	
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
    doorbellData.bytesUartRead = 0;
    doorbellData.bytesUsbRead = 0;
    doorbellData.UartRdPtr = 0;
    doorbellData.UartWrPtr = 0;
    doorbellData.UsbRdPtr = 0;
    doorbellData.UsbWrPtr = 0;
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
    U1RXRbits.U1RXR = 0x03; // RPB13 = U1RX
    RPB15Rbits.RPB15R = 0x01; //RPB15 = U1TX
}

bool vShellMain(char *tmpdata)
{
	bool f_quit;
	char cmdhead;
	if(tmpdata == NULL) return false;

	cmdhead = tmpdata[0];
	// parse string
	f_quit = false;
	switch(cmdhead) {
	case 'q':
		f_quit = true;
		break;
	case 't':
		// sanitize time/data string.
		// ts: set time
		// tr: read time
		// If okay,
		//vRTCTimeSet(&(tmpdata[1]));
		break;
	case 'g':
		// Get Time
		// arg1=YYYYMMDDhhmmss.sss
		break;
	case 'p':
		// Test sound arg1 = num
		// If don't set, play default.
		break;
	case 's':
		// Stop sound.
		break;
	case 'm':
		// manage MML
		// arg0 = ms : Set number of MML
		//        md : Delete number of MML.
		//        mu : Upload MML
		//             arg1 = num;
		//             if prompt(MML>) displayed, you can push MML.
		break;
	case 'l':
		// Load log
		// l/lu : load unsent log.
		// la   : load all log.
		// lcc  : Clear chalacter logs.
		// lcb  : Clear binary logs.
		// ln   : print number of logs.
		break;
	case 'v':
		// Read temp sensors
		// arg1 = num.
		break;
	case 'x':
		//  Read/Set switch value.
		break;
	default:
		break;
	}
	return f_quit;
}

#define LMT01_SENSOR_NUM 1

static char pStrBufHK[128];
SYS_RTCC_ALARM_HANDLE *hAlarmTick;
TaskHandle_t xHandleHouseKeeping;

static void prvHouseKeeping(void *pvParameters)
{
	bool first = true;
	bool time_set = false;
	BaseType_t stat;
	uint32_t tval;
	uint32_t ctmp;
	uint32_t data;
	int i;
	bool debug_mode;
	bool rtcc_sleep = true;
	TaskHandle_t rtcc_handle;
	DEEP_SLEEP_EVENT wakeup_reason;
	DRV_TEMP_LM01_T temp[LMT01_SENSOR_NUM];
	
	for(i = 0; i < LMT01_SENSOR_NUM; i++) {
		DRV_TEMP_LM01_Init(&(temp[i]), NULL);
	}
	while(1) {
		if(first) {
			// Req time from HOST.
			// Wait 1Sec.
			// If got, set rtc to time/date.
			// Printout OK.
			// if OK, first = false;
			TWE_Wakeup(true);
			pushUartQueue("REQ TIME\n");
			memset(pStrBufHK, 0x00, sizeof(pStrBufHK));
			recvUartQueue(pStrBufHK, sizeof(pStrBufHK) / sizeof(char), cTick5Sec);
			if(checkTimeStr(pStrBufHK)) {
				setTimeRTC(pStrBufHK);
				pushUartQueue("OK\n");
			} else {
				pushUartQueue("Ignore time setting\nOK\n");
			}
			first = false;
		}
		if(rtcc_sleep) {
			rtcAlarmSet(1800 - 1, true);
		}
		TWE_Wakeup(true);
		for(i = 0; i < LMT01_SENSOR_NUM; i++) {
			if(DRV_TEMP_LM01_StartConversion(&(temp[i]))) {
				vTaskDelay(cTick110ms);
				tval = DRV_TEMP_EndConversion(&(temp[i]));
				printThermalLMT01(0, i, tval);
			}
		}
		//vTaskDelay(TICK_20MS);
		pushUartQueue("OK\n");
		if(xQueuePeek(xUartRecvQueue, (char *)&ctmp, TICK_1SEC) == pdPASS) {
			debug_mode = true;
			for(i = 0; i < 3; i++) {
				stat = xQueueReceive(xUartRecvQueue, (char *)&ctmp, TICK_500MS);
				if(stat != pdPASS) {
					debug_mode = false;
					break;
				}
				if(ctmp != '+') {
					debug_mode = false;
					break;
				}
			}
			if(debug_mode) {
				// Enter to debug mode.
				while(1) {
					if(recvUartQueue(pStrBufHK, sizeof(pStrBufHK) / sizeof(char), 1000)) {
						if(strlen(pStrBufHK) > 0) {
							if(!vShellMain(pStrBufHK)) break;
						}
					}
					pushUartQueue("OK\n");
				}
				debug_mode = false;
			}
		} else if(xQueuePeek(xUsbRecvQueue, (char *)&ctmp, TICK_1SEC) == pdPASS) {
		}
		
		TWE_Wakeup(false);
		//rtcc_handle = xTaskGetHandle("Sys RTCC Tasks");
		vTaskSuspendAll();
		if(LIB_POWER_ExistsDeepSleepMode(POWER_ID_0)) {
			// Turn TMRx OFF.
			// Turn PWM OFF.
			// Turn USB OFF.
			// Turn Interrupts ON.
			// Turn RTCC ON.
			
			PLIB_POWER_DeepSleepStatusClear(POWER_ID_0);
			PLIB_POWER_DeepSleepEventStatusClear(POWER_ID_0, 0xffffffff);
			PLIB_POWER_DeepSleepWakeupEventEnable(POWER_ID_0, DEEP_SLEEP_WAKE_UP_EVENT_RTCC);
			PLIB_POWER_DeepSleepWakeupEventEnable(POWER_ID_0, DEEP_SLEEP_WAKE_UP_EXTERNAL_INTERRUPT);
			
			//Enable power to DEEP_SLEEP_GPR_1 through DEEP_SLEEP_GPR_32 in Deep Sleep
			PLIB_POWER_DeepSleepGPRsRetentionEnable(POWER_ID_0);
			//Write 32-bit data into the DEEP_SLEEP_GPR_1
			PLIB_POWER_DeepSleepGPRWrite(POWER_ID_0, DEEP_SLEEP_GPR_1, 0x1234);
			//Enter the Deep Sleep mode and Exit
			//Now read the data from DEEP_SLEEP_GPR_1
			data = PLIB_POWER_DeepSleepGPRRead(POWER_ID_0, DEEP_SLEEP_GPR_1);
			wakeup_reason = PLIB_POWER_DeepSleepEventStatusGet(POWER_ID_0);
		} else {
			wakeup_reason = 0;
		}
		switch(wakeup_reason) {
		case DEEP_SLEEP_EVENT_EXTERNAL_INTERRUPT:
			// Check interrupt pin
			// INT -> Rendering and sound out
			rtcc_sleep = false;
			break;
		case DEEP_SLEEP_EVENT_RTCC_ALARM:
			// Resume task.
			rtcc_sleep = true;
			break;
		case DEEP_SLEEP_EVENT_BOR:
		case DEEP_SLEEP_EVENT_WDT_TIMEOUT: 
		case DEEP_SLEEP_EVENT_FAULT_DETECTION:
		case DEEP_SLEEP_EVENT_MCLR:
			rtcc_sleep = true;
			// Reset all values, reset to main().
			break;
		default: // Unknown
			rtcc_sleep = true;
			break;
		}
		//xTaskResume(rtcc_handle);
		// Resume alld Jobs.
		// Sometimes, Request to send host's time.
		
		//vTaskSuspendAll();
		xTaskResumeAll();
		vTaskDelay(cTick1Sec);
	}		
}

static void prvShell(void *pvParameters)
{
	int wakecount = 0;
	char tmpdata[128];
	bool n;
	int stdwait = 2000;
	int longwait = (3600 * 1000);
	int _stdin;
	int _stdout;
	bool f_quit = false;
	while(1) {
		// Suspend Job
		memset(tmpdata, 0x00, sizeof(tmpdata));
		// Wakeup 
		vPrt(_stdout, "MAIN", "CMD REQ AT %d mSec\nCMD>", stdwait);
		n = vGetStr(_stdin, tmpdata, 128, stdwait); // ptr, len, wait
		if(n) { // Got STR
			f_quit = vShellMain(tmpdata);
			if(!f_quit) continue;
		} else {
			// None Got
			vPrt(_stdout, "MAIN", "BYE...");
		}
	}
}
int main ( void )
{
	TimerHandle_t xTimer = NULL;
	RESET_REASON reason = prvSetupHardware();

	vCreateBlockTimeTasks();
	vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
	vStartGenericQueueTasks( mainGEN_QUEUE_TASK_PRIORITY );
	vStartQueuePeekTasks();
	vStartInterruptQueueTasks();
	
	if(!doorbellData.bootparam_passthrough) {
		// FULL
		xUartRecvQueue = xQueueCreate(128, sizeof(char));
		xUartSendQueue = xQueueCreate(256, sizeof(char));
		//xShellInQueue =
		//xShellOutQueue =
		//xLoggerBinaryQueue =
		//xLoggerCharQueue =
		xTaskCreate( prvHouseKeeping, "HouseKeeping", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, &xHandleHouseKeeping );
		xTaskCreate( prvWriteToUart,   "WriteToUart",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvWriteToUSB,   "WriteToUSB", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		
		xTaskCreate( prvLEDs, "LEDs", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvRenderThread, "Render_and_SOUND", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
	} else {
		xUsbRecvQueue = xQueueCreate(128, sizeof(char));
		xUsbSendQueue = xQueueCreate(256, sizeof(char));
		//xLoggerBinaryQueue =
		//xLoggerCharQueue =
		xTaskCreate( prvReadFromUsb,  "ReadFromUsb",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvWriteToUsb,   "WriteToUsb",  configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		//xTaskCreate( prvThermal_LMT01, "LMT01", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvWriteToUSB,   "WriteToUSB", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvWriteToUart, "WriteToUART", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvLEDs, "LEDs", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		//xTaskCreate( prvShell, "SHELL", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvBinLogger, "BinaryLogger", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		xTaskCreate( prvCharLogger, "CharLogger", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
		//xTaskCreate( prvRenderThread, "Render_and_SOUND", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL );
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
	vTaskStartScheduler();

	/* If all is well, the scheduler will now be running, and the following line
	will never be reached.  If the following line does execute, then there was
	insufficient FreeRTOS heap memory available for the idle and/or timer tasks
	to be created.  See the memory management section on the FreeRTOS web site
	for more details. */
	for( ;; );
    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/
