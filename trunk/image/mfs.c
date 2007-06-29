///////////////////////////////////////////////////////////////////////////////
// Micron System V3 System Image Build Utility
// Copyright (C) 2007, Martin Tang
// martintang25 AT gmail.com
///////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include "mfs.h"

int mfs_mkdir(char *name)
{
	printf("**  mfs_mkdir %s\n", name);
	return 0;
}

int mfs_chdir(char *name)
{
	printf("**  mfs_chdir %s\n", name);
	return 0;
}

int mfs_write(char *name, char *buff, int size)
{
	printf("**  mfs_write %s\n", name);
	return 0;
}

