extern TransitionMain
extern MemoryCheckout
global _start
_start:
    call MemoryCheckout
    call TransitionMain
jmp $