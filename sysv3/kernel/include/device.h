/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_DEVICES_H__
#define __MICRON_KERNEL_DEVICES_H__

#include <config.h>
#include <types.h>
#include <module.h>
#include <vfs.h>
#include <export.h>
#include <errno.h>
#include <chrdev.h>
#include <blkdev.h>
/* DEVICE TODO: Add other device type headers here */

/* Device type enumeration */
enum dev_type
{
	CHRDEV,
	BLKDEV
	/* DEVICE TODO: Add other device type names here */
};

/* Device descriptor holder */
extern struct dev_chr dev_chr[NCHRDEVS];
extern struct dev_blk dev_blk[NBLKDEVS];

/* Macros to deal with device IDs */
#define DEVID(major, minor)	((major&0xffff)<<16)|(minor&0xffff)
#define MAJOR(dID)		((dID>>16)&0xffff)
#define MINOR(dID)		(dID&0xffff)

/* Lseek whence value */
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* Device operation method function interface */
extern void  dev_init();
extern int   dev_open (enum dev_type type, id_t dID, int oflag, mode_t mode);
extern int   dev_close(enum dev_type type, id_t dID);
extern int   dev_lseek(enum dev_type type, id_t dID, off_t off, int whence);
extern int   dev_read (enum dev_type type, id_t dID, char *buf, off_t cnt);
extern int   dev_write(enum dev_type type, id_t dID, char *buf, off_t cnt);
extern int   dev_ioctl(enum dev_type type, id_t dID, int cmd, int arg);
/* DEVICE TODO: Add new device model operations here */

#endif

