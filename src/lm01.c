
#include "doorbell.h"


#include "lm01_drv.h"

static void DRV_TEMP_LM01_SetPort(bool stat)
{
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_A, 0, stat);
}

void DRV_TEMP_LM01_Init(DRV_TEMP_LM01_T *p, void *update_port)
{
    DRV_TMR_INIT init;
    p->wait_ms = (50 + 54);
    p->temp1_Obj = NULL;
    p->temp1_Handle = NULL;
    if (update_port == NULL) {
        p->update_port_p = &DRV_TEMP_LM01_SetPort;
    } else {
        p->update_port_p = update_port;
    }
    // Initialize Counter (DRV_TMR_INDEX2)
    init.moduleInit.value = SYS_MODULE_POWER_IDLE_RUN;
    init.tmrId = TMR_ID_5;
    init.clockSource = DRV_TMR_CLKSOURCE_EXTERNAL_SYNCHRONOUS;
    init.prescale = TMR_PRESCALE_VALUE_1;
    init.interruptSource = INT_SOURCE_TIMER_2;
    init.mode = DRV_TMR_OPERATION_MODE_16_BIT;
    init.asyncWriteEnable = false;
    p->temp1_Obj = DRV_TMR_Initialize(DRV_TMR_INDEX_2, (SYS_MODULE_INIT*) & init);
}

bool DRV_TEMP_LM01_StartConversion(DRV_TEMP_LM01_T p)
{
    if (p.temp1_Obj == NULL) {
        return false;
    }
    if (p.temp1_Handle != NULL) return false;
    p.temp1_Handle = DRV_TMR_Open(DRV_TMR_INDEX_2, DRV_IO_INTENT_EXCLUSIVE);
    if (p.temp1_Handle != NULL) {
        DRV_TMR_CounterClear(p.temp1_Handle);
        p.update_port_p(true);
        p.delay_Handle = SYS_TMR_DelayMS(p.wait_ms);
        return true;
    }
    return false;
}

uint32_t DRV_TEMP_LM01_EndConversion(DRV_TEMP_LM01_T p)
{
    uint32_t answer;
    p.update_port_p(false);
    // Record time_temp1, date_temp1;
    answer = DRV_TMR_CounterValueGet(p.temp1_Handle);
    DRV_TMR_Close(p.temp1_Handle);
    p.temp1_Handle = NULL;
    //SYS_RANDOM_CryptoEntropyAdd(answer & 0xff);
    //SYS_RANDOM_CryptoEntropyAdd(answer >> 8);
    return answer;
}