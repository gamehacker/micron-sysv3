///////////////////////////////////////////////////////////////////////////////
// Micron System V3 System Image Build Utility
// Copyright (C) 2007, Martin Tang
// martintang25 AT gmail.com
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mfs.h"

// Parameter scan result storage
struct config
{
	char  c_showhelp;	// just show help and exit
	char  c_debug;		// show debug imfo ?
	char  c_verbose;	// verbosely list files processed
	char *c_boot;		// selected boot section file name
	char *c_fmt;		// selected file system format
	char *c_root;		// root dir to copy to new image
	char *c_image;		// output filename
}config;

// The dynamic transfer buffer structure
struct xbuff
{
	int b_size;
	char *b_buff;
}xbuff;

// The output file system format library connector
// NOTE: Only a subset of FS functions needed
struct fslib
{
	int (*l_new  )(char *outname, char debug);
	int (*l_end  )();
	int (*l_lboot)(char *bootname);
	int (*l_mkdir)(char *path, char *name);
	int (*l_write)(char *path, char *name, char *buff, int size);
}fslib;

// Read a file into dynamic transfer buffer
int xbuff_read(char *name)
{
	int fildes=open(name, O_RDONLY|O_BINARY);
	int filcnt=lseek(fildes, 0, SEEK_END);
	lseek(fildes, 0, SEEK_SET);
	free(xbuff.b_buff);
	xbuff.b_size=filcnt;
	xbuff.b_buff=malloc(filcnt);
	read(fildes, xbuff.b_buff, filcnt);
	close(fildes);
	return 0;
}

// Pack a directory tree into target image
int cutsize;
int packdir(char *dirname)
{
	DIR *dir;		// directory descriptor
	struct dirent *dp;	// dirent pointer
	struct stat buf;	// file status buffer
	char path[512];		// current directory path
	char pathtemp[512];	// temp path buffer

	// Process relative directory strings
	if(cutsize==0) {
		cutsize=strlen(dirname);
	}
	strcpy(path, dirname);
	strcat(path, "/");

	// Read directory conteits
	if((dir=opendir(dirname)) == NULL) {
		printf("ERROR: Target directory not exist: %s\n", dirname);
		exit(-1);
	}

	// Parse through directory entries
	while((dp = readdir(dir)) != NULL) {
		strcpy(pathtemp, path);
		strcat(pathtemp, dp->d_name);
		stat(  pathtemp, &buf);
	//	if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
		if(dp->d_name[0]=='.') {
			continue;
		}else if(S_ISDIR(buf.st_mode)) {
			if(config.c_verbose)
				printf("%s\n", &pathtemp[cutsize]);
			fslib.l_mkdir(&path[cutsize], dp->d_name);
			packdir(pathtemp);
		}else if(S_ISREG(buf.st_mode)) {
			if(config.c_verbose)
				printf("%s\n", &pathtemp[cutsize]);
			xbuff_read(pathtemp);
			fslib.l_write(&path[cutsize], dp->d_name, xbuff.b_buff, xbuff.b_size);
		}
	}
	closedir(dir);

	return 0;
}

int main(int argc, char *argv[])
{
	// Scaning arguments
	config.c_showhelp=1;
	int argci=1;
	while(argci<argc) {
		if(!strcmp(argv[argci], "-d") ||
		   !strcmp(argv[argci], "--debug")) {
			config.c_debug=1;
			config.c_showhelp=0;
		}else if(!strcmp(argv[argci], "-v") ||
		   !strcmp(argv[argci], "--verbose")) {
			config.c_verbose=1;
		}else if(!strcmp(argv[argci], "-b") ||
		   !strcmp(argv[argci], "--boot")) {
			config.c_boot=argv[argci+1];
			config.c_showhelp=0;
			argci++;
		}else if(!strcmp(argv[argci], "-f") ||
		   !strcmp(argv[argci], "--fmt")) {
			config.c_fmt=argv[argci+1];
			config.c_showhelp=0;
			argci++;
		}else if(!strcmp(argv[argci], "-r") ||
		   !strcmp(argv[argci], "--root")) {
			config.c_root=argv[argci+1];
			config.c_showhelp=0;
			argci++;
		}else if(!strcmp(argv[argci], "-o") ||
		   !strcmp(argv[argci], "--output")) {
			config.c_image=argv[argci+1];
			config.c_showhelp=0;
			argci++;
		}
		argci++;
	}

	// Argument section debug information
	if(config.c_debug) {
		printf("* debug *************************************\n");
		printf(" showhelp= %d\n", config.c_showhelp);
		printf(" verbose = %d\n", config.c_verbose);
		printf(" bootload= %s\n", config.c_boot);
		printf(" format  = %s\n", config.c_fmt);
		printf(" rootdir = %s\n", config.c_root);
		printf(" output  = %s\n", config.c_image);
		printf("*********************************************\n");
	}

	// Showing the help and quit
	if(config.c_showhelp) {
		printf("Micron System V3 Root File System Builder, V1.0\n");
		printf("  Copyright (C) 2007, Huaxin Tang\n");
		printf("Useage:\n");
		printf("  image [--help] [--boot BOOTSECT] [--fmt TYPE] [--root DIRECTORY] \n");
		printf("        [--image FILENAME]\n");
		printf("Options:\n");
		printf(" -h, --help              Show this help and exit\n");
		printf(" -v, --verbose           Verbosely display the file names being processed\n");
		printf(" -b, --boot   BOOTSECT   Specify the bootloader file write to boot sector\n");
		printf(" -f, --fmt    TYPE       Target file system format, see Supported Formats\n");
		printf(" -r, --root   DIRECTORY  Specify the root directory\n");
		printf(" -o, --output FILENAME   Specify output filename\n");
		printf("Supported Formats:\n");
		printf("  mfs, e2fs, vfat, ntfs\n");
		return 0;
	}

	// Setup output file format
	if(config.c_fmt==0) {
		printf("ERROR: File system format not specified\n");
		return -1;
	}else if(!strcmp(config.c_fmt, "mfs")) {
		fslib.l_new   = mfs_new;
		fslib.l_end   = mfs_end;
		fslib.l_mkdir = mfs_mkdir;
		fslib.l_write = mfs_write;
		fslib.l_lboot = mfs_lboot;
	} // Add new fs format support here

	// Image creation process
	printf("Creating image...\n");
	if(config.c_root==0) {
		printf("ERROR: Root directory not specified\n");
		return -1;
	}
	if(fslib.l_mkdir==0 || fslib.l_write==0) {
		printf("ERROR: File system format specified not supported\n");
		return -1;
	}
	if(config.c_image==0) {
		printf("ERROR: Output image file name not specified\n");
		return -1;
	}
	fslib.l_new(config.c_image, config.c_debug);
	if(config.c_boot) {
		printf("Installing bootloader...\n");
		fslib.l_lboot(config.c_boot);
	}
	printf("Packing directories...\n");
	packdir(config.c_root);
	fslib.l_end();

	printf("Image creation completed\n");
	return 0;
}

