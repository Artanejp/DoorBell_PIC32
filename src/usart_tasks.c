
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
static char rdStrBuf[128];
extern DRV_HANDLE xDevHandleUart_Recv;
extern DRV_HANDLE xDevHandleUart_Send;
//extern QueueHandle_t xUartSendQueue;
//extern QueueHandle_t xUartRecvQueue;
extern RingBuffer_Char_t  xUartRecvRing;

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

enum {
    UART_PHASE_INITIAL = 0,
    UART_PHASE_WAIT_FOR_PROMPT,
    UART_PHASE_WAIT_FOR_CMD,
    UART_PHASE_SEND_DATA,
};

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

enum {
    N_STR_BEGIN = 0,
    N_STR_PROGRESS = 2,
    N_STR_CR = 0x20, //\n
    N_STR_LF = 0x40, //\r
    N_STR_NUL = 0x80,
    N_STR_OK = 0x100,
    N_STR_MES = 0x2000, // [
    N_STR_PROMPT = 0x4000, // >
    N_STR_END = 0x8000,
};

typedef struct {
    size_t size;
    size_t ptr;
    char *buf;
} strpacket_t;

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
            beforetype = beforetype | N_STR_PROMPT | N_STR_OK;
            beforetype = beforetype & ~N_STR_PROGRESS;
        }
        break;
    default:
        break;
    }
    return beforetype;
}

bool pushUartQueue1(char *str)
{
    ssize_t _len;
    BaseType_t stat;
    int i;
    size_t bsize;
    size_t nsize;
    size_t tsize;
    size_t tmpsize;
    DRV_USART_BUFFER_HANDLE bhandle;
    
    if (str == NULL) return false;
    if (xDevHandleUart_Send == DRV_HANDLE_INVALID) return false;
    _len = strlen(str);
    i = 0;
    tsize = _len;
    while(tsize > 0) {
        bsize = DRV_USART_Write(xDevHandleUart_Send, &(str[i]), sizeof(char));
        if(bsize != sizeof(char)) {
            vTaskDelay(cTick100ms / 4);
            continue;
        }
        //vTaskDelay(2);
        i++;
        tsize--;
    }
    return true;
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
    volatile uint32_t strtype = N_STR_BEGIN;
    int sptr = 0;
    int ssptr;
    bool brf;
    DRV_USART_BUFFER_HANDLE bhandle;
    size_t bsize;
    while (1) {
        brf = false;
        if (xDevHandleUart_Recv != DRV_HANDLE_INVALID) {
            //_len = DRV_USART_Read(xDevHandleUart_Recv, &(rdTmpUartBuf[0]), sizeof (char));
#if 0            
            if (!DRV_USART_TransmitBufferIsFull(xDevHandleUart_Recv)) {
                stat = xQueueReceive(xUartSendQueue, wrTmpUartBuf, cTick100ms / 2);
                if (stat == pdPASS) {
                    brf = true;
                    DRV_USART_WriteByte(xDevHandleUart_Recv, wrTmpUartBuf[0]);
                } else {
                    //vTaskDelay(cTick100ms);
                }
            }
#endif
            bsize = DRV_USART_Read(xDevHandleUart_Recv, (void *)rdTmpUartBuf, sizeof(char));
            if(bsize >= sizeof(char)) {
#if 1
                _len = 1;
                {
                    i = 0;
                    //while (i < _len) {
                    rdStrBuf[sptr] = rdTmpUartBuf[0];
                    sptr++;
                    if (sptr >= (sizeof (rdStrBuf) / sizeof (char))) {
                        sptr = 0;
                        strtype = N_STR_BEGIN;
                        memset(rdStrBuf, 0x00, sizeof (rdStrBuf));
                        continue;
                    }
                    strtype = checkStrType(rdTmpUartBuf[0], strtype);

                    //if ((strtype & N_STR_END) != 0) {
                    //    continue;
                    //}
                    //i++;
                    //}
                    if ((strtype & N_STR_END) != 0) {
                        ssptr = 0;
                        if ((strtype & N_STR_MES) != 0) {
                            if ((strtype & N_STR_OK) != 0) {
                                for (ssptr = 0; ssptr < sptr; ssptr++) {
                                    //b_stat = false;
                                    stat = !pdPASS;
                                    do {
                                        //stat = xQueueSend(xUartRecvQueue, &(rdStrBuf[ssptr]), 4);
                                        b_stat = vRingBufferWrite_Char(&xUartRecvRing, rdStrBuf[ssptr]);
                                        if (b_stat) break;
                                        vTaskDelay(cTick100ms / 4);
                                    } while (stat != pdPASS);
                                }
                                asm volatile("NOP");
                            }
                        }
                        sptr = 0;
                        strtype = N_STR_BEGIN;
                        memset(rdStrBuf, 0x00, sizeof (rdStrBuf));
                    }
                    brf = true;
                    //vTaskDelay(2);
                }
#else
                if(rdTmpUartBuf[0] != '\0') {
                    char n[2];
                    n[0] = rdTmpUartBuf[0];
                    n[1] = '\0';
                    DRV_USART_Write(xDevHandleUart_Recv, n, sizeof(char));
                do {
                    stat = xQueueSend(xUartRecvQueue, &(rdStrBuf[ssptr]), 0);
                    //b_stat = vRingBufferWrite_Char(&xUartRecvRing, rdStrBuf[ssptr]);
                    //if (b_stat) break;
                    vTaskDelay(cTick100ms);
                } while (stat != pdPASS);
                }
                brf = true;
#endif
            }
            if (!brf) vTaskDelay(cTick500ms);
        } else {
            vTaskDelay(cTick500ms);
        }
    }
}

