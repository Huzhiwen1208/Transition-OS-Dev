#pragma once

#include "type.h"

void InitializeIDT();
void InitializeGDT();
void SetTSSEsp0(u32 esp0);