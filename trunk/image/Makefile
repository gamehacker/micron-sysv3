###############################################################################
# Micron System V3 File System Image Creator Build System
# Copyright (C) 2007, Huaxin Tang
# martintang25 AT gmail.com
###############################################################################

# Toolchain Flags
CCFLAGS=-g -Wall
CXXFLAGS=-g -Wall

# Toolchain Configuration
CC=gcc $(CCFLAGS)
CXX=c++ $(CXXFLAGS)

# Objective Destinations
OBJECTS=main.o mfs.o

# Targets
all:$(OBJECTS)
	@echo "[BUILD   ] image"; $(CC) -o image.exe $?

clean:
	@echo "[REMOVE] $(OBJECTS) image.exe"; rm $(OBJECTS) image.exe

# Standard Procedures
.c.o:
	@echo "[HOSTCC] $@"; $(CC) -c -o $@ $<

.cc.o:
	@echo "[HOSTCXX] $@"; $(CXX) -c -o $@ $<


