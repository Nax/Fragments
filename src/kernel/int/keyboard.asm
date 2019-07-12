BITS 64
SECTION .text
GLOBAL int_handler_keyboard

int_handler_keyboard:
    xchg bx, bx
    iretq
