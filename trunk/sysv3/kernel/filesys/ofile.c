/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <ofile.h>

struct ofile ofile[NMXOFILE];

struct ofile *ofile_alloc()
{
	int i;
	for(i=0; i<NMXOFILE; i++) {
		if(ofile[i].node ==0) {
			return &ofile[i];
		}
	}
	return 0;	/* no empty one found */
}

