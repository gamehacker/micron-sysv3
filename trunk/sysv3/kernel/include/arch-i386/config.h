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
#define NBLKENTS	8	// Maximum number of block device entries

#define NESYMLEN	20	// Maximum export symbol name length

#define NMIDSLEN	8	// Maximum module ID string length
#define NMDESLEN	20	// Maximum module description string length

#define NMXBLKSZ	4096	// Maximum block size supported
#define NVNAMLEN	68	// Maximum node name string length
#define NVFSMNTS	8	// Maximum number of mount points
#define NFSENTYS	5	// File system support install points
#define NVCHCNTS	4096	// Number of vnodes inside vcache
#define NSECACHS	512	// Number of sector caches
#define NPATHSTR	512	// Maximum path string length
#define NMXOFILE	128	// Maximum number of open files

/*
 * Various device major device IDs
 */

/* Char devices */
#define CHR_TTY		0

/* Block devices */
#define BLK_HDA		0

/*
 * Device driver I/O base definitions
 */
#define BLK_HDD_IOBASE	0x01F0

//Note: setting rtc frequency to 1000 is not stable when running in virtual machines.
#define RTC_FREQUENCY 100

#endif

