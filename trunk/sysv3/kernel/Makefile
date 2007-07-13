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
CCFLAGS=-Wall -g -Iinclude
CXXFLAGS=-Wall -g -Iinclude

# Toolchain Configuration
AS=$(CROSS)as  $(ASFLAGS)
CC=$(CROSS)gcc $(CCFLAGS)
CXX=$(CROSS)c++ $(CXXFLAGS)
LD=$(CROSS)ld  $(LDFLAGS)
OC=$(CROSS)objcopy

# Include Target Platform Makefile
include arch/$(ARCH)/Makefile

# Portable Kernel Objective Code Targets
OBJECTS+=driver/modinit.o

# Targets
all: boot micron

boot:$(BOOTSECT)
	@echo "[LINKING ] $@"; $(LD) $(BOOTSECT) -T$(BOOTSCPT) -o$@.elf
	@echo "[BUILDING] $@"; $(OC) -Obinary $@.elf $@

micron:$(OBJECTS)
	@echo "[LINKING ] $@"; $(LD) $(OBJECTS) -T$(LDSCRIPT) -o$@.elf
	@echo "[BUILDING] $@"; $(OC) -Obinary $@.elf $@

clean:
	@echo "[REMOVE] $(OBJECTS) $(BOOTSECT) boot micron"; \
		rm $(OBJECTS) $(BOOTSECT) \
		boot.elf boot micron.elf micron

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

