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
    processManager.ZombieProcesses = NewQueue("PCB*", MAX_PROCESS_COUNT);
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
    
    PCB* current = processManager.Current;
    /*
        1. 当前进程是IDLE，切换时不需要进等待队列
        2. 当前进程是ZOMBIE，需要挂到ZOMBIE队列
        3. 当前进程是BLOCKED，需要挂到阻塞队列
        4. 当前进程是RUNNABLE，不可能
        5. 当前进程是RUNNING，需要挂到等待队列
    */
    if (current->ID == 0) {
        // IDLE进程，什么也不做
        Debug("IDLE process scheduled");
    } else {
        switch (current->Status) {
            case PROCESS_STATE_ZOMBIE:
                processManager.ZombieProcesses->Push(processManager.ZombieProcesses, current);
                break;
            case PROCESS_STATE_BLOCKED:
                // 阻塞队列
                break;
            case PROCESS_STATE_RUNNABLE:
                Panic("Process is runnable???");
            case PROCESS_STATE_RUNNING:
                current->Status = PROCESS_STATE_RUNNABLE;
                AddProcess(current);
                break;
            default:
                Panic("Unknown process state: %d", current->Status);
        }
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

// 将当前进程的所有子进程的父进程设置为当前进程的父进程（包括Ready、Zombie、Block）
void RedirectParentOfChildren() {
    PCB* current = processManager.Current;
    Queue* queue = processManager.RunnableProcesses;
    for (i32 i = queue->Front; i < queue->Rear; i = (i + 1) % queue->MallocSize) {
        PCB* process = (PCB*)queue->Value[i];
        if (process && process->ParentID == current->ID) {
            process->ParentID = current->ParentID;
        }
    }

    queue = processManager.ZombieProcesses;
    for (i32 i = queue->Front; i < queue->Rear; i = (i + 1) % queue->MallocSize) {
        PCB* process = (PCB*)queue->Value[i];
        if (process && process->ParentID == current->ID) {
            process->ParentID = current->ParentID;
        }
    }
}

PCB* FindActivatedChildProcessByPID(PID pid) {
    PCB* current = processManager.Current;
    Queue* queue = processManager.RunnableProcesses;
    for (i32 i = queue->Front; i < queue->Rear; i = (i + 1) % queue->MallocSize) {
        PCB* process = (PCB*)queue->Value[i];
        if (process && process->ParentID == current->ID && (process->ID == pid || pid == -1)) {
            return process;
        }
    }
    return NULL;
}

PCB* TakeZombieProcess(PID pid) {
    PCB* current = processManager.Current;
    Queue* queue = processManager.ZombieProcesses;
    for (i32 i = queue->Front; i < queue->Rear; i = (i + 1) % queue->MallocSize) {
        PCB* process = (PCB*)queue->Value[i];
        if (process && process->ParentID == current->ID && (process->ID == pid || pid == -1)) {
            void* value = queue->Value[i];
            queue->Value[i] = queue->Value[queue->Front];
            queue->Value[queue->Front] = value;
            return (PCB*)(queue->Pop(queue));
        }
    }

    return NULL;
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