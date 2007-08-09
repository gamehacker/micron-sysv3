/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <vcache.h>

struct vnode vcache[NVCHCNTS];

struct vcache_data
{
	unsigned cnt;
	struct vnode *begin;
	struct vnode *end;
}vcache_data;

struct vnode *vcache_alloc()
{
	struct vnode *vp;
	if(vcache_data.cnt < NVCHCNTS) {
		vp = &vcache[vcache_data.cnt];
		vp->cnext = 0;
		vcache_data.end->cnext = vp;
		vcache_data.end = vp;
		vcache_data.cnt++;
		return vp;
	} else {
		for(vp=vcache_data.begin; vp!=0; vp=vp->cnext) if(!vp->sem) {
			vp->cnext = 0;
			vcache_data.end->cnext = vp;
			vcache_data.end = vp;
			return vp;
		}
	}
	return 0;
}

