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

