#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"

void TransitionMain() {
    InitConsole();
    Printf("Welcome to Transition OS!");
    InitMemoryManager();
    InitInterrupt();

    u32 a = 5 / 0;
}