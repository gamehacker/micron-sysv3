/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_MODULE_H__
#define __MICRON_KERNEL_MODULE_H__

/* Here are the module automatic init and exit macros */
#define MODULE_INIT(sym) \
	const int inittabc_##sym __attribute__((section(".inittab"))) \
	= (int) sym;

#define MODULE_EXIT(sym) \
	const int exittabc_##sym __attribute__((section(".exittab"))) \
	= (int) sym;

/* Module init and exit invokers */
extern int modinit();
extern int modexit();

#endif
