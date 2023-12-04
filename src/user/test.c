#include "stdlib.h"

void UserTest() {
    while(TRUE) {
        Info("Hello! User test process!");
        Sleep(100);
    }
    Suspend();
}