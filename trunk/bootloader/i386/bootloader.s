
.code16

.global init
init:
	hlt

.org 510
.byte 0x55
.byte 0xaa
