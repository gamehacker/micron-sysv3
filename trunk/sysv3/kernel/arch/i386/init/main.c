/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <config.h>
#include <libc.h>

void kmain()
{
	SYSTEM("Total System Memory: %d KB\n", arch_memsize/1024);
	kprintf("\nWelcome to Micron System Version 3\n\n");
}

