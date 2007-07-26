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
#include <export.h>
#include <errno.h>
#include <chrdev.h>
/* DEVICE TODO: Add other device type headers here */

/* Device type enumeration */
enum DevType
{
	CHRDEV
	/* DEVICE TODO: Add other device type names here */
};

/* Macros to deal with device IDs */
#define DEVID(major, minor)	((major&0xffff)<<16)|(minor&0xffff)
#define MAJOR(dID)		(dID>>16)&0xffff
#define MINOR(dID)		dID&0xffff

/* Device descriptor holder */
struct ChrDev ChrDev[NCHRDEVS];

/* Device operation method function interface */
extern void  DeviceInit ();
extern int   DeviceOpen (enum DevType type, id_t dID, int oflag, mode_t mode);
extern int   DeviceClose(enum DevType type, id_t dID);
extern int   DeviceRead (enum DevType type, id_t dID, char *buf, size_t cnt);
extern int   DeviceWrite(enum DevType type, id_t dID, char *buf, size_t cnt);
extern int   DeviceIoctl(enum DevType type, id_t dID, int cmd, int arg);
/* DEVICE TODO: Add new device model operations here */

#endif

