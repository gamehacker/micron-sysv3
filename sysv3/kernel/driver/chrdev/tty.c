/*****************************************************************************
 * Micron System V3 - I386 Driver - Console
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Device Interface Specification
 *     The tty device on PC architecture should hold 8 consoles, which can be
 *   switched to each other by the call of ioctl command. The driver should be
 *   capable of handling escapes including:
 *
 *     '\n', '\t', '\b', '\r'
 *     
 *     Operation ioctl command list:
 *     cmd	arg
 *****************************************************************************/
#include <device.h>

struct
{
	unsigned char sem;	/* records number of opened handles */
	unsigned char pos_x;	/* current writting position X on screen */
	unsigned char pos_y;	/* current writting position Y on screen */
	unsigned char max_x;	/* maximum X coordinate */
	unsigned char max_y;	/* maximum Y coordinate */
	unsigned short *buf;	/* display memory address to write to */
}tty_data[8];

// Current device descriptor
struct ChrDev *tty_dev;

int tty_open(id_t id, int oflag, mode_t mode)
{
	return 0;
}

int tty_close(id_t id)
{
	return 0;
}

int tty_read(id_t id, char *buf, size_t cnt)
{
	return 0;
}

int tty_write(id_t id, char *buf, size_t cnt)
{
	return 0;
}

int tty_ioctl(id_t id, int cmd, int arg)
{
	return 0;
}

int tty_init()
{
	tty_dev = DeviceAlloc(CHRDEV);
	if(tty_dev == (struct ChrDev*)ENODEV) {
		return -1;	// Initialization Failure
	}
	tty_dev->open = tty_open;
	tty_dev->close= tty_close;
	tty_dev->read = tty_read;
	tty_dev->write= tty_write;
	tty_dev->ioctl= tty_ioctl;
	return 0;
}

int tty_exit()
{
	if(tty_dev == (struct ChrDev*)ENODEV)
		return -1;	// Device not installed
	tty_dev->open = 0;
	tty_dev->close= 0;
	tty_dev->read = 0;
	tty_dev->write= 0;
	tty_dev->ioctl= 0;
	return 0;
}

MODULE_INIT(tty_init);
MODULE_EXIT(tty_exit);

