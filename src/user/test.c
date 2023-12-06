#include "stdlib.h"

void UserTest() {
    Info("Hello! User test process!");
    Sleep(100);
    char buf[100];
    Size size = Readline(buf, 100);
    Assert(size == StringLength(buf));

    Info("You input: %s", buf);
    Suspend();
}