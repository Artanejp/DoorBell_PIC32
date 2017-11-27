/*
 * ringbuffer.h
 * Ringbuffer for FreeRTOS.
 * (C) 2017 K.Ohta <whatisthis.sowhat _at_ gmail.com>
 * License: Apache
 */

#ifndef __RTOS_RINGBUFFER_H
#define __RTOS_RINGBUFFER_H
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct {
        char *baseaddr;
        int32_t members;
        int32_t read_ptr;
        int32_t write_ptr;
        int32_t read_size;
        int32_t write_size;
        int32_t remain;
    } RingBuffer_Char_t;

    static __inline__ bool vRingBufferRead_Char_ISR(RingBuffer_Char_t *ringBuffer, char *p) {
        int np;
        char *vp;
        bool stat = false;
        if (p == NULL) return false;
        if (ringBuffer == NULL) return false;
        vp = ringBuffer->baseaddr;
        if ((ringBuffer->remain <= 0) || (vp == NULL)) {
            // Empty
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
        return stat;
    }

    static __inline__ bool vRingBufferWrite_Char_ISR(RingBuffer_Char_t *ringBuffer, char c) {
        int np;
        char *vp;
        if (ringBuffer == NULL) return false;
        vp = ringBuffer->baseaddr;
        if (vp == NULL) {
            // Empty
            return false;
        }
        // If fill overwrite.
        np = ringBuffer->write_ptr;
        vp[np] = c;
        ringBuffer->write_ptr++;
        if (ringBuffer->write_ptr >= ringBuffer->members) ringBuffer->write_ptr = 0;
        ringBuffer->remain++;
        if (ringBuffer->remain > ringBuffer->members) ringBuffer->remain = ringBuffer->members;
        return true;
    }

    static __inline__ void vRingBufferClear_Char_ISR(RingBuffer_Char_t *p)
    {
        int i;
        char *vp = p->baseaddr;
        for (i = 0; i < p->members; i++) {
            vp[i] = '\0';
        }
        p->remain = 0;
        p->write_ptr = 0;
        p->read_ptr = 0;
        p->read_size = 0;
        p->write_size = 0;
    }

    extern bool vRingBufferCreate_Char(RingBuffer_Char_t *p, char *realBuffer, int size);
    extern bool vRingBufferRead_Char(RingBuffer_Char_t *ringBuffer, char *p);
    extern bool vRingBufferWrite_Char(RingBuffer_Char_t *ringBuffer, char c);
    extern void vRingBufferClear_Char(RingBuffer_Char_t *p);

#ifdef __cplusplus
}
#endif
#endif
