//
// Created by Administrator on 2025-12-29.
//

#ifndef CLIONTEST_RTC_H
#define CLIONTEST_RTC_H

#include "../sys/sys.h"
/* 时间结构体, 包括年月日周时分秒等信息 */
typedef struct
{
    uint8_t hour;       /* 时 */
    uint8_t min;        /* 分 */
    uint8_t sec;        /* 秒 */
    /* 公历年月日周 */
    uint16_t year;      /* 年 */
    uint8_t  month;     /* 月 */
    uint8_t  date;      /* 日 */
    uint8_t  week;      /* 周 */
} calendar_obj;
extern calendar_obj g_calendar;

uint8_t rtc_init(void);
uint32_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);
calendar_obj rtc_get_time(void);
void rtc_write_bkr(uint32_t bkrx, uint16_t data);
uint16_t rtc_read_bkr(uint32_t bkrx);


long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* 将年月日时分秒转换成秒钟数 */
calendar_obj sec2rtc_date(uint32_t seccount);

#endif //CLIONTEST_RTC_H