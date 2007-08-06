/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

.global _start; _start:
.extern kboot
.extern kmain
.extern stack
.extern modinit
.extern gdt_init
.extern idt_init
.extern irq_init
.extern mem_size
.extern rtc_init
.extern page_init
.extern dma_init
.extern tty_init
.extern dev_init
	movb %al,    kboot
	movl $stack, %esp	/* setup stack for kernel */
	call dev_init		/* initialize device manager first */
	call tty_init		/* initialize TTY */
	call idt_init		/* install new gdt managed by kernel */
	call irq_init		/* initialize IRQ */
	call mem_size		/* get memory size info */
	call rtc_init		/* initialize RTC */
	call gdt_init		/* install new gdt managed by kernel */
	call page_init		/* initialize RTC */
	sti			/* enable all interrupts */
	call modinit		/* initialize all modules */
	call kmain
halt:	hlt			/* kmain function should never return */
	jmp halt
