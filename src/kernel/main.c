#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"
#include "process/mod.h"

void ProcessA() {
    asm volatile ("sti");
    u32 count = 0;
    while (TRUE) {
        count ++;
        Test();
        if (count > 3) break;
    }
    Suspend();
}

void ProcessB() {
    asm volatile ("sti");
    u32 count = 0;
    while (TRUE) {
        count ++;
        Printf("B: ");
        Test();
        if (count > 3) break;
    }
    Suspend();
}

void TransitionMain() {
    InitializeConsole();
    Printf("Welcome to Transition OS!\n");
    InitializeMemoryManager();
    InitializeInterrupt();
    InitializeProcessManager();

    CreateKernelProcess(ProcessA);
    CreateKernelProcess(ProcessB);
}