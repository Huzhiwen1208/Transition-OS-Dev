#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"
#include "process/mod.h"

void ProcessA() {
    asm volatile ("sti");
    while (TRUE) {
        Printf("ProcessA: time = %d\n", GetTimeMS());
    }
}

void ProcessB() {
    asm volatile ("sti");
    while (TRUE) {
        Printf("ProcessB: time = %d\n", GetTimeMS());
    }
}

void TransitionMain() {
    InitializeConsole();
    Printf("Welcome to Transition OS!");
    InitializeMemoryManager();
    InitializeInterrupt();
    InitializeProcessManager();

    CreateKernelProcess(ProcessA);
    CreateKernelProcess(ProcessB);
    Schedule();
}