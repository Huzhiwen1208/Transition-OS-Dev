#pragma once

#include "type.h"

void InitializeProcessManager();
PCB* GetCurrentProcess();

void CreateKernelProcess(void* entry);
void CreateUserProcess(void* entry);
PID ForkProcess();
PID GetProcessID();
PID GetProcessParentID();
void Schedule();