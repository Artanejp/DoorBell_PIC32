#include "doorbell.h"

extern DOORBELL_DATA doorbellData;

void ledTimerCallback(uintptr_t context, uint32_t currTick)
{
    DOORBELL_TIMER_TICK_T *p = (DOORBELL_TIMER_TICK_T *) context;
    bool b;
    p->toggle_status = !(p->toggle_status);
    b = p->toggle_status;
    switch (p->num) {
    case 1:
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, b);
        break;
    default:
        break;
    }
}

void pollCallback(uintptr_t context, uint32_t currTick)
{
    /*
    if(!doorbellData.wrUsbComplete) {
        if (SYS_CONSOLE_Status(sysObj.sysConsole1) == SYS_STATUS_READY) {
            doorbellData.state = DOORBELL_STATE_USB_WRITE_TEST_1;
        }
    }
     }
     */
}
// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************

void DOORBELL_ReadUSBComplete(void *handle)
{
    doorbellData.rdUsbComplete = true;
}

void DOORBELL_ReadUARTComplete(void *handle)
{
    doorbellData.rdUartComplete = true;
}

void DOORBELL_WriteUSBComplete(void *handle)
{
    doorbellData.wrUsbComplete = true;
}

void DOORBELL_WriteUARTComplete(void *handle)
{
    doorbellData.wrUartComplete = true;
}
