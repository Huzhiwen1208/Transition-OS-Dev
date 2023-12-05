#pragma once

#include "type.h"

void InitializeProcessManager();
PCB* GetCurrentProcess();

void CreateKernelProcess(void* entry);
void CreateUserProcess(void* entry);
void Schedule();