#!/bin/sh
set -e
. ./iso.sh

objcopy --only-keep-debug isodir/boot/plos.kernel kernel.sym
objcopy --strip-debug isodir/boot/plos.kernel
qemu-system-$(./target-triplet-to-arch.sh $HOST) -s -S -cdrom plos.iso &
gdb target remote localhost:1234 --command="debugGDB.txt"