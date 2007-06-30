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
#include <time.h>
#include <fcntl.h>
#include "mfs.h"

// MFS image creation operation platform
struct mfs_data
{
	char  m_debug;		// show debug informartion flag
	int   m_outfile;	// output open file descriptor (unix)
	char *m_blocks[6];	// block serial buffer
}mfs_data;

int mfs_markbmp(char *bmp, int index, char stat)
{
	if(stat==0) {
		bmp[index/8] &=~(1<<(7-index%8));
	} else {
		bmp[index/8] |=(1<<(7-index%8));
	}
	return 0;
}

int mfs_seekbmp(char *bmp, int begin, int end)
{
	int bbegin=begin/8;
	int bend  =end/8;
	int temp, index=0;

	// search by byte
	for(; bbegin < bend; bbegin++) {
		if((unsigned char)bmp[bbegin]!=0xff) {
			goto search_byte;
		}
	}
	return -1;

	// search by bit
search_byte:
	temp=(unsigned char)~bmp[bbegin];
	while((unsigned char)(temp&0x80) != 0x80) {
		temp<<=1;
		index++;
	}
	return index+bbegin*8;
}

int mfs_new(char *outname, char debug)
{
	mfs_data.m_debug= debug;
	if(mfs_data.m_debug)
		printf("[MFS]: Entering MFS module\n");

	// Create the output file
	if(mfs_data.m_debug)
		printf("[MFS]: Creating image file: %s\n", outname);
	mfs_data.m_outfile= open(outname, O_TRUNC|O_BINARY|O_RDWR|O_CREAT);

	// Allocate the blocks
	mfs_data.m_blocks[0] = malloc(MFS_BLKSIZE);	// the boot block
	mfs_data.m_blocks[1] = malloc(MFS_BLKSIZE);	// the super block
	mfs_data.m_blocks[2] = malloc(MFS_BLKSIZE);	// the inode bitmap blk
	mfs_data.m_blocks[3] = malloc(MFS_BLKSIZE);	// the data bitmap blk
	mfs_data.m_blocks[4] = malloc(MFS_BLKSIZE*512);	// the inode blk
	mfs_data.m_blocks[5] = malloc(MFS_BLKSIZE*512);	// the data blk 8192
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

	// Setup the root inode
	struct mfs_inode *ip = (struct mfs_inode*)mfs_data.m_blocks[4];
	mfs_markbmp(mfs_data.m_blocks[2], 0, 1);
	strcpy(ip[0].i_name, "root");
	ip[0].i_mode = S_IFDIR|S_IRWXU|S_IRWXG|S_IROTH;

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

int mfs_lboot(char *bootname)
{
	int fboot = open(bootname, O_RDONLY|O_BINARY);
	int fsize = lseek(fboot, 0, SEEK_END);
	lseek(fboot, 0, SEEK_SET);
	read(fboot, mfs_data.m_blocks[0], fsize);
	close(fboot);
	return 0;
}

int mfs_iget(char *path)
{
	int i, j=0, ibi=0;
	struct mfs_inode *iblk;
	char *pathbuf, *pathent[128];	// 128 lvl of dir

	// parse the path entries into item
	pathbuf=malloc(strlen(path));
	strcpy(pathbuf, path);
	for(i=0; i<strlen(path); i++) {
		if(pathbuf[i]=='/') {
			pathbuf[i]=0;
			if(i+1 < strlen(path)) {
				pathent[j++]=&pathbuf[i+1];
			}
		}
	}

	// setup inode block access
	iblk=(struct mfs_inode*)mfs_data.m_blocks[4];

	// get entry inode sn
	for(i=0; i<j; i++) {
		if(mfs_data.m_debug)
			printf("[MFS]: -->%s\n", pathent[i]);
//		printf("%d->%s\n", i, pathent[i]);
//		printf("==%d\n", iblk[ibi].i_child);
		for(ibi=iblk[ibi].i_child; ibi!=0; ibi=iblk[ibi].i_level)
			if(!strcmp(iblk[ibi].i_name, pathent[i]))
				break;
		if(strcmp(iblk[ibi].i_name, pathent[i])) {
			ibi=-1;
		}
	}
	return ibi;
}

int mfs_mkdir(char *path, char *name)
{
	int indexi, pathi;
	struct mfs_inode *iblk;

	// error checks
	if(strlen(name)>MFS_NAMELEN) {
		printf("ERROR: File name too long: %s\n", name);
		exit(-1);
	}
	if(mfs_data.m_debug)
		printf("[MFS]: Creating directory: %s\n", name);

	// setup inode block pointer
	iblk=(struct mfs_inode*)mfs_data.m_blocks[4];

	// get the path inode
	if((pathi=mfs_iget(path))==-1) {
		printf("ERROR: bug inside mfs.c : mfs_mkdir()\n");
		exit(-1);
	}

	// seek for a empty inode
	indexi=mfs_seekbmp(mfs_data.m_blocks[2], 0, MFS_BLKSIZE*8);
	if(indexi==-1) {
		printf("ERROR: Out of inode\n");
		exit(-1);
	}
	mfs_markbmp(mfs_data.m_blocks[2], indexi, 1);	// mark used
	strcpy(iblk[indexi].i_name, name);
	iblk[indexi].i_mode  = S_IFDIR|S_IRWXU|S_IRWXG|S_IROTH;
	iblk[indexi].i_sn    = indexi;
	iblk[indexi].i_ctime = time(0);
	iblk[indexi].i_mtime = time(0);
	iblk[indexi].i_atime = time(0);
	iblk[indexi].i_uid   = 0;
	iblk[indexi].i_gid   = 0;
	iblk[indexi].i_level = iblk[pathi].i_child;
	iblk[indexi].i_parent= pathi;
	iblk[indexi].i_child = 0;
	iblk[indexi].i_file  = 0;
	iblk[indexi].i_blk      = 0;
	iblk[indexi].i_blk_count= 0;
	iblk[pathi].i_child = indexi;

	return 0;
}

int mfs_write(char *path, char *name, char *buff, int size)
{
	int indexi, pathi, dblki=0, dblkr;
	struct mfs_inode *iblk;

	// error checks
	if(strlen(name)>MFS_NAMELEN) {
		printf("ERROR: File name too long: %s\n", name);
		exit(-1);
	}
	if(mfs_data.m_debug)
		printf("[MFS]: Writting file: %s\n", name);

	// setup inode block pointer
	iblk=(struct mfs_inode*)mfs_data.m_blocks[4];

	// get the path inode
	if((pathi=mfs_iget(path))==-1) {
		printf("ERROR: bug inside mfs.c : mfs_mkdir() : 1\n");
		exit(-1);
	}else if((iblk[pathi].i_mode & S_IFDIR) != S_IFDIR) {
		printf("ERROR: bug inside mfs.c : mfs_mkdir() : 2\n");
		exit(-1);
	}

	// seek for a empty inode
	indexi = mfs_seekbmp(mfs_data.m_blocks[2], 0, MFS_BLKSIZE*8);
	if(indexi==-1) {
		printf("ERROR: Out of inode\n");
		exit(-1);
	}
	mfs_markbmp(mfs_data.m_blocks[2], indexi, 1);	// mark used
	strcpy(iblk[indexi].i_name, name);
	iblk[indexi].i_mode  = S_IFREG|S_IRWXU|S_IRWXG|S_IROTH;
	iblk[indexi].i_sn    = indexi;
	iblk[indexi].i_ctime = time(0);
	iblk[indexi].i_mtime = time(0);
	iblk[indexi].i_atime = time(0);
	iblk[indexi].i_uid   = 0;
	iblk[indexi].i_gid   = 0;
	iblk[indexi].i_level = iblk[pathi].i_child;
	iblk[indexi].i_parent= pathi;
	iblk[indexi].i_child = 0;
	iblk[indexi].i_file  = 0;
	iblk[pathi].i_child = indexi;

	// allocate data block and copy data, then setup block link data

	// count needed blocks for storage
	dblkr = size/MFS_BLKSIZE;
	if(size % MFS_BLKSIZE)
		dblkr++;
	iblk[indexi].i_blk_count= dblkr;
	if(mfs_data.m_debug)
		printf("[MFS]: Needed blocks: %d\n", dblkr);
	
	// allocate blocks
	while(dblkr--) {
		dblki = mfs_seekbmp(mfs_data.m_blocks[3], 0, MFS_BLKSIZE*8);
		if(dblki==-1) {
			printf("ERROR: Out of data block\n");
			exit(-1);
		}
		mfs_markbmp(mfs_data.m_blocks[3], dblki, 1);	// mark used
	}
	iblk[indexi].i_blk      = dblki - iblk[indexi].i_blk_count + 1;
	if(mfs_data.m_debug)
		printf("[MFS]: Beginning from block: %d\n", iblk[indexi].i_blk);

	// writing blocks
	memcpy(&mfs_data.m_blocks[5][iblk[indexi].i_blk * MFS_BLKSIZE], \
		buff, size);
	return 0;
}

