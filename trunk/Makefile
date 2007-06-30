###############################################################################
# Micron System V3 Platform Builder
# Copyright (C) 2007, Huaxin Tang
# martintang25 AT gmail.com
###############################################################################

all:
	@make --no-print-directory -C bootloader
	@make --no-print-directory -C kernel
	@make --no-print-directory -C library
	@make --no-print-directory -C application
	@make --no-print-directory -C image
	@cp kernel/kernel.bin rootfs/system/
	@image/image.exe -v -r rootfs -f mfs -o micron.img \
		-b bootloader/bootloader.bin

qemu:
	@qemu -L /qemu -hda micron.img -s -no-kqemu

dqemu:
	@qemu -L /qemu -hda micron.img -s -no-kqemu -S

clean:
	@-rm -r rootfs/system/* rootfs/program/* micron.img
	@make --no-print-directory -C bootloader clean
	@make --no-print-directory -C kernel clean
	@make --no-print-directory -C library clean
	@make --no-print-directory -C image clean
	@make --no-print-directory -C application clean

