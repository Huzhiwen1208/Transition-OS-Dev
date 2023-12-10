#include "stdlib.h"

void UserTest() {
    Info("Hello! User test process!");

    PID pid = Fork();
    if (pid) {
        // 父进程
        PID pid = GetPID();
        PID ppid = GetPPID();
        Info("Hello! I'm parent process! pid, ppid: {%d, %d}", pid, ppid);
    } else {
        // 子进程
        PID pid = GetPID();
        PID ppid = GetPPID();
        Info("Hello! I'm child process! pid, ppid: {%d, %d}", pid, ppid);
    }
    Suspend();
}