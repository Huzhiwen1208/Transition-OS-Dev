#pragma once

#include "type.h"

void InitializeInterrupt();
void SetInterruptHandler(u32 vector, void* handler);
void SetInterrupt(u32 vector);
void OuteralInterruptCompleted(u32 vector);
u8 GetInterruptStatus();
void RestoreInterruptStatus(u8 status);

// clock

void InitializeClock();
void GetTime(TimeValue* tv);
u32 GetTimeMS();

// page fault

void InitializePageFaultHandler();

// keyboard
void InitializeKeyboard();
Size ReadLine(char* buf, Size len);