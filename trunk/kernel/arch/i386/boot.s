/******************************************************************************
 * Micron System V3 - I386 Boot & Loader
 * Copyright (C) 2007, Huaxin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 ******************************************************************************
 * Design Notes:
 *   Based on the idea of implementing a MFS loader searching for kernel stored
 * in /boot/micron absolute path, and implement it in just one disk sector
 * (boot sector), the searching sequence is as follow:
 *   SuperBlk -> inode blocks -> root -> boot -> micron
 ******************************************************************************/

/* Tell the assembler that we are generating 16 bit code */
.code16
.file "boot.s"
.text

/* This is our entry point, everything starts here */
.global _start; _start:
	hlt


/* Write our boot signature here so that BIOS can recongnize */
.org  510
.word 0xaa55

