/*****************************************************************************
 * Micron System V3 - I386 Driver - Console
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Device Interface Specification
 *     The tty device on PC architecture should hold 8 consoles, which can be
 *   switched to each other by the call of ioctl command. The driver should be
 *   capable of handling escapes including:
 *
 *     '\n', '\t', '\b', '\r'
 *     
 *     Operation ioctl command list:
 *     cmd	arg
 *****************************************************************************/
#include <device.h>

struct
{
	unsigned char sem;	/* records number of opened handles */
	unsigned int pos_x;	/* current writting position X on screen */
	unsigned int pos_y;	/* current writting position Y on screen */
	unsigned int max_x;	/* maximum X coordinate */
	unsigned int max_y;	/* maximum Y coordinate */
	unsigned short color;   /* the bgcolor and wordscolor;*/
	unsigned short *buf;	/* display memory address to write to */
}tty_data[8];

// Current device descriptor
struct ChrDev *tty_dev;

int cleanpass(int id, int pos_jmp)
{
	int pos = tty_data[MINOR(id)].pos_y*tty_data[MINOR(id)].max_x+tty_data[MINOR(id)].pos_x;
	for(;pos_jmp<pos;pos_jmp++)
	{
		tty_data[MINOR(id)].buf[pos_jmp] = ' '|(0x0f<<8);
	}
	return 0;
}

int bufscroll(id_t id)
{
	int max_num = tty_data[MINOR(id)].max_x*tty_data[MINOR(id)].max_y;
	int init = 0;
	for( ;init<(max_num - tty_data[MINOR(id)].max_x);init++)
	{
	   tty_data[MINOR(id)].buf[init] = tty_data[MINOR(id)].buf[init+tty_data[MINOR(id)].max_x];
	}
	init = max_num - tty_data[MINOR(id)].max_x+1;
	for(;init<max_num;init++)
	{
		tty_data[MINOR(id)].buf[init] = '\0';
	}
	tty_data[MINOR(id)].pos_x = 0;
	tty_data[MINOR(id)].pos_y = tty_data[MINOR(id)].max_y-1;
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

int tty_read(id_t id, char *buf, size_t cnt)
{
	return 0;
}

int tty_write(id_t id, char *buf, size_t cnt)
{
	int i = 0,pos_rmb = 0;
	for( ; i < cnt; i++)
	{
		pos_rmb = tty_data[MINOR(id)].pos_y*tty_data[MINOR(id)].max_x+tty_data[MINOR(id)].pos_x;
		switch( buf[i] )
		{
		case '\n':
			if(tty_data[MINOR(id)].pos_y<tty_data[MINOR(id)].max_y-1)
			{
				tty_data[MINOR(id)].pos_y += 1;
				tty_data[MINOR(id)].pos_x = 0;
				cleanpass(id,pos_rmb);
			}
			else
			{
				bufscroll(id);
			}
			break;
		case '\t':
			if(tty_data[MINOR(id)].pos_x<(tty_data[MINOR(id)].max_x-8))
			{
				tty_data[MINOR(id)].pos_x+=8;
				cleanpass(id,pos_rmb);
			}
			else
			{
			     if(tty_data[MINOR(id)].pos_y<tty_data[MINOR(id)].max_y-1)
			      {
				tty_data[MINOR(id)].pos_y += 1;
				tty_data[MINOR(id)].pos_x = 0;
				cleanpass(id,pos_rmb);
			      }
			      else
			      {
				      bufscroll(id);
			      }
			}
			break;
		case '\b':
			break;
		case '\r':
			tty_data[MINOR(id)].pos_x = 0;
			break;
		default:
			{
		       int pos = tty_data[MINOR(id)].pos_y*tty_data[MINOR(id)].max_x+tty_data[MINOR(id)].pos_x;
		       tty_data[MINOR(id)].buf[pos]=buf[i]|(0x0f<<8);
			if(tty_data[MINOR(id)].pos_x<(tty_data[MINOR(id)].max_x-1))
			{
				tty_data[MINOR(id)].pos_x+=1;
			}
			else
			{
			     if(tty_data[MINOR(id)].pos_y<tty_data[MINOR(id)].max_y-1)
			      {
				tty_data[MINOR(id)].pos_y += 1;
				tty_data[MINOR(id)].pos_x = 0;
			      }
			      else
			      {
				      bufscroll(id);
			      }
			}
		//int pos = 20;
 		//tty_data[MINOR(id)].buf[pos]=buf[i]|0xf0<<8;
			break;
			}
		}
	}
	return 0;
}

int tty_ioctl(id_t id, int cmd, int arg)
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
	int in_tty = 0;
	for(; in_tty<8; in_tty++)
	{
		tty_data[in_tty].color = 0x00;
		tty_data[in_tty].pos_x = 0;
		tty_data[in_tty].pos_y = 0;
		tty_data[in_tty].max_x = 80;
		tty_data[in_tty].max_y = 25;
		tty_data[in_tty].buf   = (unsigned short*)0xb8000;
	}
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

