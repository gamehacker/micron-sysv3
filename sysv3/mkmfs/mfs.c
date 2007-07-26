/******************************************************************************
 * Micron System V3 System Image Build Utility
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 ******************************************************************************
 * Note:
 *   1 inode struct  = 128 bytes
 *   1 inode bmp blk = 8192 inodes = 1048576 bytes = 1024 blocks
 *   1 data bmp blk  = 8192 blocks
 *****************************************************************************/
 #define NKBLK	512					// kernel size in kb
 #define NIBMP	1					// inode no ctl
 #define NDBMP	1					// data blk no ctl
 #define NIBLK	(NIBMP*8*sizeof(struct mfs_inode))
 #define NDBLK	NIBLK //(NDBMP*8*MFS_BLKSIZE)
/*****************************************************************************/
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
	char *m_blocks[7];	// block serial buffer
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

int mfs_new(char debug)
{
	mfs_data.m_debug= debug;
	if(mfs_data.m_debug)
		printf("[MFS]: Entering MFS module\n");

	// Allocate the blocks
	mfs_data.m_blocks[0] = malloc(MFS_BLKSIZE);	 // the boot block
	mfs_data.m_blocks[1] = malloc(MFS_BLKSIZE);	 // the super block
	mfs_data.m_blocks[2] = malloc(MFS_BLKSIZE*NKBLK);// the kernel block
	mfs_data.m_blocks[3] = malloc(MFS_BLKSIZE*NIBMP);// the inode bitmap blk
	mfs_data.m_blocks[4] = malloc(MFS_BLKSIZE*NDBMP);// the data bitmap blk
	mfs_data.m_blocks[5] = malloc(MFS_BLKSIZE*NIBLK);// the inode blk
	mfs_data.m_blocks[6] = malloc(MFS_BLKSIZE*NDBLK);// the data blk
	memset(mfs_data.m_blocks[0], 0, MFS_BLKSIZE);
	memset(mfs_data.m_blocks[1], 0, MFS_BLKSIZE);
	memset(mfs_data.m_blocks[2], 0, MFS_BLKSIZE*NKBLK);
	memset(mfs_data.m_blocks[3], 0, MFS_BLKSIZE*NIBMP);
	memset(mfs_data.m_blocks[4], 0, MFS_BLKSIZE*NDBMP);
	memset(mfs_data.m_blocks[5], 0, MFS_BLKSIZE*NIBLK);
	memset(mfs_data.m_blocks[6], 0, MFS_BLKSIZE*NDBLK);

	// Setup super block info
	struct mfs_superblk *sblk = (struct mfs_superblk*)mfs_data.m_blocks[1];
	sblk->s_magic[0]='M';
	sblk->s_magic[1]='F';
	sblk->s_magic[2]='S';
	sblk->s_kblk    =2;
	sblk->s_kblkcnt =NKBLK;
	sblk->s_ibmp    =sblk->s_kblk + NKBLK;
	sblk->s_ibmpcnt =NIBMP;
	sblk->s_dbmp    =sblk->s_ibmp + NIBMP;
	sblk->s_dbmpcnt =NDBMP;
	sblk->s_iblk    =sblk->s_dbmp + NDBMP;
	sblk->s_iblkcnt =NIBLK;
	sblk->s_dblk    =sblk->s_iblk + NIBLK;
	sblk->s_dblkcnt =NDBLK;
	if(mfs_data.m_debug) {
		printf("[SBLK]: i=%-8dcnt=%-8d", 1, 1);
		printf("add=0x%-8x\n", 1*MFS_BLKSIZE);
		printf("[KBLK]: i=%-8dcnt=%-8d", 2, NKBLK);
		printf("add=0x%-8x\n", 2*MFS_BLKSIZE);
		printf("[IBMP]: i=%-8dcnt=%-8d", sblk->s_ibmp, sblk->s_ibmpcnt);
		printf("add=0x%-8x\n", sblk->s_ibmp*MFS_BLKSIZE);
		printf("[DBMP]: i=%-8dcnt=%-8d", sblk->s_dbmp, sblk->s_dbmpcnt);
		printf("add=0x%-8x\n", sblk->s_dbmp*MFS_BLKSIZE);
		printf("[IBLK]: i=%-8dcnt=%-8d", sblk->s_iblk, sblk->s_iblkcnt);
		printf("add=0x%-8x\n", sblk->s_iblk*MFS_BLKSIZE);
		printf("[DBLK]: i=%-8dcnt=%-8d", sblk->s_dblk, sblk->s_dblkcnt);
		printf("add=0x%-8x\n", sblk->s_dblk*MFS_BLKSIZE);
	}

	// Setup the root inode
	struct mfs_inode *ip = (struct mfs_inode*)mfs_data.m_blocks[5];
	mfs_markbmp(mfs_data.m_blocks[3], 0, 1);
	strcpy(ip[0].i_name, "root");
	ip[0].i_mode = S_IFDIR|S_IRWXU|S_IRWXG|S_IROTH;

	return 0;
}

int mfs_end(char *outname)
{
	int out;

	// Create the output file
	printf("Creating image file: %s\n", outname);
	out = open(outname, O_TRUNC|O_BINARY|O_RDWR|O_CREAT, S_IRWXU);
	
	// Write the blocks
	lseek(out, 0, SEEK_SET);
	write(out, mfs_data.m_blocks[0], MFS_BLKSIZE);
	write(out, mfs_data.m_blocks[1], MFS_BLKSIZE);
	write(out, mfs_data.m_blocks[2], MFS_BLKSIZE*NKBLK);
	write(out, mfs_data.m_blocks[3], MFS_BLKSIZE*NIBMP);
	write(out, mfs_data.m_blocks[4], MFS_BLKSIZE*NDBMP);
	write(out, mfs_data.m_blocks[5], MFS_BLKSIZE*NIBLK);
	write(out, mfs_data.m_blocks[6], MFS_BLKSIZE*NDBLK);

	int i;
	for(i=0; i<7; i++) {
		free(mfs_data.m_blocks[i]);
	}

	// Close out put file and save
	close(out);

	if(mfs_data.m_debug)
		printf("[MFS]: Quiting MFS module\n");
	return 0;
}

int mfs_lboot(char *bootname)
{
	printf("%s\n", bootname);
	int fboot = open(bootname, O_RDONLY|O_BINARY);
	int fsize = lseek(fboot, 0, SEEK_END);
	if(fsize>512) {
		printf("ERROR: Bootloader size too big\n");
	}
	lseek(fboot, 0, SEEK_SET);
	read(fboot, mfs_data.m_blocks[0], fsize);
	close(fboot);
	return 0;
}

int mfs_lkern(char *kernname)
{
	printf("%s\n", kernname);
	int fkern = open(kernname, O_RDONLY|O_BINARY);
	int fsize = lseek(fkern, 0, SEEK_END);
	if(fsize>NKBLK*MFS_BLKSIZE) {
		printf("ERROR: Bootloader size too big\n");
	}
	lseek(fkern, 0, SEEK_SET);
	read(fkern, mfs_data.m_blocks[2], fsize);
	close(fkern);
	return 0;
}

int mfs_iget(char *path)
{
	int i, j=0, ibi=0;
	struct mfs_inode *iblk;
	char *pathbuf, *pathent[128];	// 128 lvl of dir

	// parse the path entries into item
	pathbuf=malloc(strlen(path)+1);
	if(pathbuf == NULL) {
		printf("ERROR: Memory Unavailable \n");
	}
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
	iblk=(struct mfs_inode*)mfs_data.m_blocks[5];

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
	free(pathbuf);
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
	iblk=(struct mfs_inode*)mfs_data.m_blocks[5];

	// get the path inode
	if((pathi=mfs_iget(path))==-1) {
		printf("ERROR: bug inside mfs.c : mfs_mkdir()\n");
		exit(-1);
	}

	// seek for a empty inode
	indexi=mfs_seekbmp(mfs_data.m_blocks[3], 0, MFS_BLKSIZE*8);
	if(mfs_data.m_debug)
		printf("[MFS]: Allocated inode %d\n", indexi);
	if(indexi==-1) {
		printf("ERROR: Out of inode\n");
		exit(-1);
	}
	mfs_markbmp(mfs_data.m_blocks[3], indexi, 1);	// mark used
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
	iblk=(struct mfs_inode*)mfs_data.m_blocks[5];

	// get the path inode
	if((pathi=mfs_iget(path))==-1) {
		printf("ERROR: bug inside mfs.c : mfs_mkdir() : 1\n");
		exit(-1);
	}else if((iblk[pathi].i_mode & S_IFDIR) != S_IFDIR) {
		printf("ERROR: bug inside mfs.c : mfs_mkdir() : 2\n");
		exit(-1);
	}

	// seek for a empty inode
	indexi = mfs_seekbmp(mfs_data.m_blocks[3], 0, MFS_BLKSIZE*NIBMP*8);
	if(mfs_data.m_debug)
		printf("[MFS]: Allocated inode %d\n", indexi);
	if(indexi==-1) {
		printf("ERROR: Out of inode\n");
		exit(-1);
	}
	mfs_markbmp(mfs_data.m_blocks[3], indexi, 1);	// mark used
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
	// TODO: Add continuity check here, discuss: really needed??
	while(dblkr--) {
		dblki=mfs_seekbmp(mfs_data.m_blocks[4], 0, MFS_BLKSIZE*NDBLK*8);
		if(dblki==-1) {
			printf("ERROR: Out of data block\n");
			exit(-1);
		}
		mfs_markbmp(mfs_data.m_blocks[4], dblki, 1);	// mark used
	}
	iblk[indexi].i_blk      = dblki - iblk[indexi].i_blk_count + 1;
	if(mfs_data.m_debug)
		printf("[MFS]: Beginning from block: %d\n", iblk[indexi].i_blk);

	// writing blocks
	memcpy(&mfs_data.m_blocks[6][iblk[indexi].i_blk * MFS_BLKSIZE], \
		buff, size);
	return 0;
}

