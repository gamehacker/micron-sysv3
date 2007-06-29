###############################################################################
# Micron System V3 Platform Builder
# Copyright (C) 2007, Huaxin Tang
# martintang25 AT gmail.com
###############################################################################

all:
	@echo "Building bootloader..."; \
		make --no-print-directory -C bootloader
	@echo "Building kernel..."; \
		make --no-print-directory -C kernel
	@echo "Building library..."; \
		make --no-print-directory -C library
	@echo "Building application..."; \
		make --no-print-directory -C application
	@echo "Building image tool..."; \
		make --no-print-directory -C image
	@echo "Installing kernel..."; \
		cp kernel/kernel.bin rootfs/system/
	@echo "Building root file system image..."; \
		./image/image.exe -v -r rootfs -f mfs -o micron.img \
		-b bootloader/bootloader.bin

clean:
	@-echo "Cleaning rootfs..."; \
		rm -r rootfs/system/* rootfs/program/*
	@-echo "Cleaning bootloader..."; \
		make --no-print-directory -C bootloader clean
	@-echo "Cleaning kernel..."; \
		make --no-print-directory -C kernel clean
	@-echo "Cleaning library..."; \
		make --no-print-directory -C library clean
	@-echo "Cleaning image tool..."; \
		make --no-print-directory -C image clean
	@-echo "Cleaning application..."; \
		make --no-print-directory -C application clean

