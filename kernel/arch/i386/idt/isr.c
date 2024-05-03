#include <interrupts/idt.h>

__attribute__((noreturn))
void ISR_master_handler(void)
{
    __asm__ volatile ("cli; hlt");
}