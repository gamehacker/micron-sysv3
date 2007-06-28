
#define MFS_NAMELEN 16		// MFS supported file name length

// MFS Data Type Definitions (currently i386)
typedef unsigned int   mfs_u32;
typedef int            mfs_s32;
typedef unsigned short mfs_u16;
typedef short          mfs_s16;
typedef unsigned char  mfs_u8;
typedef char           mfs_s8;

// MFS Super block data structure
struct mfs_superblk {
	mfs_u8  s_magic[8];	// Magic identifier
};

// MFS Inode structure
struct mfs_inode {
	mfs_u8  i_name[MFS_NAMELEN];	// File name

	// Time
	mfs_u32 i_ctime;	// File creation time
	mfs_u32 i_mtime;	// Last modify time
	mfs_u32 i_atime;	// Last access time

	// Ownership
	mfs_u32 i_uid;		// Owner user ID
	mfs_u32 i_gid;		// Owner group ID
	mfs_u32 i_access;	// Access control bits

	// File allocation
};

// Exporting library functions
extern int mfs_mkdir(char *name);
extern int mfs_chdir(char *name);
extern int mfs_write(char *name, char *buff, int size);

