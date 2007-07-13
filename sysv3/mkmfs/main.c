/******************************************************************************
 * Micron System V3 System Image Build Utility
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "mfs.h"
#include "license.h"

// Parameter scan result storage
struct config
{
	char  c_showhelp;	// just show help and exit
	char  c_showlic;	// show license agreement and exit
	char  c_debug;		// show debug imfo ?
	char  c_verbose;	// verbosely list files processed
	char *c_boot;		// selected boot section file name
	char *c_kern;		// selected kernel section file name
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
	// File system library initialization function
	// create the target file, and setup the debug sign
	int (*l_new  )(char debug);

	// File system library finalization function
	// Write the blocks and save the file
	int (*l_end  )(char *outname);

	// Install the target bootloader file specified by
	// bootname variable
	int (*l_lboot)(char *bootname);

	// Install the target kernel file specified by
	// kernname variable
	int (*l_lkern)(char *kernname);

	// Create a directory into target image, with real path = path and
	// directory name = name
	int (*l_mkdir)(char *path, char *name);

	// Write a regular file into target image, with real path = path
	// and file name = name, data comes from buff, and with buffer size
	// equals size
	int (*l_write)(char *path, char *name, char *buff, int size);
}fslib;

// Read a file into dynamic transfer buffer
int xbuff_read(char *name)
{
	int fildes=open(name, O_RDONLY|O_BINARY);
	
	// Get the target file's size so that we can allocate a suitable
	// buffer to store the data
	int filcnt=lseek(fildes, 0, SEEK_END);

	// Return to the beginning of the file
	lseek(fildes, 0, SEEK_SET);

	// Release origional buffer (abandon)
	free(xbuff.b_buff);

	// Now do the malloc
	xbuff.b_size=filcnt;
	xbuff.b_buff=malloc(filcnt);

	// Copy data into buffer
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

	// Process relative directory path strings
	if(cutsize==0) {
		cutsize=strlen(dirname);
	}
	strcpy(path, dirname);
	strcat(path, "/");

	// Open the directory contents for read
	if((dir=opendir(dirname)) == NULL) {
		printf("ERROR: Target directory not exist: %s\n", dirname);
		exit(-1);
	}

	// Parse through directory entries
	while((dp = readdir(dir)) != NULL) {

		// Make up the complete path for current file
		strcpy(pathtemp, path);
		strcat(pathtemp, dp->d_name);
		stat(  pathtemp, &buf);
		if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, "..")) {
			continue;

		// If the current file is a directory, then we go recursively
		// into this directory
		}else if(S_ISDIR(buf.st_mode)) {
			if(config.c_verbose)
				printf("%s\n", &pathtemp[cutsize]);
			fslib.l_mkdir(&path[cutsize], dp->d_name);
			packdir(pathtemp);

		// If the file is a ordinary file, just store it in data blocks
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
			// Don't show help so that it won't exit
			config.c_showhelp=0;
		}else if(!strcmp(argv[argci], "-v") ||
		   !strcmp(argv[argci], "--verbose")) {
			config.c_verbose=1;
		}else if(!strcmp(argv[argci], "-b") ||
		   !strcmp(argv[argci], "--boot")) {
			config.c_boot=argv[argci+1];
			config.c_showhelp=0;
			argci++;
		}else if(!strcmp(argv[argci], "-k") ||
		   !strcmp(argv[argci], "--kernel")) {
			config.c_kern=argv[argci+1];
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
		}else if(!strcmp(argv[argci], "-l") ||
		   !strcmp(argv[argci], "--license")) {
			config.c_showlic=1;
		}
		argci++;
	}

	if(config.c_showlic) {
		printf("%s\n", license);
		return 0;
	}

	// Argument section debug information
	if(config.c_debug) {
		printf("* debug *************************************\n");
		printf(" showhelp= %d\n", config.c_showhelp);
		printf(" verbose = %d\n", config.c_verbose);
		printf(" bootload= %s\n", config.c_boot);
		printf(" kernel  = %s\n", config.c_kern);
		printf(" format  = %s\n", config.c_fmt);
		printf(" rootdir = %s\n", config.c_root);
		printf(" output  = %s\n", config.c_image);
		printf("*********************************************\n");
	}

	// Showing the help and quit
	if(config.c_showhelp) {
		printf("Micron System V3 Root File System Builder, V1.0\n");
		printf("  Copyright (C) 2007, Martin Tang\n");
		printf("  PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.\n");
		printf("Useage:\n");
		printf("  mkmfs [--help] [--boot BOOTSECT] [--fmt TYPE] [--root DIRECTORY] \n");
		printf("        [--image FILENAME]\n");
		printf("Options:\n");
		printf(" -h, --help              Show this help and exit\n");
		printf(" -v, --verbose           Verbosely display the file names being processed\n");
		printf(" -b, --boot   BOOTSECT   Specify the bootloader file write to boot sector\n");
		printf(" -k, --kernel KERNEL     Specify the kernel file to copy to special section\n");
		printf(" -f, --fmt    TYPE       Target file system format, see Supported Formats\n");
		printf(" -r, --root   DIRECTORY  Specify the root directory\n");
		printf(" -o, --output FILENAME   Specify output filename\n");
		printf(" -l, --license           Print license agreement and quit\n");
		printf("Supported Formats:\n");
		printf("  mfs, #ext2fs, #vfat, #ntfs\n");
		return 0;
	}

	// Setup output file format
	if(config.c_fmt==0) {
		printf("ERROR: File system format not specified\n");
		return -1;
	}else if(!strcmp(config.c_fmt, "mfs")) {
		// install the target file system image creation library's
		// operation handlers:
		fslib.l_new   = mfs_new;
		fslib.l_end   = mfs_end;
		fslib.l_mkdir = mfs_mkdir;
		fslib.l_write = mfs_write;
		fslib.l_lboot = mfs_lboot;
		fslib.l_lkern = mfs_lkern;
	} // to add a new file system's support, just type in another else if
	  // here and install its corresponding operation handlers.

	// Image creation process
	if(config.c_root==0) {
		printf("ERROR: Root directory not specified\n");
		return -1;
	}
	
	// Check if the fs operation handlers are set ok, or program crashes :(
	if(fslib.l_mkdir==0 || fslib.l_write==0) {
		printf("ERROR: File system format specified not supported\n");
		return -1;
	}

	// check if the target output's name is setup
	if(config.c_image==0) {
		printf("ERROR: Output image file name not specified\n");
		return -1;
	}

	// Call fs lib's initialization operation, create the target
	// file and the debug sign (show debug messages)
	fslib.l_new(config.c_debug);
	printf("Creating image...\n");

	// Call fs lib's bootloader installation operation
	if(config.c_boot) {
		printf("Installing boot... ");
		fslib.l_lboot(config.c_boot);
	}

	// Call fs lib's kernel installation operation
	if(config.c_kern) {
		printf("Installing kernel... ");
		fslib.l_lkern(config.c_kern);
	}

	// Packup the directory contents
	printf("Packing directories...\n");
	packdir(config.c_root);
	
	// Close the output file and end procession
	fslib.l_end(config.c_image);

	printf("Image creation completed\n");
	return 0;
}

