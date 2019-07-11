BITS 64

SECTION .bss
EXTERN kernel_stack_top

SECTION .text
GLOBAL kernel_wait
kernel_wait:
    ; Reset the kernel stack
    mov rsp, kernel_stack_top

    ; Enable interrupts
    sti

.loop:
    ; Wait for an interrupt
    hlt
    jmp .loop
