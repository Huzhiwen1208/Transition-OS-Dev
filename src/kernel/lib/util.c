#include "mod.h"

void Suspend() {
    while (1);
}

void AssertFailed(char *exp, char *file, char *base, i32 line) {
    PrintWithColor(LIGHT_RED, "[Assertion] %s failed!, %s:%d\n", exp, file, line);

    Suspend();
}