; ---------------------------
; Bootloader file (NASM)
; Author: Polo Leonardo
; Source info: https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
; ---------------------------

; GRUB Multiboot2 header constants
; --- This number identifies the GRUB multiboot2
MAGIC_NUMBER    equ     0xE85250D6
; --- 0 = 32 bit i386, 4 = 32 bit MIPS
ARCHITECTURE	equ		0


; GDT 32/64 bit constants (for a flat memory model)
; --- The base addresses and limits of the descriptors
KERNEL_BASE 	equ		0x00
KERNEL_LIMIT	equ		0xfffff
USER_BASE		equ		0x00
USER_LIMIT		equ		0xfffff
; --- GDT descriptor flag field
; ------ 4Kib limit granularity
LIMIT_GRAN		equ		0b1
; ------ Those are 32bit descriptors
DB_SIZE			equ		0b1
; ------ The actual flag for every descriptor
GDT_FLAG		equ 	(LIMIT_GRAN << 3) | (DB_SIZE << 2)
; --- GDT Access bytes fields
; ------ It indicates that a descriptor is present
PRESENT			equ		0b1
; ------ Descriptor privilege level (0 = highest, 3 = lowest)
KERNEL_DPL		equ		0
USER_DPL		equ		0b11
; ------ Descriptor type (0 = system descriptor, 1 = data or code descriptor)
NONSYSTEM_DESC	equ		1
; ------ Defines the segment as a code segment or a data segment
EXECUTABLE		equ		1
; ------ Code in code descriptor can only be executed from rings lower or equal to DPL 
CODE_DC			equ 	0
; ------ Setting the code segments as readable (1) or not (0)
CODE_READ		equ		1
; ------ Setting the data segments as writable (1) or not (0)
DATA_WRITE		equ		1
; ------ Access byte flags
KERNEL_CODE_ACC	equ		(PRESENT << 7) | (KERNEL_DPL << 5) | (NONSYSTEM_DESC << 4) | (EXECUTABLE << 3) | (CODE_DC << 2) | (CODE_READ << 1)
KERNEL_DATA_ACC equ 	(PRESENT << 7) | (KERNEL_DPL << 5) | (NONSYSTEM_DESC << 4) | (CODE_DC << 2) | (DATA_WRITE << 1)
USER_CODE_ACC	equ		(PRESENT << 7) | (USER_DPL << 5) | (NONSYSTEM_DESC << 4) | (EXECUTABLE << 3) | (CODE_DC << 2) | (CODE_READ << 1)
USER_DATA_ACC 	equ 	(PRESENT << 7) | (USER_DPL << 5) | (NONSYSTEM_DESC << 4) | (CODE_DC << 2) | (DATA_WRITE << 1)

; Start of the .multiboot section
; This will be relocated by the linker script
; It must be aligned to 32-bit boundary and it must be
; in the first 8192 bytes of the OS image
	section .multiboot

	align 8 ; alignment on 8 bytes (64 bit)
multiboot_header:
	dd  MAGIC_NUMBER
	dd	ARCHITECTURE
	dd	multiboot_header_end - multiboot_header
	dd -(MAGIC_NUMBER + ARCHITECTURE + (multiboot_header_end - multiboot_header))

	align 8
start_frambuffer_tag:
	dw 0x5
	dw 0
	dd end_framebuffer_tag - start_frambuffer_tag
	dd 0
	dd 0
	dd 0
end_framebuffer_tag:

	align 8
	dw 0
	dw 0
	dd 8
multiboot_header_end:


; Start of .bss section
; We need a functioning stack and unfortunately
; GRUB multiboot doesn't specify the ESP and EBP values
; it's up to us to create it.
; We define it in it's own section (marked as nobits)
	section .bss

	; System V ABI specified that stack must be aligned on 2 bytes
	align 2 

stack_bottom:
	resb 16384 ; Reserve 16 KiB 
stack_top:


; Start of .text section
; Here we will specify the entry point of our kernel _start
; as defined in the linker script.
; Obviously this function does not return.
	section .text

	global _start:function (end - _start )
	global panic

	[BITS 32]
_start:

	; Here we are in 32-bit x86 protected mode 
	; --- Interrupts are disabled
	; --- Paging is disabled
	; --- Processor state as defined in GRUB Multiboot protocol
	
	; Setup stack
	; Necessary for calling functions
	mov esp, stack_top

	; EBX contains the base address of the multiboot address
	push ebx
	; EAX contains the magic number to validate the GRUB multiboot boot
	push eax 

	; Crucial CPU configuration
	; GDT (Global descriptor table)
	; 1) It's necessary to disable interrupts otherwise something bad can happen...
	cli
	; 2) Loading the GDTR register with the appropriate structure
	lgdt [gdtr]
	; 3) The effects apply on the new load of the segment selectors
	; 0x08 is the kernel code segment 
	jmp	0x08:.reload_CS
.reload_CS:
	; 0x10 is the kernel data segment
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	; 4) Re-enable interrupts
	sti
	
	; TODO configure paging
	
	; Since the state of EFLAGS is not defined
	; We define it
	push 0x00
	popfd

	; Calling the global contructors
	extern _init
	call _init

	; Calling the high-level kernel function (writte in C)
	; The stack is aligned so the call won't have any problems
	extern kernel_main
	call kernel_main

	; Calling the global destructors
	extern _fini
	call _fini

panic:
	; After the return we put the CPU in an infinite loop
	; 1) Disable interrupts
	cli ; Clear interrupt flag
	; 2) Wait for the next interrupt (won't happen)
.a:	hlt ; Halt
	; 3) Jump to halt again (only happens if the CPU wakes up due to a non-maskable interrupt)
	jmp .a

end:

; Start of .rodata section
	section .rodata
	; Definition of the GDT (each entry is 8 bytes)
gdt:
	; The null entry
	dq	0x00
	; Kernel code segment descriptor
	dw	KERNEL_LIMIT & 0xffff
	dw	KERNEL_BASE & 0xffff
	db	(KERNEL_BASE >> 16) & 0xff
	db 	KERNEL_CODE_ACC
	db	((KERNEL_LIMIT >> 16) & 0xf) | (GDT_FLAG << 4)
	db  (KERNEL_BASE >> 24) & 0xff
	; Kernel data segment descriptor
	dw	KERNEL_LIMIT & 0xffff
	dw	KERNEL_BASE & 0xffff
	db	(KERNEL_BASE >> 16) & 0xff
	db 	KERNEL_DATA_ACC
	db	((KERNEL_LIMIT >> 16) & 0xf) | (GDT_FLAG << 4)
	db  (KERNEL_BASE >> 24) & 0xff
	; User code segment descriptor
	dw	USER_LIMIT & 0xffff
	dw	USER_BASE & 0xffff
	db	(USER_BASE >> 16) & 0xff
	db 	USER_CODE_ACC
	db	((USER_LIMIT >> 16) & 0xf) | (GDT_FLAG << 4)
	db  (USER_BASE >> 24) & 0xff
	; User data segment descriptor
	dw	USER_LIMIT & 0xffff
	dw	USER_BASE & 0xffff
	db	(USER_BASE >> 16) & 0xff
	db 	USER_DATA_ACC
	db	((USER_LIMIT >> 16) & 0xf) | (GDT_FLAG << 4)
	db  (USER_BASE >> 24) & 0xff
	; Tss 
endgdt:

; The GDTR structure
gdtr:
	dw	endgdt-gdt-1
	dd	gdt
