/*****************************************************************************
 * Micron System V3 - Device Driver - I386 - TTY
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <device.h>
#include <config.h>
#include <io.h>
#include <libc.h>
#include <irq.h>

/* IMPLEMENTATION NOTICE: 
 *   The following macros must be fined prior to use this driver:
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
#define CMD_INITIALIZE	0x91	/* initialize drive parameters */
#define CMD_IDENTIFY	0xEC	/* identify device */
#define CMD_READ_DMA	0xC8	/* DMA mode read */
#define CMD_WRITE_DMA	0xCA	/* DMA mode write */

struct BlkDev *hdd_dev=&BlkDev[BLK_HDD];

struct hdd_stat
{
}hdd_stat;

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

void hdd_intr(struct Register *regs)
{
	DEBUG(inportb(REG_STATUS));
	DEBUG(inportw(REG_DATA));
	DEBUG(inportw(REG_DATA));
	DEBUG(inportw(REG_DATA));
	DEBUG(inportw(REG_DATA));
	DEBUG(inportw(REG_DATA));
}

int hdd_init()
{
	/* Device handler initialization */
	hdd_dev->open = hdd_open;
	hdd_dev->close= hdd_close;
	hdd_dev->rdwr = hdd_rdwr;
	hdd_dev->ioctl= hdd_ioctl;

	/* Install device interrupt handler */
	irq_install(14, hdd_intr);

	/* TEST ONLY */
	kprintf("\n--- HDD TEST ---\n");
	outportb(REG_DEVICE , 0x00);
	outportb(REG_COMMAND, CMD_IDENTIFY);
	kprintf("--- HDD TEST END ---\n");
	/* TEST ONLY */
	return 0;
}

int hdd_exit()
{
	/* Remove entry */
	hdd_dev->open = 0;
	hdd_dev->close= 0;
	hdd_dev->rdwr = 0;
	hdd_dev->ioctl= 0;
	return 0;
}

REGISTER_MODULE(hdd, "ATA/IDE HDD Driver", hdd_init, hdd_exit);

