###############################################################################
# Micron System V3 File System Image Creator Build System
# Copyright (C) 2007, Micron System Team
# Copyright (C) 2007, Martin Tang
# PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
###############################################################################

# Toolchain Flags
CCFLAGS=-Wall -O2

# Toolchain Configuration
CC=gcc $(CCFLAGS)

# Objective Destinations
OBJECTS=main.o mfs.o

# Targets
all:$(OBJECTS)
	@echo "[BUILDING] mkmfs"; $(CC) -o mkmfs.exe $?

clean:
	@echo "[REMOVE] $(OBJECTS) mkmfs.exe"; rm $(OBJECTS) mkmfs.exe

# Standard Procedures
.c.o:
	@echo "[HOSTCC] $@"; $(CC) -c -o $@ $<


