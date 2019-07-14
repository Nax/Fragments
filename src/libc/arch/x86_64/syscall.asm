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

BITS 64
SECTION .text
GLOBAL $syscall

; The x86_64 syscall interface
;
; On kernel entry, the arguments are as follow:
; RAX - Syscall number
; RSI - arg0
; RDX - arg1
; R10 - arg2
; R8  - arg3
; R9  - arg4
; RDI - arg5
; RCX - RIP (automatic)
; R11 - RFLAGS (automatic)
$syscall:
    ; Save callee regs
    ; We use the red zone for that
    mov [rsp - 0x08], rbx
    mov [rsp - 0x10], rbp
    mov [rsp - 0x18], r12
    mov [rsp - 0x20], r13
    mov [rsp - 0x28], r14
    mov [rsp - 0x30], r15

    ; Load the syscall number
    mov rax, rdi

    ; Free rcx for the RIP
    mov r10, rcx

    ; RDI is the last argument, it comes from the stack
    ; RSP + 0x00 is the return addr,
    ; so it's in RSP + 0x08
    mov rdi, [rsp + 0x08]

    ; Let's make the jump
    syscall

    ; Now the return value is in RAX, just like with regular functions
    ; We just have to reload callee-saved registers
    mov rbx, [rsp - 0x08]
    mov rbp, [rsp - 0x10]
    mov r12, [rsp - 0x18]
    mov r13, [rsp - 0x20]
    mov r14, [rsp - 0x28]
    mov r15, [rsp - 0x30]

    ret
