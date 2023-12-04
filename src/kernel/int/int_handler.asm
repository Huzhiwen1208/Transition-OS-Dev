[bits 32]
section .text

ErrCodeMagic equ 0x88888888

extern InterruptHandlerList
%macro InterruptHandlerMacro 2
InterruptHandler_%1:
%ifn %2
    push ErrCodeMagic
%endif
    ; save context 
    push ds
    push es
    push fs
    push gs
    pushad

    push %1
    call [InterruptHandlerList + %1 * 4]
    add esp, 4; pop %1

    ; restore context
    popad
    pop gs
    pop fs
    pop es
    pop ds
    add esp, 4; pop 0x88888888/error code

    iret
%endmacro

InterruptHandlerMacro 0x00, 0; 0x00 #DE Divide Error
InterruptHandlerMacro 0x01, 0; 0x01 #DB Debug Exception
InterruptHandlerMacro 0x02, 0; 0x02 NMI Interrupt, non maskable interrupt
InterruptHandlerMacro 0x03, 0; 0x03 #BP Breakpoint
InterruptHandlerMacro 0x04, 0; 0x04 #OF Overflow
InterruptHandlerMacro 0x05, 0; 0x05 #BR BOUND Range Exceeded
InterruptHandlerMacro 0x06, 0; 0x06 #UD Invalid Opcode (Undefined Opcode)
InterruptHandlerMacro 0x07, 0; 0x07 #NM Device Not Available (No Math Coprocessor)
InterruptHandlerMacro 0x08, 1; 0x08 #DF Double Fault
InterruptHandlerMacro 0x09, 0; 0x09 Coprocessor Segment Overrun (reserved)
InterruptHandlerMacro 0x0a, 1; 0x0A #TS Invalid TSS
InterruptHandlerMacro 0x0b, 1; 0x0B #NP Segment Not Present
InterruptHandlerMacro 0x0c, 1; 0x0C #SS Stack-Segment Fault
InterruptHandlerMacro 0x0d, 1; 0x0D #GP General Protection
InterruptHandlerMacro 0x0e, 1; 0x0E #PF Page Fault
InterruptHandlerMacro 0x0f, 0; 0x0F reserved
InterruptHandlerMacro 0x10, 0; 0x10 #MF x87 FPU Floating-Point Error (Math Fault)
InterruptHandlerMacro 0x11, 1; 0x11 #AC Alignment Check
InterruptHandlerMacro 0x12, 0; 0x12 #MC Machine Check
InterruptHandlerMacro 0x13, 0; 0x13 #XF SIMD Floating-Point Exception

InterruptHandlerMacro 0x14, 0; Virtualization Exception
InterruptHandlerMacro 0x15, 1; Control Protection Exception
InterruptHandlerMacro 0x16, 0; reserved
InterruptHandlerMacro 0x17, 0; reserved

InterruptHandlerMacro 0x18, 0; reserved
InterruptHandlerMacro 0x19, 0; reserved
InterruptHandlerMacro 0x1a, 0; reserved
InterruptHandlerMacro 0x1b, 0; reserved

InterruptHandlerMacro 0x1c, 0; reserved
InterruptHandlerMacro 0x1d, 0; reserved
InterruptHandlerMacro 0x1e, 0; reserved
InterruptHandlerMacro 0x1f, 0; reserved

; 外中断
InterruptHandlerMacro 0x20, 0; clock interrupt
InterruptHandlerMacro 0x21, 0; keyboard interrupt
InterruptHandlerMacro 0x22, 0; cascade
InterruptHandlerMacro 0x23, 0; com2
InterruptHandlerMacro 0x24, 0; com1
InterruptHandlerMacro 0x25, 0; sb16
InterruptHandlerMacro 0x26, 0; floppy
InterruptHandlerMacro 0x27, 0; 并行口1
InterruptHandlerMacro 0x28, 0; realtime clock
InterruptHandlerMacro 0x29, 0; INTOAH
InterruptHandlerMacro 0x2a, 0
InterruptHandlerMacro 0x2b, 0; nic
InterruptHandlerMacro 0x2c, 0
InterruptHandlerMacro 0x2d, 0
InterruptHandlerMacro 0x2e, 0; harddisk1
InterruptHandlerMacro 0x2f, 0; harddisk2

section .data
global InterruptHandlerEntryTable
InterruptHandlerEntryTable:
    dd InterruptHandler_0x00
    dd InterruptHandler_0x01
    dd InterruptHandler_0x02
    dd InterruptHandler_0x03
    dd InterruptHandler_0x04
    dd InterruptHandler_0x05
    dd InterruptHandler_0x06
    dd InterruptHandler_0x07
    dd InterruptHandler_0x08
    dd InterruptHandler_0x09
    dd InterruptHandler_0x0a
    dd InterruptHandler_0x0b
    dd InterruptHandler_0x0c
    dd InterruptHandler_0x0d
    dd InterruptHandler_0x0e
    dd InterruptHandler_0x0f
    dd InterruptHandler_0x10
    dd InterruptHandler_0x11
    dd InterruptHandler_0x12
    dd InterruptHandler_0x13
    dd InterruptHandler_0x14
    dd InterruptHandler_0x15
    dd InterruptHandler_0x16
    dd InterruptHandler_0x17
    dd InterruptHandler_0x18
    dd InterruptHandler_0x19
    dd InterruptHandler_0x1a
    dd InterruptHandler_0x1b
    dd InterruptHandler_0x1c
    dd InterruptHandler_0x1d
    dd InterruptHandler_0x1e
    dd InterruptHandler_0x1f

    dd InterruptHandler_0x20
    dd InterruptHandler_0x21
    dd InterruptHandler_0x22
    dd InterruptHandler_0x23
    dd InterruptHandler_0x24
    dd InterruptHandler_0x25
    dd InterruptHandler_0x26
    dd InterruptHandler_0x27
    dd InterruptHandler_0x28
    dd InterruptHandler_0x29
    dd InterruptHandler_0x2a
    dd InterruptHandler_0x2b
    dd InterruptHandler_0x2c
    dd InterruptHandler_0x2d
    dd InterruptHandler_0x2e
    dd InterruptHandler_0x2f