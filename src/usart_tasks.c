
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

/* Standard demo includes. */
//#include "partest.h"

/* Hardware specific includes. */
//#include "ConfigPerformance.h"
static char rdTmpUartBuf[4];
static char wrTmpUartBuf[4];
extern RingBuffer_Char_t xUartRecvRing;
extern DRV_HANDLE xDevHandleUart_Recv;
extern DRV_HANDLE xDevHandleUart_Send;

ssize_t recvUartQueue(char *buf, ssize_t len, int timeout)
{
    ssize_t i = 0;
    int ncount = 0;
    bool b_stat;
    BaseType_t stat;
    //TickType_t prev;
    if (buf == NULL) return -1;
    if (len <= 0) return 0;
    //prev = xTaskGetTickCount();
    while (i < len) {
        b_stat = vRingBufferRead_Char(&xUartRecvRing, &(buf[i]));
        if (b_stat) {
            i++;
            ncount += 10;
        } else {
            ncount += 10;
        }
        if (ncount >= timeout) break;
        //vTaskDelayUntil(&prev, 2);
        vTaskDelay(10);
    }
    if (i < len) {
        buf[i] = '\0';
    }
    return i;
}

ssize_t recvUartQueueDelim(char *buf, ssize_t maxlen, char delim, int timeout)
{
    volatile bool b_stat;
    volatile ssize_t i = 0;
    volatile int ncount = 0;
    BaseType_t stat;
    //TickType_t prev;
    if (buf == NULL) return -1;
    if (maxlen <= 0) return 0;

    //prev = xTaskGetTickCount();
    while (i < maxlen) {
        b_stat = vRingBufferRead_Char(&xUartRecvRing, &(buf[i]));
        if (b_stat) {
            if (buf[i] == delim) {
                if ((i + 1) >= maxlen) {
                    buf[0] = '\0';
                    return 0;
                }
                buf[i + 1] = '\0';
                return i;
            }
            i++;
            if (i >= maxlen) {
                buf[0] = '\0';
                return 0;
            }
            ncount += 2;
        } else {
            ncount += 2;
        }
        if ((ncount >= timeout) && (timeout > 0)) return 0;
        vTaskDelay(2);
    }
    return i;
}

enum {
    UART_PHASE_INITIAL = 0,
    UART_PHASE_WAIT_FOR_PROMPT,
    UART_PHASE_WAIT_FOR_CMD,
    UART_PHASE_SEND_DATA,
};

int checkSender(char *data, uint32_t *hostnum, size_t maxlen)
{
    int i, j;
    int pre_case;
    uint32_t pval;
    uint32_t n;
    if ((data == NULL) || (hostnum == NULL)) return N_HOST_FAIL;

    switch (data[0]) {
    case '[':
        pre_case = N_HOST_PROGRESS;
        break;
    default:
        pre_case = N_UNSTABLE;
        break;
    }
    for (i = 0; i < maxlen; i++) {
        if ((data[i] == '\n') || (data[i] == '\r') || (data[i] == '\0')) {
            return N_HOST_EOF;
        }
        if (pre_case == N_UNSTABLE) {
            if (data[i] == ':') {
                if (i == 8) pre_case = N_PROMPT;
            }
        } else if (pre_case == N_PROMPT) {
            if (data[i] == '>') {
                if (i > 9) {
                    *hostnum = (uint32_t) i + 1;
                    return N_PROMPT;
                }
            }
        } else if (pre_case == N_HOST_PROGRESS) {
            if ((data[i] == ']') && (i > 10)) {
                if (data[9] == ':') {
                    // Let's Go
                    pval = 0;
                    for (j = 1; j < 9; j++) {

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
                            if (j == 8) {
                                *hostnum = pval;
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

int uartSendData_HK()
{
    BaseType_t stat;
    ssize_t _wlen = 0;
    int i = 0;
    if (xUartSendQueue != NULL) {
        stat = xQueueReceive(xUartSendQueue, wrTmpUartBuf, cTick500ms);
        if (stat == pdTRUE) {
            if (xDevHandleUart_Send != DRV_HANDLE_INVALID) {
                _wlen = 0;
                while (_wlen <= 0) {
                    _wlen = DRV_USART_Write(xDevHandleUart_Send, wrTmpUartBuf, sizeof (char));
                    if (_wlen <= 0) vTaskDelay(cTick100ms);
                    i++;
                    if (i > 32) break; // Retry 32 times failed.
                }
                return _wlen;
            } else {
                return -1; // Nothing to SEND
            }
        } else {
            return -1; // Nothing to SEND
        }
    }
    return -1;
}

void prvReadFromUart_HK(void *pvparameters)
{
    size_t _len = 0;
    BaseType_t stat;
    UBaseType_t avail;
    bool b_stat;
    int i = 0;
    int j = 0;
    char c;
    bool nsync = false;
    while (1) {
        if (xDevHandleUart_Recv != DRV_HANDLE_INVALID) {
            _len = DRV_USART_Read(xDevHandleUart_Recv, &(rdTmpUartBuf[0]), sizeof (char));
            if ((_len > 0) && (_len != DRV_USART_READ_ERROR)) {
                    i = 0;
                    while (i < _len) {
                        b_stat = vRingBufferWrite_Char(&xUartRecvRing, rdTmpUartBuf[i]);
                        if (b_stat) {
                            i++;
                        }
                    }
            } else {
                vTaskDelay(cTick200ms);
            }
        } else {
            vTaskDelay(cTick500ms);
        }
    }
}

void prvWriteToUart_HK(void *pvparameters)
{
    int rcount = 0;
    while (1) {
        if (xUartSendQueue != NULL) {
            uartSendData_HK();
            //rcount++;
            //if(rcount >16) {
            //    rcount = 0;
            //    vTaskDelay(cTick200ms);
            //}
        } else {
            vTaskDelay(cTick500ms);
        }
    }
}
