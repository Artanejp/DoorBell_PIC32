/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
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
#include "sounder.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */
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
 * EXCEPTS:
 * Sx[,y] : Set envelope type [x] to param[y]. Y = 1228000 / (expect_freq * 256)
 * type [x] : bit8..
 * Xxxxxx : Set noise frequency [Hz].If 0 or minus, Unset noise.
 */

const char *test_mml1 = "T150V12S16,1800\
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
>b8<e8d+8>b2^b6r1r12\
<a+8d+8g4d+8g+8a+8b2a+4<d8d+8>g+1^g+1a+4b8d+4r4.a+4b8<c+4>b4a+8d+4.f+1.^f+8b4<c+8>g+4r4.\
b4<c+8e4d+4^d+16c+2.^c+16d+8>a+1^a+8b4g+8r4<e4>b4b8g+8r4<e8r4d+2..>b8r1\
<c+4>b8r4g+4f4.r4.f8g+8a+4a+8a+8r4b4a+4a+8a+8r4b4a+4a+8a+4a+4b8a+1^a+8\
";
#if 1
const char *test_mml2 = "T150V3S16,400\
r2o3f4f+4\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
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
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
g+8g+8g+8g+16g+16g+8g+8g+8g+4\
g+8g+8g+16g+16g+8g+8g+8g+8\
e8e8e8e8e8e8e8f+4\
f+8f+8f+8f+8f+8f+8f+8\
e8e8e8e8e8e8e8e8\
e8e8e8e8e8e8e8e8\
f+8f+8f+8f+8f+8f+8f+8f+8\
f+8f+8f+8f+8f+8f+8f+8f+8\
c+8c+8c+8c+8c+8c+8c+8c+8\
c+8c+8c+8c+8c+8c+8c+8d+4\
d+8d+8r4d+4d+4\
d+8d+8r4d+4r4.\
d+8r4d+4d+1^d+8\
r2f4f+4\
g+8g+16g+16g+8g+16g+16g+8g+16g+16g+8g+16g+16\
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
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
f+8f+8f+8f+16f+16f+8f+8f+8f+16f+8.\
f+8f+8f+16f+16f+8f+8f+8f+8\
g+8g+8g+8g+16g+16g+8g+8g+8g+4\
g+8g+8g+16g+16g+8g+8g+8g+8\
e8e8e8e8e8e8e8f+4\
f+8f+8f+8f+8f+8f+8f+8\
e8e8e8e8e8e8e8e8\
e8e8e8e8e8e8e8e8\
f+8f+8f+8f+8f+8f+8f+8f+8\
f+8f+8f+8f+8f+8f+8f+8f+8\
c+8c+8c+8c+8c+8c+8c+8c+8\
c+8c+8c+8c+8c+8c+8c+8d+4\
d+8d+8r4d+4d+4\
d+8d+8r4d+4r4.\
d+8r4d+4d+1^d+8\
";

const char *test_mml3 = "T150V4\
r1\
o5d+8r4d+1^d+4\
d+4.\
d+8r4>b1^b4.\
r4\
a+1\
b2<c+2\
c+1\
>a+1\
<d+8r4d+1^d+4\
d+8r4\
>b8r4b1^b4\
b4.\
a+1\
b2a+2\
<d+1^d+1\
a+4a+8a+8r2\
a+4a+8a+4a+4.\
f+4.f+2.\
f+4f+2^f+8\
d+4d+8d+8d+2\
d+4d+8d+4d+4.\
g+2.g+8a+1^a+8\
g+4g+8r4g+4g+4\
g+8g+8r4g+4.\
f+2..f+2.\
f+4.\
g+4g+8r4g+4g+2.\
g+8g+8g+4\
a+8a+8r4a+4a+4\
a+8a+8r4a+4a+4\
a+8a+8r4.a+8a+1^a+8\
r1\
d+8r4d+1^d+4\
d+4.\
d+8r4>b1^b4.\
r4\
a+1\
b2<c+2\
c+1\
>a+1\
<d+8r4d+1^d+4\
d+8r4\
>b8r4b1^b4\
b4.\
a+1\
b2a+2\
<d+1^d+1\
a+4a+8a+8r2\
a+4a+8a+4a+4.\
f+4.f+2.\
f+4f+2^f+8\
d+4d+8d+8d+2\
d+4d+8d+4d+4.\
g+2.g+8a+1^a+8\
g+4g+8r4g+4g+4\
g+8g+8r4g+4.\
f+2..f+2.\
f+4.\
g+4g+8r4g+4g+2.\
g+8g+8g+4\
a+8a+8r4a+4a+4\
a+8a+8r4a+4a+4\
a+8a+8r4.a+8a+1^a+8\
";
#else
const char *test_mml2 = "T150V0";
const char *test_mml3 = "T150V0";
#endif

uint32_t env_vol_table[32];

static char __attribute__((aligned(16))) mmlstr[3][MML_SIZE];
static int mmllen[3];
SOUND_MML_T mmldata[3];
static uint8_t random_table[512];



/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */

void make_noise_table()
{
    int i;
    uint32_t n;
    SYS_RTCC_BCD_TIME nt;
    SYS_RTCC_TimeGet(&nt);
    SYS_RANDOM_PseudoSeedSet((uint32_t) nt);
    for (i = 0; i < (sizeof (random_table) / sizeof (uint8_t)); i++) {
        uint32_t radix = SYS_RANDOM_PseudoGet();
        n = ((radix >> 8) & 0xff) ^ ((radix >> 16) & 0xff);
        n = n ^ ((radix >> 24) & 0xff) ^ (radix & 0xff);
        //uint32_t n = (radix >> (i & 15)) & 0xff;
        random_table[i] = (uint8_t) n;
    }
}

static uint32_t render_null(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    uint32_t rp = 0;
    int16_t *p = data;

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
    if (data == NULL) return samples;
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
    if (regs->regs.noise_on) {
        regs->regs.noise_pos = regs->regs.noise_pos & 0x1ff;
        int32_t nl = (int32_t) random_table[regs->regs.noise_pos];
        nl = (nl * on_vol) >> 8;
        rp = regs->regs.rmod;
        freq = (uint32_t) regs->regs.noise_freq;
        for (i = 0; i < samples; i++) {
            if (rp >= sp) {
                uint32_t nf = rp / sp;
                uint32_t nm = rp - (nf * sp);
                onoff = !onoff;
                rp = (uint16_t)nm;
                regs->regs.noise_pos = regs->regs.noise_pos + (uint16_t)nf;
                regs->regs.noise_pos = regs->regs.noise_pos & 0x1ff;
                nl = (int32_t) random_table[regs->regs.noise_pos];
                nl = (nl * on_vol) >> 8;
            }
            p[i] += (int16_t)nl;
            rp += freq;
        }
    } else {
        for (i = 0; i < samples; i++) {
            if (rp >= sp) {
                rp -= sp;
                onoff = !onoff;
                pvol = (onoff) ? on_vol : off_vol;
            }
            p[i] += pvol;
            rp += freq;
        }
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

static uint32_t envelope_base(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    int64_t ui = (int64_t) samples;
    int algo = (int) (regs->regs.env_type);
    int64_t ncount;
    bool __is_continue = false;
    bool __is_attack = false;
    bool __is_alternate = false;
    bool __is_hold = false;
    bool __is_multiply = false;
    bool __is_sound_memory = false;
    bool __is_maxvolume = false;
    int16_t max_volume;
    int __bank_num = 0;
    if (algo >= 32) {
        __is_sound_memory = true;
        __bank_num = (algo >> 5) & 7;
    }
    if ((algo & 8) != 0) {
        __is_continue = true;
    }
    if ((algo & 4) != 0) {
        __is_attack = true;
    }
    if ((algo & 2) != 0) {
        __is_alternate = true;
    }
    if ((algo & 1) != 0) {
        __is_hold = true;
    }
    if (!(__is_continue)) {
        __is_alternate = false;
        __is_hold = false;
    } else {
        if (__is_hold) {
            __is_maxvolume = __is_alternate ^ __is_attack;
            __is_alternate = false;
            __is_continue = false;
        }
    }
    if ((algo & 16) != 0) {
        __is_multiply = true;
    }
    int16_t el = regs->regs.env_pos;
    if (regs->regs.envelope_pitch_val <= 0) return samples;
    if (!(__is_continue) && !(__is_alternate) && (el > 32)) {
        if (__is_maxvolume) {
            regs->regs.env_tmp_vol = env_calc_volume(32, max_volume);
            render_op(data, regs, samples);
        } else {
            regs->regs.env_tmp_vol = 0;
            //
        }
        return samples;
    }
    if (__is_multiply) {
        max_volume = regs->regs.vol;
    } else {
        max_volume = sound_max;
    }
    ncount = regs->regs.envelope_pitch_val - regs->regs.env_count;
    if (ncount < 0) ncount = ncount + regs->regs.envelope_pitch_val;
    while (ui > 0) {
        ncount = regs->regs.envelope_pitch_val - regs->regs.env_count;
        if (ncount >= SOUND_LENGTH) ncount = SOUND_LENGTH;
        if (ncount >= ui) ncount = ui;
        int16_t el = regs->regs.env_pos;
        if (__is_alternate) {
            if (el > 32) {
                el = 64 - el;
            }
        }
        if (!(__is_attack)) {
            el = 32 - el;
        }
        if (el >= 32) el = 32;
        regs->regs.env_tmp_vol = env_calc_volume(el, max_volume);
        render_op(data, regs, (int32_t) ncount);
        data += ncount;
        ui -= ncount;
        regs->regs.env_count = regs->regs.env_count + ncount;
        if (regs->regs.env_count >= regs->regs.envelope_pitch_val) {
            regs->regs.env_pos++;
            regs->regs.env_count = regs->regs.env_count - regs->regs.envelope_pitch_val;
        }
        if (!__is_alternate) {
            if (regs->regs.env_pos > 32) {
                if (!__is_continue) {
                    if (__is_maxvolume) {
                        regs->regs.env_tmp_vol = env_calc_volume(32, max_volume);
                        if (ui > 0) render_op(data, regs, ui);
                        ui = 0;
                    } else {
                        regs->regs.env_tmp_vol = 0;
                        ui = 0;
                        //
                    }
                    break;
                } else {
                    regs->regs.env_pos = 0;
                }
            }
        } else {
            if (regs->regs.env_pos >= 64) {
                if (!__is_continue) {
                    if (__is_maxvolume) {
                        regs->regs.env_tmp_vol = env_calc_volume(32, max_volume);
                        if (ui > 0) render_op(data, regs, ui);
                        ui = 0;
                    } else {
                        regs->regs.env_tmp_vol = 0;
                        ui = 0;
                        //
                    }
                    break;
                } else {
                    regs->regs.env_pos = 0;
                }
            }
        }
    }
    return samples;
}

static uint32_t render_sound(int16_t *data, SOUND_MML_T *regs, uint32_t samples)
{
    if (regs->regs.env_on) {
        return envelope_base(data, regs, samples);
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
        regs->regs.env_pos = 0;
        regs->regs.env_count = 0;
        regs->regs.env_mod = 0;
    } else if ((head_char >= 'a') && (head_char <= 'g')) {
        basefreq = note_std_o4[head_char - 'a'];
        need_next = true;
        b_note = true;
        note = head_char;
        regs->regs.env_pos = 0;
        regs->regs.env_count = 0;
        regs->regs.env_mod = 0;
    } else if ((head_char == 'R') || (head_char == 'r')) {
        need_next = true;
        b_note = false;
        note = 0;
        regs->regs.env_pos = 0;
        regs->regs.env_count = 0;
        regs->regs.env_mod = 0;
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
                int16_t el = longval << 1;
                regs->regs.vol = calc_volume(el, sound_max);
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
        } while (mmlptr < (uint32_t) mml_len);
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
        if (mmlptr < (uint32_t) mmllen) {
            if (*p == ',') { // Freq
                p++;
                mmlptr++;
                memset(nbuf, 0x00, sizeof (nbuf));
                nptr = 0;
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
                        const int64_t basefreq = ((int64_t) PSG_CLOCK) >> 8;
                        // Freq = basefreq / (256 * val) [Hz]
                        // Pitch_val = Freq / 48KHz 
                        int64_t nval = (int64_t) longval;
                        int64_t freq;
                        int64_t freq_mod;
                        int64_t pitch_val;
                        int64_t pitch_mod;
                        pitch_val = (nval * SAMPLE_FREQ) / basefreq;
                        pitch_mod = (nval * SAMPLE_FREQ) % basefreq;
                        pitch_val >>= 5;
                        pitch_mod >>= 5;
                        if (pitch_val == 0) {
                            regs->regs.env_on = false;
                        }
                        regs->regs.envelope_pitch_val = pitch_val;
                        regs->regs.envelope_pitch_mod = pitch_mod;
                        regs->regs.env_count = 0;
                        regs->regs.env_mod = 0;
                    }
                }
            }
        }
    } else if ((head_char == 'X') || (head_char == 'x')) { // NOISE
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
            if (nptr > 5) break;
        }
        // regs->regs.env_pos = 0;
        // regs->regs.env_count = 0;
        //  regs->regs.env_mod = 0;
        //  regs->regs.env_on = true;
        nlen = strlen(nbuf);
        longval = strtol(nbuf, NULL, 10);
        if (nlen > 0) { // Set type
            regs->regs.noise_freq = longval;
            if (regs->regs.noise_freq > 0) {
                regs->regs.noise_on = true;
                regs->regs.noise_pos = 0;
            } else {
                regs->regs.noise_on = false;
                regs->regs.noise_pos = 0;
            }
        } else {
            regs->regs.noise_on = false;
            regs->regs.noise_pos = 0;
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

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
int get_mml_len(int num)
{
    if(num < 0) return 0;
    if(num >= 3) return 0;
    return mmllen[num];
}
void init_env_vol_table()
{
    int i;
    uint32_t val = 65536;
    for (i = 31; i >= 0; i--) {
        env_vol_table[i] = val;
        val = val * env_rate;
        val = val >> 16;
    }
}

uint32_t render_mml(int16_t *head_data, SOUND_MML_T *regs, uint32_t max_samples)
{
    uint32_t left = max_samples;
    uint32_t p_samples = 0;
    size_t mmllen;
    uint32_t mmlpos;
    int i, j;
    char *p;
    int16_t *_sssp = get_sound_buffer();
    
    if (head_data == NULL) return 0;
    if (_sssp == NULL) return max_samples;
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

void init_mmls(char *mml1, char *mml2, char *mml3)
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
        mmldata[i].regs.vol = calc_volume(16, sound_max);
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
        memset(mmlstr[i], 0x00, MML_SIZE * sizeof (char));
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
            if (mmllen[i] > (MML_SIZE - 1)) mmllen[i] = MML_SIZE - 1;
            strncpy(mmlstr[i], pmml[i], MML_SIZE - 1);
        }
        mmldata[i].mmlbase = mmlstr[i];
    }
    taskEXIT_CRITICAL();
}

/* *****************************************************************************
 End of File
 */
