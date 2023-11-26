[org 0x7c00]
; 要执行的代码, MBR: Main Boot R

mov edi, 0x500; memory address to load disk content
mov ecx, 1; seek disk to the second sector
mov bl, 3; read 10 sectors
call read_disk

jmp 0:0x500

jmp $; while(1); $是当前行，$$是当前磁盘位置

mbgsdl:
    db "mian bao ge shi da lao!"
mbgsdl_end:

read_disk:
    ; read disk to memory
    ; edi - memory address to load disk content
    ; ecx - specified start sector number for reading
    ; bl -  sector count to read
    pushad; eax, ecx, edx, ebx, esp, ebp, esi, edi
    ; pushad; eax, ecx, edx, ebx, esp, ebp, esi, edi
    ; pusha 16 bits registers: ax, cx, dx, bx, sp, bp, si, di

    mov dx, 0x1f2
    mov al, bl
    out dx, al; set sector number

    ; scatter ecx to【4,4,8,8,8】
    mov al, cl
    inc dx; 0x1f3
    out dx, al; low 8 bits of start sector

    shr ecx, 8
    mov al, cl
    inc dx; 0x1f4
    out dx, al; middle 8 bits of start sector

    shr ecx, 8
    mov al, cl
    inc dx; 0x1f5
    out dx, al; high 8 bits of start sector

    shr ecx, 8
    and cl, 0b1111

    inc dx; 0x1f6
    mov al, 0b1110_0000
    or al, cl
    out dx, al

    inc dx; 0x1f7
    mov al, 0x20; set read mode
    out dx, al

    xor ecx, ecx
    mov cl, bl

.read:
    push cx
    call .waits
    call .reads
    pop cx 
    loop .read

    popad
    ; popa

    ret

.waits:
    mov dx, 0x1f7
    .check:
        nop
        nop
        nop ; a little delay
        in al, dx
        and al, 0b1000_1000
        cmp al, 0b0000_1000
        jnz .check
    ret

.reads:
    mov dx, 0x1f0
    mov cx, 256; 256 words per sector
    .readw:
        nop
        nop
        nop
        in ax, dx
        mov [edi], ax
        add edi, 2
        loop .readw
    ret
times 510 - ($ - $$) db 0
db 0x55, 0xaa