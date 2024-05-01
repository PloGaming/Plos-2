#ifndef GDT_H
#define GDT_H

#include <stdint.h>

struct gdt_accessByte
{
    unsigned int accessed : 1;
    unsigned int readWrite : 1;
    unsigned int dc : 1;
    unsigned int executable : 1;
    unsigned int system : 1;
    unsigned int dpl : 2;
    unsigned int present : 1;
} __attribute__((packed));

struct gdt_entry 
{
    uint16_t limit1;
    uint16_t base1;
    uint8_t base2;
    struct gdt_accessByte accessByte;
    uint8_t limit2Flag;
    uint8_t base3;
}  __attribute__((packed));

struct gdt_register
{
    uint16_t size;
    uint32_t address;
}  __attribute__((packed));

enum flags {

};

#endif // GDT_H