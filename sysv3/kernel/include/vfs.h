/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_VFS_H__
#define __MICRON_KERNEL_VFS_H__

/* Open file modes */
#ifndef S_IRWXU
#define	S_IRWXU 	0x0007
#define		S_IRUSR 0x0001	// Read permission, owner
#define		S_IWUSR 0x0002	// Write permission, owner
#define		S_IXUSR 0x0004	// Execute/search permission, owner
#define	S_IRWXG		0x0070	// Read, write, execute/search by group 
#define		S_IRGRP	0x0010	// Read permission, group
#define 	S_IWGRP	0x0020	// Write permission, group
#define		S_IXGRP	0x0040	// Execute/search permission, group
#define	S_IRWXO		0x0700	// Read, write, execute/search by others
#define		S_IROTH	0x0100	// Read permission, others
#define		S_IWOTH	0x0200	// Write permission, others
#define		S_IXOTH	0x0400	// Execute/search permission, others
#define	S_IFMT		0xF000	// Type of file. 
#define		S_IFIFO	0x1000	// FIFO special. 
#define		S_IFCHR	0x2000	// Character special. 
#define		S_IFBLK	0x3000	// Block special. 
#define		S_IFDIR	0x4000	// Directory. 
#define		S_IFLNK	0x5000	// Symbolic link. 
#define		S_IFREG	0x6000	// Regular file.
#define		S_IFSCK 0x7000	// Socket file
#define		S_IFPRC 0x8000	// Procfs
#define		S_IFDEV 0x9000 // Device fs.

#endif

