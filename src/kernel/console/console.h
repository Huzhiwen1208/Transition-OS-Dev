#pragma once

#include "../common/common.h"

typedef struct Cursor {
    u32 Row, Col; // current Cursor position
} Cursor;

void ConsoleWrite(const char* buf, u32 len);
void ConsoleWriteWithColor(const char* buf, u32 len, ConsoleColor color);

/// @brief clear the screen and initialize
void ConsoleInit();