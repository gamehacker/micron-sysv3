/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <module.h>
#include <device.h>
#include <libc.h>

/* init and exit symbol table location */
extern struct modtab modtab;
extern struct modtab modtab_end;

/* init and exit symbol table */
struct modtab *modtabp = &modtab;

/* The function pointer for invokations */
int (*modhdl)();

int modinit()
{
	int i=0;

	/* Initialize each compiled modules */
	while((modtabp[i].init!=0) && (&modtabp[i]<&modtab_end)) {
		modhdl = modtabp[i].init;
		kprintf("%C[MODULE]%C ", 0x0E, 0x0F);
		kprintf("%s Initializing...", modtabp[i].desc);
		if(modhdl() == 0) {
			kprintf("%X%C[SUCCESS]%C\n", 68, 0x0A, 0x0F);
		} else {
			kprintf("%X%C[FAILURE]%C\n", 68, 0x0C, 0x0F);
			modhdl = modtabp[i].exit;
			kprintf("%C[MODULE]%C ", 0x0E, 0x0F);
			kprintf("%s Unitialized...", modtabp[i].desc);
			if(modhdl() == 0) {
				kprintf("%X%C[SUCCESS]%C\n", 68, 0x0A, 0x0F);
			} else {
				kprintf("%X%C[FAILURE]%C\n", 68, 0x0C, 0x0F);
			}
		}
		i++;
	}
	return 0;
}

int modstart()
{
	return 0;
}

int modkill()
{
	return 0;
}

