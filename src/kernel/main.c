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
    CreateFile(-1, "test2.txt", FT_FILE);
    ListFiles("", "/");
    WriteFileContent("/test.txt", "Hello World!", FALSE);
    PrintFileContent("test.txt", 1);

    MakeDirectory("/test", "");
    MakeDirectory("/testdir/x1", "-p");
    MakeDirectory("/testdir/x33", "-p");
    ChangeDirectory("/testdir/");
    ListFiles("", ".");
    ChangeDirectory("/");

    ListFiles("", ".");
    RemoveFile("-r", "/testdir");
    ListFiles("", "/");

    // CreateUserProcess(UserTest);
}