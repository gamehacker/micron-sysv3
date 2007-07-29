/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************/
#include <libc.h>

struct page
{
	unsigned p:1;		/* present */
	unsigned rw:1;		/* read/write */
	unsigned us:1;		/* user/supervisor */
	unsigned pwt:1;		/* write through */
	unsigned pcd:1;		/* cache disabled */
	unsigned a:1;		/* accessed */
	unsigned d:1;		/* ENTRY: dirty */
	unsigned ps:1;		/* DIRECTORY: page size, ENTRY = 0 */
	unsigned g:1;		/* global page, ignored by DIRECTORY */
	unsigned avail:3;	/* available for any usage */
	unsigned addr:20;
}__attribute__((packed));

//struct page page[1024] __attribute__((aligned (0x1000)));

void page_init()
{
//	PANIC((unsigned)page%0x1000, "I386:cpu/mmu.c");
}

