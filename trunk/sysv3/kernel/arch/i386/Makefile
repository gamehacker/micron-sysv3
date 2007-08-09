###############################################################################
# Micron System V3 - i386 Processor Driver Build Sub-system
# Copyright (C) 2007, Micron System Team
# Copyright (C) 2007, Martin Tang
# PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
###############################################################################

# Compiler Flags
CCFLAGS+= -I include/arch-i386
CXXFLAGS+=-I include/arch-i386

# Linker Script Selection
BOOTSCPT=arch/i386/boot/boot.ld		# Bootloader Linker Script
LDSCRIPT=arch/i386/kernel.ld		# Kernel Linker Script

# Targets
# Bootloader objectives
BOOTSECT=arch/i386/boot/boot.o

# kernel objectives
OBJECTS+=arch/i386/init/init.o \
	 arch/i386/init/main.o \
	 arch/i386/init/memsize.o \
	 arch/i386/init/config.o \
	 arch/i386/cpu/idt.o \
	 arch/i386/cpu/irq.o \
	 arch/i386/cpu/isr.o \
	 arch/i386/cpu/clock.o \
	 arch/i386/cpu/gdt.o \
	 arch/i386/cpu/mmu.o \
	 arch/i386/cpu/dma.o \
	 arch/i386/cpu/tss.o \
	 arch/i386/cpu/io.o \
	 arch/i386/cpu/tty.o \
	 driver/blkdev/hdd.o

