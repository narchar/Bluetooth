#ifndef __DS3231_H
#define __DS3231_H

#include "LIB_Config.h"

// i2c slave address of the DS3231 chip
#define DS3231_I2C_ADDR             (0x68 << 1)

#define SECONDS_FROM_1970_TO_2000  946684800

// timekeeping registers
#define DS3231_TIME_CAL_ADDR        0x00
#define DS3231_ALARM1_ADDR          0x07
#define DS3231_ALARM2_ADDR          0x0B
#define DS3231_CONTROL_ADDR         0x0E
#define DS3231_STATUS_ADDR          0x0F
#define DS3231_AGING_OFFSET_ADDR    0x10
#define DS3231_TEMPERATURE_ADDR     0x11

// control register bits
#define DS3231_A1IE     0x1
#define DS3231_A2IE     0x2
#define DS3231_INTCN    0x4

// status register bits
#define DS3231_A1F      0x1
#define DS3231_A2F      0x2
#define DS3231_OSF      0x80

struct ts {
    uint8_t sec;         /* seconds */
    uint8_t min;         /* minutes */
    uint8_t hour;        /* hours */
    uint8_t mday;        /* day of the month */
    uint8_t mon;         /* month */
    int16_t year;        /* year */
    uint8_t wday;        /* day of the week */
    uint8_t yday;        /* day in the year */
    uint8_t isdst;       /* daylight saving time */
    uint8_t year_s;      /* year in short notation*/
#ifdef CONFIG_UNIXTIME
    uint32_t unixtime;   /* seconds since 01.01.1970 00:00:00 UTC*/
#endif
};

void DS3231_Init(const uint8_t creg);
void DS3232_Stop(void);
void DS3231_Set(struct ts t);
void DS3231_Get(struct ts *t);

void DS3231_Set_Creg(const uint8_t val);
void DS3231_Set_Addr(const uint8_t addr, const uint8_t val);

#endif	//__DS3231_H
