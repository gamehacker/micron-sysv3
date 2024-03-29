/******************************************************************************
 * Micron System V3 System Image Build Utility
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __IMAGE_MFS_H__
#define __IMAGE_MFS_H__

// Configurations
#define MFS_NAMELEN 68		// MFS supported file name length
#define MFS_BLKSIZE 1024	// MFS block size

// Definitions
#ifndef S_IRWXU

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

// MFS Data Type Definitions (currently i386)
typedef unsigned int   mfs_u32;
typedef int            mfs_s32;
typedef unsigned short mfs_u16;
typedef short          mfs_s16;
typedef unsigned char  mfs_u8;
typedef char           mfs_s8;

// MFS Super block data structure
struct mfs_superblk {
	// identifiers
	mfs_u8  s_magic[4];	// Magic identifier

	// block location
	mfs_u16 s_kblk;		// Inode bitmap block
	mfs_u16 s_kblkcnt;	// Inode bitmap block count
	mfs_u32 s_ibmp;		// Inode bitmap block
	mfs_u32 s_ibmpcnt;	// Inode bitmap block count
	mfs_u32 s_dbmp;		// Data bitmap block
	mfs_u32 s_dbmpcnt;	// Data bitmap block count
	mfs_u32 s_iblk;		// Inode block
	mfs_u32 s_iblkcnt;	// Inode block count
	mfs_u32 s_dblk;		// Data block
	mfs_u32 s_dblkcnt;	// Data block count
	mfs_u32 s_blksize;	// Block size
};

// MFS Inode structure
struct mfs_inode {
	// Identifiers
	mfs_s8  i_name[MFS_NAMELEN];	// File name
	mfs_u32 i_ino;		// Inode serial number
	mfs_u32 i_mode;		// Inode mode
	mfs_u32 i_nlink;	// Number of linked files
	mfs_u16 i_uid;		// Owner user ID
	mfs_u16 i_gid;		// Owner group ID
	mfs_s32 i_rdev;		// Device ID (if char or blk special)
	mfs_u32 i_size;		// File size
	mfs_u32 i_atime;	// Last access time
	mfs_u32 i_mtime;	// Last modify time
	mfs_u32 i_ctime;	// File creation time
	mfs_u32 i_blkentry;	// Block index of first storage block
	mfs_u32 i_blocks;	// Block index storage blocks continuity
	mfs_u32 i_next;		// Next node on same level
	mfs_u32 i_parent;	// Parent inode index
	mfs_u32 i_child;	// Child inode index
	mfs_u32 i_expand;	// Node expansion (same file blk extention)
}__attribute__((packed));

// Exporting library functions
extern int mfs_new(char debug);
extern int mfs_end(char *outname);
extern int mfs_lboot(char *bootname);
extern int mfs_lkern(char *kernname);
extern int mfs_mkdir(char *path, char *name);
extern int mfs_write(char *path, char *name, char *buff, int size);

#endif
