#pragma once

#include "type.h"
#include "console.h"

Size ConsoleWrite(const char* buf, Size len);
Size ConsoleWriteWithColor(const char* buf, Size len, ConsoleColor color);
void ConsoleAlignLine();

/// @brief clear the screen and initialize
void InitConsole();