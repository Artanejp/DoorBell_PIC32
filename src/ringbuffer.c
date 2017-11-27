
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"
#include "doorbell.h"   // SYS function prototypes
#include "ringbuffer.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

bool vRingBufferCreate_Char(RingBuffer_Char_t *p, char *realBuffer, int size)
{
    if (p == NULL) return false;
    if (realBuffer == NULL) return false;
    if (size <= 0) return false;
    memset(realBuffer, 0x00, sizeof (char) * size);
    memset(p, 0x00, sizeof (RingBuffer_Char_t));

    p->baseaddr = realBuffer;
    p->members = size;
    return true;
}


bool vRingBufferRead_Char(RingBuffer_Char_t *ringBuffer, char *p)
{
    bool stat;
    taskENTER_CRITICAL();
    stat = vRingBufferRead_Char_ISR(ringBuffer, p);
    taskEXIT_CRITICAL();
    return stat;
}


bool vRingBufferWrite_Char(RingBuffer_Char_t *ringBuffer, char c)
{
    bool stat;
    taskENTER_CRITICAL();
    stat = vRingBufferWrite_Char_ISR(ringBuffer, c);
    taskEXIT_CRITICAL();
    return stat;
}

void vRingBufferClear_Char(RingBuffer_Char_t *p)
{
    taskENTER_CRITICAL();
    vRingBufferClear_Char_ISR(p);
    taskEXIT_CRITICAL();
}

