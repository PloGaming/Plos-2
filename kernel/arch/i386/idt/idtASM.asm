[BITS 32]

section .text

    global idt_load
idt_load:
    push ebp
    mov ebp, esp

    mov eax, [ebp + 0x8]
    lidt [eax]

    mov esp, ebp
    pop ebp
    
    
