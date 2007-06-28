
.code16

.globl init
init:
	hlt

# Marking the ending of boot signature
.org 510
.byte 0x55
.byte 0xaa

