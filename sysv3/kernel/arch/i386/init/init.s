/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

.global _start; _start:
.extern kmain
.extern stack
.extern modinit
.extern i386_gdt_init
.extern i386_idt_init
.extern i386_irq_init
.extern i386_rtc_init
.extern i386_page_init
	movl $stack, %esp	/* setup stack for kernel */
	call i386_idt_init	/* install new gdt managed by kernel */
	call i386_irq_init	/* initialize IRQ */
	call i386_rtc_init	/* initialize RTC */
	call i386_gdt_init	/* install new gdt managed by kernel */
	call i386_page_init	/* initialize RTC */
	call modinit		/* initialize all modules */
	call kmain
halt:	hlt			/* kmain function should never return */
	jmp halt
