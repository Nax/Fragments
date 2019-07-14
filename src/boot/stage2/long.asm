;
; Copyright (c) 2019, Maxime Bacoux
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions are met:
;
; 1. Redistributions of source code must retain the above copyright notice, this
;    list of conditions and the following disclaimer.
; 2. Redistributions in binary form must reproduce the above copyright notice,
;    this list of conditions and the following disclaimer in the documentation
;    and/or other materials provided with the distribution.
;
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
; WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
; DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
; ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
; (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
; ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
; (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
; SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
;

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

