/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

kernel_begin = 0x100000;
page_size    = 0x1000;
stack_size   = 0x1000;

SECTIONS
{
	.text kernel_begin : AT(kernel_begin)
	{
		code = . ;
		*(.text)
		*(.rodata.str1.1)
		*(.rodata.str1.4)
		*(.rodata.*)
		. = ALIGN(page_size);
	}
	.data : AT(kernel_begin + (data - code))
	{
		data = . ;
		*(.data)
		*(.rodata)
		. = ALIGN(page_size);
	}
	.inittab : AT(kernel_begin + (inittab - code))
	{
		inittab = . ;
		*(.inittab)
		. = ALIGN(page_size);
		inittab_end = . ;
	}
	.exittab : AT(kernel_begin + (exittab - code))
	{
		exittab = . ;
		*(.exittab)
		. = ALIGN(page_size);
		exittab_end = . ;
	}
	.exporttab : AT(kernel_begin + (exporttab - code))
	{
		exporttab = . ;
		*(.exporttab)
		. = ALIGN(page_size);
		exporttab_end = . ;
	}
	.bss : AT(kernel_begin + (bss - code))
	{
		bss = . ;
		*(.bss)
		. = ALIGN(page_size);
	}
	.stack : AT(kernel_begin +(stack_end - code))
	{
		stack_end = . ;
		stack = stack_end + stack_size;
	}
	. = ALIGN(page_size);
	kernel_end = . ;
}

