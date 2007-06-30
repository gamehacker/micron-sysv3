
.code16
.text

.global init
init:
   mov $0x07C0, %ax
   mov %ax, %ds
   mov $msg, %si
1: lodsb
   or %al,%al # zero=end of str
   jz 2f   # get out
   mov $0x0E, %ah
   int $0x10
   jmp 1b

2:
   hlt

msg: .asciz "Hi, buddies, we got our first Micron boot :)\n" 

.org 510
.word 0xaa55
