extern TransitionMain
global _start
_start:
    call TransitionMain
    mov byte [0xb8000], 'H'
jmp $