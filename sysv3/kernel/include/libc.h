/*****************************************************************************
 * Micron System Version 3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_LIBC_H__
#define __MICRON_KERNEL_LIBC_H__

#include <types.h>

extern char *itoa(int value, char *str, int radix);
extern int strlen(char *str);
extern char *memcpy(char *dest, char *src, size_t cnt);
extern int __attribute__((format(printf, 1, 2))) kprintf(char *fmt, ...);

/* debug functions */
#define PANIC(cond, str...) \
		if(cond) {\
			kprintf("%CPANIC: %s: ", 0x0C, #cond); \
			kprintf(str); \
			kprintf("\n%C", 0x0F); \
			while(1); \
		}
#define DEBUG(data)      kprintf("%CDEBUG:%s= %d\n%C",0x0C,#data,data,0x0F);

#endif

