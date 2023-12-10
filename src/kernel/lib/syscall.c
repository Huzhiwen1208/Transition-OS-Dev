#include "mod.h"

static u32 SystemCall(u32 syscallNum, u32 arg1, u32 arg2, u32 arg3) {
    asm volatile ("movl %0, %%eax" : : "m"(syscallNum));
    asm volatile ("movl %0, %%ebx" : : "m"(arg1));
    asm volatile ("movl %0, %%ecx" : : "m"(arg2));
    asm volatile ("movl %0, %%edx" : : "m"(arg3));
    asm volatile ("int $0x80");
}

void Test() {
    SystemCall(SYSCALL_TEST, 1, 2, 3);
}

void GetTimeValue(TimeValue* tv) {
    SystemCall(SYSCALL_TIME, (u32)tv, 0, 0);
}

void Yield() {
    SystemCall(SYSCALL_YIELD, 0, 0, 0);
}

Size Write(char* buf, Size len, ConsoleColor color) {
    SystemCall(SYSCALL_WRITE, (u32)buf, len, (u32)color);
}

void Sleep(u32 ms) {
    TimeValue tv;
    GetTimeValue(&tv);
    u32 entryMS = tv.Second * 1000 + tv.MicroSecond / 1000;

    while(TRUE) {
        GetTimeValue(&tv);
        u32 currentMS = tv.Second * 1000 + tv.MicroSecond / 1000;
        if (currentMS >= ms + entryMS)
            break;
        Yield();
    }
}

Size Readline(char* buf, Size len) {
    return SystemCall(SYSCALL_READ, (u32)buf, len, 0);
}

PID Fork() {
    return SystemCall(SYSCALL_FORK, 0, 0, 0);
}

PID GetPID() {
    return SystemCall(SYSCALL_GETPID, 0, 0, 0);
}

PID GetPPID() {
    return SystemCall(SYSCALL_GETPPID, 0, 0, 0);
}

void Exit(i32 exitCode) {
    SystemCall(SYSCALL_EXIT, (u32)exitCode, 0, 0);
}

PID WaitPid(PID pid, i32* exitCode) {
    while (TRUE) {
        PID rid = SystemCall(SYSCALL_WAIT_PID, (u32)pid, (u32)exitCode, 0);
        switch (rid) {
            case -2:
                Yield();
                break;
            default:
                return rid;
        }
    }
}