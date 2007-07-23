/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <io.h>

void i386_rtc_freq(unsigned freq)
{
	unsigned div = 1193180/freq;
	outport(0x43, 0x36);
	outport(0x40, div & 0xFF);
	outport(0x40, div>>8);
}

void i386_rtc_init()
{
	i386_rtc_freq(1000);
}

