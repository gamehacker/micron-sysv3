/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

#ifndef __MICRON_KERNEL_IDT_H__
#define __MICRON_KERNEL_IDT_H__


enum idt_type
{
	TASK = 5,		/* Task Gate */
	INTR = 6,		/* Interrupt Gate */
	TRAP = 7		/* Trap Gate */
};

enum idt_bits
{
	BITS16,			/* 16 bits gate */
	BITS32			/* 32 bits gate */
};

void idt_setup(unsigned index, unsigned sel, unsigned offset, unsigned p, unsigned dpl,
    enum idt_type type, enum idt_bits size);

#endif //__MICRON_KERNEL_IDT_H__
