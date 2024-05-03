#include <interrupts/idt.h>
#include <kernel/gdt.h>
#include <string.h>

__attribute__((aligned(0x10))) 
static struct idt_descriptor_entry idt[IDT_NUM_ENTRIES]; // Create an array of IDT entries; aligned for performance
static struct idtr_register idtr;

extern __attribute__((cdecl)) void idt_load(struct idtr_register *idtr);
ISRHandler handlers[IDT_NUM_ENTRIES];

void idt_table_init()
{
    int i;

    memset(idt, 0, sizeof(idt));
    memset(handlers, 0, sizeof(handlers));

    isr_install();
    for(i = 0; i < IDT_NUM_ENTRIES; i++)
    {
        idt_enable_gate(i);
    }

    idtr.Base = (struct idt_descriptor_entry *) &idt;
    idtr.size = sizeof(idt) - 1;
    idt_load(&idtr);
}

void idt_table_set_gate(int interrupt, void *address, uint8_t flags)
{
    idt[interrupt].base1 = (uint32_t)address & 0x0000ffff;
    idt[interrupt].segment_selector = KERNEL_CODE_SEGMENT;
    idt[interrupt].reserved = 0;
    idt[interrupt].type_attributes = flags;
    idt[interrupt].base2 = ((uint32_t)address >> 16) & 0x0000ffff;
}

void idt_enable_gate(int interrupt)
{
    idt[interrupt].type_attributes |= IDT_FLAG_PRESENT;
}

void idt_disable_gate(int interrupt)
{
    idt[interrupt].type_attributes &= ~IDT_FLAG_PRESENT;
}