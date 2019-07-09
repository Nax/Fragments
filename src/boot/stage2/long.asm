BITS 32

SECTION .data
ALIGN 16
GDT64:
.Null:
    dw 0
    dw 0
    db 0
    db 0
    db 0
    db 0
.Code:
    dw 0
    dw 0
    db 0
    db 10011000b
    db 10101111b
    db 0
.Data:
    dw 0
    dw 0
    db 0
    db 10010010b
    db 00000000b
    db 0
.Pointer:
    dw $ - GDT64 - 1
    dq GDT64

SECTION .text
GLOBAL enter_kernel

; esp + 0x04 - Kernel info
; esp + 0x08 - Kernel pagetable
; esp + 0x0c - Kernel entry
enter_kernel:
    ; Load the page table
    mov eax, [esp + 0x08]
    mov cr3, eax

    ; Enable PAE
    mov eax, cr4
    or eax, 1 << 5
    mov cr4, eax

    ; Set the LM bit
    mov ecx, 0xC0000080
    rdmsr
    or eax, 1 << 8
    wrmsr

    ; Enable paging
    mov eax, cr0
    or eax, 1 << 31
    mov cr0, eax

    ; Load the 64 bits GDT
    lgdt [GDT64.Pointer]
    jmp 0x08:lmode

BITS 64
SECTION .text
lmode:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Load the kernel info
    mov edi, [esp + 0x04]

    ; Load the kernel entrypoint
    mov rax, [esp + 0x0c]

    ; Enter kernel land
    jmp rax

