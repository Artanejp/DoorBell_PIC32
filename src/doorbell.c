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

DOORBELL_DATA doorbellData;

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


// ToDo: Will Move
const uint16_t sound_level_table[32] = {
    0, 5, 10, 15, 20, 25, 30, 35,
    41, 47, 53, 60, 67, 74, 81, 88,
    95, 103, 111, 119, 127, 136, 145,
    154, 164, 174, 185, 196, 208, 220, 235
};
uint16_t sample_buffer[SOUND_LENGTH]; // 0.2sec


extern void ledTimerCallback(uintptr_t context, uint32_t currTick);
extern void pollCallback(uintptr_t context, uint32_t currTick);

void SLEEP_Periferals(bool onoff)
{
    int n = (onoff) ? 1 : 0; // OFF = TRUE
    // ToDo: Unlock PMDxbits.
    // A/D OFF
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

    // UART2 OFF
    PMD5bits.U2MD = n;
    // I2C-2 OFF
    PMD5bits.I2C2MD = n;
    // SPI OFF
    PMD5bits.SPI1MD = n;
    PMD5bits.SPI2MD = n;
    // PMP OFF
    PMD6bits.PMPMD = n;
    // ToDo: Lock PMDxbits.
}

void TWE_Wakeup(bool onoff)
{
    // Enter critical
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 3, onoff); // ON/OFF LED
    SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 2, onoff); // ON/OFF TWE
    // Leave critical
    // ON/OFF TWE Module
    vTaskDelay(cTick100ms / 2);
}
/******************************************************************************
  Function:
    void DOORBELL_Tasks ( void )

  Remarks:
    See prototype in doorbell.h.
 */

extern void prvHouseKeeping(void *pvParameters);

void DOORBELL_Tasks(void)
{
    prvHouseKeeping((void *) (&doorbellData));
}



/*******************************************************************************
 End of File
 */
