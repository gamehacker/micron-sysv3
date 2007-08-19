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

/* 
 * This is our entry point, everything starts here
 * Buggy BIOSes should add initialization codes later
 */
.global _start; _start:
	movb %dl, b_devi	/* dl stores the boot device id */
	test $0x80, %dl		/* which is passed from BIOS */
	jz  boot_fdd		/* goto floppy disk startup */
	jmp boot_hdd		/* goto hard disk startup */
	hlt

/* Booted from floppy disk */
boot_fdd:			/* TODO: Write floppy specified boot */
	hlt			/*       there's no specification for */
				/*       floppy in MFS now*/

/* Booted from hard disk */
boot_hdd:		
	call stat_dev 		/* detect boot device attribute */
	call stat_ksize		/* get kernel storage info on dev */
	call kern_copy		/* copy kernel from disk to buffer */
	jmp  entr_pm		/* enter protected mode */
	ret

/* 
 * Acquire the information about geometry of boot disk
 * should be fesiable for both floppy and hdd
 */
stat_dev:
	movb $0x08, %ah		/* function number */
	movb b_devi,%dl		/* drive number */
	int  $0x13
	jc   error		/* we don't want this to happen */
	movb %bl,    b_devt	/* store dev type if is floppy */
	movb %ch,    b_devc	/* low 8 bits of max cyl number */
	movb %cl,    b_devs	/* max sector number (bits 5-0) */
	andb $0x3F,  b_devs
	andw $0xC0,  %cx	/* get high 2 bits of max cyl number */
	shlw $0x02,  %cx	/* align it to position */
	addw %cx,    b_devc	/* add up to the cyl number data */
	andb $0x0F,  %dh	/* only lower 4 bits are used */
	movb %dh,    b_devh	/* max head number */
	movb %dl,    b_devn	/* number of drives */
	movw b_devs, %ax	/* calculate bytes per head */
	movw $512,   %bx
	mulw %bx
	movw %ax,    b_sizh	/* store size per head */
	ret

copy_c:	.word 0
copy_h:	.byte 0
copy_s:	.byte 0
copy_n:	.byte 0
copy_d:	.word 0x1000

/* Copy kernel from disk to buffer */
kern_copy:
	movw  b_ksec,  %ax	/* only for the first time */
	movb  b_devs,  %bl
	subb  %al,     %bl
	movb  %bl,     copy_n
	movb  %al,     copy_s
1:	movb  b_devi,  %dl	/* boot device i */
	movb  copy_n,  %al	/* sectors to transfer per time */
	movw  copy_c,  %bx	/* start cylinder number */
	movb  copy_h,  %dh	/* start head number */
	movb  copy_s,  %cl	/* start sector number */
	addb  $1,      %cl
	push  copy_d
	pop   %es
	movw  $0,      %si	/* beginning from 0x10000 */
	call  copy_block
	movw  b_kcnt,  %ax
	subw  copy_n,  %ax
	js    3f
	movw  %ax,     b_kcnt
	movw  b_sizh,  %ax
	shrw  $4,      %ax
	addw  %ax,     copy_d
	movb  b_devs,  %al
	movb  %al,     copy_n
	movb  b_ksec,  %al
	movb  %al,     copy_s
	movb  copy_h,  %al
	incb  %al
	movb  %al,     copy_h
	cmpb  b_devh,  %al
	jz    2f
	jmp   1b
2:	movw  copy_c,  %ax
	incw  %ax
	movw  %ax,     copy_c
	movb  $0,      copy_h
	cmpw  b_devc,  %ax
	jz    error
	jmp   1b
3:	ret

/* 
 * Function to acquire kernel size to copy, result in sectors, store
 * to b_ksec and b_kcnt.
 */
stat_ksize:
	movb $0x02,      %al	/* read super block, 2 sectors */
	movw $0x00,      %bx
	movb $0x00,      %dh
	movb $0x02,      %cl	/* sector 2 */
	movb b_devi,     %dl
	push %cs
	pop  %es
	movw $buf,       %si
	call copy_block
	movw %es:4(%bx), %ax	/* s_kblk */
	movw $0x02,      %cx	/* 2 sectors per block in MFS */
	mulw %cx
	incw %ax
	movw %ax,        b_ksec
	movw %es:6(%bx), %ax	/* s_kblkcnt */
	movw $0x02,      %cx	/* same as above */
	mulw %cx
	movw %ax,        b_kcnt
	ret

/* 
 * Function to read specified CHS to dest memory
 * Specifications: 
 *
 * Inputs:
 *
 *   AL    : Sectors to read
 *   BX    : Cylinder
 *   DH    : Head
 *   CL    : Sector
 *   DL    : Drive
 *   ES:SI : Destination
 */
copy_block:
	mov   $0x02, %ah
	shlb  $0x06, %bh	/* process high 2 bits of cylinder */
	orb   %bh,   %cl
	movb  %bl,   %ch
	movw  %si,   %bx
	int   $0x13
	jc    error
	ret
	
/* Function to enter protected mode */
entr_pm:
	cli			/* from here on, we disable interrupts */
	lgdt b_gdtr		/* load GDT register */
	movl %cr0,  %eax	/* enable protected mode */
	orl  $0x01, %eax
	movl %eax,  %cr0
	movw $0x10, %ax		/* setup value of various segments */
	movw %ax,   %ds
	movw %ax,   %es
	movw %ax,   %fs
	movw %ax,   %gs
	movw %ax,   %ss
	ljmp $0x08, $_start32	/* flush instruction pipe */

/* error handler */
error:
	hlt			/* when error is met, just halt */

/* Tell the assembler that we are generating 32 bit code */
.code32

/* Entry point of 32 bit mode */
_start32:
	call kern_move
	orl  %eax,   %eax
	movb b_devi, %al
	ljmp $0x08,  $0x100000	/* jump to kernel entry point */
				/* this is the point of no return */

/* Kernel Loader */
kern_move:
	movl $0x10000,  %esi
	movl $0x100000, %edi
1:	movl (%esi),    %eax
	movl %eax,      (%edi)
	cmpl $0x90000,  %esi
	jz   2f
	addl $0x04,     %esi
	addl $0x04,     %edi
	jmp  1b
2:	ret

/* Boot up information storage */
.global b_devi, b_devt, b_devn, b_devc, b_devh, b_devs, b_ksec, b_kcnt
b_devi: .byte 0			/* boot device indicator */
b_devt: .byte 0			/* boot device type for floppy only */
b_devn: .byte 0			/* number of drives */
b_devc: .word 0			/* boot device cylinder number */
b_devh: .byte 0			/* boot device head number */
b_devs: .byte 0			/* boot device sector number */
b_sizh: .word 0			/* bytes per head */
b_ksec: .long 0			/* kernel beginning sector */
b_kcnt: .long 0			/* kernel size in sector(s) */

/* Boot up temporary GDT, abandoned by kernel */
b_gdt:

/* Null entry */
.long 0
.long 0

/* 0x08 code segment */
.word 0xFFFF			/* limit 0..15 */
.word 0x0000			/* base  0..15 */
.byte 0x00			/* base 16..23 */
.byte 0x9E			/* P=1, DPL=0, code, Conforming, Read */
.byte 0xCF			/* 4 kb, 32bit code, limit 16..19=1111B */
.byte 0x00			/* base 24..31=00H */

/* 0x10 data segment */
.word 0xFFFF			/* limit 0..15 */
.word 0x0000			/* base  0..15 */
.byte 0x00			/* base 16..23 */
.byte 0x92			/* P=1, DPL=0, data, Read/Write */
.byte 0xCF			/* 4 kb, 32bit stack, limit 16..19=1111B */
.byte 0x00			/* base 24..31=00H */

/* GDT Register data */
b_gdtr:
.value b_gdtr - b_gdt -1
.long  b_gdt

/* Write our boot signature here so that BIOS can recongnize */
.org  510
.word 0xaa55
buf:				/* Temporary buffer for reading super blk */

