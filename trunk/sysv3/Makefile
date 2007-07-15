###############################################################################
# Micron System V3 Platform Builder
# Copyright (C) 2007, Micron System Team
# Copyright (C) 2007, Huaxin Tang
# PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
###############################################################################

all:
	@make --no-print-directory -C kernel
	@make --no-print-directory -C library
	@make --no-print-directory -C application
	@make --no-print-directory -C mkmfs
	@mkmfs/mkmfs.exe -v -r rootfs -f mfs \
	        -b kernel/boot -k kernel/micron -o micron.img
	@chmod +rwx micron.img

run:
	@start bochs -q -f scripts/run.bxrc

kdbg:
	@start bochs -q -f scripts/dbg.bxrc
	@i686-elf-gdb kernel/micron.elf

bdbg:
	@start bochs -q -f scripts/dbg.bxrc
	@i686-elf-gdb kernel/boot.elf

clean:
	-rm micron.img
	@make --no-print-directory -C kernel clean
	@make --no-print-directory -C library clean
	@make --no-print-directory -C mkmfs clean
	@make --no-print-directory -C application clean

