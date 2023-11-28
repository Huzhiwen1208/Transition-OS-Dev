#include "console/console.h"
#include "lib/lib.h"
#include "memory/memory.h"

void TransitionMain() {
    InitConsole();
    Printf("Welcome to Transition OS!");
    Info("This is a test for log");
    InitMemoryManager();

    char* message = (char*)Malloc(100);
    i32* arr = (i32*)Malloc(sizeof(i32) * 10);
    u8* bytes = (u8*)Malloc(sizeof(u8) * 1025);

    *message = 'H';
    *(message + 1) = 'E';
    Assert(StringLength(message) == 2);
    Assert(StringEqual(message, "HE"));

    *(arr + 3) = 267;

    Free(bytes);
    Free(arr);
    Free(message);
}