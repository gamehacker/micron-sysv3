/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <vfs.h>
#include <libc.h>
#include <time.h>
#include <ofile.h>

struct vnode *vget(char *path, int parent)
{
	struct vnode *vp;
	struct mountab *mntp;

	char *pathp = path, *pathe;

	/* path must be started with '/' */
	if(path[0] == '/') {
		if((mntp = mountab_getroot()) == 0) {
			MSG(1, "Cannot locate /\n");
			return (struct vnode*)-1;	/* not root specified */
		}
		vp = &mntp->root;
		pathp++;	/* point pathp after '/' */
	} else {
		MSG(1, "path not started from /\n");
		return (struct vnode*)-1;
	}

	/* setup pathe to the last '/' */
	pathe = path + strlen(path);
	while(pathe[-1]!='/') pathe--;

	/* parse through the *path string */
	for(; strchr(pathp, '/')!=0; pathp=strchr(pathp, '/')+1) {
		/* read the mount point info */
		mntp = mountab_get(vp->dev);
		if(mntp == 0) {
			return (struct vnode*)-1;	/* dev not mounted */
		}
		if(vp->child->parent != vp) {
			fs_lnode(mntp->fs, mntp->sblk, vp);
		}
		for(vp=vp->child; vp!=0; vp=vp->next) {
			/* mount point has the first priority */
			if((mntp = mountab_seek(vp->dev, vp->ino)) != 0) {
				vp = &mntp->root;
				break;
			}
			/* then local nodes */
			int len1=strlen(vp->name);
			int len2=strchr(pathp, '/') - pathp;
			if((len1==len2) && !strncmp(vp->name, pathp, len1)) {
				break;
			}
		}
		if(vp == 0) {
			return 0;
		}
	}

	/* if the parent is enough */
	if(parent) {
		/* read the mount point info */
		mntp = mountab_get(vp->dev);
		if(mntp == 0) {
			return (struct vnode*)-1;	/* dev not mounted */
		}

		/* prepare vp's children before return */
		if(vp->child->parent != vp) {
			fs_lnode(mntp->fs, mntp->sblk, vp);
		}
		return vp;
	}
	
	/* read the mount point info */
	mntp = mountab_get(vp->dev);
	if(mntp == 0) {
		return (struct vnode*)-1;	/* dev not mounted */
	}

	/* procceed to the final destination */
	if(vp->child->parent != vp) {
		fs_lnode(mntp->fs, mntp->sblk, vp);
	}
	for(vp=vp->child; vp!=0; vp=vp->next) {
		int len1=strlen(vp->name);
		int len2=strlen(pathp);
		if((len1==len2) && !strncmp(vp->name, pathp, len1)) {
			/* read the mount point info */
			mntp = mountab_get(vp->dev);
			if(mntp == 0) {
				return (struct vnode*)-1;/* dev not mounted */
			}

			/* prepare vp's children before return */
			if(vp->child->parent != vp) {
				fs_lnode(mntp->fs, mntp->sblk, vp);
			}
			return vp;
		}
	}
	return 0;
}

int mkdir(char *path, mode_t mode)
{
	struct vnode *vp, *np;
	struct mountab *mtp;
	char *name;

	/* get path's parent node */
	vp = vget(path, 1);
	if(vp == 0) {
		MSG(1, "parent not found\n");
		return 0;	/* parent node not found */
	} else if(vp == (struct vnode*)-1) {
		MSG(1, "error in input\n");
		return -1;	/* error occured in input */
	}

	/* create node under target's parent */
	mtp = mountab_get(vp->dev);
	if(mtp == 0) {
		MSG(1, "mount point not found\n");
		return -1;	/* mount point not found */
	}
	np = fs_ialloc(mtp->fs, mtp->sblk, vp->dev);

	/* get target's name */
	name = path + strlen(path);
	while((*(name-1) != '/') && ((name-1) > path)) name--;

	/* setup values */
	strcpy(np->name, name);
	np->dev      = vp->dev;
	np->mode     = (mode&0xFFF)|S_IFDIR;
	np->nlink    = 0;
	np->uid      = 0;
	np->gid      = 0;
	np->rdev     = 0;
	np->size     = 0;
	np->atime    = time(0);
	np->mtime    = time(0);
	np->ctime    = time(0);
	np->blkentry = 0;
	np->blocks   = 0;

	/* linkage */
	np->parent   = vp;
	np->child    = 0;
	np->next     = vp->child;
	np->expand   = 0;
	vp->child    = np;

	/* save nodes under vp */
	fs_snode(mtp->fs, mtp->sblk, vp);

	return 0;
}

int rmdir (char *path)
{
	struct vnode *vp, *vnp;
	struct mountab *mtp;

	/* get target's vnode */
	vp = vget(path, 0);

	/* if target is not a directory, failure */
	if(!S_ISDIR(vp->mode)) {
		return -1;
	}

	/* if target has child, failure */
	if(vp->child != 0) {
		return -1;
	}

	/* unlink vnode from list */
	vnp = vp->parent;
	if(vnp->child == vp) {
		vnp->child = vp->next;
	} else {
		for(vnp=vnp->child; vnp!=0; vnp=vnp->next) {
			if(vnp->next == vp) {
				vnp->next = vp->next;
			}
		}
	}

	/* get mount point info */
	mtp = mountab_get(vp->dev);	
	if(mtp == 0) {
		MSG(1, "mount point not found\n");
		return -1;	/* mount point not found */
	}

	/* release vnode */
	fs_ifree(mtp->fs, mtp->sblk, vp);
	
	/* save vp and it's child */
	fs_snode(mtp->fs, mtp->sblk, vnp);
	return 0;
}

/* flags to support:
 * O_RDONLY
 * O_WRONLY
 * O_RDWR
 * O_CREAT
 * O_EXCL
 * O_TRUNC
 * O_APPEND
 */
int open(char *path, int oflag, ...)
{
	struct vnode *vp;
	struct scache *scp;
	struct mountab *mtp;
	struct ofile *ofp;

	/* first, find the corresponding vnode of *path */
	vp = vget(path, 0);
	if((vp == (struct vnode*)-1)||(vp == 0)) {
		/* file not found */
		if(oflag&O_CREAT) {
			if((oflag&O_EXCL) == O_EXCL) {
				return 0;
			}
			/* create the file and continue the process */
		}
	} else if(((oflag&O_CREAT) == O_CREAT) && ((oflag&O_EXCL) == O_EXCL)) {
		return -1;
	}

	/* checking read/write permitions, 
	 * kernel is the owner of all files */
	if(((oflag&O_RDONLY) == O_RDONLY) && ((vp->mode&S_IRUSR) != S_IRUSR)) {
		return -1;
	}
	if(((oflag&O_WRONLY) == O_WRONLY) && ((vp->mode&S_IWUSR) != S_IWUSR)) {
		return -1;
	}

	/* load sectors containing the requested data position */
	mtp = mountab_get(vp->dev);
	if(mtp == 0) {
		return -1;	/* mount point not found */
	}
	scp = scache_get(vp->dev, vp->blkentry, mtp->blksize);
	if(scp == 0) {
		return -1;	/* cannot locate data block */
	}

	/* request a open file descriptor 
	 * describing kernel file operation,
	 * which containing vnode data, 
	 * mount data and beginning sector cache data*/
	ofp = ofile_alloc();
	if(ofp == 0) {
		MSG(1, "no open file descriptor available\n");
		return -1;	/* no ofile available */
	}
	ofp->node   = vp;
	ofp->mount  = mtp;
	ofp->oflag  = oflag;

	/* setup pos value, load pos sector to buffer */
	if((oflag&&O_APPEND) == O_APPEND) {
		/* set pos to end of file */
		ofp->pos = vp->size;
	} else if((oflag&&O_TRUNC) == O_TRUNC) {
		/* release all data blocks allocated,
		 * setup file size to 0 and block data to 0*/
		ofp->pos = 0;
		vp->size = 0;
		blkcnt_t bc;
		for(bc=vp->blkentry; bc<=vp->blkentry+vp->blocks; bc++) {
			fs_dfree(mtp->fs, mtp->sblk, vp->dev, bc);
		}
		vp->blkentry = 0;
		vp->blocks   = 0;
	} else {
		ofp->pos = 0;
	}
	lseek((int)ofp, ofp->pos, SEEK_SET);

	return (int)ofp;
}

int close(int fildes)
{
	struct ofile *ofp;

	ofp = (struct ofile*)fildes;
	
	/* check validation of fildes */
	if((ofp < &ofile[0]) || (ofp >= &ofile[NMXOFILE])) {
		return -1;
	}

	/* commited all changed sectors */
	scache_commit(ofp->node->dev, ofp->mount->blksize);

	/* invalidate fildes */
	ofp->node = 0;

	return 0;
}

off_t lseek(int fildes, off_t offset, int whence)
{
	blkcnt_t blk;
	struct ofile *ofp = (struct ofile*)fildes;

	/* general methods: 
	 *  1. setup displacement
	 *  2. update sector buffer
	 */
	switch(whence) {
	case SEEK_SET:
		ofp->pos = offset;
		break;
	case SEEK_CUR:
		ofp->pos+= offset;
		break;
	case SEEK_END:
		ofp->pos = ofp->node->size;
		ofp->pos+= offset;
		break;
	}

	/* boundary checks */
	if((ofp->pos > ofp->node->size) ||
	   (ofp->pos < 0)) {
		return -1;	/* offset out of boundary */
	}

	/* calculate block index */
	blk = ofp->pos/ofp->mount->blksize;
	blk+= ofp->node->blkentry;
	blk+= fs_daddr(ofp->mount->fs, ofp->mount->sblk);

	/* load block if not in buffer */
	scache_get(ofp->node->dev, blk, ofp->mount->blksize);

	return ofp->pos;
}

/* WARNING: This read function is far beyond complete, 
 * vnode expansion support should be added
 */
ssize_t read(int fildes, void *buf, size_t nbyte)
{
	blkcnt_t blk, oldblk;
	off_t disp;
	size_t xfer = nbyte, i=0;
	struct scache *scp;
	struct ofile *ofp = (struct ofile*)fildes;

	/* do the data copy */
	while(xfer) {
		/* calculate pos defined block number
		 * and displacement.
		 */
		blk  = ofp->pos/ofp->mount->blksize;
		blk += ofp->node->blkentry;
		blk += fs_daddr(ofp->mount->fs, ofp->mount->sblk);
		disp = ofp->pos%ofp->mount->blksize;

		/* load block to cache if not loaded */
		if(oldblk != blk) {
			scp = scache_get(ofp->node->dev,
						blk, ofp->mount->blksize);
			oldblk = blk;
		}
	
		/* do the copy */
		if((xfer+disp) > ofp->mount->blksize) {
			memcpy(&((char*)buf)[i], &scp->buf[disp], 
					ofp->mount->blksize - disp);
			ofp->pos += ofp->mount->blksize - disp;
			disp  = 0;
			xfer -= ofp->mount->blksize - disp;
			i    += ofp->mount->blksize - disp;
		} else {
			memcpy(&((char*)buf)[i], &scp->buf[disp], xfer);
			ofp->pos += xfer;
			disp  = 0;
			xfer -= xfer;
			i    += xfer;
		}

		/* check end of file with pos */
		if(ofp->pos > ofp->node->size) {
			ofp->pos = ofp->node->size;
			break;
		}
	}
	return nbyte;
}

/* WARNING: This write function is far beyond complete, 
 * data block allocation and vnode expansion support should be added
 */
ssize_t write(int fildes, void *buf, size_t nbyte)
{
	blkcnt_t blk, oldblk;
	off_t disp;
	size_t xfer = nbyte, i=0;
	struct scache *scp;
	struct ofile *ofp = (struct ofile*)fildes;

	/* do the data copy */
	while(xfer) {
		/* calculate pos defined block number
		 * and displacement.
		 */
		blk  = ofp->pos/ofp->mount->blksize;
		blk += ofp->node->blkentry;
		blk += fs_daddr(ofp->mount->fs, ofp->mount->sblk);
		disp = ofp->pos%ofp->mount->blksize;

		/* load block to cache if not loaded */
		if(oldblk != blk) {
			scp = scache_get(ofp->node->dev,
						blk, ofp->mount->blksize);
			oldblk = blk;
		}
	
		/* do the copy */
		if((xfer+disp) > ofp->mount->blksize) {
			memcpy(&scp->buf[disp], &((char*)buf)[i],
					ofp->mount->blksize - disp);
			scp->mode |= S_CHANGED;
			ofp->pos  += ofp->mount->blksize - disp;
			disp  = 0;
			xfer -= ofp->mount->blksize - disp;
			i    += ofp->mount->blksize - disp;
		} else {
			memcpy(&scp->buf[disp], &((char*)buf)[i], xfer);
			scp->mode |= S_CHANGED;
			ofp->pos  += xfer;
			disp  = 0;
			xfer -= xfer;
			i    += xfer;
		}

		/* check end of file with pos */
		if(ofp->pos > ofp->node->size) {
			ofp->pos = ofp->node->size;
			break;
		}
	}
	return nbyte;
}

