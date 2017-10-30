
/*
 * Sounder routine
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


QueueHandle_t xSoundCmdQueue;
QueueHandle_t xSoundQueue;
TimerHandle_t xSoundTimer;
const uint16_t sound_level_table[32] = {
    0, 5, 10, 15, 20, 25, 30, 35,
    41, 47, 53, 60, 67, 74, 81, 88,
    95, 103, 111, 119, 127, 136, 145,
    154, 164, 174, 185, 196, 208, 220, 235
};

uint16_t __attribute__((coherent)) __attribute__((aligned(16))) sample_buffer[SOUND_LENGTH + 32]; // 0.2sec

static uint32_t add_sound(int16_t *data, uint32_t samples)
{
    //
    sndData_t dq;
    int i;
    dq.cmd = C_SOUND_START;
    dq.ptr = data;
    dq.len = samples;
    
    for(i = 0; i < 10; i++) {
        if(xQueueSend(xSoundQueue, (void *)(&dq), cTick100ms / 5) == pdPASS) {
            return samples;
        }
    }
    return 0;
}

#define TEST_FREQ 440
#define SAMPLE_FREQ 16000

static uint32_t render_sound(int16_t *data, uint32_t samples, int16_t vol, uint32_t *rmod, bool *ponoff)
{
    uint32_t freq = TEST_FREQ;
    uint32_t rp = 0;
    int16_t *p = data;
    int i;
    bool onoff = true;
    
    if(rmod == NULL) {
        rp = *rmod;
    }
    if(ponoff != NULL) {
        onoff = *ponoff;
    }
    {
    // First
        if(onoff) {
            *p += vol;
        } else {
            *p += 0;
        }
        onoff = !onoff;
    }
    for(i = 0; i < samples; i++) {
        if(rp >= (SAMPLE_FREQ / 2)) {
            rp -= (SAMPLE_FREQ / 2);
            onoff = !onoff;
            *p++ += (onoff) ? vol : 0; 
        }
        rp += freq;
    }
    
    if(rmod == NULL) {
        *rmod = rp;
    }
    if(ponoff != NULL) {
        *ponoff = onoff;
    }
    return samples;
}
static int s_count;

void prvSoundDMA(void *pvParameters)
{
    sndData_t dq;
    bool before_start = false;
    SYS_DMA_CHANNEL_HANDLE dHandle;
    uint32_t dlen = 0;
    // ToDo: Stop Timer & OC.
    // ToDo: Init DMA.
    while (1) {
        if (xQueueReceive(xSoundQueue, (void *) (&dq), cTick100ms / 4) == pdPASS) {
            if (dq.cmd != C_SOUND_STOP) {
                // ToDo:
                // Start timer and OC.
                if (!before_start) {
                    dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_1);
                    if (dHandle == SYS_DMA_CHANNEL_HANDLE_INVALID) {
                        // Error Handling
                    } else {
                        before_start = true;
                        SYS_DMA_ChannelSetup(dHandle, SYS_DMA_CHANNEL_OP_MODE_BASIC, DMA_TRIGGER_OUTPUT_COMPARE_1);
                    }
                }
                if (before_start) {
                    SYS_DMA_ChannelTransferAdd(dHandle, (const void *) (dq.ptr), sizeof (int16_t) * dq.len, (const void *) (&OC1RS), sizeof (int16_t), 1);
                }
                vTaskDelay(cTick100ms / 4);
            } else {
                // ToDo: Stop Timer & OC.
                // Stop Sound
                before_start = false;
                SYS_DMA_ChannelForceAbort(dHandle);
                xQueueReset(xSoundQueue);
            }
        } else {
            if (!before_start) {
                vTaskDelay(cTick1Sec);
            }
        }
    }
}

void prvSound(void *pvParameters)
{
    int CmdQueue;
    int state = C_SOUND_STOP;
    int16_t *p;
    sndData_t dq;
    int nlen;
    uint32_t ptr;
    bool onoff;
    uint32_t rmod;
    uint32_t rlen;
    taskENTER_CRITICAL();
    s_count = 0;
    taskEXIT_CRITICAL();
    p = sample_buffer;
    while (1) {
        // Wait Queue
        if (xQueueReceive(xSoundCmdQueue, (void *) (&CmdQueue), cTick100ms / 4) == pdPASS) {
            // 
            switch (CmdQueue) {
            case C_SOUND_START:
                printLog(0, "MSG", "START SOUND", LOG_TYPE_SOUND_ON, NULL, 0);
                state = C_SOUND_START;
                p = &(sample_buffer[0]);
                nlen = SOUND_LENGTH / 2;
                taskENTER_CRITICAL();
                s_count = 0;
                taskEXIT_CRITICAL();
                onoff = true;
                rmod = 0;
                ptr = 0;
                p = sample_buffer;
                break;
            case C_SOUND_STOP:
                printLog(0, "MSG", "STOP SOUND", LOG_TYPE_SOUND_OFF, NULL, 0);
                state = C_SOUND_STOP;
                dq.cmd = C_SOUND_STOP;
                dq.len = 0;
                taskENTER_CRITICAL();
                s_count = 0;
                taskEXIT_CRITICAL();
                onoff = false;
                rmod = 0;
                ptr = 0;
                nlen = 0;
                p = sample_buffer;
                dq.ptr = NULL;
                while (xQueueSend(xSoundQueue, (void *) (&dq), cTick1Sec) != pdPASS) {
                    vTaskDelay(cTick100ms / 2);
                }
                break;
            }
        }
    _nn1:
        if (state == C_SOUND_START) {
            if (s_count <= (SOUND_LENGTH / 2)) {
                memset(&(p[ptr]), 0x00, nlen * sizeof(int16_t));
                rlen = render_sound(&(p[ptr]), nlen, 255, &rmod, &onoff);
                add_sound(&(p[ptr]), rlen);
                taskENTER_CRITICAL();
                s_count += rlen;
                taskEXIT_CRITICAL();
                ptr += rlen;
                if (ptr >= SOUND_LENGTH) {
                    ptr = 0;
                }
            } else {
                vTaskDelay(cTick100ms / 2); // 20ms
                // ToDo: Render when half transferred
                taskENTER_CRITICAL();
                s_count -= nlen;
                taskEXIT_CRITICAL();
            }
        } else {
            vTaskDelay(cTick1Sec);
        }
    }
}
