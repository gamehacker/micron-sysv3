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

	if( handler ){
        irq_mask( index, 1 );   //enable this irq
    }else{
        irq_mask( index, 0 );   //disable this irq
    }
}

void isr_install_syscall(void (*handler)(struct Register*))
{
	syscall = handler;
}

void isr_entry(struct Register *regs)
{
	if((regs->intn >= 0) && (regs->intn <= 31)) {
		PANIC(isr_handler[regs->intn] == 0, "ISR=%d  eip=%x", regs->intn, regs->eip);
		isr_handler[regs->intn](regs);
	} else if((regs->intn >= 32) && (regs->intn <= 47)) {
	    int irq = regs->intn-32;
		MSG(irq_handler[irq] == 0, "IRQ=%d", irq);
		if ( irq_handler[irq] )
            irq_handler[irq](regs);
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
	// is this a mistake?
	//outportb(0x20, 0x0);
	//outportb(0xA0, 0x0);
	outportb(0x21, 0xFB); //note: this is not 0xFF, or the second pic won't work :)
	outportb(0xA1, 0xFF);
}

//enable all irq
void pic_enable()
{
	outportb(0x21, 0x00);
	outportb(0xA1, 0x00);
}

//disable all irq
void pic_disable()
{
	outportb(0x21, 0xFF);
	outportb(0xA1, 0xFF);
}

void irq_init()
{
	pic_init();
}

// mask an irq
// enabled: 1   allow this irq
//          0   disable this irq
void irq_mask( int irq, int enabled )
{
	unsigned char mask_word;
	if (irq <0 || irq > 15 ) return;
	int flags;
	local_irq_save( flags );
	if( irq < 8 )
        mask_word = inportb( 0x21 ) ;
    else
        mask_word = inportb( 0xA1 ) ;
    switch( irq )
    {
        case 1:
        case 9:
            if( enabled )
                mask_word &= 0xFD ; // 0xFD = 1111 1101
            else
                mask_word |= 0x02 ; // 0x01 = 0000 0010
            break ;
        case 0:
        case 8:
            if( enabled )
                mask_word &= 0xFE ; // 0xFE = 1111 1110
            else
                mask_word |= 0x01 ; // 0x00 = 0000 0001
            break ;
        case 2 :
        case 10:
            if( enabled )
                mask_word &= 0xFB ; // 0xFB = 1111 1011
            else
                mask_word |= 0x04 ; // 0x04 = 0000 0100
            break ;
        case 3 :
        case 11:
            if( enabled )
                mask_word &= 0xF7 ; // 0xF7 = 1111 0111
            else
                mask_word |= 0x08 ; // 0x08 = 0000 1000
            break ;
        case 4 :
        case 12:
            if( enabled )
                mask_word &= 0xef ; // 0xEF = 1110 1111
            else
                mask_word |= 0x10 ; // 0x10 = 0001 0000
            break ;
        case 5 :
        case 13:
            if( enabled )
                mask_word &= 0xdf ; // 0xDF = 1101 1111
            else
                mask_word |= 0x20 ; // 0x20 = 0010 0000
            break ;
        case 6 :
        case 14:
            if( enabled )
                mask_word &= 0xBF ; // 0xBF = 1011 1111
            else
                mask_word |= 0x40 ; // 0x40 = 0100 0000
            break ;
        case 7 :
        case 15:
            if( enabled )
                mask_word &= 0x7F ; // 0x7f = 0111 1111
            else
                mask_word |= 0x80 ; // 0x80 = 1000 0000
            break ;
    }
    if( irq < 8 )
    {
        outportb( 0x21, mask_word );
    }else{
        outportb( 0xA1, mask_word );
    }
    local_irq_restore( flags );
}


