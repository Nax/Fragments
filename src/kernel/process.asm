BITS 64
SECTION .text
GLOBAl process_return

; RDI: CPU state ptr
process_return:
    ; Allocate the 5 64bit fields for IRETQ
    sub rsp, 0x28

    ; RIP
    mov rax, [rdi + 0x80]
    mov [rsp + 0x00], rax

    ; CS
    mov QWORD [rsp + 0x08], 0x1b

    ; RFLAGS
    mov rax, [rdi + 0x88]
    mov [rsp + 0x10], rax

    ; RSP
    mov rax, [rdi + 0x30]
    mov [rsp + 0x18], rax

    ; SS
    mov eax, 0x23
    mov QWORD [rsp + 0x20], rax

    ; Now we need to load the whole CPU state
    mov rax, [rdi + 0x00]
    mov rbx, [rdi + 0x08]
    mov rcx, [rdi + 0x10]
    mov rdx, [rdi + 0x18]
    mov rsi, [rdi + 0x20]
    mov rbp, [rdi + 0x38]
    mov r8,  [rdi + 0x40]
    mov r9,  [rdi + 0x48]
    mov r10, [rdi + 0x50]
    mov r11, [rdi + 0x58]
    mov r12, [rdi + 0x60]
    mov r13, [rdi + 0x68]
    mov r14, [rdi + 0x70]
    mov r15, [rdi + 0x78]

    ; Restore the x87 / MMX / SSE state
    fxrstor64 [rdi + 0x80]

    ; We need to reload RDI last
    ; Since it's the pointer itself
    mov rdi, [rdi + 0x28]

    xchg bx, bx

    ; Here we go
    iretq
