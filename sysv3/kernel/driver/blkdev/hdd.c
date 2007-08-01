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
#define CMD_IDENTIFY	0xEC	/* identify device */
#define CMD_READ_DMA	0xC8	/* DMA mode read */
#define CMD_WRITE_DMA	0xCA	/* DMA mode write */
#define CMD_READ	0x20	/* PIO mode read */
#define CMD_WRITE	0x30	/* PIO mode write */
#define CMD_SEEK	0x70	/* seek function */

struct dev_blk *hdd_dev=&dev_blk[BLK_HDD];

struct hdd_stat
{
	unsigned cmd;		/* currently executing command */
	unsigned short *buf;	/* read/write buffer */
	unsigned char rready;	/* data ready signal */
	unsigned cnt;		/* transfer block count */
	unsigned C, H, S;	/* total geometry info */
	unsigned c, h, s;	/* current address info */
}hdd_stat;

/**************************** STANDARD PROCEDURES ****************************/

void hdd_command(unsigned cmd)
{
	hdd_stat.cmd = cmd;
	outportb(REG_COMMAND, cmd);
}

unsigned hdd_status()
{
	return inportb(REG_STATUS);
}

unsigned hdd_readdata()
{
	return inportw(REG_DATA);
}

void hdd_writedata(unsigned data)
{
	outportw(REG_DATA, data);
}

/**************************** DRIVER INTERFACE *******************************/

int hdd_open(id_t id, int oflag, mode_t mode)
{
	return 0;
}

int hdd_close(id_t id)
{
	return 0;
}

int hdd_read(id_t id, char *buf, off_t cnt)
{
	if(MINOR(id) >=2) {
		return -1;
	}
	hdd_stat.buf = (unsigned short*)buf;
	hdd_stat.cnt = cnt;
	outportb(REG_SECTC, cnt);
	outportb(REG_SECT, hdd_stat.s & 0xFF);
	outportb(REG_CYLL, hdd_stat.c & 0xFF);
	outportb(REG_CYLH, (hdd_stat.c>>8) & 0xFF);
	if(MINOR(id) == 0) {
		outportb(REG_DEVICE, hdd_stat.h & 0x0F);
	} else {
		outportb(REG_DEVICE, (hdd_stat.h & 0x0F)|0x10);
	}

	// clear interrupt status
	hdd_stat.rready = 0;

	// send the command
	hdd_command(CMD_READ);

	// wait for interrupt
	while(!hdd_stat.rready) asm("hlt");
	
	// Actual writing sequence
	int i = 0;
	while(hdd_status() & 0x08) {
		if(i < hdd_stat.cnt * 256) {
			hdd_stat.buf[i] = hdd_readdata();
			i++;
		}
	}
	return 0;
}

int hdd_write(id_t id, char *buf, off_t cnt)
{
	if(MINOR(id) >=2) {
		return -1;
	}
	hdd_stat.buf = (unsigned short*)buf;
	hdd_stat.cnt = cnt;
	outportb(REG_SECTC, cnt);
	outportb(REG_SECT, hdd_stat.s & 0xFF);
	outportb(REG_CYLL, hdd_stat.c & 0xFF);
	outportb(REG_CYLH, (hdd_stat.c>>8) & 0xFF);
	if(MINOR(id) == 0) {
		outportb(REG_DEVICE, hdd_stat.h & 0x0F);
	} else {
		outportb(REG_DEVICE, (hdd_stat.h & 0xF)|0x10);
	}
	hdd_command(CMD_WRITE);
	
	// Writing in
	int i = 0;
	while(hdd_status() & 0x08) {
		if(i < hdd_stat.cnt * 256) {
			hdd_writedata(hdd_stat.buf[i]);
			i++;
		}
	}
	return 0;
}

int hdd_lseek(id_t id, off_t offset, int whence)
{
	if(MINOR(id) >=2) {
		return -1;
	}
	if(whence == SEEK_SET) {
		hdd_stat.c = (offset/hdd_stat.S/hdd_stat.H) % hdd_stat.C;
		hdd_stat.h = (offset/hdd_stat.S) % hdd_stat.H;
		hdd_stat.s = offset % hdd_stat.S;
		outportb(REG_SECT, hdd_stat.s & 0xFF);
		outportb(REG_CYLL, hdd_stat.c & 0xFF);
		outportb(REG_CYLH, (hdd_stat.c>>8) & 0xFF);
		if(MINOR(id) == 0) {
			outportb(REG_DEVICE, hdd_stat.h & 0x0F);
		} else {
			outportb(REG_DEVICE, (hdd_stat.h & 0x0F)|0x10);
		}
		hdd_command(CMD_SEEK);
		DEBUG(hdd_stat.c);
		DEBUG(hdd_stat.h);
		DEBUG(hdd_stat.s);
		return 0;
	}
	return -1;
}

int hdd_ioctl(id_t id, int cmd, int arg)
{
	return 0;
}

/******************************* INTERRUPT HANDLER ***************************/

void hdd_intr1(struct Register *regs)
{
	PANIC(hdd_status() & 0x01, "instruction exec error\n");
	switch(hdd_stat.cmd) {
	case CMD_IDENTIFY:
		{
			int i=0;
			while(hdd_status() & 0x08) {
				int data = hdd_readdata();
				if(i==1) {
					hdd_stat.C = data;
				} else if (i==3) {
					hdd_stat.H = data;
				} else if (i==6) {
					hdd_stat.S = data;
				}
				i++;
			}
			kprintf("\nCylinders=%d, ", hdd_stat.C);
			kprintf("Heads=%d, ", hdd_stat.H);
			kprintf("Sectors=%d, ", hdd_stat.S);
		}
		break;
	case CMD_READ:
		kprintf("__int_read__\n");
		hdd_stat.rready = 1;
		break;
	case CMD_WRITE:
		kprintf("__int_write__\n");
		break;
	case CMD_SEEK:
		//kprintf("__int_seek__\n");
		break;
	}
	hdd_stat.cmd = 0;
}

/****************************** MODULE INTERFACE *****************************/

int hdd_init()
{
	/* Device handler initialization */
	hdd_dev->open = hdd_open;
	hdd_dev->close= hdd_close;
	hdd_dev->read = hdd_read;
	hdd_dev->write= hdd_write;
	hdd_dev->lseek= hdd_lseek;
	hdd_dev->ioctl= hdd_ioctl;

	/* Install device interrupt handler */
	irq_install(14, hdd_intr1);

	/* Require Device Information */
	hdd_command(CMD_IDENTIFY);

	return 0;
}

int hdd_exit()
{
	/* Remove entry */
	hdd_dev->open = 0;
	hdd_dev->close= 0;
	hdd_dev->read = hdd_read;
	hdd_dev->write= hdd_write;
	hdd_dev->lseek= 0;
	hdd_dev->ioctl= 0;
	return 0;
}

REGISTER_MODULE(hdd, "ATA/IDE HDD Driver", hdd_init, hdd_exit);

