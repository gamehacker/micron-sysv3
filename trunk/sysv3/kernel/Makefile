###############################################################################
# Micron System V3 Kernel Build Environment
# Copyright (C) 2007, Micron System Team
# Copyright (C) 2007, Martin Tang
# PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
###############################################################################

# Configure Host And Target Platform
ARCH=i386
CROSS=i686-elf-

# Compile Flags
ASFLAGS=-g
CCFLAGS=-g -Wall -fno-strict-aliasing -Iinclude -fno-builtin
CXXFLAGS=$(CCFLAGS)

# Toolchain Configuration
AS= $(CROSS)as  $(ASFLAGS)
CC= $(CROSS)gcc $(CCFLAGS)
CXX=$(CROSS)c++ $(CXXFLAGS)
LD= $(CROSS)ld  $(LDFLAGS)
OC= $(CROSS)objcopy
OD= $(CROSS)objdump

# Include Target Platform Makefile
include arch/$(ARCH)/Makefile

# Portable Kernel Objective Code Targets
OBJECTS+=filesys/vcache.o \
	 filesys/scache.o \
	 filesys/mount.o \
	 filesys/vfs.o \
	 filesys/ofile.o \
	 filesys/fs.o \
	 filesys/fs/mfs.o \
	 driver/device.o \
	 lib/module.o \
	 lib/export.o \
	 lib/libc.o \
	 lib/vsprintf.o \
	 lib/time.o

# Targets
all: boot micron

boot:$(BOOTSECT)
	@echo "[LINKING ] $@"; $(LD) $(BOOTSECT) -T$(BOOTSCPT) -o$@.elf
	@echo "[BUILDING] $@"; $(OC) -Obinary $@.elf $@
	@echo "[DUMPPING] $@"; $(OD) -S $@.elf > $@.dmp

micron:$(OBJECTS)
	@echo "[LINKING ] $@"; $(LD) $(OBJECTS) -T$(LDSCRIPT) -o$@.elf
	@echo "[BUILDING] $@"; $(OC) -Obinary $@.elf $@
	@echo "[DUMPPING] $@"; $(OD) -S $@.elf > $@.dmp

clean:
	@echo "[REMOVE] $(OBJECTS) $(BOOTSECT) boot micron"; \
		rm $(OBJECTS) $(BOOTSECT) \
		boot.elf boot micron.elf micron micron.dmp

help:
	@echo "****************************************************************"
	@echo "Before the build process, please setup the following variables:"
	@echo "    ARCH  - Target architecture(see directory names under arch)"
	@echo "    CROSS - Cross compiler prefix"
	@echo "Then type: make all"
	@echo "****************************************************************"

# Standard Procedures
.s.o:
	@echo "[TARGETAS] $@"; $(AS) -o $@ $<

.c.o:
	@echo "[TARGETCC] $@"; $(CC) -c -o $@ $<

.cc.o:
	@echo "[TARGETCXX] $@"; $(CXX) -c -o $@ $<

