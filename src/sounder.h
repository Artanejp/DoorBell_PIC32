/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _SOUNDER_H    /* Guard against multiple inclusion */
#define _SOUNDER_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include <limits.h>
#include "system/common/sys_module.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */
static const int16_t sound_max = 104; // (625/2)=1.0Vol; OPs=3
static const int16_t sound_limit = 312;


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */
typedef struct {
    uint32_t freq; // Hz
    uint32_t envelope_pitch_val; // Tick. 1Tick = 16KHz.
    uint32_t envelope_pitch_mod; // Tick. 1Tick = 16KHz.
    uint32_t howlong;
    uint32_t noise_freq;
    uint16_t noise_pos;
    int16_t vol;
    int16_t env_tmp_vol;
    uint8_t env_pos;
    uint8_t env_type;
    int64_t env_count;
    int64_t env_mod;
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
     extern SOUND_MML_T mmldata[3];
     extern uint32_t env_vol_table[32];

    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */

#define TEST_FREQ 440
#define SAMPLE_FREQ SOUND_RATE
#define PSG_CLOCK 1228000

#define MML_SIZE 1920

//static int16_t *sample_buffer;

//static const uint32_t env_rate = 55109; //65536 / 1.189207115 : From fmgen0.8's psg.cpp .
static const uint32_t env_rate = 58254; // 65536 / 1.125

static inline int16_t calc_volume(int16_t vol, int16_t maxvol)
{
    int32_t n = (int32_t) vol;
    n = (n * maxvol) >> 5;
    return (int16_t) n;
}

static inline int16_t env_calc_volume(int16_t vol, int16_t maxvol)
{
#if 1
    uint32_t rvol;
    if (vol > 32) vol = 32;
    if (vol <= 0) {
        rvol = 0;
    } else {
        rvol = env_vol_table[vol - 1];
        rvol = (rvol * (uint32_t) maxvol) >> 16;
    }
    return (int16_t) rvol;
#else
    int32_t n = (int32_t) vol;
    n = (n * maxvol) >> 5;
    return (int16_t) n;
#endif
}


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

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
    extern void make_noise_table();
    extern int16_t *get_sound_buffer(void);
    extern int get_mml_len(int num);
    extern uint32_t render_mml(int16_t *head_data, SOUND_MML_T *regs, uint32_t max_samples);
    extern void init_env_vol_table();
    extern void init_mmls(char *mml1, char *mml2, char *mml3);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
