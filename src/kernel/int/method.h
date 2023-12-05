#pragma once

#include "type.h"

void InitializeInterrupt();
void SetInterruptHandler(u32 vector, void* handler);
void SetInterrupt(u32 vector);
void OuteralInterruptCompleted(u32 vector);

// clock

void InitializeClock();
void GetTime(TimeValue* tv);
u32 GetTimeMS();