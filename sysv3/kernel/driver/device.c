/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <device.h>
#include <libc.h>

/* Various device mount points */
struct ChrDev ChrDev[NCHRDEVS];
struct BlkDev BlkDev[NBLKDEVS];

void DeviceInit()
{
	memset(&ChrDev, 0, sizeof(struct ChrDev)*NCHRDEVS);
	memset(&BlkDev, 0, sizeof(struct BlkDev)*NBLKDEVS);
}

int DeviceOpen (enum DevType type, id_t dID, int oflag, mode_t mode)
{
	switch(type) {
	case CHRDEV:
		// Check if device exists
		if(ChrDev[MAJOR(dID)].open == 0) {
			return ENODEV;
		} else {
			return ChrDev[MAJOR(dID)].open(dID, oflag, mode);
		}
		break;
	case BLKDEV:
		// Check if device exists
		if(BlkDev[MAJOR(dID)].open == 0) {
			return ENODEV;
		} else {
			return BlkDev[MAJOR(dID)].open(dID, oflag, mode);
		}
		break;
	}
	return ENODEV;
}

int DeviceClose(enum DevType type, id_t dID)
{
	switch(type) {
	case CHRDEV:
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(ChrDev[MAJOR(dID)].open == 0 ||
		   ChrDev[MAJOR(dID)].close== 0) {
			return ENODEV;
		} else {
			return ChrDev[MAJOR(dID)].close(dID);
		}
		break;
	case BLKDEV:
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(BlkDev[MAJOR(dID)].open == 0 ||
		   BlkDev[MAJOR(dID)].close== 0) {
			return ENODEV;
		} else {
			return BlkDev[MAJOR(dID)].close(dID);
		}
		break;
	}
	return ENODEV;
}

int DeviceRead (enum DevType type, id_t dID, char *buf, off_t cnt)
{
	switch(type) {
	case CHRDEV:
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(ChrDev[MAJOR(dID)].open == 0 ||
		   ChrDev[MAJOR(dID)].read == 0) {
			return ENODEV;
		} else {
			return ChrDev[MAJOR(dID)].read(dID, buf, cnt);
		}
		break;
	case BLKDEV:
		break;
	}
	return ENODEV;
}

int DeviceWrite(enum DevType type, id_t dID, char *buf, off_t cnt)
{
	switch(type) {
	case CHRDEV:
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(ChrDev[MAJOR(dID)].open == 0 ||
		   ChrDev[MAJOR(dID)].write== 0) {
			return ENODEV;
		} else {
			return ChrDev[MAJOR(dID)].write(dID, buf, cnt);
		}
		break;
	case BLKDEV:
		break;
	}
	return ENODEV;
}

int DeviceRdwr (enum DevType type, id_t dID, mode_t mode, char *buf, 
		blkcnt_t sectors)
{
	switch(type) {
	case CHRDEV:
		break;
	case BLKDEV:
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(BlkDev[MAJOR(dID)].open == 0 ||
		   BlkDev[MAJOR(dID)].rdwr == 0) {
			return ENODEV;
		} else {
			return BlkDev[MAJOR(dID)].rdwr(dID, mode, buf, sectors);
		}
		break;
	}
	return ENODEV;
}

int DeviceIoctl(enum DevType type, id_t dID, int cmd, int arg)
{
	switch(type) {
	case CHRDEV:
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(ChrDev[MAJOR(dID)].open == 0 ||
		   ChrDev[MAJOR(dID)].ioctl== 0) {
			return ENODEV;
		} else {
			return ChrDev[MAJOR(dID)].ioctl(dID, cmd, arg);
		}
		break;
	case BLKDEV:
		// Check if device exists, open operation is always checked
		// as the existence of device entry
		if(BlkDev[MAJOR(dID)].open == 0 ||
		   BlkDev[MAJOR(dID)].ioctl== 0) {
			return ENODEV;
		} else {
			return BlkDev[MAJOR(dID)].ioctl(dID, cmd, arg);
		}
		break;
	}
	return ENODEV;
}

