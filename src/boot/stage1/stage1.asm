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

BITS 16
SECTION .text

_start:
    ; Disable interrupts
    cli

    ; Set the stack at 0x600
    mov ax, 0x600
    mov sp, ax
    mov bp, ax

    ; Set the segments to zero
    xor ax, ax
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; Relocate
    mov cx, 0x100
    mov si, 0x7c00
    mov di, 0x600
    rep movsw

    ; Save dl
    mov [saved_dl], dl

    ; Jump to the correct address
    jmp 0x00:load_stage1

ALIGN 4
DAPACK_STAGE1:
    db  0x10
    db  0
    dw  39
    dw  0x800
    dw  0
    dd  1
    dd  0

saved_dl:
    db  0

load_stage1:
    ; Load stage1
    mov si, DAPACK_STAGE1
    mov ah, 0x42
    int 0x13
    jmp 0x00:stage1

times (0x1b4 - ($-$$)) db 0

MBR:
.ID:
    times 10 db 0
.Partitions:
    times 64 db 0
.Signature:
    dw 0xaa55

; Start of sector 2

ALIGN 4
DAPACK_STAGE2:
    db  0x10
    db  0
    dw  40
    dw  0x7c00
    dw  0
.lba:
    dd  0
    dd  0

msg: db 'No Operating System found.', 0

stage1:
    ; Clear screen
    mov ax, 0x0003
    int 0x10

    ; Hide cursor
    mov ah, 0x01
    mov ch, 0x3f
    int 0x10

    ; Try to find an active partition
    mov si, MBR.Partitions
.loop:
    call try_boot
    add si, 0x10
    cmp si, MBR.Partitions + 0x40
    jne .loop

    mov si, msg
    call print

.halt:
    ; Halt forever
    hlt
    jmp .halt

; si - MBR entry pointer
try_boot:
    ; Check for a bootable partition
    mov al, [si]
    test al, 0x80
    jz .noboot

    ; We found a partition with the active flag
    push si
    add si, 0x08
    mov ax, [si]
    mov [DAPACK_STAGE2.lba], ax
    add si, 2
    mov ax, [si]
    mov [DAPACK_STAGE2.lba + 2], ax

    ; Load the VBR
    mov si, DAPACK_STAGE2
    mov ah, 0x42
    int 0x13

    ; Restore the MBR pointer
    pop si

    ; Reload dl
    mov dl, [saved_dl]

    ; Jump to stage2
    jmp 0x0:0x7c00

.noboot:
    ret

print:
    pusha

    xor dx, dx
    mov bx, 0x0007
    mov cx, 1

.loop:
    mov ah, 0x02
    int 0x10
    inc dx

    mov al, [si]
    test al, al
    jz .loop_end
    mov ah, 0x09
    int 0x10
    inc si
    jmp .loop

.loop_end:
    popa
    ret
