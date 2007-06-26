###############################################################################
# Micron Kernel Build Environment
# Copyright (C) 2007, Martin Tang
# martintang25 AT gmail.com
###############################################################################

# Configure Host And Target Platform
ARCH=i386
CROSS=i686-elf

# Compile Flags
ASFLAGS=-g
CCFLAGS=-Wall -g -Iinclude
CXXFLAGS=-Wall -g -Iinclude

# Toolchain Configuration
AS=$(CROSS)-as  $(ASFLAGS)
CC=$(CROSS)-gcc $(CCFLAGS)
CXX=$(CROSS)-c++ $(CXXFLAGS)
LD=$(CROSS)-ld  $(LDFLAGS)
export AS CC CXX LD

# Include Target Platform Makefile
include arch/$(ARCH)/Makefile

# Portable Kernel Objective Code Targets
OBJECTS+=

# Targets
kernel.bin:$(OBJECTS)
	@echo "[TARGETLD] $@"; $(LD) $(OBJECTS) -T$(LDSCRIPT) -o$@

clean:
	@echo "[REMOVE] $(OBJECTS) kernel.bin"; rm $(OBJECTS) kernel.bin

help:
	@echo "****************************************************************"
	@echo "Before the build process, please setup the following variables:"
	@echo "    ARCH  - Target architecture(see directory names under arch)"
	@echo "    CROSS - Cross compiler prefix"
	@echo "Then type: make image"
	@echo "****************************************************************"

# Standard Procedures
.s.o:
	@echo "[TARGETAS] $@"; $(AS) -o $@ $<

.c.o:
	@echo "[TARGETCC] $@"; $(CC) -c -o $@ $<

.cc.o:
	@echo "[TARGETCXX] $@"; $(CXX) -c -o $@ $<

