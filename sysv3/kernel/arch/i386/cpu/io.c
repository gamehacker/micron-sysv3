/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************
 * References:
 *   Bran's Kernel Development Tutorial
 *****************************************************************************/
#include <io.h>

unsigned char inport(unsigned short port)
{
	unsigned char rv;
	__asm__ __volatile__ ("inb %1, %0" : "=a"(rv) : "dN"(port));
	return rv;
}

void outport(unsigned short port, unsigned char data)
{
	__asm__ __volatile__ ("outb %1, %0"::"dN"(port), "a"(data));
}

