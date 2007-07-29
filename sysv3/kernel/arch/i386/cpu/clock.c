/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <io.h>
#include <irq.h>

void rtc_intr(struct Register *regs)
{
}

void rtc_freq(unsigned freq)
{
	unsigned div = 1193180/freq;
	outportb(0x43, 0x36);
	outportb(0x40, div & 0xFF);
	outportb(0x40, div>>8);
}

void rtc_init()
{
	rtc_freq(1000);
	irq_install(0, rtc_intr);
	// TODO: REMOVE THIS !!!
	irq_install(7, rtc_intr);
}

