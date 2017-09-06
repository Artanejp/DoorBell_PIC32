/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    doorbell.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _DOORBELL_H
#define _DOORBELL_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "crypto/crypto.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	DOORBELL_STATE_INIT=0,
	DOORBELL_STATE_SERVICE_TASKS,

	/* TODO: Define states used by the application state machine. */
         //DOORBELL_STATE_BUTTON_PRESSED,
         //DOORBELL_STATE_BUTTON_REMOVED,
            
         DOORBELL_STATE_PLAYING_MUSIC,
         DOORBELL_STATE_STOP_MUSIC,
         DOORBELL_STATE_REPEAT_MUSIC,
            
         DOORBELL_STATE_RENDER_MUSIC,
         DOORBELL_STATE_UPDATE_PLAY_POINTER,
            
         DOORBELL_STATE_SLAVE_REQ_DEBUG,
         DOORBELL_STATE_SLAVE_REQ_COMM,
         DOORBELL_STATE_SLAVE_RESET,
            
         DOORBELL_STATE_PUSH_STATE,
         DOORBELL_STATE_COMMUNICATE,
            
         DOORBELL_STATE_REQ_TEMP0,
         DOORBELL_STATE_READING_TEMP0,
         DOORBELL_STATE_DONE_TEMP0,
            
         DOORBELL_STATE_REQ_TEMP1,
         DOORBELL_STATE_READING_TEMP1,
         DOORBELL_STATE_DONE_TEMP1,
            
         DOORBELL_STATE_USB_CMDMODE,
         DOORBELL_STATE_USB_PASSTHROUGH,
         DOORBELL_STATE_USB_NOTCONNECTED,
         
         DOORBELL_STATE_SLEEP_UNTIL_ALARM,
         DOORBELL_STATE_WAIT_COMMAND,
         DOORBELL_STATE_PROCESS_COMMAND,
         // Shell Command
         DOORBELL_STATE_CMD_TIME_SET,
         DOORBELL_STATE_CMD_ALARM_SET,
         DOORBELL_STATE_CMD_READ_TEMP0, // External sensor
         DOORBELL_STATE_CMD_READ_TEMP1, // Internal sensor
         DOORBELL_STATE_CMD_READ_MUSIC,
         DOORBELL_STATE_CMD_WRITE_MUSIC,
         DOORBELL_STATE_CMD_PLAY_MUSIC,
         DOORBELL_STATE_CMD_READ_LOG,
         DOORBELL_STATE_CMD_WRITE_LOG,
         DOORBELL_STATE_CMD_COMM_TO_SLAVE,
         DOORBELL_STATE_CMD_EXECUTE_SLAVE,
         DOORBELL_STATE_CMD_RESET,
         DOORBELL_STATE_CMD_WAKEUP_SLAVE,
         DOORBELL_STATE_CMD_SUSPEND_SLAVE,
         DOORBELL_STATE_CMD_LOGOUT,
} DOORBELL_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct 
{
    SYS_RTCC_BCD_DATE r_date;
    SYS_RTCC_BCD_TIME r_time;
} DOORBELL_TIME_T;
typedef struct {
    bool initialized;
    bool wakeup_randomize;
    DOORBELL_TIME_T before_wakeup;
    DOORBELL_TIME_T next_wakeup;
    uint32_t tick_sec; // Wakeup timer
    
} DOORBELL_REAL_DATA_T;
typedef struct
{
    /* The application's current state */
    DOORBELL_STATES state;
    DOORBELL_REAL_DATA_T realdata;
    uint8_t data_md5sum[];
    /* TODO: Define any additional data used by the application. */
} DOORBELL_DATA;


// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void DOORBELL_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the 
    application in its initial state and prepares it to run so that its 
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    DOORBELL_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void DOORBELL_Initialize ( void );


/*******************************************************************************
  Function:
    void DOORBELL_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    DOORBELL_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void DOORBELL_Tasks( void );


#endif /* _DOORBELL_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

