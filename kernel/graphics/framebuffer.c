#define SSFN_CONSOLEBITMAP_TRUECOLOR        /* use the special renderer for 32 bit truecolor packed pixels */
#include <graphics/framebuffer.h>
#include <fonts/ssfn.h>
#include <string.h>
#include <stdio.h>

extern ssfn_font_t _binary_kernel_graphics_font_sfn_start[];
struct multiboot_tag_framebuffer framebuffer;

void framebuffer_initialize(struct multiboot_tag_framebuffer* tmp)
{
    uint32_t i, j;

    if(!tmp)
        return;

    memcpy(&framebuffer, tmp, sizeof(struct multiboot_tag_framebuffer));
    for(i = 0; i < framebuffer.common.framebuffer_height; i++)
    {
        for(j = 0; j < framebuffer.common.framebuffer_width; j++)
        {
            framebuffer_putpixel(j, i, TERMINAL_BACKGROUND_COLOR);
        }
    }

    // Set up for font drawing library
    ssfn_src = _binary_kernel_graphics_font_sfn_start;
    ssfn_dst.ptr = (uint8_t *)(unsigned long)framebuffer.common.framebuffer_addr;
    ssfn_dst.p = (framebuffer.common.framebuffer_bpp/8) * framebuffer.common.framebuffer_width;
    ssfn_dst.bg = 0xFFFFFFFF;
    ssfn_dst.fg = 0;
    ssfn_dst.x = 5;
    ssfn_dst.y = 0;
}

void framebuffer_putpixel(uint32_t x, uint32_t y, uint32_t color)
{
    if(!framebuffer.common.framebuffer_addr)
        return;
    
    if(x > framebuffer.common.framebuffer_width || y > framebuffer.common.framebuffer_height)
        return;

    *(uint32_t *)((uint8_t *)(unsigned long)framebuffer.common.framebuffer_addr + 
        (framebuffer.common.framebuffer_bpp/8)*(x + y*framebuffer.common.framebuffer_width)) = color;
}

void framebuffer_terminal_putchar(char c)
{
    if(c == '\n' || ssfn_dst.x >= framebuffer.common.framebuffer_width) 
    {
        ssfn_dst.y += ssfn_src->height;
        ssfn_dst.x = 5;
        if(c != '\n')
        {
            ssfn_putc(c); 
        }
    } 
    else
    {
        ssfn_putc(c);
    }
}