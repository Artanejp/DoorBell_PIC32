/*******************************************************************************
  Interface definition of the RTCC System Service.

  Company:
    Microchip Technology Inc.

  File Name:
    sys_rtcc.h

  Summary:
    Interface definition of the Real-Time Clock and Calendar System Service (RTCC).

  Description:
    This file defines the interface for the Real-Time Clock and Calendar (RTCC)
    System Service.
    The Real-Time Clock allows events to be triggered by time and date.
*******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2016 released Microchip Technology Inc.  All rights reserved.

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

#ifndef _SYS_RTCC_H    // Guards against multiple inclusion
#define _SYS_RTCC_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

/*  This section lists the other files that are included in this file.
*/
#include "system/common/sys_module.h"
#include "peripheral/rtcc/plib_rtcc.h"
#include "peripheral/int/plib_int.h"

#define RTCC_PLIB_ID    RTCC_ID_0
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* SYS_RTCC_BCD_DATE

  Summary:
    Holds a date in BCD format.

  Description:
	Each nibble: 
	  [year10][year1][mnth10][mnth1][day10][day1][xxxx][DOTW] (Day Of The Week).

  Remarks:
    Fill this in manually: date.year10 = 1; or use helper function.
*/

typedef uint32_t SYS_RTCC_BCD_DATE;

// *****************************************************************************
/* SYS_RTCC_BCD_TIME

  Summary:
    Holds a time in BCD format.

  Description:
	Each nibble: 
	  [hour10][hour1][mins10][mins1][secs10][secs1][xxxx][xxxx].

  Remarks:
    Fill this in manually: time.secs1 = 5; or use helper function.
*/

typedef uint32_t SYS_RTCC_BCD_TIME;

// *****************************************************************************
/* SYS_RTCC_STATUS

  Summary:
     Return value of status for most RTCC calls.

  Description:
    A member is returned based on the results (status) of the call made to
    a SYS_RTCC_xxxx function.

  Remarks:
  	Status should be checked after a call to see if the call executed okay and
  	returns with a value of SYS_RTCC_STATUS_OK.
*/

typedef enum 
{
    SYS_RTCC_STATUS_OK = 0,
    SYS_RTCC_STATUS_NOT_READY,
    SYS_RTCC_STATUS_NO_DEVICE,
    SYS_RTCC_STATUS_IN_USE,
} SYS_RTCC_STATUS;

// *****************************************************************************
/* SYS_RTCC_ALARM_HANDLE

  Summary:
	Holds a handle for an alarm.
	
  Description:
	SYS_RTCC_ALARM_HANDLE holds a handle to an alarm.
	
  Remarks:
    Use SYS_RTCC_ALARM_HANDLE to receive a handle from AlarmRegister function.
*/

typedef uintptr_t SYS_RTCC_ALARM_HANDLE;
#define SYS_RTCC_ALARM_HANDLE_INVALID ((SYS_RTCC_ALARM_HANDLE)0)

// *****************************************************************************
/* SYS_RTCC_ALARM_CALLBACK

  Summary:
    Use to register a callback with the RTCC.

  Description:
    When the alarm is asserted, a callback can be activated. 
    Use SYS_RTCC_ALARM_CALLBACK as the function pointer to register the callback
    with the alarm.

  Remarks:
    The callback should look like: 
      void callback(handle, context);
	Make sure the return value and parameters of the callback are correct.
*/

typedef void (*SYS_RTCC_ALARM_CALLBACK)(SYS_RTCC_ALARM_HANDLE handle, 
    uintptr_t context);

// *****************************************************************************
typedef struct _SYS_RTCC_OBJ_STRUCT
{
    /* - Object status */
    SYS_RTCC_STATUS status;
	/* - Call back function for RTCC. Happens at ALARMED */
    SYS_RTCC_ALARM_CALLBACK  callback;
	/* - Client data (Event Context) that will be passed to callback */
    uintptr_t context;
	/* - RTCC Alarm handle */
    SYS_RTCC_ALARM_HANDLE handle;
    /* - Interrupt source */
    INT_SOURCE interruptSource;
} SYS_RTCC_OBJECT;

	
	
// *****************************************************************************
// *****************************************************************************
// Section: Interface Routines
// *****************************************************************************
// *****************************************************************************

/*  Functions to support the system service for RTCC.
*/

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_Start( void )

  Summary:
    Starts the Real-Time Clock Calendar.

  Description:
    The function starts the RTCC.

  Precondition:
    The clock source should be selected.

  Parameters:
    None.
  
  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_STATUS status = SYS_RTCC_Start();
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>

  Remarks:
     If the RTCC was running it continues.
*/

SYS_RTCC_STATUS SYS_RTCC_Start(void);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_Stop( void )

  Summary:
    Stops the Real-Time Clock Calendar.

  Description:
    The function stops the RTCC.

  Precondition:
    None.

  Parameters:
    None.
  
  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_STATUS status = SYS_RTCC_Stop();
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_Stop(void);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_TimeSet(SYS_RTCC_BCD_TIME  time, bool start)

  Summary:
    Sets the Real-Time Clock Calendar time.

  Description:
    The function sets the time for the RTCC. 

  Precondition:
    None.

  Parameters:
    time    - time is in BCD format - see description of SYS_RTCC_BCD_TIME

    start   - if true, the RTCC is also started
  
  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_TIME time = 0x23121200;
    SYS_RTCC_STATUS status = SYS_RTCC_TimeSet(time, true);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_TimeSet(SYS_RTCC_BCD_TIME  time, bool start);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_TimeGet(SYS_RTCC_BCD_TIME  *time)

  Summary:
    Obtains the Real-Time Clock Calendar time.

  Description:
    The function obtains the time from the RTCC. 

  Precondition:
    None.

  Parameters:
    *time    - a pointer to a time type (see description of SYS_RTCC_BCD_TIME)

    Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_TIME time;
    SYS_RTCC_STATUS status = SYS_RTCC_TimeSet(&time);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_TimeGet(SYS_RTCC_BCD_TIME *time);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_DateSet(SYS_RTCC_BCD_DATE  date)

  Summary:
    Sets the Real-Time Clock Calendar date.

  Description:
    The function sets the date for the RTCC in BCD format.

  Precondition:
    None.

  Parameters:
    date    - date is in BCD format (see description of SYS_RTCC_BCD_DATE)
  
  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_DATE date = 0x15121201;
    SYS_RTCC_STATUS status = SYS_RTCC_DateSet(date);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_DateSet(SYS_RTCC_BCD_DATE  date);


// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_DateGet(SYS_RTCC_BCD_DATE *date)

  Summary:
    Gets the Real-Time Clock Calendar date.

  Description:
    The function gets the date from the RTCC in BCD format.

  Precondition:
    None.

  Parameters:
    *date    - a pointer to a date type (see description of SYS_RTCC_BCD_DATE)
  
  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_DATE date;
    SYS_RTCC_STATUS status = SYS_RTCC_DateGet(&date);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_DateGet(SYS_RTCC_BCD_DATE *date);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_AlarmTimeSet(SYS_RTCC_BCD_TIME  time, bool enable)

  Summary:
    Sets the Real-Time Clock Calendar alarm time.

  Description:
    The function sets the time for the RTCC alarm.

  Precondition:
    None.

  Parameters:
    time     - time is in BCD format (see description of SYS_RTCC_BCD_TIME)

    enable   - if true, the alarm is enabled
  
  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_TIME time = 0x00001000;    // ten seconds
    SYS_RTCC_STATUS status = SYS_RTCC_AlarmTimeSet(time, true);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_AlarmTimeSet(SYS_RTCC_BCD_TIME  time, bool enable);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_AlarmTimeGet(SYS_RTCC_BCD_TIME *time)

  Summary:
    Gets the Real-Time Clock Calendar alarm time.

  Description:
    The function gets the time from the RTCC alarm.

  Precondition:
    None.

  Parameters:
    *time    - a pointer to the time type (see description of SYS_RTCC_BCD_TIME)
  
  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_TIME time;
    SYS_RTCC_STATUS status = SYS_RTCC_AlarmTimeGet(&time);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_AlarmTimeGet(SYS_RTCC_BCD_TIME *time);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_AlarmDateSet(SYS_RTCC_BCD_DATE date)

  Summary:
    Sets the Real-Time Clock Calendar alarm date.

  Description:
    The function sets the time for the RTCC alarm. The date for the alarm
    does not include the year. If the year is included it will be ignored.

  Precondition:
    None.

  Parameters:
    date    - date is in BCD format (see description of SYS_RTCC_BCD_DATE)

  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_DATE date = 0x00020100;
    SYS_RTCC_STATUS status = SYS_RTCC_AlarmDateSet(date);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_AlarmDateSet(SYS_RTCC_BCD_DATE  date);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_AlarmDateGet(SYS_RTCC_BCD_DATE *date)

  Summary:
    Gets the Real-Time Clock Calendar alarm date.

  Description:
    The function gets the time for the RTCC alarm.

  Precondition:
    None.

  Parameters:
    *date    - pointer to date type (see description of SYS_RTCC_BCD_DATE)

  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_BCD_DATE date;
    SYS_RTCC_STATUS status = SYS_RTCC_AlarmDateGet(&date);
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_AlarmDateGet(SYS_RTCC_BCD_DATE *date);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_AlarmEnable( void )

  Summary:
    Enables RTCC alarm.

  Description:
    The function enables the alarm in the RTCC.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_STATUS status = SYS_RTCC_AlarmEnable();
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_AlarmEnable(void);

// *****************************************************************************
/* Function:
  SYS_RTCC_STATUS SYS_RTCC_AlarmDisable( void )

  Summary:
    Disables the RTCC alarm.

  Description:
    The function disables the alarm in the RTCC.

  Precondition:
    None.

  Parameters:
    None.

  Returns:
    SYS_RTCC_STATUS type (see above).

  Example:
    <code>
    SYS_RTCC_STATUS status = SYS_RTCC_AlarmDisable();
    if (status != SYS_RTCC_STATUS_OK)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_STATUS SYS_RTCC_AlarmDisable(void);

// *****************************************************************************
/* Function:
  SYS_RTCC_ALARM_HANDLE SYS_RTCC_AlarmRegister(SYS_RTCC_ALARM_CALLBACK *callback, 
    uintptr_t context );
  
  Summary:
    Sets the callback function for an alarm.

  Description:
    This function sets the callback function that will be called when the RTCC
    alarm is reached.

  Precondition:
    None.

  Parameters:
    *callback   - a pointer to the function to be called when alarm is reached.
    context     - a pointer to user defined data to be used when the callback
                  function is called. NULL can be passed in for no data needed.

  Returns:
    SYS_RTCC_ALARM_HANDLE type (see above).

  Example:
    <code>
    SYS_RTCC_ALARM_HANDLE handle = SYS_RTCC_AlarmRegister(myCallback, NULL);
    if (handle == SYS_RTCC_ALARM_HANDLE_INVALID)
    {
        // error!
    }
    </code>
*/

SYS_RTCC_ALARM_HANDLE SYS_RTCC_AlarmRegister(SYS_RTCC_ALARM_CALLBACK callback, 
    uintptr_t context );

// *****************************************************************************
/* Function:
  uint32_t SYS_RTCC_TimeBCD2Seconds(SYS_RTCC_BCD_TIME time)
  
  Summary:
    Helper function for time.
    
  Description:
    This function returns the number of seconds when given a BCD encoded time
    value (see SYS_RTCC_BCD_TIME typedef above).

  Precondition:
    None.

  Parameters:
    time    - a SYS_RTCC_BCD_TIME value.

  Returns:
    The number of seconds represented by the BCD value.

  Example:
    <code>
    uint32_t seconds_left;
    seconds_left = SYS_RTCC_TimeBCD2Seconds(0x00002200);
    </code>
*/

uint32_t SYS_RTCC_TimeBCD2Seconds(SYS_RTCC_BCD_TIME time);

// *****************************************************************************
/* Function:
  SYS_RTCC_BCD_TIME SYS_RTCC_TimeSeconds2BCD(uint32_t seconds)
  
  Summary:
    Helper function for time.
    
  Description:
    This function returns the BCD encoded time value for the given number of seconds.

  Precondition:
    None.

  Parameters:
    seconds    - number of seconds to convert.

  Returns:
    A SYS_RTCC_BCD_TIME type value in BCD of the number of seconds given.

  Example:
    <code>
    SYS_RTCC_BCD_TIME time;
    time = SYS_RTCC_TimeSeconds2BCD(1000);
    SYS_RTCC_AlarmTimeSet(time, true);
    </code>
*/
SYS_RTCC_BCD_TIME SYS_RTCC_TimeSeconds2BCD(uint32_t seconds);

//*******************************************************************************
/*
  Function:
    SYS_MODULE_OBJ SYS_RTCC_Initialize ( const SYS_MODULE_INIT * const init )

  Summary:
    Real-Time Clock Calendar System Service initialization function.

  Description:
    This function initializes the RTCC Service.  It ensures that RTCC
	is stable and places the RTCC service in its initial state so that the 
	service's state machine(s) can run.

  Precondition:
    None

  Parameters:
    None

  Returns:
    SYS_MODULE_OBJ.

  Example:
    <code>
    sysObj.sysRtcc = SYS_RTCC_Initialize( );
    </code>

  Remarks:
    This function must be called from the SYS_Initialize function.
    This function loops and blocks until RTCC is stable.
*/

SYS_MODULE_OBJ SYS_RTCC_Initialize ( void );

// *****************************************************************************
/* Function:
    void SYS_RTCC_Tasks ( SYS_MODULE_OBJ object )

  Summary:
    Maintains the RTCC System Service state machine and implements its ISR.

  Description:
    This function is used to maintain the internal state machine of the RTCC
    System Service.
	It should be always be called from the 'Sys_Tasks' function.

  Precondition:
    The SYS_RTCC_Initialize function must have been called for the specified TMR
    driver instance.
	
  Parameters:
    object          - SYS RTCC object handle, returned from SYS_RTCC_Initialize

  Returns:
    None.

  Example:
    <code>
    SYS_MODULE_OBJ      object;     // Returned from SYS_RTCC_Initialize

    while (true)
    {
        SYS_RTCC_Tasks ( object );

        // Do other tasks
    }
    </code>

  Remarks:
    This function is normally not called directly by an application. It is
    called by the system's Tasks routine (SYS_Tasks) or by the appropriate raw
    ISR.
*/

void SYS_RTCC_Tasks ( SYS_MODULE_OBJ object );

#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif

#endif /* _SYS_RTCC_H */

/*******************************************************************************
 End of File
*/