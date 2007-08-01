/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_VCACHE_H__
#define __MICRON_KERNEL_VCACHE_H__

#include <types.h>
#include <config.h>

/* Micron Specific vnode cache status */
#define V_AVAIL		0x01	/* data still available, not reallocated */

/* Micron Specific vnode macros */
#define V_ISAVAIL(m)	((m&V_AVAIL)==V_AVAIL)
#define V_ISFSROOT(m)	((m&V_FSROOT)==V_FSROOT)

/* VFS vnode structure */
struct vnode
{
	/* POSIX compliant */
	dev_t	v_dev;		/* Device ID of device containing file */
	ino_t	v_ino;		/* File serial number */
	mode_t	v_mode;		/* Mode of file */
	nlink_t	v_nlink;	/* Number of hard links to the file */
	uid_t	v_uid;		/* User ID of file */
	gid_t	v_gid;		/* Group ID of file */
	dev_t	v_rdev;		/* Device ID (if file is char or blk special */
	off_t	v_size;		/* For regular files, the file size in bytes
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
	time_t	v_atime;	/* Time of last access */
	time_t	v_mtime;	/* Time of last data modification */
	time_t	v_ctime;	/* Time of last status change */
	blkcnt_t  v_blkentry;	/* Beginning of descripted blocks */
	blksize_t v_blksize;	/* A file system-specific preferred I/O block
				 * size for this object. In some file system
				 * types, this may vary from file to file. */
	blkcnt_t  v_blocks;	/* Number of blocks allocated for this object */

	/* Micron specific internal linkage */
	ino_t	v_id;		/* Vnode ID in vnode cache */
	ino_t	v_next;		/* Next node on same level */
	ino_t	v_parent;	/* Parent node */
	ino_t	v_child;	/* Child node list */
	ino_t	v_expand;	/* Node expansion (same file blk extention) */

	/* Micron specific vnode cache specific */
	u32_t	v_vsem;		/* in use count semaphore */
	ino_t	v_vnext;	/* next in the cache's allocated list */
	mode_t	v_vstat;	/* cache vnode status */
}__attribute__((packed));

/* Vnode cache data */
extern struct vnode vcache[NVNODES];

/* Vnode cache operations */
extern int vcache_init();
extern ino_t vcache_alloc();

#endif

