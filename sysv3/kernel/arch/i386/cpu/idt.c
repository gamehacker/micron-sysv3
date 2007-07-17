/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

#define IDT_ENTRIES 256

struct i386_idt
{
	unsigned offset_l:16;	/* lower 16 bit of Offset to procedure entry */
	unsigned sel:16;	/* destination code segment selector */
	unsigned zero0:8;	/* these bits are always zero */
	unsigned type:3;	/* gate type 5=Task, 6=Intr, 7=Trap*/
	unsigned size:1;	/* size of gate: 1=32bits; 0=16bits */
	unsigned zero1:1;	/* this bit is always zero */
	unsigned dpl:2;		/* descriptor privilege level */
	unsigned p:1;		/* present */
	unsigned offset_h:16;	/* higher 16 bits of offset */
}__attribute__((packed)) i386_idt[IDT_ENTRIES];

enum i386_idt_type
{
	TASK = 5,		/* Task Gate */
	INTR = 6,		/* Interrupt Gate */
	TRAP = 7		/* Trap Gate */
};

enum i386_idt_bits
{
	BITS16,			/* 16 bits gate */
	BITS32			/* 32 bits gate */
};

void i386_idt_setup(unsigned index,		// target idt entry index
		    unsigned sel,		// handler code segment
		    unsigned offset,		// handler offset
		    unsigned p,			// presence
		    unsigned dpl,		// privilege level
		    enum i386_idt_type type,	// idt type
		    enum i386_idt_bits size)	// idt code bits
{
	i386_idt[index].offset_l = offset & 0xFFFF;
	i386_idt[index].offset_h = (offset>>16) & 0xFFFF;
	i386_idt[index].sel      = sel;
	i386_idt[index].p        = p;
	i386_idt[index].dpl      = dpl;
	i386_idt[index].type     = type;
	i386_idt[index].size     = size;
	i386_idt[index].zero0    = 0;
	i386_idt[index].zero1    = 0;
}

void i386_idt_load()
{
	asm("lidt i386_idt");
}

void i386_idt_init()
{
	i386_idt_load();
}

