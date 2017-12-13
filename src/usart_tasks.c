
/*
 * DOOR BELL PIC32 MX
 *  USART TASKS.
 * (C) 2017 K.Ohta <whatisthis.sowhat _at_ gmail.com>
 */


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

static char wrTmpUartBuf[4];

extern DRV_HANDLE xDevHandleUart_Recv;
extern RingBuffer_Char_t xUartRecvRing;
extern QueueHandle_t xUartSendQueue;

DRV_HANDLE xDevHandleUart_Recv;

ssize_t recvUartQueue(char *buf, ssize_t len, int timeout)
{
    ssize_t i = 0;
    int ncount = 0;
    bool b_stat;
    BaseType_t stat;
    //TickType_t prev;
    SYS_WDT_TimerClear();
    if (buf == NULL) return -1;
    if (len <= 0) return 0;
    //prev = xTaskGetTickCount();
    while (i < len) {
        b_stat = vRingBufferRead_Char(&xUartRecvRing, &(buf[i]));
        //stat = xQueueReceive(xUartRecvQueue, &(buf[i]), 10);
        if (b_stat) {
            i++;
        }
        ncount += 5;
        if (ncount >= timeout) break;
        //vTaskDelayUntil(&prev, 2);
        vTaskDelay(5);
        SYS_WDT_TimerClear();
    }
    if (i < len) {
        buf[i] = '\0';
    } else {
        buf[len - 1] = '\0';
        i = len - 1;
    }
    SYS_WDT_TimerClear();
    return i;
}

ssize_t recvUartQueueDelim(char *buf, ssize_t maxlen, char delim, uint32_t timeout)
{
    volatile bool b_stat;
    volatile ssize_t i = 0;
    uint32_t ncount = 0;
    BaseType_t stat;
    //TickType_t prev;
    SYS_WDT_TimerClear();
    if (buf == NULL) return -1;
    if (maxlen <= 0) return 0;
    vRingBufferClear_Char(&xUartRecvRing);
    //prev = xTaskGetTickCount();
    while (i < maxlen) {
        b_stat = vRingBufferRead_Char(&xUartRecvRing, &(buf[i]));
        //stat = xQueueReceive(xUartRecvQueue, &(buf[i]), 5);
        if (b_stat) {
            if (buf[i] == delim) {
                if ((i + 1) >= maxlen) {
                    buf[0] = '\0';
                    return 0;
                }
                buf[i + 1] = '\0';
                return i + 1;
            }
            i++;
            if (i >= maxlen) {
                buf[0] = '\0';
                return 0;
            }
        }
        ncount += 5;
        if (timeout != 0) {
            if (ncount >= timeout) {
                buf[i] = '\0';
                return -1;
            }
        }
        vTaskDelay(5);
        SYS_WDT_TimerClear();
    }
    SYS_WDT_TimerClear();
    buf[0] = '\0';
    return 0;
}

int checkSender(char *data, uint32_t *hostnum, char **ps, size_t maxlen)
{
    int i, j, k, l;
    int pre_case;
    uint32_t pval;
    uint32_t n;
    bool pflag = false;
    if ((data == NULL) || (hostnum == NULL)) return N_HOST_FAIL;
    k = 0;
    if (ps != NULL) *ps = NULL;
    *hostnum = 0;
    pre_case = N_UNSTABLE;
    for (i = 0; i < maxlen; i++) {
        if (pre_case == N_UNSTABLE) {
            if ((data[i] == '\n') || (data[i] == '\r') || (data[i] == '\0')) {
                return N_HOST_EOF;
            }
            if (data[i] == ':') {
                pre_case = N_PROMPT;
                k = i;
            }
            if (data[i] == '[') {
                pre_case = N_HOST_PROGRESS;
                k = i;
            }
        } else if (pre_case == N_PROMPT) {
            if (data[i] == '>') {
                //if (i > 9) {
                *ps = &(data[i + 1]);
                return N_PROMPT;
                //}
            }
        } else if (pre_case == N_HOST_PROGRESS) {
            if (!pflag) {
                if (data[i] == ':') {
                    pflag = true;
                }
            } else {
                if (data[i] == ']') {
                    // Let's Go
                    pval = 0;
                    l = 0;
                    for (j = (k + 1); j < (i - 1); j++) {
                        if ((data[j] < '0') && (data[j] > 'F')) {
                            *hostnum = 0;
                            return N_HOST_FAIL;
                        } else {
                            if ((data[j] >= 'A') && (data[j] <= 'F')) {
                                n = (uint32_t) (data[j] - 'A') + 10;
                            } else if ((data[j] >= '0') && (data[j] <= '9')) {
                                n = (uint32_t) (data[j] - '0');
                            } else {
                                *hostnum = 0;
                                return N_HOST_FAIL;
                            }
                            pval = n | pval;
                            l++;
                            if (l == 8) {
                                *hostnum = pval;
                                *ps = &(data[i + 1]);
                                return N_HOST_COMPLETE;
                            } else {
                                pval <<= 4;
                            }
                        }
                    }
                }
            }
        }
    }
    return N_HOST_FAIL;
}

uint32_t checkStrType(char c, uint32_t type)
{
    uint32_t beforetype = type;
    switch (c) {
    case '\0':
        beforetype = beforetype | N_STR_NUL | N_STR_END;
        beforetype = beforetype & ~N_STR_PROGRESS;
        break;
    case '\n':
        beforetype = beforetype | N_STR_LF | N_STR_END;
        beforetype = beforetype & ~N_STR_PROGRESS;
        break;
    case '\r':
        beforetype = beforetype | N_STR_CR;
        beforetype = beforetype & ~N_STR_PROGRESS;
        break;
    case '[':
        if ((beforetype & N_STR_PROMPT) == 0) {
            beforetype = beforetype | N_STR_MES;
            if ((beforetype & N_STR_OK) == 0) {
                beforetype = beforetype | N_STR_PROGRESS;
            }
        }
        break;
    case ']':
        if ((beforetype & N_STR_MES) != 0) {
            beforetype = beforetype & ~N_STR_PROGRESS;
            beforetype = beforetype | N_STR_OK;
        }
        break;
    case '>':
        if ((beforetype & N_STR_MES) == 0) {
            beforetype = beforetype | N_STR_PROMPT;
        }
    case ' ':
        if (((beforetype & N_STR_PROMPT) != 0) && ((beforetype & N_STR_PROGRESS) != 0)) {
            beforetype = beforetype | N_STR_OK;
            beforetype = beforetype & ~N_STR_PROGRESS;
        }
        break;
    default:
        break;
    }
    return beforetype;
}
DRV_HANDLE xDevHandleUart_Send;
extern bool uart_wakeup;

bool pushUartQueue1(char *str)
{
    ssize_t _len;
    int i;
    size_t tsize;
    BaseType_t stat;
    ssize_t bsize;
    size_t clen;
    bool bstat;
    if (str == NULL) return false;
    _len = strlen(str);
    i = 0;
    tsize = _len;
    
#if 1
    while (tsize > 0) {
        //b_stat = false;
        //while(uxQueueSpacesAvailable(xUartSendQueue) ) { vTaskDelay(cTick100ms / 4); }
        stat = xQueueSend(xUartSendQueue, &(str[i]), cTick100ms);
        if (stat != pdPASS) {
            continue;
        }
        i++;
        tsize--;
    }
#else
    i = 0;
    if(tsize > 0) {
        taskENTER_CRITICAL();
        bstat = uart_wakeup;
        uart_wakeup = true;
        taskEXIT_CRITICAL();
        if(!bstat) {
            TWE_Wakeup(true);
        }
    }
    while (tsize > 0) {
        //clen = (tsize >= 8) ? 8 : tsize;
        clen = _len;
        bsize = DRV_USART_Write(xDevHandleUart_Send, &(str[i]), sizeof (char) * clen);
        if (bsize != (sizeof (char) * clen)) {
            vTaskDelay(cTick100ms);
            continue;
        }
        i += clen;
        tsize -= clen;
        //vTaskDelay(4);
    }
    //vTaskDelay(cTick100ms * (_len / 4 + 1));
    //while(!U1STAbits.TRMT) { vTaskDelay(cTick100ms / 2); }
    taskENTER_CRITICAL();
    uart_wakeup = false;
    taskEXIT_CRITICAL();
    TWE_Wakeup(false);
#endif    
    return true;
}


