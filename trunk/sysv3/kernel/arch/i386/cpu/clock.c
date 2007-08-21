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
#include <time.h>

#define BCD2HEXB(bcd)	(bcd&0xf)+((bcd>>4)&0xf)*10

//global variables, they may be accessed by another file
unsigned rtc_time;      //unit: ms
unsigned rtc_second;    //unit: s

void rtc_freq(unsigned freq)
{
	unsigned div = 1193180/freq;
	outportb(0x43, 0x36);
	outportb(0x40, div & 0xFF);
	outportb(0x40, div>>8);
}

void rtc_intr(struct Register *regs)
{
	rtc_time ++;
	if( !(rtc_time % 1000) ){
        //printf(".");  //Let's see how long dose a minute last?
        rtc_second ++;
	}
}

void rtc_init()
{
	struct tm t;
	char timestr[48];

	/* initialize RTC */
	//rtc_freq(1000);
	irq_install(0, rtc_intr);

	/* initialize system time */
	outportb(0x70, 0);	/* seconds */
	t.tm_sec = BCD2HEXB(inportb(0x71));
	outportb(0x70, 2);	/* minutes */
	t.tm_min = BCD2HEXB(inportb(0x71));
	outportb(0x70, 4);	/* hours */
	t.tm_hour = BCD2HEXB(inportb(0x71));
	outportb(0x70, 7);	/* date of month */
	t.tm_mday = BCD2HEXB(inportb(0x71));
	outportb(0x70, 8);	/* month */
	t.tm_mon = BCD2HEXB(inportb(0x71));
	outportb(0x70, 9);	/* year */
	t.tm_year = BCD2HEXB(inportb(0x71));
	t.tm_mon --;
	/* calculate to time calculated in ms from 1970-01-01 */
	rtc_second = mktime( &t );
	rtc_time = rtc_second * 1000;
	// Show time
	strtime( &rtc_second, timestr );
	SYSTEM("System Initialization Start: %s\n", timestr );
}

