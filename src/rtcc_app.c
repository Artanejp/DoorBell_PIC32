/*
 * Manipulate RTCC .
 * (C) 2017 Kyuma Ohta <whatisthis.sowhat _at_ gmail.com >
 * This is part of "DoorBell_PIC32" . You must have MPLAB HARMONY to use this routines.
 * License : Apache OSS License.
 */


#include "doorbell.h"

extern DOORBELL_DATA doorbellData;

void rtcAlarmSet(uint32_t _sec, bool do_random)
{
    SYS_RTCC_BCD_TIME nexttime;
    if (do_random) {
        int d = (int) SYS_RANDOM_CryptoByteGet();
        int s = (int) _sec;
        s = s + d;
        nexttime = SYS_RTCC_TimeSeconds2BCD(s);
    } else {
        nexttime = SYS_RTCC_TimeSeconds2BCD(_sec);
    }
    SYS_RTCC_AlarmTimeSet(nexttime, true);
}

extern TaskHandle_t xHandleHouseKeeping;
void wakeupTimerCallback(SYS_RTCC_ALARM_HANDLE handle, uintptr_t context)
{
    rtcAlarmSet(ALARM_TICK_SECONDS, false);
	xTaskResume(xHandleHouseKeeping);
    return;
}

#include <stdio.h>
#include <string.h>
static const char *monthStr[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec", "NOP"
};
static const char *dotwStr[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun", "NOP"
};

void getDateTime(SYS_RTCC_BCD_DATE *_nowdate, SYS_RTCC_BCD_TIME *_nowtime)
{
    if (_nowdate != NULL) SYS_RTCC_DateGet(_nowdate);
    if (_nowtime != NULL) SYS_RTCC_TimeGet(_nowtime);
}

void getDateTimeStr(SYS_RTCC_BCD_DATE nowdate, SYS_RTCC_BCD_TIME nowtime, char *buf, int len, bool print_dotw)
{
    uint32_t _dotw, _year, _month, _day;
    uint32_t _hour, _minute, _sec;

    SYS_RTCC_TimeGet(&nowtime);
    SYS_RTCC_DateGet(&nowdate);

    _dotw = nowdate & 0x07;
    _day = ((nowdate & 0x0000f000) >> 12) * 10 + ((nowdate & 0x00000f00) >> 8);
    _month = ((nowdate & 0x00f00000) >> 20) * 10 + ((nowdate & 0x000f0000) >> 16) - 1;
    _year = ((nowdate & 0xf0000000) >> 28) * 10 + ((nowdate & 0x0f000000) >> 24) + 2000;

    _sec = ((nowtime & 0x0000f000) >> 12) * 10 + ((nowtime & 0x00000f00) >> 8);
    _minute = ((nowtime & 0x00f00000) >> 20) * 10 + ((nowtime & 0x000f0000) >> 16);
    _hour = ((nowtime & 0xf0000000) >> 28) * 10 + ((nowtime & 0x0f000000) >> 24);
    memset(buf, 0x00, len);

    if (_month > 12) _month = 0;
    if (_month == 0) _month = 0;
    if (_day > 31) _day = 31;
    if (_day == 0) _day = 1;
    _dotw = _dotw & 7;
    if (print_dotw) {
        snprintf(buf, len, "%s, %d %s %d %d:%d%d", dotwStr[_dotw], _day, monthStr[_month], _year, _hour, _minute, _sec);
    } else {
        snprintf(buf, len, "%d %s %d %d:%d%d", _day, monthStr[_month], _year, _hour, _minute, _sec);
    }
}

static inline bool check_digit(char n)
{
    if (n >= '0') {
        if (n <= '9') return true;
    }
    return false;
}

bool setDateTimeStr(char *_date, char *_time)
{
    if (_date != NULL) {
        // "yyyy/MM/dd DoW hh:mm:ss"
        SYS_RTCC_BCD_DATE ndate = 0;
        // YY
        if (check_digit(_date[2])) {
            ndate |= ((_date[2] - '0') << 8);
        }
        ndate <<= 4;
        if (check_digit(_date[3])) {
            ndate |= ((_date[3] - '0') << 8);
        }
        ndate <<= 4;
        // MM
        if (_date[4] != '/') return false;
        if (check_digit(_date[5])) {
            ndate |= ((_date[5] - '0') << 8);
        }
        ndate <<= 4;
        if (check_digit(_date[6])) {
            ndate |= ((_date[6] - '0') << 8);
        }
        ndate <<= 4;
        // DD
        if (_date[7] != '/') return false;
        if (check_digit(_date[8])) {
            ndate |= ((_date[8] - '0') << 8);
        }
        ndate <<= 4;
        if (check_digit(_date[9])) {
            ndate |= ((_date[9] - '0') << 8);
        }
        //ndate <<= 4;
        // DoW
        char mbuf[8];
        int i;
        _date = &(_date[10]);
        if (_date[0] == ' ') _date++;
        for (i = 0; i < 4; i++) {
            if (_date[i] == 0x00) break;
            if (_date[i] == '\n') break;
            if (_date[i] == ' ') break;
            mbuf[i] = _date[i];
        }
        mbuf[i] = NULL;
        i = 0;
        do {
            if (strcmp(dotwStr[i], "NOP") == 0) break;
            if (strncasecmp(dotwStr[i], mbuf, 8) != 0) continue;
            i++;
        } while (i < 9);
        if (i < 0) i = 0;
        if (i >= 8) i = 0;
        ndate = (ndate & 0xffffff00) | (uint32_t) (i & 0x07);
        SYS_RTCC_DateSet(ndate);
    }
    // Time
    if (_time != NULL) {
        // "yyyy/MM/dd DoW hh:mm:ss"
        SYS_RTCC_BCD_TIME ntime = 0;
        // HH
        if (check_digit(_time[0])) {
            ntime |= ((_time[0] - '0') << 8);
        }
        ntime <<= 4;
        if (check_digit(_time[1])) {
            ntime |= ((_time[1] - '0') << 8);
        }
        ntime <<= 4;
        // mm
        if (_time[2] != ':') return false;
        if (check_digit(_time[3])) {
            ntime |= ((_time[3] - '0') << 8);
        }
        ntime <<= 4;
        if (check_digit(_time[4])) {
            ntime |= ((_time[4] - '0') << 8);
        }
        ntime <<= 4;
        // ss
        if (_time[5] != ':') return false;
        if (check_digit(_time[6])) {
            ntime |= ((_time[6] - '0') << 8);
        }
        ntime <<= 4;
        if (check_digit(_time[7])) {
            ntime |= ((_time[7] - '0') << 8);
        }
        //ntime <<= 4;
        SYS_RTCC_TimeSet(ntime, true);
    }
    return true;
}
