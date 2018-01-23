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
#include "peripheral/peripheral.h"
#include "system/common/sys_module.h"
#include "system/system.h"

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 
#ifndef ssize_t
#define ssize_t int
#endif
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
         // It's dummy
         DOORBELL_STATE_EXIT_SYSTEM,

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
enum {
    N_UNSTABLE = -1,
    N_PROMPT = -2,
    N_HOST_PROGRESS = -3,
    N_HOST_COMPLETE = 0,
    N_HOST_EOF = -16,
    N_HOST_FAIL = -256
};

enum {
    C_TWE_ON = 1,
    C_TWE_FULL_RUN,
    C_TWE_IDLE_ON,
    C_TWE_IDLE_OFF,
    C_TWE_FORCE_WAKEUP,
    C_TWE_FORCE_SLEEP,
	C_TWE_WRITE_OK,
	C_TWE_WRITE_WAIT,
};

typedef struct
{
    uint32_t num;
    bool toggle_status;
    void *userdata;
} DOORBELL_TIMER_TICK_T;



enum {
    LOG_TYPE_MESSAGE = 0,
    LOG_TYPE_PUSH_RING,
    LOG_TYPE_RELEASE_RING,
    LOG_TYPE_PUSH_DEBUG,
    LOG_TYPE_RELEASE_DEBUG,
    LOG_TYPE_ALART,
    LOG_TYPE_MAINPOW_OFF,
    LOG_TYPE_MAINPOW_ON,
    LOG_TYPE_BACKUP_LOW,
    LOG_TYPE_BACKUP_HIGH,
    LOG_TYPE_WDT_RESET,
    LOG_TYPE_BOR_RESET,
    LOG_TYPE_HOTSTART,
    LOG_TYPE_ILLSUM,
    LOG_TYPE_UNCORRECTABLE,
    LOG_TYPE_TIME_DIFF,
    LOG_TYPE_TIME_UPDATED,
    LOG_TYPE_TIME_ERROR,
    LOG_TYPE_SOUND_ON,
    LOG_TYPE_SOUND_OFF,
    LOG_TYPE_DIPSW, 
    LOG_TYPE_NOP,
    LOG_TYPE_TEMP1 = 0x50,
    LOG_TYPE_SENSOR1 = 0x60,
    LOG_TYPE_END = 0x7f,
    LOG_TYPE_READ = 0x80, // FLAG
} DOORBELL_LOG_TYPE_T;

#pragma pack(push, 1)
typedef struct {
    SYS_RTCC_BCD_DATE n_date;
    SYS_RTCC_BCD_TIME n_time;
    uint8_t n_type;
    uint8_t data[8];
    int8_t n_sum;
} DOORBELL_LOG_DATA_T;

#define LOG_LENGTH 128
typedef struct {
    bool initialized;
    bool wakeup_randomize;
    SYS_RTCC_BCD_DATE recent_date;
    SYS_RTCC_BCD_TIME recent_time;
    uint32_t tick_sec; // Wakeup timer
    uint32_t recent_temp1;
    uint16_t log_top; // already read.
    uint16_t log_tail;
    uint16_t log_numbers;
    DOORBELL_LOG_DATA_T logdata[LOG_LENGTH]; // Maybe 8KB.
    char uniqueName[16];
} DOORBELL_REAL_DATA_T;
#pragma pack(pop)

#define SOUND_RATE 16000
#define SOUND_LENGTH ((8 * SOUND_RATE) / 100)
enum {
    C_SOUND_STOP = 0,
    C_SOUND_INIT,
    C_SOUND_START,
    C_SOUND_PLAY,
    C_SOUND_ABORT,
    C_SOUND_PAUSE,
};
typedef struct {
    int16_t cmd;
    void *ptr;
    uint32_t len;
} sndData_t;

#define MD5_DIGEST_SIZE 64
#pragma (pack, 1)
typedef struct
{
    /* The application's current state */
    bool bootparam_passthrough;
    bool ringed;
    bool uart_ready;
    bool usb_ready;
    RESET_REASON resetReason;
    DOORBELL_REAL_DATA_T realdata;
    unsigned char data_md5sum[MD5_DIGEST_SIZE];
    CRYPT_MD5_CTX md5_context;
} DOORBELL_DATA;
#pragma (pop)

#define ALARM_TICK_SECONDS 1800
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

extern void SLEEP_Periferals(bool onoff);
// Tasks
void DOORBELL_Tasks();
void READUART_Tasks();

extern void CALC_MD5Sum(void);
extern bool CHECK_MD5Sum(void);
extern int checkSender(char *data, uint32_t *hostnum, char **ps, size_t maxlen);
// Push string to UART
extern bool pushUartQueue1(char *str);

extern SYS_RTCC_ALARM_HANDLE *hAlarmTick;

extern TaskHandle_t xHandleReadFromUSB;
extern TaskHandle_t xHandleWriteToUSB;
extern QueueHandle_t xUsbRecvQueue;
extern QueueHandle_t xUsbSendQueue;

extern void prvHouseKeeping(void *pvParameters);

extern uint32_t cTick100ms;
extern uint32_t cTick110ms;
extern uint32_t cTick200ms;
extern uint32_t cTick500ms;
extern uint32_t cTick1Sec;
extern uint32_t cTick5Sec;

#include "logger.h"

#endif /* _DOORBELL_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

