#include "mod.h"

extern void __switch(PhysicalAddress* currentKernelSP, PhysicalAddress* nextKernelSP);
static ProcessManager processManager;
static PIDAllocator pidAllocator;
static PCB* idleProcess;

static PCB* fetchProcess();
void runFirstProcess();
static void idle();


void InitProcessManager() {
    processManager.Current = NULL;
    processManager.RunnableProcesses = NewQueue("PCB*", MAX_PROCESS_COUNT);
    CreateKernelProcess(idle);
    idleProcess = fetchProcess();
    Schedule();
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
    if (processManager.Current == NULL) {
        runFirstProcess();
        return;
    }

    PCB* current = processManager.Current;
    if (current->ID && current->Status != PROCESS_STATE_BLOCKED) {
        current->Status = PROCESS_STATE_RUNNABLE;
        AddProcess(current);
    }

    PCB* next = fetchProcess();
    next->Status = PROCESS_STATE_RUNNING;

    processManager.Current = next;
    __switch(current->KernelStackPointer, next->KernelStackPointer);
}

void runFirstProcess() {
    PCB* next = fetchProcess();
    next->Status = PROCESS_STATE_RUNNING;

    PCB unused;
    processManager.Current = next;
    __switch(unused.KernelStackPointer, next->KernelStackPointer);
}

static PCB* fetchProcess() {
    PCB* process = processManager.RunnableProcesses->Pop(processManager.RunnableProcesses);
    return process == NULL? idleProcess: process;
}

static void idle() {
    while (TRUE) {
        asm volatile ("sti");
        asm volatile ("hlt");
        Schedule();
    }
}