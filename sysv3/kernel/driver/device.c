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

void DeviceInit()
{
	memset(&ChrDev, 0, sizeof(struct ChrDev)*NCHRDEVS);
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
	default:
		return ENODEV;
		break;
	}
	return ENODEV;	/* We will never come here */
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
	default:
		return ENODEV;
		break;
	}
	return ENODEV;	/* We will never come here */
}

int DeviceRead (enum DevType type, id_t dID, char *buf, size_t cnt)
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
	default:
		return ENODEV;
		break;
	}
	return ENODEV;	/* We will never come here */
}

int DeviceWrite(enum DevType type, id_t dID, char *buf, size_t cnt)
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
	default:
		return ENODEV;
		break;
	}
	return ENODEV;	/* We will never come here */
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
	default:
		return ENODEV;
		break;
	}
	return ENODEV;	/* We will never come here */
}
