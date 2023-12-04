#pragma once

#include "type.h"

void InitializeProcessManager();
void CreateKernelProcess(void* entry);
void CreateUserProcess(void* entry);
void Schedule();