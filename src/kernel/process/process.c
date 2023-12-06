#include "mod.h"

extern void RestoreContext();

/// @brief 让栈指针指向Interrupt Context方便返回用户态
static void restore();

void CreateKernelProcess(void* entry) {
    PCB* process = (PCB*)Malloc(sizeof(PCB));
    process->ID = AllocatePID();
    process->Status = PROCESS_STATE_RUNNABLE;
    u32 stack = AllocateOnePage(KernelMode) + PageSize;
    process->Type = PROCESS_TYPE_KERNEL;
    process->RootPPN = GetPPNFromAddressFloor(GetRootPageTableAddr());

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
    // 构造用户专属页表，复制内核页表的前4MB等页号映射
    u32 userRootPPN = GetPPNFromAddressFloor(AllocateOnePage(KernelMode));
    process->RootPPN = userRootPPN;
    MemoryCopy(
        GetAddressFromPPN(userRootPPN),
        GetRootPageTableAddr(),
        PageSize
    );

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
    context->EIP = restore;
    context->EBP = 0;
    context->ESI = 0;
    context->EDI = 0;
    context->EBX = 0;
    process->KernelStackPointer = (PhysicalAddress*)stack;

    AddProcess(process);
}


static void restore() {
    PCB* current = GetCurrentProcess();
    u32 stack = ((u32)current->KernelStackPointer + PageSize - 1) / PageSize * PageSize;
    stack -= sizeof(InterruptContext);
    asm volatile ("movl %0, %%esp" : : "m"(stack));
    asm volatile ("jmp RestoreContext");
}