/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <device.h>
#include <libc.h>

/* Various device mount points */
struct dev_chr dev_chr[NCHRDEVS];
struct dev_blk dev_blk[NBLKDEVS];

void dev_init()
{
	memset(&dev_chr, 0, sizeof(struct dev_chr)*NCHRDEVS);
	memset(&dev_blk, 0, sizeof(struct dev_blk)*NBLKDEVS);
}

int dev_open(enum dev_type type, id_t dID, int oflag, mode_t mode)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NCHRDEVS) {
			return -1;
		}
		// Check if device exists
		if(dev_chr[MAJOR(dID)].open == 0) {
			return ENODEV;
		} else {
			return dev_chr[MAJOR(dID)].open(dID, oflag, mode);
		}
		break;
	case BLKDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		// Check if device exists
		if(dev_blk[MAJOR(dID)].open == 0) {
			return ENODEV;
		} else {
			return dev_blk[MAJOR(dID)].open(dID, oflag, mode);
		}
		break;
	}
	return ENODEV;
}

int dev_close(enum dev_type type, id_t dID)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NCHRDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_chr[MAJOR(dID)].open == 0 ||
		   dev_chr[MAJOR(dID)].close== 0) {
			return ENODEV;
		} else {
			return dev_chr[MAJOR(dID)].close(dID);
		}
		break;
	case BLKDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_blk[MAJOR(dID)].open == 0 ||
		   dev_blk[MAJOR(dID)].close== 0) {
			return ENODEV;
		} else {
			return dev_blk[MAJOR(dID)].close(dID);
		}
		break;
	}
	return ENODEV;
}

int dev_read (enum dev_type type, id_t dID, char *buf, off_t cnt)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NCHRDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_chr[MAJOR(dID)].open == 0 ||
		   dev_chr[MAJOR(dID)].read == 0) {
			return ENODEV;
		} else {
			return dev_chr[MAJOR(dID)].read(dID, buf, cnt);
		}
		break;
	case BLKDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_blk[MAJOR(dID)].open == 0 ||
		   dev_blk[MAJOR(dID)].read == 0) {
			return ENODEV;
		} else {
			return dev_blk[MAJOR(dID)].read(dID, buf, cnt);
		}
		break;
	}
	return ENODEV;
}

int dev_write(enum dev_type type, id_t dID, char *buf, off_t cnt)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NCHRDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_chr[MAJOR(dID)].open == 0 ||
		   dev_chr[MAJOR(dID)].write== 0) {
			return ENODEV;
		} else {
			return dev_chr[MAJOR(dID)].write(dID, buf, cnt);
		}
		break;
	case BLKDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_blk[MAJOR(dID)].open == 0 ||
		   dev_blk[MAJOR(dID)].write== 0) {
			return ENODEV;
		} else {
			return dev_blk[MAJOR(dID)].write(dID, buf, cnt);
		}
		break;
	}
	return ENODEV;
}

int dev_ioctl(enum dev_type type, id_t dID, int cmd, int arg)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NCHRDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_chr[MAJOR(dID)].open == 0 ||
		   dev_chr[MAJOR(dID)].ioctl== 0) {
			return ENODEV;
		} else {
			return dev_chr[MAJOR(dID)].ioctl(dID, cmd, arg);
		}
		break;
	case BLKDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_blk[MAJOR(dID)].open == 0 ||
		   dev_blk[MAJOR(dID)].ioctl== 0) {
			return ENODEV;
		} else {
			return dev_blk[MAJOR(dID)].ioctl(dID, cmd, arg);
		}
		break;
	}
	return ENODEV;
}

int dev_lseek(enum dev_type type, id_t dID, int off, int whence)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NCHRDEVS) {
			return -1;
		}
		break;
	case BLKDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(dev_blk[MAJOR(dID)].open == 0 ||
		   dev_blk[MAJOR(dID)].lseek== 0) {
			return ENODEV;
		} else {
			return dev_blk[MAJOR(dID)].lseek(dID, off, whence);
		}
		break;
	}
	return ENODEV;
}

