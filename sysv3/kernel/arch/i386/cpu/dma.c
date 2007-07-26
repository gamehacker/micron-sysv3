/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Martin Tang
 * Copyright (C) 2007, Micron System Team
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <dma.h>

#define DMA1_CHAN 4				// 4 channels in total
#define DMA1_BSIZ 1024				// one block per time

/* DMA-1 registers */			 /* ch0   ch1   ch2   ch3 */
unsigned char i386_dma1_badd[DMA1_CHAN] = { 0x00, 0x02, 0x04, 0x06 };
unsigned char i386_dma1_bcnt[DMA1_CHAN] = { 0x01, 0x03, 0x05, 0x07 };

/* DMA-1 usage mutex */
unsigned int i386_dma1_mutex[DMA1_CHAN];

/* DMA-1 transfer buffer */
unsigned int i386_dma1_buff[DMA1_CHAN][DMA1_BSIZ];

/* shortcuts for easy access */
#define MUTX1 i386_dma1_mutex
#define BADD1 i386_dma1_badd
#define BCNT1 i386_dma1_bcnt
#define BUFF1 i386_dma1_buff
#define STAT1 0x08	/* status register */
#define CMDR1 0x08	/* command register */
#define WREQ1 0x09	/* write request register */
#define MASK1 0x0A	/* mask register */
#define MODE1 0x0B	/* mode register */
#define CBFF1 0x0C	/* clear byte ptr flip-flop register */
#define TEMP1 0x0D	/* temporary register */
#define MCLR1 0x0D	/* master clear register */
#define CLRM1 0x0E	/* clear mask register */
#define WRTM1 0x0F	/* write mask register */

/* here we assumes that the invoker has already disabled all the interrupts */

/* initialize channel for transfer */
void i386_dma1_init(unsigned channel)
{
}

/* disable any further transfer */
void i386_dma1_deinit(unsigned channel)
{
}

/* pause any existing transfers */
void i386_dma1_pause()
{
}

/* resume existing transfers */
void i386_dma1_resume()
{
}

