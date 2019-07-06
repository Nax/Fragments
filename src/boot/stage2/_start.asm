BITS 16
SECTION .text.start

EXTERN enable_a20
EXTERN enter_mode_protected
EXTERN pmain

_start:
    ; We first need to establish a sane environment

    ; Disable interrupts
    cli

    ; Setup the stack at 0x7c00
    mov ax, 0x7c00
    mov sp, ax
    mov bp, ax

    ; Pre-push dl and si
    and edx, 0xff
    and esi, 0xffff
    push esi
    push edx

    ; Push a dummy return address for pmain
    push DWORD 0

    ; Set the segments to zero
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Force CS to zero
    jmp 0x00:_zerocs

_zerocs:
    ; Enable the a20 gate, this will be required
    ; for protected mode
    call enable_a20

    ; Enter the main bootloader code
    push DWORD pmain
    jmp enter_mode_protected
