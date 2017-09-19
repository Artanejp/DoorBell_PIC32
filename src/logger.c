/*
 * Logging features .
 * (C) 2017 Kyuma Ohta <whatisthis.sowhat _at_ gmail.com >
 * This is part of "DoorBell_PIC32" . You must have MPLAB HARMONY to use this routines.
 * License : Apache OSS License.
 */


#include "doorbell.h"

extern DOORBELL_DATA doorbellData;

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
    memcpy(doorbellData.realdata.logdata[current].data, _data, _len & 7);
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

void printLog(const SYS_MODULE_INDEX cons_index, char *head, char *str, uint8_t _type, uint8_t *rawdata, uint8_t _rawlen)
{
    SYS_RTCC_BCD_DATE _nowdate;    
    SYS_RTCC_BCD_TIME _nowtime;
    char buf[1024];
    char daybuf[256];

    memset(buf, 0x00, sizeof (buf));
    getDateTime(&_nowdate, &_nowtime);
    getDateTimeStr(daybuf, _nowdate,  _nowtime, sizeof (daybuf), false);
    pushLog(_nowdate, _nowtime, _type, rawdata, _rawlen);

    if (head != NULL) {
        snprintf(buf, 1024, "[%s] %s: %s\n", head, daybuf, str);
    } else {
        snprintf(buf, 1024, "[%s] %s: %s\n", doorbellData.realdata.uniqueName, daybuf, str);
    }
    SYS_CONSOLE_Write(cons_index, STDOUT_FILENO, buf, strlen(buf));
}

