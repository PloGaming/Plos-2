#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <kernel/multiboot2.h>

#define SSFN_CONSOLEBITMAP_TRUECOLOR

#define RGB_COLOR(red, green, blue) ((red << 16) | (green << 8) | (blue))
#define GET_RED(color) ((color >> 16) & 0xFF)
#define GET_GREEN(color) ((color >> 8) & 0xFF)
#define GET_BLUE(color) (color & 0xFF)

#define TERMINAL_BACKGROUND_COLOR RGB_COLOR(0xFF, 0xFF, 0xFF)
#define TERMINAL_FOREGROUND_COLOR RGB_COLOR(0x00, 0x00, 0x00)

void framebuffer_initialize(struct multiboot_tag_framebuffer* tmp);
void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color);
void framebuffer_terminal_putchar(char c);

#endif // FRAMEBUFFER_H