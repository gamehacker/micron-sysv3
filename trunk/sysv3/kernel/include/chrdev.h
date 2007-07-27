/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_CHRDEV_H__
#define __MICRON_KERNEL_CHRDEV_H__

#include <config.h>
#include <types.h>

/* Character Device I/O Interface */
struct ChrDev
{
	dev_t major;
	int (*open )(id_t id, int oflag, mode_t mode);
	int (*close)(id_t id);
	int (*read )(id_t id, char *buf, off_t cnt);
	int (*write)(id_t id, char *buf, off_t cnt);
	int (*ioctl)(id_t id, int cmd, int arg);
};

#endif

