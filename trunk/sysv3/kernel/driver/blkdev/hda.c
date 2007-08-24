/*****************************************************************************
 * Micron System V3 - Device Driver - I386 - HDA
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <device.h>
#include <scache.h>
#include <config.h>
#include <io.h>
#include <libc.h>
#include <irq.h>

/* IMPLEMENTATION NOTICE:
 *   The following macros must be defined prior to use this driver:
 *   BLK_HDD		- Block device major ID
 *   BLK_HDD_IOBASE	- Device I/O base
 */

/* Register sets */
#define REG_DATA	BLK_HDD_IOBASE		/* data register */
#define REG_ERROR	BLK_HDD_IOBASE + 1	/* error register */
#define REG_FEATURE	BLK_HDD_IOBASE + 1	/* feature register */
#define REG_SECTC	BLK_HDD_IOBASE + 2	/* sector count */
#define REG_SECT	BLK_HDD_IOBASE + 3	/* CHS: sector number */
						/* LBA: bits 0-7 */
#define REG_CYLL	BLK_HDD_IOBASE + 4	/* CHS: cylinder low */
						/* LBA: bits 15-8 */
#define REG_CYLH	BLK_HDD_IOBASE + 5	/* CHS: cylinder high */
						/* LBA: bits 23-16 */
#define REG_DEVICE	BLK_HDD_IOBASE + 6	/* drive/head */
#define REG_STATUS	BLK_HDD_IOBASE + 7	/* status register */
#define REG_COMMAND	BLK_HDD_IOBASE + 7	/* command register */

/* Command Sets */
#define CMD_CALIBRATE	0x10	/* calibrate device */
#define CMD_IDENTIFY	0xEC	/* identify device */
#define CMD_READ_DMA	0xC8	/* DMA mode read */
#define CMD_WRITE_DMA	0xCA	/* DMA mode write */
#define CMD_READ	0x20	/* PIO mode read */
#define CMD_WRITE	0x30	/* PIO mode write */
#define CMD_SEEK	0x70	/* seek function */

struct dev_blk *hda_dev=&dev_blk[BLK_HDA];

struct hda_stat
{
	unsigned cmd;		/* currently executing command */
	unsigned short *buf;	/* read/write buffer */
	unsigned char rready;	/* data ready signal */
	unsigned cnt;		/* transfer block count */
	unsigned C, H, S;	/* total geometry info */
	unsigned c, h, s;	/* current address info */
}hda_stat;

struct hda_partition
{
	unsigned begin;		/* start of lba */
	unsigned end;		/* end of lba */
}hda_partition[NVFSMNTS];

/**************************** STANDARD PROCEDURES ****************************/

void hda_command(unsigned cmd)
{
	hda_stat.cmd = cmd;
	outportb(REG_COMMAND, cmd);
}

unsigned hda_status()
{
	return inportb(REG_STATUS);
}

unsigned hda_readdata()
{
	return inportw(REG_DATA);
}

void hda_writedata(unsigned data)
{
	outportw(REG_DATA, data);
}

/**************************** DRIVER INTERFACE *******************************/

int hda_open(dev_t id, int oflag, mode_t mode)
{
	return 0;
}

int hda_close(dev_t id)
{
	return 0;
}

int hda_read(dev_t id, char *buf, off_t cnt)
{
	hda_stat.buf = (unsigned short*)buf;
	hda_stat.cnt = cnt;
	outportb(REG_SECTC, cnt);
	outportb(REG_SECT, hda_stat.s & 0xFF);
	outportb(REG_CYLL, hda_stat.c & 0xFF);
	outportb(REG_CYLH, (hda_stat.c>>8) & 0xFF);
	outportb(REG_DEVICE, hda_stat.h & 0x0F);

	// clear interrupt status
	hda_stat.rready = 0;

	// send the command
	hda_command(CMD_READ);

	// wait for interrupt
	while(!hda_stat.rready);

	// Actual writing sequence
	int i = 0;
	while(hda_status() & 0x08) {
		if(i < hda_stat.cnt * 256) {
			hda_stat.buf[i] = hda_readdata();
			i++;
		}
	}
	return 0;
}

int hda_write(dev_t id, char *buf, off_t cnt)
{
	hda_stat.buf = (unsigned short*)buf;
	hda_stat.cnt = cnt;
	outportb(REG_SECTC, cnt);
	outportb(REG_SECT, hda_stat.s & 0xFF);
	outportb(REG_CYLL, hda_stat.c & 0xFF);
	outportb(REG_CYLH, (hda_stat.c>>8) & 0xFF);
	outportb(REG_DEVICE, hda_stat.h & 0x0F);
	hda_command(CMD_WRITE);

	// Writing in
	int i = 0;
	while(hda_status() & 0x08) {
		if(i < hda_stat.cnt * 256) {
			hda_writedata(hda_stat.buf[i]);
			i++;
		}
	}
	return 0;
}

int hda_lseek(dev_t id, off_t offset, int whence)
{
	if(MINOR(id) > 0) {
		if(MINOR(id) > NVFSMNTS) {
			MSG(1, "Partition no over limit");
			return -1;
		}

		/* request is on partitions */
		offset+= hda_partition[MINOR(id)-1].begin;
		if(offset > hda_partition[MINOR(id)-1].end) {
			MSG(1, "Partition lba over limit");
			kprintf("<>%d\n", hda_partition[MINOR(id)-1].end);
			return -1;
		}
	}

	if(offset > hda_stat.C * hda_stat.H * hda_stat.S) {
		return -1;
	}

	hda_stat.c = (offset/hda_stat.S/hda_stat.H) % hda_stat.C;
	hda_stat.h = (offset/hda_stat.S) % hda_stat.H;
	hda_stat.s = offset % hda_stat.S;
	return 0;
}

int hda_ioctl(dev_t id, int cmd, int arg)
{
	return 0;
}

/******************************* INTERRUPT HANDLER ***************************/

void hda_intr(struct Register *regs)
{
	PANIC(hda_status() & 0x01, "instruction exec error\n");
	switch(hda_stat.cmd) {
	case CMD_IDENTIFY:
	{
		int i=0;
		while(hda_status() & 0x08) {
			int data = hda_readdata();
			if(i==1) {
				hda_stat.C = data;
			} else if (i==3) {
				hda_stat.H = data;
			} else if (i==6) {
				hda_stat.S = data;
			}
			i++;
		}
		kprintf("\n  hda:\tCylinders=%d, ", hda_stat.C);
		kprintf("Heads=%d, ", hda_stat.H);
		kprintf("Sectors=%d", hda_stat.S);
		hda_stat.rready = 1;
		break;
	}
	case CMD_READ:
		//kprintf("__int_read__\n");
		hda_stat.rready = 1;
		break;
	case CMD_WRITE:
		//kprintf("__int_write__\n");
		hda_stat.rready = 1;
		break;
	}
	hda_stat.cmd = 0;
}

/****************************** MODULE INTERFACE *****************************/

/************ For Partition Detection Use *************/
/* MFS super block */
struct mfs_sblk {
	char      s_magic[4];	// Magic identifier
	u16_t     s_kblk;	// Inode bitmap block
	u16_t     s_kblkcnt;	// Inode bitmap block count
	blkcnt_t  s_ibmp;	// Inode bitmap block
	blkcnt_t  s_ibmpcnt;	// Inode bitmap block count
	blkcnt_t  s_dbmp;	// Data bitmap block
	blkcnt_t  s_dbmpcnt;	// Data bitmap block count
	blkcnt_t  s_iblk;	// Inode block
	blkcnt_t  s_iblkcnt;	// Inode block count
	blkcnt_t  s_dblk;	// Data block
	blkcnt_t  s_dblkcnt;	// Data block count
	blksize_t s_blksize;	// Block size
};
/************ For Partition Detection Use *************/


int hda_init()
{
	int i;
	char buf[512];

	/* Device handler initialization */
	hda_dev->open = hda_open;
	hda_dev->close= hda_close;
	hda_dev->read = hda_read;
	hda_dev->write= hda_write;
	hda_dev->lseek= hda_lseek;
	hda_dev->ioctl= hda_ioctl;

	/* Install device interrupt handler */
	irq_install(14, hda_intr);

	/* Require Device Information */
	hda_stat.rready = 0;
	hda_command(CMD_IDENTIFY);
	while(!hda_stat.rready);

	/* Detection of partitions */
	hda_partition[0].begin = 2;	/* first partition at 1st sector */
	for(i=0; i<NVFSMNTS; i++) {
		if(hda_lseek(DEVID(BLK_HDA, 0), hda_partition[i].begin, 0)
								== -1) {
			break;
		}
		hda_read(0, buf, 1);

		/* detection mechanisms */
		if(!strncmp(buf, "MFS", 3)) {
			struct mfs_sblk *sp  = (struct mfs_sblk*)buf;
			hda_partition[i].end = (sp->s_dblk + sp->s_dblkcnt) *
						(sp->s_blksize/512);
			kprintf("\n  hda%d:\t%d->%d: Micron File System", i+1,
				hda_partition[i].begin, hda_partition[i].end);
			hda_partition[i+1].begin = hda_partition[i].end+1;
			continue;
		} else {
			break;
		}
	}

	return 0;
}

int hda_exit()
{
	/* Remove entry */
	hda_dev->open = 0;
	hda_dev->close= 0;
	hda_dev->read = 0;
	hda_dev->write= 0;
	hda_dev->lseek= 0;
	hda_dev->ioctl= 0;
	return 0;
}

REGISTER_MODULE(DRV, hda, "ATA/IDE Hard Disk A", hda_init, hda_exit);

