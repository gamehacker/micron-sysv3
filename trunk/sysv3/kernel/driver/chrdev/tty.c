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
#include <irq.h>
#include<libc.h>
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

#define BUF_SIZE  4
//special key 
#define CAPS_KEY 58
#define INSERT_KEY 82
#define SCROLL_KEY 70
#define NUM_KEY 71
#define STAT_KEY_SIZE 4
//save sepcail key status,
//if current key's values is 1 ,the key is downa,0 respent up
unsigned char tty_spec_key_stat[STAT_KEY_SIZE];
//buf is used to save keyboard value
struct tty_queue {
  long head;
  long tail;
  char buf[BUF_SIZE];
};
struct tty_queue tty_kbd_buf;
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
int tty_read(id_t id, char *buf, off_t cnt)
{
	return 0;
}

// Screen output
int tty_write(id_t id, char *buf, off_t cnt)
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

void timer_handler(struct isr_regs* regs)
{
	
}
//set head =tail=BUF_SIZE-1 and clean kdb buff
void tty_kbd_init()
{
  int i;
  tty_kbd_buf.head=BUF_SIZE-1;
  tty_kbd_buf.tail=tty_kbd_buf.head;
  for(i=0;i<BUF_SIZE;i++)//clean kbd buff
	   tty_kbd_buf.buf[i]=0; 
  for(i=0;i<STAT_KEY_SIZE;i++)
       tty_spec_key_stat[i]=0;
}
void tty_kbd_buff_push( unsigned char scan_code)
{
  
  if (tty_kbd_buf.buf[tty_kbd_buf.head]==0)
	{
	  tty_kbd_buf.buf[tty_kbd_buf.head]=scan_code;
      if(tty_kbd_buf.head>0)
	      tty_kbd_buf.head--;
	  else
		  tty_kbd_buf.head=BUF_SIZE-1;
	}
 
	
}
/* purpose:
   the function will pop one element.
   if return value is 0,it respent that buff is empty.
   write by liubo 
   write date: 2007-07-25
*/
unsigned char tty_kbd_buff_pop()
{
  unsigned char kbd_res=0;
  if (tty_kbd_buf.buf[tty_kbd_buf.tail]>0)
	{
      kbd_res=tty_kbd_buf.buf[tty_kbd_buf.tail];
	  tty_kbd_buf.buf[tty_kbd_buf.tail]=0;//release one buffer
	  if(tty_kbd_buf.tail>0)
	      tty_kbd_buf.tail--;
	  else//it reach buffer's head
		  tty_kbd_buf.tail=BUF_SIZE-1;
	}
   return kbd_res;
}
void tty_kbd_key_press(void)
{
 while (inport(0x64) & 0x01);
 outport(0x60, 0xed);
 outport(0x60, 0x02); 
 return ;
}
void tty_key_handler(struct isr_regs* regs)
{
   unsigned char code ;//used to save keyboard's scan code
   //tty_kbd_key_press();
   code=inport(0x60);//get keyboard's scan code
   if(!(code&0x80))//key is down
	{
      if (code==28)//enter key
      {
         DEBUG(tty_kbd_buff_pop());//pop one element
      }
	  else
		{
		  
		  switch(code)
			  {
                case CAPS_KEY:
					tty_spec_key_stat[0]=!tty_spec_key_stat[0];
				    break;
                case INSERT_KEY:
                    tty_spec_key_stat[1]=!tty_spec_key_stat[1];
				    break;
				case SCROLL_KEY:
                    tty_spec_key_stat[2]=!tty_spec_key_stat[2];
				   break;
				case NUM_KEY://the key value is not exists in my laptop,please you define key value that base on your machine:)
					tty_spec_key_stat[3]=!tty_spec_key_stat[3];
				   break;
                 default:
				   DEBUG(code);
                   tty_kbd_buff_push(code);//push one element
				   break;
		      }
		}
	}
   else
    {
	   //key is up
	}
}

void rlt_clk_handler(struct isr_regs* regs)
{
	 
}
int tty_init()
{
	
	
	tty_dev = &ChrDev[0];
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
		tty_disp[i].buf   = (unsigned short*)(0xb8000 + 
			tty_disp[i].max_x * tty_disp[i].max_y * 2 * i);
	}
	
	/* clean screen get ready for use */
	tty_ioctl(0, 3, 0);

	kprintf("TTY Driver V1.0 Initialized Successfully\n");
	i386_irq_install(0,timer_handler);
    i386_irq_install(1,tty_key_handler);
	i386_irq_install(7,rlt_clk_handler);
	tty_kbd_init();
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

