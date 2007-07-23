/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_IRQ_H__
#define __MICRON_KERNEL_IRQ_H__

struct isr_regs
{
	unsigned int gs, fs, es, ds, ss;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int intn, err;
	unsigned int eip, cs, eflags, useresp, userss; // only valid from ring 3
};

extern void i386_isr_install(int index, void (*)(struct isr_regs*));
extern void i386_irq_install(int index, void (*)(struct isr_regs*));
extern void i386_isr_install_syscall(void (*)(struct isr_regs*));

#endif

