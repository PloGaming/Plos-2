#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H
 
#include <stdint.h>
#include <stddef.h>

#define VGA_VIDEO_MEMORY_COLOR ((uint8_t *)0xB8000)
#define VGA_VIDEO_MEMORY_MONOCHROME ((uint8_t *)0xB0000)
#define VGA_VIDEO_MEMORY_DETECT_TYPE ((uint8_t *)0x410)

#define VGA_ROWS 25
#define VGA_COLS 80

enum video_type
{
    VIDEO_TYPE_NONE = 0x00,
    VIDEO_TYPE_COLOUR = 0x20,
    VIDEO_TYPE_MONOCHROME = 0x30,
};

enum video_color
{
    VIDEO_COLOR_BLACK,
    VIDEO_COLOR_BLUE,
    VIDEO_COLOR_GREEN,
    VIDEO_COLOR_CYAN,
    VIDEO_COLOR_RED,
    VIDEO_COLOR_PURPLE,
    VIDEO_COLOR_BROWN,
    VIDEO_COLOR_GRAY,
    VIDEO_COLOR_DARK_GRAY,
    VIDEO_COLOR_LIGHT_BLUE,
    VIDEO_COLOR_LIGHT_GREEN,
    VIDEO_COLOR_LIGHT_CYAN,
    VIDEO_COLOR_LIGHT_RED,
    VIDEO_COLOR_LIGHT_PURPLE,
    VIDEO_COLOR_YELLOW,
    VIDEO_COLOR_WHITE,
};

#define FONT_COLOR(FOREGROUND, BACKGROUND) ((uint8_t)((FOREGROUND & 0xF) | ((BACKGROUND & 0x7) << 4)))
#define TEXT_COLOR FONT_COLOR(VIDEO_COLOR_WHITE, VIDEO_COLOR_BLACK)


void vga_terminal_initialize(uint16_t *video_mem, uint32_t height, uint32_t width);
void vga_terminal_putchar(char c);

#endif