
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


#define TEST_FREQ 440
#define SAMPLE_FREQ 16000

static uint32_t render_sound(int16_t *data, uint32_t samples, int16_t vol, uint32_t *rmod, bool *ponoff)
{
    uint32_t freq = TEST_FREQ;
    uint32_t rp = 0;
    int16_t *p = data;
    int i;
    bool onoff = true;

    if (rmod == NULL) {
        rp = *rmod;
    }
    if (ponoff != NULL) {
        onoff = *ponoff;
    }
    {
        // First
        if (onoff) {
            *p += vol;
        } else {
            *p += 0;
        }
        onoff = !onoff;
    }
    for (i = 0; i < samples; i++) {
        if (rp >= (SAMPLE_FREQ / 2)) {
            rp -= (SAMPLE_FREQ / 2);
            onoff = !onoff;
        }
        *p++ += ((onoff) ? vol : 0);
        rp += freq;
    }

    if (rmod == NULL) {
        *rmod = rp;
    }
    if (ponoff != NULL) {
        *ponoff = onoff;
    }
    return samples;
}
static int s_count;
static uint32_t sound_rd_ptr;
static uint32_t sound_wr_ptr;
static int sound_data_length;
static bool sound_pending;

static void add_sound(SYS_DMA_CHANNEL_HANDLE handle, uint32_t sp, uint32_t len)
{
    static const uint32_t nlen = SOUND_LENGTH;
    uint32_t sl = nlen - sp;
    if (nlen <= sp) return;
    if (len > nlen) len = nlen;
    if (len > sl) {
        SYS_DMA_ChannelTransferAdd(handle, (const void *) (&(sample_buffer[sp])), sl * sizeof (int16_t), (const void *) (&OC2RS), sizeof (int16_t), sizeof (int16_t));
        SYS_DMA_ChannelTransferAdd(handle, (const void *) (&(sample_buffer[0])), (len - sl) * sizeof (int16_t), (const void *) (&OC2RS), sizeof (int16_t), sizeof (int16_t));
    } else {
        SYS_DMA_ChannelTransferAdd(handle, (const void *) (&(sample_buffer[sp])), len * sizeof (int16_t), (const void *) (&OC2RS), sizeof (int16_t), sizeof (int16_t));
    }
    taskENTER_CRITICAL();
    sp = sp + len;
    if (sp >= nlen) sp = sp - nlen;
    sound_rd_ptr = sp;
    sound_data_length -= ((int) len);
    if (sound_data_length < 0) sound_data_length = 0;
    taskEXIT_CRITICAL();
}

void sndDmaEventHandler(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
{
    static const int nlen = SOUND_LENGTH;
    int alen;
    int dlen;
    uint32_t dp;
    switch (event) {
    case SYS_DMA_TRANSFER_EVENT_COMPLETE:
        taskENTER_CRITICAL();
        dlen = sound_data_length;
        dp = sound_rd_ptr;
        alen = SOUND_LENGTH - sound_rd_ptr;
        taskEXIT_CRITICAL();
        if (dlen > 0) {
            add_sound(handle, dp, dlen);
            taskENTER_CRITICAL();
            sound_pending = false;
            taskEXIT_CRITICAL();
        } else {
            taskENTER_CRITICAL();
            sound_pending = true;
            taskEXIT_CRITICAL();
        }
        break;
    default:
        break;
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
    int i;
    bool pending;
    taskENTER_CRITICAL();
    s_count = 0;
    sound_rd_ptr = 0;
    sound_data_length = 0;
    sound_pending = true;
    taskEXIT_CRITICAL();
    p = sample_buffer;
    DRV_HANDLE tHandle = DRV_HANDLE_INVALID;
    DRV_HANDLE oHandle = DRV_HANDLE_INVALID;
    SYS_DMA_CHANNEL_HANDLE dHandle = SYS_DMA_CHANNEL_HANDLE_INVALID;

    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 5, true); // Audio OFF
    memset(sample_buffer, 0x00, sizeof (sample_buffer));
    rmod = 0;
    onoff = true;
    nlen = SOUND_LENGTH;
    state = C_SOUND_PLAY;
    //rlen = render_sound(sample_buffer, nlen, 255, &rmod, &onoff);
    while (1) {
        // Wait Queue
#if 0
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
                memset(&(p[ptr]), 0x00, nlen * sizeof (int16_t));
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
#else
        CmdQueue = C_SOUND_INIT;
        if (xQueueReceive(xSoundCmdQueue, (void *) (&CmdQueue), 0) == pdPASS) {
            switch (CmdQueue) {
            case C_SOUND_START:
                memset(sample_buffer, 0x00, sizeof (sample_buffer));
                dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
                tHandle = DRV_TMR_Open(DRV_TMR_INDEX_1, DRV_IO_INTENT_EXCLUSIVE);
                oHandle = DRV_OC_Open(DRV_OC_INDEX_0, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_EXCLUSIVE);
                if (tHandle != DRV_HANDLE_INVALID) {
                    DRV_TMR_CounterClear(tHandle);
                    DRV_TMR_CounterValueSet(tHandle, 1000);
                    DRV_TMR_Start(tHandle);
                }
                if(oHandle != DRV_HANDLE_INVALID) {
                    DRV_OC_PulseWidthSet(oHandle, 10);
                    DRV_OC_Start(DRV_OC_INDEX_0, DRV_IO_INTENT_READWRITE);
                }
                if (dHandle != SYS_DMA_CHANNEL_HANDLE_INVALID) {
                    SYS_DMA_ChannelTransferAdd(dHandle, (const void *) (sample_buffer), sizeof (sample_buffer), (const void *) (&OC2RS), sizeof (int16_t), sizeof (int16_t));
                    SYS_DMA_ChannelSetup(dHandle, SYS_DMA_CHANNEL_OP_MODE_BASIC, DMA_TRIGGER_TIMER_3);
                    SYS_DMA_ChannelEnable(dHandle);
                    SYS_DMA_ChannelTransferEventHandlerSet(dHandle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), 0);
                    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 5, false); // Audio ON
                    state = C_SOUND_PLAY;
                }
                break;
            case C_SOUND_ABORT:
            case C_SOUND_STOP:
                if (tHandle != DRV_HANDLE_INVALID) {
                    DRV_TMR_Stop(tHandle);
                    DRV_TMR_Close(tHandle);
                }
                if (oHandle != DRV_HANDLE_INVALID) {
                    DRV_OC_Stop(oHandle);
                    DRV_OC_Close(oHandle);
                }
                if(dHandle != SYS_DMA_CHANNEL_HANDLE_INVALID) {
                    SYS_DMA_ChannelForceAbort(dHandle);
                    SYS_DMA_ChannelDisable(dHandle);
                    SYS_DMA_ChannelRelease(dHandle);
                    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 5, true); // Audio OFF
                    dHandle = SYS_DMA_CHANNEL_HANDLE_INVALID;
                }
                state = C_SOUND_INIT;
                tHandle = DRV_HANDLE_INVALID;
                oHandle = DRV_HANDLE_INVALID;
                break;
            default:
                break;
            }
        }
        if (state == C_SOUND_PLAY) {
            if (dHandle == SYS_DMA_CHANNEL_HANDLE_INVALID) {
                vTaskDelay(cTick100ms);
            } else {
                taskENTER_CRITICAL();
                i = (int) sound_data_length;
                pending = sound_pending;
                taskEXIT_CRITICAL();
                if (i < nlen) {
                    i = nlen - i;
                    if (i > (nlen / 2)) i = i - nlen / 2;
                    memset(&(sample_buffer[s_count]), 0x00, i * sizeof (int16_t));
                    rlen = render_sound(&(sample_buffer[s_count]), i * sizeof (int16_t), 512, &rmod, &onoff);
                    if (pending) {
                        uint32_t sp, sr;
                        taskENTER_CRITICAL();
                        sp = sound_rd_ptr;
                        sr = sound_data_length;
                        taskEXIT_CRITICAL();
                        if (rlen > 0) {
                            sr = sr + rlen;
                            if (sr > nlen) sr = nlen;
                            add_sound(dHandle, sp, sr);
                            taskENTER_CRITICAL();
                            sound_pending = false;
                            taskEXIT_CRITICAL();
                        } else if (sr > 0) {
                            add_sound(dHandle, sp, sr);
                            taskENTER_CRITICAL();
                            sound_pending = false;
                            taskEXIT_CRITICAL();
                        }
                    } else {
                        taskENTER_CRITICAL();
                        if (rlen > 0) sound_data_length += rlen;
                        taskEXIT_CRITICAL();
                    }
                    taskENTER_CRITICAL();
                    s_count += rlen;
                    if (s_count >= nlen) {
                        //s_count = s_count - nlen;
                        s_count = 0;
                    }
                    taskEXIT_CRITICAL();
                } else {
                    uint32_t sp;
                    taskENTER_CRITICAL();
                    sp = sound_rd_ptr;
                    taskEXIT_CRITICAL();
                    if (pending) {
                        add_sound(dHandle, sp, nlen);
                    }
                }
            }
        } else {
            vTaskDelay(cTick100ms);
        }
    }
#endif

}
