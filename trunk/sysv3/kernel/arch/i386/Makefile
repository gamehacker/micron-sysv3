###############################################################################
# Micron System V3 - i386 Processor Driver Build Sub-system
# Copyright (C) 2007, Martin Tang
# PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
###############################################################################

# Compiler Flags
CCFLAGS+=-I include/arch-i386
CXXFLAGS+=-I include/arch-i386

# Linker Script Selection
BOOTSCPT=arch/i386/boot.ld	# Bootsector Linker Script
LDSCRIPT=arch/i386/kernel.ld	# Kernel Linker Script

# Targets
BOOTSECT=arch/i386/boot.o
OBJECTS+=arch/i386/init.o

