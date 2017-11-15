/* 
 * File:   pca9655.c
 * Description: Driver of PCA9655 I/O expander.
 *                 This is for MPLAB-Harmony with FreeRTOS.
 * Author: Kyuma Ohta <whatisthis.sowhat _at_ gmail.com>
 *
 * Created on 2017/11/12, 19:44
 * License: Apache
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
#include "semphr.h"

#include "pca9655.h"

#if 1

DRV_HANDLE sv_open_i2c(int num)
{
    int n = -1;
    switch (num) {
    case 0:
        n = DRV_I2C_INDEX_0;
        break;
    case 1:
        n = DRV_I2C_INDEX_1;
        break;
    case 2:
        n = DRV_I2C_INDEX_2;
        break;
    case 3:
        n = DRV_I2C_INDEX_3;
        break;
    case 4:
        n = DRV_I2C_INDEX_4;
        break;
    case 5:
        n = DRV_I2C_INDEX_5;
        break;
    default:
        return DRV_HANDLE_INVALID;
        break;
    }
    return DRV_I2C_Open(n, DRV_IO_INTENT_READWRITE);
}
#endif

void DRV_PCA9655_sample_open_port(void)
{
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, 0, false); //I2C Bus ON
}

void DRV_PCA9655_sample_close_port(void)
{
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, 0, true); //I2C Bus ON
}

bool DRV_PCA9655_Reset(PCA9655_t *desc, uint8_t port0_val, uint8_t port1_val, bool b_reset)
{
    DRV_I2C_BUFFER_HANDLE bHandle;
    uint8_t sbuffer[4];
    if (b_reset) {
        taskENTER_CRITICAL();
        desc->port0_out_data = port0_val;
        desc->port1_out_data = port1_val;
        taskEXIT_CRITICAL();
    }

    if (desc->open_port != NULL) desc->open_port();
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        sbuffer[0] = 0x06; // Config
        taskENTER_CRITICAL();
        sbuffer[1] = desc->direction_0;
        sbuffer[2] = desc->direction_1;
        taskEXIT_CRITICAL();
        bHandle = DRV_I2C_Transmit(desc->devHandle, desc->devAddr, (void *) sbuffer, 3, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(2);
        }
        sbuffer[0] = 0x04; // Porality
        taskENTER_CRITICAL();
        sbuffer[1] = desc->polality_0;
        sbuffer[2] = desc->polality_1;
        taskEXIT_CRITICAL();
        bHandle = DRV_I2C_Transmit(desc->devHandle, desc->devAddr, (void *) sbuffer, 3, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(2);
        }
        sbuffer[0] = 0x02; // Output
        taskENTER_CRITICAL();
        sbuffer[1] = desc->port0_out_data;
        sbuffer[2] = desc->port1_out_data;
        taskEXIT_CRITICAL();
        bHandle = DRV_I2C_Transmit(desc->devHandle, desc->devAddr, (void *) sbuffer, 3, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(2);
        }
    }
    if (desc->close_port != NULL) desc->close_port();
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        return true;
    }
    return false;
}

bool DRV_PCA9655_Init(int num, DRV_HANDLE handle, PCA9655_t *desc, uint16_t devaddr, PCA9655_INIT_t *init)
{
    if (desc == NULL) return false;
    desc->devAddr = devaddr & 0xfffe;
    desc->num = num;
    desc->devHandle = handle;
    if (init == NULL) {
        desc->port0_in_data = 0;
        desc->port1_in_data = 0;
        desc->port0_out_data = 0xff;
        desc->port1_out_data = 0xff;
        desc->direction_0 = 0xff;
        desc->direction_1 = 0xff;
        desc->polality_0 = 0x00;
        desc->polality_1 = 0x00;
        desc->open_port = NULL;
        desc->close_port = NULL;
    } else {
        desc->port0_out_data = init->data_0;
        desc->port1_out_data = init->data_1;
        desc->direction_0 = init->direction_0;
        desc->direction_1 = init->direction_1;
        desc->polality_0 = init->polality_0;
        desc->polality_1 = init->polality_1;
        desc->open_port = init->open_port;
        desc->close_port = init->close_port;
    }
    return DRV_PCA9655_Reset(desc, 0, 0, false);
}

uint8_t DRV_PCA9655_GetPort_Uint8(void *p, int port)
{
    uint8_t sbuf[2];
    uint8_t rbuf[2];
    PCA9655_t *desc = (PCA9655_t *) p;
    DRV_I2C_BUFFER_HANDLE bHandle;
    if (desc == NULL) return 0x00;
    if ((port < 0) || (port > 1)) return 0x00;
    rbuf[0] = 0x00;
    rbuf[1] = 0x00;

    if (desc->open_port != NULL) desc->open_port();
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        sbuf[0] = 0x00; // Output
        bHandle = DRV_I2C_TransmitThenReceive(desc->devHandle, desc->devAddr, sbuf, 1, rbuf, 2, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(2);
        }
    }
    taskENTER_CRITICAL();
    desc->port0_in_data = rbuf[0];
    desc->port1_in_data = rbuf[1];
    taskEXIT_CRITICAL();
    if (desc->close_port != NULL) desc->close_port();
    return rbuf[port];
}

bool DRV_PCA9655_GetPort_Bit(void *p, uint32_t bit)
{
    int port;
    uint8_t dat;
    PCA9655_t *desc = (PCA9655_t *) p;

    if (bit >= 16) return false;
    port = (int) (bit & 8);
    dat = DRV_PCA9655_GetPort_Uint8(desc, port);
    dat = (dat >> (bit & 0x07)) & 1;
    if (dat == 0) return false;
    return true;
}

void DRV_PCA9655_SetPort(void *p, int bitnum, bool set)
{
    uint8_t sbuf[3];
    DRV_I2C_BUFFER_HANDLE bHandle;
    PCA9655_t *desc = (PCA9655_t *) p;
    if ((bitnum >= 16) || (bitnum < 0)) return;
    if (desc == NULL) return;
    if (desc->open_port != NULL) desc->open_port();
    if (set) {
        taskENTER_CRITICAL();
        if (bitnum < 8) {
            desc->port0_out_data |= (1 << bitnum);
        } else {
            desc->port1_out_data |= (1 << (bitnum - 8));
        }
        taskEXIT_CRITICAL();
    } else {
        taskENTER_CRITICAL();
        if (bitnum < 8) {
            desc->port0_out_data &= (uint8_t) (~(1 << bitnum));
        } else {
            desc->port1_out_data &= (uint8_t) (~(1 << (bitnum - 8)));
        }
        taskEXIT_CRITICAL();
    }
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        sbuf[0] = 0x02; // Output
        taskENTER_CRITICAL();
        sbuf[1] = desc->port0_out_data;
        sbuf[2] = desc->port1_out_data;
        taskEXIT_CRITICAL();
        bHandle = DRV_I2C_Transmit(desc->devHandle, desc->devAddr, (void *) sbuf, 3, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(3);
        }
    }
    if (desc->close_port != NULL) desc->close_port();
}

void DRV_PCA9655_SetPort_Uint8(void *p, int port, uint8_t val)
{
    uint8_t sbuf[3];
    DRV_I2C_BUFFER_HANDLE bHandle;
    PCA9655_t *desc = (PCA9655_t *) p;
    if ((port > 1) || (port < 0)) return;
    if (desc == NULL) return;
    if (desc->open_port != NULL) desc->open_port();
    taskENTER_CRITICAL();
    if (port == 0) {
        desc->port0_out_data = val;
        sbuf[1] = val;
        sbuf[2] = desc->port1_out_data;
    } else {
        desc->port1_out_data = val;
        sbuf[1] = desc->port0_out_data;
        sbuf[2] = val;
    }
    taskEXIT_CRITICAL();
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        sbuf[0] = 0x02; // Output
        bHandle = DRV_I2C_Transmit(desc->devHandle, desc->devAddr, (void *) sbuf, 3, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(2);
        }
    }
    if (desc->close_port != NULL) desc->close_port();
}

void DRV_PCA9655_SetReg(PCA9655_t *desc, uint32_t regnum, uint8_t val)
{
    uint8_t sbuf[2];
    DRV_I2C_BUFFER_HANDLE bHandle;
    if (desc == NULL) return;
    if (desc->open_port != NULL) desc->open_port();
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        sbuf[0] = regnum & 7; // Output
        sbuf[1] = val;
        bHandle = DRV_I2C_Transmit(desc->devHandle, desc->devAddr, (void *) sbuf, 2, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(2);
        }
    }
    if (desc->close_port != NULL) desc->close_port();
}

uint8_t DRV_PCA9655_GetReg(PCA9655_t *desc, uint32_t regnum)
{
    uint8_t sbuf[2];
    uint8_t rbuf[4];
    DRV_I2C_BUFFER_HANDLE bHandle;
    if (desc == NULL) return 0x00;
    rbuf[0] = 0x00;
    rbuf[1] = 0x00;
    rbuf[2] = 0x00;
    rbuf[3] = 0x00;
    if (desc->open_port != NULL) desc->open_port();
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        sbuf[0] = regnum & 7; // Output
        bHandle = DRV_I2C_TransmitThenReceive(desc->devHandle, desc->devAddr, sbuf, 1, rbuf, 1, NULL);
        while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
            vTaskDelay(2);
        }
    }
    if (desc->close_port != NULL) desc->close_port();
    return rbuf[0];
}

void DRV_PCA9655_GetRegs(PCA9655_t *desc, uint8_t *buf)
{
    uint8_t sbuf[2];
    uint8_t regnum;
    DRV_I2C_BUFFER_HANDLE bHandle;
    if(buf == NULL) return;
    if (desc == NULL) return;
    if (desc->open_port != NULL) desc->open_port();
    if (desc->devHandle != DRV_HANDLE_INVALID) {
        regnum = 0;
        while (regnum < 8) {
            sbuf[0] = regnum & 7; // Output
            bHandle = DRV_I2C_TransmitThenReceive(desc->devHandle, desc->devAddr, sbuf, 1, &(buf[regnum]), 1, NULL);
            while ((DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_COMPLETE) &&
                    (DRV_I2C_TransferStatusGet(desc->devHandle, bHandle) != DRV_I2C_BUFFER_EVENT_ERROR)) {
                vTaskDelay(2);
            }
            regnum++;
        }

    }
    if (desc->close_port != NULL) desc->close_port();
    return;
}
