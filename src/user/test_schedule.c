#include "stdlib.h"

void UserTestSchedule() {
    while(TRUE) {
        Info("Hello! User test schedule process!");
        Sleep(100);
    }
    Suspend();
}