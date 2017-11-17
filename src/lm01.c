
#include "doorbell.h"


#include "lm01_drv.h"


static void DRV_TEMP_LM01_SetPort(void *p, uint32_t num, bool stat)
{
    //SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, 0, stat);  // For I2C Bus
}

void DRV_TEMP_LM01_Init(DRV_TEMP_LM01_T *p, uint32_t num, void *update_port, void *userptr)
{
    DRV_TMR_INIT init;
    p->wait_ms = (((50 + 54) * 100) / portTICK_PERIOD_MS) / 100;
    p->sensor_num = num;
    p->temp1_Handle = NULL;
    p->userptr = userptr;
    if (update_port == NULL) {
        p->update_port_p = &DRV_TEMP_LM01_SetPort;
    } else {
        p->update_port_p = update_port;
    }
}

bool DRV_TEMP_LM01_StartConversion(DRV_TEMP_LM01_T *p)
{
    if(p == NULL) return false;
    if (p->temp1_Handle != NULL) return false;
    p->temp1_Handle = DRV_TMR_Open(DRV_TMR_INDEX_2, DRV_IO_INTENT_EXCLUSIVE);
    if (p->temp1_Handle != NULL) {
        
        DRV_TMR_CounterClear(p->temp1_Handle);
        DRV_TMR_Start(p->temp1_Handle);
       p->update_port_p(p->userptr, p->sensor_num, true);
       vTaskDelay(p->wait_ms);
        return true;
    }
    return false;
}

uint32_t DRV_TEMP_LM01_EndConversion(DRV_TEMP_LM01_T *p)
{
    uint32_t answer;
    // Record time_temp1, date_temp1;
    p->update_port_p(p->userptr, p->sensor_num, false);
    DRV_TMR_Stop(p->temp1_Handle);
    answer = DRV_TMR_CounterValueGet(p->temp1_Handle);
    DRV_TMR_Close(p->temp1_Handle);
    p->temp1_Handle = NULL;
    return answer;
}

#include <string.h>
#include <stdio.h>
#include "logger.h"

void printThermalLMT01(int cons_index, int index, uint32_t temp)
{
    char shead[64];
    char str[16];
    uint8_t dbuf[10];
    
    int itemp;
    float ftemp;

    memset(dbuf, 0x00, sizeof(dbuf));
    ftemp = (float) ((temp & 0x1fff) << 8) - (50.0 * 4096.0);
    ftemp = ftemp / 4096.0;
    memcpy(dbuf, &ftemp, sizeof(float));
    
    memset(shead, 0x00, sizeof(shead));
    memset(str, 0x00, sizeof(str));
    snprintf(shead, sizeof (shead), "TEMP%02d", index);
    snprintf(str, sizeof (str), "%.3f", ftemp);
    printLog(cons_index, shead, str, LOG_TYPE_TEMP1 + index, dbuf, sizeof(float));
}
