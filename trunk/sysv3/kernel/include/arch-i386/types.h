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
typedef unsigned int	u32_t;
typedef unsigned short	u16_t;
typedef unsigned char	u8_t;
typedef int		s32_t;
typedef short		s16_t;
typedef char		s8_t;

/* POSIX types */
typedef s32_t	id_t;		// General identifier 
typedef s16_t	uid_t;		// User ID
typedef s16_t	gid_t;		// Group ID
typedef s16_t	dev_t;		// Used for device IDs
typedef u32_t	ino_t;		// Inode IDs
typedef u32_t	nlink_t;	// File hard link count
typedef u32_t	time_t;		// Time type
typedef u32_t	size_t;		// Counter for sizes
typedef s32_t	blkcnt_t;	// Block counter
typedef s32_t	blksize_t;	// Block size descriptor
typedef s32_t	off_t;		// file sizes
typedef u32_t	mode_t;		// File attributes

#endif

