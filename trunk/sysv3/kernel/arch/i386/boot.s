/*****************************************************************************
 * Micron System V3 - I386 Boot & Loader
 * Copyright (C) 2007, Micron System Team
 * Copyright (C) 2007, Martin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************
 * Design Notes:
 *
 *   We try to make this as simple as possible.
 *
 *   First implement a basic kernel loading process, get info from super block
 *   then do the copy.
 *
 *   The identification of disk geometry is needed.
 *
 * Bootloader Memory Map:
 *
 *   As memtioned in BIOS global memory reference, the memory from range 0x500
 *   to 0xA0000 is avaliable for free use, our boot code loads through 0x7C00
 *   to 0x7E00, our kernel needs 512 kb of memory and also such size of buffer,
 *   so we concluded the following memory map:
 *
 *   0x07C00 ~ 0x07DFF = 512 byte : Bootloader
 *   0x0FFFF ~ 0x07E00 =  33 kb   : Bootloader stack (set by BIOS)
 *   0x10000 ~ 0x90000 = 512 kb   : Kernel image transfer buffer
 *   0x100000                     : Final destination of kernel image
 *
 * Hard Disk Geometry(BIOS CHS MODE ONLY):
 *
 *   Cylinder : 10 bits = 0 ~ 1023
 *   Head     :  8 bits = 0 ~  255
 *   Sector   :  6 bits = 1 ~   63
 *   Total    : 0x1F8000000 bytes
 *
 * Floppy Disk Geometry(Same as physical geometry):
 *
 *   Cylinder : 0 ~ 79
 *   Head     : 0 ~  1
 *   Sector   : 1 ~ 18
 *
 *   Total    : 0x168000 bytes
 *
 * MFS Hard Disk Data Map:
 *
 *   Block size: 1024 byte (2 sectors)
 *
 *   Block N : COUNT : TYPE
 *   Block 0 :     1 : boot block
 *   Block 1 :     1 : super block
 *   Block 2 :   512 : kernel block	** count is registered in super block
 *   ...
 *
 * MFS Floppy Disk Data Map:
 *
 *   (Unspecified yet)
 *
 *****************************************************************************/
.file "boot.s"
.text

/* Tell the assembler that we are generating 16 bit code */
.code16

/* This is our entry point, everything starts here */
/* Buggy BIOSes should add initialization codes later */
.global _start; _start:
	movb %dl, b_devi/* dl stores the boot device id */
	test %dl, 0x80	/* which is passed from BIOS */
	jz  boot_fdd	/* goto floppy disk startup */
	jmp boot_hdd	/* goto hard disk startup */
	hlt

/* Booted from floppy disk */
boot_fdd:		/* TODO: Write floppy specified boot method */
	hlt		/*       currently there's no specification for */
			/*       floppy in MFS standard, to be written later */

/* Booted from hard disk */
boot_hdd:		
	call stat_dev 	/* detect boot device attribute */
	call kern_copy	/* copy kernel from disk to buffer */
	jmp  entr_pm	/* enter protected mode */
	ret

/* Acquire the information about geometry of boot disk */
stat_dev:		/* TODO: Add int 0x13 call for dev status here */
	ret

/* Read from disk to kernel buffer */
kern_copy:		/* TODO: Add int 0x13 call for kern copy here */
	movb $0x02, %ah	/* function number */
	movb $0xFF, %al	/* number of sectors to read  */
	ret
	
/* Function to enter protected mode */
entr_pm:
	lgdt b_gdtr
	movl %cr0,  %eax/* enable protected mode */
	orl  $0x01, %eax
	movl %eax,  %cr0
	movw $0x10, %ax	/* setup value of various segments */
	movw %ax,   %ds
	movw %ax,   %es
	movw %ax,   %fs
	movw %ax,   %gs
	movw %ax,   %ss
	ljmp $0x08, $_start32	/* flush instruction pipe */

/* Tell the assembler that we are generating 32 bit code */
.code32

/* Entry point of 32 bit mode */
_start32:
	call kern_move
	jmp .
	jmp *0x100000	/* jump to kernel entry point */
			/* this is the point of no return */

/* Kernel Loader */
kern_move:
	movl $0x10000,  %esi
	movl $0x100000, %edi
1:
	movl (%esi),    %eax
	movl %eax,      (%edi)
	cmpl $0x90000,  %esi
	jz   2f
	addl $0x04,     %esi
	addl $0x04,     %edi
	jmp  1b
2:
	ret

/* Boot up information storage */
b_devi: .byte 0		/* boot device indicator */
b_devc: .word 0		/* boot device cylinder number */
b_devh: .byte 0		/* boot device head number */
b_devs: .byte 0		/* boot device sector number */

/* Boot up temporary GDT, abandoned by kernel */
b_gdt:

/* Null entry */
.long 0
.long 0

/* 0x08 code segment */
.word 0xFFFF		/* segment limit 0..15 */
.word 0x0000		/* base address  0..15 */
.byte 0x00		/* base address 16..23 */
.byte 0x9E		/* P=1, DPL=0, code, Conforming, Read */
.byte 0xCF		/* 4 kb, 32bit code, segment limit 16..19=1111B */
.byte 0x00		/* base address 24..31=00H */

/* 0x10 data segment */
.word 0xFFFF		/* segment limit 0..15 */
.word 0x0000		/* base address  0..15 */
.byte 0x00		/* base address 16..23 */
.byte 0x92		/* P=1, DPL=0, data, Read/Write */
.byte 0xCF		/* 4 kb, 32bit stack, segment limit 16..19=1111B */
.byte 0x00		/* base address 24..31=00H */

/* GDT Register data */
b_gdtr:
.value b_gdtr - b_gdt -1
.long  b_gdt

/* Write our boot signature here so that BIOS can recongnize */
.org  510
.word 0xaa55

