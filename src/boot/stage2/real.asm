BITS 32
SECTION .text
GLOBAL enter_mode_real

enter_mode_real:
    ; Load a 16 bit temporary GDT
    lgdt [GDT16.Descriptor]

    ;Jump into 16 bit pmode
    jmp 0x08:_protected16_thunk

BITS 16
SECTION .data16
ALIGN 16

GDT16:
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
    db 0x0f
    db 0x00

.Data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 0x92
    db 0x0f
    db 0x00

.Descriptor:
    dw  $ - GDT16 - 1
    dd  GDT16

SECTION .text16
ALIGN 4
_protected16_thunk:
    ; Load descriptors
    mov ax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Disable protected mode
    mov eax, cr0
    and ax, 0xfffe
    mov cr0, eax

    ; Jump into real mode
    jmp 0x00:_real_thunk

_real_thunk:
    ; Load real descriptors
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; jmp to the target
    ret

