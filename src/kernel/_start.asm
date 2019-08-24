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

BITS 64
SECTION .text
GLOBAL _start
EXTERN kmain

ALIGN 8

enable_SSE:
    ; Enable SSE
    mov rax, cr0
    and rax, 0xfffffffffffffffb
    or rax, 0x2
    mov cr0, rax
    mov rax, cr4
    or rax, 3 << 9
    mov cr4, rax

    ret

; RSI: Kernel info ptr
_start:
    ; Initialize the stack
    mov rsp, kernel_stack_top
    mov rbp, rsp

    ; Save the kernel info pointer
    push rdi

    ; Enable SSE if available
    call enable_SSE

    ; Enable AVX if available
    ; call enable_AVX

    ; Restore the kernel info ptr and enter kmain()
    pop rdi
    jmp kmain

SECTION .bss
ALIGN 16
GLOBAL kernel_stack_top
kernel_stack_bottom: resb 0x4000
kernel_stack_top:
