BITS 64

SECTION .bss
ALIGN 16
GDT:
.Size:
    resb 0x2
.Pointer:
    resb 0x8

SECTION .text
GLOBAL gdt_load

; RDI: A pointer to GDT entries
; RSI: A count of entries
gdt_load:
    shl esi, 3
    dec esi

    mov [GDT.Pointer], rdi
    mov [GDT.Size], si

    lgdt [GDT]

    ; Force a CS reload
    push QWORD 0x08
    push QWORD gdt_loaded
    retfq

gdt_loaded:
    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ret
