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
    push ebx
    push esi
    push edi

    ; Enter real mode, at _bios_real_thunk
    push WORD _bios_real_thunk
    jmp enter_mode_real

_bios_return:
    ; Pushed by the thunk
    pop eax

    ; restore registers
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret

BITS 16
SECTION .text16
ALIGN 4
_bios_real_thunk:
    ; Load the int number
    mov     ax, [bp + 0x08]
    mov     [.int_number], al

    ; Load registers
    mov     bx, [bp + 0x0c]
    mov     eax, [bx + 0x00]
    mov     ecx, [bx + 0x08]
    mov     edx, [bx + 0x0c]
    mov     esi, [bx + 0x10]
    mov     edi, [bx + 0x14]
    mov     ebx, [bx + 0x04]

    ; Perform the bios call
    ; 0xcd 0xII is the encoding for int II
    db 0xcd
.int_number:
    db 0x00

    jc .failure
    push DWORD 0
    jmp .return

.failure:
    push DWORD 1

.return:
    ; The same but in reverse
    push    ebx
    mov     bx, [bp + 0x0c]
    mov     [bx + 0x00], eax
    mov     [bx + 0x08], ecx
    mov     [bx + 0x0c], edx
    mov     [bx + 0x10], esi
    mov     [bx + 0x14], edi
    pop     eax
    mov     [bx + 0x04], eax

    ; We're done, let's exit this madness
    push DWORD _bios_return
    jmp enter_mode_protected
