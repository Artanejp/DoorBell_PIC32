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
#include "doorbell.h"   // SYS function prototypes

/* Hardware specific includes. */
//#include "ConfigPerformance.h"

//extern RingBuffer_Char_t xUartRecvRing;

static char pStrBufHK[128];
DRV_TEMP_LM01_T x_Temp[LMT01_SENSOR_NUM];
bool f_interrupted;

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
PCA9655_t ioexpander1_data;
DRV_HANDLE i2cHandle;
extern bool uart_wakeup;
static PCA9655_INIT_t ioexpander1_init_data;

bool interrupt_from_lowvoltage;
bool interrupt_from_expander;
bool b_ring;
bool b_debugsw;
bool b_battlost;
uint8_t val_dipsw;
uint32_t sleep_sec;

static inline bool check_button_level(PCA9655_t *desc, uint32_t checkbit)
{
    return DRV_PCA9655_GetPort_Bit((void *) desc, checkbit);
}

void CmdPlayMusic(void)
{
    int sndcmd = C_SOUND_START;
    I2C1CONbits.ON = 1;
    DRV_PCA9655_SetPort(&ioexpander1_data, 5, true); // Audio ON
    I2C1CONbits.ON = 0;
    SYS_DEVCON_SystemUnlock();
    PMD3bits.OC2MD = 0;
    PMD4bits.T3MD = 0;
    SYS_DEVCON_SystemLock();
    xQueueSend(xSoundCmdQueue, &sndcmd, 0);
}

void CmdStopMusic(void)
{
    int sndcmd = C_SOUND_STOP;
    I2C1CONbits.ON = 1;
    xQueueSend(xSoundCmdQueue, &sndcmd, 0);
    DRV_PCA9655_SetPort(&ioexpander1_data, 5, false); // Audio ON
    I2C1CONbits.ON = 0;
}

bool check_uart_run(void)
{
    bool bf;
    taskENTER_CRITICAL();
    bf = uart_wakeup;
    taskEXIT_CRITICAL();
    return bf;
}

void wait_uart_ready(int max_wait)
{
    int _wait = 0;
    while (check_uart_run()) {
        SYS_WDT_TimerClear();
        if (max_wait >= 0) {
            _wait++;
            if (_wait > max_wait) break;
        }
        vTaskDelay(cTick100ms);
    }
}

//extern SYS_RTCC_ALARM_CALLBACK wakeupCallback(SYS_RTCC_ALARM_HANDLE handle, uintptr_t context);

SYS_RTCC_ALARM_CALLBACK wakeupCallback(SYS_RTCC_ALARM_HANDLE handle, uintptr_t context)
{
    BaseType_t req_yield;
    //b_wakeup_alarm = true;
    xSemaphoreGiveFromISR(xWakeupTimerSemaphore, &req_yield);
}

static void init_ioexpander(void)
{
    ioexpander1_init_data.close_port = &DRV_PCA9655_sample_close_port;
    //ioexpander1_init_data.close_port = NULL;
    ioexpander1_init_data.open_port = &DRV_PCA9655_sample_open_port;
    ioexpander1_init_data.direction_0 = 0b11010000; // IO0_5 = Out (AUDIO_ON)
    ioexpander1_init_data.direction_1 = 0b11110000; // ALL OUT
    ioexpander1_init_data.polality_0 = 0b00000000; // Not invert
    ioexpander1_init_data.polality_1 = 0b00000000; // Not invert
    ioexpander1_init_data.data_0 = 0b11011100; // IO0_5 = ON
    ioexpander1_init_data.data_1 = 0b11110000; // ALL OFF
    I2C1CONbits.ON = 1;
    DRV_PCA9655_Init(0, i2cHandle, &ioexpander1_data, (uint16_t) I2C_EXPANDER_ADDRESS, &ioexpander1_init_data);
    DRV_PCA9655_SetPort(&ioexpander1_data, 5, false);

}
TimerHandle_t xMusicTimer;

extern uint32_t getDiffuseRTCCClockCount(int _sec, uint32_t *freq);

void adjustRTCCWithSystemClock()
{
    uint32_t rtcc_count;
    uint32_t freq;
    char ssbuf[128];
    uartcmd_keep_off();
    printLog(0, "MSG", "ADJUST RTCC FOR 6 SEC", LOG_TYPE_MESSAGE, NULL, 0);
    rtcc_count = getDiffuseRTCCClockCount(6, &freq);
    int32_t wantclock = ((int32_t) freq) * 6;
    int32_t diffuse;
    diffuse = wantclock - (int32_t) rtcc_count;
    diffuse = (int32_t) (((int64_t) diffuse * 32768 * 10) / (int64_t) freq);
    int32_t rawdiff = diffuse;
    if (diffuse >= 512) {
        diffuse = 511;
    } else if (diffuse < -511) {
        diffuse = -511;
    }
    snprintf(ssbuf, sizeof (ssbuf), "RTCC DIFF RAW=%d DIFFUSE=%ld SET=%d", rtcc_count, rawdiff, diffuse);
    printLog(0, "MSG", ssbuf, LOG_TYPE_MESSAGE, NULL, 0);

    if (RTCCONbits.ON) {
        SYS_RTCC_BCD_TIME _tim1, _tim2;
        SYS_RTCC_TimeGet(&_tim1);
        do {
            SYS_RTCC_TimeGet(&_tim2);
        } while (_tim1 == _tim2);
        while (!RTCCONbits.HALFSEC) {
        }
        RTCCONCLR = 0x03FF0000;
        RTCCONbits.CAL = 0;
        RTCCONbits.CAL = (int16_t) diffuse;
    }
}

void prvHouseKeeping(void *pvParameters)
{
    bool first = true;
    //bool first = false;
    bool time_set = false;
    BaseType_t stat;
    uint32_t tval[8];
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
    bool check_heap = false;
    int n;
    char *ps;
    SYS_RTCC_ALARM_HANDLE wakeupHandle;
    uint32_t rtcc_count;
    uint32_t rtcc_adjust_count = 0;
    int sndcmd;
    bool b_expander, b_lowvoltage, before_b_lowvoltage;
    bool ring_changed, debugsw_changed, battlost_changed, dipsw_changed;

    xWakeupTimerSemaphore = xSemaphoreCreateBinary();
    //xSemaphoreGive(xWakeupTimerSemaphore);
    i2cHandle = sv_open_i2c(I2C_USING_DRV);
    init_ioexpander();

    b_expander = b_lowvoltage = before_b_lowvoltage = false;
    ring_changed = debugsw_changed = battlost_changed = dipsw_changed = false;
    taskENTER_CRITICAL();
    interrupt_from_expander = false;
    b_ring = false;
    b_debugsw = false;
    b_battlost = false;
    val_dipsw = 0;
    f_interrupted = false;
    sleep_sec = 600;
    xMusicTimer = NULL;
    taskEXIT_CRITICAL();
    for (i = 0; i < LMT01_SENSOR_NUM; i++) {
        DRV_TEMP_LM01_Init(&(x_Temp[i]), (uint32_t) (i + 8), &DRV_PCA9655_SetPort, (void *) (&ioexpander1_data));
    }
    memset(pStrBufHK, 0x00, sizeof (pStrBufHK));
    memset(tval, 0x00, sizeof (tval));

    //TWE_Reset();
    //printMessage(0, NULL, "S");
    SLEEP_Periferals(true); // Disable unused periferals.
#if 0    
    SYS_DEVCON_SystemUnlock();
    PMD3bits.OC2MD = 1;
    PMD4bits.T3MD = 1;
    SYS_DEVCON_SystemLock();
#endif
    wakeupHandle = SYS_RTCC_AlarmRegister((SYS_RTCC_ALARM_CALLBACK) (&wakeupCallback), NULL);
    DOORBELL_DATA *pdd = getDoorbellData();
    SYS_RESET_ReasonClear(pdd->resetReason);

    uartcmd_on();
    RPA1Rbits.RPA1R = 0b0000; // Sound OFF
    vTaskDelay(cTick100ms);
    TWE_Reset();
    TWE_Wakeup(true);

    //uartcmd_keep_off();
    while (1) {
        SYS_WDT_TimerClear();
        SYS_WDT_Enable(false);
#if 0
        printMessage(0, NULL, "AAAAA");
        vTaskDelay(cTick100ms);
        continue;
#endif
        print_heap_left(check_heap);

        //CmdPlayMusic();
        if (first) {
            SYS_WDT_TimerClear();
            //uartcmd_keep_on();
            //vRingBufferClear_Char(&xUartRecvRing);
            xQueueReset(xUartRecvQueue);
            uartcmd_keep_off();
            wait_uart_ready(0);
            printLog(0, "BEGIN", "DOORBELL HOUSEKEEPING TASK", LOG_TYPE_MESSAGE, NULL, 0);
            wait_uart_ready(-1);
            if ((pdd->resetReason & RESET_REASON_WDT_TIMEOUT) != 0) {
                printLog(0, "MSG", "WDT EXPIRED.", LOG_TYPE_WDT_RESET, NULL, 0);
                SYS_WDT_TimerClear();
                //wait_uart_ready(-1);
            }
            if ((pdd->resetReason & 0x03) == RESET_REASON_BROWNOUT) {
                printLog(0, "MSG", "LOW VOLTAGE RESET HAPPENED.", LOG_TYPE_BOR_RESET, NULL, 0);
                //wait_uart_ready(-1);
            }
            i = 0;
            uartcmd_keep_off();
            check_ioexpander_flags(0);
            adjustRTCCWithSystemClock();

            SYS_WDT_TimerClear();
            do {
                SYS_WDT_TimerClear();
                uartcmd_keep_off();
                printLog(0, "REQ", "DATETIME WAIT 15 SEC", LOG_TYPE_MESSAGE, NULL, 0);
                wait_uart_ready(-1);
                printMessage(0, NULL, "OK");
                pStrBufHK[0] = '\0';
            _nn0:
                uartcmd_keep_on();
                n = recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick5Sec * 3);
                SYS_WDT_TimerClear();
                if (n > 0) {
                    if (checkSender(pStrBufHK, &hostnum, &ps, sizeof (pStrBufHK) / sizeof (char)) == N_HOST_COMPLETE) {
                        if (ps != NULL) {
                            if (!setDateTimeStr(ps)) {
                                uartcmd_keep_off();
                                wait_uart_ready(-1);
                                printLog(0, "ERR", "WRONG TIME STRING.", LOG_TYPE_MESSAGE, NULL, 0);
                                wait_uart_ready(-1);
                                retry++;
                                if (retry >= 5) break;
                            } else {
                                uartcmd_keep_off();
                                wait_uart_ready(-1);
                                printLog(0, "MSG", "TIME SET.", LOG_TYPE_MESSAGE, NULL, 0);
                                pass = true;
                                wait_uart_ready(-1);
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
                uartcmd_keep_off();
                wait_uart_ready(-1);
                if (timeout) {
                    printLog(0, "ERR", "TIME OUT.", LOG_TYPE_MESSAGE, NULL, 0);
                    wait_uart_ready(-1);
                }
                printLog(0, "ERR", "IGNORE TIME SETTING.", LOG_TYPE_MESSAGE, NULL, 0);
                wait_uart_ready(-1);
            }
            first = false;
        }
        {
            uartcmd_keep_off();
            if (!check_interrupts(0)) {
                check_ioexpander_flags(0);
            } else {
                while (check_interrupts(0)) {
                }
            }
        }

        SYS_RTCC_DateGet(&_nowdate);
        SYS_RTCC_TimeGet(&_nowtime);
        SYS_WDT_TimerClear();
        uartcmd_keep_off();
        printLog(0, "MSG", "WAKE UP", LOG_TYPE_MESSAGE, NULL, 0);
        wait_uart_ready(-1);
        {
            if (!check_interrupts(0)) {
                ;
                check_ioexpander_flags(0);
            } else {
                while (check_interrupts(0)) {
                }
            }
        }

        print_heap_left(check_heap);

        SYS_WDT_TimerClear();
        {
            int _n = (int) (val_dipsw & 0x03) + 1;
            if (_n > LMT01_SENSOR_NUM) _n = LMT01_SENSOR_NUM;
            if (_n <= 0) _n = 1;
            check_sensors(tval, _n, true);
        }
        {
            uartcmd_keep_off();
            if (!check_interrupts(0)) {
                ;
                check_ioexpander_flags(0);
            } else {
                while (check_interrupts(0)) {
                }
            }
        }
        SYS_WDT_TimerClear();
        //uartcmd_on();
        uartcmd_keep_off();
        printMessage(0, NULL, "READY");
        wait_uart_ready(-1);
        //vTaskDelay(cTick200ms);
        //if (xQueuePeek(xUartRecvQueue, pStrBufHK, cTick1Sec) == pdPASS) {
        debug_mode = false;
        i = 0;
        do {
            //            TWE_Wakeup(true);
            //uartcmd_on();
            uartcmd_keep_on();
            n = recvUartQueueDelim(pStrBufHK, sizeof (pStrBufHK) / sizeof (char), '\n', cTick1Sec * 1);
            if (n > 0) {
                int nn;
                ps = NULL;
                if (checkSender(pStrBufHK, &hostnum, &ps, sizeof (pStrBufHK) / sizeof (char)) == N_HOST_COMPLETE) {
                    if (ps != NULL) {
                        if (strlen(ps) > (3 + 19)) {
                            if (strncmp(ps, " T ", 3) == 0) {
                                ps = &(ps[3]);
                                if (setDateTimeStr(ps)) {
                                    uartcmd_keep_off();
                                    //wait_uart_ready(-1);
                                    printLog(0, "MSG", "TIME SET.", LOG_TYPE_MESSAGE, NULL, 0);
                                    //wait_uart_ready(-1);
                                }
                            } else {
                                // Another command.
                            }
                        }
                    }
                }
                uartcmd_keep_off();
                //wait_uart_ready(-1);
                snprintf(ssbuf, sizeof (ssbuf), "ECHO: %s", pStrBufHK);
                printLog(0, "MSG", ssbuf, LOG_TYPE_NOP, NULL, 0);
                //wait_uart_ready(-1);
                printMessage(0, NULL, "READY");
                //wait_uart_ready(-1);
                SYS_RTCC_DateGet(&_nowdate);
                SYS_RTCC_TimeGet(&_nowtime);
                SYS_WDT_TimerClear();
            }
            i++;
        } while (i < 3);
        uartcmd_on();
        uartcmd_keep_off();
        vTaskDelay(cTick100ms);
        debug_mode = false;
        {
            uartcmd_keep_off();
            if (!check_interrupts(0)) {
                check_ioexpander_flags(0);
            } else {
                while (check_interrupts(0)) {
                }
            }
        }
        if (f_interrupted) {
            rtcc_sleep = false;
        } else if (!rtcc_sleep) {
            rtcc_sleep = true;
        }
        SYS_WDT_TimerClear();
        if (rtcc_sleep) {
            rtcc_sleep = true;
            int left_sec;
            taskENTER_CRITICAL();
            if (sleep_sec < 15) {
                sleep_sec = 15;
            } else if (sleep_sec > (3600 * 12)) {
                sleep_sec = 3600 * 12;
            }
            rtcc_adjust_count += sleep_sec;
            left_sec = sleep_sec;
            taskEXIT_CRITICAL();
            nexttime = rtcAlarmSet(_nowtime, sleep_sec, false);
            //CmdStopMusic();
            print_heap_left(check_heap);
            //uartcmd_on();
            uartcmd_keep_on();
            snprintf(ssbuf, sizeof (ssbuf) / sizeof (char), "ENTER TO SLEEP UNTIL %d%d:%d%d:%d%d",
                    (nexttime >> 28) & 0x0f,
                    (nexttime >> 24) & 0x0f,
                    (nexttime >> 20) & 0x0f,
                    (nexttime >> 16) & 0x0f,
                    (nexttime >> 12) & 0x0f,
                    (nexttime >> 8) & 0x0f
                    );
            SYS_WDT_TimerClear();
            SYS_WDT_Enable(false);
            //wait_uart_ready(-1);
            printLog(0, "MSG", ssbuf, LOG_TYPE_MESSAGE, NULL, 0);
            //wait_uart_ready(-1);
            uartcmd_keep_off();
            vTaskDelay(cTick100ms);
            printMessage(0, NULL, "BYE");
            wait_uart_ready(-1);
            uartcmd_off();
            TWE_Wakeup(false);
            //I2C1CONbits.ON = 0;
            wakeupHandle = SYS_RTCC_AlarmRegister((SYS_RTCC_ALARM_CALLBACK) (&wakeupCallback), NULL);
            int rr_sec = left_sec;
            if (rr_sec >= 450) rr_sec = 450;
            uint32_t real_nexttime = rtcAlarmSet(_nowtime, rr_sec, true); // WDT is about 528Sec.
            left_sec = left_sec - rr_sec;
            bool need_later_exit = false;
            if(left_sec < 0) left_sec = 0;
            f_interrupted = false;
            while (1) {
                if (!f_interrupted) {
                    bool bs = true;
                    xQueueSend(xIdleSleepQueue, &bs, 0xffffffff);
                    vTaskDelay(cTick500ms);
                    SYS_RTCC_DateGet(&_nowdate);
                    SYS_RTCC_TimeGet(&_nowtime);
                    uint32_t nc = 0;
                    SYS_WDT_TimerClear();
                    SYS_WDT_Enable(false);
                    if (!check_interrupts(0)) {
                        while (xSemaphoreTake(xWakeupTimerSemaphore, cTick500ms) == pdFAIL) {
                            TWE_Wakeup(true);
                            if (check_interrupts(0)) {
                                need_later_exit = true;
                                goto _nl_exit;
                            }
                            check_ioexpander_flags(0);
                            TWE_Wakeup(false);
                            {
                                bool bs = true;
                                xQueueSend(xIdleSleepQueue, &bs, 0xffffffff);
                                vTaskDelay(cTick500ms);
                            }
                            nc++;
                        }
                        rr_sec = left_sec;
                        if(rr_sec >= 450) rr_sec = 450;
                        real_nexttime = rtcAlarmSet(_nowtime, rr_sec, true); // WDT is about 528Sec.
                        if (need_later_exit || (left_sec == 0)) goto _nl_exit;
                        left_sec -= rr_sec;
                        if (left_sec <= 0) {
                            left_sec = 0;
                            need_later_exit = true;
                        }
                    } else {
                        goto _nl_exit;
                    }
                }
                if (f_interrupted) goto _nl_exit;
                ;
            }
        _nl_exit:
            TWE_Wakeup(true);
            uartcmd_on();
            uartcmd_keep_off();
            {
                uartcmd_keep_off();
                if (!check_interrupts(0)) {
                    ;
                    check_ioexpander_flags(0);
                } else {
                    while (check_interrupts(0)) {
                    }
                }
            }
            SYS_WDT_TimerClear();
            if (rtcc_adjust_count >= (24 * 3600)) {
                rtcc_adjust_count = 0;
                // Adujust RTCC per a day.
                //adjustRTCCWithSystemClock();
            }
        } else {
            uartcmd_off();
            vTaskDelay(cTick1Sec);
            TWE_Reset();
            uartcmd_on();
        }
    }
}

