BITS 32

SECTION .text
GLOBAL bios

EXTERN enter_mode_real
EXTERN enter_mode_protected

ALIGN 4

; [ebp + 0x08] --- INT number
; [rbp + 0x0C] --- Regs pointer
bios:
    push ebp
    mov ebp, esp
    pusha

    ; Enter real mode, at _bios_real_thunk
    push WORD _bios_real_thunk
    jmp enter_mode_real

_bios_return:
    popa
    xor eax, eax
    pop ebp
    ret



BITS 16
SECTION .text16
ALIGN 4
_int_jump_table:
    dw 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    dw _int10, 0, 0, _int13

;SECTION .text16
_int10:
    int 0x10
    jmp _bios_after_call

_int13:
    int 0x13
    jmp _bios_after_call

_bios_real_thunk:
    ; Load int handler
    mov     bx, [bp + 0x08]
    shl     bx, 1
    mov     ax, [_int_jump_table + bx]
    push    ax

    ; Load registers
    mov     bx, [bp + 0x0c]
    mov     ax, [bx + 0x00]
    mov     cx, [bx + 0x04]
    mov     dx, [bx + 0x06]
    mov     si, [bx + 0x08]
    mov     di, [bx + 0x0a]
    mov     bx, [bx + 0x02]

    ; Perform the bios call
    ret

_bios_after_call:
    ; The same but in reverse
    push    bx
    mov     bx, [bp + 0x0c]
    mov     [bx + 0x00], ax
    mov     [bx + 0x04], cx
    mov     [bx + 0x06], dx
    mov     [bx + 0x08], si
    mov     [bx + 0x0a], di
    pop     ax
    mov     [bx + 0x02], ax

    ; We're done, let's exit this madness
    push DWORD _bios_return
    jmp enter_mode_protected
