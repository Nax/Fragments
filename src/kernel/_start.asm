BITS 64
SECTION .text
GLOBAL _start
EXTERN kmain

ALIGN 8

enable_SSE:
    ; Check for SSE support
    mov eax, 0x1
    cpuid
    test edx, 1<<25
    jz .end

    ; Enable SSE
    mov rax, cr0
    and rax, 0xfffffffffffffffb
    or rax, 0x2
    mov cr0, rax
    mov rax, cr4
    or rax, 3 << 9
    mov cr4, rax

.end:
    ret

enable_AVX:
    ; TODO: Check for AVX support

    ; Enable OSXSAVE
    mov rax, cr4
    or rax, 0x00040000
    mov cr4, rax

    ; Enable AVX
    xor ecx, ecx
    xgetbv
    or eax, 7
    xsetbv

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
kernel_stack_bottom: resb 0x1000
kernel_stack_top:
