/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    write_uart.c

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

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "system/common/sys_module.h"
#include "doorbell.h"   // SYS function prototypes
#include "ringbuffer.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "write_uart.h"

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

WRITE_UART_DATA write_uartData;

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
    void WRITE_UART_Initialize ( void )

  Remarks:
    See prototype in write_uart.h.
 */

extern QueueHandle_t xUartSendCmdQueue;
extern QueueHandle_t xUartSendQueue;

DRV_HANDLE xDevHandleUart_Send;
extern void TWE_Wakeup(bool onoff);

void WRITE_UART_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    write_uartData.state = WRITE_UART_STATE_INIT;


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
    xDevHandleUart_Send = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_WRITE | DRV_IO_INTENT_BLOCKING);
    //xDevHandleUart_Send = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_WRITE);
}


/******************************************************************************
  Function:
    void WRITE_UART_Tasks ( void )

  Remarks:
    See prototype in write_uart.h.
 */


bool uart_wakeup;

static inline void do_send_prompt(void)
{
    BaseType_t bsize = 0;
    static const char sbuf[4] = "\n";
    while (bsize != sizeof (char)) {
        bsize = DRV_USART_Write(xDevHandleUart_Send, &(sbuf[0]), sizeof (char) * 1);
        if (bsize != sizeof (char)) vTaskDelay(cTick100ms / 4);
    }
}

static inline int check_idle(bool req_wakeup, bool req_idle, int idle_counter)
{
    bool tmpb;
    // Check Idle.
    vTaskDelay(cTick200ms);
    taskENTER_CRITICAL();
    tmpb = uart_wakeup;
    taskEXIT_CRITICAL();
    if ((tmpb) || (!req_wakeup)) {
        idle_counter++;
        if ((idle_counter > 15) && (req_idle)) {
            TWE_Wakeup(false);
            taskENTER_CRITICAL();
            uart_wakeup = false;
            taskEXIT_CRITICAL();
        }
        if (idle_counter > 15) idle_counter = 15;
    }
    return idle_counter;
}

static inline bool check_sleep(bool *req_sleep)
{
    if (*req_sleep) {
        //if (uxQueueSpacesAvailable(xUartSendQueue) == 0) {
        TWE_Wakeup(false);
        taskENTER_CRITICAL();
        uart_wakeup = false;
        taskEXIT_CRITICAL();
        *req_sleep = false;
        return true;
        //}
    }
    return false;
}


static char in_strbuf[76];
static bool req_idle;
static bool req_wakeup;
static bool req_sleep;
static bool check_prompt;

static bool check_command(void)
{
    BaseType_t stat;
    bool bstat = true;
    char cbuf[4];
    stat = xQueueReceive(xUartSendCmdQueue, cbuf, 4);
    if (stat == pdPASS) {
        // Update command
        /* if (cbuf[0] != cmd) */
        {
            switch (cbuf[0]) {
            case C_TWE_ON:
                taskENTER_CRITICAL();
                uart_wakeup = true;
                taskEXIT_CRITICAL();
                TWE_Wakeup(true);
                req_wakeup = true;
                //block_data = true;
                break;
            case C_TWE_FULL_RUN:
                taskENTER_CRITICAL();
                uart_wakeup = true;
                taskEXIT_CRITICAL();
                TWE_Wakeup(true);
                req_sleep = false;
                req_wakeup = true;
                req_idle = false;
                break;
            case C_TWE_IDLE_ON:
                req_idle = true;
                if(!uart_wakeup) {
                    taskENTER_CRITICAL();
                    uart_wakeup = true;
                    taskEXIT_CRITICAL();
                    TWE_Wakeup(true);
                }
                break;
            case C_TWE_IDLE_OFF:
                req_idle = false;
                if(!uart_wakeup) {
                    taskENTER_CRITICAL();
                    uart_wakeup = true;
                    taskEXIT_CRITICAL();
                    TWE_Wakeup(true);
                }
                break;
            case C_TWE_FORCE_WAKEUP:
                taskENTER_CRITICAL();
                uart_wakeup = true;
                taskEXIT_CRITICAL();
                req_wakeup = true;
                req_sleep = false;
                //block_data = false;
                TWE_Wakeup(true);
                break;
            case C_TWE_FORCE_SLEEP:
                TWE_Wakeup(false);
                req_wakeup = false;
                req_sleep = false;
                taskENTER_CRITICAL();
                uart_wakeup = false;
                taskEXIT_CRITICAL();
                
                break;
            case C_TWE_WRITE_OK: // IN PROMPT
                //block_data = false;
                bstat = false;
                break;
            case C_TWE_WRITE_WAIT:
                //block_data = true;
                bstat = false;
                break;
            default:
                bstat = false;
                break;
            }
        }
    } else {
        bstat = false;
    }
    return bstat;
}

void WRITE_UART_Tasks(void)
{

    BaseType_t stat;
    ssize_t bsize;
    int idle_counter = 0;
    char cbuf[4] = {0, 0, 0, 0};
    bool tmpb;
    int i;
    int nlen;
    char sbuf[2];
    //bool block_data = false;
    int ccount = 0;
    req_idle = true;
    req_wakeup = true;
    req_sleep = false;
    check_prompt = false;
    taskENTER_CRITICAL();
    uart_wakeup = false;
    taskEXIT_CRITICAL();
    TWE_Wakeup(false);
    ccount = 0;
    memset(in_strbuf, 0x00, sizeof (in_strbuf));
    while (1) {
        //vTaskDelay(cTick1Sec);
        //continue;
        if (xDevHandleUart_Send != DRV_HANDLE_INVALID) {
            check_command();
            ccount = 0;
            idle_counter = 0;
            do {
                stat = xQueueReceive(xUartSendQueue, &(in_strbuf[ccount]), cTick100ms);
                SYS_WDT_TimerClear();
                //check_command();
                if (stat != pdPASS) {
                    if(idle_counter < 8) { 
                        idle_counter++;
                        continue;
                    }
                    check_command();
                    if(req_idle) {
                       taskENTER_CRITICAL();
                       tmpb = uart_wakeup;
                        uart_wakeup = false;
                        taskEXIT_CRITICAL();
                        if(tmpb) TWE_Wakeup(false);
                        idle_counter = 0;
                        continue;
                    } else {
                        check_command();
                        idle_counter = 0;
                        continue;
                    }
                }
                idle_counter = 0;
                taskENTER_CRITICAL();
                tmpb = uart_wakeup;
                taskEXIT_CRITICAL();
                if (!tmpb) {
                    taskENTER_CRITICAL();
                    uart_wakeup = true;
                    taskEXIT_CRITICAL();
                    TWE_Wakeup(true);
                }
                if (in_strbuf[ccount] == '\0') {
                    //in_strbuf[ccount] = '\r';
                    in_strbuf[ccount + 0] = '\n';
                    ccount += 1;
                    break;
                } else if (in_strbuf[ccount] == '\n') {
                    //in_strbuf[ccount + 0] = '\r';
                    in_strbuf[ccount + 0] = '\n';
                    ccount += 1;
                    break;
                }
                ccount++;
                if (ccount >= ((sizeof (in_strbuf) / sizeof (char)) - 2)) {
                    in_strbuf[ccount - 1] = '\n'; // OK?
                    in_strbuf[ccount] = '\0';
                    break;
                }
            } while (1);

            bsize = 0;
            check_command();
#if 1
            while (bsize != (ccount * sizeof (char))) {
                SYS_WDT_TimerClear();
                bsize = DRV_USART_Write(xDevHandleUart_Send, in_strbuf, ccount * sizeof (char));
            }
            vTaskDelay(cTick200ms);
#if 0
           bsize = 0;
           sbuf[0] = ' ';
           sbuf[1] = '\n';
           while (bsize != (sizeof (char) * 2)) {
                bsize = DRV_USART_Write(xDevHandleUart_Send, sbuf, sizeof (char) * 2);
           }
#endif
#else
            i = 0;
            while(ccount > 0) {
                //bsize = 0;
                //while(bsize != (nlen * sizeof(char))) {
                while(DRV_USART_TransmitBufferIsFull(xDevHandleUart_Send)) { vTaskDelay(cTick100ms); }
                DRV_USART_WriteByte(xDevHandleUart_Send, in_strbuf[i]);
                //}
                i++;
                ccount--;
            }
#endif
            //vTaskDelay(cTick200ms * (2 + ccount / 4));
            
            check_command();
            if (req_idle) {
                stat = xQueuePeek(xUartSendQueue, cbuf, cTick100ms);
                if (stat != pdPASS) {
                    taskENTER_CRITICAL();
                    uart_wakeup = false;
                    taskEXIT_CRITICAL();
                    TWE_Wakeup(false);
                }
            } else {
                vTaskDelay(cTick100ms);
            }
            memset(in_strbuf, 0x00, sizeof (in_strbuf));
        } else {
            xQueueReset(xUartSendQueue);
            vTaskDelay(cTick1Sec);
        }
    }
}



/*******************************************************************************
 End of File
 */
