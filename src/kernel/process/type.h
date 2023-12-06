#pragma once

#include "../common/mod.h"
#include "../ds/type.h"

// Process
typedef enum ProcessState {
    PROCESS_STATE_RUNNABLE,
    PROCESS_STATE_RUNNING,
    PROCESS_STATE_BLOCKED
} ProcessState;

typedef enum ProcessType {
    PROCESS_TYPE_USER,
    PROCESS_TYPE_KERNEL
} ProcessType;

typedef struct PCB {
    PhysicalAddress* KernelStackPointer;
    PID ID;
    ProcessState Status;
    ProcessType Type;
    u32 RootPPN; 
    // u32 Stride;
    // u32 Pass;
    // u32 Priority;
    // u32 OverflowTimes; // stride溢出次数
} PCB;

// Context
typedef struct SwitchContext {
    u32 EDI;
    u32 ESI;
    u32 EBX;
    u32 EBP;
    u32 EIP;
} SwitchContext;

typedef struct InterruptContext {
    u32 Vector;
    u32 EDI;
    u32 ESI;
    u32 EBP;
    u32 ESP;
    u32 EBX;
    u32 EDX;
    u32 ECX;
    u32 EAX;
    u32 GS;
    u32 FS;
    u32 ES;
    u32 DS;
    u32 ErrCode;
    u32 EIP;
    u32 CS;
    u32 PSW;
    u32 ESP3;
    u32 SS3;
} InterruptContext;

// Process Manager
typedef struct ProcessManager {
    PCB* Current;
    Queue* RunnableProcesses;
} ProcessManager;

// PID allocator
typedef struct PIDAllocator {
    u32 Bitmap[MAX_PROCESS_COUNT / 32];
} PIDAllocator;