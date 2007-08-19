/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <scache.h>
#include <device.h>
#include <vnode.h>
#include <libc.h>

/* cache storage */
struct scache scache[NSECACHS];

/* clist index */
struct scache *scache_clist[NBLKDEVS][NBLKENTS];

/* cache module internal data for management */
struct scache_data
{
	unsigned cnt;		/* count of allocated caches */
	struct scache *begin;	/* beginning of allocation list */
	struct scache *end;	/* end of allocation list */
}scache_data;

struct scache *scache_alloc()
{
	struct scache *sp;
	if(scache_data.cnt < NSECACHS) {
		/* just allocate each cache in order */
		sp = &scache[scache_data.cnt];
		if(scache_data.cnt == 0) {
			scache_data.begin= sp;
			sp->alast = 0;
		} else {
			sp->alast = scache_data.end;
		}
		sp->anext = 0;
		if(scache_data.end) {
			scache_data.end->anext = sp;
		}
		scache_data.end = sp;
		scache_data.cnt++;
		return sp;
	} else {
		/* do reallocates in the policy of least used reallocation */
		for(sp=scache_data.begin; sp!=0; sp=sp->anext) if(!sp->sem) {
			/* put sp to end of alist */
			/*  1. unlink */
			if(sp->alast != 0) {
				sp->alast->anext = sp->anext;
			}
			if(sp->anext != 0) {
				sp->anext->alast = sp->alast;
			}
			/*  2. relinking to end */
			sp->alast = scache_data.end;
			sp->anext = 0;
			scache_data.end->anext = sp;
			scache_data.end = sp;

			/* remove sp from clist */
			if(sp->clast != 0) {
				sp->clast->cnext = sp->cnext;
			}
			if(sp->cnext != 0) {
				sp->cnext->clast = sp->clast;
			}
			return sp;
		}
	}
	return 0;
}

/* Promote cache entry in LUR policy */
void scache_promo(struct scache *sp)
{
	/* 1. unlinking from alist */
	if(sp->anext != 0) {
		sp->anext->alast = sp->alast;
	} else {
		/* sp is already the least priority */
		return;
	}
	if(sp->alast != 0) {
		sp->alast->anext = sp->anext;
	} else {
		scache_data.begin = sp->anext;
	}

	/* 2. put it after it's original next node (promotion) */
	if(sp->anext->anext != 0) {
		sp->anext->anext->alast = sp;
	}
	sp->alast = sp->anext;
	sp->anext = sp->anext->anext;
	sp->alast->anext = sp;
	if(sp->anext == 0) {
		scache_data.end = sp;
	}
}

int scache_insert(struct scache **clistp, struct scache *item)
{
	struct scache *clist = *clistp;

	/* case the clist is empty */
	if(clist == 0) {
		*clistp = item;
		item->cnext = 0;
		item->clast = 0;
		return 0;
	} else {
		/* not empty, search and insert (sorted) */
		for(; clist->cnext!=0; clist=clist->cnext) {
			/* find entry with blk bigger than item's */
			if(clist->blk == item->blk) {
				MSG(1, "same blk exists\n");
				return -1;	/* if same blk exists */
			} else if(clist->blk > item->blk) {
				item->clast = clist->clast;
				item->cnext = clist;
				clist->clast->cnext = item;
				clist->clast = item;
				if(item->clast == 0) {
					*clistp = item;
				}
				return 0;
			}
		}
		/* not found till the end, compare with the end first */
		if(clist->blk == item->blk) {
			/* if same blk exists */
			MSG(1, "same blk exists\n");
			return -1;
		} else if(clist->blk > item->blk) {
			item->clast = clist->clast;
			item->cnext = clist;
			clist->clast->cnext = item;
			clist->clast = item;
			if(item->clast == 0) {
				*clistp = item;
			}
		} else {
			/* when the end's blk is smaller than item */
			/* append item to end of clist */
			item->clast = clist;
			item->cnext = 0;
			clist->cnext = item;
			if(item->clast == 0) {
				*clistp = item;
			}
		}
	}
	return 0;
}

struct scache *scache_seek(struct scache *clist, blkcnt_t blk)
{
	for(; clist!=0; clist=clist->cnext) {
		if(clist->blk == blk) {
			return clist;
		}
	}
	/* NULL */
	return 0;
}

struct scache *scache_get(dev_t dev, blkcnt_t blk, blksize_t blksize)
{
	struct scache *sp;

	/* seek for alredy read blocks */
	sp = scache_seek(scache_clist[MAJOR(dev)][MINOR(dev)], blk);

	if(sp != 0) {
		/* if really already read */
		scache_promo(sp);	/* lur promotion */
		return sp;
	}
	
	/* try if device can be used */
	if(dev_open(BLKDEV, dev, O_RDONLY, 0) != 0) {
		MSG(1, "Cannot open blkdev %d\n", dev);
		/* null */
		return 0;
	}

	/* block has never been read */
	MSG(blksize%512, "blksize is not 512 aligned");
	MSG((blksize/512) > (NMXBLKSZ/512), "block size unsupported\n");

	/* allocate new scache storge */
	sp = scache_alloc();
	sp->blk = blk;
	dev_lseek(BLKDEV, dev, blk*(blksize/512), 0);
	dev_read(BLKDEV, dev, sp->buf, blksize/512);
	scache_insert(&scache_clist[MAJOR(dev)][MINOR(dev)], sp);
	
	/* try to close device */
	if(dev_close(BLKDEV, dev) != 0) {
		MSG(1, "Failed to close blkdev %d\n", dev);
	}
	return sp;
}

int scache_sync(dev_t dev, blkcnt_t blk, blksize_t blksize)
{
	struct scache *sp;

	/* seek for alredy read blocks */
	sp = scache_seek(scache_clist[MAJOR(dev)][MINOR(dev)], blk);

	if(sp == 0) {
		/* if not in cache */
		MSG(1, "requested block not in cache, cannot sync\n");
		return -1;
	}

	/* try if device can be used */
	if(dev_open(BLKDEV, dev, O_RDONLY, 0) != 0) {
		MSG(1, "Cannot open blkdev %d\n", dev);
		return -1;
	}

	/* block has never been read */
	MSG(blksize%512, "blksize is not 512 aligned");
	MSG((blksize/512) > (NMXBLKSZ/512), "block size unsupported\n");

	/* allocate new scache storge */
	dev_lseek(BLKDEV, dev, blk*(blksize/512), 0);
	dev_write(BLKDEV, dev, sp->buf, blksize/512);
	
	/* try to close device */
	if(dev_close(BLKDEV, dev) != 0) {
		MSG(1, "Failed to close blkdev %d\n", dev);
	}

	return 0;

}

int scache_commit(dev_t dev, blksize_t blksize)
{
	struct scache *sp;

	/* get the head of dev's list */
	sp = scache_clist[MAJOR(dev)][MINOR(dev)];

	/* look for sectors that has been changed */
	for(; sp!=0; sp=sp->cnext) {
		if(sp->mode & S_CHANGED) {
			/* save the sector */
			scache_sync(dev, sp->blk, blksize);
		}
	}

	return 0;
}

