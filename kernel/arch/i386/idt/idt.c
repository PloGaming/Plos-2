#include <interrupts/idt.h>

__attribute__((aligned(0x10))) 
static struct idt_entry_x86 idt[IDT_NUM_ENTRIES]; // Create an array of IDT entries; aligned for performance
static struct idtr_x86 idtr;

extern void idt_load(struct idtr_x86 *idtr);

void initilize_idt(void)
{
    uint16_t i;
    for(i = 0; i < IDT_NUM_ENTRIES; i++)
    {
        
    }

    idtr.base = (uint32_t) &idt;
    idtr.size = (uint16_t) sizeof(idt);
    idt_load(&idtr);
}