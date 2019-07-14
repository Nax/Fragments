BITS 64
SECTION .text
GLOBAL _start
EXTERN _init
EXTERN main

_start:
    ; In case we have a frame-pointer,
    ; we need to terminate the trace
    xor eax, eax
    sub rsp, 0x10
    mov [rsp + 0x00], rax
    mov [rsp + 0x08], rax

    call _init

    call main

.die:
    jmp .die

