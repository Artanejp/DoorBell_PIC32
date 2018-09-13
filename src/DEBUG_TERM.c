/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    debug_term.c

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

#include "debug_term.h"

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

DEBUG_TERM_DATA debug_termData;

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
    void DEBUG_TERM_Initialize ( void )

  Remarks:
    See prototype in debug_term.h.
 */

void DEBUG_TERM_Initialize(void)
{
    /* Place the App state machine in its initial state. */
    debug_termData.state = DEBUG_TERM_STATE_INIT;


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void DEBUG_TERM_Tasks ( void )

  Remarks:
    See prototype in debug_term.h.
 */

extern QueueHandle_t xUartSendQueue;
extern QueueHandle_t xUartSendCmdQueue;
extern QueueHandle_t xUartRecvQueue;

static char pStrTermReceiveBuf[256];
static char pStrTermSendBuf[256];

bool in_atoh(char *p, uint32_t *ansp, uint32_t *sum)
{
    uint32_t ans = 0;
    if (ansp == NULL) return false;
    if ((*p >= '0') && (*p <= '9')) {
        ans = *p - '0';
    } else if ((*p >= 'A') && (*p <= 'F')) {
        ans = *p - 'A' + 10;
    } else if ((*p == '\n') || (*p == '\r') || (*p == '\0')) {
        return false;
    }
    p++;
    ans <<= 8;
    if ((*p >= '0') && (*p <= '9')) {
        ans += (*p - '0');
    } else if ((*p >= 'A') && (*p <= 'F')) {
        ans += (*p - 'A' + 10);
    } else if ((*p == '\n') || (*p == '\r') || (*p == '\0')) {
        return false;
    }
    if (sum != NULL) *sum += ans;
    *ansp = ans;
    return true;
}

bool ReceiveMOTBin(char *str, uint8_t *buf, uint32_t bufsize, uint32_t *np, bool *is_end, uint32_t *exec_addr)
{
    if (str == NULL) return false;
    if (buf == NULL) return false;
    if (bufsize == 0) return false;
    if(is_end != NULL) *is_end = false;
    if(exec_addr != NULL) *exec_addr = 0;

    uint8_t *p = buf;
    char *strp = str;
    uint32_t addr = 0;
    uint32_t bytes = 0;
    uint32_t sum = 0;
    uint32_t dat;
    uint32_t datbytes = 0;
    // Parse header
    if (*strp != 'S') return false;
    strp++;
    switch (*strp) {
    case '0':
        strp++;
        if (!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        if(dat < 3) return false;
        bytes = dat - 1;
        while (bytes > 0) {
            // Dummy read
            if (!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            bytes--;
        }
        sum = ~sum;
        if(!in_atoh(strp, &dat, NULL)) return false;
        if(sum != dat) return false; // SUM ERROR
        strp += 2;
        if((*strp != '\n') && (*strp != '\r') && (*strp != '\0')) return false;
        break;
        // HEADER RECORD
    case '1':
        strp++;
        if (!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        if(dat < 3) return false;
        bytes = dat - 1;
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes = dat << 8;
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes |= dat;
        bytes -= 2;
        while ((bytes > 0) && (datbytes > 0) && (bufsize > 0)) {
            // Dummy read
            if (!in_atoh(strp, &dat, &sum)) return false;
            *buf = (uint8_t)dat;
            buf++;
            strp += 2;
            bytes--;
            bufsize--;
            datbytes--;
        }
        if((bytes != 0) || (datbytes != 0)) return false;
        sum = ~sum;
        if(!in_atoh(strp, &dat, NULL)) return false;
        if(sum != dat) return false; // SUM ERROR
        strp += 2;
        if((*strp != '\n') && (*strp != '\r') && (*strp != '\0')) return false;
        break;
    case '2':
        strp++;
        if (!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        if(dat < 4) return false;
        bytes = dat - 1;
        
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes = dat << 16;
        
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes |= (dat << 8);
        
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes |= dat;
        
        bytes -= 3;
        while ((bytes > 0) && (datbytes > 0) && (bufsize > 0)) {
            // Dummy read
            if (!in_atoh(strp, &dat, &sum)) return false;
            *buf = (uint8_t)dat;
            buf++;
            strp += 2;
            bytes--;
            datbytes--;
            bufsize--;
        }
        if((bytes != 0) || (datbytes != 0)) return false;
        sum = ~sum;
        if(!in_atoh(strp, &dat, NULL)) return false;
        if(sum != dat) return false; // SUM ERROR
        strp += 2;
        if((*strp != '\n') && (*strp != '\r') && (*strp != '\0')) return false;
        break;
    case '3':
        strp++;
        if (!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        if(dat < 5) return false;
        bytes = dat - 1;
        
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes = dat << 24;
        
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes |= (dat << 16);
        
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes |= (dat << 8);
        
        if(!in_atoh(strp, &dat, &sum)) return false;
        strp += 2;
        datbytes |= dat;
        bytes -= 3;
        while ((bytes > 0) && (datbytes > 0) && (bufsize > 0)) {
            // Dummy read
            if (!in_atoh(strp, &dat, &sum)) return false;
            *buf = (uint8_t)dat;
            buf++;
            strp += 2;
            bytes--;
            datbytes--;
            bufsize--;
        }
        if((bytes != 0) || (datbytes != 0)) return false;
        sum = ~sum;
        if(!in_atoh(strp, &dat, NULL)) return false;
        if(sum != dat) return false; // SUM ERROR
        strp += 2;
        if((*strp != '\n') && (*strp != '\r') && (*strp != '\0')) return false;
        break;
    case '5':
            // Record
            break;
    case '7':
        {
            uint32_t eadr;
            strp++;
            if (!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            if(dat < 5) return false;
            bytes = dat - 1;
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr = (dat << 24);
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr |= (dat << 16);
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr |= (dat << 8);
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr |= dat;
            
            bytes -= 2;
            if(bytes != 0) return false;
            
            sum = ~sum;
            if(!in_atoh(strp, &dat, NULL)) return false;
            if(sum != dat) return false; // SUM ERROR
            strp += 2;
            if((*strp != '\n') && (*strp != '\r') && (*strp != '\0')) return false;
            if(exec_addr != NULL) *exec_addr = eadr;
            if(is_end != NULL) *is_end = true;
        }   
        break;
    case '8':
        {
            uint32_t eadr;
            strp++;
            if (!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            if(dat < 4) return false;
            bytes = dat - 1;
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr = (dat << 16);
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr |= (dat << 8);
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr |= dat;
            
            bytes -= 2;
            if(bytes != 0) return false;
            
            sum = ~sum;
            if(!in_atoh(strp, &dat, NULL)) return false;
            if(sum != dat) return false; // SUM ERROR
            strp += 2;
            if((*strp != '\n') && (*strp != '\r') && (*strp != '\0')) return false;
            if(exec_addr != NULL) *exec_addr = eadr;
            if(is_end != NULL) *is_end = true;
        }   
        break;
    case '9':
        {
            uint32_t eadr;
            strp++;
            if (!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            if(dat < 3) return false;
            bytes = dat - 1;
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr = dat << 8;
            
            if(!in_atoh(strp, &dat, &sum)) return false;
            strp += 2;
            eadr |= dat;
            
            bytes -= 2;
            if(bytes != 0) return false;
            
            sum = ~sum;
            if(!in_atoh(strp, &dat, NULL)) return false;
            if(sum != dat) return false; // SUM ERROR
            strp += 2;
            if((*strp != '\n') && (*strp != '\r') && (*strp != '\0')) return false;
            if(exec_addr != NULL) *exec_addr = eadr;
            if(is_end != NULL) *is_end = true;
        }   
        break;
    }
    return true;
}

void DEBUG_TERM_Tasks(void)
{

    /* Check the application's current state. */
    while (1) {
        vTaskDelay(cTick1Sec);
    }
}



/*******************************************************************************
 End of File
 */
