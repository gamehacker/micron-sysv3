/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <config.h>
#include <module.h>
#include <device.h>
#include <types.h>
#include <libc.h>
#include <fs.h>

#define MFS_NAMELEN 64

// MFS Super block data structure
struct mfs_sblk {
	// identifiers
	u8_t  s_magic[4];	// Magic identifier

	// block location
	u16_t s_kblk;		// Inode bitmap block
	u16_t s_kblkcnt;	// Inode bitmap block count
	u32_t s_ibmp;		// Inode bitmap block
	u32_t s_ibmpcnt;	// Inode bitmap block count
	u32_t s_dbmp;		// Data bitmap block
	u32_t s_dbmpcnt;	// Data bitmap block count
	u32_t s_iblk;		// Inode block
	u32_t s_iblkcnt;	// Inode block count
	u32_t s_dblk;		// Data block
	u32_t s_dblkcnt;	// Data block count
	u32_t s_blksize;	// Block size
};

// MFS Inode structure
struct mfs_inode {
	// Identifiers
	s8_t  i_name[MFS_NAMELEN];	// File name
	s32_t i_dev;		// Device ID of device containing file
	u32_t i_ino;		// Inode serial number
	u32_t i_mode;		// Inode mode
	u32_t i_nlink;		// Number of linked files
	u16_t i_uid;		// Owner user ID
	u16_t i_gid;		// Owner group ID
	s32_t i_rdev;		// Device ID (if char or blk special)
	u32_t i_size;		// File size
	u32_t i_atime;		// Last access time
	u32_t i_mtime;		// Last modify time
	u32_t i_ctime;		// File creation time
	u32_t i_blkentry;	// Block index of first storage block
	u32_t i_blocks;		// Block index storage blocks continuity
	u32_t i_next;		// Next node on same level
	u32_t i_parent;		// Parent inode index
	u32_t i_child;		// Child inode index
	u32_t i_expand;		// Node expansion (same file blk extention)
}__attribute__((packed));

struct vfs *mfs_dev;

struct mfs_data
{
	dev_t dev;
	char  buf[4096];
	struct mfs_sblk  sblk;
	struct mfs_inode inode;
}mfs_data;

void mfs_readblk(dev_t dev, int off)
{
	dev_lseek(BLKDEV, dev, off, SEEK_SET);
	dev_read (BLKDEV, dev, mfs_data.buf, 2);
}

void mfs_writeblk(dev_t dev, int off)
{
	dev_lseek(BLKDEV, dev, off, SEEK_SET);
	dev_write(BLKDEV, dev, mfs_data.buf, 2);
}

void mfs_readsblk(dev_t dev)
{
	mfs_data.dev = dev;
	mfs_readblk(dev, 1);
	PANIC(strncmp(mfs_data.buf, "MFS", 3), "FS not supported");
	memcpy(&mfs_data.sblk, mfs_data.buf, sizeof(struct mfs_sblk));
}

int mfs_readibmp(dev_t dev, int page)
{
	if(mfs_data.dev != dev) {
		mfs_readsblk(dev);
	}
	if(page > mfs_data.sblk.s_ibmpcnt) {
		return -1;
	}
	mfs_readblk(dev, (mfs_data.sblk.s_ibmp+page)*2+1);
	return 0;
}

int mfs_writeibmp(dev_t dev, int page)
{
	if(mfs_data.dev != dev) {
		mfs_readsblk(dev);
	}
	if(page > mfs_data.sblk.s_ibmpcnt) {
		return -1;
	}
	mfs_writeblk(dev, (mfs_data.sblk.s_ibmp+page)*2+1);
	return 0;
}

int mfs_readdbmp(dev_t dev, int page)
{
	if(mfs_data.dev != dev) {
		mfs_readsblk(dev);
	}
	if(page > mfs_data.sblk.s_dbmpcnt) {
		return -1;
	}
	mfs_readblk(dev, (mfs_data.sblk.s_dbmp+page)*2+1);
	return 0;
}

int mfs_writedbmp(dev_t dev, int page)
{
	if(mfs_data.dev != dev) {
		mfs_readsblk(dev);
	}
	if(page > mfs_data.sblk.s_dbmpcnt) {
		return -1;
	}
	mfs_writeblk(dev, (mfs_data.sblk.s_dbmp+page)*2+1);
	return 0;
}

int mfs_readnode(dev_t dev, ino_t id)
{
	int page, entry;
	struct mfs_inode *ip;

	/* check superblock validation */
	if(mfs_data.dev != dev) {
		mfs_readsblk(dev);
	}

	/* calculate position */
	page = id / (mfs_data.sblk.s_blksize / sizeof(struct mfs_sblk));
	entry= id % (mfs_data.sblk.s_blksize / sizeof(struct mfs_sblk));

	/* load block from disk */
	mfs_readblk(dev, (mfs_data.sblk.s_iblk*2)+1+page);

	ip = (struct mfs_inode*) mfs_data.buf;

	/* do the data copy */
	memcpy(&mfs_data.inode, &ip[entry], sizeof(struct mfs_inode));

	return 0;
}

int mfs_writenode(dev_t dev, ino_t id)
{
	int page, entry;
	struct mfs_inode *ip;

	/* check superblock validation */
	if(mfs_data.dev != dev) {
		mfs_readsblk(dev);
	}

	/* calculate position */
	page = id / (mfs_data.sblk.s_blksize / sizeof(struct mfs_sblk));
	entry= id % (mfs_data.sblk.s_blksize / sizeof(struct mfs_sblk));

	ip = (struct mfs_inode*) mfs_data.buf;

	/* do the data copy */
	memcpy(&ip[entry], &mfs_data.inode, sizeof(struct mfs_inode));

	/* load block from disk */
	mfs_writeblk(dev, (mfs_data.sblk.s_iblk*2)+1+page);

	return 0;
}

int mfs_lschild(ino_t vid, ino_t id)
{
	return 0;
}

int mfs_mknode(ino_t id, char *name, mode_t mode, dev_t dev)
{
	return 0;
}

int mfs_rmnode(ino_t id, char *name)
{
	return 0;
}

int mfs_lseek(ino_t id, off_t offset, int whence)
{
	return 0;
}

int mfs_read(ino_t id, char *buf, size_t nbyte)
{
	return 0;
}

int mfs_write(ino_t id, char *buf, size_t nbyte)
{
	return 0;
}

int mfs_init()
{
	mfs_dev = &vfs[MFS];
	mfs_dev->lschild = mfs_lschild;
	mfs_dev->mknode  = mfs_mknode;
	mfs_dev->rmnode  = mfs_rmnode;
	mfs_dev->lseek   = mfs_lseek;
	mfs_dev->read    = mfs_read;
	mfs_dev->write   = mfs_write;

	mfs_readnode(DEVID(1, 0), 1);

	return 0;
}

int mfs_exit()
{
	mfs_dev->lschild = 0;
	mfs_dev->mknode  = 0;
	mfs_dev->rmnode  = 0;
	mfs_dev->lseek   = 0;
	mfs_dev->read    = 0;
	mfs_dev->write   = 0;
	return 0;
}

REGISTER_MODULE(mfs, "Micron File System", mfs_init, mfs_exit);

