/*******************************************************************************
 System Tasks File

  File Name:
    system_tasks.c

  Summary:
    This file contains source code necessary to maintain system's polled state
    machines.

  Description:
    This file contains source code necessary to maintain system's polled state
    machines.  It implements the "SYS_Tasks" function that calls the individual
    "Tasks" functions for all the MPLAB Harmony modules in the system.

  Remarks:
    This file requires access to the systemObjects global data structure that
    contains the object handles to all MPLAB Harmony module objects executing
    polled in the system.  These handles are passed into the individual module
    "Tasks" functions to identify the instance of the module to maintain.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2015 released Microchip Technology Inc.  All rights reserved.

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

#include "system_config.h"
#include "system_definitions.h"
#include "doorbell.h"
#include "read_uart.h"
#include "t_sounder.h"
#include "write_uart.h"


// *****************************************************************************
// *****************************************************************************
// Section: Local Prototypes
// *****************************************************************************
// *****************************************************************************


 
static void _SYS_Tasks ( void );
 
 
static void _DOORBELL_Tasks(void);
static void _READ_UART_Tasks(void);
static void _T_SOUNDER_Tasks(void);
static void _WRITE_UART_Tasks(void);


// *****************************************************************************
// *****************************************************************************
// Section: System "Tasks" Routine
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Tasks ( void )

  Remarks:
    See prototype in system/common/sys_module.h.
*/

void SYS_Tasks ( void )
{
    /* Create OS Thread for Sys Tasks. */
    xTaskCreate((TaskFunction_t) _SYS_Tasks,
                "Sys Tasks",
                1280, NULL, 1, NULL);

 
 
    /* Create OS Thread for DOORBELL Tasks. */
    xTaskCreate((TaskFunction_t) _DOORBELL_Tasks,
                "DOORBELL Tasks",
                1024, NULL, 2, NULL);

    /* Create OS Thread for READ_UART Tasks. */
    xTaskCreate((TaskFunction_t) _READ_UART_Tasks,
                "READ_UART Tasks",
                384, NULL, 2, NULL);

    /* Create OS Thread for T_SOUNDER Tasks. */
    xTaskCreate((TaskFunction_t) _T_SOUNDER_Tasks,
                "T_SOUNDER Tasks",
                1024, NULL, 1, NULL);

    /* Create OS Thread for WRITE_UART Tasks. */
    xTaskCreate((TaskFunction_t) _WRITE_UART_Tasks,
                "WRITE_UART Tasks",
                256, NULL, 2, NULL);

    /**************
     * Start RTOS * 
     **************/
    vTaskStartScheduler(); /* This function never returns. */
}


/*******************************************************************************
  Function:
    void _SYS_Tasks ( void )

  Summary:
    Maintains state machines of system modules.
*/
static void _SYS_Tasks ( void)
{
    while(1)
    {
        /* Maintain system services */
        SYS_RTCC_Tasks(sysObj.sysRtcc);
    SYS_MSG_Tasks( (SYS_OBJ_HANDLE) sysObj.sysMsg0 );

        /* Maintain Device Drivers */
 
 

        /* Maintain Middleware */

        /* Task Delay */
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

 
 

/*******************************************************************************
  Function:
    void _DOORBELL_Tasks ( void )

  Summary:
    Maintains state machine of DOORBELL.
*/

static void _DOORBELL_Tasks(void)
{
    while(1)
    {
        DOORBELL_Tasks();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}


/*******************************************************************************
  Function:
    void _READ_UART_Tasks ( void )

  Summary:
    Maintains state machine of READ_UART.
*/

static void _READ_UART_Tasks(void)
{
    while(1)
    {
        READ_UART_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _T_SOUNDER_Tasks ( void )

  Summary:
    Maintains state machine of T_SOUNDER.
*/

static void _T_SOUNDER_Tasks(void)
{
    while(1)
    {
        T_SOUNDER_Tasks();
    }
}


/*******************************************************************************
  Function:
    void _WRITE_UART_Tasks ( void )

  Summary:
    Maintains state machine of WRITE_UART.
*/

static void _WRITE_UART_Tasks(void)
{
    while(1)
    {
        WRITE_UART_Tasks();
    }
}


/*******************************************************************************
 End of File
 */
