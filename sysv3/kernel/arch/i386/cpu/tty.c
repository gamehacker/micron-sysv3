/*****************************************************************************
 * Micron System V3 - I386 Specific - TTY
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * Copyright (C) 2007, Jerry Liu
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Design Notes
 *   To include this driver, the following macros in config.h must be defined:
 *   CHR_TTY	-	TTY character device major ID
 *
 * Operation ioctl command list:
 *   cmd	arg		function
 *   1		page index	display page n
 *   2		color		set display color
 *   3		any		clear screen
 *   4		X value		set X
 *   5		Y value		set Y
 *****************************************************************************/
#include <device.h>
#include <io.h>
#include <libc.h>
#include <irq.h>

/* Device descriptor */
struct dev_chr *tty_dev = &dev_chr[CHR_TTY];

/************************** TTY Driver Begin *********************************/

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
	outportb(0x3D4, 0x0C);
	outportb(0x3D5, (page>>8) & 0xFF);
	outportb(0x3D4, 0x0D);
	outportb(0x3D5, page & 0xFF);
}

void tty_setcursor(int page, int pos_x, int pos_y)
{
	unsigned location = pos_x + pos_y * tty_disp[page].max_x;
	outportb(0x3D4, 0x0E);
	outportb(0x3D5, (location>>8) & 0xFF);
	outportb(0x3D4, 0x0F);
	outportb(0x3D5, location & 0xFF);
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
	dest= tty_disp[id].buf + tty_disp[id].max_x * (tty_disp[id].max_y - 1);
	while(cnt--) {
		dest[cnt] = ' '|(0x0f<<8);
	}
}

// Screen output
int tty_write(dev_t id, char *buf, off_t cnt)
{
	unsigned temp;
	/* get target display struct */
	struct tty_disp *disp = &tty_disp[MINOR(id)];
	
	while(cnt--) {
		if(disp->pos_x >= disp->max_x) {
			disp->pos_x = 0;
			disp->pos_y++;
		}
		if(disp->pos_y >= disp->max_y) {
			tty_scroll(MINOR(id));
			disp->pos_y--;
		}
		switch(*buf) {
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
			temp = disp->pos_x;
			disp->pos_x = (temp/8 + 1)*8;
			buf++;
			break;
		default:
			disp->buf[disp->pos_x+disp->pos_y*disp->max_x] = 
				*buf|(disp->color<<8);
	
			/* prepare for next character */
			disp->pos_x++;
			buf++;
			break;
		}

		/* update cursor */
		tty_setcursor(MINOR(id), disp->pos_x, disp->pos_y);
	}

	return 0;
}

/*************************** BEGIN OF KEYBOARD DEV ***************************/

#define BUF_SIZE 16

struct tty_queue {
	long head;
	long tail;
	char buf[BUF_SIZE];
	char map[BUF_SIZE];
}tty_kbd_buf = {0, 0};

void tty_kbd_buf_push(unsigned char scan_code)
{
	MSG(tty_kbd_buf.map[tty_kbd_buf.head]==0x55, "kbd buffer overflow\n");
	if(tty_kbd_buf.map[tty_kbd_buf.head]==0) {
		tty_kbd_buf.buf[tty_kbd_buf.head] = scan_code;
		tty_kbd_buf.map[tty_kbd_buf.head] = 0x55;
		if(tty_kbd_buf.head > 0) {
			tty_kbd_buf.head--;
		} else {
			tty_kbd_buf.head = BUF_SIZE -1;
		}	
	}
}

unsigned char tty_kbd_buf_pop()
{
	unsigned char kbd_res;
	MSG(tty_kbd_buf.map[tty_kbd_buf.tail] == 0, "reading empty buffer\n");
	if(tty_kbd_buf.map[tty_kbd_buf.tail] == 0x55) {
		tty_kbd_buf.map[tty_kbd_buf.tail] = 0;
		kbd_res = tty_kbd_buf.buf[tty_kbd_buf.tail];
		if(tty_kbd_buf.tail > 0) {
			tty_kbd_buf.tail --;
		} else {
			tty_kbd_buf.tail = BUF_SIZE - 1;
		}
		return kbd_res;
	}
	return 0;
}

void tty_intr(struct Register *regs)
{
	MSG((inportb(0x64) & 0x01) != 0x01, "Buffer not full\n");
	tty_kbd_buf_push(inportb(0x60));
	kprintf("<%x>\n", tty_kbd_buf_pop());
	MSG((inportb(0x64) & 0x01) != 0x00, "Buffer not empty after read\n");
}

// Keyboard input
int tty_read(dev_t id, char *buf, off_t cnt)
{
	buf[0] = tty_kbd_buf_pop();
	return 0;
}

/*************************** Begin of Common Interface ***********************/

int tty_ioctl(dev_t id, int cmd, int arg)
{
	int i;
	struct tty_disp *tty = &tty_disp[MINOR(id)];
	switch(cmd) {
	case 1:				/* change current display page */
		tty_setpage(arg);
		break;
	case 2:
		tty->color = arg;
		break;
	case 3:
		for(i=0; 
		    i<tty->max_x * tty->max_y;
		    i++) {
		   tty->buf[i] = ' '|0x0f00;
		}
		break;
	case 4:
		tty->pos_x = arg;
		break;
	case 5:
		tty->pos_y = arg;
		break;
	}
	return 0;
}

int tty_open(dev_t id, int oflag, mode_t mode)
{
	return 0;
}

int tty_close(dev_t id)
{
	return 0;
}

int tty_init()
{
	/* Device handler initialization */
	tty_dev->open = tty_open;
	tty_dev->close= tty_close;
	tty_dev->read = tty_read;
	tty_dev->write= tty_write;
	tty_dev->ioctl= tty_ioctl;

	/* private initialization */
	int i;
	for(i=0; i<8; i++)
	{
		tty_disp[i].color = 0x0F;
		tty_disp[i].pos_x = 0;
		tty_disp[i].pos_y = 0;
		tty_disp[i].max_x = 80;
		tty_disp[i].max_y = 25;
		tty_disp[i].buf   = (unsigned short*)(0xb8000 + 
			tty_disp[i].max_x * tty_disp[i].max_y * 2 * i);
	}
	
	/* clean screen get ready for use */
	tty_ioctl(0, 3, 0);

	/* print test string */
	tty_write(0, "TTY Display ready\n\n", 19);

	/* install interrupt handler for keyboard */
	irq_install(1, tty_intr);
	return 0;
}

