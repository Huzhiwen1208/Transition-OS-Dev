#include "console/console.h"
#include "lib/lib.h"

void TransitionMain() {
    ConsoleInit();
    char* welcome = "Welcome to Transition!\n";
    Printf("%s", welcome);
    u32 count = 0;
    

    while (1) {
        PrintWithColor(LIGHT_RED, "This is a test for PrintWithColor %d\n", count);
        count ++;
    }
}