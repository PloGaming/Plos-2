#include <graphics/framebuffer.h>
#include <string.h>

struct multiboot_tag_framebuffer framebuffer;

void framebuffer_initialize(struct multiboot_tag_framebuffer* tmp)
{
    memcpy(&framebuffer, tmp, sizeof(struct multiboot_tag_framebuffer));
}

void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color)
{
    if(!framebuffer.common.framebuffer_addr)
        return;
    
    if(x > framebuffer.common.framebuffer_width || y > framebuffer.common.framebuffer_height)
        return;

    *(uint32_t *)((uint8_t *)framebuffer.common.framebuffer_addr + 
        (framebuffer.common.framebuffer_bpp/8)*(x + y*framebuffer.common.framebuffer_width)) 
        = (GET_RED(color) << framebuffer.framebuffer_red_field_position) |
        (GET_GREEN(color) << framebuffer.framebuffer_green_field_position) |
        (GET_BLUE(color) << framebuffer.framebuffer_blue_field_position);
}