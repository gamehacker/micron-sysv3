/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

#ifndef __MICRON_KERNEL_INIT_H__
#define __MICRON_KERNEL_INIT_H__

#define MAX_EXPSYMLEN 20	// Maxium length of supported export symbol

#define MODULE_INIT(sym) \
	const int __inittab_##sym __attribute__((section(".inittab"))) \
	= (int) sym;

#define MODULE_EXIT(sym) \
	const int __exittab_##sym __attribute__((section(".exittab"))) \
	= (int) sym;

#define EXPORT_SYMBOL(sym) \
	const char __exporttab_name_##sym[MAX_EXPSYMLEN] __attribute__((section(".exporttab"))) \
	=#sym; \
	const int __exporttab_value_##sym __attribute__((section(".exporttab"))) \
	=(int)sym;

struct _exportitem
{
	char name[MAX_EXPSYMLEN];
	int  value;
};

extern int modinit();
extern int modexit();

#endif
