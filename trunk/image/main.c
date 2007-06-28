#include <stdio.h>
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
	int (*l_mkdir)(char *name);
	int (*l_chdir)(char *name);
	int (*l_write)(char *name, char *buff, int size);
}fslib;

// Read a file into dynamic transfer buffer
int xbuff_read(char *name)
{
	int fildes=open(name, O_RDONLY);
	
	lseek(fildes, 0, SEEK_END);
	close(fildes);
	return 0;
}

// Pack a directory tree into target image
int packdir(char *dirname, char *olddirname)
{
	DIR *dir;
	struct dirent *dp;
	struct stat buf;
	char curdirname[256];

	// Process relative directory strings
	curdirname[0]='\0';
	if(strcmp(olddirname, "")) {
		strcpy(curdirname, olddirname);
	}
	strcat(curdirname, dirname);
	strcat(curdirname, "/");

	// Open target directory for reading items
	if((dir = opendir(dirname)) == NULL) {
		printf("ERROR: No such dir: %s\n", dirname);
		return -1;
	}

	// Process items one by one
	chdir(dirname);	while((dp=readdir(dir)) != NULL) {
		// Ignore "." and ".."
		if(!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
			continue;
		// Stat the file's type
		stat(dp->d_name, &buf);
		if(buf.st_mode & S_IFDIR) {
			fslib.l_mkdir(dp->d_name);
			fslib.l_chdir(dp->d_name);
			packdir(dp->d_name, curdirname);
			fslib.l_chdir("..");
		}else if(buf.st_mode & S_IFREG) {
			xbuff_read(dp->d_name);
			fslib.l_write(dp->d_name, xbuff.b_buff, xbuff.b_size);
			if(config.c_verbose) {
				printf("%s%s\n", curdirname, dp->d_name);
			}
		}
	}
	chdir("..");
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
		printf(" -r, --root   DIRECTORY  Specify the directory to create the image\n");
		printf(" -o, --output FILENAME   Specify the image's filename\n");
		printf("Supported Formats:\n");
		printf("  mfs\n");
		return 0;
	}

	// Setup output file format
	if(config.c_fmt==0) {
		printf("ERROR: File system format not specified\n");
		return -1;
	}else if(!strcmp(config.c_fmt, "mfs")) {
		fslib.l_mkdir = mfs_mkdir;
		fslib.l_chdir = mfs_chdir;
		fslib.l_write = mfs_write;
	}

	// Image creation process
	if(config.c_root==0) {
		printf("ERROR: Root directory not specified\n");
		return -1;
	}
	packdir(config.c_root, "");

	return 0;
}

