#include "doorbell.h"

extern DOORBELL_DATA doorbellData;

void CALC_MD5Sum(void)
{
    unsigned int datasize = sizeof (DOORBELL_REAL_DATA_T);
    CRYPT_MD5_Initialize(&(doorbellData.md5_context));
    CRYPT_MD5_DataSizeSet(&(doorbellData.md5_context), datasize);
    CRYPT_MD5_DataAdd(&(doorbellData.md5_context), (const unsigned char*) &(doorbellData.realdata), datasize);
    CRYPT_MD5_Finalize(&(doorbellData.md5_context), doorbellData.data_md5sum);
}

bool CHECK_MD5Sum(void)
{
    unsigned char tmpmd5[MD5_DIGEST_SIZE];
    unsigned int datasize = sizeof (DOORBELL_REAL_DATA_T);
    CRYPT_MD5_Initialize(&(doorbellData.md5_context));
    CRYPT_MD5_DataSizeSet(&(doorbellData.md5_context), datasize);
    CRYPT_MD5_DataAdd(&(doorbellData.md5_context), (const unsigned char*) &(doorbellData.realdata), datasize);
    CRYPT_MD5_Finalize(&(doorbellData.md5_context), tmpmd5);
    if (memcmp(tmpmd5, doorbellData.data_md5sum, MD5_DIGEST_SIZE) != 0) {
        return false;
    }
    return true;
}

void ledTimerCallback(uintptr_t context, uint32_t currTick)
{
    DOORBELL_TIMER_TICK_T *p = (DOORBELL_TIMER_TICK_T *) context;
    bool b;
    p->toggle_status = !(p->toggle_status);
    b = p->toggle_status;
    switch (p->num) {
    case 0:
        SYS_PORTS_PinWrite(PORTS_ID_0, PORT_CHANNEL_B, 14, b);
        break;
    default:
        break;
    }
}

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

void wakeupTimerCallback(SYS_RTCC_ALARM_HANDLE handle, uintptr_t context)
{
    rtcAlarmSet(ALARM_TICK_SECONDS, false);
    doorbellData.state = DOORBELL_STATE_REQ_TEMP1;
    return;
}

#include <stdio.h>
#include <string.h>
static const char *monthStr[] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};
static const char *dotwStr[] = {
    "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"
};

void getDateTime(char *buf, int len, bool print_dotw)
{
    SYS_RTCC_BCD_TIME nowtime;
    SYS_RTCC_BCD_DATE nowdate;
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

void printLog(const SYS_MODULE_INDEX cons_index, char *head, char *str)
{
    char buf[1024];
    char daybuf[256];

    memset(buf, 0x00, sizeof (buf));
    getDateTime(daybuf, sizeof (daybuf), false);

    if (head != NULL) {
        snprintf(buf, 1024, "[%s] %s: %s\n", head, daybuf, str);
    } else {
        snprintf(buf, 1024, "[%s] %s: %s\n", doorbellData.realdata.uniqueName, daybuf, str);
    }
    SYS_CONSOLE_Write(cons_index, STDOUT_FILENO, buf, strlen(buf));
}

void printThermalLMT01(const SYS_MODULE_INDEX cons_index, int index, uint32_t temp)
{
    char buf[256];
    int itemp;
    float ftemp;

    ftemp = (float) ((temp & 0x1fff) << 8) - (50.0 * 4096.0);
    ftemp = ftemp / 4096.0;

    snprintf(buf, sizeof (buf), "[TEMP%d] %3f ", index, ftemp);
    printLog(cons_index, NULL, buf);
}