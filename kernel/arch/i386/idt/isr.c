#include <interrupts/idt.h>
#include <stdio.h>
#include <stddef.h>

extern ISRHandler handlers[];

char *g_Exceptions[] = {
    "Divide by zero error\n",
    "Debug\n",
    "Non-maskable Interrupt\n",
    "Breakpoint\n",
    "Overflow\n",
    "Bound Range Exceeded\n",
    "Invalid Opcode\n",
    "Device Not Available\n",
    "Double Fault\n",
    "Coprocessor Segment Overrun\n",
    "Invalid TSS\n",
    "Segment Not Present\n",
    "Stack-Segment Fault\n",
    "General Protection Fault\n",
    "Page Fault\n",
    "\n",
    "x87 Floating-Point Exception\n",
    "Alignment Check\n",
    "Machine Check\n",
    "SIMD Floating-Point Exception\n",
    "Virtualization Exception\n",
    "Control Protection Exception\n",
    "\n",
    "\n",
    "\n",
    "\n",
    "\n",
    "\n",
    "Hypervisor Injection Exception\n",
    "VMM Communication Exception\n",
    "Security Exception\n",
    "\n"};

__attribute__((noreturn))
void ISR_master_handler(struct regs *registers)
{
    if(handlers[registers->int_no] != NULL)
    {
        handlers[registers->int_no](registers);
    } 
    else if(registers->int_no > 31)
    {
        printf("Interrupt non gestito: %d\n", registers->int_no);
    }
    else
    {
        printf("Eccezione non gestita: %d\n", registers->int_no);
        printf("%s", g_Exceptions[registers->int_no]);
        __asm__ volatile ("cli; hlt");
    }   
}
