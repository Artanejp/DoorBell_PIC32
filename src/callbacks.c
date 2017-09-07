#include "doorbell.h"

extern DOORBELL_DATA doorbellData;

void ledTimerCallback(uintptr_t context, uint32_t currTick)
{
    DOORBELL_TIMER_TICK_T *p = (DOORBELL_TIMER_TICK_T *)context;
    bool b;
    p->toggle_status = !(p->toggle_status);
    b = p->toggle_status;
    switch(p->num) {
        case 0:
            LATBbits.LATB14 = b;
            break;
        case 1:  // LED_1
            LATBbits.LATB3 = b;
            break;
        default:
            break;
    }
}

