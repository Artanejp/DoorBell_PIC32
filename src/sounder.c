
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


const uint16_t sound_level_table[32] = {
    0, 2, 3, 4, 6, 8, 12, 16,
    20, 23, 28, 30, 33, 37, 41, 44,
    47, 52, 55, 59, 64, 68, 73, 77,
    82, 87, 92, 98, 103, 110, 120, 125
};

const uint32_t note_std_o4[] = {
    440000, 493883, 523251, 587330, 659255, 698456, 783991, // A B C D E F G
};

const uint32_t note_upper_o4[] = {
    466164, 493883, 554365, 622254, 659255, 739989, 830609, // A# B C# D# E F# G#
};

const uint32_t note_lower_o4[] = {
    415305, 466164, 523251, 554365, 622254, 698456, 739989, // Ab(G#), Bb C Db Eb F Gb
};

/*
 * MML SYNTAX from "FM-7 F-BASIC Grammer book (???)" 
 * By FUJITSU LIMITED, Apr. 1984, at PAGE 3-137 to 3-144.
 */

const char *test_mml1 = "T150V7\
r1o5d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12\
<g8g+8a+8d+4<c+4>b2a+2^a+8<c+8d+8e8>g+4<a+4g1>d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12\
<a+8d+8g4d+8g+8a+8b2a+4<d8d+8>g+1^g+1a+4b8d+4r4.a+4b8<c+4>b4a+8d+4.f+1.^f+8b4<c+8>g+4r4.\
b4<c+8e4d+4^d+16c+2.^c+16d+8>a+1^a+8b4g+8r4\
<e4>b4b8g+8r4<e8r4d+2..>b8r1<c+4>b8r4g+4f4.r4.\
f8g+8a+4a+8a+8r4b4a+4a+8a+8r4b4a+4a+8a+4a+4b8a+1^a+8r1d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12\
<g8g+8a+8d+4<c+4>b2a+2^a+8<c+8d+8e8>g+4<a+4g1>d+8g+8g8d+2^d+6r2..r12\
>b8<e8d+8>b2^b6r1r12";
/*
<a+8d+8g4d+8g+8a+8b2a+4<d8d+8>g+1^g+1a+4b8d+4r4.a+4b8<c+4>b4a+8d+4.f+1.^f+8b4<c+8>g+4r4.\
b4<c+8e4d+4^d+16c+2.^c+16d+8>a+1^a+8b4g+8r4<e4>b4b8g+8r4<e8r4d+2..>b8r1\
<c+4>b8r4g+4f4.r4.f8g+8a+4a+8a+8r4b4a+4a+8a+8r4b4a+4a+8a+4a+4b8a+1^a+8";
 */
//const char *test_mml1= "T150V2 ";
/*
const char *test_mml2 = "T150V2\
r2o3f4f+4g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
e8e16e16e8e16e16e8e16e16e8e16e16\
e8e16e16e8e16e16e8e16e16e8e16e16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
b8b16b16b8b16b16a+8a+16a+16a+8a+16a+16\
f+8f+16f+16f+8f+16f+16f+8f+16f+16f+8f+16f+16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
e8e16e16e8e16e16e8e16e16e8e16e16\
e8e16e16e8e16e16e8e16e16e8e16e16\
d+8d+16d+16d+8d+16d+16d+8d+16d+16d+8d+16d+16\
b8b16b16b8b16b16a+8a+16a+16a+8a+16a+16\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+8.\
g+16g+16g+8g+16g+16g+8g+8g8g8\
";
 */
const char *test_mml2 = "T150V2\
o4g+8r4g+1^g+4g+4.g+8r4e1^e4.r4\
d+1d+2f+2f+1d+1g+8r4g+1^g+4g+8r4e8r4e1^e4d+4.d+1\
d+2d+2g+1^g+1<c+4c+8c+8r2c+4c+8c+4c+4.>b4.b2.b4b2^b8g+4g+8g+8g+2g+4g+8g+4g+4.\
<c+2.c+8d+1^d+8>b4b8r4b4b4b8b8r4b4.b2..b2.b4.<c+4c+8r4c+4c+2.c+8c+8c+4\
d+8d+8r4d+4d+4d+8d+8r4d+4d+4d+8d+8r4.d+8d+1^d+8r1\
>g+8r4g+1^g+4g+4.g+8r4e1^e4.r4\
d+1d+2f+2f+1d+1g+8r4g+1^g+4g+8r4\
e8r4e1^e4d+4.d+1d+2d+2g+1^g+1<c+4c+8c+8r2c+4c+8c+4c+4.>b4.b2.";
/*b4b2^b8g+4g+8g+8g+2g+4g+8g+4g+4.<c+2.c+8d+1^d+8>b4b8r4b4b4b8b8r4b4.b2..b2.b4.\
<c+4c+8r4c+4c+2.c+8c+8c+4d+8d+8r4d+4d+4d+8d+8r4d+4d+4d+8d+8r4.d+8d+1^d+8";*/

const char *test_mml3 = "T150V3\
r1o5d+8r4d+1^d+4d+4.d+8r4>b1^b4.r4\
a+1b2<c+2c+1>a+1<d+8r4d+1^d+4d+8r4\
>b8r4b1^b4b4.a+1b2a+2<d+1^d+1a+4a+8a+8r2\
a+4a+8a+4a+4.f+4.f+2.f+4f+2^f+8d+4d+8d+8d+2\
d+4d+8d+4d+4.g+2.g+8a+1^a+8g+4g+8r4g+4g+4g+8g+8r4g+4.f+2..f+2.f+4.\
g+4g+8r4g+4g+2.g+8g+8g+4a+8a+8r4a+4a+4a+8a+8r4a+4a+4a+8a+8r4.a+8a+1^a+8r1\
d+8r4d+1^d+4d+4.d+8r4>b1^b4.r4\
a+1b2<c+2c+1>a+1<d+8r4d+1^d+4d+8r4>b8r4\
";
/* b1^b4b4.a+1b2a+2<d+1^d+1a+4a+8a+8r2 \
a+4a+8a+4a+4.f+4.f+2.f+4f+2^f+8d+4d+8d+8d+2d+4d+8d+4d+4.g+2.g+8a+1^a+8g+4g+8r4g+4g+4g+8g+8r4\
g+4.f+2..f+2.f+4.g+4g+8r4g+4g+2.g+8g+8g+4a+8a+8r4a+4a+4a+8a+8r4a+4a+4a+8a+8r4.a+8a+1^a+8";
*/
//const char test_mml1[] = "O4C16R16C16";

QueueHandle_t xSoundCmdQueue;
QueueHandle_t xSoundQueue;
TimerHandle_t xSoundTimer;

static uint32_t scallback_count;
static int render_slot;
static int play_avail;
static bool render_flag[4];

extern PCA9655_t ioexpander1_data;

static int16_t __attribute__((aligned(16))) sample_buffer[SOUND_LENGTH + 32]; // 0.2sec
static char __attribute__((aligned(16))) mmlstr[3][512];
static int mmllen[3];

//static int16_t *sample_buffer;

#define TEST_FREQ 440
#define SAMPLE_FREQ SOUND_RATE
#define PSG_CLOCK 1228000

typedef struct {
    uint32_t freq; // Hz
    uint32_t envelope_pitch_val; // Tick. 1Tick = 16KHz.
    uint32_t envelope_pitch_mod; // Tick. 1Tick = 16KHz.
    uint32_t howlong;
    uint32_t noise_freq;
    uint8_t noise_pos;
    int16_t vol;
    int16_t env_tmp_vol;
    uint8_t env_pos;
    uint8_t env_type;
    uint32_t env_count;
    uint32_t env_mod;
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


static SOUND_MML_T mmldata[3];
static uint32_t wrote_bytes;

static uint32_t render_null(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    uint32_t rp = 0;
    int16_t *p = data;

    if (regs == NULL) return samples;
    if ((sample_buffer == NULL) || (data == NULL)) return samples;
    int16_t pvol;
    int i;
#if 0
    pvol = 0;

    for (i = 0; i < samples; i++) {
        p[i] += pvol;
        if (p[i] > 450) p[i] = 450; // Saturation
    }
#endif
    return samples;
}

static uint32_t render_op(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    uint32_t rp = 0;
    int16_t *p = data;
    int i;
    bool onoff = true;
    static const uint32_t sp = (SAMPLE_FREQ * 1000) / 2;

    if (regs == NULL) return samples;
    if ((sample_buffer == NULL) || (data == NULL)) return samples;
    uint32_t freq = regs->regs.freq;
    int16_t vol;
    int16_t pvol;
    int16_t on_vol;
    int16_t off_vol;

    if (regs->regs.env_on) {
        vol = regs->regs.env_tmp_vol;
    } else {
        vol = regs->regs.vol;
    }
    rp = regs->regs.rmod;
    onoff = regs->regs.ponoff;
    on_vol = vol;
    off_vol = 0;
    pvol = (onoff) ? on_vol : off_vol;
    for (i = 0; i < samples; i++) {
        if (rp >= sp) {
            rp -= sp;
            onoff = !onoff;
            pvol = (onoff) ? on_vol : off_vol;
        }
        p[i] += pvol;
        //if (p[i] > 450) p[i] = 450; // Saturation
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

// See PAGE 13 of http://www.ym2149.com/ay8910.pdf .

static uint32_t envelope_type9(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos < 31) {
            if (ui >= regs->regs.env_count) {
                regs->regs.env_tmp_vol = sound_level_table[31 - regs->regs.env_pos];
                render_op(data, regs, regs->regs.env_count);
                ui -= (int) (regs->regs.env_count);
                regs->regs.env_count = regs->regs.envelope_pitch_val;
                regs->regs.env_mod += regs->regs.envelope_pitch_mod;
                if (regs->regs.env_mod >= SAMPLE_FREQ) {
                    regs->regs.env_mod -= SAMPLE_FREQ;
                    regs->regs.env_count++;
                }
                regs->regs.env_pos++;
            } else {
                regs->regs.env_tmp_vol = sound_level_table[31 - regs->regs.env_pos];
                render_op(data, regs, ui);
                regs->regs.env_count -= (uint32_t) ui;
                ui = 0;
                break;
            }
        } else {
            regs->regs.env_tmp_vol = 0;
            ui = 0;
            break;
        }
    }
    return samples;
}

// Type9 With Keep Upper

static uint32_t envelope_type11(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos < 31) {
            if (ui >= regs->regs.env_count) {
                regs->regs.env_tmp_vol = sound_level_table[31 - regs->regs.env_pos];
                render_op(data, regs, regs->regs.env_count);
                ui -= (int) (regs->regs.env_count);
                regs->regs.env_count = regs->regs.envelope_pitch_val;
                regs->regs.env_mod += regs->regs.envelope_pitch_mod;
                if (regs->regs.env_mod >= SAMPLE_FREQ) {
                    regs->regs.env_mod -= SAMPLE_FREQ;
                    regs->regs.env_count++;
                }
                regs->regs.env_pos++;
            } else {
                regs->regs.env_tmp_vol = sound_level_table[31 - regs->regs.env_pos];
                render_op(data, regs, ui);
                regs->regs.env_count -= (uint32_t) ui;
                ui = 0;
                break;
            }
        } else {
            regs->regs.env_tmp_vol = sound_level_table[30];
            ui = 0;
            break;
        }
    }
    return samples;
}

static uint32_t envelope_type4(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos < 31) {
            if (ui >= regs->regs.env_count) {
                regs->regs.env_tmp_vol = sound_level_table[regs->regs.env_pos];
                render_op(data, regs, regs->regs.env_count);
                ui -= (int) (regs->regs.env_count);
                regs->regs.env_count = regs->regs.envelope_pitch_val;
                regs->regs.env_mod += regs->regs.envelope_pitch_mod;
                if (regs->regs.env_mod >= SAMPLE_FREQ) {
                    regs->regs.env_mod -= SAMPLE_FREQ;
                    regs->regs.env_count++;
                }
                regs->regs.env_pos++;
            } else {
                regs->regs.env_tmp_vol = sound_level_table[regs->regs.env_pos];
                render_op(data, regs, ui);
                regs->regs.env_count -= (uint32_t) ui;
                ui = 0;
                break;
            }
        } else {
            regs->regs.env_tmp_vol = 0;
            render_op(data, regs, ui);
            ui = 0;
            break;
        }
    }
    return samples;
}

static uint32_t envelope_type13(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos < 31) {
            if (ui >= regs->regs.env_count) {
                regs->regs.env_tmp_vol = sound_level_table[regs->regs.env_pos];
                render_op(data, regs, regs->regs.env_count);
                ui -= (int) (regs->regs.env_count);
                regs->regs.env_count = regs->regs.envelope_pitch_val;
                regs->regs.env_mod += regs->regs.envelope_pitch_mod;
                if (regs->regs.env_mod >= SAMPLE_FREQ) {
                    regs->regs.env_mod -= SAMPLE_FREQ;
                    regs->regs.env_count++;
                }
                regs->regs.env_pos++;
            } else {
                regs->regs.env_tmp_vol = sound_level_table[regs->regs.env_pos];
                render_op(data, regs, ui);
                regs->regs.env_count -= (uint32_t) ui;
                ui = 0;
                break;
            }
        } else {
            regs->regs.env_tmp_vol = sound_level_table[30];
            render_op(data, regs, ui);
            ui = 0;
            break;
        }
    }
    return samples;
}

// Type0 with repeat.

static uint32_t envelope_type8(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos >= 32) {
            regs->regs.env_pos = 0;
        }
        if (ui >= regs->regs.env_count) {
            regs->regs.env_tmp_vol = sound_level_table[31 - regs->regs.env_pos];
            render_op(data, regs, regs->regs.env_count);
            ui -= (int) (regs->regs.env_count);
            regs->regs.env_count = regs->regs.envelope_pitch_val;
            regs->regs.env_mod += regs->regs.envelope_pitch_mod;
            if (regs->regs.env_mod >= SAMPLE_FREQ) {
                regs->regs.env_mod -= SAMPLE_FREQ;
                regs->regs.env_count++;
            }
            regs->regs.env_pos++;
        } else {
            regs->regs.env_tmp_vol = sound_level_table[31 - regs->regs.env_pos];
            render_op(data, regs, ui);
            regs->regs.env_count -= (uint32_t) ui;
            ui = 0;
            break;
        }
    }
    return samples;
}
//Type4 with repeat;

static uint32_t envelope_type12(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos >= 32) {
            regs->regs.env_pos = 0;
        }
        if (ui >= regs->regs.env_count) {
            regs->regs.env_tmp_vol = sound_level_table[regs->regs.env_pos];
            render_op(data, regs, regs->regs.env_count);
            ui -= (int) (regs->regs.env_count);
            regs->regs.env_count = regs->regs.envelope_pitch_val;
            regs->regs.env_mod += regs->regs.envelope_pitch_mod;
            if (regs->regs.env_mod >= SAMPLE_FREQ) {
                regs->regs.env_mod -= SAMPLE_FREQ;
                regs->regs.env_count++;
            }
            regs->regs.env_pos++;
        } else {
            regs->regs.env_tmp_vol = sound_level_table[regs->regs.env_pos];
            render_op(data, regs, ui);
            ui = 0;
            regs->regs.env_pos++;
            regs->regs.env_count = regs->regs.envelope_pitch_val;
            regs->regs.env_mod += regs->regs.envelope_pitch_mod;
            if (regs->regs.env_mod >= SAMPLE_FREQ) {
                regs->regs.env_mod -= SAMPLE_FREQ;
                regs->regs.env_count++;
            }
            break;
        }
    }
    return samples;
}

static uint32_t envelope_type10(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos >= 64) {
            regs->regs.env_pos = 0;
        }
        uint8_t npos = (regs->regs.env_pos >= 32) ? (regs->regs.env_pos - 32) : (32 - regs->regs.env_pos);
        if (ui >= regs->regs.env_count) {
            regs->regs.env_tmp_vol = sound_level_table[npos];
            render_op(data, regs, regs->regs.env_count);
            ui -= (int) (regs->regs.env_count);
            regs->regs.env_count = regs->regs.envelope_pitch_val;
            regs->regs.env_mod += regs->regs.envelope_pitch_mod;
            if (regs->regs.env_mod >= SAMPLE_FREQ) {
                regs->regs.env_mod -= SAMPLE_FREQ;
                regs->regs.env_count++;
            }
            regs->regs.env_pos++;
        } else {
            regs->regs.env_tmp_vol = sound_level_table[npos];
            render_op(data, regs, ui);
            regs->regs.env_count -= (uint32_t) ui;
            ui = 0;
            break;
        }
    }
    return samples;
}

static uint32_t envelope_type14(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int ui = (int) samples;
    while (ui > 0) {
        if (regs->regs.env_pos >= 64) {
            regs->regs.env_pos = 0;
        }
        uint8_t npos = (regs->regs.env_pos >= 32) ? (63 - regs->regs.env_pos) : regs->regs.env_pos;
        if (ui >= regs->regs.env_count) {
            regs->regs.env_tmp_vol = sound_level_table[npos];
            render_op(data, regs, regs->regs.env_count);
            ui -= (int) (regs->regs.env_count);
            regs->regs.env_count = regs->regs.envelope_pitch_val;
            regs->regs.env_mod += regs->regs.envelope_pitch_mod;
            if (regs->regs.env_mod >= SAMPLE_FREQ) {
                regs->regs.env_mod -= SAMPLE_FREQ;
                regs->regs.env_count++;
            }
            regs->regs.env_pos++;
        } else {
            regs->regs.env_tmp_vol = sound_level_table[npos];
            render_op(data, regs, ui);
            regs->regs.env_count -= (uint32_t) ui;
            ui = 0;
            break;
        }
    }
    return samples;
}

static uint32_t render_sound(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    if (regs->regs.env_on) {
        uint32_t retval;
        switch (regs->regs.env_type & 15) {
        case 0:
        case 1:
        case 2:
        case 3:
            retval = envelope_type9(data, regs, samples);
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            retval = envelope_type4(data, regs, samples);
            break;
        case 8:
            retval = envelope_type8(data, regs, samples);
            break;
        case 9:
            retval = envelope_type9(data, regs, samples);
            break;
        case 10:
            retval = envelope_type10(data, regs, samples);
            break;
        case 11:
            retval = envelope_type11(data, regs, samples);
            break;
        case 12:
            retval = envelope_type12(data, regs, samples);
            break;
        case 13:
            retval = envelope_type13(data, regs, samples);
            break;
        case 14:
            retval = envelope_type13(data, regs, samples);
            break;
        case 15:
            retval = envelope_type4(data, regs, samples);
            break;
        default:
            retval = render_op(data, regs, samples);
            break;
        }
        return retval;
    } else {
        return render_op(data, regs, samples);
    }
}

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
            p_samples += render_null(head_data, regs, n_samples);
        } else {
            p_samples += render_sound(head_data, regs, n_samples);
        }
        if (left == 0) {
            *ps = p_samples;
            *p_left = 0;
            return false;
        }
        *p_left = left;
        *ps = p_samples;
        return true;
    } else if (regs->stop_length > 0) {
        if (left > regs->stop_length) {
            n_samples = regs->stop_length;
            left -= regs->stop_length;
            regs->stop_length = 0;
        } else {
            n_samples = left;
            regs->stop_length -= left;
            left = 0;
        }
        //p_samples += n_samples;
        p_samples += render_null(head_data, regs, n_samples);
        if (left == 0) {
            *ps = p_samples;
            *p_left = 0;
            return false;
        }
        *p_left = left;
        *ps = p_samples;
        return true;
    }
    return true;
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
    char nbuf[12];
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
        if (regs->regs.env_on) { // ToDo: Connect note (TI).
            regs->regs.env_pos = 0;
            regs->regs.env_count = 0;
            regs->regs.env_mod = 0;
        }
    } else if ((head_char >= 'a') && (head_char <= 'g')) {
        basefreq = note_std_o4[head_char - 'a'];
        need_next = true;
        b_note = true;
        note = head_char;
        if (regs->regs.env_on) { // ToDo: Connect note (TI).
            regs->regs.env_pos = 0;
            regs->regs.env_count = 0;
            regs->regs.env_mod = 0;
        }
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
        if (regs->regs.env_on) { // "V" triggers turning off ENVELOPE.
            regs->regs.env_pos = 0;
            regs->regs.env_count = 0;
            regs->regs.env_mod = 0;
            regs->regs.env_on = false;
        }
        nlen = strlen(nbuf);
        if (nlen > 0) { // "Vxx" sets volume, "V" restores recent volume.
            longval = strtol(nbuf, NULL, 10);
            if ((longval >= 0) && (longval < 16)) {
                regs->regs.vol = sound_level_table[longval << 1];
            }
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
        } while (mmlptr < (uint32_t)mml_len);
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
        if ((longval >= 0) && (longval <= 12)) {
            regs->octave = longval;
        }
    } else if (head_char == '>') {
        if (regs->octave > 0) {
            regs->octave -= 1;
        }
        need_next = false;
        mmlptr++;
        p++;
    } else if (head_char == '<') {
        if (regs->octave < 12) {
            regs->octave += 1;
        }
        need_next = false;
        mmlptr++;
        p++;
    } else if ((head_char == 'S') || (head_char == 's')) { // ENVELOPE
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
        regs->regs.env_pos = 0;
        regs->regs.env_count = 0;
        regs->regs.env_mod = 0;
        regs->regs.env_on = true;
        nlen = strlen(nbuf);
        if (nlen > 0) { // Set type
            longval = strtol(nbuf, NULL, 10);
            if ((longval >= 0) && (longval < 16)) {
                regs->regs.env_type = (uint8_t) longval;
            }
        }
        if (mmlptr < (uint32_t)mmllen) {
            if (*p == ',') { // Freq
                p++;
                mmlptr++;
                memset(nbuf, 0x00, sizeof (nbuf));
                nptr = 0;
                // Freq = (1.228e6 /16) / y
                const uint32_t basefreq = PSG_CLOCK;
                for (; mmlptr < mml_len; mmlptr++, p++) {
                    head_char = *p;
                    if ((*p >= '0') && (*p <= '9')) {
                        nbuf[nptr] = head_char;
                        nptr++;
                    } else {
                        break;
                    }
                    if (nptr > 5) break;
                }
                nlen = strlen(nbuf);
                if (nlen > 0) { // Set type
                    longval = strtol(nbuf, NULL, 10);
                    if (longval >= 0) {
                        // Freq = basefreq / (256 * val) [Hz]
                        // Pitch_val = Freq / 16KHz 
                        longval = longval & 0x3ff; // 10bit
                        uint32_t freq = basefreq / longval;
                        uint32_t pitch_val = (SAMPLE_FREQ << 7) / freq;
                        uint32_t pitch_mod = (SAMPLE_FREQ << 7) - (pitch_val * freq);
                        pitch_val >>= 5;
                        pitch_mod >>= 5;
                        if(pitch_val == 0) {
                            regs->regs.env_on = false;
                        }
                        regs->regs.envelope_pitch_val = pitch_val;
                           regs->regs.envelope_pitch_mod = pitch_mod;
                    }
                }
            }
        }
    } else {
        mmlptr++; // Skip
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
            if ((head_char == 'R') || (head_char == 'r')) break; // Next
            if ((head_char == 'S') || (head_char == 's')) break; // Next
            if ((head_char == '<') || (head_char == '>')) break; // Next

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
            if (longval >= 16) {
                regs->tmp_sound_length = regs->tempo / (longval / 16);
            } else {
                regs->tmp_sound_length = regs->tempo; // Fallback
            }
            regs->stop_length = 0;
        } else {
            regs->tmp_sound_length = 0;
            if (longval >= 16) {
                regs->stop_length = regs->tempo / (longval / 16);
            } else {
                regs->stop_length = regs->tempo; // Fallback
            }
        }
        // Calc Frequency
        if (b_note) {
            if (regs->octave < 5) {
                basefreq = basefreq >> (5 - regs->octave);
                //basefreq = basefreq / 1000;
            } else if (regs->octave > 5) {
                basefreq = basefreq << (regs->octave - 5);
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
            sndptr += render_sound(&(head_data[sndptr]), regs, n_samples);
            regs->tmp_sound_length -= n_samples;
        } else {
            sndptr += render_null(&(head_data[sndptr]), regs, n_samples);
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
    if (head_data == NULL) return 0;
    if (sample_buffer == NULL) return max_samples;
    if (regs == NULL) return max_samples;
    if (regs->mmlbase == NULL) return max_samples;
    if (regs->is_end) return max_samples;

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
            return p_samples;
        }
    }
    configASSERT((p_samples > (SOUND_LENGTH / 2)) ? 0 : 1);
    return p_samples;
}
void sndDmaEventHandler(SYS_DMA_TRANSFER_EVENT event, SYS_DMA_CHANNEL_HANDLE handle, uintptr_t contextHandle);

static __inline__ void add_sound_isr(SYS_DMA_CHANNEL_HANDLE handle, uint32_t slot)
{
    static const uint32_t nlen = SOUND_LENGTH;
    uint32_t sp = ((slot % 2) * nlen) / 2;
    if (sample_buffer == NULL) return;
    SYS_DMA_ChannelTransferEventHandlerSet(handle, (const SYS_DMA_CHANNEL_TRANSFER_EVENT_HANDLER) (&sndDmaEventHandler), slot);
    SYS_DMA_ChannelTransferSet(handle, (const void *) (&(sample_buffer[sp])), (nlen * sizeof (int16_t)) / 2, (const void *) (&OC2RS), sizeof (int32_t), sizeof (int16_t));
    SYS_DMA_ChannelSetup(handle, SYS_DMA_CHANNEL_OP_MODE_BASIC, DMA_TRIGGER_TIMER_3);
    SYS_DMA_ChannelEnable(handle);
}

static void add_sound(SYS_DMA_CHANNEL_HANDLE handle, uint32_t slot)
{
    static const uint32_t nlen = SOUND_LENGTH;
    uint32_t sp = ((slot % 2) * nlen) / 2;
    if (sample_buffer == NULL) return;
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

static void init_mmls(char *mml1, char *mml2, char *mml3);

static void sound_start(int *state, DRV_HANDLE *ptHandle, DRV_HANDLE *poHandle, SYS_DMA_CHANNEL_HANDLE *pdHandle)
{
    DRV_HANDLE tHandle;
    DRV_HANDLE oHandle;
    SYS_DMA_CHANNEL_HANDLE dHandle;

    RPA1Rbits.RPA1R = 0b0101; // Sound ON / OC2
    dHandle = SYS_DMA_ChannelAllocate(DMA_CHANNEL_0);
    if (sample_buffer != NULL) {
        SYS_DMA_Suspend();
        memset(sample_buffer, 0x00, sizeof (int16_t) * (SOUND_LENGTH + 32));
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
    init_mmls(NULL, NULL, NULL);
}

static void init_mmls(char *mml1, char *mml2, char *mml3)
{
    int i;
    char *pmml[3];
    // INIT MML.
    taskENTER_CRITICAL();
    for (i = 0; i < 3; i++) {
        mmldata[i].is_eof = false;
        mmldata[i].is_end = false;
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
        mmldata[i].regs.vol = sound_level_table[16];
        mmldata[i].regs.noise_freq = 440;
        mmldata[i].regs.noise_pos = 0;

        mmldata[i].regs.env_tmp_vol = 31;
        mmldata[i].regs.env_pos = 0;
        mmldata[i].regs.env_count = 0;
        mmldata[i].regs.env_mod = 0;
        mmldata[i].regs.env_type = 0;
        mmldata[i].regs.envelope_pitch_val = 1;
        mmldata[i].regs.envelope_pitch_mod = SAMPLE_FREQ;
        mmldata[i].regs.ponoff = false;
        mmldata[i].regs.rmod = 0;
        memset(mmlstr[i], 0x00, 512 * sizeof (char));
        mmllen[i] = 0;
    }

    if (mml1 == NULL) {
        pmml[0] = (char *) test_mml1;
    } else {
        pmml[0] = mml1;
    }
    if (mml2 == NULL) {
        pmml[1] = (char *) test_mml2;
    } else {
        pmml[1] = mml2;
    }
    if (mml3 == NULL) {
        pmml[2] = (char *) test_mml3;
    } else {
        pmml[2] = mml3;
    }
    for (i = 0; i < 3; i++) {
        if (pmml[i] != NULL) {
            mmllen[i] = strlen(pmml[i]);
            strncpy(mmlstr[i], pmml[i], 512 - 1);
        }
        mmldata[i].mmlbase = mmlstr[i];
    }
    //taskENTER_CRITICAL();
    render_slot = 0;
    play_avail = 0;
    render_flag[0] = render_flag[1] = false;
    wrote_bytes = 0;
    taskEXIT_CRITICAL();
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
    p = sample_buffer;
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
            if ((dHandle == SYS_DMA_CHANNEL_HANDLE_INVALID) || (sample_buffer == NULL)) {
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
                        memset(&(sample_buffer[s_count]), 0x00, maxlen * sizeof (int16_t));
                        //SYS_DMA_Resume();
                        int __ch = 0;
                        int __sc;
                        for (__ch = 0; __ch < 3; __ch++) {
                            __sc = s_count;
                            if (mmldata[__ch].is_end) continue;
                            while (_mb[__ch] > 0) {
                                //SYS_DMA_Suspend();
                                rlen = render_mml(&(sample_buffer[__sc]), &(mmldata[__ch]), _mb[__ch]);
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
                                sample_buffer[s_count + ii] = sample_buffer[s_count + ii] >> 5;
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
