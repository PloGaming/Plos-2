#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <graphics/vga.h>

uint16_t currX, currY;

static uint16_t *vga_video_mem = (uint16_t *) NULL;
static uint32_t vga_height = 0;
static uint32_t vga_width = 0;

void vga_terminal_initialize(uint16_t *video_mem, uint32_t height, uint32_t width)
{
    uint16_t *i;
    currX = currY = 0;

    if(!video_mem || height <= 0 || width <= 0)
        return;

    vga_video_mem = video_mem;
    vga_height = height;
    vga_width = width;

    // Clearing the screen
    for(i = vga_video_mem; i < video_mem + vga_height * vga_width; i++)
    {
        *i = ' ' | (TEXT_COLOR << 8);
    }
}

static void goOneLineDown(void)
{
    uint16_t *i;
    if(currY == vga_height-1)
    {
        for(i = vga_video_mem; i < vga_video_mem + vga_width * (vga_height-1); i++)
        {
            *i = *(i + vga_width);
        }
        for(; i < vga_video_mem + (vga_width * vga_height); i++)
        {
            *i = ' ' | (TEXT_COLOR << 8);
        }
    } 
    else 
    {
        currY++;
    }

    currX = 0;
}

void vga_terminal_putchar(char c)
{
    if(currX == vga_width-1 || c == '\n')
    {
        goOneLineDown();
    }
    
    if(c != '\n')
    {
        *(vga_video_mem + currX + currY*vga_width) = c | (TEXT_COLOR << 8);
        currX++;
    }
}