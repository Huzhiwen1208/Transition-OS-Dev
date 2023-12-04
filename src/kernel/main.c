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
        TimeValue time;
        GetTime(&time);
        Printf("Process A ms: %d\n", time.Second * 1000 + time.MicroSecond / 1000);
        Yield();
        if (count > 3) break;
    }
    Suspend();
}

void ProcessB() {
    asm volatile ("sti");
    u32 count = 0;
    while (TRUE) {
        count ++;
        TimeValue time;
        GetTime(&time);
        Printf("Process B ms: %d\n", time.Second * 1000 + time.MicroSecond / 1000);
        Yield();
        if (count > 3) break;
    }
    Suspend();
}

void UserProcess() {
    u32 count = 0;
    while (TRUE) {
        u32 count = 0;
        count ++;
    }
    Suspend();
}

void TransitionMain() {
    InitializeConsole();
    Printf("Welcome to Transition OS!\n");
    InitializeMemoryManager();
    InitializeInterrupt();
    InitializeProcessManager();

    CreateUserProcess(UserProcess);
}