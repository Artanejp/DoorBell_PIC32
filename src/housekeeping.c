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
#include "semphr.h"

#include "pca9655.h"
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
extern QueueHandle_t xSoundCmdQueue;
extern uint32_t rtcAlarmSet(uint32_t _nowtime, uint32_t _sec, bool do_random);

static char pStrBufHK[128];

#define LMT01_SENSOR_NUM 1
DRV_TEMP_LM01_T x_Temp[LMT01_SENSOR_NUM];

bool f_Interrupted;
uint32_t hk_TickVal;
extern DOORBELL_DATA doorbellData;
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
    while (xSemaphoreGive(xWakeupTimerSemaphore) != pdPASS) {
        vTaskDelay(cTick100ms);
    }
}
static const uint32_t days_of_month[12] ={
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
// AD2..0 = SCL,SDA,VDD
#define I2C_EXPANDER_ADDRESS 0xa6
#define I2C_USING_DRV 0
#define IO_CHECK_BUZZER_BIT 7
PCA9655_t ioexpander1_data;
DRV_HANDLE i2cHandle;
static  PCA9655_INIT_t ioexpander1_init_data;

static inline bool check_button_level(PCA9655_t *desc, uint32_t checkbit)
{
    return DRV_PCA9655_GetPort_Bit((void *)desc, checkbit);
}
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
    SYS_RTCC_BCD_DATE _nowdate;
    SYS_RTCC_BCD_TIME _nowtime;
    static char ssbuf[64];
    uint32_t hostnum;
    bool timeout = false;
    bool pass = false;
    int retry = 0;
    uint32_t nexttime;
    int n;
    char *ps;
    SYS_RTCC_ALARM_HANDLE wakeupHandle;
    int sndcmd;
    
    uint8_t pca9655_regs[8];
    
    f_Interrupted = false; // External switch status
    //hk_TickVal = 300;
    hk_TickVal = 15;
    xWakeupTimerSemaphore = xSemaphoreCreateBinary();
    //xSemaphoreGive(xWakeupTimerSemaphore);
    i2cHandle = sv_open_i2c(I2C_USING_DRV);
    
    ioexpander1_init_data.close_port = &DRV_PCA9655_sample_close_port;
    //ioexpander1_init_data.close_port = NULL;
    ioexpander1_init_data.open_port = &DRV_PCA9655_sample_open_port;
    ioexpander1_init_data.direction_0 = 0b11111111; // ALL IN
    ioexpander1_init_data.direction_1 = 0b11111100; // ALL OUT
    ioexpander1_init_data.polality_0 = 0b00000000; // Not invert
    ioexpander1_init_data.polality_1 = 0b00000000; // Not invert
    ioexpander1_init_data.data_0 = 0b11111111; // ALL ON
    ioexpander1_init_data.data_1 = 0b11110000; // ALL OFF
    I2C1CONbits.ON = 1;
    DRV_PCA9655_Init(0, i2cHandle, &ioexpander1_data, (uint16_t)I2C_EXPANDER_ADDRESS, &ioexpander1_init_data);
    for (i = 0; i < LMT01_SENSOR_NUM; i++) {
        DRV_TEMP_LM01_Init(&(x_Temp[i]),  (uint32_t)(i + 8), &DRV_PCA9655_SetPort, (void *)(&ioexpander1_data)); 
    }
    memset(pStrBufHK, 0x00, sizeof (pStrBufHK));
    SLEEP_Periferals(true); // Disable unused periferals.
    wakeupHandle = SYS_RTCC_AlarmRegister(&wakeupCallback, NULL);
    SYS_RESET_ReasonClear(doorbellData.resetReason);
    
    TWE_Wakeup(false);
    RPA1Rbits.RPA1R = 0b0000; // Sound OFF
    while (1) {
        SYS_WDT_TimerClear();
        SYS_WDT_Enable(false);
        //sndcmd = C_SOUND_START;
        //xQueueSend(xSoundCmdQueue, &sndcmd, 0);
        if (first) {
            SYS_WDT_TimerClear();
            TWE_Wakeup(true);
            vTaskDelay(cTick100ms);
            vRingBufferClear_Char(&xUartRecvRing);
            vTaskDelay(cTick100ms);
            // "yyyy/MM/dd DoW hh:mm:ss"
            printLog(0, "BEGIN", "DOORBELL HOUSEKEEPING TASK", LOG_TYPE_MESSAGE, NULL, 0);
            printLog(0, "MSG", "NOW TIME IS", LOG_TYPE_MESSAGE, NULL, 0);
            if((doorbellData.resetReason & RESET_REASON_WDT_TIMEOUT) != 0) {
                printLog(0, "MSG", "WDT EXPIRED.", LOG_TYPE_WDT_RESET, NULL, 0);
            } 
            if((doorbellData.resetReason & 0x03) == RESET_REASON_BROWNOUT) {
                printLog(0, "MSG", "LOW VOLTAGE RESET HAPPENED.", LOG_TYPE_BOR_RESET, NULL, 0);
            }
            i = 0;
            do {
                SYS_WDT_TimerClear();
                printLog(0, "REQ", "DATETIME WAIT 15 SEC", LOG_TYPE_MESSAGE, NULL, 0);
                //vTaskDelay(cTick1Sec * 5);
                
                printMessage(0, NULL, "OK");
                //vTaskDelay(cTick200ms);
                pStrBufHK[0] = '\0';
            _nn0:
                n = recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick5Sec * 3);
                if (n > 0) {
                    if (checkSender(pStrBufHK, &hostnum, &ps, sizeof (pStrBufHK) / sizeof (char)) == N_HOST_COMPLETE) {
                        if (ps != NULL) {
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
           SYS_WDT_TimerClear();
            if (!pass) {
                if (timeout) printLog(0, "ERR", "TIME OUT.", LOG_TYPE_MESSAGE, NULL, 0);
                printLog(0, "ERR", "IGNORE TIME SETTING.", LOG_TYPE_MESSAGE, NULL, 0);
            }
            first = false;
        }
        SYS_RTCC_DateGet(&_nowdate);
        SYS_RTCC_TimeGet(&_nowtime);
        SYS_WDT_TimerClear();
        TWE_Wakeup(true);
        //vTaskDelay(cTick1Sec);
        printLog(0, "MSG", "WAKE UP", LOG_TYPE_MESSAGE, NULL, 0);
        TWE_Wakeup(false);
        SYS_WDT_TimerClear();
        DRV_PCA9655_Reset(&ioexpander1_data, 0, 0, false); // Reset I/O Expander.
#if 1
        for (i = 0; i < LMT01_SENSOR_NUM; i++) {
            I2C1CONbits.ON = 1;
            if (DRV_TEMP_LM01_StartConversion(&(x_Temp[i]))) {
            I2C1CONbits.ON = 1;
                tval = DRV_TEMP_LM01_EndConversion(&(x_Temp[i]));
                TWE_Wakeup(true);
                printThermalLMT01(0, i, tval); // USB
                TWE_Wakeup(false);
                SYS_WDT_TimerClear();
            }
        }
#endif
        DRV_PCA9655_GetRegs(&ioexpander1_data, pca9655_regs);
        TWE_Wakeup(true);
        printLog(0, "DEBUG",   "PCA6955 REGS   0  1  2  3", LOG_TYPE_MESSAGE, NULL, 0);
        snprintf(pStrBufHK, 48, "PCA9655 REG +0: %02x %02x %02x %02x", pca9655_regs[0], pca9655_regs[1], pca9655_regs[2], pca9655_regs[3]);
        printLog(0, "DEBUG", pStrBufHK, LOG_TYPE_MESSAGE, NULL, 0);
        snprintf(pStrBufHK, 48, "PCA9655 REG +4: %02x %02x %02x %02x", pca9655_regs[4], pca9655_regs[5], pca9655_regs[6], pca9655_regs[7]);
        printLog(0, "DEBUG", pStrBufHK, LOG_TYPE_MESSAGE, NULL, 0);
        TWE_Wakeup(false);
       //vTaskDelay(cTick1Sec * 5);
        TWE_Wakeup(true);
        //sndcmd = C_SOUND_STOP;
        //xQueueSend(xSoundCmdQueue, &sndcmd, 0);
        printMessage(0, NULL, "READY");
        //vTaskDelay(cTick200ms);

        //if (xQueuePeek(xUartRecvQueue, pStrBufHK, cTick1Sec) == pdPASS) {
        debug_mode = false;
        //do {
            n = recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick1Sec * 3);
            if (n > 0) {
                int nn;
                ps = NULL;
                if(checkSender(pStrBufHK, &hostnum, &ps, sizeof (pStrBufHK) / sizeof (char)) == N_HOST_COMPLETE) {
                    if (ps != NULL) {
                        if (strlen(ps) > (3 + 19)) {
                            if (strncmp(ps, " T ", 3) == 0) {
                                ps = &(ps[3]);
                                    if (setDateTimeStr(ps)) {
                                    printLog(0, "MSG", "TIME SET.", LOG_TYPE_MESSAGE, NULL, 0);
                                }
                            } else {
                                // Another command.
                            } 
                        }
                    }
                }
                snprintf(ssbuf, sizeof (ssbuf), "ECHO: %s", pStrBufHK);
                printLog(0, "MSG", ssbuf, LOG_TYPE_NOP, NULL, 0);
                printMessage(0, NULL, "READY");
                SYS_RTCC_DateGet(&_nowdate);
                SYS_RTCC_TimeGet(&_nowtime);
                SYS_WDT_TimerClear();
            }
        //} while (n > 0);
        debug_mode = false;
        if (f_Interrupted) {
            rtcc_sleep = false;
        } else if (!rtcc_sleep) {
            rtcc_sleep = true;
        }
        SYS_WDT_TimerClear();
        if (rtcc_sleep) {
            f_Interrupted = false;
#if 0   /* Debugging */
            {
                SYS_RTCC_DateGet(&_nowdate);
                SYS_RTCC_TimeGet(&_nowtime);
                nexttime = rtcAlarmSet(_nowtime, 5, false);
            }
#else
           nexttime = rtcAlarmSet(_nowtime, hk_TickVal, false);
#endif /* Debugging */
            snprintf(ssbuf, sizeof (ssbuf) / sizeof (char), "ENTER TO SLEEP UNTIL %d%d:%d%d:%d%d", 
                    (nexttime >> 28) & 0x0f,
                    (nexttime >> 24) & 0x0f,
                    (nexttime >> 20) & 0x0f,
                    (nexttime >> 16) & 0x0f,
                    (nexttime >> 12) & 0x0f,
                    (nexttime >>  8) & 0x0f
                    );
            SYS_WDT_TimerClear();
            SYS_WDT_Enable(false);
            printLog(0, "MSG", ssbuf, LOG_TYPE_MESSAGE, NULL, 0);
            //vTaskDelay(cTick1Sec);
            TWE_Wakeup(false);
            {
                // Set alarm and Sleep all tasks.
                SYS_RTCC_AlarmEnable();
                SYS_WDT_TimerClear();
                SYS_WDT_Disable();
                vTaskSuspendAll();
            }
            {
                // Sleep machine except RTCC and interrupts.
                SYS_DEVCON_SystemUnlock();
                OSCCONbits.NOSC = 0b100; // SOSC.
                OSCCONbits.OSWEN = 1;
                OSCCONbits.SLPEN = 1;
#if 1           
                PMD1bits.AD1MD = 1;
                PMD1bits.CTMUMD = 1;
                //PMD1bits.CVRMD = 1;
                PMD2bits.CMP1MD = 1;
                PMD2bits.CMP2MD = 1;
                PMD2bits.CMP3MD = 1;
                PMD3bits.IC1MD = 1;
                PMD3bits.IC2MD = 1;
                PMD3bits.IC3MD = 1;
                PMD3bits.IC4MD = 1;
                PMD3bits.IC5MD = 1;
                PMD3bits.OC1MD = 1;
                //PMD3bits.OC2MD = 1;
                PMD3bits.OC3MD = 1;
                PMD3bits.OC4MD = 1;
                PMD3bits.OC5MD = 1;
                //PMD4bits.T1MD = 1;
                //PMD4bits.T2MD = 1;
                //PMD4bits.T3MD = 1;
                //PMD4bits.T4MD = 1;
                //PMD4bits.T5MD = 1;
                //PMD5bits.U1MD = 1;
                PMD5bits.U2MD = 1;
                //PMD5bits.I2C1MD = 1;
                PMD5bits.I2C2MD = 1;
                PMD5bits.SPI1MD = 1;
                PMD5bits.SPI2MD = 1;
                //PMD5bits.USB1MD = 1;
                //PMD5bits.USBMD = 1;
                PMD6bits.PMPMD = 1;
#endif
                SYS_DEVCON_SystemLock();

                asm volatile("WAIT");
//                SYS_WDT_Disable();
            }    
            {
                // Resume all tasks and Wait for alarm waking.
                // ToDo: button pressed.
                SYS_DEVCON_SystemUnlock();
                OSCCONbits.SLPEN = 0;
#if 1
                PMD4bits.T1MD = 0;
                PMD4bits.T2MD = 0;
                PMD4bits.T3MD = 0;
                PMD4bits.T4MD = 0;
                PMD4bits.T5MD = 0;
                PMD3bits.OC2MD = 0;
                PMD5bits.U1MD = 0;
                PMD5bits.I2C1MD = 0;
                PMD5bits.USB1MD = 0;
                PMD5bits.USBMD = 0;
                OSCCONbits.NOSC = 0b011; // PLL.
                OSCCONbits.OSWEN = 1;
#endif
                SYS_DEVCON_SystemLock();

                xTaskResumeAll();
                // Check Interrupts
                
                // Check ALARM
                uint32_t nc = 0;
                SYS_WDT_TimerClear();
                SYS_WDT_Enable(false);
                while(xSemaphoreTake(xWakeupTimerSemaphore, cTick1Sec) != pdPASS) {
                   vTaskDelay(cTick1Sec);
                   nc++;
                }
                SYS_WDT_TimerClear();
                SYS_WDT_Disable();
            }
            //vTaskSuspendAll();
            // Stop Periferals.
            //vTaskDelay(cTick1Sec);
        } else {
            vTaskDelay(cTick200ms);
        }
    }
}

