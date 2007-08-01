/*****************************************************************************
 * Micron System Version 3 - System Core
 * Copyright (C) 2007, Martin Tang
 * Copyright (C) 2007, Micron System Team
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_SYSTEM_BLKDRV_H__
#define __MICRON SYSTEM_BLKDRV_H__

#include <config.h>
#include <types.h>

struct BlkDev
{
	int (*open )(id_t id, int oflag, mode_t mode);
	int (*close)(id_t id);	
	int (*read )(id_t id, char *buf, off_t cnt);
	int (*write)(id_t id, char *buf, off_t cnt);
	int (*lseek)(id_t id, off_t offset, int whence);
	int (*ioctl)(id_t id, int cmd, int arg);
};

#endif

