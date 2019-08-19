/*******************************************************************************
  MPLAB Harmony Project Main Source File

  Company:
    Microchip Technology Inc.
  
  File Name:
    main.c

  Summary:
    This file contains the "main" function for an MPLAB Harmony project.

  Description:
    This file contains the "main" function for an MPLAB Harmony project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state 
    machines of all MPLAB Harmony modules in the system and it calls the 
    "SYS_Tasks" function from within a system-wide "super" loop to maintain 
    their correct operation. These two functions are implemented in 
    configuration-specific files (usually "system_init.c" and "system_tasks.c")
    in a configuration-specific folder under the "src/system_config" folder 
    within this project's top-level folder.  An MPLAB Harmony project may have
    more than one configuration, each contained within it's own folder under
    the "system_config" folder.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

//Microchip licenses to you the right to use, modify, copy and distribute
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

#include <system_config.h>
#include <xc.h>

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"
#include "doorbell.h"   // SYS function prototypes
#include "ringbuffer.h"
#include "read_uart.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "pca9655.h"
/* Standard demo includes. */
//#include "partest.h"

/* Hardware specific includes. */
//#include "ConfigPerformance.h"


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define UART_RECV_BUFFER_SIZE 148 
//RingBuffer_Char_t xUartRecvRing;
//static char xUartRecvBuf[UART_RECV_BUFFER_SIZE];
QueueHandle_t xUartRecvQueue;

static DOORBELL_DATA doorbellData;

static char assert_str_buf[384];

void prvAssertCalledSub(char *file, unsigned long line)
{
    doorbellData.assert_called = true;
    memset(assert_str_buf, 0x00, sizeof(assert_str_buf));
    snprintf(assert_str_buf, 384 - 1, "assert called at %s Line %d\n", file, line);
    CALC_MD5Sum();
}

void prvAssertCalledSub2(char *message, unsigned long line)
{
    doorbellData.assert_called = true;
    memset(assert_str_buf, 0x00, sizeof(assert_str_buf));
    snprintf(assert_str_buf, 384 - 1, "assert at Line %d MESSAGE=%s\n", line, message);
    CALC_MD5Sum();
}

const char *prvGetAssertMessage(void)
{
    if(doorbellData.assert_called) {
        return (const char *)assert_str_buf;
    } else {
        return (const char *)NULL;
    }
}
void prvResetAssertStatus(void) 
{
    doorbellData.assert_called = false;
    memset(assert_str_buf, 0x00, sizeof(assert_str_buf));
    CALC_MD5Sum();
}

RESET_REASON prvSetupHardware(void)
{
    uint32_t _time, _date;
    RESET_REASON reset_reason;

    PLIB_RTCC_Enable(RTCC_ID_0);
    reset_reason = PLIB_RESET_ReasonGet(RESET_ID_0);
    _time = PLIB_RTCC_RTCTimeGet(RTCC_ID_0);
    _date = PLIB_RTCC_RTCDateGet(RTCC_ID_0);
    SYS_Initialize(NULL);

    switch (reset_reason) {
    case RESET_REASON_POWERON:
    case RESET_REASON_BROWNOUT:
        //case RESET_REASON_VBAT:
        break;
    default:
        if((_time <= 0x24000000) && (_date <= 0xa0000000) ) {
            PLIB_RTCC_RTCTimeSet(RTCC_ID_0, _time);
            PLIB_RTCC_RTCDateSet(RTCC_ID_0, _date);
        }
        break;
    }
    // Reset RTC value if POWERON, restore values if not.
    //PLIB_RESET_ReasonClear(RESET_ID_0, reset_reason);
 
    U1RXRbits.U1RXR = 0x03; // RPB13 = U1RX
    RPB15Rbits.RPB15R = 0x01; //RPB15 = U1TX
    return reset_reason;
}

DOORBELL_DATA *getDoorbellData(void)
{
    return &doorbellData;
}

void DOORBELL_Initialize(void)
{
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    uint8_t data_md5sum[MD5_DIGEST_SIZE];
    RESET_REASON reason;
    reason = SYS_RESET_ReasonGet();
    switch (reason) {
    case RESET_REASON_POWERON:
    case RESET_REASON_CONFIG_MISMATCH:
        memset(&doorbellData, 0x00, sizeof (doorbellData));
        break;
    default:
        // Check MD5SUM
        // If failed, clear.
        break;
    }
    if (!CHECK_MD5Sum()) {
        memset(&(doorbellData.realdata), 0x00, sizeof (doorbellData.realdata));
        // Re-Calc MD5.
        CALC_MD5Sum();
    }
    doorbellData.ringed = false;
    doorbellData.uart_ready = true;
    doorbellData.usb_ready = false;
    doorbellData.resetReason = reason;
    
#if 0
    if (!SYS_PORTS_PinRead(PORTS_ID_0, PORT_CHANNEL_B, 5)) {
        // IF S_MAINTAIN is LOW, PASSTHROUGH
        doorbellData.bootparam_passthrough = true;
    } else {
        doorbellData.bootparam_passthrough = false;
    }
#endif
    {
        // Resume all tasks and Wait for alarm waking.
        // ToDo: button pressed.
        //SYS_CLK_REFERENCE_SETUP sr;
        //sr.stopInIdle = false;
        //sr.suspendInSleep = false;
        OSCCONbits.SLPEN = 0;
        //SYS_CLK_ReferenceClockSetup(CLK_BUS_REFERENCE_1, &sr);
    }
    

    SYS_RTCC_Stop();
    RTCALRMbits.AMASK = 0b0110; // Once a day.
    SYS_RTCC_Start();
}

SYS_RTCC_ALARM_HANDLE *hAlarmTick;

TaskHandle_t xHandleReadFromUSB;
TaskHandle_t xHandleWriteToUSB;

QueueHandle_t xUartSendQueue;
QueueHandle_t xUsbRecvQueue;
QueueHandle_t xUsbSendQueue;
QueueHandle_t xUartSendCmdQueue;
QueueHandle_t xPortInterruptQueue;
QueueHandle_t xIdleSleepQueue;

extern QueueHandle_t xSoundCmdQueue;
extern QueueHandle_t xSoundQueue;
extern TimerHandle_t xSoundTimer;

extern void prvHouseKeeping(void *pvParameters);

uint32_t cTick100ms;
uint32_t cTick110ms;
uint32_t cTick200ms;
uint32_t cTick500ms;
uint32_t cTick1Sec;
uint32_t cTick5Sec;

extern void prvReadFromUsb(void *pvparameters);
extern void prvWriteToUsb(void *pvparameters);

extern void prvHouseKeeping(void *pvParameters);
extern void prvReadFromUart_HK(void *pvparameters);

void setupTicks(void)
{
    cTick100ms = (uint32_t) ((10000 / portTICK_PERIOD_MS) / 100);
    cTick110ms = (uint32_t) ((11000 / portTICK_PERIOD_MS) / 100);
    cTick200ms = (uint32_t) ((20000 / portTICK_PERIOD_MS) / 100);
    cTick500ms = (uint32_t) ((5000 / portTICK_PERIOD_MS) / 10);
    cTick1Sec = (uint32_t) ((1000 / portTICK_PERIOD_MS));
    cTick5Sec = (uint32_t) ((5000 / portTICK_PERIOD_MS));

}
#if 1
void vAssertCalled(const char * pcFile, unsigned long ulLine)
{
    volatile unsigned long ul = 0;
    volatile int i;
    volatile int bsize;
    (void) pcFile;
    (void) ulLine;

    taskENTER_CRITICAL();
    {
        /* Set ul to a non-zero value using the debugger to step out of this
           function. */
        prvAssertCalledSub(pcFile, ulLine);
        PLIB_RESET_SoftwareResetEnable(RESET_ID_0);
        SYS_RESET_SoftwareReset();
        while (ul == 0) {
            portNOP();
        }
    }
    taskEXIT_CRITICAL();
#ifdef __DEBUG
    while(1) {
    }
#else
    // Release
    SYS_RESET_SoftwareReset();
#endif
}
#endif
volatile uint32_t intreg_portb_val;
int main(void)
{
    TimerHandle_t xTimer = NULL;
    RESET_REASON reason = prvSetupHardware();
    bool passthrough;
    //IEC0bits.T1IE = 0;
    //IFS0bits.T1IF = 0;
    intreg_portb_val = 0xffffffff;
    xUsbRecvQueue = NULL;
    xUsbSendQueue = NULL;
    xSoundCmdQueue = xQueueCreate(8, sizeof (uint32_t));
    vQueueAddToRegistry(xSoundCmdQueue, "SoundCmd");    
    
    xUartSendQueue = xQueueCreate(256, sizeof(char));
    xUartSendCmdQueue = xQueueCreate(16, sizeof(uint8_t));
    xPortInterruptQueue = xQueueCreate(128, sizeof(uint32_t));
    xUartRecvQueue = xQueueCreate(UART_RECV_BUFFER_SIZE, sizeof(char));
    vQueueAddToRegistry(xUartSendQueue, "SendToTWE");    
    vQueueAddToRegistry(xUartRecvQueue, "RecvFromTWE");    
    vQueueAddToRegistry(xPortInterruptQueue, "InterruptQueue");    
    vQueueAddToRegistry(xUartSendCmdQueue, "SendCmd4TWE");    

    xIdleSleepQueue = xQueueCreate(5, sizeof(bool));
    vQueueAddToRegistry(xIdleSleepQueue, "IdleTaskCmd");
    
    
    if (!passthrough) {
        // FULL
        setupTicks();
        //vRingBufferCreate_Char(&xUartSendRing, xUartSendBuf, UART_SEND_BUFFER_SIZE);
        //if (xDevHandleUart_Recv != DRV_HANDLE_INVALID) {
    } else {
        setupTicks();
        xUsbRecvQueue = xQueueCreate(128, sizeof (char));
        xUsbSendQueue = xQueueCreate(256, sizeof (char));
        xTaskCreate(prvReadFromUsb, "ReadFromUsb", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 1, &xHandleReadFromUSB);
        xTaskCreate(prvWriteToUsb, "WriteToUsb", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 2, &xHandleWriteToUSB);
        //xTaskCreate( prvLEDs, "LEDs", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY + 0, &xHandleLED );
    }
    /* A software timer is also used to start the high frequency timer test.
    This is to ensure the test does not start before the kernel.  This time a
    one shot software timer is used. */
    //xTimer = xTimerCreate( "HighHzTimerSetup", 1, pdFALSE, ( void * ) 0, prvSetupHighFrequencyTimerTest );
    //if( xTimer != NULL )
    //{
    //	xTimerStart( xTimer, mainDONT_BLOCK );
    //}
    /* Finally start the scheduler. */
    //vTaskStartScheduler();
    setupTicks();

    /* If all is well, the scheduler will now be running, and the following line
    will never be reached.  If the following line does execute, then there was
    insufficient FreeRTOS heap memory available for the idle and/or timer tasks
    to be created.  See the memory management section on the FreeRTOS web site
    for more details. */
    SYS_Tasks();

    //for( ;; );
    return ( EXIT_FAILURE);
}


/*******************************************************************************
 End of File
 */
