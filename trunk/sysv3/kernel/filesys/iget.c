/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <fs.h>
#include <libc.h>

ino_t iget(ino_t cvdir, char *path, int parent)
{
	ino_t ip;
	char *pathc=path, *pathn;

	/* are we starting from the root ? */
	if(pathc[0] == '/') {
		ip = 0;
	} else {
		ip = cvdir;
	}

	/* seek for its parent */
	while((pathn = strchr(pathc, '/')) != 0) {
		pathc = pathn + 1;
	}

	/* if target's parent is enough (for mknode etc) */
	if(parent) {
		return ip;
	}
	return 0;
}

