#pragma once

#include "../common/common.h"

typedef struct Cursor {
    u32 Row, Col; // current Cursor position
} Cursor;

Size ConsoleWrite(const char* buf, Size len);
Size ConsoleWriteWithColor(const char* buf, Size len, ConsoleColor color);
void ConsoleAlignLine();

/// @brief clear the screen and initialize
void InitConsole();