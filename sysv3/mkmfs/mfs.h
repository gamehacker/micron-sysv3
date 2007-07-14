/******************************************************************************
 * Micron System V3 System Image Build Utility
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __IMAGE_MFS_H__
#define __IMAGE_MFS_H__

// Configurations
#define MFS_NAMELEN 80		// MFS supported file name length
#define MFS_BLKSIZE 1024	// MFS block size

// Definitions
#ifndef S_IRWXU
#define	S_IRWXU 	0x0007
#define		S_IRUSR 0x0001	// Read permission, owner
#define		S_IWUSR 0x0002	// Write permission, owner
#define		S_IXUSR 0x0004	// Execute/search permission, owner
#define	S_IRWXG		0x0070	// Read, write, execute/search by group 
#define		S_IRGRP	0x0010	// Read permission, group
#define 	S_IWGRP	0x0020	// Write permission, group
#define		S_IXGRP	0x0040	// Execute/search permission, group
#define	S_IRWXO		0x0700	// Read, write, execute/search by others
#define		S_IROTH	0x0100	// Read permission, others
#define		S_IWOTH	0x0200	// Write permission, others
#define		S_IXOTH	0x0400	// Execute/search permission, others
#define	S_IFMT		0xF000	//Type of file. 
#define		S_IFBLK	0x1000	//Block special. 
#define		S_IFCHR	0x2000	//Character special. 
#define		S_IFIFO	0x3000	//FIFO special. 
#define		S_IFREG	0x4000	//Regular. 
#define		S_IFDIR	0x5000	//Directory. 
#define		S_IFLNK	0x6000	//Symbolic link. 
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
};

// MFS Inode structure
struct mfs_inode {
	// Identifiers
	mfs_u8  i_name[MFS_NAMELEN];	// File name
	mfs_u16 i_mode;			// Inode mode
	mfs_u16 i_nlinks;		// Number of linked files
	mfs_u32 i_sn;			// Inode serial number

	// Time
	mfs_u32 i_ctime;	// File creation time
	mfs_u32 i_mtime;	// Last modify time
	mfs_u32 i_atime;	// Last access time

	// Ownership
	mfs_u16 i_uid;		// Owner user ID
	mfs_u16 i_gid;		// Owner group ID

	// Structure linking (all storage are block indexes)
	mfs_u32 i_level;	// Level inode index
	mfs_u32 i_parent;	// Parent inode index
	mfs_u32 i_child;	// Child inode index
	mfs_u32 i_file;		// File storage link inode block index

	// File allocation
	mfs_u32 i_blk;		// Block index of first storage block
	mfs_u32 i_blk_count;	// Block index storage blocks continuity
};

// Exporting library functions
extern int mfs_new(char debug);
extern int mfs_end(char *outname);
extern int mfs_lboot(char *bootname);
extern int mfs_lkern(char *kernname);
extern int mfs_mkdir(char *path, char *name);
extern int mfs_write(char *path, char *name, char *buff, int size);

#endif
