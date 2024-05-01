#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idtr_x86 {
    uint16_t size;
    uint32_t base;
} __attribute__((packed));

struct idt_entryFlags_x86 {
    unsigned int gate_type : 4;
    unsigned int zero: 1;
    unsigned int DPL: 2;
    unsigned int present: 1;
} __attribute__((packed));

struct idt_entry_x86 {
    uint16_t address_first;
    uint16_t segmentSelector;
    uint8_t reserved;
    struct idt_entryFlags_x86 entry_flags;
    uint16_t address_second;
} __attribute__((packed));

#define IDT_NUM_ENTRIES 256

void idt_initialize(void);

#endif // IDT_H