/*****************************************************************************
 * Micron System V3 - Device Driver - I386 - TTY
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <device.h>
#include <config.h>

struct BlkDev *hdd_dev=&BlkDev[BLK_HDD];

//struct hdd_data
//{
//}hdd_data;

int hdd_open(id_t id, int oflag, mode_t mode)
{
	return 0;
}

int hdd_close(id_t id)
{
	return 0;
}

int hdd_rdwr(id_t id, mode_t mode, char *buf, blkcnt_t sectors)
{
	return 0;
}

int hdd_ioctl(id_t id, int cmd, int arg)
{
	return 0;
}

int hdd_init()
{
	/* Device handler initialization */
	hdd_dev->open = hdd_open;
	hdd_dev->close= hdd_close;
	hdd_dev->rdwr = hdd_rdwr;
	hdd_dev->ioctl= hdd_ioctl;
	return 0;
}

int hdd_exit()
{
	return 0;
}

REGISTER_MODULE(hdd, "ATA/IDE HDD Driver", hdd_init, hdd_exit);

