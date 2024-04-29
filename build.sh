#!/bin/sh
set -e
. ./headers.sh
 
i686-elf-objcopy -O elf32-i386 -B i386 -I binary kernel/graphics/font.sfn kernel/graphics/font.o
for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE -j15 install)
done