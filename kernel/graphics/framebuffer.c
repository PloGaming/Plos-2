#include <graphics/framebuffer.h>

void framebuffer_putpixel(struct multiboot_tag_framebuffer* framebuffer, uint32_t x, uint32_t y, uint32_t color)
{
    if(!framebuffer)
        return;

    uint32_t pixel = x*framebuffer->common.framebuffer_bpp + y*framebuffer->common.framebuffer_pitch;
    uint8_t *location = ((void *) (unsigned long)framebuffer->common.framebuffer_addr) + pixel;

    location[0] = BLUE(color);
    location[1] = GREEN(color);
    location[2] = RED(color);
}