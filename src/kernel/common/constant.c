#include "type.h"

const u32 EOF = -1;
const u32 NULL = 0;
const u8 true = 1;
const u8 false = 0;

// CathodeRayTube Monitor (CRT)
const u16 MonitorAddressRegister = 0x3D4;
const u16 MonitorDataRegister = 0x3D5;

const u8 ScreenAddressHigh = 0xC;
const u8 ScreenAddressLow = 0xD;
const u8 CursorAddressHigh = 0xE;
const u8 CursorAddressLow = 0xF;

const u32 MonitorBaseAddress = 0xB8000;
const u32 MonitorSize = 0x4000;
const u32 ScreenWidthByte = 80 * 2; // 80 chars per line, but 160 bytes per line
const u32 ScreenWidth = 80;
const u32 ScreenHigh = 25;
const u32 ScreenSize = ScreenWidthByte * ScreenHigh;

// Character
const i32 NullChar = 0x00;
const i32 BS = 0x08; // \b
const i32 HT = 0x09; // \t
const i32 LF = 0x0A; // \n
const i32 CR = 0x0D; // \r
const i32 Space = 0x20;