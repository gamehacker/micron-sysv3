/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_IRQ_H__
#define __MICRON_KERNEL_IRQ_H__

struct Register
{
	unsigned int gs, fs, es, ds, ss;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int intn, err;
	unsigned int eip, cs, eflags, useresp, userss; // only valid from ring 3
};

extern void isr_install(int index, void (*)(struct Register*));
extern void irq_install(int index, void (*)(struct Register*));
extern void isr_install_syscall(void (*)(struct Register*));

#endif

