/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <fs.h>

struct fs fs[NFSENTYS];

enum fstype fs_probe(dev_t dev)
{
	int i;
	for(i=0; i<NFSENTYS; i++) {
		if((fs[i].probe != 0) && (fs[i].probe(dev) != -1)) {
			return i;
		}
	}
	return -1;	/* unknown type */
}

int fs_blksz(enum fstype type, dev_t dev)
{
	if(fs[type].blksz == 0) {
		return -1;
	}
	return fs[type].blksz(dev);
}

int fs_lsblk(enum fstype type, dev_t dev, char *buf)
{
	if(fs[type].lsblk == 0) {
		return -1;
	}
	return fs[type].lsblk(dev, buf);
}

int fs_lnode(enum fstype type, void *sblk, struct vnode *node)
{
	if(fs[type].lnode == 0) {
		return -1;
	}
	return fs[type].lnode(sblk, node);
}

int fs_snode(enum fstype type, void *sblk, struct vnode *node)
{
	if(fs[type].snode == 0) {
		return -1;
	}
	return fs[type].snode(sblk, node);
}

struct vnode *fs_ialloc(enum fstype type, void *sblk, dev_t dev)
{
	if(fs[type].ialloc == 0) {
		return (struct vnode*)-1;
	}
	return fs[type].ialloc(sblk, dev);
}

int fs_ifree(enum fstype type, void *sblk, struct vnode *node)
{
	if(fs[type].ifree == 0) {
		return -1;
	}
	return fs[type].ifree(sblk, node);
}

blkcnt_t fs_daddr(enum fstype type, void *sblk)
{
	if(fs[type].daddr == 0) {
		return -1;
	}
	return fs[type].daddr(sblk);
}

blkcnt_t fs_dalloc(enum fstype type, void *sblk, dev_t dev)
{
	if(fs[type].dalloc == 0) {
		return -1;
	}
	return fs[type].dalloc(sblk, dev);
}

int fs_dfree(enum fstype type, void *sblk, dev_t dev, blkcnt_t blk)
{
	if(fs[type].dfree == 0) {
		return -1;
	}
	return fs[type].dfree(sblk, dev, blk);
}

