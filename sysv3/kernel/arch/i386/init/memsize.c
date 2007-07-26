/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <config.h>

extern unsigned kernel_end;

void i386_mem_size()
{
	unsigned *tp, tmp, memsize;

	memsize = (unsigned)&kernel_end;

	do {
		tp  = (unsigned*)memsize;
		tmp = *tp;
		*tp = 0x55AA55AA;
		asm("":::"memory");
		if(*tp != 0x55AA55AA) {
			break;
		} else {
			*tp = 0xAA55AA55;
			asm("":::"memory");
			if(*tp != 0xAA55AA55) {
				break;
			}
		}
		*tp = tmp;
		asm("":::"memory");
		memsize+= 0x1000;
	} while(memsize < 0xFFFFFFFF);

	arch_memsize = memsize;
}

