/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_MOUNT_H__
#define __MICRON_KERNEL_MOUNT_H__

#include <config.h>
#include <types.h>
#include <fs.h>

struct mountab
{
	char stat;			/* mount point status */
	char sblk[NMXBLKSZ];		/* super block of mounted fs */
	blkcnt_t blksize;		/* device block size */
	enum fstype fs;			/* file system type */
	dev_t hdev;			/* device ID of host device */
	ino_t hino;			/* inode ID of host device */
	struct vnode root;		/* root node of fs */
};

/* data */
extern struct mountab mountab[NVFSMNTS];

/* operations */
extern struct mountab *mountab_getroot();	/* mountab[0] is root */
extern struct mountab *mountab_get(dev_t dev);
extern struct mountab *mountab_seek(dev_t hdev, ino_t hino);
extern int mountab_mount(dev_t hdev, ino_t hino, dev_t dev);
extern int mountab_umount(dev_t dev);

#endif

