/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <config.h>
#include <device.h>
#include <libc.h>

/* boot device enum */
enum kbootype
{
	FLOPPY0 = 0,
	FLOPPY1 = 1,
	HDDISK0 = 0x80,
	HDDISK1 = 0x81
};

/* boot device parameter, passed from bootloader */
enum kbootype kboot;

void kmain()
{
	/* setup file system root */
	switch(kboot) {
	case FLOPPY0:
		SYSTEM("Booted from floppy0\n");
		break;
	case FLOPPY1:
		SYSTEM("Booted from floppy1\n");
		break;
	case HDDISK0:
		SYSTEM("Booted from hda0, setting it up as root\n");
		break;
	case HDDISK1:
		SYSTEM("Booted from hdb0\n");
		break;
	}

	SYSTEM("Total System Memory: %d KB\n", arch_memsize/1024);
	kprintf("\nWelcome to Micron System Version 3\n\n");
}

