/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_OFILE_H__
#define __MICRON_KERNEL_OFILE_H__

#include <vnode.h>
#include <vfs.h>
#include <scache.h>
#include <mount.h>
#include <config.h>

/* The open file descriptor
 * This structure describes each open file ready for read/write/lseek etc, and
 * at the same time it handles the operations for named/unamed pipes.
 */
struct ofile
{
	int oflag;			/* open file flags */
	struct vnode *node;		/* file vnode pointer */
	struct mountab *mount;		/* file's mount point info */
	/* for regular files */
	off_t pos;			/* r/w position */

	/* TODO: design FIFO support based on this struct */
};

extern struct ofile ofile[NMXOFILE];

/* operations */
extern struct ofile *ofile_alloc();

#endif

