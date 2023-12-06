#include "mod.h"

// global var

static ProcessManager processManager;
static PIDAllocator pidAllocator;
static PCB* idleProcess;

// static methods declaration

static PCB* fetchProcess();
static void runFirstProcess();
static void idle();

// public methods

void InitializeProcessManager() {
    processManager.Current = NULL;
    processManager.RunnableProcesses = NewQueue("PCB*", MAX_PROCESS_COUNT);
    CreateKernelProcess(idle);
    idleProcess = fetchProcess();
}

PCB* GetCurrentProcess() {
    return processManager.Current;
}

void AddProcess(PCB* process) {
    processManager.RunnableProcesses->Push(processManager.RunnableProcesses, process);
}

PID AllocatePID() {
    for (i32 i = 0; i < MAX_PROCESS_COUNT / 32; i++) {
        for (i32 j = 0; j < 32; j++) {
            if ((pidAllocator.Bitmap[i] & (1 << j)) == 0) {
                pidAllocator.Bitmap[i] |= (1 << j);
                return i * 32 + j;
            }
        }
    }
}

void FreePID(PID pid) {
    pidAllocator.Bitmap[pid / 32] &= ~(1 << (pid % 32));
}

void Schedule() {
    // 如果就绪队列里没有进程，并且当前进程也不存在，说明还没加载任何进程，不执行Idle进程
    if (processManager.Current == NULL && processManager.RunnableProcesses->Empty(processManager.RunnableProcesses)) {
        return;
    }

    // 如果就绪队列里有进程，但是当前进程不存在，说明可以执行第一个进程
    if (processManager.Current == NULL) {
        runFirstProcess();
        return;
    }
    
    // 如果就绪队列里有进程，并且当前进程存在，说明可以执行切换
    PCB* current = processManager.Current;
    if (current->ID && current->Status != PROCESS_STATE_BLOCKED) {
        current->Status = PROCESS_STATE_RUNNABLE;
        AddProcess(current);
    }

    PCB* next = fetchProcess();
    next->Status = PROCESS_STATE_RUNNING;
    // 下一个进程为用户进程时，需要设置TSS的ESP0为内核栈栈顶，ceil(按页对齐)
    if (next->Type == PROCESS_TYPE_USER) {
        SetTSSEsp0(((u32)next->KernelStackPointer + PageSize - 1) / PageSize * PageSize);
    }

    // 下一个进程的页表和当前进程的页表不同，需要切换页表
    if (next->RootPPN != processManager.Current->RootPPN) {
        SetRootPageTableAddr(GetAddressFromPPN(next->RootPPN));
    }

    processManager.Current = next;
    SwitchProcess(current, next);
}

// static methods implement

static void runFirstProcess() {
    PCB* next = fetchProcess();
    next->Status = PROCESS_STATE_RUNNING;
    // 下一个进程为用户进程时，需要设置TSS的ESP0
    if (next->Type == PROCESS_TYPE_USER) {
        SetTSSEsp0(((u32)next->KernelStackPointer + PageSize - 1) / PageSize * PageSize);
    }
    SetRootPageTableAddr(GetAddressFromPPN(next->RootPPN));

    PCB unused;
    PCB* unusedPtr = &unused;
    processManager.Current = next;
    SwitchProcess(unusedPtr, next);
}

static PCB* fetchProcess() {
    PCB* process = processManager.RunnableProcesses->Pop(processManager.RunnableProcesses);
    return process == NULL? idleProcess: process;
}

static void idle() {
    while (TRUE) {
        Schedule();
    }
}