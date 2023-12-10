#include "stdlib.h"

void UserTest() {
    Info("Hello! User test process!");

    PID pid = Fork();
    if (pid) {
        // 父进程
        PID id = GetPID();
        PID ppid = GetPPID();
        i32 exitCode = -2;
        Warn("waiting for child process to exit!");
        PID cid = WaitPid(pid, &exitCode);
        Warn("child process {pid: %d} exited! exit code: %d", cid, exitCode);
    } else {
        // 子进程
        PID pid = GetPID();
        PID ppid = GetPPID();
        int cnt = 0;
        while (TRUE) {
            Sleep(1000);
            cnt ++;
            if (cnt == 5) {
                Debug("child process will exit!");
                Exit(0);
            }
            Info("Hello! I'm child process! pid, ppid: {%d, %d}", pid, ppid);
        }
    }
    Suspend();
}