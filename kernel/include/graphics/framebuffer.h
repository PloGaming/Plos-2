#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <kernel/multiboot2.h>

#define RGB_COLOR(red, green, blue) ((red << 16) | (green << 8) | (blue))
#define GET_RED(color) ((color >> 16) & 0xFF)
#define GET_GREEN(color) ((color >> 8) & 0xFF)
#define GET_BLUE(color) (color & 0xFF)

void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color);

#endif // FRAMEBUFFER_H