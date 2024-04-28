#include <stdio.h>

#include <kernel/tty.h>
#include <kernel/multiboot2.h>

void kernel_main() {

	terminal_initialize();
	printf("Hello, kernel World!\n");
}
