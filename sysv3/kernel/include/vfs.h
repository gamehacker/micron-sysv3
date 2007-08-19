/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_VFS_H__
#define __MICRON_KERNEL_VFS_H__

#include <mount.h>		/* mount point management, vfs core */
#include <vnode.h>
#include <ofile.h>
#include <vcache.h>		/* vnode management system */
#include <scache.h>		/* sector cache */

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

/* Internal Services */
extern struct vnode *vget(char *path, int parent);

/* Internal System Calls (for kernel modules only) */
extern int mkdir(char *path, mode_t mode);
extern int rmdir(char *path);

/* current implementation progress: */
extern int open(char *path, int oflag, ...);
extern int close(int fildes);
extern ssize_t read(int fildes, void *buf, size_t nbyte);
extern ssize_t write(int fildes, void *buf, size_t nbyte);
extern off_t lseek(int fildes, off_t offset, int whence);

/* to be scheduled */
//fcntl
//ioctl

//extern int mknod(char *path, mode_t mode, dev_t dev);
//creat
//mkfifo
//extern int unlink(char *path);
//remove

//opendir
//readdir
//rewinddir

#endif

