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
GLOBAL syscall_handler
GLOBAL syscall_return
EXTERN syscall_dispatch

; x86_64 syscall ABI:
;
; RAX - Syscall number
; RSI - arg0
; RDX - arg1
; R10 - arg2
; R8  - arg3
; R9  - arg4
; RDI - arg5

; The single entrypoint for syscalls
syscall_handler:
    ; We just entered kernel-land and have no kernel stack
    ; A little dance with swapgs will solve that

    ; First, we need to preserve the userland stack
    mov rbx, rsp

    ; Then, load the proper kernel stack
    swapgs
    mov rsp, [gs:0x08]
    swapgs

    ; Now we can alloc our SyscallReq
    sub rsp, 0x48

    ; Save RIP, RFLAGS and RSP
    mov [rsp + 0x00], rcx
    mov [rsp + 0x08], r11
    mov [rsp + 0x10], rbx

    ; Save the syscall args
    mov [rsp + 0x18], rsi
    mov [rsp + 0x20], rdx
    mov [rsp + 0x28], r10
    mov [rsp + 0x30], r8
    mov [rsp + 0x38], r9
    mov [rsp + 0x40], rdi

    ; Load the syscall number
    mov rdi, rax

    ; Load the syscall req
    mov rsi, rsp

    ; Dispatch the syscall
    jmp syscall_dispatch

; RDI: SyscallReq* req
; RSI: uint64_t value
syscall_return:
    ; Disable interrupts
    cli

    ; Load RIP and RFLAGS
    mov rcx, [rdi + 0x00]
    mov r11, [rdi + 0x08]

    ; Load the return value
    mov rax, rsi

    ; Load the userland stack
    mov rsp, [rdi + 0x10]

    ; Return to userland
    o64 sysret

