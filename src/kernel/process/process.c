#include "mod.h"

extern void RestoreContext();

/// @brief 让栈指针指向Interrupt Context方便返回用户态
static void restore();
static void copyPageTableRecursion(u32 dstRootPPN, u32 srcRootPPN);
static void freePageTableRecursion(u32 rootPPN);


// public methods

void CreateKernelProcess(void* entry) {
    PCB* process = (PCB*)Malloc(sizeof(PCB));
    process->ID = AllocatePID();
    process->ParentID = 0;
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
    process->ParentID = 0;
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

PID ForkProcess() {
    PCB* parent = GetCurrentProcess();
    Assert(parent->Type == PROCESS_TYPE_USER); // 目前我们只允许用户进程fork

    PCB* child = (PCB*)Malloc(sizeof(PCB));
    child->ID = AllocatePID();
    child->ParentID = parent->ID;
    child->Status = PROCESS_STATE_RUNNABLE;
    child->Type = parent->Type;
    // 构造子进程专属页表，递归完全复制父进程页表
    u32 childRootPPN = GetPPNFromAddressFloor(AllocateOnePage(KernelMode));
    child->RootPPN = childRootPPN;
    copyPageTableRecursion(childRootPPN, parent->RootPPN);

    // 构造子进程内核栈信息，复制父进程内核栈信息并修改返回值
    u32 stack = AllocateOnePage(KernelMode) + PageSize;
    MemoryCopy(
        stack - PageSize,
        GetAddressFromPPN(GetPPNFromAddressFloor(parent->KernelStackPointer)),
        PageSize
    );
    // 修改返回值
    stack -= sizeof(InterruptContext);
    InterruptContext* ctx = (InterruptContext*)stack;
    ctx->EAX = 0; // 子进程返回0

    stack -= sizeof(SwitchContext);
    SwitchContext* context = (SwitchContext*)stack;
    context->EIP = restore;
    context->EBP = 0;
    context->ESI = 0;
    context->EDI = 0;
    context->EBX = 0;
    child->KernelStackPointer = (PhysicalAddress*)stack;

    AddProcess(child);
    Schedule();
    return child->ID;
}

/*
    1. 释放进程的PID
    2. 释放进程的所有页表
    3. 释放进程的栈
    4. 将所有的子进程挂到当前进程的父进程下
*/
void ExitProcess(i32 exitCode) {
    PCB* current = GetCurrentProcess();
    current->Status = PROCESS_STATE_ZOMBIE;
    current->ExitCode = exitCode;
    FreePID(current->ID);
    freePageTableRecursion(current->RootPPN);
    FreeOnePage(GetAddressFromPPN(GetPPNFromAddressFloor(current->KernelStackPointer)));

    // 将所有的子进程挂到当前进程的父进程下
    RedirectParentOfChildren();

    Schedule();
}

PID WaitProcess(PID pid, i32* exitCode) {
    PCB* current = GetCurrentProcess();
    /*
        1. 如果遍历完所有Runnable进程和Zombie进程都没有找到，说明进程不存在，返回-1
        2. 如果在Runnable进程中找到，说明进程还没退出，返回-2即可，让上游来决定如何处理
        3. 如果在Zombie进程中找到，说明进程已经退出，返回退出码并回收进程的PCB
    */
    PCB* activatedChild = FindActivatedChildProcessByPID(pid);
    PCB* zombieChild = TakeZombieProcess(pid);

    if (activatedChild == NULL && zombieChild == NULL) {
        return -1;
    }

    if (zombieChild == NULL) {
        return -2;
    }

    // 存在僵尸子进程
    if (exitCode != NULL) {
        *exitCode = zombieChild->ExitCode;
    }

    PID rid = zombieChild->ID;
    Free(zombieChild);
    return rid;
}

PID GetProcessID() {
    return GetCurrentProcess()->ID;
}

PID GetProcessParentID() {
    return GetCurrentProcess()->ParentID;
}

// static methods implementation

static void restore() {
    PCB* current = GetCurrentProcess();
    u32 stack = ((u32)current->KernelStackPointer + PageSize - 1) / PageSize * PageSize;
    stack -= sizeof(InterruptContext);
    asm volatile ("movl %0, %%esp" : : "m"(stack));
    asm volatile ("jmp RestoreContext");
}

/*
    1. 此处可使用经典的copy on write技术，只需要修改页表项的权限+引用计数即可，后续有时间实现下
    2. 注意此处i不可从0开始，因为0号页表项是内核页表的等值映射，不可修改，否则在访问内核页表时会出现问题
*/
static void copyPageTableRecursion(u32 dstRootPPN, u32 srcRootPPN) {
    // 复制页目录
    MemoryCopy(
        GetAddressFromPPN(dstRootPPN),
        GetAddressFromPPN(srcRootPPN),
        PageSize
    );

    // 复制二级页表
    DisablePaging();
    PageTableEntry* srcPTE = (PageTableEntry*)GetAddressFromPPN(srcRootPPN);
    PageTableEntry* dstPTE = (PageTableEntry*)GetAddressFromPPN(dstRootPPN);
    
    for (Size i = 1; i < 1024; i++) {
        if (srcPTE[i].Present == 0) continue; // 不存在的页表项不复制

        u32 srcPPN = srcPTE[i].NextPPN;
        u32 dstPPN = GetPPNFromAddressFloor(AllocateOnePage(UserMode));
        dstPTE[i].NextPPN = dstPPN;
        MemoryCopy(
            GetAddressFromPPN(dstPPN),
            GetAddressFromPPN(srcPPN),
            PageSize
        );

        // 复制页帧
        PageTableEntry* secondSrcPageTablePTE = (PageTableEntry*)GetAddressFromPPN(srcPPN);
        PageTableEntry* secondDstPageTablePTE = (PageTableEntry*)GetAddressFromPPN(dstPPN);
        for (Size j = 0; j < 1024; j++) {
            if (secondSrcPageTablePTE[j].Present == 0) continue; // 不存在的页帧不复制

            u32 srcPFN = secondSrcPageTablePTE[j].NextPPN;
            u32 dstPFN = GetPPNFromAddressFloor(AllocateOnePage(UserMode));
            secondDstPageTablePTE[j].NextPPN = dstPFN;
            MemoryCopy(
                GetAddressFromPPN(dstPFN),
                GetAddressFromPPN(srcPFN),
                PageSize
            );
        }
    }
    EnablePaging();
}

static void freePageTableRecursion(u32 rootPPN) {
    DisablePaging();
    PageTableEntry* rootPTE = (PageTableEntry*)GetAddressFromPPN(rootPPN);
    for (Size i = 1; i < 1024; i++) {
        if (rootPTE[i].Present == 0) continue; // 不存在的页表项不释放

        u32 secondPPN = rootPTE[i].NextPPN;
        PageTableEntry* secondPTE = (PageTableEntry*)GetAddressFromPPN(secondPPN);
        for (Size j = 0; j < 1024; j++) {
            if (secondPTE[j].Present == 0) continue; // 不存在的页帧不释放

            u32 thirdPPN = secondPTE[j].NextPPN;
            FreeOnePage(GetAddressFromPPN(thirdPPN));
        }
        FreeOnePage(GetAddressFromPPN(secondPPN));
    }
    FreeOnePage(GetAddressFromPPN(rootPPN));
    EnablePaging();
}