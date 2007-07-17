/*****************************************************************************
 * Micron System V3 - I386 Architecture Driver
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Design Notes:
 *     This is the I386 GDT manager, providing a standard controling interface
 *   to I386 processor memory management segment subsystem.
 *
 * GDT Entries:
 *   1. Null entry
 *   2. Kernel code segment
 *   3. Kernel data segment
 *   4. Kernel stack segment
 *   4. V8086 code segment
 *   5. V8086 data segment
 *   6. V8086 stack segment
 *   7. Application code segment
 *   8. Application data segment
 *   9. Application stack segment
 *  10. Task switch selector(TSS) segment
 *
 * Developer Reference:
 *   1. IA-32 Intel Architecture Software Developer's Manual, Volume 3
 *****************************************************************************/

#define GDT_ENTRIES 10

struct i386_gdt
{
	unsigned slim_l:16;	/* segment limit 15..0 */
	unsigned badd_l:16;	/* base address 15..0 */
	unsigned badd_m:8;	/* base address 23..16 */
	unsigned type:5;	/* segment type */
	unsigned dpl:2;		/* descriptor privilege level */
	unsigned p:1;		/* segment present */
	unsigned slim_h:4;	/* segment limit 19..16 */
	unsigned avl:1;		/* available for use by system software */
	unsigned l:1;		/* 64bit code segment */
	unsigned db:1;		/* default operation syze */
	unsigned g:1;		/* granularity */
	unsigned badd_h:8;	/* base address 31..24 */
}__attribute__((packed)) i386_gdt[10];

struct i386_gdtr
{
	unsigned limit:16;
	unsigned base:32;
}__attribute__((packed)) i386_gdtr;

enum i386_gdt_s
{
	SYSTEM,			/* system segment */
	CODE_DATA		/* code or data */
};

enum i386_gdt_db
{
	BITS16,			/* 16 bit segment */
	BITS32			/* 32 bit segment */
};

enum i386_gdt_gran
{
	UNIT_1B,		/* segment limit in 1byte unit */
	UNIT_4K			/* segment limit in 4KB unit */
};

enum i386_gdt_type
{
	SYST_RES0,
	SYST_BITS16_TSS_AVAL,
	SYST_LDT,
	SYST_BITS16_TSS_BUSY,
	SYST_BITS16_CAL_GATE,
	SYST_TASK_GATE,
	SYST_BITS16_INT_GATE,
	SYST_BITS16_TRP_GATE,
	SYST_RES1,
	SYST_BITS32_TSS_AVAL,
	SYST_RES2,
	SYST_BITS32_TSS_BUSY,
	SYST_BITS32_CAL_GATE,
	SYST_RES3,
	SYST_BITS32_INT_GATE,
	SYST_BITS32_TRP_GATE,
	DATA_READ,
	DATA_READ_ACCESSED,
	DATA_READ_WRITE,
	DATA_READ_WRITE_ACCESSED,
	DATA_READ_EXP,
	DATA_READ_EXP_ACCESSED,
	DATA_READ_WRITE_EXP,
	DATA_READ_WRITE_EXP_ACCESSED,
	CODE_EXEC,
	CODE_EXEC_ACCESSED,
	CODE_READ_EXEC,
	CODE_READ_EXEC_ACCESSED,
	CODE_EXEC_CONFORM,
	CODE_EXEC_CONFORM_ACCESSED,
	CODE_READ_EXEC_CONFORM,
	CODE_READ_EXEC_CONFORM_ACCESSED
};

/* GDT entry modifier */
int i386_gdt_edit(unsigned index,		/* GDT Index */
		  unsigned address,		/* segment address */
		  unsigned limit,		/* segment limit */
		  unsigned present,		/* segment present */
		  unsigned dpl,			/* descriptor privilege */
		  enum i386_gdt_db   db,	/* default data length */
		  enum i386_gdt_type type, 	/* descriptor type */
		  enum i386_gdt_gran gran)	/* data granularity */
{
	if(index > GDT_ENTRIES)
		return -1;			/* no such GDT entry */
	i386_gdt[index].slim_l = limit & 0xFFFF;
	i386_gdt[index].slim_h = (limit>>16) & 0xF;
	i386_gdt[index].badd_l = address & 0xFFFF;
	i386_gdt[index].badd_m = (address>>16) & 0xFF;
	i386_gdt[index].badd_h = (address>>24) & 0xFF;
	i386_gdt[index].p      = present;
	i386_gdt[index].dpl    = dpl;
	i386_gdt[index].db     = db;
	i386_gdt[index].type   = type;
	i386_gdt[index].g      = gran;
	i386_gdt[index].l      = 0;
	i386_gdt[index].avl    = 0;
	return 0;
}

void i386_gdt_load()
{
	i386_gdtr.base = (unsigned)&i386_gdt;
	i386_gdtr.limit= sizeof(i386_gdt) - 1;
	asm("lgdt i386_gdtr");
	asm("movw $0x18, %ax");
	asm("movw %ax,   %ss");
	asm("movw $0x10, %ax");
	asm("movw %ax,   %ds");
	asm("movw %ax,   %es");
	asm("movw %ax,   %fs");
	asm("movw %ax,   %gs");
	asm("ljmp $0x08, $newgdt");
	asm("newgdt:");
}

/* GDT manager initialization */
void i386_gdt_init()
{
	/* Null GDT entry */
	i386_gdt_edit(0,0,0,0,0,0,0,0);

	/* Kernel segments */
	i386_gdt_edit(1,0,0xFFFFF,1,0,BITS32,CODE_READ_EXEC_CONFORM,UNIT_4K);
	i386_gdt_edit(2,0,0xFFFFF,1,0,BITS32,DATA_READ_WRITE,UNIT_4K);
	i386_gdt_edit(3,0,0xFFFFF,1,0,BITS32,DATA_READ_WRITE,UNIT_4K);

	/* V8086 segments */
	i386_gdt_edit(4,0,0xFFFFF,1,0,BITS16,CODE_READ_EXEC_CONFORM,UNIT_1B);
	i386_gdt_edit(5,0,0xFFFFF,1,0,BITS16,DATA_READ_WRITE,UNIT_1B);
	i386_gdt_edit(6,0,0xFFFFF,1,0,BITS16,DATA_READ_WRITE,UNIT_1B);

	/* Application segments */
	i386_gdt_edit(7,0,0xFFFFF,1,3,BITS32,CODE_READ_EXEC_CONFORM,UNIT_4K);
	i386_gdt_edit(8,0,0xFFFFF,1,3,BITS32,DATA_READ_WRITE,UNIT_4K);
	i386_gdt_edit(9,0,0xFFFFF,1,3,BITS32,DATA_READ_WRITE,UNIT_4K);

	/* Load new GDT */
	i386_gdt_load();
}

/* Install a TSS to segment */
void i386_gdt_tss(unsigned tadd, unsigned size)
{
	i386_gdt_edit(10,tadd,size,1,0,BITS32,SYST_BITS32_TSS_AVAL,UNIT_1B);
}

