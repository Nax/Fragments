BITS 16

SECTION .data16
ALIGN 16
GDT32:
.Null:
    dw 0x0000
    dw 0x0000
    db 0x00
    db 0x00
    db 0x00
    db 0x00

.Code:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 0x9a
    db 0xcf
    db 0x00

.Data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 0x92
    db 0xcf
    db 0x00

.Descriptor:
    dw  $ - GDT32 - 1
    dd  GDT32

SECTION .text16
GLOBAL enter_mode_protected
enter_mode_protected:
    ; Load a 32 bit GDT
    lgdt [GDT32.Descriptor]

    ; Enable PE
    mov eax, cr0
    or ax, 1
    mov cr0, eax

    ; Enter protected mode
    jmp 0x08:_pmode_thunk

BITS 32
SECTION .text
_pmode_thunk:
    ; Load data segments
    mov eax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Jump to the argument
    ret
