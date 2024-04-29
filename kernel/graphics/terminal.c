#include <graphics/terminal.h>

static uint8_t video_mode = 0;

void terminal_initialize(struct multiboot_tag_framebuffer *framebuffer)
{
    if(!framebuffer)
        return;

    video_mode = framebuffer->common.framebuffer_type;
    switch(video_mode)
    {
        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
            // No one will use this mode ... right??
            break;
        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
            framebuffer_initialize(framebuffer);
            break;
        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
            vga_terminal_initialize(framebuffer->common.framebuffer_addr,
                                    framebuffer->common.framebuffer_height,
                                    framebuffer->common.framebuffer_width);	
            break;
        default:
            break;
    }
}

void terminal_putchar(char c)
{
    switch(video_mode)
    {
        case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
            // No one will use this mode ... right??
            break;
        case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
            
            break;
        case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
            vga_terminal_putchar(c);
            break;
        default:
            break;
    }
}

void terminal_write(const char* data, size_t size)
{
    size_t i;
    for(i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}
