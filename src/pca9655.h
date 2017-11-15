/* 
 * File:   pca9655.h
 * Description: Driver of PCA9655 I/O expander.
 *                 This is for MPLAB-Harmony with FreeRTOS.
 * Author: Kyuma Ohta <whatisthis.sowhat _at_ gmail.com>
 *
 * Created on 2017/11/12, 19:44
 * License: Apache
 */

#ifndef PCA9655_H
#define	PCA9655_H

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"

#ifdef	__cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t num;
    uint16_t devAddr;
    DRV_HANDLE devHandle;
    //I2C_MODULE_ID module_id;
    uint8_t port0_out_data; // Output
    uint8_t port1_out_data; // Output
    uint8_t port0_in_data;
    uint8_t port1_in_data;
    uint8_t direction_0;
    uint8_t direction_1;
    uint8_t polality_0;
    uint8_t polality_1;
    void (*open_port)(void);
    void (*close_port)(void);
} PCA9655_t;

typedef struct {
    uint8_t direction_0;
    uint8_t direction_1;
    uint8_t polality_0;
    uint8_t polality_1;
    uint8_t data_0;
    uint8_t data_1;
    void (*open_port)(void);
    void (*close_port)(void);
} PCA9655_INIT_t;

extern DRV_HANDLE sv_open_i2c(int num);

extern void DRV_PCA9655_sample_open_port(void);
extern void DRV_PCA9655_sample_close_port(void);
extern bool DRV_PCA9655_Init(int num, DRV_HANDLE handle, PCA9655_t *desc, uint16_t devaddr, PCA9655_INIT_t *init);
//extern bool DRV_PCA9655_Init(int num, I2C_MODULE_ID modid, PCA9655_t *desc, uint16_t devaddr, PCA9655_INIT_t *init);
extern uint8_t DRV_PCA9655_GetPort_Uint8(void *p, int port);
extern bool DRV_PCA9655_GetPort_Bit(void *p, uint32_t bit);
extern void DRV_PCA9655_SetPort(void *p, int bitnum, bool set);
extern void DRV_PCA9655_SetPort_Uint8(void *p, int port, uint8_t val);
extern void DRV_PCA9655_SetReg(PCA9655_t *desc, uint32_t regnum, uint8_t val);
extern uint8_t DRV_PCA9655_GetReg(PCA9655_t *desc, uint32_t regnum);
extern void DRV_PCA9655_GetRegs(PCA9655_t *desc, uint8_t *buf);

#ifdef	__cplusplus
}
#endif

#endif	/* PCA9655_H */
