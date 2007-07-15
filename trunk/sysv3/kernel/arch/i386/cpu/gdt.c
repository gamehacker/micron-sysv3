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
 *   8. Application data segment *   9. Application stack segment
 *  10. Task switch selector(TSS) segment
 *
 * Developer Reference:
 *   1. IA-32 Intel Architecture Software Developer's Manual, Volume 3
 *****************************************************************************/

#define GDT_ENTRIES 10

struct i386_gdt
{
	unsigned int slim_l:16;	/* segment limit 15..0 */
	unsigned int badd_l:16;	/* base address 15..0 */
	unsigned int badd_m:8;	/* base address 23..16 */
	unsigned int type:5;	/* segment type */
	unsigned int dpl:2;	/* descriptor privilege level */
	unsigned int p:1;	/* segment present */
	unsigned int slim_h:4;	/* segment limit 19..16 */
	unsigned int avl:1;	/* available for use by system software */
	unsigned int l:1;	/* 64bit code segment */
	unsigned int db:1;	/* default operation syze */
	unsigned int g:1;	/* granularity */
	unsigned int badd_h:8;	/* base address 31..24 */
}i386_gdt[10];

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
	LIMIT_1B_UNIT,		/* segment limit in 1byte unit */
	LIMIT_4KB_UNIT		/* segment limit in 4KB unit */
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
int i386_gdt_edit(int index,			/* GDT Index */
		  int address,			/* segment address */
		  int limit,			/* segment limit */
		  int present,			/* segment present */
		  enum i386_gdt_db   db,	/* default data length */
		  enum i386_gdt_type type, 	/* descriptor type */
		  enum i386_gdt_gran gran)	/* data granularity */
{
	if(index > GDT_ENTRIES) {
	}
	return 0;
}


/* GDT manager initialization */
int i386_gdt_init()
{
	return 0;
}

