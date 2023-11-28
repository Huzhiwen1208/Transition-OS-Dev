#include "console/console.h"
#include "lib/lib.h"
#include "memory/memory.h"

void TransitionMain() {
    InitConsole();
    InitMemoryManager();
    char* welcome = "Welcome to Transition!\n";
    Printf("%s", welcome);

    char* m = Malloc(10);
    *m = 'H';
    *(m+1) = 'E';
}