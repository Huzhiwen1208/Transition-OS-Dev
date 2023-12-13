[bits 32]

section .text

; u8 ReadByte(u16 port);
global ReadByte
ReadByte:
    push edx
    push ebp
    mov ebp, esp

    xor eax, eax; eax = 0
    mov edx, [ebp + 4*3]; edx = port
    in al, dx; al = result
    nop
    nop
    nop

    leave
    pop edx
    ret

; void WriteByte(u16 port, u8 value);
global WriteByte
WriteByte:
    push edx
    push ebp
    mov ebp, esp

    mov eax, [ebp + 4*4]; eax = value
    mov edx, [ebp + 4*3]; edx = port
    out dx, al
    nop
    nop
    nop

    leave
    pop edx
    ret

; u16 ReadWord(u16 port);
global ReadWord
ReadWord:
    push edx
    push ebp
    mov ebp, esp

    xor eax, eax; eax = 0
    mov edx, [ebp + 4*3]; edx = port
    in ax, dx; ax = result
    nop
    nop
    nop

    leave
    pop edx
    ret

; void WriteWord(u16 port, u16 value);
global WriteWord
WriteWord:
    push edx
    push ebp
    mov ebp, esp

    mov eax, [ebp + 4*4]; eax = value
    mov edx, [ebp + 4*3]; edx = port
    out dx, ax
    nop
    nop
    nop

    leave
    pop edx
    ret