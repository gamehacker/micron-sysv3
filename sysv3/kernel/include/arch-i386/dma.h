/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Martin Tang
 * Copyright (C) 2007, Micron System Team
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#ifndef __MICRON_KERNEL_DMA_H__
#define __MICRON_KERNEL_DMA_H__

/* Values used with dma1_init */
#define D1_M2MX_ENABLE  0x01		/* Memory to memory enable */
					/*   else disable */
#define D1_C0AH_ENABLE  0x02		/* Channel 0 address hold enable */
					/*   else disable */
#define D1_DMAC_DISABLE 0x04		/* Controller disable */
					/*   else enable */
#define D1_CMPT_ENABLE  0x08		/* Compressed timing enable */
					/*   else disable */
#define D1_PRIO_ROTATE  0x10		/* Rotating priority */
					/*   else fixed prio */
#define D1_EXTW_SELECT  0x20		/* Extended write selection */
					/*   else late write selection */
#define D1_DREQ_LOW     0x40		/* DREQ sense active low */
					/*   else active high */
#define D1_DACK_HIGH    0x80		/* DACK sense active high */
					/*   else active low */

/* Values used with dma1_read_write */
#define D1_AUTOINIT 0x10		/* Autoinitialization enable */
					/*   else disable */
#define D1_INCR     0x20		/* Address increment select */
					/*   else decrement */
#define D1_DEMAND   0x00		/* Demand mode */
#define D1_SINGLE   0x40		/* Single mode */
#define D1_BLOCK    0x80		/* Block mode */
#define D1_CASCADE  0xC0		/* Cascade mode */

extern void dma1_init (unsigned mode);
extern void dma1_stat (char *stat, unsigned int *buf);
extern int  dma1_read (unsigned channel, unsigned mode);
extern int  dma1_write(unsigned channel, unsigned mode);
extern int  dma1_close(unsigned channel);

#endif

