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
	int (*rdwr )(id_t id, mode_t mode, char *buf, blkcnt_t sectors);
	int (*ioctl)(id_t id, int cmd, int arg);
};

#endif

