# Introduction #

Device drivers gives Micron System new hardware support, notice that this layer is should be only hardware specific. Protocols such as TCP/IP, File systems, etc. are implemented in other type of kernel modules.

In this article we describe some basis of Micron Device Driver Framework.

# Character Special Device #

```
#include <chrdev.h>
struct dev_chr
{
	int (*open )(id_t id, int oflag, mode_t mode);
	int (*close)(id_t id);
	int (*read )(id_t id, char *buf, off_t cnt);
	int (*write)(id_t id, char *buf, off_t cnt);
	int (*ioctl)(id_t id, int cmd, int arg);
};
```

# Block Special Device #

```
#include <blkdev.h>
struct dev_blk
{
	int (*open )(id_t id, int oflag, mode_t mode);
	int (*close)(id_t id);	
	int (*read )(id_t id, char *buf, off_t cnt);
	int (*write)(id_t id, char *buf, off_t cnt);
	int (*lseek)(id_t id, off_t offset, int whence);
	int (*ioctl)(id_t id, int cmd, int arg);
};

```

# Example Device Driver (Character Special) #

All device driver implementations should first refer to [Kernel Module Developer's Guide](Mod.md), and here is how to use kernel module to install a new device driver:

```
#include <device.h>
#include <libc.h>
/* IMPLEMENTATION NOTICE:
 *   The following macros must be defined prior to use this driver:
 *   CHR_DEVICENAME      - Char device major ID in config.h
 */
struct dev_chr *device_dev = &dev_chr[CHR_DEVICENAME];
int device_open(id_t id, int oflag, mode_t mode)
{
    return 0;
}
int device_close(id_t id)
{
    return 0;
}
int device_read (id_t id, char *buf, off_t cnt)
{
    return 0;
}
int device_write(id_t id, char *buf, off_t cnt)
{
    return 0;
}
int device_ioctl(id_t id, int cmd, int arg)
{
    return 0;
}
int device_init()
{
    if(device_dev->open != 0) {
        return -1;      /* device descriptor already used */
    }
    device_dev->open = device_open;
    device_dev->close= device_close;
    device_dev->read = device_read;
    device_dev->write= device_write;
    device_dev->ioctl= device_ioctl;
    kprintf("\nhello world\n");
    return 0;           /* 0 upon success, -1 upon failure */
}
int device_exit()
{
    device_dev->open = 0;
    device_dev->close= 0;
    device_dev->read = 0;
    device_dev->write= 0;
    device_dev->ioctl= 0;
    kprintf("\ngoodbye world\n");
    return 0;    /* 0 upon success, -1 upon failure */
}
REGISTER_MODULE(device, "Device Description", device_init, device_exit);
```

NOTICE: Theoretically not all operations should be implemented as function, but if you are not familiar with the internal working of modulization, please implement them all.

Please put your source **.c file under /device/$TYPE/, according to its type it belongs.**

# How To Add A New Device Interface & Install Point #

The effectiency of a device driver is measured by the perfectness of its operating interface, if you found any of thoese device type and interfaces not matching yours, please follow this section.

To add a new device interface means you want to modify the kernel source tree a bit and design yourself a new interface and install point, please prepare yourself and this is not a easy task :)

If you are sure you are going to implement a interface that wanted to be included in the official source tree, please contact [Martin Tang](PsnMartin.md) first :)

Currently the files describing device interface including:
```
/include/arch-$ARCH/config.h
/include/chrdev.h
/include/blkdev.h
/include/device.h
/device/device.c
/device/chrdev/*
/device/blkdev/*
```

To add a new interface, please read those files above, then follow here:

## Step 1. Add your own interface descriptor ##

Suppose your device type is TYPE, then, first add a file under /include/TYPE.h with the following information:

```
/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_TYPE_H__
#define __MICRON_KERNEL_TYPE_H__

#include <config.h>
#include <types.h>

/* Character Device I/O Interface */
struct dev_TYPE
{
	int (*open)(id_t dID, int oflag, mode_t mode);
	int (*operation)(id_t id, Parameters...);
};

#endif
```

## Step 2. Modify /include/device.h ##

Add your TYPE to enum dev\_type declearation, for example:

```
/* Device type enumeration */
enum dev_type
{
	CHRDEV,
	BLKDEV,
	TYPEDEV              // added here
	/* DEVICE TODO: Add other device type names here */
};
```

Add your device interface install point:

```
/* Device descriptor holder */
extern struct dev_chr dev_chr[NCHRDEVS];
extern struct dev_blk dev_blk[NBLKDEVS];
extern struct dev_TYPE dev_TYPE[NTYPEDEVS]; // added here
```

Notice: the NTYPEDEVS is defined in /include/arch-ARCH/config.h

Add your new operations to externs:

```
/* Device operation method function interface */
extern void  dev_init();
extern int   dev_open (enum dev_type type, id_t dID, int oflag, mode_t mode);
extern int   dev_close(enum dev_type type, id_t dID);
extern int   dev_lseek(enum dev_type type, id_t dID, off_t off, int whence);
extern int   dev_read (enum dev_type type, id_t dID, char *buf, off_t cnt);
extern int   dev_write(enum dev_type type, id_t dID, char *buf, off_t cnt);
extern int   dev_ioctl(enum dev_type type, id_t dID, int cmd, int arg);
extern int   dev_operation(enum dev_type type, id_t dID, Parameters...);  // added here
...
/* DEVICE TODO: Add new device model operations here */
```

## Step 3. Modify /device/device.c ##

Add your device to handlers of already existed operations, for example:

```

int dev_open(enum dev_type type, id_t dID, int oflag, mode_t mode)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed chrdev limit\n");
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
	case TYPEDEV:
		MSG(MAJOR(dID) > NTYPEDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NTYPEDEVS) {
			return -1;
		}
		// Check if device exists
		if(dev_TYPE[MAJOR(dID)].open == 0) {
			return ENODEV;
		} else {
			return dev_TYPE[MAJOR(dID)].open(dID, oflag, mode);
		}
		break;
	}
	return ENODEV;
}

```

Add your own operations:

```

int dev_operation(enum dev_type type, id_t dID, Parameters...)
{
	switch(type) {
	case CHRDEV:
		MSG(MAJOR(dID) > NCHRDEVS, "dID excceed chrdev limit\n");
		if(MAJOR(dID) > NCHRDEVS) {
			return -1;
		}
		break;
	case BLKDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		break;
	case TYPEDEV:
		MSG(MAJOR(dID) > NBLKDEVS, "dID excceed blkdev limit\n");
		if(MAJOR(dID) > NBLKDEVS) {
			return -1;
		}
		// Check if device exists
		if(dev_TYPE[MAJOR(dID)].operation == 0) {
			return ENODEV;
		} else {
			return dev_TYPE[MAJOR(dID)].operation(dID, Parameters...);
		}

		break;
	}
	return ENODEV;
}

```