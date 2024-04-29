#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#include <graphics/vga.h>

uint16_t currX, currY;
enum video_type currentVideoType;
 
static enum video_type get_bios_area_video_type(void)
{
    return (enum video_type) ((*(const uint16_t*) VGA_VIDEO_MEMORY_DETECT_TYPE) & 0x30);
}

void vga_terminal_initialize(void)
{
    uint16_t *i;
    currX = currY = 0;
    currentVideoType = get_bios_area_video_type();

    // Clearing the screen
    switch (currentVideoType)
    {
        case VIDEO_TYPE_NONE:
        case VIDEO_TYPE_COLOUR:
            for(i = (uint16_t *)VGA_VIDEO_MEMORY_COLOR; i < (uint16_t *)VGA_VIDEO_MEMORY_COLOR + VGA_COLS * VGA_ROWS; i++)
            {
                *i = ' ' | (FONT_COLOR(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLUE) << 8);
            }
            break;
        case VIDEO_TYPE_MONOCHROME:
            for(i = (uint16_t *)VGA_VIDEO_MEMORY_MONOCHROME; i < (uint16_t *)VGA_VIDEO_MEMORY_MONOCHROME + VGA_COLS * VGA_ROWS; i++)
            {
                *i = ' ' | (FONT_COLOR(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK) << 8);
            }
            break;
    }
}

static void goOneLineDown(void)
{
    uint16_t *i;
    if(currY == VGA_ROWS-1)
    {
        switch (currentVideoType)
        {
            case VIDEO_TYPE_NONE:
            case VIDEO_TYPE_COLOUR:
                for(i = (uint16_t *)VGA_VIDEO_MEMORY_COLOR; i < (uint16_t *)VGA_VIDEO_MEMORY_COLOR + VGA_COLS * (VGA_ROWS-1); i++)
                {
                    *i = *(i + VGA_COLS);
                }
                for(; i < (uint16_t *) VGA_VIDEO_MEMORY_COLOR + (VGA_COLS * VGA_ROWS); i++)
                {
                    *i = ' ' | (FONT_COLOR(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLUE) << 8);
                }
                break;
            case VIDEO_TYPE_MONOCHROME:
                for(i = (uint16_t *)VGA_VIDEO_MEMORY_MONOCHROME; i < (uint16_t *)VGA_VIDEO_MEMORY_MONOCHROME + VGA_COLS * (VGA_ROWS-1); i++)
                {
                    *i = *(i + 2*VGA_COLS);
                }
                for(; i < (uint16_t *) VGA_VIDEO_MEMORY_MONOCHROME + (VGA_COLS * VGA_ROWS); i++)
                {
                    *i = ' ' | (FONT_COLOR(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK) << 8);
                }
                break;
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
    if(currX == VGA_COLS-1 || c == '\n')
    {
        goOneLineDown();
    }
    
    if(c != '\n')
    {
        switch (currentVideoType)
        {
            case VIDEO_TYPE_NONE:
            case VIDEO_TYPE_COLOUR:
                *(((uint16_t *)VGA_VIDEO_MEMORY_COLOR) + currX + currY*VGA_COLS) = c | (FONT_COLOR(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLUE) << 8);
                break;
            case VIDEO_TYPE_MONOCHROME:
                *(((uint16_t *)VGA_VIDEO_MEMORY_MONOCHROME) + currX + currY*VGA_COLS) = c | (FONT_COLOR(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK) << 8);
                break;
        }
        currX++;
    }
}

void vga_terminal_write(const char* data, size_t size)
{
    size_t i;
    for(i = 0; i < size; i++)
        vga_terminal_putchar(data[i]);
}

void vga_terminal_writestring(const char* data)
{
    vga_terminal_write(data, strlen(data));
}
