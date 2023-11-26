
[org 0x500]
GPU equ 0xb800; 文本显示器内存段

mov ax, 3
int 0x10

mov ax, GPU
mov es, ax; es->GPU, extra segment

mov si, init_message
call print

; memory checkout
call memory_checkout

; protect mode: use global descriptor table
call protect_mode

jmp $

mbgsdl:
    db "mian bao ge shi da lao!"
mbgsdl_end:

print:
    push ax
    mov ah, 0xe
    .write:
        mov al, [si]
        cmp al, 0
        jz .done
        int 0x10
        inc si
        jmp .write
    .done:
        pop ax
        ret

; ARDS: Address Range Descriptor Structure
; struct {
;       BaseAddrLow   low 32 bits of base address of memory
;       BaseAddrHigh  high 32 bits of base address of memory
;       LengthLow     low 32 bits of memory length
;       LengthHigh    high 32 bits of memory length
;       Type          type of the section(1: available, 2: unavailable)
;}
memory_checkout:
    ; int 0x15 args:
    ;   eax: function number, 0xe820
    ;   ebx: which ards should be returned, 0x0 first, auto change to next by bios
    ;   es: segment base address
    ;   edi: buffer address that store ards, perhapse size = 0
    ;   ecx: sizeof ards, 0x14 bytes
    ;   edx: signature, 0x534d4150
    ;
    ; int 0x15 return:
    ;   cf: 0x0 success, 0x1 failed
    ;   eax: 0x534d4150
    ;   es: segment base address
    ;   edi: buffer address that store ards, should contain some ards
    ;   ecx: sizeof ards, 0x14 bytes
    ;   ebx: which ards should be returned, 0x0 first, auto change to next by bios, should not be care
    ;           ebx = 0 refers to all ards has been loaded
    mov ax, 0
    mov es, ax
    mov di, ards
    mov ecx, 0x14
    mov edx, 0x534d4150
    xor ebx, ebx

    .check:
        mov eax, 0xe820
        int 0x15

        jc memory_checkout_failed
        xor eax, 0x534d4150
        cmp eax, 0
        jnz memory_checkout_failed

        add di, 0x14
        add dword [ards_cnt], 1
        cmp ebx, 0
        jnz .check

    mov si, memory_checkout_success_message
    call print
    ret

memory_checkout_failed:
    mov si, memory_checkout_failed_message
    call print
    jmp $

protect_mode:
    cli ; close interrupt

    in al, 0x92
    or al, 0b0000_0010
    out 0x92, al ; enable A20

    lgdt [gdt_pointer]

    mov eax, cr0
    or eax, 0b0000_0001
    mov cr0, eax ; enable protect mode
    
    jmp dword code_segment_selector:entry_protect_mode ; jump to protect mode
        
[bits 32]
entry_protect_mode:
    mov ax, data_segment_selector
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x7000 ; stack top
    
    mov edi, 0x7e00; memory address to load disk content
    mov ecx, 4; seek disk to 12th sector
    mov bl, 200; read 200 sectors
    call read_disk
    push ards_cnt
    jmp dword code_segment_selector:0x7e00 ; jump to kernel
    ud2

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

; messages
init_message:
    db "MianbaoGe wansui!!", 10, 13, 0  ; 10: /n 13: /r
memory_checkout_message:
    db "Memory checkout...", 10, 13, 0
memory_checkout_failed_message:
    db "Memory checkout failed!", 10, 13, 0
memory_checkout_success_message:
    db "Memory checkout success!", 10, 13, 0
entry_protect_mode_message:
    db "Entry protect mode~~~", 10, 13, 0

; strcuct segment_selector {
;   RPL: 2          request privilege level
;   TI: 1           table indicator, 0: GDT, 1: LDT
;   index: 13       index of segment in GDT or LDT
;}
code_segment_selector equ 0b0000000000001000
data_segment_selector equ 0b0000000000010000

; struct pointer{
;   limit: 16   byte size of gdt - 1
;   base: 32    gdt base address
;}
gdt_pointer:
    dw global_descriptor_table_end - global_descriptor_table - 1
    dd global_descriptor_table

; struct descriptor {
;   limit_low: 16     low 16 bits of segment limit
;   base_low: 24      low 24 bits of memory base address
;   type: 4           type of the segment
;                        E C/D R/W A    1CRA, 0DWA refer to code segment, data segment, respectively
;                         C: conforming segment(user accessable?), D: direction bit, R: readable, W: writable, A: accessed
;   segment:1         1: code/data, 0: system segment
;   DPL:2             descriptor privilege level, 0: kernel, 3: user
;   present:1         1: present in memory, 0: not present in memory
;   limit_high:4      high 4 bits of segment limit  16-19
;   available:1       available for system use
;   long_mode:1       1: 64-bit mode, 0: 32-bit mode
;   big:1             1: 32-bit mode, 0: 16-bit mode
;   granularity:1     1: limit is in 4kb per block, 0: limit is in byte per block
;   base_high:8       high 8 bits of memory base address 24-31
;}
; global descriptor table: descriptor[]{null, code, data}
global_descriptor_table:
    dd 0
    dd 0 ; the first descriptor should be null
code_segment:
    ; mem base: 0
    ; segment size: 2^20 * granularity(4kb) = 4GB
    dw 0xffff
    dw 0
    db 0
    db 0b1001_1010; type= 1CRA(1110), segment=1, DPL=00, present=1
    db 0xdf       ; granularity=1, big=1, long_mode=0, available=1, limit_high=1111
    db 0
data_segment:
    ; mem base: 0
    ; segment size: 2^20 * granularity(4kb) = 4GB
    dw 0xffff
    dw 0
    db 0
    db 0b1001_0010; type= 0DWA(1010), segment=1, DPL=00, present=1
    db 0xdf       ; granularity=1, big=1, long_mode=0, available=1, limit_high=1111
    db 0
global_descriptor_table_end:

ards_cnt:
    dd 0
ards: