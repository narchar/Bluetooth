#include "DS3231.h"

static uint8_t bcdtodec(const uint8_t val);
static uint8_t dectobcd(const uint8_t val);

void DS3231_Init(const uint8_t ctrl_reg)
{
    DS3231_Set_Creg(ctrl_reg);
}

void DS3231_Set_Creg(const uint8_t val)
{
    DS3231_Set_Addr(DS3231_CONTROL_ADDR, val);
}

void DS3231_Set_Addr(const uint8_t addr, const uint8_t val)
{
	uint8_t buff[2];
	buff[0] = addr;
	buff[1] = val;
	
	I2C_WriteBuff(DS3231_I2C_ADDR,addr,2,buff);
}


void DS3231_Get(struct ts *t)
{
    uint8_t TimeDate[7];        //second,minute,hour,dow,day,month,year
    uint8_t century = 0;
    uint8_t i, n;
    uint16_t year_full;
	  uint8_t buff[10];

		buff[0] = DS3231_TIME_CAL_ADDR;
		I2C_ReadBuff(DS3231_I2C_ADDR,buff[0],7,buff);
	
    for (i = 0; i <= 6; i++) {
        n = buff[i];
        if (i == 5)
				{
            TimeDate[5] = bcdtodec(n & 0x1F);
            century = (n & 0x80) >> 7;
        } 
				else
				{
            TimeDate[i] = bcdtodec(n);
				}
    }		
		

    if (century == 1) {
        year_full = 2000 + TimeDate[6];
    } else {
        year_full = 1900 + TimeDate[6];
    }

    t->sec = TimeDate[0];
    t->min = TimeDate[1];
    t->hour = TimeDate[2];
    t->mday = TimeDate[4];
    t->mon = TimeDate[5];
    t->year = year_full;
    t->wday = TimeDate[3];
    t->year_s = TimeDate[6];
#ifdef CONFIG_UNIXTIME
    t->unixtime = get_unixtime(*t);
#endif
}


void DS3231_Set(struct ts t)
{
    uint8_t i, century;
	  uint8_t buff[9];
		uint8_t TimeDate[7];
	
    if (t.year > 2000) {
        century = 0x80;
        t.year_s = t.year - 2000;
    } else {
        century = 0;
        t.year_s = t.year - 1900;
    }

		TimeDate[0] = t.sec;
		TimeDate[1] = t.min;
		TimeDate[2] = t.hour;
		TimeDate[3] = t.wday;
		TimeDate[4] = t.mday;
		TimeDate[5] = t.mon;
		TimeDate[6] = t.year_s;
		
		for(i = 0; i < 7; i++)
		{
			TimeDate[i] = dectobcd(TimeDate[i]);
			if(i == 5)
				TimeDate[5] += century;
		}
		
		buff[8] = DS3231_TIME_CAL_ADDR;
		for(i = 0; i < 7; i++)
		{
			buff[i] = TimeDate[i];
		}
		
		I2C_WriteBuff(DS3231_I2C_ADDR, buff[8], 7, buff);
}

static uint8_t bcdtodec(const uint8_t val)
{
    return ((val / 16 * 10) + (val % 16));
}

static uint8_t dectobcd(const uint8_t val)
{
    return ((val / 10 * 16) + (val % 10));
}
