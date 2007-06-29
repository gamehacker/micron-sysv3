///////////////////////////////////////////////////////////////////////////////
// Micron System V3 System Image Build Utility
// Copyright (C) 2007, Martin Tang
// martintang25 AT gmail.com
///////////////////////////////////////////////////////////////////////////////
// Configurations
#define MFS_NAMELEN 16		// MFS supported file name length
#define MFS_BLKSIZE 1024	// MFS block size

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
	mfs_u8  s_magic[3];	// Magic identifier

	// block location
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
	mfs_u32 i_mode;			// Inode mode

	// Time
	mfs_u32 i_ctime;	// File creation time
	mfs_u32 i_mtime;	// Last modify time
	mfs_u32 i_atime;	// Last access time

	// Ownership
	mfs_u16 i_uid;		// Owner user ID
	mfs_u16 i_gid;		// Owner group ID
	mfs_u32 i_access;	// Access control bits

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
extern int mfs_new(char *outname, char debug);
extern int mfs_end();
extern int mfs_mkdir(char *name);
extern int mfs_write(char *name, char *buff, int size);

