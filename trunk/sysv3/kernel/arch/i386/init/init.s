/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

.global _start; _start:
.extern kmain
.extern stack
.extern i386_gdt_init
.extern i386_idt_init
	movl $stack, %esp	/* setup stack for kernel */
	call i386_gdt_init	/* install new gdt managed by kernel */
	call i386_idt_init	/* install new gdt managed by kernel */
	call kmain
	hlt			/* kmain function should never return */
