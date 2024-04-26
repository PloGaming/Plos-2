#!/bin/sh
set -e
. ./build.sh
 
mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub
 
cp sysroot/boot/plos.kernel isodir/boot/plos.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "plos" {
	multiboot /boot/plos.kernel
}
EOF
grub-mkrescue -o plos.iso isodir