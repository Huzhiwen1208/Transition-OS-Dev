#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"
#include "process/mod.h"

void ProcessA() {
    while (TRUE) {
        Printf("A");
    }
}

void ProcessB() {
    while (TRUE) {
        Printf("B");
    }
}

void TransitionMain() {
    InitConsole();
    Printf("Welcome to Transition OS!");
    InitMemoryManager();
    InitInterrupt();
    InitProcessManager();

    CreateKernelProcess(ProcessA);
    CreateKernelProcess(ProcessB);
    Schedule();
}