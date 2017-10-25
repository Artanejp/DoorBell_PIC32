
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
#include "ringbuffer.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Standard demo includes. */
//#include "partest.h"

/* Hardware specific includes. */
//#include "ConfigPerformance.h"
extern void prvWriteToUart_HK(void *pvparameters);
extern void prvReadFromUart_HK(void *pvparameters);
extern ssize_t recvUartQueue(char *buf, ssize_t len, int timeout);
extern ssize_t recvUartQueueDelim(char *buf, ssize_t maxlen, char delim, uint32_t timeout);
extern void TWE_Wakeup(bool onoff);
extern RingBuffer_Char_t xUartRecvRing;
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
                printThermalLMT01(index, num, tval); // USB
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
SemaphoreHandle_t xWakeupTimerSemaphore;
SYS_RTCC_ALARM_CALLBACK wakeupCallback(SYS_RTCC_ALARM_HANDLE handle, uintptr_t context)
{
    while(xSemaphoreGive(xWakeupTimerSemaphore) != pdPASS) {
    vTaskDelay(cTick100ms);
    }
}
static const uint32_t days_of_month[12] =
{
    0x00003100,
    0x00002800,
    0x00003100,
    0x00003000,
    0x00003100,
    0x00003000,
    0x00003100,
    0x00003100,
    0x00003000,
    0x00003100,
    0x00003000,
    0x00003100,
};
void prvHouseKeeping(void *pvParameters)
{
    bool first = true;
    //bool first = false;
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
    static char ssbuf[64];
    uint32_t hostnum;
    bool timeout = false;
    bool pass = false;
    int retry = 0;
    int n;
    char *ps;
    SYS_RTCC_ALARM_HANDLE wakeupHandle;
    f_Interrupted = false; // External switch status
    hk_TickVal = 1800;

    xWakeupTimerSemaphore = xSemaphoreCreateBinary();
    //xSemaphoreGive(xWakeupTimerSemaphore);
    for (i = 0; i < LMT01_SENSOR_NUM; i++) {
        DRV_TEMP_LM01_Init(&(x_Temp[i]), NULL);
    }
    memset(pStrBufHK, 0x00, sizeof (pStrBufHK));
    SLEEP_Periferals(true); // Disable unused periferals.
    wakeupHandle = SYS_RTCC_AlarmRegister(&wakeupCallback, NULL);
    while (1) {
        if (first) {
            TWE_Wakeup(true);
            vTaskDelay(cTick100ms);
            vRingBufferClear_Char(&xUartRecvRing);
            vTaskDelay(cTick100ms);
            // "yyyy/MM/dd DoW hh:mm:ss"
            printLog(0, "BEGIN", "DOORBELL HOUSEKEEPING TASK", LOG_TYPE_MESSAGE, NULL, 0);
            printLog(0, "MSG", "NOW TIME IS", LOG_TYPE_MESSAGE, NULL, 0);
            pStrBufHK[0] = '\0';
            i = 0;
            do {
                printLog(0, "REQ", "DATETIME WAIT 15 SEC", LOG_TYPE_MESSAGE, NULL, 0);
                pStrBufHK[0] = '\0';
            _nn0:
                n = recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick5Sec * 3);
                if (n > 0) {
                    if (checkSender(pStrBufHK, &hostnum, &ps, sizeof (pStrBufHK) / sizeof (char)) == N_HOST_COMPLETE) {
                        if(ps != NULL) {
                           if (!setDateTimeStr(ps)) {
                                printLog(0, "ERR", "WRONG TIME STRING.", LOG_TYPE_MESSAGE, NULL, 0);
                                retry++;
                                if (retry >= 5) break;
                            } else {
                                printLog(0, "MSG", "TIME SET.", LOG_TYPE_MESSAGE, NULL, 0);
                                pass = true;
                                break;
                            }
                        }
                    } else {
                        goto _nn0;
                    }
                } else if (n == 0) {
                    goto _nn0;
                    //printLog(0, "ERR", "UNKNOWN TIME STRING.", LOG_TYPE_MESSAGE, NULL, 0);
                } else {
                    timeout = true;
                    break;
                }
            } while (n > 0);
            if (!pass) {
                if (timeout) printLog(0, "ERR", "TIME OUT.", LOG_TYPE_MESSAGE, NULL, 0);
                printLog(0, "ERR", "IGNORE TIME SETTING.", LOG_TYPE_MESSAGE, NULL, 0);
            }
            first = false;
            //printLog(0, head, "OK", LOG_TYPE_NOP, NULL, 0);
        }
        TWE_Wakeup(true);
        printLog(0, "MSG", "WAKE UP", LOG_TYPE_MESSAGE, NULL, 0);
        for (i = 0; i < LMT01_SENSOR_NUM; i++) {
            if (DRV_TEMP_LM01_StartConversion(&(x_Temp[i]))) {
                //vTaskDelay(cTick110ms* 25);
                tval = DRV_TEMP_LM01_EndConversion(&(x_Temp[i]));
                printThermalLMT01(0, i, tval); // USB
            }
        }
        printMessage(0, NULL, "OK");
        //if (xQueuePeek(xUartRecvQueue, pStrBufHK, cTick1Sec) == pdPASS) {
        debug_mode = false;
#if 0        
        if (recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick5Sec) > 0) {
            if (strlen(pStrBufHK) > 0) {
                snprintf(ssbuf, sizeof (ssbuf), "ECHO: %s", pStrBufHK);
                printLog(0, head, ssbuf, LOG_TYPE_NOP, NULL, 0);
            }
            if (strcmp(pStrBufHK, "shell\n") == 0) {
                debug_mode = true;
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
#else
        vTaskDelay(cTick100ms);
        vRingBufferClear_Char(&xUartRecvRing);
        vTaskDelay(cTick100ms);
        do {
            //pStrBufHK[0] = '\0';
            n = recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick1Sec * 3);
            //n = recvUartQueue(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), cTick1Sec);
            if (n > 0) {
                if (checkSender(pStrBufHK, &hostnum, &ps, sizeof (pStrBufHK) / sizeof (char)) == N_HOST_COMPLETE) {
                    if(ps != NULL) {
                        if(strlen(ps) > (3 + 19)) {
                            if(strncmp(ps, " T ", 3) == 0) {
                                ps = &(ps[3]);
                                if(setDateTimeStr(ps)) {
                                    printLog(0, "MSG", "TIME SET.", LOG_TYPE_MESSAGE, NULL, 0);
                                }
                            }
                        }
                    }
                    snprintf(ssbuf, sizeof (ssbuf), "ECHO: %s", pStrBufHK);
                    printLog(0, "MSG", ssbuf, LOG_TYPE_NOP, NULL, 0);
                    printMessage(0, NULL, "OK");
                }
            }
        } while (n > 0);
#endif
        debug_mode = false;
        //}

        //rtcc_handle = xTaskGetHandle("Sys RTCC Tasks");
        if (f_Interrupted) {
            rtcc_sleep = false;
        } else if (!rtcc_sleep) {
            rtcc_sleep = true;
        }
        if (rtcc_sleep) {
            f_Interrupted = false;
            rtcAlarmSet(hk_TickVal, false);

            snprintf(ssbuf, sizeof (ssbuf) / sizeof (char), "ENTER TO SLEEP UNTIL %d SEC.", hk_TickVal);
            printLog(0, "MSG", ssbuf, LOG_TYPE_MESSAGE, NULL, 0);

            TWE_Wakeup(false);
            SYS_RTCC_BCD_TIME nwakeup;
            volatile SYS_RTCC_BCD_DATE dwakeup;
            SYS_RTCC_BCD_TIME _d = 0x00001000; // 00:00:10
            SYS_RTCC_TimeGet(&nwakeup);
            SYS_RTCC_DateGet(&dwakeup);
            nwakeup = nwakeup + _d;
            if((nwakeup & 0x00000f00) > 0x00000900) {
                nwakeup = ((nwakeup & 0xfffff000) + 0x00001000) | ((nwakeup & 0x00000f00) - 0x00000a00);
            }
            if((nwakeup & 0x0000ff00) > 0x00005900) {
                nwakeup = ((((nwakeup & 0x0000ff00) - 0x00005900) | (nwakeup & 0xffff00ff))) + 0x00010000;
            }
            if((nwakeup & 0x000f0000) > 0x00090000) {
                nwakeup = ((nwakeup & 0xfff0ff00) + 0x00100000) | ((nwakeup & 0x000f0000) - 0x000a0000);
            }
            if((nwakeup & 0x00ff0000) > 0x00590000) {
                nwakeup = ((((nwakeup & 0x00ff0000) - 0x00590000) | (nwakeup & 0xff00ffff))) + 0x01000000;
            }
            if((nwakeup & 0x0f000000) > 0x09000000) {
                nwakeup = ((nwakeup & 0xf0ffff00) + 0x10000000) | ((nwakeup & 0x0f000000) - 0x0a000000);
            }
            if((nwakeup & 0xff000000) >= 0x24000000) {
                nwakeup = nwakeup - 0x24000000;
                dwakeup = dwakeup + 0x00000100;
            }
            if((dwakeup & 0x00000f00) > 0x00000900) {
                dwakeup = ((dwakeup & 0xffff0fff) - 0x00000a00) | ((dwakeup & 0xffff0fff) + 0x00001000); 
            } 
            uint32_t mm = (dwakeup & 0x00ff0000) >> 16;
            if((mm & 0xf0) >= 0x10) {
                if((mm & 0x0f) > 0x02) {
                    mm = 0x01;
                } else {
                    mm = 10 + (mm & 0x0f);
                }
            } else {
                mm = mm & 0x0f;
                if(mm > 9) mm = 1;
                if(mm < 1) mm = 1;
            }
            mm = mm - 1;
            uint32_t monthtick = days_of_month[mm];
            uint32_t y = (dwakeup & 0xff000000) >> 24;
            if((y & 0x0f) > 9) {
                y = (y & 0xf0) | ((y & 0x0f) - 10);
            }
            y = (((y & 0xf0) >> 4) * 10) + (y & 0x0f);
            if(mm == 1) { // 2
                if((y & 3) == 0) {
                    monthtick = 29;
                }
            }
            if((dwakeup & 0x0000ff00) > monthtick) {
                dwakeup = (dwakeup & 0xffff00ff) | 0x00000100; 
                mm = mm + 1;
            } 
            mm = mm + 1;
            if(mm > 12) {
                mm = 1;
                y = y + 1;
            }
            mm = (((mm / 10) << 4) | (mm % 10)) << 16;
            dwakeup = (dwakeup & 0xff00ffff) | mm;
            if(y > 99) y = 0;
            dwakeup = (dwakeup & 0x00ffffff) | ((y / 10) << 28) | ((y % 10) << 24);
            
            SYS_RTCC_AlarmDateSet(dwakeup);
            SYS_RTCC_AlarmTimeSet(nwakeup, true);
            SYS_RTCC_AlarmEnable();
            //while(xSemaphoreTake(xWakeupTimerSemaphore, 0xffffffff) != pdPASS) {
             //   vTaskDelay(cTick100ms);
            //}
            //xSemaphoreGive(xWakeupTimerSemaphore);
            SYS_RTCC_AlarmDisable();
            vTaskDelay(cTick5Sec);
            //vTaskSuspendAll();
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
            //xTaskResumeAll();
            //vTaskDelay(cTick1Sec);
        } else {
            vTaskDelay(cTick200ms);
        }
    }
}

