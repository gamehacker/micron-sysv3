/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_TYPES_H__
#define __MICRON_KERNEL_TYPES_H__

/*
 * This file defines various data types used in kernel interlinking, which may
 * be different from architecture to architecture, and can be modified by porter
 */

/* portable basic data types */
typedef unsigned int   u32_t;
typedef unsigned short u16_t;
typedef unsigned char  u8_t;
typedef int            s32_t;
typedef short          s16_t;
typedef char           s8_t;

/* IDs */
typedef unsigned int   id_t;		// General identifier 
typedef unsigned short dev_t;		// Used for device IDs

/* Sizes */
typedef unsigned int   size_t;		// Counter for sizes

/* File attribute */
typedef unsigned short mode_t;		// File attributes

#endif

