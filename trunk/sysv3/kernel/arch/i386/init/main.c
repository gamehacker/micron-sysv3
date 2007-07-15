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
}

