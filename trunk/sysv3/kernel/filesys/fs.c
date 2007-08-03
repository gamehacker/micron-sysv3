/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <fs.h>

struct vfs vfs[NVFSENTS];

int vfs_lschild(enum vfs_type t, ino_t vid, ino_t id)
{
	if(vfs[t].lschild == 0) {
		return -1;		/* file system not exist */
	}
	return vfs[t].lschild(vid, id);
}

int vfs_mknode(enum vfs_type t, ino_t id, char *name, mode_t mode, dev_t dev)
{
	if(vfs[t].lschild == 0 &&
	   vfs[t].mknode  == 0) {
		return -1;		/* file system not exist */
	}
	return vfs[t].mknode(id, name, mode, dev);
}

int vfs_rmnode(enum vfs_type t, ino_t id, char *name)
{
	if(vfs[t].lschild == 0 &&
	   vfs[t].rmnode  == 0) {
		return -1;		/* file system not exist */
	}
	return vfs[t].rmnode(id, name);
}

int vfs_lseek(enum vfs_type t, ino_t id, off_t offset, int whence)
{
	if(vfs[t].lschild == 0 &&
	   vfs[t].lseek   == 0) {
		return -1;		/* file system not exist */
	}
	return vfs[t].lseek(id, offset, whence);
}

int vfs_read(enum vfs_type t, ino_t id, char *buf, size_t nbyte)
{
	if(vfs[t].lschild == 0 &&
	   vfs[t].read    == 0) {
		return -1;		/* file system not exist */
	}
	return vfs[t].read(id, buf, nbyte);
}

int vfs_write(enum vfs_type t, ino_t id, char *buf, size_t nbyte)
{
	if(vfs[t].lschild == 0 &&
	   vfs[t].write   == 0) {
		return -1;		/* file system not exist */
	}
	return vfs[t].write(id, buf, nbyte);
}

