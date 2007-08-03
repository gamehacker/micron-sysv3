/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Design Notes:
 *   The vnode cache uses 2 modes in operation, initially it uses continuous
 * allocation for new nodes, when all the nodes in the cache has been
 * allocated, it switchs to lur (latest used reallocation) algorithm, which
 * full fills the needs of high speed and resource recycling.
 *****************************************************************************/
#include <icache.h>
#include <libc.h>

enum inode_mode
{
	CON,			/* continuous allocation */
	LUR			/* lur algorithm */
};

struct inode_stat
{
	enum inode_mode mode;
	ino_t	acnt;		/* allocated node count */
	ino_t	cbegin;		/* cache's allocated list beginning node */
	ino_t	cend;		/* cache's allocated list end node */
}inode_stat;

struct inode icache[NINODES];

int icache_init()
{
	memset(&inode_stat, 0, sizeof(inode_stat));
	memset(icache, 0, sizeof(icache));
	return 0;
}

ino_t icache_alloc()
{
	ino_t out = 0;
	ino_t i;
	switch(inode_stat.mode)
	{
	case CON:
		if(inode_stat.acnt < NINODES) {
			out = inode_stat.acnt;
			if(inode_stat.acnt == 0) {
				inode_stat.cbegin = inode_stat.acnt;
				inode_stat.cend   = inode_stat.acnt;
				icache[inode_stat.cend].i_vnext = 0;
			} else {
				icache[inode_stat.cend].i_vnext=inode_stat.acnt;
				inode_stat.cend = inode_stat.acnt;
				icache[inode_stat.cend].i_vnext = 0;
			}
			inode_stat.acnt++;

			icache[out].i_id = out;
			return out;
			break;
		} else {
			inode_stat.mode = LUR;
			MSG(1, "enter LUR mode\n");
		}
		/* NOTE: here goes on, no break */
	case LUR:
		/* find the least used allocated node in list, which match
		 * the condition of being reallocated (i_vsem == 0) */
		for(i = -1; i != inode_stat.cend; i = icache[i].i_vnext) {
			if(icache[icache[i].i_vnext].i_vsem == 0) {
				/* store target inode ID */
				out = icache[i].i_vnext;

				/* mark target inode's data as unavailable */
				icache[out].i_vstat &= ~V_AVAIL;

				/* cut down the link between target node */
				icache[i].i_vnext = 
					icache[icache[i].i_vnext].i_vnext;

				/* 
				 * add target inode to end of list (latest 
				 * allocated) 
				 */
				icache[inode_stat.cend].i_vnext = out;
				inode_stat.cend = out;
				icache[inode_stat.cend].i_vnext = 0;

				/* return node ID */
				icache[out].i_id = out;
				return out;
			}
		}
		break;
	}
	PANIC(1, "out of allocatable vnode");
	return -1;
}

