#!/bin/bash

set -x -e

make -j8 CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm multi_v7_defconfig
make -j8 CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm
make -j8 CROSS_COMPILE=arm-linux-gnueabihf- ARCH=arm uImage LOADADDR=0xc2000040
mv arch/arm/boot/uImage arch/arm/boot/uImage.bin


# To boot into linux, create a tsv.tsv file with the following content:
#Opt	id	Name	Type		IP		Offset		Binary
#-	0x01	fsbl		Binary		none	0x00000000	tf-a-stm32mp/build/stm32mp1/release/tf-a-stm32mp157-somic.stm32
#-	0x03	fip		FIP		none	0x00000000	tf-a-stm32mp/build/stm32mp1/release/fip.bin
#P	0x10	kernel		System		ram0	0xc2000000	linux-stm32mp/arch/arm/boot/uImage.bin
#P	0x11	dtb		FileSystem	ram0	0xc4000000	linux-stm32mp/arch/arm/boot/dts/stm32mp157-somic.dtb

# FLash the tsv file to the board:
# STM32_Programmer_CLI -c port=usb1 -w tsv.tsv --detach
