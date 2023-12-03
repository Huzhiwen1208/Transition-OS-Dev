[bits 32]
section .text

; void SwitchProcess(PCB* current, PCB* next);
global SwitchProcess
SwitchProcess:
    push ebp
    push ebx
    push esi
    push edi


    mov eax, [esp + 4*5]; currentKernelSP
    mov [eax], esp;  save new sp to current currentKernelSP

    mov eax, [esp + 4*6]; next nextKernelSP
    mov esp, [eax];  switch esp to nextKernelSP

    pop edi
    pop esi
    pop ebx
    pop ebp

    ret; pop eip, return eip