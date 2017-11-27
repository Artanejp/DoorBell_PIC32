
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
#include "pca9655.c"
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

static int16_t __attribute__((aligned(16))) sample_buffer[SOUND_LENGTH + 32]; // 0.2sec


#define TEST_FREQ 440
#define SAMPLE_FREQ SOUND_RATE

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
    uint32_t rmod;
    bool ponoff;
} SOUND_PSG_EMU_T;

typedef struct {
    char *mmlbase;
    uint32_t mmlpos;
    bool sound_stop; // Sound = true, stop = false;
    bool is_eof; // Ended?
    bool is_end; // Rendering end.
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
const char *test_mml1 = "L4O4CDEFGO5ABCDEFGO6ABCDEFGO3ABCDEFG";
//const char test_mml1[] = "O4C16R16C16";

static SOUND_MML_T mmldata[3];

static uint32_t render_op(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    uint32_t rp = 0;
    int16_t *p = data;
    int i;
    bool onoff = true;
    static const uint32_t sp = (SAMPLE_FREQ * 1000) / 2;
    
    if (regs == NULL) return samples;
    uint32_t freq = regs->regs.freq;
    int16_t vol = regs->regs.vol;
    int16_t pvol;
    
    rp = regs->regs.rmod;
    onoff = regs->regs.ponoff;
    pvol = (onoff) ? vol : 0;
    //{
    // First
    //    if (onoff) {
    //        *p += vol;
    //    } else {
    //        *p += 0;
    //    }
    //    onoff = !onoff;
    // }
    for (i = 0; i < samples; i++) {
        if (rp >= sp) {
            rp -= sp;
            onoff = !onoff;
            pvol = (onoff) ? vol : 0;
        }
        p[i]= pvol;
        if (p[i] > 650) p[i] = 650; // Saturation
        //configASSERT(!(&(p[i]) >= (&(sample_buffer[SAMPLE_FREQ]))));
        //configASSERT(!(&(p[i]) < (&(sample_buffer[0]))));
        rp += freq;
    }
    if (rp >= sp) {
        rp -= sp;
        onoff = !onoff;
    }
    regs->regs.rmod = rp;
    regs->regs.ponoff = onoff;
    return samples;
}

static uint32_t render_sound(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    regs->regs.freq = TEST_FREQ;
    return render_op(data, regs, samples);
}

static const uint32_t note_std_o4[] = {
    440000, 493883, 523251, 587330, 659255, 698456, 783991, // A B C D E F G
};
static const uint32_t note_upper_o4[] = {
    466164, 493883, 554365, 622254, 659255, 739989, 830609, // A# B C# D# E F# G#
};

static const uint32_t note_lower_o4[] = {
    415305, 466164, 523251, 554365, 622254, 698456, 739989, // Ab(G#), Bb C Db Eb F Gb
};

static bool pre_render(int16_t *head_data, SOUND_MML_T *regs, uint32_t *ps, uint32_t *p_left, uint32_t max_samples)
{
    uint32_t p_samples;
    uint32_t n_samples;
    uint32_t left;
    if ((regs == NULL) || (ps == NULL) || (p_left == NULL)) return false;
    p_samples = *ps;
    left = *p_left;
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
            p_samples += render_op(head_data, regs, n_samples);
        }
        if (left == 0) {
            *ps = p_samples;
            *p_left = 0;
            return false;
        }
        *p_left = left;
        *ps = p_samples;
        return true;
    }
}

static bool render_mml_core(int16_t *head_data, SOUND_MML_T *regs, uint32_t *psndptr, uint32_t max_samples, uint32_t mml_len)
{
    uint32_t mmlptr;
    uint32_t sndptr;
    char head_char;
    char *p;
    char note;
    uint32_t basefreq = 1;
    bool b_note = false;
    bool loopval;
    bool need_next = false;
    size_t nlen;
    uint32_t left = max_samples;
    int extra_length;
    long longval;
    long tlval;
    uint32_t n_samples;
    char nbuf[8];
    int nptr = 0;
    int i;
    if ((regs == NULL) || (psndptr == NULL)) return false; // Not continue.    
    sndptr = *psndptr;
    mmlptr = regs->mmlpos;
    if (sndptr >= max_samples) return false;
    if (mmlptr >= mml_len) return false;

    p = &(regs->mmlbase[mmlptr]);
    head_char = *p;

    if ((head_char >= 'A') && (head_char <= 'G')) {
        basefreq = note_std_o4[head_char - 'A'];
        need_next = true;
        b_note = true;
        note = head_char - 'A' + 'a';
    } else if ((head_char >= 'a') && (head_char <= 'g')) {
        basefreq = note_std_o4[head_char - 'a'];
        need_next = true;
        b_note = true;
        note = head_char;
    } else if ((head_char == 'R') || (head_char == 'r')) {
        need_next = true;
        b_note = false;
        note = 0;
    } else if ((head_char == 'V') || (head_char == 'v')) { // Volume
        memset(nbuf, 0x00, sizeof (nbuf));
        nptr = 0;
        mmlptr++;
        p++;
        for (; mmlptr < mml_len; mmlptr++, p++) {
            head_char = *p;
            if ((*p >= '0') && (*p <= '9')) {
                nbuf[nptr] = head_char;
                nptr++;
            } else {
                break;
            }
            if (nptr > 2) break;
        }
        nlen = strlen(nbuf);
        longval = strtol(nbuf, NULL, 10);
        if ((longval >= 0) && (longval < 16)) {
            regs->regs.vol = sound_level_table[longval << 1];
        }
    } else if ((head_char == 'T') || (head_char == 't')) { //Tempo
        memset(nbuf, 0x00, sizeof (nbuf));
        nptr = 0;
        mmlptr++;
        p++;
        for (; mmlptr < mml_len; mmlptr++, p++) {
            head_char = *p;
            if ((head_char >= '0') && (head_char <= '9')) {
                nbuf[nptr] = head_char;
                nptr++;
            } else {
                break;
            }
            if (nptr > 3) break;
        }
        nlen = strlen(nbuf);
        longval = strtol(nbuf, NULL, 10);
        if ((longval >= 32) && (longval < 255)) {
            regs->tempo = (60 * SOUND_RATE * 4) / longval;
        }
    } else if ((head_char == 'L') || (head_char == 'l')) { // Length
        memset(nbuf, 0x00, sizeof (nbuf));
        nptr = 0;
        mmlptr++;
        p++;
        for (; mmlptr < mml_len; mmlptr++, p++) {
            head_char = *p;
            if ((head_char >= '0') && (head_char <= '9')) {
                nbuf[nptr] = head_char;
                nptr++;
            } else {
                break;
            }
            if (nptr > 4) break;
        }
        nlen = strlen(nbuf);
        extra_length = 0;
        do {
            if (*p != '.') break;
            extra_length++;
            p++;
            mmlptr++;
        } while (mmlptr < mml_len);
        longval = 4;
        if (nlen > 0) {
            tlval = strtol(nbuf, NULL, 10);
            if ((tlval > 0) && (tlval <= 64)) {
                longval = tlval;
            }
        }

        regs->sound_length = longval;
    } else if ((head_char == 'O') || (head_char == 'o')) { //Octave
        memset(nbuf, 0x00, sizeof (nbuf));
        nptr = 0;
        mmlptr++;
        p++;
        for (; mmlptr < mml_len; mmlptr++, p++) {
            head_char = *p;
            if ((head_char >= '0') && (head_char <= '9')) {
                nbuf[nptr++] = head_char;
            } else {
                break;
            }
            if (nptr > 2) break;
        }
        nlen = strlen(nbuf);
        longval = strtol(nbuf, NULL, 10);
        if ((longval >= 1) && (longval < 12)) {
            regs->octave = longval;
        }
    }

    if (need_next) {
        loopval = true;
        do {
            mmlptr++;
            p++;
            if (mmlptr >= mml_len) break;
            head_char = *p;
            if ((head_char >= 'A') && (head_char <= 'G')) break; // Next
            if ((head_char >= 'a') && (head_char <= 'g')) break; // Next
            switch (head_char) {
            case '#':
            case '+':
                if ((b_note) && (note >= 'a') && (note <= 'g')) {
                    basefreq = note_upper_o4[note - 'a'];
                    note = 0;
                }
                break;
            case '-':
                if ((b_note) && (note >= 'a') && (note <= 'g')) {
                    basefreq = note_lower_o4[note - 'a'];
                    note = 0;
                }
                break;
                //case '.':
                //    extra_length++;
                //    break;
            default:
                loopval = false;
                break;
            }
        } while (loopval);
        // Check length
        extra_length = 0;
        nptr = 0;
        memset(nbuf, 0x00, sizeof (nbuf));
        for (; mmlptr < mml_len; mmlptr++, p++) {
            head_char = *p;
            if ((head_char >= '0') && (head_char <= '9')) {
                nbuf[nptr] = head_char;
                nptr++;
            } else if (head_char == '.') {
                extra_length++;
            } else {
                break;
            }
            if (nptr >= 6) break; // Force exit
        }
        nlen = strlen(nbuf);
        longval = regs->sound_length * 16;
        if (nlen > 0) {
            longval = strtol(nbuf, NULL, 10);
            if ((longval > 0) && (longval <= 64)) {
                longval = longval * 16;
            }
        }
        for (i = 0; i < extra_length; i++) {
            longval = (longval * 2) / 3;
        }
        if (b_note) {
            regs->tmp_sound_length = regs->tempo / (longval / 16);
            regs->stop_length = 0;
        } else {
            regs->tmp_sound_length = 0;
            regs->stop_length = regs->tempo / (longval / 16);
        }
        // Calc Frequency
        if (b_note) {
            if (regs->octave < 4) {
                basefreq = basefreq >> (4 - regs->octave);
                //basefreq = basefreq / 1000;
            } else if (regs->octave > 4) {
                basefreq = basefreq << (regs->octave - 4);
                //basefreq = basefreq / 1000;
            } else {
                //basefreq = basefreq / 1000;
            }
            regs->regs.freq = basefreq;
            // Do Sound
            if (regs->tmp_sound_length > left) {
                n_samples = left;
                left = 0;
            } else {
                n_samples = regs->tmp_sound_length;
                left -= regs->tmp_sound_length;
            }
            sndptr += render_op(&(head_data[sndptr]), regs, n_samples);
            regs->tmp_sound_length -= n_samples;
        } else {
            sndptr += n_samples;
            regs->stop_length -= n_samples;
        }
    }
    *psndptr = sndptr;
    regs->mmlpos = mmlptr;
    return true;
}

static uint32_t render_mml(int16_t *head_data, SOUND_MML_T *regs, uint32_t max_samples)
{
    uint32_t left = max_samples;
    uint32_t p_samples = 0;
    size_t mmllen;
    uint32_t mmlpos;
    int i, j;
    char *p;
    char note = 0x00;
    if (head_data == NULL) return 0;
    if (regs == NULL) return max_samples;
    if (regs->mmlbase == NULL) return max_samples;
    if (regs->is_end) return 0;

    if (!pre_render(&(head_data[p_samples]), regs, &p_samples, &left, max_samples)) {
        if (regs->is_eof) { // EOF and END
            if ((regs->tmp_sound_length == 0) && (regs->stop_length == 0)) {
                //memset(&(head_data[p_samples]), 0x00, max_samples * sizeof(int16_t));
                regs->is_end = true;
            }
        }
        configASSERT((p_samples > (SOUND_LENGTH / 2)) ? 0 : 1);
        return p_samples;
    }
    if (regs->is_eof) { // EOF and END
        if ((regs->tmp_sound_length == 0) && (regs->stop_length == 0)) {
            //memset(&(head_data[p_samples]), 0x00, max_samples * sizeof(int16_t));
            regs->is_end = true;
            configASSERT((p_samples > (SOUND_LENGTH / 2)) ? 0 : 1);
            return p_samples;
        }
    }

    mmllen = strlen(regs->mmlbase);
    p = regs->mmlbase;
    mmlpos = regs->mmlpos;
    if (mmllen <= regs->mmlpos) {
        regs->is_eof = true;
        //regs->mmlpos = 0;
        configASSERT((p_samples > (SOUND_LENGTH / 2)) ? 0 : 1);
        return p_samples;
    }

    while (p_samples < max_samples) {
        left = max_samples - p_samples;
        if (!(regs->is_eof)) {
            if (!render_mml_core(&(head_data[p_samples]), regs, &p_samples, left, mmllen)) {
                configASSERT((p_samples > (SOUND_LENGTH / 2)) ? 0 : 1);
                return p_samples;
            }
        }
        mmlpos = regs->mmlpos;
        if (mmlpos >= mmllen) {
            regs->is_eof = true;
            //return p_samples;
        }
    }
    configASSERT((p_samples > (SOUND_LENGTH / 2)) ? 0 : 1);
    return p_samples;
}

static int play_slot;
static int render_slot;
static int play_avail;
static bool render_flag[4];

void sndDmaEventHandler(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle);

static __inline__ void add_sound_isr(SYS_DMA_CHANNEL_HANDLE handle, uint32_t slot)
{
    static const uint32_t nlen = SOUND_LENGTH;
    uint32_t sp = ((slot % 2) * nlen) / 2;
    SYS_DMA_ChannelTransferEventHandlerSet(handle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), slot);
    SYS_DMA_ChannelTransferSet(handle, (const void *) (&(sample_buffer[sp])), (nlen * sizeof (int16_t)) / 2, (const void *) (&OC2RS), sizeof (int32_t), sizeof (int16_t));
    SYS_DMA_ChannelSetup(handle, SYS_DMA_CHANNEL_OP_MODE_BASIC, DMA_TRIGGER_TIMER_3);
    SYS_DMA_ChannelEnable(handle);
}

static void add_sound(SYS_DMA_CHANNEL_HANDLE handle, uint32_t slot)
{
    static const uint32_t nlen = SOUND_LENGTH;
    uint32_t sp = ((slot % 2) * nlen) / 2;
    SYS_DMA_ChannelTransferEventHandlerSet(handle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), slot);
    SYS_DMA_ChannelTransferSet(handle, (const void *) (&(sample_buffer[sp])), (nlen * sizeof (int16_t)) / 2, (const void *) (&OC2RS), sizeof (int32_t), sizeof (int16_t));
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

extern PCA9655_t ioexpander1_data;

static void sound_stop(int *state, DRV_HANDLE *ptHandle, DRV_HANDLE *poHandle, SYS_DMA_CHANNEL_HANDLE *pdHandle)
{
    DRV_HANDLE tHandle = *ptHandle;
    DRV_HANDLE oHandle = *poHandle;
    SYS_DMA_CHANNEL_HANDLE dHandle = *pdHandle;
    RPA1Rbits.RPA1R = 0b0000; // Sound OFF
#if 1
    if (dHandle != SYS_DMA_CHANNEL_HANDLE_INVALID) {
        SYS_DMA_ChannelForceAbort(dHandle);
        SYS_DMA_ChannelDisable(dHandle);
        //SYS_DMA_ChannelRelease(dHandle);
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
    //DRV_PCA9655_SetPort(&ioexpander1_data, 5, fa2lse); // FALSE

}

static uint32_t scallback_count;

static void callback_sound_onoff(uintptr_t context, uint32_t alarmcount)
{
    DRV_HANDLE *handle = (DRV_HANDLE *) context;
    if (scallback_count >= (2 / SOUND_LENGTH)) {
        scallback_count = 0;
        render_flag[play_slot % 2] = false;
        play_slot++;
        play_slot = play_slot % 2;
        if (render_flag[play_slot]) {
            //add_sound_nodma(play_slot);
        }
        return;
    }
    if (render_flag[play_slot]) {
        int16_t *p = &(sample_buffer[(play_slot % 2) * SOUND_LENGTH / 2]);
        asm volatile("NOP");
        taskENTER_CRITICAL();
        OC2RS = p[scallback_count];
        //DRV_OC_PulseWidthSet(*handle, p[scallback_count]);
        scallback_count++;
        taskEXIT_CRITICAL();

    }
    return;
}
static uint32_t wrote_bytes;

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

    play_slot = 0;
    render_slot = 1;
    taskENTER_CRITICAL();
    play_slot = 0;
    render_slot = 0;

    taskEXIT_CRITICAL();
    p = sample_buffer;
    DRV_HANDLE tHandle = DRV_HANDLE_INVALID;
    DRV_HANDLE oHandle = DRV_HANDLE_INVALID;
    SYS_DMA_CHANNEL_HANDLE dHandle = SYS_DMA_CHANNEL_HANDLE_INVALID;

    //DRV_PCA9655_SetPort(&ioexpander1_data, 4, false); // Audio OFF: Must call from housekeeping at initialize.
#if 1
    dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
#endif

    memset(sample_buffer, 0x00, sizeof (sample_buffer));
    rmod = 0;
    onoff = true;
    nlen = SOUND_LENGTH;
    state = C_SOUND_INIT;
    //state = C_SOUND_PLAY;
    //dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
    //rlen = render_sound(sample_buffer, nlen, 255, &rmod, &onoff);
    render_flag[0] = render_flag[1] = false;
    while (1) {
        // Wait Queue
        CmdQueue = C_SOUND_INIT;
        if (xQueueReceive(xSoundCmdQueue, (void *) (&CmdQueue), 0) == pdPASS) {
            switch (CmdQueue) {
            case C_SOUND_START:
                //a = SYS_DMA_CHANNEL_1;
                if (state != C_SOUND_PLAY) {
                    RPA1Rbits.RPA1R = 0b0101; // Sound ON / OC2
                    SYS_DMA_Suspend();
                    memset(sample_buffer, 0x00, sizeof (sample_buffer));
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
                        //DRV_TMR_AlarmRegister(tHandle, 1, true, (uintptr_t)(&oHandle), (DRV_TMR_CALLBACK)callback_sound_onoff);
                        //DRV_TMR_AlarmEnable(tHandle, true);
                    }
#if 1
                    if (dHandle != SYS_DMA_CHANNEL_HANDLE_INVALID) {
                        //SYS_DMA_ChannelTransferEventHandlerSet(dHandle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), 0);
                        state = C_SOUND_PLAY;
                    }
#endif
                    state = C_SOUND_PLAY;
                    {
                        // INIT MML.
                        for (i = 0; i < 3; i++) {
                            mmldata[i].is_eof = false;
                            mmldata[i].is_end = false;
                            mmldata[i].mmlbase = (char *) test_mml1; // ToDo: Change this.
                            mmldata[i].mmlpos = 0;
                            mmldata[i].sound_length = 4;
                            mmldata[i].tempo = (60 * SOUND_RATE * 4) / 64; // T=64 OK?
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
                            mmldata[i].regs.ponoff = false;
                            mmldata[i].regs.rmod = 0;
                        }
                        taskENTER_CRITICAL();
                        play_slot = 1;
                        render_slot = 0;
                        play_avail = 0;
                        render_flag[0] = render_flag[1] = false;
                        wrote_bytes = 0;
                        taskEXIT_CRITICAL();
                    }
                }
                break;
            case C_SOUND_ABORT:
            case C_SOUND_STOP:
                if (state == C_SOUND_PLAY) {
                    sound_stop(&state, &tHandle, &oHandle, &dHandle);
                    vTaskDelay(cTick100ms);
                }
                break;
            default:
                break;
            }
        }
        if (state == C_SOUND_PLAY) {
            if (dHandle == SYS_DMA_CHANNEL_HANDLE_INVALID) {
                vTaskDelay(cTick100ms);
            } else {
                b_cont = mmldata[0].is_end; // ToDo
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
                        s_count = (SOUND_LENGTH * render_slot) / 2;
                        rlen = 0;
                        maxlen = SOUND_LENGTH / 2;
                        wrote_bytes = 0;
                        //SYS_DMA_Suspend();
                        memset(&(sample_buffer[s_count]), 0x00, maxlen * sizeof (int16_t));
                        //SYS_DMA_Resume();
                        while (maxlen > 0) {
                            //SYS_DMA_Suspend();
                            rlen = render_mml(&(sample_buffer[s_count]), &(mmldata[0]), maxlen);
                            //SYS_DMA_Resume();
                            s_count += rlen;
                            maxlen = maxlen - (int) rlen;
                            wrote_bytes += rlen;
                            if (s_count >= SOUND_LENGTH) break;
                            if (mmldata[0].is_end) break;
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
                        //SYS_DMA_ChannelEnable(dHandle);
                        if (render_slot >= 2) render_slot = 0;
                        wrote_bytes = 0;
                        //}
                        //vTaskDelay(cTick100ms / 8);
                    } else {
                        //vTaskDelay(2);
                        vTaskDelay(cTick100ms / 10);
                    }
                } else {
                    { // re-enter
                        for (i = 0; i < 3; i++) {
                            mmldata[i].is_eof = false;
                            mmldata[i].is_end = false;
                            mmldata[i].mmlbase = (char *) test_mml1; // ToDo: Change this.
                            mmldata[i].mmlpos = 0;
                            mmldata[i].sound_length = 4;
                            mmldata[i].tempo = (60 * SOUND_RATE * 4) / 64; // T=64 OK?
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
                            mmldata[i].regs.ponoff = false;
                            mmldata[i].regs.rmod = 0;
                        }
                        taskENTER_CRITICAL();
                        play_slot = 1;
                        render_slot = 0;
                        play_avail = 0;
                        render_flag[0] = render_flag[1] = false;
                        wrote_bytes = 0;
                        taskEXIT_CRITICAL();

                    }
                    vTaskDelay(cTick100ms / 6);
                }
            }
        } else {
            vTaskDelay(cTick100ms);
        }
    }

}
