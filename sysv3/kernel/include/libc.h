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
extern char *strchr(char *s, int c);
extern char *strcpy(char *str1, char *str2);
extern int strlen (char *str);
extern int strcmp (char *str1, char *str2);
extern int strncmp(char *str1, char *str2, size_t n);
extern void *memcpy(void *dest, void *src, size_t cnt);
extern void *memset(void *dest, int ch, size_t cnt);
extern int __attribute__((format(printf, 1, 2))) kprintf(char *fmt, ...);

/* debug functions */
#define PANIC(cond, str...) \
	if(cond) {\
		kprintf("%C[PANIC]: %s:%d: ", 0x0C, __FILE__, __LINE__); \
		kprintf(str); \
		kprintf("\n%C", 0x0F); \
		while(1); \
	}

#define MSG(cond, str...) \
	if(cond) { \
		kprintf("%C[MESSAGE]: %s:%d: ", 0x0E, __FILE__, __LINE__); \
		kprintf(str); \
		kprintf("\n%C", 0x0F); \
	}

#define DEBUG(data) \
	kprintf("%CDEBUG:%s= 0x%x\n%C",0x0E,#data,(unsigned)data,0x0F);

#define SYSTEM(fmt, arg...) \
	kprintf("%C[SYSTEM]%C:", 0x0E, 0x0F); \
	kprintf(fmt, ##arg);


#endif

