/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_SCACHE_H__
#define __MICRON_KERNEL_SCACHE_H__

#include <types.h>
#include <config.h>

#define S_CHANGED	0x01	/* content has been changed, request commit */

/* core structure of sector cache */
struct scache
{
	blkcnt_t blk;		/* logical block number */
	unsigned sem;		/* in use count semaphore */
	char   buf[NMXBLKSZ];	/* the size is 1 block for all file systems */
	unsigned char mode;	/* mode bits of sector cache */
	struct scache *cnext;	/* cache list's next entry */
	struct scache *clast;	/* cache list's last entry */
	struct scache *anext;	/* allocation list's next entry */
	struct scache *alast;	/* allocation list's last entry */
};

/* sector buffer interface (based on blkdev) */
extern struct scache *scache_get(dev_t dev, blkcnt_t blk, blksize_t blksize);
extern int scache_sync(dev_t dev, blkcnt_t blk, blksize_t blksize);
extern int scache_commit(dev_t dev, blksize_t blksize);

#endif

