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

void i386_pic_eoi()
{
	outport(0x20, 0x20);
	outport(0xA0, 0x20);
}

void (*i386_isr_handler[32])(struct isr_regs*);

void (*i386_irq_handler[16])(struct isr_regs*);

void (*i386_isr_syscall)(struct isr_regs*);

void i386_isr_install(int index, void (*handler)(struct isr_regs*))
{
	i386_isr_handler[index] = handler;
}

void i386_irq_install(int index, void (*handler)(struct isr_regs*))
{
	i386_irq_handler[index] = handler;
}

void i386_isr_install_syscall(void (*handler)(struct isr_regs*))
{
	i386_isr_syscall = handler;
}

void i386_isr_entry(struct isr_regs *regs)
{
	if((regs->intn >= 0) && (regs->intn <= 31)) {
		PANIC(i386_irq_handler[regs->intn] == 0, "ISR=%d", regs->intn);
		i386_isr_handler[regs->intn](regs);
	} else if((regs->intn >= 32) && (regs->intn <= 47)) {
		PANIC(i386_irq_handler[regs->intn-32] == 0, "IRQ=%d", 
								regs->intn-32);
		i386_irq_handler[regs->intn-32](regs);
		i386_pic_eoi();
	}else if(regs->intn == 0x80) {
		i386_isr_handler[regs->intn](regs);
	}
}

void i386_pic_init()
{
	/* Master Initialization */
	outport(0x20, 0x11);	/* ICW1 */
	outport(0x21, 0x20);	/* ICW2 */
	outport(0x21, 0x04);	/* ICW3 */
	outport(0x21, 0x01);	/* ICW4 */

	/* Slave Initialization */
	outport(0xA0, 0x11);	/* ICW1 */
	outport(0xA1, 0x28);	/* ICW2 */
	outport(0xA1, 0x02);	/* ICW3 */
	outport(0xA1, 0x01);	/* ICW4 */

	/* Enable PIC */
	outport(0x20, 0x00);
	outport(0xA0, 0x00);
}

void i386_pic_enable()
{
	outport(0x21, 0x00);
	outport(0xA1, 0x00);
}

void i386_pic_disable()
{
	outport(0x21, 0xFF);
	outport(0xA1, 0xFF);
}

void i386_irq_init()
{
	i386_pic_init();
}

