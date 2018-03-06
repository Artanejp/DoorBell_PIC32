/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    doorbell.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "lm01_drv.h"
#include "doorbell.h"
#include "system_definitions.h"
#include "system_config.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
 */

//DOORBELL_DATA doorbellData;

/* Fulfill USB DMA transfer criteria */

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void DOORBELL_Initialize ( void )

  Remarks:
    See prototype in doorbell.h.
 */


extern void ledTimerCallback(uintptr_t context, uint32_t currTick);
extern void pollCallback(uintptr_t context, uint32_t currTick);

void SLEEP_Periferals(bool onoff)
{
    int n = (onoff) ? 1 : 0; // OFF = TRUE
    // ToDo: Unlock PMDxbits.
    // A/D OFF
    SYS_DEVCON_SystemUnlock();
    PMD1bits.AD1MD = n;
    PMD1bits.CTMUMD = n;

    // Internal Comparator off
    PMD2bits.CMP1MD = n;
    PMD2bits.CMP2MD = n;
    PMD2bits.CMP3MD = n;

    // Input Capture OFF
    PMD3bits.IC1MD = n;
    PMD3bits.IC2MD = n;
    PMD3bits.IC3MD = n;
    PMD3bits.IC4MD = n;
    PMD3bits.IC5MD = n;

    // Output Compare 1, 3-5 OFF
    PMD3bits.OC1MD = n;
    //PMD3bits.IC2MD = n;
    PMD3bits.OC3MD = n;
    PMD3bits.OC4MD = n;
    PMD3bits.OC5MD = n;

    // Timer4 OFF
    PMD4bits.T4MD = n;

    PMD5bits.U2MD = n;
    // I2C-2 OFF
    PMD5bits.I2C2MD = n;
    // SPI OFF
    PMD5bits.SPI1MD = n;
    PMD5bits.SPI2MD = n;
    // PMP OFF
    PMD6bits.PMPMD = n;
    // ToDo: Lock PMDxbits.
    SYS_DEVCON_SystemLock();
}

void Sleep_OSC(void)
{

    SYS_DEVCON_SystemUnlock();
    //OSCCONbits.COSC = 0b011; // PRIPLL
    //OSCCONbits.NOSC = 0b100; // SOSC.
    //OSCCONbits.OSWEN = 1;
    OSCCONbits.SLPEN = 1;
    SYS_DEVCON_SystemLock();
    //T1CONbits.ON = 0;
}

void Wakeup_OSC(void)
{
    int count = 0;
    volatile uint32_t dummy;
    do {
        SYS_DEVCON_SystemUnlock();
        //OSCCONbits.COSC = 0b100; // SOSC
        //OSCCONbits.NOSC = 0b011; // PRIPLL.
        //OSCCONbits.NOSC = 0b111; // FRCPLL.
        //OSCCONbits.OSWEN = 1;
        OSCCONbits.SLPEN = 0;
        //while (OSCCONbits.SLOCK == 0) {
        //}
        SYS_DEVCON_SystemLock();
        count++;
        if (count > 8) { // If Clock fail sometimes, force RESET.
            SYS_DEVCON_SystemUnlock();
            RSWRSTbits.SWRST = 1;
            dummy = RSWRST;
            while (1);
        }
    } while (OSCCONbits.CF != 0);
    //T1CONbits.ON = 1;
}

void Force_Reset(void)
{
    SYS_DEVCON_SystemUnlock();
    RSWRSTbits.SWRST = 1;
    volatile uint32_t dummy = RSWRST;
    while (1);
}

void Sleep_Periferals2(void)
{
    SYS_DEVCON_SystemUnlock();
    PMD1bits.AD1MD = 1;
    PMD1bits.CTMUMD = 1;
    //PMD1bits.CVRMD = 1;
    PMD2bits.CMP1MD = 1;
    PMD2bits.CMP2MD = 1;
    PMD2bits.CMP3MD = 1;

    PMD3bits.IC1MD = 1;
    PMD3bits.IC2MD = 1;
    PMD3bits.IC3MD = 1;
    PMD3bits.IC4MD = 1;
    PMD3bits.IC5MD = 1;

    PMD3bits.OC1MD = 1;
    //PMD3bits.OC2MD = 1;
    PMD3bits.OC3MD = 1;
    PMD3bits.OC4MD = 1;
    PMD3bits.OC5MD = 1;
    //PMD4bits.T1MD = 1;
    //PMD4bits.T2MD = 1;
    PMD4bits.T3MD = 1;
    //PMD4bits.T4MD = 1;
    PMD4bits.T5MD = 1;
    //PMD5bits.U1MD = 1;
    PMD5bits.I2C2MD = 1;
    PMD5bits.SPI1MD = 1;
    PMD5bits.SPI2MD = 1;
    //PMD5bits.USB1MD = 1;
    //PMD5bits.USBMD = 1;
    PMD6bits.PMPMD = 1;
    SYS_DEVCON_SystemLock();

}

void Wakeup_Periferals2(void)
{
    SYS_DEVCON_SystemUnlock();
    OSCCONbits.SLPEN = 0;

    PMD4bits.T1MD = 0;
    PMD4bits.T2MD = 0;
    PMD4bits.T3MD = 0;
    PMD4bits.T4MD = 0;
    PMD4bits.T5MD = 0;
    PMD3bits.OC2MD = 0;
    PMD5bits.I2C1MD = 0;
    PMD5bits.USB1MD = 0;
    PMD5bits.USBMD = 0; // UART2 OFF
    SYS_DEVCON_SystemLock();
}

void Enter_Sleep(void)
{
    SYS_RTCC_AlarmEnable();
    SYS_WDT_TimerClear();
    SYS_WDT_Enable(false);
}

void TWE_Wakeup(bool onoff)
{
    // Enter critical
    if (!onoff) vTaskDelay(cTick200ms / 4);

    taskENTER_CRITICAL();
    WAKEUPStateSet((onoff) ? 1 : 0);
    taskEXIT_CRITICAL();

    // Leave critical
    // ON/OFF TWE Module
    if (onoff) vTaskDelay(cTick100ms / 2);
}
/******************************************************************************
  Function:
    void DOORBELL_Tasks ( void )

  Remarks:
    See prototype in doorbell.h.
 */

extern void prvHouseKeeping(void *pvParameters);
extern DOORBELL_DATA *getDoorbellData();

void DOORBELL_Tasks(void)
{
    DOORBELL_DATA *pdd = getDoorbellData();
    prvHouseKeeping((void *) pdd);
}



/*******************************************************************************
 End of File
 */
