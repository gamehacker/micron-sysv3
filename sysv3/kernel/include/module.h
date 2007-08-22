/*****************************************************************************
 * Micron System V3 - System Core
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#ifndef __MICRON_KERNEL_MODULE_H__
#define __MICRON_KERNEL_MODULE_H__

#include <config.h>

/* Here are the module automatic init and exit macros */
#define REGISTER_MODULE(type, id, desc, init, exit) \
	char mod_type_##id __attribute__((section(".modtab"))) \
	= type; \
	char mod_id_##id[NMIDSLEN] __attribute__((section(".modtab"))) \
	= #id; \
	char mod_desc_##id[NMDESLEN] __attribute__((section(".modtab"))) \
	= desc; \
	int mod_init_##id __attribute__((section(".modtab"))) \
	= (int) init; \
	int mod_exit_##id __attribute__((section(".modtab"))) \
	= (int) exit;

#define DRV	0		/* device driver */
#define FS	1		/* file system support */
#define PROT	2		/* protocol support */
#define SRV	3		/* system service */

/* Accessing datatype */
struct modtab
{
	char type;		/* module type */
	char id[NMIDSLEN];	/* module id string */
	char desc[NMDESLEN];	/* module description */
	int (*init)();		/* module initialize function */
	int (*exit)();		/* module uninitialize function */
};

/* Module init and exit invokers */
extern int modinit();
extern int modexit();

#endif
