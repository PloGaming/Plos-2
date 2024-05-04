#ifndef UTILITY_H
#define UTILITY_H

#include <stdint.h>
#include <stdio.h>

static inline void outb(uint16_t port, uint8_t val)
{
    __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint8_t inb(uint16_t port)
{
    uint8_t ret;
    __asm__ volatile ( "inb %w1, %b0"
                   : "=a"(ret)
                   : "Nd"(port)
                   : "memory");
    return ret;
}

static inline void io_wait(void)
{
    outb(0x80, 0);
}

__attribute__((noreturn))
extern void panic();

enum errors {
    INVALID_MAGIC_NUMBER,
};

__attribute__((noreturn))
static void kernel_panic(int error)
{
    printf("[ERROR] kernel panic: %d\n", error);
    panic();
}

#endif // UTILITY_H