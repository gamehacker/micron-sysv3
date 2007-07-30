/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Martin Tang
 * Copyright (C) 2007, Micron System Team
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <dma.h>
#include <libc.h>
#include <io.h>

#define DMA1_CHAN 4				// 4 channels in total
#define DMA1_BSIZ 1024				// one block per time

/* DMA-1 registers */			 /* ch0   ch1   ch2   ch3 */
unsigned char dma1_badd[DMA1_CHAN] = { 0x00, 0x02, 0x04, 0x06 };
unsigned char dma1_page[DMA1_CHAN] = { 0x87, 0x83, 0x81, 0x82 };
unsigned char dma1_bcnt[DMA1_CHAN] = { 0x01, 0x03, 0x05, 0x07 };

/* DMA-1 usage mutex */
unsigned int dma1_mutex[DMA1_CHAN];

/* DMA-1 mask status buffer */
unsigned char dma1_mask;

/* DMA-1 transfer buffer */
unsigned char dma1_buff[DMA1_CHAN][DMA1_BSIZ] __attribute__((aligned (0x1000)));

/* shortcuts for easy access */
#define MUTX1 dma1_mutex
#define BADD1 dma1_badd
#define BPAG1 dma1_page
#define BCNT1 dma1_bcnt
#define BUFF1 dma1_buff
#define TMSK1 dma1_mask
#define STAT1 0x08			/* status register */
#define CMDR1 0x08			/* command register */
#define WREQ1 0x09			/* write request register */
#define MASK1 0x0A			/* mask register */
#define MODE1 0x0B			/* mode register */
#define CBFF1 0x0C			/* clear byte ptr flip-flop register */
#define TEMP1 0x0D			/* temporary register */
#define MCLR1 0x0D			/* master clear register */
#define CLRM1 0x0E			/* clear mask register */
#define WRTM1 0x0F			/* write mask register */

/* DMA driver master initializer */
void dma_init()
{
	PANIC((unsigned)BUFF1%0x1000, "Uncompatiable compiler detected\n");
	DEBUG((unsigned)BUFF1);
	DEBUG((unsigned)BUFF1 + DMA1_BSIZ*DMA1_CHAN);
	PANIC(((unsigned)BUFF1 + DMA1_BSIZ * DMA1_CHAN) > (0x1000000), 
			"DMA buffer out of operation range\n");
}

/* NOTICE: the following functions must be used in pair */
/* pause any existing transfers */
void dma1_pause()
{
	TMSK1 = inportb(WRTM1);
	outportb(WRTM1, 0x0F);
}

/* resume existing transfers */
void dma1_resume()
{
	outportb(WRTM1, TMSK1);
}

/* initialize DMA-1 Chip */
void dma1_init(unsigned mode)
{
	dma1_pause();
	outportb(MODE1, mode);
	dma1_resume();
}

/* read DMA-1 Chip status */
void dma1_stat (char *stat, unsigned int *buf)
{
	dma1_pause();
	*stat = inportb(STAT1);
	*buf = (unsigned int)BUFF1;
	dma1_resume();
}

/* !!! in the following, assumes the invoker disabled all the interrupts */

/* initialize DMA-1 Chip for a read transfer */
int dma1_read (unsigned channel, unsigned mode)
{
	if(channel > 3) {
		return -1;	/* channel invalid or not driven */
	}
	if(MUTX1[channel]) {
		return -1;	/* channel is already in use */
	}

	/* pause all transfers for setup */
	dma1_pause();
	
	/* mark mutex */
	MUTX1[channel]++;
	
	/* clear flip flops */
	outportb(CBFF1, 0xFF);			

	/* setup base address */
	outportb(BADD1[channel], (char) (unsigned)BUFF1&0xFF);
	outportb(BADD1[channel], (char)((unsigned)BUFF1>>8) &0xFF);
	outportb(BPAG1[channel], (char)((unsigned)BUFF1>>16)&0xFF);

	/* setup transfer size */
	outportb(BCNT1[channel], DMA1_BSIZ &0xFF);
	outportb(BCNT1[channel], (DMA1_BSIZ>>8)&0xFF);

	/* write to mode register */
	outportb(MODE1, (mode&0xF0)|channel|0x08);
	
	/* unmask the requested channel */
	TMSK1 &= ~(1<<channel);

	/* resume stopped transfers */
	dma1_resume();
	return 0;
}

/* initialize DMA-1 Chip for a read transfer */
int dma1_write(unsigned channel, unsigned mode)
{
	if(channel > 3) {
		return -1;	/* channel invalid or not driven */
	}
	if(MUTX1[channel]) {
		return -1;	/* channel is already in use */
	}

	/* pause all transfers for setup */
	dma1_pause();
	
	/* mark mutex */
	MUTX1[channel]++;
	
	/* clear flip flops */
	outportb(CBFF1, 0xFF);			

	/* setup base address */
	outportb(BADD1[channel], (char) (unsigned)BUFF1&0xFF);
	outportb(BADD1[channel], (char)((unsigned)BUFF1>>8) &0xFF);
	outportb(BPAG1[channel], (char)((unsigned)BUFF1>>16)&0xFF);

	/* setup transfer size */
	outportb(BCNT1[channel], DMA1_BSIZ &0xFF);
	outportb(BCNT1[channel], (DMA1_BSIZ>>8)&0xFF);

	/* write to mode register */
	outportb(MODE1, (mode&0xF0)|channel|0x04);
	
	/* unmask the requested channel */
	TMSK1 &= ~(1<<channel);

	/* resume stopped transfers */
	dma1_resume();

	return 0;
}

/* end a DMA transfer session */
int dma1_close(unsigned channel)
{
	if(channel >3) {
		return -1;
	}
	dma1_pause();
	TMSK1 |= 1<<channel;
	dma1_resume();
	return 0;
}
