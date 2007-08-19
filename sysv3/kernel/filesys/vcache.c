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
		/* just allocate each node in order */
		vp = &vcache[vcache_data.cnt];
		if(vcache_data.cnt == 0) {
			vcache_data.begin = vp;
			vp->clast = 0;
		} else {
			vp->clast = vcache_data.end;
		}
		vp->cnext = 0;
		if(vcache_data.end) {
			vcache_data.end->cnext = vp;
		}
		vcache_data.end = vp;
		vcache_data.cnt++;
		return vp;
	} else {
		/* do reallocates in the policy of least used reallocation */
		for(vp=vcache_data.begin; vp!=0; vp=vp->cnext) if(!vp->sem) {
			/* put sp to end of list */
			/*  1. unlink */
			if(vp->clast != 0) {
				vp->clast->cnext = vp->cnext;
			}
			if(vp->cnext != 0) {
				vp->cnext->clast = vp->clast;
			}
			/*  2. relinking to end */
			vp->clast = vcache_data.end;
			vp->cnext = 0;
			vcache_data.end->cnext = vp;
			vcache_data.end = vp;

			return vp;
		}
	}
	return 0;
}

void vcache_promo(struct vnode *vp)
{
	/* 1. unlinking from alist */
	if(vp->cnext != 0) {
		vp->cnext->clast = vp->clast;
	} else {
		/* vp is already the least priority */
		return;
	}
	if(vp->clast != 0) {
		vp->clast->cnext = vp->cnext;
	} else {
		vcache_data.begin = vp->cnext;
	}
	/* 2. put it after it's original next node (promotion) */
	if(vp->cnext != 0) {
		if(vp->cnext->cnext != 0) {
			vp->cnext->cnext->clast = vp;
		}
		vp->clast = vp->cnext;
		vp->cnext = vp->cnext->cnext;
		vp->clast->cnext = vp;
		if(vp->cnext == 0) {
			vcache_data.end = vp;
		}
	}
}

