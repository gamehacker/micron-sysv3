/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_EXPORT_H__
#define __MICRON_KERNEL_EXPORT_H__

#include <config.h>

/* The symbol exporting function */
#define EXPORT_SYMBOL(sym) \
	const char exporttabc_name_##sym[NESYMLEN] \
	__attribute__((section(".exporttab"))) = #sym; \
	const int  exporttabc_value_##sym \
	__attribute__((section(".exporttab"))) = (int) sym;

/* Export Symbol database entry type */
struct _exportitem
{
	char name[NESYMLEN];
	int  value;
};

#endif

