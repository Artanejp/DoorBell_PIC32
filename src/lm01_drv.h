/* 
 * File:   lm01_drv.h
 * Author: whatisthis
 *
 * Created on 2017/09/10, 2:57
 */

#ifndef LM01_DRV_H
#define	LM01_DRV_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "crypto/crypto.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t wait_ms;
    uint32_t sensor_num;
    DRV_HANDLE temp1_Handle;
    //SYS_TMR_HANDLE delay_Handle;
    void (*update_port_p)(uint32_t, bool); // Num, onoff
} DRV_TEMP_LM01_T;

extern void DRV_TEMP_LM01_Init(DRV_TEMP_LM01_T *p , uint32_t num, void *update_port);
extern bool DRV_TEMP_LM01_StartConversion(DRV_TEMP_LM01_T *p);
extern uint32_t DRV_TEMP_LM01_EndConversion(DRV_TEMP_LM01_T *p);
extern void printThermalLMT01(int cons_index, int index, uint32_t temp);

#ifdef	__cplusplus
}
#endif

#endif	/* LM01_DRV_H */

