#include "console/mod.h"
#include "lib/mod.h"
#include "ds/mod.h"
#include "memory/mod.h"
#include "int/mod.h"
#include "process/mod.h"
#include "device/mod.h"
#include "fs/mod.h"

extern void UserTest();

void TransitionMain() {
    InitializeConsole();
    InitializeMemoryManager();
    InitializeInterrupt();
    Printf("Welcome to Transition OS!\n");
    InitializeProcessManager();
    InitializeDevice();
    InitializeFileSystem();
    
    CreateFile(-1, "test.txt", FT_FILE);
    WriteFileContent("test.txt", "Hello, world2222!\n", FALSE);
    WriteFileContent("test.txt", "Hello, world3333!\n", FALSE);

    char buf[100];
    ReadFileLine("test.txt", 1, buf);
    Printf("Read: %s\n", buf);
    MemoryFree(buf, 100);
    ReadFileLine("test.txt", 2, buf);
    Printf("Read: %s\n", buf);

    ClearFileContent("test.txt");
    MemoryFree(buf, 100);
    Boolean read = ReadFileLine("test.txt", 1, buf);
    Assert(read == FALSE);
    Assert(StringLength(buf) == 0);

    // CreateUserProcess(UserTest);
}