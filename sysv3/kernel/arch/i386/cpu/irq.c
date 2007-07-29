/*****************************************************************************
 * Micron System Version 3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT
 *****************************************************************************
 * Reference:
 *   Intel 8259A Datasheet
 *   Ralf Brown's Interrupt List
 *   Bran's Kernel Development Tutorial
 *****************************************************************************/
#include <io.h>
#include <libc.h>
#include <irq.h>

void irq_eoi()
{
	outportb(0x20, 0x20);
	outportb(0xA0, 0x20);
}

void (*isr_handler[32])(struct Register*);

void (*irq_handler[16])(struct Register*);

void (*syscall)(struct Register*);

void isr_install(int index, void (*handler)(struct Register*))
{
	isr_handler[index] = handler;
}

void irq_install(int index, void (*handler)(struct Register*))
{
	irq_handler[index] = handler;
}

void isr_install_syscall(void (*handler)(struct Register*))
{
	syscall = handler;
}

void isr_entry(struct Register *regs)
{
	if((regs->intn >= 0) && (regs->intn <= 31)) {
		PANIC(irq_handler[regs->intn] == 0, "ISR=%d", regs->intn);
		isr_handler[regs->intn](regs);
	} else if((regs->intn >= 32) && (regs->intn <= 47)) {
		MSG(irq_handler[regs->intn-32] == 0, "IRQ=%d", regs->intn-32);
		irq_handler[regs->intn-32](regs);
		irq_eoi();
	}else if(regs->intn == 0x80) {
		isr_handler[regs->intn](regs);
	}
}

void pic_init()
{
	/* Master Initialization */
	outportb(0x20, 0x11);	/* ICW1 */
	outportb(0x21, 0x20);	/* ICW2 */
	outportb(0x21, 0x04);	/* ICW3 */
	outportb(0x21, 0x01);	/* ICW4 */

	/* Slave Initialization */
	outportb(0xA0, 0x11);	/* ICW1 */
	outportb(0xA1, 0x28);	/* ICW2 */
	outportb(0xA1, 0x02);	/* ICW3 */
	outportb(0xA1, 0x01);	/* ICW4 */

	/* Enable PIC */
	outportb(0x20, 0x00);
	outportb(0xA0, 0x00);
}

void pic_enable()
{
	outportb(0x21, 0x00);
	outportb(0xA1, 0x00);
}

void pic_disable()
{
	outportb(0x21, 0xFF);
	outportb(0xA1, 0xFF);
}

void irq_init()
{
	pic_init();
}

