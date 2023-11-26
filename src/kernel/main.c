#include "console/console.h"
#include "utils/utils.h"

void TransitionMain() {
    ConsoleInit();
    char* welcome = "Welcome to Transition!\n";
    ConsoleWriteWithColor(welcome, StringLength(welcome), LIGHT_BLUE);
}