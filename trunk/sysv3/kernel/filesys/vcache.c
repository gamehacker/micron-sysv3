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
#include <vcache.h>
#include <libc.h>

enum vnode_mode
{
	CON,			/* continuous allocation */
	LUR			/* lur algorithm */
};

struct vnode_stat
{
	enum vnode_mode mode;
	ino_t	acnt;		/* allocated node count */
	ino_t	cbegin;		/* cache's allocated list beginning node */
	ino_t	cend;		/* cache's allocated list end node */
}vnode_stat;

struct vnode vcache[NVNODES];

int vcache_init()
{
	memset(&vnode_stat, 0, sizeof(vnode_stat));
	memset(vcache, 0, sizeof(vcache));
	return 0;
}

ino_t vcache_alloc()
{
	ino_t out = 0;
	ino_t i;
	switch(vnode_stat.mode)
	{
	case CON:
		if(vnode_stat.acnt < NVNODES) {
			out = vnode_stat.acnt;
			if(vnode_stat.acnt == 0) {
				vnode_stat.cbegin = vnode_stat.acnt;
				vnode_stat.cend   = vnode_stat.acnt;
				vcache[vnode_stat.cend].v_vnext = 0;
			} else {
				vcache[vnode_stat.cend].v_vnext=vnode_stat.acnt;
				vnode_stat.cend = vnode_stat.acnt;
				vcache[vnode_stat.cend].v_vnext = 0;
			}
			vnode_stat.acnt++;

			vcache[out].v_id = out;
			return out;
			break;
		} else {
			vnode_stat.mode = LUR;
			MSG(1, "enter LUR mode\n");
		}
		/* NOTE: here goes on, no break */
	case LUR:
		/* find the least used allocated node in list, which match
		 * the condition of being reallocated (v_vsem == 0) */
		for(i = -1; i != vnode_stat.cend; i = vcache[i].v_vnext) {
			if(vcache[vcache[i].v_vnext].v_vsem == 0) {
				/* store target inode ID */
				out = vcache[i].v_vnext;

				/* mark target inode's data as unavailable */
				vcache[out].v_vstat &= ~V_AVAIL;

				/* cut down the link between target node */
				vcache[i].v_vnext = 
					vcache[vcache[i].v_vnext].v_vnext;

				/* 
				 * add target inode to end of list (latest 
				 * allocated) 
				 */
				vcache[vnode_stat.cend].v_vnext = out;
				vnode_stat.cend = out;
				vcache[vnode_stat.cend].v_vnext = 0;

				/* return node ID */
				vcache[out].v_id = out;
				return out;
			}
		}
		break;
	}
	PANIC(1, "out of allocatable vnode");
	return -1;
}

