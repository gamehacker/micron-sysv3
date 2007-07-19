/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_IO_H__
#define __MICRON_KERNEL_IO_H__

extern unsigned char inportb(unsigned short port);
extern void outportb(unsigned short port, unsigned char data);

#endif

