/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_CONFIG_H__
#define __MICRON_KERNEL_CONFIG_H__

/*
 * This file contains the limits of micron kernel, may be different from
 * architecture to architecture, and should be changed here.
 */

#define NCHRDEVS	256	// Maximum number of char devices
#define NBLKDEVS	256	// Maximum number of block devices
#define NESYMLEN	20	// Maximum export symbol name length

extern unsigned arch_memsize;	/* architecture memory size */
extern void arch_disablei();	/* disable all interrupts */
extern void arch_enablei();	/* enable all interrupts */

#endif

