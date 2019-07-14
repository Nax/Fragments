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
