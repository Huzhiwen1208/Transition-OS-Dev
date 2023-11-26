#pragma once

#include "type.h"

extern const u32 EOF;
extern const u32 NULL;
extern const u8 True;
extern const u8 False;

/// @brief console
typedef enum ConsoleColor {
    BLACK,
    BLUE,
    GREEN,
    CYAN,
    RED,
    PURPLE,
    YELLOW,
    WHITE,
    GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_CYAN,
    LIGHT_RED,
    LIGHT_PURPLE,
    LIGHT_YELLOW,
    BRIGHT_WHITE
} ConsoleColor;

// CathodeRayTube Monitor (CRT)
extern const u16 MonitorAddressRegister;
extern const u16 MonitorDataRegister;
extern const u8 ScreenAddressHigh;
extern const u8 ScreenAddressLow;
extern const u8 CursorAddressHigh;
extern const u8 CursorAddressLow;
extern const u32 MonitorBaseAddress;
extern const u32 MonitorSize;
extern const u32 ScreenWidth;
extern const u32 ScreenWidthByte;
extern const u32 ScreenHigh;
extern const u32 ScreenSize;

// Character
extern const u8 NullChar;
extern const u8 BS;
extern const u8 HT;
extern const u8 LF;
extern const u8 CR;
extern const u8 Space;