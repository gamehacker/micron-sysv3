/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <module.h>
#include <device.h>

void kmain()
{
	modinit();
//	char *ch = (char*)0xb8000;
//	ch[160] = 'q';
//	ch[161] = 0xc0;
//	ch[82] = 'x';
//	ch[83] = 0xac;
	DeviceOpen(CHRDEV, DEVID(0, 0), 0, 0);
	DeviceWrite(CHRDEV, DEVID(0, 0), "a\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "b\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "c\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "d\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "e\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "f\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "g\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "h\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "i\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "j\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "k\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "l\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "m\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "n\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "o\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "p\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "q\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "r\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "s\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "t\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "u\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "v\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "w\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "x\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "y\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "z\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "1\tbcde\nfghi", 11);
	DeviceWrite(CHRDEV, DEVID(0, 0), "2\tbcde\nfghi", 11);
}

