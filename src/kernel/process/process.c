#include "mod.h"

extern void RestoreContext();

void CreateKernelProcess(void* entry) {
    PCB* process = (PCB*)Malloc(sizeof(PCB));
    process->ID = AllocatePID();
    process->Status = PROCESS_STATE_RUNNABLE;
    u32 stack = AllocateOnePage(KernelMode) + PageSize;
    process->Type = PROCESS_TYPE_KERNEL;

    stack -= sizeof(SwitchContext);
    SwitchContext* context = (SwitchContext*)stack;
    context->EIP = (u32)entry;
    context->EBP = 0;
    context->ESI = 0;
    context->EDI = 0;
    context->EBX = 0;
    process->KernelStackPointer = (PhysicalAddress*)stack;

    AddProcess(process);
}

void CreateUserProcess(void* entry) {
    PCB* process = (PCB*)Malloc(sizeof(PCB));
    process->ID = AllocatePID();
    process->Status = PROCESS_STATE_RUNNABLE;
    u32 stack = AllocateOnePage(KernelMode) + PageSize;
    process->Type = PROCESS_TYPE_USER;

    stack -= sizeof(InterruptContext);
    InterruptContext* trapContext = (InterruptContext*)stack;
    trapContext->Vector = 0x80;
    trapContext->ErrCode = 0x88888888;
    trapContext->EDI = 0;
    trapContext->ESI = 0;
    trapContext->EBP = 0;
    trapContext->ESP = 0;
    trapContext->EBX = 0;
    trapContext->EDX = 0;
    trapContext->ECX = 0;
    trapContext->EAX = 0;
    trapContext->GS = UserDataSegmentSelector;
    trapContext->FS = UserDataSegmentSelector;
    trapContext->ES = UserDataSegmentSelector;
    trapContext->DS = UserDataSegmentSelector;
    trapContext->EIP = (u32)entry;
    trapContext->CS = UserCodeSegmentSelector;
    trapContext->PSW = 0x202; // 0000 0010 0000 0010
    trapContext->SS3 = UserDataSegmentSelector;
    trapContext->ESP3 = UserStackTop;

    stack -= sizeof(SwitchContext);
    SwitchContext* context = (SwitchContext*)stack;
    context->EIP = RestoreContext;
    context->EBP = 0;
    context->ESI = 0;
    context->EDI = 0;
    context->EBX = 0;
    process->KernelStackPointer = (PhysicalAddress*)stack;

    AddProcess(process);
}