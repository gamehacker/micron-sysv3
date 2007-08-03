/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_ICACHE_H__
#define __MICRON_KERNEL_ICACHE_H__

#include <types.h>
#include <config.h>

/* Micron Specific vnode cache status */
#define V_AVAIL		0x01	/* data still available, not reallocated */

/* Micron Specific vnode macros */
#define V_ISAVAIL(m)	((m&V_AVAIL)==V_AVAIL)
#define V_ISFSROOT(m)	((m&V_FSROOT)==V_FSROOT)

/* VFS vnode structure */
struct inode
{
	/* POSIX compliant */
	dev_t	i_dev;		/* Device ID of device containing file */
	ino_t	i_ino;		/* File serial number */
	mode_t	i_mode;		/* Mode of file */
	nlink_t	i_nlink;	/* Number of hard links to the file */
	uid_t	i_uid;		/* User ID of file */
	gid_t	i_gid;		/* Group ID of file */
	dev_t	i_rdev;		/* Device ID (if file is char or blk special */
	off_t	i_size;		/* For regular files, the file size in bytes
				 * For symbolic, the length in bytes of the
				 * pathname contained in the symbolic link.
				 * 
				 * For a shared memory object, the length in
				 * bytes.
				 *
				 * For a typed memory object, the length in
				 * bytes.
				 *
				 * For other file types, the use of this field
				 * is unspecified */
	time_t	i_atime;	/* Time of last access */
	time_t	i_mtime;	/* Time of last data modification */
	time_t	i_ctime;	/* Time of last status change */
	blkcnt_t  i_blkentry;	/* Beginning of descripted blocks */
	blksize_t i_blksize;	/* A file system-specific preferred I/O block
				 * size for this object. In some file system
				 * types, this may vary from file to file. */
	blkcnt_t  i_blocks;	/* Number of blocks allocated for this object */

	/* Micron specific internal linkage */
	ino_t	i_id;		/* Vnode ID in vnode cache */
	ino_t	i_next;		/* Next node on same level */
	ino_t	i_parent;	/* Parent node */
	ino_t	i_child;	/* Child node list */
	ino_t	i_expand;	/* Node expansion (same file blk extention) */

	/* Micron specific vnode cache specific */
	u32_t	i_vsem;		/* in use count semaphore */
	ino_t	i_vnext;	/* next in the cache's allocated list */
	mode_t	i_vstat;	/* cache vnode status */
}__attribute__((packed));

/* Vnode cache data */
extern struct inode icache[NINODES];

/* Vnode cache operations */
extern int icache_init();
extern ino_t icache_alloc();

#endif

