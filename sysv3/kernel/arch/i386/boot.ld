/*****************************************************************************
 * Micron System V3 - i386 Processor Driver Build Sub-system
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

boot_begin = 0x7c00;

SECTIONS
{
	.text boot_begin : AT(boot_begin)
	{
		code = . ;
		*(.text)
	}
	.data : AT(boot_begin + (data - code))
	{
		data = . ;
		*(.data)
	}
	.bss : AT(boot_begin + (bss - code))
	{
		bss = . ;
		*(.bss)
	}
}
