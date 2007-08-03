/*****************************************************************************
 * Micron System V3 - Device Driver - I386 - TTY
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <config.h>
#include <device.h>
#include <libc.h>

/* IMPLEMENTATION NOTICE:
 *   This partition driver is only for hdd.c and must be initialized after hdd
 *   The following macros must be defined prior to use this driver:
 *   BLK_HDDP		- Block device major ID for hddp
 *   NPARTITION		- Number of supported partitions
 */

// MFS Super block data structure
struct mfs_superblk {
	// identifiers
	u8_t  s_magic[4];	// Magic identifier

	// block location
	u16_t s_kblk;		// Inode bitmap block
	u16_t s_kblkcnt;	// Inode bitmap block count
	u32_t s_ibmp;		// Inode bitmap block
	u32_t s_ibmpcnt;	// Inode bitmap block count
	u32_t s_dbmp;		// Data bitmap block
	u32_t s_dbmpcnt;	// Data bitmap block count
	u32_t s_iblk;		// Inode block
	u32_t s_iblkcnt;	// Inode block count
	u32_t s_dblk;		// Data block
	u32_t s_dblkcnt;	// Data block count
};

struct dev_blk *hddp_dev;

enum hddp_type
{
	MFS
};

char hddp_typestr[][8] = {"MFS"};

struct hddp_data
{
	unsigned begin;
	unsigned end;
	enum hddp_type type;
}hddp_data[NPARTION];

int hddp_open(id_t id, int oflag, mode_t mode)
{
	return dev_open(BLKDEV, DEVID(BLK_HDD, 0), oflag, mode);
}

int hddp_close(id_t id)
{
	return dev_close(BLKDEV, DEVID(BLK_HDD, 0));
}

int hddp_read(id_t id, char *buf, off_t cnt)
{
	return dev_read(BLKDEV, DEVID(BLK_HDD, 0), buf, cnt);
}

int hddp_write(id_t id, char *buf, off_t cnt)
{
	return dev_write(BLKDEV, DEVID(BLK_HDD, 0), buf, cnt);
}

int hddp_lseek(id_t id, off_t offset, int whence)
{
	if(MINOR(id) >= NPARTION) {
		return -1;
	}
	if(offset > (hddp_data[MINOR(id)].end - hddp_data[MINOR(id)].begin)) {
		return -1;	// operation excceed limit
	}
	return dev_lseek(BLKDEV, DEVID(BLK_HDD, 0),
			hddp_data[MINOR(id)].begin + offset, whence);
}

int hddp_ioctl(id_t id, int cmd, int arg)
{
	return dev_ioctl(BLKDEV, DEVID(BLK_HDD, 0), cmd, arg);
}

int hddp_init()
{
	char buf[512];

	/* initialize operations */
	hddp_dev = &dev_blk[BLK_HDDP];
	hddp_dev->open = hddp_open;
	hddp_dev->close= hddp_close;
	hddp_dev->read = hddp_read;
	hddp_dev->write= hddp_write;
	hddp_dev->lseek= hddp_lseek;
	hddp_dev->ioctl= hddp_ioctl;

	/* bootstraping first partition */
	hddp_data[0].begin = 1;
	hddp_data[0].end   = 2;

	/* probing various file systems */
	int i;
	for(i=0; i<NPARTION; i++) {
		/* seek for 1st sector for identification info */
		if(dev_lseek(BLKDEV, DEVID(BLK_HDDP, i), 1, SEEK_SET) == -1) {
			return 0;
		}

		/* read to buffer */
		dev_read(BLKDEV, DEVID(BLK_HDDP, i), buf, 1);

		/* Type probes */
		if(!strncmp(buf, "MFS", 3)) {
			hddp_data[i].type = MFS;
		} else {
			hddp_data[i].type = -1;
		}

		/* get end sector of partition */
		switch(hddp_data[i].type) {
		case MFS:
		{
			struct mfs_superblk *sblk = (struct mfs_superblk*)buf;
			hddp_data[i].end = hddp_data[i].begin + 
				sblk->s_dblk + sblk->s_dblkcnt;
			break;
		}
		default:
			return 0;
		}

		/* print probed fs info */
		kprintf("\nhda%d: %d-%d: %s", i, 
			hddp_data[i].begin, hddp_data[i].end, hddp_typestr[i]);

		/* setup next partition begin and end */
		hddp_data[i+1].begin = hddp_data[i].end + 1;
		hddp_data[i+1].end   = hddp_data[i].end + 3;
	}
	return 0;
}

int hddp_exit()
{
	hddp_dev->open = 0;
	hddp_dev->close= 0;
	hddp_dev->read = 0;
	hddp_dev->write= 0;
	hddp_dev->lseek= 0;
	hddp_dev->ioctl= 0;
	return 0;
}

REGISTER_MODULE(hddp, "HDD Partition Layer", hddp_init, hddp_exit);

