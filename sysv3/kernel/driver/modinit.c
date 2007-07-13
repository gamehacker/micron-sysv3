/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <init.h>

/* init and exit symbol table location */
extern int inittab;
extern int exittab;

/* init and exit symbol table */
int *inittabp = &inittab;
int *exittabp = &exittab;

/* The function pointer for invokations */
int (*mod_hdl)();

int modinit()
{
	int i;
	for(i=0; inittabp[i]!=0; i++) {
		mod_hdl = (int(*)()) inittabp[i];
		mod_hdl();
	}
	return 0;
}

int modexit()
{
	int i;
	for(i=0; exittabp[i]!=0; i++) {
		mod_hdl = (int(*)()) exittabp[i];
		mod_hdl();
	}
	return 0;
}

