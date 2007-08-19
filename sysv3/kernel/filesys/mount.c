/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <mount.h>
#include <libc.h>

struct mountab mountab[NVFSMNTS];

struct mountab *mountab_getroot()
{
	if(mountab[0].stat != 0) {
		return &mountab[0];
	}
	return 0;
}

struct mountab *mountab_get(dev_t dev)
{
	int i;

	/* search for mount point with mnt=path */
	for(i=0; i<NVFSMNTS; i++) {
		if(mountab[i].root.dev == dev) {
			return &mountab[i];
		}
	}
	return 0;	/* not found */
}

struct mountab *mountab_seek(dev_t hdev, ino_t hino)
{
	int i;

	/* search for mount point with mnt=path */
	for(i=0; i<NVFSMNTS; i++) {
		if((mountab[i].hdev == hdev) &&
		   (mountab[i].hino == hino)) {
			return &mountab[i];
		}
	}
	return 0;	/* not found */
}

int mountab_mount(dev_t hdev, ino_t hino, dev_t dev)
{
	int i;

	/* check if dev has already been mounted */
	for(i=0; i<NVFSMNTS; i++) {
		if(mountab[i].root.dev == dev) {
			return -1;	/* dev has already been mounted */
		}
	}
	
	/* search for a unused mount point */
	for(i=0; i<NVFSMNTS; i++) {
		if(mountab[i].stat == 0) {
			/* setup root node */
			mountab[i].root.ino = 0;
			mountab[i].root.dev = dev;

			/* setup host node info */
			mountab[i].hdev = hdev;
			mountab[i].hino = hino;

			/* setup fstype, auto probe */
			mountab[i].fs = fs_probe(dev);

			/* if unsupported fs type */
			if(mountab[i].fs == -1) {
				return -1;	/* mount failure */
			}

			/* setup block size */
			mountab[i].blksize = fs_blksz(mountab[i].fs, dev);

			/* load super block */
			fs_lsblk(mountab[i].fs, dev, mountab[i].sblk);

			/* mark mount point as used */
			mountab[i].stat = 1;

			return 0;
		}
	}
	return -1;
}

int mountab_umount(dev_t dev)
{
	int i;

	/* search for mount point *path */
	for(i=0; i<NVFSMNTS; i++) {
		if((mountab[i].stat != 0) &&
		   (mountab[i].root.dev == dev)) {
			/* mount string is the only allocated mark */
			mountab[i].stat = 0;
	
			return 0;
		}
	}
	return -1;
}

