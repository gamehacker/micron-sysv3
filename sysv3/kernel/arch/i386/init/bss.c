/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

extern unsigned bssend;
extern unsigned bss;

void bss_init()
{
	unsigned *bssp = &bss;
	while(bssp < &bssend) {
		*bssp++ = 0;
	}
	*bssp = 0;
}

