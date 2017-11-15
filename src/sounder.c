
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
    95, 103, 111, 119, 127, 136, 145, 154,
    164, 174, 185, 196, 208, 220, 235, 255
};

uint16_t __attribute__((coherent)) __attribute__((aligned(16))) sample_buffer[SOUND_LENGTH + 32]; // 0.2sec


#define TEST_FREQ 440
#define SAMPLE_FREQ 16000

typedef struct {
    uint32_t freq; // Hz
    uint32_t envelope_pitch; // Tick. 1Tick = 16KHz.
    uint32_t howlong;
    uint32_t noise_freq;
    int16_t vol;
    int16_t env_tmp_vol;
    uint8_t env_pos;
    uint8_t env_type;
    bool noise_on;
    bool env_on;
} SOUND_PSG_EMU_T;

typedef struct {
    char *mmlbase;
    uint32_t mmlpos;
    bool sound_stop; // Sound = true, stop = false;
    bool is_eof; // Ended?
    uint32_t tempo; // Txx :
    uint32_t octave;
    uint32_t sound_length;
    uint32_t stop_length;
    uint32_t tmp_sound_length;
    SOUND_PSG_EMU_T regs;
} SOUND_MML_T;

/*
 * MML SYNTAX from "FM-7 F-BASIC Grammer book (???)" 
 * By FUJITSU LIMITED, Apr. 1984, at PAGE 3-137 to 3-144.
*/ 
const char test_mml1[] = "O4CDEFGO5ABC";
//const char test_mml1[] = "O4CR16DR16ER16FR16GO5ABC";

static SOUND_MML_T mmldata[3];

static uint32_t render_op(int16_t *data, uint32_t freq, uint32_t samples, int16_t vol, uint32_t *rmod, bool *ponoff)
{
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
        *p += ((onoff) ? vol : 0);
        if(*p > 600) *p = 600;
        p++; // Saturation
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

static uint32_t render_sound(int16_t *data, uint32_t samples, int16_t vol, uint32_t *rmod, bool *ponoff)
{
    uint32_t freq = TEST_FREQ;
    return render_op(data, freq, samples, vol, rmod, ponoff);
}

const uint32_t note_std_o4[] = {
    440000, 493883, 523251, 587330, 659255, 698456, 783991, // A B C D E F G
};
const uint32_t note_upper_o4[] = {
    466164, 493883, 554365, 622254, 659255, 739989, 830609, // A# B C# D# E F# G#
};

const uint32_t note_lower_o4[] = {
    415305, 466164, 523251, 554365, 622254, 698456, 739989, // Ab(G#), Bb C Db Eb F Gb
};

static uint32_t render_mml(int16_t *head_data, SOUND_MML_T *regs, uint32_t max_samples, int16_t vol, uint32_t *rmod, bool *ponoff)
{
    uint32_t left = max_samples;
    uint32_t n_samples;
    uint32_t p_samples = 0;
    size_t mmllen;
    uint32_t mmlpos;
    uint32_t basefreq = 0;
    int i, j;
    char *p;
    char note = 0x00;
    char nbuf[4];
    size_t nlen;
    int32_t longval;
    if (head_data == NULL) return 0;
    if(regs->mmlbase == NULL) return 0;
    if (regs == NULL) return 0;
    
    if (regs->tmp_sound_length > 0) {
        if (left > regs->tmp_sound_length) {
            n_samples = regs->tmp_sound_length;
            left -= regs->tmp_sound_length;
            regs->tmp_sound_length = 0;
        } else {
            n_samples = left;
            regs->tmp_sound_length -= left;
            left = 0;
        }
        if (regs->sound_stop != 0) {
            // Skip
            p_samples += n_samples;
        } else {
            p_samples += render_op(&(head_data[p_samples]), regs->regs.freq, n_samples, regs->regs.vol, rmod, ponoff);
        }
        if(left == 0) {
            return max_samples;
        }
    }
    
    if(regs->is_eof) {
        return max_samples;
    }
    mmllen = strlen(regs->mmlbase);
    p = regs->mmlbase;
    mmlpos = regs->mmlpos;
    if(mmllen <= regs->mmlpos) {
        regs->is_eof = true;
        return max_samples;
    }
    
    while(left > 0) {
        if(mmlpos >= mmllen) {
            regs->is_eof = true;
            break;
        }
        if(note != 0x00) {
            if(((p[mmlpos] >= 'A') && (p[mmlpos] <= 'G')) ||
                    ((p[mmlpos] >= 'a') && (p[mmlpos] <= 'g')) ||
                    (p[mmlpos] == 'R') || (p[mmlpos] == 'r')) {
                // Do before Note.
                basefreq = note_std_o4[note - 'a'];
                note = 0x00;
            } else if((p[mmlpos] == '#') || (p[mmlpos] == '+')) {
                basefreq = note_upper_o4[note - 'a'];
                note = 0x00;
            } else if(p[mmlpos] == '-') {
                basefreq = note_lower_o4[note - 'a'];
                note = 0x00;
            }
            if(note == 0x00) {
                if(regs->octave < 4) {
                    basefreq = basefreq >> (4 - regs->octave);
                    basefreq = basefreq / 1000;
                } else if(regs->octave > 4) {
                    basefreq = basefreq << (regs->octave - 4);
                    basefreq = basefreq / 1000;
                } else {
                    basefreq =basefreq / 1000;
                }
                regs->regs.freq = basefreq;
                // Do Sound
                if(regs->tmp_sound_length > left) {
                    n_samples = left;
                    left = 0;
                } else {
                    n_samples = regs->tmp_sound_length;
                    left -= regs->tmp_sound_length;
                }
                p_samples += render_op(&(head_data[p_samples]), regs->regs.freq, n_samples, regs->regs.vol, rmod, ponoff);
                regs->tmp_sound_length -= n_samples;
                regs->stop_length = 0;
            }
        }
        if(left == 0) break;
        if(((p[mmlpos] >= 'A') && (p[mmlpos] <= 'G')) ||
          ((p[mmlpos] >= 'a') && (p[mmlpos] <= 'g')))  {
            if(p[mmlpos] <= 'G') {
                note = p[mmlpos] - 'A' + 'a';
            } else {
                note = p[mmlpos];
            }
            memset(nbuf, 0x00, sizeof(nbuf));
            for(i = 1; i < 3; i++) {
                if((mmlpos + i) >= mmllen) break;
                if((p[mmlpos + i] >= '0') && (p[mmlpos + i] <= '9')) {
                    nbuf[i - 1] = p[mmlpos + i];
                } else {
                    break;
                }
            }
            nlen = strlen(nbuf);
            mmlpos = mmlpos + nlen + 1;
            i = 0;
            do {
                if(p[mmlpos] != '.') break;
                i++;
                mmlpos++;
            } while(mmlpos < mmllen);
            
            if(nlen > 0) {
                longval = strtol(nbuf, NULL, 10);
                if((longval > 0) && (longval <= 64)) {
                    longval = longval * 16;
                } else {
                    longval = regs->sound_length * 16;
                }
            } else {
                longval = regs->sound_length * 16;
            }
            for(j = 0; j < i; j++) {
                longval = (longval * 2) / 3;
            } 
            regs->tmp_sound_length = regs->tempo / (longval / 16); 
            regs->stop_length = 0; 
        } else if((p[mmlpos] == 'V') || (p[mmlpos] == 'v')) { // Volume
            memset(nbuf, 0x00, sizeof(nbuf));
            for(i = 1; i < 2; i++) {
                if((mmlpos + i) >= mmllen) break;
                if((p[mmlpos + i] >= '0') && (p[mmlpos + i] <= '9')) {
                    nbuf[i - 1] = p[mmlpos + i];
                } else {
                    break;
                }
            }
            nlen = strlen(nbuf);
            mmlpos = mmlpos + nlen + 1;
            longval = strtol(nbuf, NULL, 10);
            if((longval >= 0) && (longval < 16)) {
                regs->regs.vol = sound_level_table[longval << 1];
            }
        } else if((p[mmlpos] == 'T') || (p[mmlpos] == 't')) { //Tempo
            memset(nbuf, 0x00, sizeof(nbuf));
            for(i = 1; i < 3; i++) {
                if((mmlpos + i) >= mmllen) break;
                if((p[mmlpos + i] >= '0') && (p[mmlpos + i] <= '9')) {
                    nbuf[i - 1] = p[mmlpos + i];
                } else {
                    break;
                }
            }
            nlen = strlen(nbuf);
            mmlpos = mmlpos + nlen + 1;
            longval = strtol(nbuf, NULL, 10);
            if((longval >= 32) && (longval < 255)) {
                regs->tempo = (60 * 16000 * 4) / longval;
            }
        } else if((p[mmlpos] == 'R') || (p[mmlpos] == 'r')) { // STOP
            memset(nbuf, 0x00, sizeof(nbuf));
            for(i = 1; i < 3; i++) {
                if((mmlpos + i) >= mmllen) break;
                if((p[mmlpos + i] >= '0') && (p[mmlpos + i] <= '9')) {
                    nbuf[i - 1] = p[mmlpos + i];
                } else {
                    break;
                }
            }
            nlen = strlen(nbuf);
            mmlpos = mmlpos + nlen + 1;
            i = 0;
            do {
                if(p[mmlpos] != '.') break;
                i++;
                mmlpos++;
            } while(mmlpos < mmllen);
            if(nlen > 0) {
                longval = strtol(nbuf, NULL, 10);
                if((longval > 0) && (longval <= 64)) {
                    longval = longval * 16;
                } else {
                    longval =  4 * 16;
                }
            } else {
                longval = 4 * 16;
            }
            for(j = 0; j < i; j++) {
                longval = (longval * 2) / 3;
            } 
            regs->stop_length = regs->tempo / (longval / 16); 
            regs->tmp_sound_length = 0;
            if(regs->stop_length >= left) {
                regs->stop_length -= left;
                p_samples += left;
                left = 0;
                break;
            } else {
                left -= regs->stop_length;
                p_samples = p_samples + regs->stop_length;
            }
        } else if((p[mmlpos] == 'L') || (p[mmlpos] == 'l')) { // Length
            memset(nbuf, 0x00, sizeof(nbuf));
            for(i = 1; i < 3; i++) {
                if((mmlpos + i) >= mmllen) break;
                if((p[mmlpos + i] >= '0') && (p[mmlpos + i] <= '9')) {
                    nbuf[i - 1] = p[mmlpos + i];
                } else {
                    break;
                }
            }
            nlen = strlen(nbuf);
            mmlpos = mmlpos + nlen + 1;
            i = 0;
            do {
                if(p[mmlpos] != '.') break;
                i++;
                mmlpos++;
            } while(mmlpos < mmllen);
            if(nlen > 0) {
                longval = strtol(nbuf, NULL, 10);
                if((longval > 0) && (longval <= 64)) {
                    longval = longval;
                } else {
                    longval =  4;
                }
            } else {
                longval = 4;
            }
            regs->sound_length = longval;
        } else if((p[mmlpos] == 'O') || (p[mmlpos] == 'o')) { //Octave
            memset(nbuf, 0x00, sizeof(nbuf));
            for(i = 1; i < 2; i++) {
                if((mmlpos + i) >= mmllen) break;
                if((p[mmlpos + i] >= '0') && (p[mmlpos + i] <= '9')) {
                    nbuf[i - 1] = p[mmlpos + i];
                } else {
                    break;
                }
            }
            nlen = strlen(nbuf);
            mmlpos = mmlpos + nlen + 1;
            longval = strtol(nbuf, NULL, 10);
            if((longval >= 1) && (longval < 12)) {
                regs->octave = longval;
            }
        }
    }
#if 0
    if(left > 0) {
        if(regs->tmp_sound_length > 0) {
            if(regs->tmp_sound_length >= left) {
                n_samples = left;
                regs->tmp_sound_length -= n_samples;
            } else {
                n_samples = regs->tmp_sound_length;
                regs->tmp_sound_length = 0;
            }
            if((regs->is_eof) && ((note >= 'a') && (note <= 'g'))) {
                basefreq = note_std_o4[note - 'a'];
                if(regs->octave < 4) {
                    basefreq = basefreq >> (4 - regs->octave);
                    basefreq = basefreq / 1000;
                } else if(regs->octave > 4) {
                    basefreq = basefreq << (regs->octave - 4);
                    basefreq = basefreq / 1000;
                } else {
                    basefreq = basefreq / 1000;
                }
                regs->regs.freq = basefreq;
            }
            p_samples += render_op(&(head_data[p_samples]), regs->regs.freq, n_samples, regs->regs.vol, rmod, ponoff);
        }
    }
#endif
    regs->mmlpos = mmlpos;
    return p_samples;
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
        CmdQueue = C_SOUND_INIT;
        if (xQueueReceive(xSoundCmdQueue, (void *) (&CmdQueue), 0) == pdPASS) {
            switch (CmdQueue) {
            case C_SOUND_START:
                DMACONbits.ON = 1;
                DMACONbits.SUSPEND = 0;
                RPA1Rbits.RPA1R = 0b0101; // Sound ON / OC2
                memset(sample_buffer, 0x00, sizeof (sample_buffer));
                dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
                tHandle = DRV_TMR_Open(DRV_TMR_INDEX_1, DRV_IO_INTENT_EXCLUSIVE);
                oHandle = DRV_OC_Open(DRV_OC_INDEX_0, DRV_IO_INTENT_READWRITE | DRV_IO_INTENT_EXCLUSIVE);
                if (tHandle != DRV_HANDLE_INVALID) {
                    DRV_TMR_CounterClear(tHandle);
                    DRV_TMR_CounterValueSet(tHandle, 1000);
                    DRV_TMR_Start(tHandle);
                }
                if (oHandle != DRV_HANDLE_INVALID) {
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
                {
                    // INIT MML.
                    for(i = 0; i < 3; i++) {
                        mmldata[i].is_eof = false;
                        mmldata[i].mmlbase = test_mml1; // ToDo: Change this.
                        mmldata[i].mmlpos = 0;
                        mmldata[i].sound_length = 4;
                        mmldata[i].tempo = (60 * 16000 * 4) / 64;  // T=64 OK?
                        mmldata[i].octave = 4;
                        mmldata[i].sound_stop = 0;
                        mmldata[i].tmp_sound_length = 4;
                        mmldata[i].regs.env_on = false;
                        mmldata[i].regs.noise_on = false;
                        mmldata[i].regs.freq = 440;
                        mmldata[i].regs.howlong = 0;
                        mmldata[i].regs.vol = sound_level_table[30];
                        mmldata[i].regs.noise_freq = 440;
                        mmldata[i].regs.env_tmp_vol = 31;
                        mmldata[i].regs.env_pos = 0;
                        mmldata[i].regs.env_type = 0;
                    }
                }
                break;
            case C_SOUND_ABORT:
            case C_SOUND_STOP:
                DMACONbits.ON = 0;
                DMACONbits.SUSPEND = 1;
                RPA1Rbits.RPA1R = 0b0000; // Sound OFF

                if (tHandle != DRV_HANDLE_INVALID) {
                    DRV_TMR_Stop(tHandle);
                    DRV_TMR_Close(tHandle);
                }
                if (oHandle != DRV_HANDLE_INVALID) {
                    DRV_OC_Stop(oHandle);
                    DRV_OC_Close(oHandle);
                }
                if (dHandle != SYS_DMA_CHANNEL_HANDLE_INVALID) {
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
                    //rlen = render_sound(&(sample_buffer[s_count]), i * sizeof (int16_t), 512, &rmod, &onoff);
                    rlen = render_mml(&(sample_buffer[s_count]), &(mmldata[0]), i, mmldata[0].regs.vol, &rmod, &onoff);
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

}
