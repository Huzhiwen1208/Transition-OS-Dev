#include "mod.h"

static void syscallTest(u32 arg1, u32 arg2, u32 arg3) {
    Info("syscall test: arg{%d, %d, %d}", arg1, arg2, arg3);
}

static void syscallGetTime(TimeValue* tv) {
    GetTime(tv);
}

static void syscallYield() {
    Schedule();
}

u32 TrapHandler(u32 syscallNum, u32 arg1, u32 arg2, u32 arg3) {
    switch (syscallNum) {
        case SYSCALL_TEST:
            syscallTest(arg1, arg2, arg3);
            break;
        case SYSCALL_TIME:
            syscallGetTime((TimeValue*)arg1);
            break;
        case SYSCALL_YIELD:
            syscallYield();
            break;
        default:
            Panic("Unknown syscall number: %d", syscallNum);
    }

    return 0;
}