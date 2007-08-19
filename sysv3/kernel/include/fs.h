/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_FS_H__
#define __MICRON_KERNEL_FS_H__

#include <config.h>
#include <types.h>
#include <vnode.h>

enum fstype
{
	MFS
	/* TODO: More file system types to go */
};

struct fs
{
	/* file system probe */
	int (*probe)(dev_t dev);

	/* get file system block size */
	blkcnt_t (*blksz)(dev_t dev);

	/* load super block to buffer */
	int (*lsblk)(dev_t dev, char *buf);

	/* loading and saving nodes */
	int (*lnode)(void *sblk, struct vnode *node);
	int (*snode)(void *sblk, struct vnode *node);

	/* node allocation and releasing */
	struct vnode *(*ialloc)(void *sblk, dev_t dev);
	int (*ifree)(void *sblk, struct vnode *node);

	/* file data block allocation and releasing */
	blkcnt_t (*daddr)(void *sblk);
	blkcnt_t (*dalloc)(void *sblk, dev_t dev);
	int (*dfree)(void *sblk, dev_t dev, blkcnt_t blk);
};

extern struct fs fs[NFSENTYS];

extern enum fstype fs_probe(dev_t dev);
extern blkcnt_t fs_blksz(enum fstype type, dev_t dev);
extern int fs_lsblk(enum fstype type, dev_t dev, char *buf);
extern int fs_lnode(enum fstype type, void *sblk, struct vnode *node);
extern int fs_snode(enum fstype type, void *sblk, struct vnode *node);
extern struct vnode *fs_ialloc(enum fstype type, void *sblk, dev_t dev);
extern int fs_ifree(enum fstype type, void *sblk, struct vnode *node);
extern blkcnt_t fs_daddr (enum fstype type, void *sblk);
extern blkcnt_t fs_dalloc(enum fstype type, void *sblk, dev_t dev);
extern int fs_dfree(enum fstype type, void *sblk, dev_t dev, blkcnt_t blk);

#endif

