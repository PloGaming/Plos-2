#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include <kernel/multiboot2.h>

#define RED(color)   ((color >> 16) & 255)
#define GREEN(color)  ((color >> 8) & 255)
#define BLUE(color) (color & 255)

void framebuffer_putpixel(struct multiboot_tag_framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t color);

#endif // FRAMEBUFFER_H