BITS 64
SECTION .init
GLOBAL _init

_init:
    push rbp
    mov rbp, rsp

SECTION .fini
GLOBAL _fini

_fini:
    push rbp
    mov rbp, rsp
