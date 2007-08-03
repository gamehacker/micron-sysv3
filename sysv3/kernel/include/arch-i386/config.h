/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Note to architecture porters:
 *   This file contains the critical section of code which describes a new
 * micron architecture.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_CONFIG_H__
#define __MICRON_KERNEL_CONFIG_H__

/* All architecture should implement the following function */

extern unsigned arch_memsize;	/* architecture memory size */
extern void arch_disablei();	/* disable all interrupts */
extern void arch_enablei();	/* enable all interrupts */

/*
 * This file contains the limits of micron kernel, may be different from
 * architecture to architecture, and should be changed here.
 */

#define NCHRDEVS	16	// Maximum number of char devices
#define NBLKDEVS	16	// Maximum number of block devices
#define NESYMLEN	20	// Maximum export symbol name length
#define NMIDSLEN	8	// Maximum module ID string length
#define NMDESLEN	20	// Maximum module description string length
#define NPARTION	8	// Number of partitions in HDD
#define NINODES		4096	// Vnode cache items

/*
 * Various device major device IDs
 */

/* Char devices */
#define CHR_TTY		0

/* Block devices */
#define BLK_HDD		0
#define BLK_HDDP	1

/*
 * Device driver I/O base definitions
 */
#define BLK_HDD_IOBASE	0x01F0

#endif

