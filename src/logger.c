/*
 * Logging features .
 * (C) 2017 Kyuma Ohta <whatisthis.sowhat _at_ gmail.com >
 * This is part of "DoorBell_PIC32" . You must have MPLAB HARMONY to use this routines.
 * License : Apache OSS License.
 */

/* Kernel includes. */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "doorbell.h"

extern DOORBELL_DATA doorbellData;
extern DRV_HANDLE xDevHandleUart_Recv;
extern DRV_HANDLE xDevHandleUart_Send;

void pushLog(SYS_RTCC_BCD_DATE _date, SYS_RTCC_BCD_TIME _time, uint8_t _type, uint8_t *_data, uint8_t _len)
{
    uint16_t current = doorbellData.realdata.log_tail;
    int8_t seed = 0;
    int i;
    int8_t *p;
    // Clear data
    memset(doorbellData.realdata.logdata[current].data, 0x00, sizeof (uint8_t) * 8);
    // Set data
    doorbellData.realdata.logdata[current].n_date = _date;
    doorbellData.realdata.logdata[current].n_time = _time;
	if(_data != NULL) {
		memcpy(doorbellData.realdata.logdata[current].data, _data, _len & 7);
	}
    // Calc check sum
    p = (int8_t *)&(doorbellData.realdata.logdata[current].n_date);
    for (i = 0; i < (sizeof (DOORBELL_LOG_DATA_T) - sizeof (uint8_t)); i++) {
        seed += p[i];
    }
    seed = -seed;
    doorbellData.realdata.logdata[current].n_sum = seed;
    current++;
    if (current >= LOG_LENGTH) {
        current = 0;
    }
    doorbellData.realdata.log_tail = current;

}


bool pushUartQueue(char *str)
{
	ssize_t _len;
	BaseType_t stat;
	int i;
	
	if(str == NULL) return false;
	if(xUartSendQueue == NULL) return false;
	
	_len = strlen(str);
	if(_len > 0) {
		i = 0;
		while(i < _len) {
			stat = xQueueSend(xUartSendQueue, &(str[i]), cTick1Sec);
			if(stat != pdTRUE) continue;
			i++;
		}
	}
	return true;
}

void printLog(int index, char *head, char *str, uint8_t _type, uint8_t *rawdata, uint8_t _rawlen)
{
    SYS_RTCC_BCD_DATE _nowdate;    
    SYS_RTCC_BCD_TIME _nowtime;
	BaseType_t stat;
    char buf[96];
	int i;
	ssize_t _len;

    memset(buf, 0x00, sizeof (buf));
    getDateTime(&_nowdate, &_nowtime);
 
    if((_type & 0x7f) != LOG_TYPE_NOP) pushLog(_nowdate, _nowtime, _type, rawdata, _rawlen);

	if(xUartSendQueue == NULL) return;
    if (head != NULL) {
        snprintf(buf, 96, "[%s] ", head);
    }

	if(index == 0) {
		pushUartQueue(buf);
	} else {
		// USB
	}
    getDateTimeStr(_nowdate,  _nowtime, buf, sizeof (buf), false);
    if(index == 0) {
		pushUartQueue(buf);
	} else {
		// USB
	}
	snprintf(buf, 96, ": %s", str);
	if(index == 0) {
		pushUartQueue(buf);
	} else {
		// USB
	}
	
	snprintf(buf, 96, "\n");
	if(index == 0) {
		pushUartQueue(buf);
	} else {
		// USB
	}
}


void prvWriteToUart(void)
{
	char qval[4];
	BaseType_t stat;
	ssize_t size;
	bool mustread = true;
	while(1) {
		stat = pdPASS;
		if(xUartSendQueue != NULL) {
			while(stat == pdTRUE) {
				if(mustread) stat = xQueueReceive(xUartSendQueue, qval, 0xffffffff);
				if((stat == pdTRUE) && (xDevHandleUart_Send != DRV_HANDLE_INVALID)) {
				    //SYS_CONSOLE_Write(SYS_CONSOLE_INDEX_0, STDOUT_FILENO, qval, 1);
					size = DRV_USART_Write(xDevHandleUart_Send, qval, sizeof(char));
					if(size < (sizeof(char) * 1)) {
						mustread = false;
					} else {
						mustread = true;
					}
				}
			}
		}
		vTaskDelay(cTick110ms);
	}		
}

void prvReadFromUart(void)
{
	char qval[4];
	BaseType_t stat;
	ssize_t _len = 0;
	while(1) {
		stat = pdPASS;
		if(xUartRecvQueue != NULL) {
			//_len = SYS_CONSOLE_Read(SYS_CONSOLE_INDEX_0, STDIN_FILENO, qval, 1);
			_len = DRV_UART_Read(xDevHandleUart_Recv, qval, sizeof(char));
			if(_len >= (sizeof(char) * 1)) {
				stat = xQueueSend(xUartRecvQueue, &qval, 0xffffffff);
			}
		}
		if(_len < 1) vTaskDelay(cTick110ms);
	}		
}
	
