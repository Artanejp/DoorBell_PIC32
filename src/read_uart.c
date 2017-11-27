/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    read_uart.c

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

#include "read_uart.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

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

READ_UART_DATA read_uartData;

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
    void READ_UART_Initialize ( void )

  Remarks:
    See prototype in read_uart.h.
 */
#define UART_RECV_BUFFER_SIZE 128 
RingBuffer_Char_t xUartRecvRing;
static char xUartRecvBuf[UART_RECV_BUFFER_SIZE];

void READ_UART_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    read_uartData.state = READ_UART_STATE_INIT;

    
    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
 xDevHandleUart_Recv = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_READ | DRV_IO_INTENT_BLOCKING);
 vRingBufferCreate_Char(&xUartRecvRing, xUartRecvBuf, UART_RECV_BUFFER_SIZE);

}


/******************************************************************************
  Function:
    void READ_UART_Tasks ( void )

  Remarks:
    See prototype in read_uart.h.
 */

static char rdTmpUartBuf[4];
static char rdStrBuf[128];

void READ_UART_Tasks(void)
{
    size_t _len = 0;
    BaseType_t stat;
    UBaseType_t avail;
    bool b_stat;
    int i = 0;
    int j = 0;
    char c;
    bool nsync = false;
    volatile uint32_t strtype = N_STR_BEGIN;
    int sptr = 0;
    int ssptr;
    bool brf;
    DRV_USART_BUFFER_HANDLE bhandle;
    size_t bsize;
    while (1) {
        brf = false;
        if (xDevHandleUart_Recv != DRV_HANDLE_INVALID) {
            //_len = DRV_USART_Read(xDevHandleUart_Recv, &(rdTmpUartBuf[0]), sizeof (char));
#if 0            
            if (!DRV_USART_TransmitBufferIsFull(xDevHandleUart_Recv)) {
                stat = xQueueReceive(xUartSendQueue, wrTmpUartBuf, cTick100ms / 2);
                if (stat == pdPASS) {
                    brf = true;
                    DRV_USART_WriteByte(xDevHandleUart_Recv, wrTmpUartBuf[0]);
                } else {
                    //vTaskDelay(cTick100ms);
                }
            }
#endif
            bsize = DRV_USART_Read(xDevHandleUart_Recv, (void *)rdTmpUartBuf, sizeof(char));
            if(bsize >= sizeof(char)) {
#if 1
                _len = 1;
                {
                    i = 0;
                    //while (i < _len) {
                    rdStrBuf[sptr] = rdTmpUartBuf[0];
                    sptr++;
                    if (sptr >= (sizeof (rdStrBuf) / sizeof (char))) {
                        sptr = 0;
                        strtype = N_STR_BEGIN;
                        memset(rdStrBuf, 0x00, sizeof (rdStrBuf));
                        continue;
                    }
                    strtype = checkStrType(rdTmpUartBuf[0], strtype);

                    //if ((strtype & N_STR_END) != 0) {
                    //    continue;
                    //}
                    //i++;
                    //}
                    if ((strtype & N_STR_END) != 0) {
                        ssptr = 0;
                        if ((strtype & N_STR_MES) != 0) {
                            if ((strtype & N_STR_OK) != 0) {
                                for (ssptr = 0; ssptr < sptr; ssptr++) {
                                    //b_stat = false;
                                    stat = !pdPASS;
                                    do {
                                        //stat = xQueueSend(xUartRecvQueue, &(rdStrBuf[ssptr]), 4);
                                        b_stat = vRingBufferWrite_Char(&xUartRecvRing, rdStrBuf[ssptr]);
                                        if (b_stat) break;
                                        vTaskDelay(cTick100ms / 4);
                                    } while (stat != pdPASS);
                                }
                                asm volatile("NOP");
                            }
                        }
                        sptr = 0;
                        strtype = N_STR_BEGIN;
                        memset(rdStrBuf, 0x00, sizeof (rdStrBuf));
                    }
                    brf = true;
                    //vTaskDelay(2);
                }
#else
                if(rdTmpUartBuf[0] != '\0') {
                    char n[2];
                    n[0] = rdTmpUartBuf[0];
                    n[1] = '\0';
                    DRV_USART_Write(xDevHandleUart_Recv, n, sizeof(char));
                do {
                    stat = xQueueSend(xUartRecvQueue, &(rdStrBuf[ssptr]), 0);
                    //b_stat = vRingBufferWrite_Char(&xUartRecvRing, rdStrBuf[ssptr]);
                    //if (b_stat) break;
                    vTaskDelay(cTick100ms);
                } while (stat != pdPASS);
                }
                brf = true;
#endif
            }
            if (!brf) vTaskDelay(cTick500ms);
        } else {
            vTaskDelay(cTick500ms);
        }
    }
}

 

/*******************************************************************************
 End of File
 */
