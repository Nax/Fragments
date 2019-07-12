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
; RDX: The new code segment
gdt_load:
    mov [GDT.Pointer], rdi
    mov [GDT.Size], si

    lgdt [GDT]

    ; Force a CS reload
    push QWORD rdx
    push QWORD gdt_loaded
    retfq

gdt_loaded:
    xor eax, eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ret
