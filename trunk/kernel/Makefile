###############################################################################
# Micron Kernel Build Environment
# Copyright (C) 2007, Martin Tang
# martintang25 AT gmail.com
###############################################################################

# Configure Host And Target Platform
ARCH=i686
CROSS=i686-elf

# Compile Flags
ASFLAGS=-g
CCFLAGS=-Wall -g -Iinclude

# Toolchain Configuration
AS=$(CROSS)-as  $(ASFLAGS)
CC=$(CROSS)-gcc $(CCFLAGS)
LD=$(CROSS)-ld  $(LDFLAGS)
export AS CC LD

include arch/$(ARCH)/Makefile

# Targets
all:
	@echo "****************************************************************"
	@echo "Before the build process, please setup the following variables:"
	@echo "    ARCH  - Target architecture(see directory names under arch)"
	@echo "    CROSS - Cross compiler prefix"
	@echo "Then type: make image"
	@echo "****************************************************************"

image: kernel.bin

kernel.bin:$(OBJECTS)
	@echo "[TARGETLD] $@"; $(LD) $(OBJECTS) -T$(LDSCRIPT) -o kernel.bin

clean:
	@echo "[REMOVE] $(OBJECTS) kernel.bin"; rm $(OBJECTS) kernel.bin

# Standard Procedures
.s.o:
	@echo "[TARGETAS] $@"; $(AS) -o $@ $<

.c.o:
	@echo "[TARGETCC] $@"; $(CC) -c -o $@ $<

