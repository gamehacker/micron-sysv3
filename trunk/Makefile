###############################################################################
# Micron System V3 Platform Builder
# Copyright (C) 2007, Huaxin Tang
# PROTECTED UNDER MICRON SYSTEM PUBLIC LICENSE AGREEMENT.
###############################################################################

all:
	@make --no-print-directory -C kernel
	@make --no-print-directory -C library
	@make --no-print-directory -C application
	@make --no-print-directory -C mkmfs
	@mkmfs/mkmfs.exe -v -d -r rootfs -f mfs \
	        -b kernel/boot -k kernel/micron -o micron.img
	@chmod +rwx micron.img

qemu:
	@qemu -hda micron.img -s -no-kqemu &

kdbg:
	@qemu -L /qemu -hda micron.img -s -no-kqemu -S &
	@i686-elf-gdb kernel/kernel.elf

bdbg:
	@qemu -L /qemu -hda micron.img -s -no-kqemu -S &
	@i686-elf-gdb kernel/boot.elf

clean:
	-rm micron.img
	@make --no-print-directory -C kernel clean
	@make --no-print-directory -C library clean
	@make --no-print-directory -C mkmfs clean
	@make --no-print-directory -C application clean

