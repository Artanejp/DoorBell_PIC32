
/*
 * Sounder routine
 * (C) 2017 K.Ohta <whatisthis.sowhat _at_ gmail.com>
 */
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <limits.h>
#include "system/common/sys_module.h"
#include "doorbell.h"   // SYS function prototypes
#include "lm01_drv.h"
#include "pca9655.h"
#include "sounder.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


QueueHandle_t xSoundCmdQueue;
QueueHandle_t xSoundQueue;
TimerHandle_t xSoundTimer;

static uint32_t scallback_count;
static int render_slot;
static int play_avail;
static bool render_flag[4];
static uint32_t wrote_bytes;
static int16_t __attribute__((aligned(16))) sample_buffer[SOUND_LENGTH + 32]; // 0.2sec

extern PCA9655_t ioexpander1_data;

void sndDmaEventHandler(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle);

int16_t *get_sound_buffer(void)
{
    return sample_buffer;
}

static __inline__ void add_sound_isr(SYS_DMA_CHANNEL_HANDLE handle, uint32_t slot)
{
    static const uint32_t nlen = SOUND_LENGTH;
    uint32_t sp = ((slot % 2) * nlen) / 2;
    int16_t *p = get_sound_buffer();
    if (p == NULL) return;
    SYS_DMA_ChannelTransferEventHandlerSet(handle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), slot);
    SYS_DMA_ChannelTransferSet(handle, (const void *) (&(p[sp])), (nlen * sizeof (int16_t)) / 2, (const void *) (&OC2RS), sizeof (int32_t), sizeof (int16_t));
    SYS_DMA_ChannelSetup(handle, SYS_DMA_CHANNEL_OP_MODE_BASIC, DMA_TRIGGER_TIMER_3);
    SYS_DMA_ChannelEnable(handle);
}

static void add_sound(SYS_DMA_CHANNEL_HANDLE handle, uint32_t slot)
{
    static const uint32_t nlen = SOUND_LENGTH;
    uint32_t sp = ((slot % 2) * nlen) / 2;
    int16_t *p = get_sound_buffer();
    
    if (p == NULL) return;
    SYS_DMA_ChannelTransferEventHandlerSet(handle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), slot);
    SYS_DMA_ChannelTransferSet(handle, (const void *) (&(p[sp])), (nlen * sizeof (int16_t)) / 2, (const void *) (&OC2RS), sizeof (int32_t), sizeof (int16_t));
    SYS_DMA_ChannelSetup(handle, SYS_DMA_CHANNEL_OP_MODE_BASIC, DMA_TRIGGER_TIMER_3);
    SYS_DMA_ChannelEnable(handle);
}

void sndDmaEventHandler(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle)
{
    static const int nlen = SOUND_LENGTH;
    uint32_t slot = (uint32_t) contextHandle;
    switch (event) {
    case SYS_DMA_TRANSFER_EVENT_COMPLETE:
        if (play_avail > 0) {
            slot++;
            slot = slot % 2;
            add_sound_isr(handle, slot);
            play_avail--;
        }
        if (play_avail < 0) play_avail = 0;
        break;
    case SYS_DMA_TRANSFER_EVENT_ERROR:
        //printLog(0, "MSG", "DMA TRANSFER ERROR.", LOG_TYPE_MESSAGE, NULL, 0);
        break;
    default:
        break;
    }
}

static void sound_stop(int *state, DRV_HANDLE *ptHandle, DRV_HANDLE *poHandle, SYS_DMA_CHANNEL_HANDLE * pdHandle)
{
    DRV_HANDLE tHandle = *ptHandle;
    DRV_HANDLE oHandle = *poHandle;
    SYS_DMA_CHANNEL_HANDLE dHandle = *pdHandle;
    RPA1Rbits.RPA1R = 0b0000; // Sound OFF
#if 1
    if (dHandle != SYS_DMA_CHANNEL_HANDLE_INVALID) {
        SYS_DMA_ChannelForceAbort(dHandle);
        SYS_DMA_ChannelDisable(dHandle);
        SYS_DMA_ChannelRelease(dHandle);
        *pdHandle = SYS_DMA_CHANNEL_HANDLE_INVALID;
    }
#endif
    if (tHandle != DRV_HANDLE_INVALID) {
        DRV_TMR_AlarmDisable(tHandle);
        DRV_TMR_Stop(tHandle);
        DRV_TMR_Close(tHandle);
    }
    if (oHandle != DRV_HANDLE_INVALID) {
        DRV_OC_Stop(oHandle);
        DRV_OC_Close(oHandle);
    }
    *state = C_SOUND_INIT;
    *ptHandle = DRV_HANDLE_INVALID;
    *poHandle = DRV_HANDLE_INVALID;
}

static void sound_start(int *state, DRV_HANDLE *ptHandle, DRV_HANDLE *poHandle, SYS_DMA_CHANNEL_HANDLE * pdHandle)
{
    DRV_HANDLE tHandle;
    DRV_HANDLE oHandle;
    SYS_DMA_CHANNEL_HANDLE dHandle;

    RPA1Rbits.RPA1R = 0b0101; // Sound ON / OC2
    dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
    int16_t *p = get_sound_buffer();
    
    if (p != NULL) {
        SYS_DMA_Suspend();
        memset(p, 0x00, sizeof (int16_t) * (SOUND_LENGTH + 32));
        SYS_DMA_Resume();
        tHandle = DRV_TMR_Open(DRV_TMR_INDEX_1, DRV_IO_INTENT_EXCLUSIVE);
        oHandle = DRV_OC_Open(DRV_OC_INDEX_0, DRV_IO_INTENT_READWRITE);
        if (tHandle != DRV_HANDLE_INVALID) {
            DRV_TMR_CounterClear(tHandle);
            DRV_TMR_CounterValueSet(tHandle, 1000);
            DRV_TMR_Start(tHandle);
        }
        if (oHandle != DRV_HANDLE_INVALID) {
            DRV_OC_PulseWidthSet(oHandle, 10);
            scallback_count = 0;
            DRV_OC_Start(DRV_OC_INDEX_0, DRV_IO_INTENT_READWRITE);
        }
#if 1
        if (dHandle != SYS_DMA_CHANNEL_HANDLE_INVALID) {
            //SYS_DMA_ChannelTransferEventHandlerSet(dHandle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), 0);
            *state = C_SOUND_PLAY;
        }
#endif
    }
    *state = C_SOUND_PLAY;
    *ptHandle = tHandle;
    *poHandle = oHandle;
    *pdHandle = dHandle;
    taskENTER_CRITICAL();
    wrote_bytes = 0;
    render_slot = 0;
    play_avail = 0;
    render_flag[0] = render_flag[1] = false;
    taskEXIT_CRITICAL();
    init_env_vol_table();
    init_mmls(NULL, NULL, NULL);
}


void _T_SOUND_Task_Main(void *pvParameters)
{
    int CmdQueue;
    int state = C_SOUND_STOP;
    int16_t *p;
    int nlen;
    uint32_t ptr;
    bool onoff;
    uint32_t rmod;
    uint32_t rlen;
    int i;
    bool pending;
    bool b_cont;

    render_slot = 1;
    taskENTER_CRITICAL();
    render_slot = 0;

    taskEXIT_CRITICAL();
    p = get_sound_buffer();
    DRV_HANDLE tHandle = DRV_HANDLE_INVALID;
    DRV_HANDLE oHandle = DRV_HANDLE_INVALID;
    SYS_DMA_CHANNEL_HANDLE dHandle = SYS_DMA_CHANNEL_HANDLE_INVALID;

#if 0
    dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
#endif
    rmod = 0;
    onoff = true;
    nlen = SOUND_LENGTH;
    state = C_SOUND_INIT;
    render_flag[0] = render_flag[1] = false;
    //sample_buffer = NULL;
    //if (sample_buffer == NULL) {
    //    taskENTER_CRITICAL();
    //    sample_buffer = pvPortMalloc((SOUND_LENGTH + 32) * sizeof (int16_t));
    //   taskEXIT_CRITICAL();
    //}
    while (1) {
        // Wait Queue
        CmdQueue = C_SOUND_INIT;
        if (xQueueReceive(xSoundCmdQueue, (void *) (&CmdQueue), 0) == pdPASS) {
            switch (CmdQueue) {
            case C_SOUND_START:
                if (state != C_SOUND_PLAY) {
                    make_noise_table();
                    sound_start(&state, &tHandle, &oHandle, &dHandle);
                }
                break;
            case C_SOUND_ABORT:
            case C_SOUND_STOP:
                if (state == C_SOUND_PLAY) {
                    sound_stop(&state, &tHandle, &oHandle, &dHandle);
                    vTaskDelay(cTick100ms);
#if 0
                    SYS_DEVCON_SystemUnlock();
                    PMD3bits.OC2MD = 1;
                    PMD4bits.T3MD = 1;
                    SYS_DEVCON_SystemLock();
#endif
                }
                break;
            default:
                break;
            }
        }
        if (state == C_SOUND_PLAY) {
            if ((dHandle == SYS_DMA_CHANNEL_HANDLE_INVALID) || (get_sound_buffer() == NULL)) {
                vTaskDelay(cTick100ms);
            } else {
                b_cont = mmldata[0].is_end; // ToDo
                b_cont &= mmldata[1].is_end;
                b_cont &= mmldata[2].is_end;
                //if(pending) {
                if (!b_cont) {
                    int sl;
                    int maxlen;
                    int _pl;
                    uint32_t s_count;
                    taskENTER_CRITICAL();
                    _pl = play_avail;
                    taskEXIT_CRITICAL();
                    if (_pl < 2) {
                        int _wb[4] = {0, 0, 0, 0};
                        int _mb[4];
                        int ii;
                        s_count = (SOUND_LENGTH * render_slot) / 2;
                        rlen = 0;
                        maxlen = SOUND_LENGTH / 2;
                        wrote_bytes = 0;
                        _mb[0] = _mb[1] = _mb[2] = _mb[3] = maxlen;
                        //SYS_DMA_Suspend();
                        uint16_t* _spp = get_sound_buffer();
                        memset(&(_spp[s_count]), 0x00, maxlen * sizeof (int16_t));
                        //SYS_DMA_Resume();
                        int __ch = 0;
                        int __sc;
                        for (__ch = 0; __ch < 3; __ch++) {
                            __sc = s_count;
                            if (mmldata[__ch].is_end) continue;
                            while (_mb[__ch] > 0) {
                                //SYS_DMA_Suspend();
                                rlen = render_mml(&(_spp[__sc]), &(mmldata[__ch]), _mb[__ch]);
                                //SYS_DMA_Resume();
                                __sc += rlen;
                                _mb[__ch] = _mb[__ch] - (int) rlen;
                                _wb[__ch] += rlen;
                                if (__sc >= SOUND_LENGTH) break;
                                if (mmldata[__ch].is_end) break;
                            }
                        }
                        wrote_bytes = 0;
                        for (__ch = 0; __ch < 3; __ch++) {
                            if (_wb[__ch] > wrote_bytes) wrote_bytes = _wb[__ch];
                        }
                        bool bp[2];
                        taskENTER_CRITICAL();
                        bp[0] = render_flag[0];
                        bp[1] = render_flag[1];
                        taskEXIT_CRITICAL();
                        //if (rlen > 0) {
                        taskENTER_CRITICAL();
                        if (wrote_bytes > 0) {
                            render_flag[render_slot] = true;
                            for (ii = 0; ii < wrote_bytes; ii++) {
                                //sample_buffer[s_count + ii] = (sample_buffer[s_count + ii] >> 2) + 16;
                                _spp[s_count + ii] = _spp[s_count + ii] >> 5;
                            }
                        }

                        _pl = play_avail;
                        taskEXIT_CRITICAL();
                        if ((wrote_bytes > 0)) {
                            if (!SYS_DMA_ChannelIsBusy(dHandle)) add_sound(dHandle, render_slot);
                            taskENTER_CRITICAL();
                            play_avail++;
                            render_slot++;
                            taskEXIT_CRITICAL();
                        }
                        if (render_slot >= 2) render_slot = 0;
                        wrote_bytes = 0;
                    } else {
                        //vTaskDelay(2);
                        vTaskDelay(cTick100ms / 10);
                    }
                } else {
                    sound_stop(&state, &tHandle, &oHandle, &dHandle);
                    vTaskDelay(cTick100ms / 2);
                    sound_start(&state, &tHandle, &oHandle, &dHandle);
                    //state = C_SOUND_STOP;
                    //sound_stop(&state, &tHandle, &oHandle, &dHandle);
                }
            }
        } else {
            vTaskDelay(cTick100ms);
        }
    }
    //    if (sample_buffer != NULL) {
    //        taskENTER_CRITICAL();
    //       vPortFree(sample_buffer);
    //        sample_buffer = NULL;
    //        taskEXIT_CRITICAL();
    //    }

}
