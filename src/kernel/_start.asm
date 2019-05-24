BITS 64
SECTION .text
GLOBAL _start

ALIGN 8
_start:
    hlt
    jmp _start
