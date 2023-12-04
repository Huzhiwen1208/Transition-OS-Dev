#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"
#include "process/mod.h"

extern void UserTest();
extern void UserTestSchedule();
void KernelTest() {
    while (TRUE) {
        Printf("Kernel\n");
        Sleep(100);
    }
}

void TransitionMain() {
    InitializeConsole();
    InitializeMemoryManager();
    InitializeInterrupt();
    Printf("Welcome to Transition OS!\n");
    InitializeProcessManager();

    CreateUserProcess(UserTest);
    CreateUserProcess(UserTestSchedule);
    CreateKernelProcess(KernelTest);
}