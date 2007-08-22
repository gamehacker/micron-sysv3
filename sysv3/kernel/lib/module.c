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

/* Module type indication string */
char *mod_str[4] = {
	"DRIVER ",
	"FILESYS",
	"PROTOCO",
	"SERVICE"
};

int modinit()
{
	/* module initialization start from MOD_ARCH */
	int type=0;
	int i;

next_mod:

	/* initialize i */
	i=0;

	/* Initialize each compiled modules */
	while((modtabp[i].init!=0) && (&modtabp[i]<&modtab_end)) {
		/* adjust initialization sequence by module type */
		if(modtabp[i].type != type) {
			i++;
			continue;
		}

		/* initialization */
		modhdl = modtabp[i].init;
		kprintf("%C[%s]%C ", 0x0E, mod_str[type], 0x0F);
		kprintf("%s Initializing...", modtabp[i].desc);
		if(modhdl() == 0) {
			kprintf("%X%C[SUCCESS]%C\n", 68, 0x0A, 0x0F);
		} else {
			kprintf("%X%C[FAILURE]%C\n", 68, 0x0C, 0x0F);
			modhdl = modtabp[i].exit;
			kprintf("%C[%s]%C ", 0x0E, mod_str[type], 0x0F);
			kprintf("%s Unitialized...", modtabp[i].desc);
			if(modhdl() == 0) {
				kprintf("%X%C[SUCCESS]%C\n", 68, 0x0A, 0x0F);
			} else {
				kprintf("%X%C[FAILURE]%C\n", 68, 0x0C, 0x0F);
			}
		}
		i++;
	}

	if(type != SRV) {
		type++;
		goto next_mod;
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

