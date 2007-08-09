/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_MOUNT_H__
#define __MICRON_KERNEL_MOUNT_H__

#include <config.h>
#include <types.h>

// Inode structure
struct vnode {
	// identifiers
	char	name[NVNAMLEN];	// File name
	dev_t	dev;		// Device ID of device containing file
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

	/* internal data */
	struct vnode *sem;	// process visit semaphore
	struct vnode *next;	// Next node on same level
	struct vnode *parent;	// Parent inode index
	struct vnode *child;	// Child inode index
	struct vnode *expand;	// Node expansion (same file blk extention)

	/* cache related */
	struct vnode *cnext;	// next entry in the cache list
};

/* POSIX Open file modes */
#define	S_IRWXU 	0x00000007
#define		S_IRUSR 0x00000001	// Read permission, owner
#define		S_IWUSR 0x00000002	// Write permission, owner
#define		S_IXUSR 0x00000004	// Execute/search permission, owner
#define	S_IRWXG		0x00000070	// Read, write, execute/search by group 
#define		S_IRGRP	0x00000010	// Read permission, group
#define 	S_IWGRP	0x00000020	// Write permission, group
#define		S_IXGRP	0x00000040	// Execute/search permission, group
#define	S_IRWXO		0x00000700	// Read, write, execute/search by others
#define		S_IROTH	0x00000100	// Read permission, others
#define		S_IWOTH	0x00000200	// Write permission, others
#define		S_IXOTH	0x00000400	// Execute/search permission, others

/* POSIX other attributes */
#define	S_ISUID		0x00001000	// Set user ID on execution
#define S_ISGID		0x00002000	// Set group ID on execution
#define S_ISVTX		0x00004000	// On directories, restricted deletion

/* POSIX File types */
#define	S_IFMT		0x000F0000	// Type of file. 
#define		S_IFIFO	0x00010000	// FIFO special. 
#define		S_IFCHR	0x00020000	// Character special. 
#define		S_IFBLK	0x00030000	// Block special. 
#define		S_IFDIR	0x00040000	// Directory. 
#define		S_IFLNK	0x00050000	// Symbolic link. 
#define		S_IFREG	0x00060000	// Regular file.
#define		S_IFSOCK 0x00070000	// Socket file
#define		S_IFPROC 0x00080000	// Procfs
#define		S_IFDEV 0x00090000	// Device fs.

#endif

