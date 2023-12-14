#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"
#include "process/mod.h"
#include "device/mod.h"

extern void UserTest();

void TransitionMain() {
    InitializeConsole();
    InitializeMemoryManager();
    InitializeInterrupt();
    Printf("Welcome to Transition OS!\n");
    InitializeProcessManager();
    InitializeDevice();
    
    char buffer[512];
    DeviceRead(0, 0, 1, buffer);
    DeviceWrite(1, 0, 1, buffer);

    char buffer2[512];
    DiskCacheRead(0, buffer2);
    MemoryFree(buffer2, 512);
    DiskCacheWrite(0, buffer2);
    // CreateUserProcess(UserTest);
}