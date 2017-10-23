
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
    int np;
    char *vp;
    bool stat = false;
    if (p == NULL) return false;
    if (ringBuffer == NULL) return false;
    taskENTER_CRITICAL();
    vp = ringBuffer->baseaddr;
    if ((ringBuffer->remain <= 0) || (vp == NULL)) {
        // Empty
        taskEXIT_CRITICAL();
        return false;
    }
    if (ringBuffer->remain > 0) {
        np = ringBuffer->read_ptr;
        *p = vp[np];
        stat = true;
        ringBuffer->read_ptr++;
        ringBuffer->remain--;
    } else {
        stat = false;
    }
    if (ringBuffer->read_ptr > ringBuffer->members) ringBuffer->read_ptr = 0;
    if (ringBuffer->remain < 0) ringBuffer->remain = 0;
    taskEXIT_CRITICAL();
    return stat;
}

bool vRingBufferWrite_Char(RingBuffer_Char_t *ringBuffer, char c)
{
    int np;
    char *vp;
    if (ringBuffer == NULL) return false;
    taskENTER_CRITICAL();
    vp = ringBuffer->baseaddr;
    if (vp == NULL) {
        // Empty
        taskEXIT_CRITICAL();
        return false;
    }
    // If fill overwrite.
    np = ringBuffer->write_ptr;
    vp[np] = c;
    ringBuffer->write_ptr++;
    if (ringBuffer->write_ptr >= ringBuffer->members) ringBuffer->write_ptr = 0;
    ringBuffer->remain++;
    if (ringBuffer->remain > ringBuffer->members) ringBuffer->remain = ringBuffer->members;
    taskEXIT_CRITICAL();
    return true;
}
