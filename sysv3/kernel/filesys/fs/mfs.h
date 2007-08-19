/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MFS_H__
#define __MFS_H__

// MFS Super block data structure
struct mfs_sblk {
	char      s_magic[4];	// Magic identifier
	u16_t     s_kblk;	// Inode bitmap block
	u16_t     s_kblkcnt;	// Inode bitmap block count
	blkcnt_t  s_ibmp;	// Inode bitmap block
	blkcnt_t  s_ibmpcnt;	// Inode bitmap block count
	blkcnt_t  s_dbmp;	// Data bitmap block
	blkcnt_t  s_dbmpcnt;	// Data bitmap block count
	blkcnt_t  s_iblk;	// Inode block
	blkcnt_t  s_iblkcnt;	// Inode block count
	blkcnt_t  s_dblk;	// Data block
	blkcnt_t  s_dblkcnt;	// Data block count
	blksize_t s_blksize;	// Block size
};

// MFS Inode structure
struct mfs_inode {
	char	name[NVNAMLEN];	// File name
	ino_t	ino;		// Inode serial number
	mode_t	mode;		// Inode mode
	nlink_t	nlink;		// Number of linked files
	uid_t	uid;		// Owner user ID
	gid_t	gid;		// Owner group ID
	dev_t	rdev;		// Device ID (if char or blk special)
	size_t	size;		// File size
	time_t	atime;		// Last access time
	time_t	mtime;		// Last modify time
	time_t	ctime;		// File creation time
	blkcnt_t blkentry;	// Block index of first storage block
	blkcnt_t blocks;	// Block index storage blocks continuity
	ino_t	next;		// Next node on same level
	ino_t	parent;		// Parent inode index
	ino_t	child;		// Child inode index
	ino_t	expand;		// Node expansion (same file blk extention)
}__attribute__((packed));

#endif

