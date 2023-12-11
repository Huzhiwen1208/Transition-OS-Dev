#pragma once

#include "type.h"

void InitializeProcessManager();
PCB* GetCurrentProcess();
void AddProcess(PCB* process);
PID AllocatePID();
void FreePID(PID pid);
PCB* FindActivatedChildProcessByPID(PID pid);
PCB* TakeZombieProcess(PID pid);

void CreateKernelProcess(void* entry);
void CreateUserProcess(void* entry);
// fork related

PID ForkProcess();
void ExitProcess(i32 exitCode);
PID WaitProcess(PID pid, i32* exitCode);

PID GetProcessID();
PID GetProcessParentID();
void Schedule();