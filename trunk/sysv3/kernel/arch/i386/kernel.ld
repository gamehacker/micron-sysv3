/*****************************************************************************
 * Micron System V3 - i386 Processor Driver Build Sub-system
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

kernel_begin = 0x100000;
page_align = 0x1000;

SECTIONS
{
	.text kernel_begin : AT(kernel_begin)
	{
		code = . ;
		*(.text)
		. = ALIGN(page_align);
	}
	.data : AT(kernel_begin + (data - code))
	{
		data = . ;
		*(.data)
		. = ALIGN(page_align);
	}
	.inittab : AT(kernel_begin + (inittab - code))
	{
		inittab = . ;
		*(.inittab)
		. = ALIGN(page_align);
	}
	.exittab : AT(kernel_begin + (exittab - code))
	{
		exittab = . ;
		*(.exittab)
		. = ALIGN(page_align);
	}
	.exporttab : AT(kernel_begin + (exporttab - code))
	{
		exporttab = . ;
		*(.exporttab)
		. = ALIGN(page_align);
	}
	.bss : AT(kernel_begin + (bss - code))
	{
		bss = . ;
		*(.bss)
		. = ALIGN(page_align);
	}
	kernel_end = . ;
}
