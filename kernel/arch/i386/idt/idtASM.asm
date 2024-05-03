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
    


KERNEL_DATA_SEGMENT equ 0x10
    global isr_common
align 4
isr_common:
    pushad
   
    mov eax, 0
    mov ax, ds
    push eax

    mov ax, KERNEL_DATA_SEGMENT
    mov ds, ax ; modifichiamo tutti i segmenti 
    mov es, ax ; a quello del kernel
    mov fs, ax
    mov gs, ax

    cld

    call ..


    popad
    iret

