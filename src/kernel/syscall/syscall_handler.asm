BITS 64
SECTION .text
GLOBAL syscall_handler

; The single entrypoint for syscalls
syscall_handler:
    ; We just entered kernel-land and have no kernel stack
    ; A little dance with swapgs will solve that

    ; First, we need to preserve the userland stack
    mov rbx, rsp

    ; Then, load the proper kernel stack
    swapgs
    mov rsp, [gs:0x08]
    swapgs

    ; This is where we should in theory call an actual handler
    mov rax, -1

    ; Reload the userland stack
    mov rsp, rbx

    ; Return from syscall
    o64 sysret
