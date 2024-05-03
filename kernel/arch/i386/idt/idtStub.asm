[BITS 32]
section .text
extern isr_common

%macro isr_no_err_stub 1
global isr%+%1
isr%+%1:
    cli
    push 0
    push %1
    jmp isr_common
%endmacro

%macro isr_err_stub 1
global isr%+%1
isr%+%1:
    cli
    push %1
    jmp isr_common
%endmacro

; Define the first 31 reserved "exceptions"
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_err_stub    17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_err_stub    30
isr_no_err_stub 31

; Define the remaining "user defined interrupts"
%assign i 32
%rep    256-32
    isr_no_err_stub i
%assign i i+1
%endrep