[BITS 32]

section .text

    global idt_load
idt_load:
    cli
    push ebp
    mov ebp, esp

    mov eax, [ebp + 0x8]
    lidt [eax]

    mov esp, ebp
    pop ebp
    sti
    ret

KERNEL_DATA_SEGMENT equ 0x10
extern ISR_master_handler

    global isr_common
align 4
isr_common:
    pusha
    push ds
    push es
    push fs
    push gs

    mov ax, KERNEL_DATA_SEGMENT
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov eax, esp
    push eax; Pointer to the stack for the c function
    cld
    call ISR_master_handler

    pop eax
    pop gs
    pop fs
    pop es
    pop ds
    
    popa
    add esp, 8
    sti
    iret

