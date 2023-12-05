[bits 32]

extern TrapHandler
section .text
global AllTrapsEntry
global RestoreContext
AllTrapsEntry:
    ; entry trap handler by "int 0x80"
    push 0x88888888
    push ds
    push es
    push fs
    push gs
    pushad
    push 0x80

    push edx; arg3
    push ecx; arg2
    push ebx; arg1
    push eax; syscall number
    call TrapHandler
    add esp, 4*4

    mov dword [esp + 8*4], eax ; save eax => stack, return value

RestoreContext:
    add esp, 4; pop vector
    ; restore context
    popad
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 4; pop 0x88888888/error code

    iret