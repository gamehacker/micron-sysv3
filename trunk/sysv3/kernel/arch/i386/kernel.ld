/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
OUTPUT_FORMAT(elf32-i386)
ENTRY(_start)

kernel_begin = 0x100000;
stack_size   = 0x1000;

SECTIONS
{
	.text kernel_begin : AT(kernel_begin)
	{
		code = . ;
		*(.text)
	}
	.data : AT(kernel_begin + (data - code))
	{
		data = . ;
		*(.data)
	}
	.inittab : AT(kernel_begin + (inittab - code))
	{
		inittab = . ;
		*(.inittab)
	}
	.exittab : AT(kernel_begin + (exittab - code))
	{
		exittab = . ;
		*(.exittab)
	}
	.exporttab : AT(kernel_begin + (exporttab - code))
	{
		exporttab = . ;
		*(.exporttab)
	}
	.bss : AT(kernel_begin + (bss - code))
	{
		bss = . ;
		*(.bss)
	}
	.stack : AT(kernel_begin +(stack_end - code))
	{
		stack_end = . ;
		stack = stack_end + stack_size;
	}
	kernel_end = . ;
}

