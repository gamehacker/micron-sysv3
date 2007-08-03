/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_VFS_H__
#define __MICRON_KERNEL_VFS_H__

#include <config.h>
#include <icache.h>

/* POSIX Open file modes */
#define	S_IRWXU 	0x00000007
#define		S_IRUSR 0x00000001	// Read permission, owner
#define		S_IWUSR 0x00000002	// Write permission, owner
#define		S_IXUSR 0x00000004	// Execute/search permission, owner
#define	S_IRWXG		0x00000070	// Read, write, execute/search by group 
#define		S_IRGRP	0x00000010	// Read permission, group
#define 	S_IWGRP	0x00000020	// Write permission, group
#define		S_IXGRP	0x00000040	// Execute/search permission, group
#define	S_IRWXO		0x00000700	// Read, write, execute/search by others
#define		S_IROTH	0x00000100	// Read permission, others
#define		S_IWOTH	0x00000200	// Write permission, others
#define		S_IXOTH	0x00000400	// Execute/search permission, others

/* POSIX other attributes */
#define	S_ISUID		0x00001000	// Set user ID on execution
#define S_ISGID		0x00002000	// Set group ID on execution
#define S_ISVTX		0x00004000	// On directories, restricted deletion

/* POSIX File types */
#define	S_IFMT		0x000F0000	// Type of file. 
#define		S_IFIFO	0x00010000	// FIFO special. 
#define		S_IFCHR	0x00020000	// Character special. 
#define		S_IFBLK	0x00030000	// Block special. 
#define		S_IFDIR	0x00040000	// Directory. 
#define		S_IFLNK	0x00050000	// Symbolic link. 
#define		S_IFREG	0x00060000	// Regular file.
#define		S_IFSOCK 0x00070000	// Socket file
#define		S_IFPROC 0x00080000	// Procfs
#define		S_IFDEV 0x00090000	// Device fs.

/* POSIX macros */
#define	S_ISBLK(m)	((m & S_IFMT)==S_IFBLK)
#define	S_ISCHR(m)	((m & S_IFMT)==S_IFCHR)
#define	S_ISDIR(m)	((m & S_IFMT)==S_IFDIR)
#define	S_ISFIFO(m)	((m & S_IFMT)==S_IFIFO)
#define	S_ISREG(m)	((m & S_IFMT)==S_IFREG)
#define	S_ISLNK(m)	((m & S_IFMT)==S_IFLNK)
#define	S_ISSOCK(m)	((m & S_IFMT)==S_IFSOCK)
#define S_ISPROC(m)	((m & S_IFMT)==S_IFPROC)
#define S_ISDEV(m)	((m & S_IFMT)==S_IFDEV)

/* POSIX File access modes */
#define	O_ACCMODE	 0x000F
#define 	O_RDONLY 0x0001		/* Read only access */
#define 	O_WRONLY 0x0002		/* Write only access */
#define 	O_RDWR	 0x0003		/* Read/Write access */

/* File system operations */
extern ino_t iget(ino_t cdir, char *path, int parent);

#endif

