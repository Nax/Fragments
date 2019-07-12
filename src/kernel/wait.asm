BITS 64

SECTION .bss
EXTERN kernel_stack_top

SECTION .text
GLOBAL kernel_wait
kernel_wait:
    ; Prevent a race condition with GS
    cli

    ; Get the local kernel stack
    swapgs
    mov rsp, [gs:0x08]
    swapgs

    ; Enable interrupts
    sti

.loop:
    ; Wait for an interrupt
    hlt
    jmp .loop
