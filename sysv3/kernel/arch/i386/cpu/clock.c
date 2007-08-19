/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <io.h>
#include <irq.h>
#include <libc.h>
#include <types.h>

#define BCD2HEXB(bcd)	(bcd&0xf)+((bcd>>4)&0xf)*10

unsigned char rtc_cal[12] = {
	31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

unsigned rtc_time;

unsigned rtc_date2sec(unsigned year, unsigned month, unsigned dayofmon,
		      unsigned hour, unsigned min, unsigned sec)
{
	unsigned rslt=0, i;

	/* convert year to day */
	rslt += year*365;

	/* if the year have 366 day */
	for(i=0; i<year; i++) {
		if(i%4==0) {
			rslt++;
		}
	}

	/* convert days in this year */
	for(i=1; i<month; i++) {
		rslt += rtc_cal[i-1];
	}
	rslt += dayofmon;

	/* convert to hours */
	rslt *= 24;

	/* convert to minutes */
	rslt *= 60;

	/* convert to seconds */
	rslt *= 60;

	return rslt;
}

void rtc_freq(unsigned freq)
{
	unsigned div = 1193180/freq;
	outportb(0x43, 0x36);
	outportb(0x40, div & 0xFF);
	outportb(0x40, div>>8);
}

void rtc_intr(struct Register *regs)
{
	rtc_time++;
}

void invalid(struct Register *regs)
{
}

time_t time(time_t *tloc)
{
	if(tloc != 0) {
		*tloc = rtc_time/1000;
	}
	return rtc_time/1000;
}

void rtc_init()
{
	unsigned char second, minute, hour, day, month, year;
	unsigned y1972 = 0;

	/* initialize RTC */
	rtc_freq(1000);
	irq_install(0, rtc_intr);

	/* initialize system time */
	outportb(0x70, 0);	/* seconds */
	second = BCD2HEXB(inportb(0x71));
	outportb(0x70, 2);	/* minutes */
	minute = BCD2HEXB(inportb(0x71));
	outportb(0x70, 4);	/* hours */
	hour = BCD2HEXB(inportb(0x71));
	outportb(0x70, 7);	/* date of month */
	day = BCD2HEXB(inportb(0x71));
	outportb(0x70, 8);	/* month */
	month = BCD2HEXB(inportb(0x71));
	outportb(0x70, 9);	/* year */
	year = BCD2HEXB(inportb(0x71));
	SYSTEM("System Initialization Start: %d-%d-%d %d:%d:%d\n", year+2000, 
		month, day, hour, minute, second);

	/* calculate to time calculated in ms from 2000-01-01 */
	y1972 = rtc_date2sec(1970, 1, 1, 0, 0, 0);
	rtc_time = rtc_date2sec(year+2000, month, day, hour, minute, second) - y1972;
	rtc_time *= 1000;

	// TODO: REMOVE THIS !!! i don't know why this happen, this is a way
	// to get around it
	irq_install(7, invalid);
}

