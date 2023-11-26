#include "console/console.h"
#include "utils/utils.h"

void TransitionMain() {
    ConsoleInit();
    char* welcome = "Welcome to Transition!\n";
    Printf("%s", welcome);

    PrintWithColor(LIGHT_RED, "This is a test for PrintWithColor\n");
}