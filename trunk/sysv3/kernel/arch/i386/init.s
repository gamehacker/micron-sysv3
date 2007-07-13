/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

.global _start; _start:
.extern kmain
.extern stack
	movl $stack, %esp	/* setup stack for kernel */
	call kmain
	hlt			/* kmain function should never return */
