/* 
 * File:   logger.h
 * Author: whatisthis
 *
 * Created on 2017/09/19, 5:21
 */

#ifndef LOGGER_H
#define	LOGGER_H

#ifdef	__cplusplus
extern "C" {
#endif

	extern void pushLog(SYS_RTCC_BCD_DATE _date, SYS_RTCC_BCD_TIME _time, uint8_t _type, uint8_t *_data, uint8_t _len);
	extern void printLog(int index, char *head, char *str, uint8_t _type, uint8_t *rawdata, uint8_t _rawlen);
	extern void printMessage(int index, char *head, char *str);

#ifdef	__cplusplus
}
#endif

#endif	/* LOGGER_H */

