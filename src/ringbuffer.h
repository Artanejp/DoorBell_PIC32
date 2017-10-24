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

    // DO NOT INSIDE OF CRITICAL SECTION
    extern bool vRingBufferCreate_Char(RingBuffer_Char_t *p, char *realBuffer, int size);
    extern bool vRingBufferRead_Char(RingBuffer_Char_t *ringBuffer, char *p);
    extern bool vRingBufferWrite_Char(RingBuffer_Char_t *ringBuffer, char c);
    extern void vRingBufferClear_Char(RingBuffer_Char_t *p);

#ifdef __cplusplus
}
#endif
#endif
