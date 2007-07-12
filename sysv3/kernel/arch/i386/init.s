/*****************************************************************************
 * Micron System V3 - I386 Boot & Loader
 * Copyright (C) 2007, Huaxin Tang
 * PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
 *****************************************************************************/

.global _start; _start:
	movb $'B', 0xb8000
	hlt

