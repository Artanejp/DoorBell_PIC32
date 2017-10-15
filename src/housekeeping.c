
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
#include "lm01_drv.h"


/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Standard demo includes. */
//#include "partest.h"

/* Hardware specific includes. */
//#include "ConfigPerformance.h"

static char rdUartBuf[16];
static char rdTmpUartBuf[4];
static char *rdUartPtr;
static ssize_t rdUartSize;
const ssize_t rdUartSizeLimit = 16;

static SemaphoreHandle_t xUartRdSemaphore;

static consoleCallbackFunction cbUartReadComplete(void *handle)
{
    char *p = rdTmpUartBuf;
    ssize_t size = (ssize_t) handle;

    if (size >= 0) {
        rdUartSize += size;
        if (rdUartSize <= rdUartSizeLimit) {
            if (rdUartPtr != NULL) {
                memcpy(rdUartPtr, p, size);
                rdUartPtr += size;
            }
        }
    }
    if (xUartRdSemaphore != NULL) xSemaphoreGive(xUartRdSemaphore);
}

void prvReadFromUart(void *pvparameters)
{
    ssize_t _len;
    BaseType_t stat;
    int i;

    rdUartSize = 0;
    rdUartPtr = rdUartBuf;
    xUartRdSemaphore = xSemaphoreCreateBinary();
    if (xUartRdSemaphore == NULL) return;

    SYS_CONSOLE_RegisterCallback(SYS_CONSOLE_INDEX_1, cbUartReadComplete, SYS_CONSOLE_EVENT_READ_COMPLETE);
    while (1) {
        if (xUartRecvQueue != NULL) {
            _len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_1, STDIN_FILENO, rdTmpUartBuf, 1);
            while (xSemaphoreTake(xUartRdSemaphore, cTick1Sec) != pdPASS) {
            }
            if (rdUartSize > 0) {
                i = 0;
                while (i < rdUartSize) {
                    stat = xQueueSend(xUartRecvQueue, &(rdUartBuf[i]), cTick500ms);
                    if (stat == pdPASS) {
                        i++;
                    }
                }
                rdUartSize = 0;
                rdUartPtr = rdUartBuf;
            }
        }
    }
}

static ssize_t recvUartQueue(char *buf, ssize_t len, int timeout)
{
    BaseType_t stat;
    ssize_t i;
    if (buf == NULL) return -1;
    if (len <= 0) return 0;
    memset(buf, 0x00, len);
    for (i = 0; i < len; i++) {
        stat = xQueueReceive(xUartRecvQueue, &(buf[i]), timeout);
        if (stat != pdPASS) return i;
    }
    return i;
}

static ssize_t recvUartQueueDelim(char *buf, ssize_t maxlen, char delim, int timeout)
{
    BaseType_t stat;
    ssize_t i = 0;
    if (buf == NULL) return -1;
    if (maxlen <= 0) return 0;

    memset(buf, 0x00, maxlen);
    while (i < maxlen) {
        stat = xQueueReceive(xUartRecvQueue, &(buf[i]), timeout);
        if (stat == pdPASS) {
            if (buf[i] == delim) return (i + 1);
            i++;
        }
    }
    return i;
}
extern void TWE_Wakeup(bool onoff);
static char pStrBufHK[128];

#define LMT01_SENSOR_NUM 1
DRV_TEMP_LM01_T x_Temp[LMT01_SENSOR_NUM];

bool f_Interrupted;
uint32_t hk_TickVal;

bool vShellMain(int index, char *head, char *tmpdata)
{
    bool f_quit;
    char cmdhead;
    int num;
    if (tmpdata == NULL) return false;

    cmdhead = tmpdata[0];

    // parse string
    f_quit = false;
    switch (cmdhead) {
    case 'Q':
    case 'q':
        printLog(index, head, "MSG QUIT SHELL", LOG_TYPE_MESSAGE, NULL, 0);
        f_quit = true;
        break;
    case 'T': // set time
        if ((tmpdata[1] == ' ') && (strlen(tmpdata) > 3)) {
            if (setDateTimeStr(&(tmpdata[2]))) {
                printLog(index, head, "MSG TIME SET", LOG_TYPE_TIME_UPDATED, NULL, 0);
            } else {
                printLog(index, head, "MSG TIME ERR", LOG_TYPE_TIME_ERROR, NULL, 0);
            }
        }
        break;
    case 't':
        // Get tempareture #
        if (strlen(tmpdata) < 3) {
            num = 0;
        } else if ((tmpdata[2] >= '0') && (tmpdata[2] <= '9')) {
            num = tmpdata[2] - '0';
        } else if ((tmpdata[2] >= 'a') && (tmpdata[2] <= 'f')) {
            num = tmpdata[2] - 'a' + 10;
        } else if ((tmpdata[2] >= 'A') && (tmpdata[2] <= 'F')) {
            num = tmpdata[2] - 'A' + 10;
        }
        if (num < LMT01_SENSOR_NUM) {
            if (DRV_TEMP_LM01_StartConversion(&(x_Temp[num]))) {
                uint32_t tval;
                vTaskDelay(cTick110ms);
                tval = DRV_TEMP_LM01_EndConversion(&(x_Temp[num]));
                printThermalLMT01(index, head, num, tval); // USB
            }
        }
        break;
    case 'h':
    case 'H':
        printLog(index, head, "MSG SHELL HELP", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP T [yyyy/MM/DD hh:mm:ss]  : Set time", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP t [num]  : Get tempareture : num = [0-9] or [A-F].", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP s : Stop Sound .", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP ms [num] : Set main MML to [num] : num = [0-9] or [A-F].", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP md num : Delete MML [num].", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP ml : List MMLs.", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP mu num : Upload MML to [num]. MML should be ended with '.'.", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP x [num] : Read switch value #num.", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP l [all-log] : Read log.If all-log = ALL print all stored log.", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP H : Print this help.", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "HELP q : Quit.", LOG_TYPE_NOP, NULL, 0);
        printLog(index, head, "MSG HELP END", LOG_TYPE_NOP, NULL, 0);
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
    case 'x':
        //  Read/Set switch value.
        break;
    default:
        break;
    }
    return f_quit;
}

void prvHouseKeeping(void *pvParameters)
{
    bool first = true;
    bool time_set = false;
    BaseType_t stat;
    uint32_t tval;
    uint32_t data;
    int i;
    bool debug_mode;
    bool rtcc_sleep = true;
    //TaskHandle_t rtcc_handle;
    SYS_RTCC_BCD_DATE _nowdate;
    SYS_RTCC_BCD_TIME _nowtime;
    DEEP_SLEEP_EVENT wakeup_reason;
    char *head = NULL; // ToDo Update.
    char ssbuf[64];

    f_Interrupted = false; // External switch status
    hk_TickVal = 1800;

    for (i = 0; i < LMT01_SENSOR_NUM; i++) {
        DRV_TEMP_LM01_Init(&(x_Temp[i]), NULL);
    }
    SLEEP_Periferals(true); // Disable unused periferals.
    while (1) {
        if (first) {
            char stbuf[128];
            // Req time from HOST.
            // Wait 1Sec.
            // If got, set rtc to time/date.
            // Printout OK.
            // if OK, first = false;
            TWE_Wakeup(true);
            // "yyyy/MM/dd DoW hh:mm:ss"
            printLog(0, head, "BEGIN DOORBELL HOUSEKEEPING TASK", LOG_TYPE_MESSAGE, NULL, 0);
            printLog(0, head, "MSG NOW TIME IS", LOG_TYPE_MESSAGE, NULL, 0);
            printLog(0, head, "REQ DATETIME", LOG_TYPE_MESSAGE, NULL, 0);
            if (recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick5Sec) > 0) {
                if (!setDateTimeStr(pStrBufHK)) {
                    printLog(0, head, "ERR Ignore time setting.", LOG_TYPE_MESSAGE, NULL, 0);
                }
            } else {
                printLog(0, head, "ERR Ignore time setting.", LOG_TYPE_MESSAGE, NULL, 0);
            }
            first = false;
            //printLog(0, head, "OK", LOG_TYPE_NOP, NULL, 0);
        }
        TWE_Wakeup(true);
        printLog(0, head, "MSG WAKE UP", LOG_TYPE_MESSAGE, NULL, 0);
        for (i = 0; i < LMT01_SENSOR_NUM; i++) {
            if (DRV_TEMP_LM01_StartConversion(&(x_Temp[i]))) {
                vTaskDelay(cTick110ms);
                tval = DRV_TEMP_LM01_EndConversion(&(x_Temp[i]));
                printThermalLMT01(0, head, i, tval); // USB
            }
        }
        printLog(0, head, "OK", LOG_TYPE_NOP, NULL, 0);
        if (xQueuePeek(xUartRecvQueue, pStrBufHK, cTick1Sec) == pdPASS) {
            debug_mode = false;
            if (recvUartQueue(pStrBufHK, 3, cTick1Sec) == 3) {
                debug_mode = true;
                for (i = 0; i < 3; i++) {
                    if (pStrBufHK[i] != '+') {
                        debug_mode = false;
                        break;
                    }
                }
            }
            // Enter to debug mode.
            while (debug_mode) {
                if (recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', 0xffffffff) > 0) {
                    if (strlen(pStrBufHK) > 0) {
                        if (!vShellMain(0, head, pStrBufHK)) debug_mode = false;
                    }
                }
            }
            printLog(0, head, "OK", LOG_TYPE_NOP, NULL, 0);
            debug_mode = false;
        }

        //rtcc_handle = xTaskGetHandle("Sys RTCC Tasks");
        if (f_Interrupted) {
            rtcc_sleep = false;
        } else if (!rtcc_sleep) {
            rtcc_sleep = true;
        }
        if (rtcc_sleep) {
            f_Interrupted = false;
            rtcAlarmSet(hk_TickVal, false);

            snprintf(ssbuf, sizeof (ssbuf) / sizeof (char), "MSG ENTER TO SLEEP UNTIL %d SEC.", hk_TickVal);
            printLog(0, head, ssbuf, LOG_TYPE_MESSAGE, NULL, 0);

            vTaskDelay(cTick200ms);
            TWE_Wakeup(false);
            vTaskSuspendAll();
            // Stop Periferals.
#if 0 // Not equipped DEE SLEEP.            
            if (PLIB_POWER_ExistsDeepSleepMode(POWER_ID_0)) {
                // Turn TMRx OFF.
                // Turn PWM OFF.
                // Turn USB OFF.
                // Turn Interrupts ON.
                // Turn RTCC ON.

                PLIB_POWER_DeepSleepStatusClear(POWER_ID_0);
                PLIB_POWER_DeepSleepEventStatusClear(POWER_ID_0, 0xffffffff);
                PLIB_POWER_DeepSleepWakeupEventEnable(POWER_ID_0, DEEP_SLEEP_EVENT_RTCC_ALARM);
                PLIB_POWER_DeepSleepWakeupEventEnable(POWER_ID_0, DEEP_SLEEP_EVENT_EXTERNAL_INTERRUPT);

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
            switch (wakeup_reason) {
            case DEEP_SLEEP_EVENT_EXTERNAL_INTERRUPT:
                // Check interrupt pin
                // INT -> Rendering and sound out
                rtcc_sleep = false;
                f_Interrupted = true;
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
#endif // Not equipped DEE SLEEP.            
            //xTaskResume(rtcc_handle);
            // Resume alld Jobs.
            // Sometimes, Request to send host's time.

            //vTaskSuspendAll();
            xTaskResumeAll();
            //vTaskDelay(cTick1Sec);
        } else {
            vTaskDelay(cTick200ms);
        }
    }
}

