/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_VCACHE_H__
#define __MICRON_KERNEL_VCACHE_H__

#include <config.h>
#include <types.h>
#include <vnode.h>

extern struct vnode vcache[NVCHCNTS];

extern struct vnode *vcache_alloc();

#endif

