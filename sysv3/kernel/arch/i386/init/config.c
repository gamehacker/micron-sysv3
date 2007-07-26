/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

unsigned arch_memsize;		/* architecture memory size */

void arch_disablei()
{
	__asm__ __volatile__("cli");
}

void arch_enablei()
{
	__asm__ __volatile__("sti");
}

