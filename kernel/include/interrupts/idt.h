#ifndef IDT_H
#define IDT_H

#include <stdint.h>

struct idt_descriptor_entry
{
    uint16_t base1;
    uint16_t segment_selector;
    uint8_t reserved;
    uint8_t type_attributes;
    uint16_t base2;
} __attribute__((packed));

struct idtr_register
{
    uint16_t size;
    struct idt_descriptor_entry *Base;
} __attribute__((packed));

typedef enum
{
    IDT_TASK_GATE = 0x5,
    IDT_INTERRUPT_GATE_16 = 0x6,
    IDT_TRAP_GATE_16 = 0X7,
    IDT_INTERRUPT_GATE_32 = 0xE,
    IDT_TRAP_GATE_32 = 0xF,

    DPL_LEVEL0 = (0 << 5),
    DPL_LEVEL1 = (1 << 5),
    DPL_LEVEL2 = (2 << 5),
    DPL_LEVEL3 = (3 << 5),

    IDT_FLAG_PRESENT = 0x80

} IDT_FLAGS;

#define IDT_NUM_ENTRIES 256

struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
} __attribute__((packed));

void idt_table_init(void);
void idt_table_set_gate(int interrupt, void *address, uint8_t flags);
void idt_enable_gate(int interrupt);
void idt_disable_gate(int interrupt);

void isr_install(void);

__attribute__((noreturn))
void ISR_master_handler(struct regs *registers);

typedef void (*ISRHandler)(struct regs *reg);

#endif // IDT_H