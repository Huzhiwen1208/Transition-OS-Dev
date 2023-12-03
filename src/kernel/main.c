#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"
#include "process/mod.h"

void TransitionMain() {
    InitConsole();
    Printf("Welcome to Transition OS!");
    InitMemoryManager();
    InitInterrupt();
    InitProcessManager();
}