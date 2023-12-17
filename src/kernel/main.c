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
    
    // InodeID id = CreateFile(-1, "/test.txt", FT_FILE);
    CreateFile(-1, "test.txt", FT_FILE);
    CreateFile(-1, "test1.txt", FT_FILE);
    CreateFile(-1, "test2.txt", FT_FILE);
    CreateFile(-1, "test3.txt", FT_FILE);
    CreateFile(-1, "test4.txt", FT_FILE);
    CreateFile(-1, "test5.txt", FT_FILE);
    MakeDirectory("/dir/dir2", "-p");
    MakeDirectory("/dir/dir2/dir3", "-p");
    MakeDirectory("/dir/dir6/dir3", "-p");
    MakeDirectory("/dir2/dir2/dir3", "-p");
    MakeDirectory("/dir2/dir2/dir3", "-p");
    MakeDirectory("/dir2/dir2/dir3", "-p");
    CreateFile(-1, "test23.txt", FT_FILE);
    ListFiles("-a", "/");
    PrintfWorkingDirectory();
    ChangeDirectory("dir");
    ChangeDirectory("..");
    ChangeDirectory("./dir");
    ChangeDirectory("../");
    ChangeDirectory("dir");
    ListFiles("-a", ".");
    PrintfWorkingDirectory();
    ChangeDirectory("../dir2/");
    ListFiles("-a", ".");
    PrintfWorkingDirectory();
    ChangeDirectory("./dir2");
    PrintfWorkingDirectory();
    
    // CreateUserProcess(UserTest);
}