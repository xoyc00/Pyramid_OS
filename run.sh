#!/bin/sh
qemu-system-i386 -drive format=raw,file=disk_image.img,if=ide,index=0 -m 512M -enable-kvm -device VGA,vgamem_mb=64