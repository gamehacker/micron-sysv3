/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/
#include <io.h>
#include <idt.h>

#define IDT_ENTRIES 256

struct idt
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
}__attribute__((packed)) idt[IDT_ENTRIES];

struct idtr
{
	unsigned limit:16;
	unsigned address:32;
}__attribute__((packed)) idtr;

void idt_setup(unsigned index,		// target idt entry index
		    unsigned sel,		// handler code segment
		    unsigned offset,		// handler offset
		    unsigned p,			// presence
		    unsigned dpl,		// privilege level
		    enum idt_type type,	// idt type
		    enum idt_bits size)	// idt code bits
{
	idt[index].offset_l = offset & 0xFFFF;
	idt[index].offset_h = (offset>>16) & 0xFFFF;
	idt[index].sel      = sel;
	idt[index].p        = p;
	idt[index].dpl      = dpl;
	idt[index].type     = type;
	idt[index].size     = size;
	idt[index].zero0    = 0;
	idt[index].zero1    = 0;
}

void idt_load()
{
	idtr.limit  = sizeof(struct idt)*IDT_ENTRIES - 1;
	idtr.address= (unsigned)&idt;
	asm("lidt idtr");
}

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();
extern void isr_invalid();
extern void isr_syscall();

void idt_init()
{
	/* setup standard isrs */
	idt_setup( 0, 0x08, (unsigned)isr0, 1, 0, INTR, BITS32);
	idt_setup( 1, 0x08, (unsigned)isr1, 1, 0, INTR, BITS32);
	idt_setup( 2, 0x08, (unsigned)isr2, 1, 0, INTR, BITS32);
	idt_setup( 3, 0x08, (unsigned)isr3, 1, 0, INTR, BITS32);
	idt_setup( 4, 0x08, (unsigned)isr4, 1, 0, INTR, BITS32);
	idt_setup( 5, 0x08, (unsigned)isr5, 1, 0, INTR, BITS32);
	idt_setup( 6, 0x08, (unsigned)isr6, 1, 0, INTR, BITS32);
	idt_setup( 7, 0x08, (unsigned)isr7, 1, 0, INTR, BITS32);
	idt_setup( 8, 0x08, (unsigned)isr8, 1, 0, INTR, BITS32);
	idt_setup( 9, 0x08, (unsigned)isr9, 1, 0, INTR, BITS32);
	idt_setup(10, 0x08, (unsigned)isr10, 1, 0, INTR, BITS32);
	idt_setup(11, 0x08, (unsigned)isr11, 1, 0, INTR, BITS32);
	idt_setup(12, 0x08, (unsigned)isr12, 1, 0, INTR, BITS32);
	idt_setup(13, 0x08, (unsigned)isr13, 1, 0, INTR, BITS32);
	idt_setup(14, 0x08, (unsigned)isr14, 1, 0, INTR, BITS32);
	idt_setup(15, 0x08, (unsigned)isr15, 1, 0, INTR, BITS32);
	idt_setup(16, 0x08, (unsigned)isr16, 1, 0, INTR, BITS32);
	idt_setup(17, 0x08, (unsigned)isr17, 1, 0, INTR, BITS32);
	idt_setup(18, 0x08, (unsigned)isr18, 1, 0, INTR, BITS32);
	idt_setup(19, 0x08, (unsigned)isr19, 1, 0, INTR, BITS32);
	idt_setup(20, 0x08, (unsigned)isr20, 1, 0, INTR, BITS32);
	idt_setup(21, 0x08, (unsigned)isr21, 1, 0, INTR, BITS32);
	idt_setup(22, 0x08, (unsigned)isr22, 1, 0, INTR, BITS32);
	idt_setup(23, 0x08, (unsigned)isr23, 1, 0, INTR, BITS32);
	idt_setup(24, 0x08, (unsigned)isr24, 1, 0, INTR, BITS32);
	idt_setup(25, 0x08, (unsigned)isr25, 1, 0, INTR, BITS32);
	idt_setup(26, 0x08, (unsigned)isr26, 1, 0, INTR, BITS32);
	idt_setup(27, 0x08, (unsigned)isr27, 1, 0, INTR, BITS32);
	idt_setup(28, 0x08, (unsigned)isr28, 1, 0, INTR, BITS32);
	idt_setup(29, 0x08, (unsigned)isr29, 1, 0, INTR, BITS32);
	idt_setup(30, 0x08, (unsigned)isr30, 1, 0, INTR, BITS32);
	idt_setup(31, 0x08, (unsigned)isr31, 1, 0, INTR, BITS32);
	idt_setup(32, 0x08, (unsigned)isr32, 1, 0, INTR, BITS32);
	idt_setup(33, 0x08, (unsigned)isr33, 1, 0, INTR, BITS32);
	idt_setup(34, 0x08, (unsigned)isr34, 1, 0, INTR, BITS32);
	idt_setup(35, 0x08, (unsigned)isr35, 1, 0, INTR, BITS32);
	idt_setup(36, 0x08, (unsigned)isr36, 1, 0, INTR, BITS32);
	idt_setup(37, 0x08, (unsigned)isr37, 1, 0, INTR, BITS32);
	idt_setup(38, 0x08, (unsigned)isr38, 1, 0, INTR, BITS32);
	idt_setup(39, 0x08, (unsigned)isr39, 1, 0, INTR, BITS32);
	idt_setup(40, 0x08, (unsigned)isr40, 1, 0, INTR, BITS32);
	idt_setup(41, 0x08, (unsigned)isr41, 1, 0, INTR, BITS32);
	idt_setup(42, 0x08, (unsigned)isr42, 1, 0, INTR, BITS32);
	idt_setup(43, 0x08, (unsigned)isr43, 1, 0, INTR, BITS32);
	idt_setup(44, 0x08, (unsigned)isr44, 1, 0, INTR, BITS32);
	idt_setup(45, 0x08, (unsigned)isr45, 1, 0, INTR, BITS32);
	idt_setup(46, 0x08, (unsigned)isr46, 1, 0, INTR, BITS32);
	idt_setup(47, 0x08, (unsigned)isr47, 1, 0, INTR, BITS32);

	/* set all the other entries as invalid */
	int i;
	for(i=48; i<0xff; i++)
	idt_setup(i, 0x08, (unsigned)isr_invalid, 1, 0, INTR, BITS32);

	/* change 0x80 to system call */
	idt_setup(0x80, 0x08, (unsigned)isr_syscall, 1, 0, INTR, BITS32);

	idt_load();
}

