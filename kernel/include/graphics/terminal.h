#ifndef TERMINAL_H
#define TERMINAL_H

#include <string.h>

#include <kernel/multiboot2.h>
#include <graphics/framebuffer.h>
#include <graphics/vga.h>

void terminal_initialize(struct multiboot_tag_framebuffer *framebuffer);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif // TERMINAL_H