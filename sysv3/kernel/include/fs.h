/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_VFS_H__
#define __MICRON_KERNEL_VFS_H__

#include <config.h>
#include <icache.h>

enum vfs_type
{
	MFS
};

/* vfs structure */
struct vfs
{
	/* identifications */
	enum vfs_type vfs;

	/* operations */
	int (*lschild)(ino_t vid, ino_t id);
	int (*mknode )(ino_t id, char *name, mode_t mode, dev_t dev);
	int (*rmnode )(ino_t id, char *name);
	int (*lseek  )(ino_t id, off_t offset, int whence);
	int (*read   )(ino_t id, char *buf, size_t nbyte);
	int (*write  )(ino_t id, char *buf, size_t nbyte);
};

/* vfs entries */
extern struct vfs vfs[NVFSENTS];

/* vfs layer operations */
extern int vfs_lschild(enum vfs_type vfs, ino_t vid, ino_t id);
extern int vfs_mknode (enum vfs_type vfs, ino_t id, char *name, 
						mode_t mode, dev_t dev);
extern int vfs_rmnode (enum vfs_type vfs, ino_t id, char *name);
extern int vfs_lseek  (enum vfs_type vfs, ino_t id, off_t offset, int whence);
extern int vfs_read   (enum vfs_type vfs, ino_t id, char *buf, size_t nbyte);
extern int vfs_write  (enum vfs_type vfs, ino_t id, char *buf, size_t nbyte);

/* File system operations */
extern ino_t iget(ino_t cdir, char *path, int parent);

#endif

