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
#define REGISTER_MODULE(id, desc, init, exit) \
	char mod_id_##id[NMIDSLEN] __attribute__((section(".modtab"))) \
	= #id; \
	char mod_desc_##id[NMDESLEN] __attribute__((section(".modtab"))) \
	= desc; \
	int mod_init_##id __attribute__((section(".modtab"))) \
	= (int) init; \
	int mod_exit_##id __attribute__((section(".modtab"))) \
	= (int) exit;

/* Accessing datatype */
struct modtab
{
	char id[NMIDSLEN];
	char desc[NMDESLEN];
	int (*init)();
	int (*exit)();
};

/* Module init and exit invokers */
extern int modinit();
extern int modexit();

#endif
