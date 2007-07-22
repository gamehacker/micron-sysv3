/*****************************************************************************
 * Micron System Version 3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_LIBC_H__
#define __MICRON_KERNEL_LIBC_H__

extern char *itoa(int value, char *str, int radix);
extern int strlen(char *str);
extern int __attribute__((format(printf, 1, 2))) kprintf(char *fmt, ...);

#endif

