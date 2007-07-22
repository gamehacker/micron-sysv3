/*****************************************************************************
 * Micron System V3 - I386 Driver - Console
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Design Notes
 *     The tty device on PC architecture should hold 8 consoles, which can be
 *   switched to each other by the use of ioctl command. The driver should be
 *   capable of handling escapes including but not limited to:
 *
 *     '\n', '\t', '\b', '\r'
 *     
 * Operation ioctl command list:
 *   cmd	arg		function
 *   1		page index	display page n
 *   2		color		set display color
 *   3		any		clear screen
 *****************************************************************************/
#include <device.h>
#include <io.h>
#include <libc.h>

struct ChrDev *tty_dev;

struct tty_disp
{
	unsigned short *buf;	/* frame buffer */
	unsigned char color;	/* current display color */
	unsigned max_x;		/* maximum x position */
	unsigned max_y;		/* maximum y position */
	unsigned pos_x;		/* x position */
	unsigned pos_y;		/* y position */
}tty_disp[8];

void tty_setpage(int index)
{
	unsigned page = index * tty_disp[index].max_x * tty_disp[index].max_y;
	outport(0x3D4, 0x0C);
	outport(0x3D5, (page>>8) & 0xFF);
	outport(0x3D4, 0x0D);
	outport(0x3D5, page & 0xFF);
}

void tty_setcursor(int page, int pos_x, int pos_y)
{
	unsigned location = pos_x + pos_y * tty_disp[page].max_x;
	outport(0x3D4, 0x0E);
	outport(0x3D5, (location>>8) & 0xFF);
	outport(0x3D4, 0x0F);
	outport(0x3D5, location & 0xFF);
}

void tty_scroll(int id)
{
	unsigned short *src, *dest;
	unsigned int cnt;
	cnt = tty_disp[id].max_x * (tty_disp[id].max_y - 1);
	src = tty_disp[id].buf + tty_disp[id].max_x;
	dest= tty_disp[id].buf;
	while(cnt--) {
		*dest++ = *src++;
	}
	cnt = tty_disp[id].max_x;
	dest= tty_disp[id].buf +
		tty_disp[id].max_x * (tty_disp[id].max_y - 1);
	while(cnt--) {
		dest[cnt] = ' '|(0x0f<<8);
	}
}

// Keyboard input
int tty_read(id_t id, char *buf, size_t cnt)
{
	return 0;
}

// Screen output
int tty_write(id_t id, char *buf, size_t cnt)
{
	/* get target display struct */
	struct tty_disp *disp = &tty_disp[MINOR(id)];
	
	while(cnt--) switch(*buf) {
	case '\b':
		if(disp->pos_x > 0)
			disp->pos_x--;
		buf++;
		break;
	case '\r':
		if(disp->pos_x > 0)
			disp->pos_x = 0;
		buf++;
		break;
	case '\n':
		disp->pos_x = 0;
		disp->pos_y+= 1;
		buf++;
		break;
	case '\t':
		disp->pos_x = ((disp->pos_x/8) + (disp->pos_x%8)?1:0)*8;
		buf++;
		break;
	default:
		if(disp->pos_x >= disp->max_x) {
			disp->pos_x = 0;
			disp->pos_y++;
		}
		if(disp->pos_y >= disp->max_y) {
			tty_scroll(MINOR(id));
			disp->pos_y--;
		}
		disp->buf[disp->pos_x+disp->pos_y*disp->max_x] = 
			*buf|(disp->color<<8);
		tty_setcursor(MINOR(id), disp->pos_x+1, disp->pos_y);

		/* prepare for next character */
		disp->pos_x++;
		buf++;
		break;
	}

	return 0;
}

int tty_ioctl(id_t id, int cmd, int arg)
{
	int i;
	unsigned short *p = tty_disp[MINOR(id)].buf;
	switch(cmd) {
	case 1:				/* change current display page */
		tty_setpage(arg);
		break;
	case 2:
		tty_disp[MINOR(id)].color = arg;
		break;
	case 3:
		for(i=0; 
		    i<tty_disp[MINOR(id)].max_x * tty_disp[MINOR(id)].max_y;
		    i++) {
		   p[i] = ' '|0x0f00;
		}
		break;
	}
	return 0;
}

int tty_open(id_t id, int oflag, mode_t mode)
{
	return 0;
}

int tty_close(id_t id)
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
	int i;
	for(i=0; i<8; i++)
	{
		tty_disp[i].color = 0x0F;
		tty_disp[i].pos_x = 0;
		tty_disp[i].pos_y = 0;
		tty_disp[i].max_x = 80;
		tty_disp[i].max_y = 25;
		tty_disp[i].buf   = (unsigned short*)0xb8000 + 80*25*2*i;
	}
	
	/* clean screen get ready for use */
	tty_ioctl(0, 3, 0);

	kprintf("TTY Driver V1.0 Initialized Successfully\n");
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

