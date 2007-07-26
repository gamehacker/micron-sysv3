/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Martin Tang
 * Copyright (C) 2007, Micron System Team
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_DMA_H__
#define __MICRON_KERNEL_DMA_H__

enum i386_dma_mode
{
	SINGLE,
	BLOCK,
	DEMAND,
	CASCADE
};

extern void i386_dma_read(unsigned channel);
extern void i386_dma_write(unsigned channel);
extern void i386_dma_pause();
extern void i386_dma_resume();

#endif

