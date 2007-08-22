/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <fs.h>
#include <libc.h>
#include <scache.h>
#include <vcache.h>
#include <device.h>
#include "mfs.h"

struct fs *mfs_fs = &fs[MFS];

/************************ MFS Service Routines *******************************/

struct mfs_inode *mfs_get(void *sblk, dev_t dev, ino_t node)
{
	struct mfs_sblk *sbp = sblk;
	struct scache *sp;
	struct mfs_inode *ip;

	/* calculate target block displacements */
	blkcnt_t blk = (sizeof(struct mfs_inode)*node)/sbp->s_blksize;
	ino_t    ety = node%(sbp->s_blksize/sizeof(struct mfs_inode));

	sp = scache_get(dev, sbp->s_iblk + blk, sbp->s_blksize);
	ip = (struct mfs_inode*)sp->buf;

	return &ip[ety];
}

void mfs_put(void *sblk, dev_t dev, ino_t node)
{
	struct mfs_sblk *sbp = sblk;
	
	/* calculate target block displacements */
	blkcnt_t blk = (sizeof(struct mfs_inode)*node)/sbp->s_blksize;

	scache_sync(dev, sbp->s_iblk + blk, sbp->s_blksize);
}

enum bmptype
{
	IBLK,	/* inode block */
	DBLK	/* data block */
};

void mfs_markbmp(void *sblk, dev_t dev, enum bmptype type, int index, char stat)
{
	struct mfs_sblk *sbp = sblk;
	struct scache *sp;

	/* count for block info */
	blkcnt_t blk = index/(8*sbp->s_blksize);
	int ety = index%(8*sbp->s_blksize);

	/* get block from cache */
	switch(type){
	case IBLK:
		sp = scache_get(dev, sbp->s_ibmp + blk, sbp->s_blksize);
		if(stat == 0) {
			sp->buf[ety/8] &= ~(1<<(7-ety%8));
		} else {
			sp->buf[ety/8] |=  (1<<(7-ety%8));
		}
		scache_sync(dev, sbp->s_ibmp + blk, sbp->s_blksize);
		break;
	case DBLK:
		sp = scache_get(dev, sbp->s_dbmp + blk, sbp->s_blksize);
		if(stat == 0) {
			sp->buf[ety/8] &= ~(1<<(7-ety%8));
		} else {
			sp->buf[ety/8] |=  (1<<(7-ety%8));
		}
		scache_sync(dev, sbp->s_dbmp + blk, sbp->s_blksize);
		break;
	}
}

int mfs_seekbmp(void *sblk, dev_t dev, enum bmptype type)
{
	struct mfs_sblk *sbp = sblk;
	struct scache *sp;

	blkcnt_t blk = 0;
	int i;
	int j=0;
	unsigned char temp;

	/* get block from cache */
	switch(type){
	case IBLK:
		while(1) {
			sp = scache_get(dev, sbp->s_ibmp + blk, sbp->s_blksize);
			for(i=0; i<sbp->s_blksize; i++) {
				if(((unsigned char*)sp->buf)[i] != 0xff) {
					goto seek_byte;
				}
			}
			if(blk < sbp->s_ibmpcnt) {
				blk++;
			} else {
				return -1;
			}
		}
		break;
	case DBLK:
		while(1) {
			sp = scache_get(dev, sbp->s_dbmp + blk, sbp->s_blksize);
			for(i=0; i<sbp->s_blksize; i++) {
				if(((unsigned char*)sp->buf)[i] != 0xff) {
					goto seek_byte;
				}
			}
			if(blk < sbp->s_dbmpcnt) {
				blk++;
			} else {
				return -1;
			}
		}
		break;
	}
	return -1;

seek_byte:

	temp = ~ sp->buf[i];
	while((temp&0x80) != 0x80) {
		temp<<=1;
		j++;
	}
	return j + i*8;
}

/************************ File System Interfaces *****************************/

int mfs_probe(dev_t dev)
{
	struct scache *sp;
	sp = scache_get(dev, 0, 512);
	if(!strncmp(sp->buf, "MFS", 3)) {
		return 0;
	}
	return -1;
}

blkcnt_t mfs_blksz(dev_t dev)
{
	struct scache *sp;
	struct mfs_sblk *sbp;
	sp = scache_get(dev, 0, 512);
	sbp= (struct mfs_sblk*)sp->buf;
	return sbp->s_blksize;
}

int mfs_lsblk(dev_t dev, char *buf)
{
	struct scache *sp;
	sp = scache_get(dev, 0, NMXBLKSZ);	/* load as much as we can */
	memcpy(buf, sp->buf, NMXBLKSZ);
	return 0;
}

/* sync node's childs with contents from media */
int mfs_lnode(void *sblk, struct vnode *node)
{
	struct mfs_inode *ip, *iep;
	struct vnode *vp, *lvp = 0, *vep, *lvep = 0;

	/* get the parent from media */
	ip = mfs_get(sblk, node->dev, node->ino);

	/* if the parent contains no child */
	if(ip->child == 0) {
		node->child = 0;
		return 0;
	}
	
	/* read the child list */
	ip = mfs_get(sblk, node->dev, ip->child);

	/* translate each childs into vnode */
	while(1) {
		vp = vcache_alloc();
		PANIC(vp == 0, "no vnode available");
		
		/* translations */
		vp->dev  = node->dev;
		strcpy(vp->name, ip->name);
		vp->ino  = ip->ino;
		vp->mode = ip->mode;
		vp->nlink= ip->nlink;
		vp->uid  = ip->uid;
		vp->gid  = ip->gid;
		vp->rdev = ip->rdev;
		vp->size = ip->size;
		vp->atime= ip->atime;
		vp->mtime= ip->mtime;
		vp->ctime= ip->ctime;
		vp->blkentry = ip->blkentry;
		vp->blocks   = ip->blocks;

		/* linkage setup */
		vp->parent = node;
		vp->child = (struct vnode*)ip->child;
		vp->next  = 0;
		if(lvp == 0) {
			node->child = vp;
		} else {
			lvp->next = vp;
		}
		if(ip->expand) {
			/* this means there's gonna be lots more to suffer */
			iep = mfs_get(sblk, node->dev, ip->expand);
			while(1) {				
				vep = vcache_alloc();
				PANIC(vep == 0, "no vnode available");

				/* translations */
				vep->dev  = node->dev;
				strcpy(vep->name, iep->name);
				vep->ino  = iep->ino;
				vep->mode = iep->mode;
				vep->nlink= iep->nlink;
				vep->uid  = iep->uid;
				vep->gid  = iep->gid;
				vep->rdev = iep->rdev;
				vep->size = iep->size;
				vep->atime= iep->atime;
				vep->mtime= iep->mtime;
				vep->ctime= iep->ctime;
				vep->blkentry = iep->blkentry;
				vep->blocks   = iep->blocks;

				/* linkage setup */
				vep->parent = vp->parent;
				vep->child  = vp->child;
				vep->next   = vp->next;
				vep->expand = 0;
				if(lvep == 0) {
					vp->expand = lvep;
				} else {
					lvep->expand = vep;
				}

				/* read next expand */
				lvep = vep;
				if(iep->expand == 0) {
					break;
				}
				iep = mfs_get(sblk, node->dev, iep->expand);
			}
		}

		/* read next child */
		lvp = vp;
		if(ip->next == 0) {
			break;
		}
		ip = mfs_get(sblk, node->dev, ip->next);
	}
	return 0;
}

/* save node and it's child nodes to media */
int mfs_snode(void *sblk, struct vnode *node)
{
	struct vnode *vp, *evp;
	struct mfs_inode *ip, *eip;

	/* save vp itself */
	vp = node;
	ip = mfs_get(sblk, node->dev, vp->ino);

	/* translations */
	strcpy(ip->name, vp->name);
	ip->ino  = vp->ino;
	ip->mode = vp->mode;
	ip->nlink= vp->nlink;
	ip->uid  = vp->uid;
	ip->gid  = vp->gid;
	ip->rdev = vp->rdev;
	ip->size = vp->size;
	ip->atime= vp->atime;
	ip->mtime= vp->mtime;
	ip->ctime= vp->ctime;
	ip->blkentry = vp->blkentry;
	ip->blocks   = vp->blocks;

	/* linkages */
	if(vp->next != 0) {
		ip->next = vp->next->ino;
	} else {
		ip->next = 0;
	}
	if(vp->child->parent == vp) {
		ip->child  = vp->child->ino;
	} else {
		ip->child = (ino_t) vp->child;
	}

	/* do the save in */
	mfs_put(sblk, node->dev, vp->ino);

	/* process child lists */
	if(node->child == 0) {
		return 0;
	}
	vp = node->child;

	/* save & translate childs */
	int stop = 0;
	while(!stop) {
		ip = mfs_get(sblk, node->dev, vp->ino);

		/* translations */
		strcpy(ip->name, vp->name);
		ip->ino  = vp->ino;
		ip->mode = vp->mode;
		ip->nlink= vp->nlink;
		ip->uid  = vp->uid;
		ip->gid  = vp->gid;
		ip->rdev = vp->rdev;
		ip->size = vp->size;
		ip->atime= vp->atime;
		ip->mtime= vp->mtime;
		ip->ctime= vp->ctime;
		ip->blkentry = vp->blkentry;
		ip->blocks   = vp->blocks;

		/* linkages */
		if(vp->next != 0) {
			ip->next = vp->next->ino;
		} else {
			ip->next = 0;
		}
		ip->parent = node->ino;
		if(vp->child->parent == vp) {
			ip->child  = vp->child->ino;
		} else {
			ip->child = (ino_t) vp->child;
		}
		if(vp->expand) {
			evp = vp->expand;
			while(1) {
				eip = mfs_get(sblk, node->dev, evp->ino);

				/* translations */
				strcpy(eip->name, evp->name);
				eip->ino  = evp->ino;
				eip->mode = evp->mode;
				eip->nlink= evp->nlink;
				eip->uid  = evp->uid;
				eip->gid  = evp->gid;
				eip->rdev = evp->rdev;
				eip->size = evp->size;
				eip->atime= evp->atime;
				eip->mtime= evp->mtime;
				eip->ctime= evp->ctime;
				eip->blkentry = evp->blkentry;
				eip->blocks   = evp->blocks;

				/* linkage */
				eip->parent = ip->parent;
				eip->child  = ip->child;
				eip->next   = ip->next;
				eip->expand = evp->expand->ino;

				/* do the save in */
				mfs_put(sblk, node->dev, evp->ino);

				/* next one or end */
				if(evp->expand == 0) {
					break;
				}
				evp = evp->expand;
			}
		}

		/* do the save in */
		mfs_put(sblk, node->dev, vp->ino);

		if(vp->next == 0) {
			break;
		}
		vp = vp->next;
	}
	return 0;
}

/* node allocation and releasing */
struct vnode *mfs_ialloc(void *sblk, dev_t dev)
{
	struct vnode *vp;
	int ino = mfs_seekbmp(sblk, dev, IBLK);
	if(ino == -1) {
		return 0;
	}
	mfs_markbmp(sblk, dev, IBLK, ino, 1);
	vp = vcache_alloc();
	vp->ino = ino;
	vp->dev = dev;
	return vp;
}

int mfs_ifree(void *sblk, struct vnode *node)
{
	mfs_markbmp(sblk, node->dev, IBLK, node->ino, 0);
	return 0;
}

blkcnt_t mfs_daddr(void *sblk)
{
	struct mfs_sblk *sbp = sblk;
	return sbp->s_dblk;
}

/* file data block allocation and releasing */
blkcnt_t mfs_dalloc(void *sblk, dev_t dev)
{
	int id = mfs_seekbmp(sblk, dev, DBLK);
	if(id == -1) {
		return 0;
	}
	mfs_markbmp(sblk, dev, DBLK, id, 1);
	return id;
}

int mfs_dfree(void *sblk, dev_t dev, blkcnt_t blk)
{
	mfs_markbmp(sblk, dev, IBLK, blk, 0);
	return 0;
}

int mfs_init()
{
	mfs_fs->probe  = mfs_probe;
	mfs_fs->blksz  = mfs_blksz;
	mfs_fs->lsblk  = mfs_lsblk;
	mfs_fs->lnode  = mfs_lnode;
	mfs_fs->snode  = mfs_snode;
	mfs_fs->ialloc = mfs_ialloc;
	mfs_fs->ifree  = mfs_ifree;
	mfs_fs->daddr  = mfs_daddr;
	mfs_fs->dalloc = mfs_dalloc;
	mfs_fs->dfree  = mfs_dfree;

	return 0;
}

int mfs_exit()
{
	mfs_fs->probe  = 0;
	mfs_fs->lsblk  = 0;
	mfs_fs->lnode  = 0;
	mfs_fs->snode  = 0;
	mfs_fs->ialloc = 0;
	mfs_fs->ifree  = 0;
	mfs_fs->dalloc = 0;
	mfs_fs->dfree  = 0;
	return 0;
}

REGISTER_MODULE(FS, mfs, "Micron File System", mfs_init, mfs_exit);

