/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_GDT_H__
#define __MICRON_KERNEL_GDT_H__

#include <types.h>

struct tss {
	u32_t	back_link;
	u32_t	esp0;
	u32_t	ss0;
	u32_t	esp1;
	u32_t	ss1;
	u32_t	esp2;
	u32_t	ss2;
	u32_t	cr3;
	u32_t	eip;
	u32_t	flags;
	u32_t	eax, ecx ,edx, ebx;
	u32_t	esp;
	u32_t	ebp;
	u32_t	esi;
	u32_t	edi;
	u32_t	es;
	u32_t	cs;
	u32_t	ss;
	u32_t	ds;
	u32_t	fs;
	u32_t	gs;
	u32_t	ldt;
	u16_t	trap;
	u16_t	iobase;
//	u8_t	iomap[2];
};

extern void gdt_tss(unsigned tadd, unsigned size);

#endif

