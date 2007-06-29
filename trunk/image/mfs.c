///////////////////////////////////////////////////////////////////////////////
// Micron System V3 System Image Build Utility
// Copyright (C) 2007, Martin Tang
// martintang25 AT gmail.com
///////////////////////////////////////////////////////////////////////////////
// Note:
//   1 inode bmp blk = 8192 inodes = 512 blocks
//   1 data bmp blk  = 8192 blocks
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "mfs.h"

// MFS image creation operation platform
struct mfs_data
{
	char  m_debug;		// show debug informartion flag
	int   m_outfile;	// output open file descriptor (unix)
	char *m_blocks[6];	// block serial buffer
}mfs_data;

int mfs_new(char *outname, char debug)
{
	mfs_data.m_debug= debug;
	if(mfs_data.m_debug)
		printf("[MFS]: Entering MFS module\n");

	// Create the output file
	if(mfs_data.m_debug)
		printf("[MFS]: Creating image file: %s\n", outname);
	mfs_data.m_outfile= open(outname, O_TRUNC|O_RDWR|O_CREAT);

	// Allocate the blocks
	mfs_data.m_blocks[0] = malloc(MFS_BLKSIZE);	// the boot block
	mfs_data.m_blocks[1] = malloc(MFS_BLKSIZE);	// the super block
	mfs_data.m_blocks[2] = malloc(MFS_BLKSIZE);	// the inode bitmap blk
	mfs_data.m_blocks[3] = malloc(MFS_BLKSIZE);	// the data bitmap blk
	mfs_data.m_blocks[4] = malloc(MFS_BLKSIZE*512);	// the inode blk
	mfs_data.m_blocks[5] = malloc(MFS_BLKSIZE*512);	// the inode blk 8192
	memset(mfs_data.m_blocks[0], 0, MFS_BLKSIZE);
	memset(mfs_data.m_blocks[1], 0, MFS_BLKSIZE);
	memset(mfs_data.m_blocks[2], 0, MFS_BLKSIZE);
	memset(mfs_data.m_blocks[3], 0, MFS_BLKSIZE);
	memset(mfs_data.m_blocks[4], 0, MFS_BLKSIZE*512);
	memset(mfs_data.m_blocks[5], 0, MFS_BLKSIZE*512);//8192

	// Setup super block info
	struct mfs_superblk *sblk = (struct mfs_superblk*)mfs_data.m_blocks[1];
	sblk->s_magic[0]='M';
	sblk->s_magic[1]='F';
	sblk->s_magic[2]='S';
	sblk->s_ibmp    =2;	sblk->s_ibmpcnt =1;
	sblk->s_dbmp    =3;	sblk->s_dbmpcnt =1;
	sblk->s_iblk    =4;	sblk->s_iblkcnt =512;
	sblk->s_dblk    =516;	sblk->s_dblkcnt =512;

	return 0;
}

int mfs_seekbmp(char *bmp, int begin, int end)
{
	return 0;
}

int mfs_end()
{
	// Write the blocks
	lseek(mfs_data.m_outfile, 0, SEEK_SET);
	write(mfs_data.m_outfile, mfs_data.m_blocks[0], MFS_BLKSIZE);
	write(mfs_data.m_outfile, mfs_data.m_blocks[1], MFS_BLKSIZE);
	write(mfs_data.m_outfile, mfs_data.m_blocks[2], MFS_BLKSIZE);
	write(mfs_data.m_outfile, mfs_data.m_blocks[3], MFS_BLKSIZE);
	write(mfs_data.m_outfile, mfs_data.m_blocks[4], MFS_BLKSIZE*512);
	write(mfs_data.m_outfile, mfs_data.m_blocks[5], MFS_BLKSIZE*512);//8192
	close(mfs_data.m_outfile);

	if(mfs_data.m_debug)
		printf("[MFS]: Quiting MFS module\n");
	return 0;
}

int mfs_mkdir(char *name)
{
	if(mfs_data.m_debug)
		printf("[MFS]: Creating directory: %s\n", name);
	return 0;
}

int mfs_write(char *name, char *buff, int size)
{
	if(mfs_data.m_debug)
		printf("[MFS]: Writting file: %s\n", name);
	return 0;
}

