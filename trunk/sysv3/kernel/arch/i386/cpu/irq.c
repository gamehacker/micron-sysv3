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
#include <device.h>

struct isr_regs
{
	unsigned int gs, fs, es, ds, ss;
	unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
	unsigned int int_no, err_code;
	unsigned int eip, cs, eflags, useresp, userss; // only valid from ring 3
};

void isr_entry(struct isr_regs *regs)
{
	DeviceWrite(CHRDEV, DEVID(0, 0), "Interrupt Occured\n", 18);
	DeviceWrite(CHRDEV, DEVID(0, 0), "TODO: cpu/irq.c  \n", 18);
	while(1);
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

