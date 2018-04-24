#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "doorbell.h"   // SYS function prototypes

bool CheckLowVoltage(void)
{
    bool status = (LVINStateGet() == 0);
    return status;
}

void uartcmd_on(void)
{
    char flag = C_TWE_ON;
    BaseType_t stat = !pdPASS;
    stat = xQueueSend(xUartSendCmdQueue, &flag, -1);
    vTaskDelay(cTick200ms);
    //}
}

void uartcmd_keep_on(void)
{
    char flag = C_TWE_FULL_RUN;
    BaseType_t stat = !pdPASS;
    stat = xQueueSend(xUartSendCmdQueue, &flag, -1);
}

void uartcmd_keep_off(void)
{
    char flag = C_TWE_IDLE_ON;
    BaseType_t stat = !pdPASS;
    stat = xQueueSend(xUartSendCmdQueue, &flag, -1);
}

void uartcmd_off(void)
{
    char flag = C_TWE_FORCE_SLEEP;
    BaseType_t stat = !pdPASS;
    stat = xQueueSend(xUartSendCmdQueue, &flag, -1);
    vTaskDelay(cTick200ms);
}

/* 
 * To/From I/O expander. 
 */
extern DRV_TEMP_LM01_T x_Temp[LMT01_SENSOR_NUM];

void check_sensors(uint32_t *ptval, int num, bool logging)
{
    int i;
    if (ptval == NULL) return;
    if (num >= LMT01_SENSOR_NUM) num = LMT01_SENSOR_NUM;
    if (num <= 0) return;
    if (num > 8) num = 8;

    I2C1CONbits.ON = 1;
    SYS_WDT_Enable(false);
    SYS_WDT_TimerClear();
    DRV_PCA9655_Reset(&ioexpander1_data, 0, 0, false); // Reset I/O Expander.
#if 0
    if (logging) {
        TWE_Wakeup(true);
        uartcmd_keep_off();
        wait_uart_ready(-1);
        vTaskDelay(cTick100ms);
    }
#endif
    SYS_WDT_TimerClear();
    for (i = 0; i < num; i++) {
        SYS_WDT_Enable(false);
        SYS_WDT_TimerClear();
        if (DRV_TEMP_LM01_StartConversion(&(x_Temp[i]))) {
            SYS_WDT_TimerClear();
            ptval[i] = DRV_TEMP_LM01_EndConversion(&(x_Temp[i]));
            SYS_WDT_TimerClear();
            SYS_WDT_Enable(false);
            if (logging) {
                uartcmd_keep_off();
                wait_uart_ready(-1);
                printThermalLMT01(0, i, ptval[i]); // USB
                wait_uart_ready(-1);
            }
            SYS_WDT_TimerClear();
        }
    }
    SYS_WDT_TimerClear();
    SYS_WDT_Enable(false);
    I2C1CONbits.ON = 0;
}

void debug_ioexpander(bool do_check)
{
    uint8_t pca9655_regs[8];
    char pStrBufHK[64];

    if (!do_check) return;
    SYS_WDT_TimerClear();
    SYS_WDT_Enable(false);
    I2C1CONbits.ON = 1;
    DRV_PCA9655_GetRegs(&ioexpander1_data, pca9655_regs);
    I2C1CONbits.ON = 0;
    SYS_WDT_TimerClear();
    SYS_WDT_Enable(false);

    uartcmd_on();
    printLog(0, "DEBUG", "PCA6955 REGS   0  1  2  3", LOG_TYPE_MESSAGE, NULL, 0);

    snprintf(pStrBufHK, 48, "PCA9655 REG +0: %02x %02x %02x %02x", pca9655_regs[0], pca9655_regs[1], pca9655_regs[2], pca9655_regs[3]);
    uartcmd_on();

    printLog(0, "DEBUG", pStrBufHK, LOG_TYPE_MESSAGE, NULL, 0);
    snprintf(pStrBufHK, 48, "PCA9655 REG +4: %02x %02x %02x %02x", pca9655_regs[4], pca9655_regs[5], pca9655_regs[6], pca9655_regs[7]);

    uartcmd_on();
    printLog(0, "DEBUG", pStrBufHK, LOG_TYPE_MESSAGE, NULL, 0);
    //vTaskDelay(cTick1Sec * 5);
}

/*
 * Debug OS.
 */
void print_heap_left(bool do_check)
{
    char strbuf[40];
    if (!do_check) return;
    snprintf(strbuf, sizeof (strbuf), "HEAP %d BYTES", xPortGetFreeHeapSize());
    uartcmd_keep_off();
    printMessage(0, "DEBUG", strbuf);
}

extern uint32_t sleep_sec;

bool check_main_power(bool _printlog, int ttynum, bool beforestat, bool force)
{
    bool _b;

    I2C1CONbits.ON = 1;
    _b = DRV_PCA9655_GetPort_Bit((void *) (&ioexpander1_data), 4); // BATTLOST, IO0_4
    I2C1CONbits.ON = 0;
    _b = !_b;
    if (_printlog) {
        if (force || (beforestat != _b)) {
            if (_b) {
                printLog(ttynum, "MSG", "MAIN POWER LOST", LOG_TYPE_MAINPOW_OFF, NULL, 0);
            } else {
                printLog(ttynum, "MSG", "MAIN POWER REVIVED", LOG_TYPE_MAINPOW_ON, NULL, 0);
            }
        }
    }
    if (_b != beforestat) {
        if (_b) {
            taskENTER_CRITICAL();
            sleep_sec = 3600; // 60 Min
            taskEXIT_CRITICAL();
        } else {
            taskENTER_CRITICAL();
            //sleep_sec = 600; // 10 Min
            sleep_sec = 600; // 5 Min
            taskEXIT_CRITICAL();
        }
    }
    return _b;
}

bool check_debug_sw(bool _printlog, int ttynum, bool beforestat, bool force)
{
    bool _b;

    I2C1CONbits.ON = 1;
    _b = DRV_PCA9655_GetPort_Bit((void *) (&ioexpander1_data), 6); // DEBUG, IO0_6
    I2C1CONbits.ON = 0;
    _b = !_b;
    if (_printlog) {
        if (force || (beforestat != _b)) {
            if (_b) {
                printLog(ttynum, "EVENT", "DEBUG SWITCH PRESSED", LOG_TYPE_PUSH_DEBUG, NULL, 0);
            } else {
                printLog(ttynum, "EVENT", "DEBUG SWITCH RELEASED", LOG_TYPE_RELEASE_DEBUG, NULL, 0);
            }
        }
    }
    return _b;
}

uint8_t check_dipswitch(bool _printlog, int ttynum, uint8_t before_val)
{
    uint8_t _n;
    I2C1CONbits.ON = 1;
    _n = DRV_PCA9655_GetPort_Uint8((void *) (&ioexpander1_data), 1);
    I2C1CONbits.ON = 0;
    _n = (_n >> 4) & 0x0f;
    if (_printlog && (_n != (before_val & 0x0f))) {
        char strbuf[64];
        snprintf(strbuf, sizeof (strbuf) - 1, "DIPSW STATUS ARE %s %s SENSORS=%d",
                ((_n & 0x08) != 0) ? "DBG" : "***",
                ((_n & 0x04) != 0) ? "RES" : "***",
                (_n & 0x03) + 1);
        printLog(ttynum, "INFO", strbuf, LOG_TYPE_MESSAGE, &_n, 1);
    }
    return _n;
}

extern bool interrupt_from_lowvoltage;
extern bool b_battlost;
extern bool b_debugsw;
extern uint8_t val_dipsw;
extern bool f_interrupted;
extern TimerHandle_t xMusicTimer;

extern uint32_t sleep_sec;

extern void CmdPlayMusic();
extern void CmdStopMusic();

static TimerCallbackFunction_t pvMusicTimerCallback(TimerHandle_t xTimer)
{
    CmdStopMusic();
    printLog(0, "INFO", "MUSIC STOPPED", LOG_TYPE_SOUND_OFF, NULL, 0);
    if (xMusicTimer != NULL) {
        //xTimerStop(xMusicTimer, cTick1Sec);
        xTimerDelete(xMusicTimer, cTick1Sec * 10);
        xMusicTimer = NULL;
    }
    taskENTER_CRITICAL();
    f_interrupted = false;
    taskEXIT_CRITICAL();
}

void check_ioexpander_flags(int ttynum)
{
    bool _lostpower;
    bool _debugsw;
    uint8_t _dipsw;

    taskENTER_CRITICAL();
    _lostpower = b_battlost;
    _debugsw = b_debugsw;
    _dipsw = val_dipsw;
    taskEXIT_CRITICAL();

    _lostpower = check_main_power(true, ttynum, _lostpower, false);
    _debugsw = check_debug_sw(true, ttynum, _debugsw, false);

    _dipsw = check_dipswitch(true, ttynum, _dipsw);
    taskENTER_CRITICAL();
    b_battlost = _lostpower;
    b_debugsw = _debugsw;
    val_dipsw = _dipsw;
    taskEXIT_CRITICAL();

}

bool check_interrupts(int ttynum)
{
    BaseType_t stat;
    uint32_t _da = 0;
    bool _b;
    bool _ret = false;
    bool _lostpower, _debugsw;
    bool before_mainlost, before_debugsw;
    uint8_t _dipsw;

    stat = xQueueReceive(xPortInterruptQueue, (const void *) (&_da), 0);
    if (stat == pdPASS) {
        switch (_da & 0xf0000000) {
        case C_INT_RINGBUTTON:
            printLog(ttynum, "EVENT", "RING BUTTON PRESSED", LOG_TYPE_PUSH_RING, NULL, 0);
            CmdPlayMusic();
            if (xMusicTimer == NULL) {
                vTaskDelay(cTick100ms * 2);
                printLog(0, "INFO", "MUSIC STARTED", LOG_TYPE_SOUND_OFF, NULL, 0);
                xMusicTimer = xTimerCreate((const char *) "Music Timer", cTick1Sec * 60, pdFALSE, (const void *) NULL, pvMusicTimerCallback);
                if (xMusicTimer != NULL) {
                    xTimerStart(xMusicTimer, cTick1Sec);
                    taskENTER_CRITICAL();
                    f_interrupted = true;
                    taskEXIT_CRITICAL();
                }
            }
            _ret = true;
            break;
        case C_INT_LOWVOLTAGE:
            _b = CheckLowVoltage();
            if (_b != interrupt_from_lowvoltage) {
                if (_b) {
                    printLog(ttynum, "EVENT", "LOW VOLTAGE HAPPENED", LOG_TYPE_BACKUP_LOW, NULL, 0);
                } else {
                    printLog(ttynum, "EVENT", "LOW VOLTAGE CANCELED", LOG_TYPE_BACKUP_HIGH, NULL, 0);
                }
            }
            _ret = true;
            interrupt_from_lowvoltage = _b;
            break;
        case C_INT_IOEXPANDER:
            check_ioexpander_flags(ttynum);
            _ret = true;
            break;
        default:
            break;
        }
    }
    return _ret;
}

void TWE_Reset(void)
{
    EXT_RESETStateSet(false);
    vTaskDelay(cTick100ms);
    EXT_RESETStateSet(true);
    vTaskDelay(cTick500ms);
}


