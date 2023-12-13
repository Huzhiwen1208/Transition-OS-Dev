#include "type.h"

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

const i32 NullChar = 0x00;
const i32 BS = 0x08; // \b
const i32 HT = 0x09; // \t
const i32 LF = 0x0A; // \n
const i32 CR = 0x0D; // \r
const i32 Space = 0x20;

const u32 ManagableMemorySize = 0x2000000; // 32MB
const u32 PageSize = 0x1000;               // 4KB
const u32 PageSizeBits = 12;
const u32 KernelPageCount = 0x400; // 4MB

// Task
const u32 UserStackTop = 0x10000000; // 256MB

// Interrupt
const u16 ClockCounter0 = 0x40;
const u16 ClockCounter1 = 0x41;
const u16 ClockCounter2 = 0x42;
const u16 ControlWordRegister = 0x43;
const u32 OsclilatorFrequency = 1193180;
const u32 ClockFrequency = 100;
const u32 ClockMaxTicks = OsclilatorFrequency / ClockFrequency;
const u32 JeffyMS = 10;
const float MSPerTick = 1.0 / OsclilatorFrequency;
const u32 KeyboardDataPort = 0x60;

// Descriptor
const u32 KernelCodeSegmentSelector = 1 << 3;
const u32 KernelDataSegmentSelector = 2 << 3;
const u32 TSSSegmentSelector = 3 << 3;
const u32 UserCodeSegmentSelector = 4 << 3 | 3;
const u32 UserDataSegmentSelector = 5 << 3 | 3;

// Disk
const u16 IdeIobaseMaster = 0x1F0;
const u16 IdeIobaseSlave = 0x170;
const u32 IdeLbaMaster = 0xe0;
const u32 IdeLbaSlave = 0xf0;
const u32 IdeTimeout = 100000;

const u16 IdeDataRegister = 0;
const u16 IdeErrorRegister = 1;
const u16 IdeFeatureRegister = 1;
const u16 IdeSectorCountRegister = 2;
const u16 IdeSectorLocationRegister = 3;
const u16 IdeCylinderLowRegister = 4;
const u16 IdeCylinderHighRegister = 5;
const u16 IdeLbaLow = 3;
const u16 IdeLbaMid = 4;
const u16 IdeLbaHigh = 5;
const u16 IdeSelectorRegister = 6;
const u16 IdeStatusRegister = 7;
const u16 IdeCommandRegister = 7;
const u16 IdeAlternateStatusRegister = 0x206;
const u16 IdeControlRegister = 0x206;
const u16 IdeDriverAddressRegister = 0x206;

// Ide 状态寄存器
const u16 IdeStatusNull = 0;
const u16 IdeStatusErr = 1;
const u16 IdeStatusIdx = 2;
const u16 IdeStatusCorr = 4;
const u16 IdeStatusDrq = 8;
const u16 IdeStatusDsc = 16;
const u16 IdeStatusDwf = 32;
const u16 IdeStatusDrdy = 64;
const u16 IdeStatusBusy = 128;

// Ide 命令
const u8 IdeCommandRead = 0x20;
const u8 IdeCommandWrite = 0x30;
const u8 IdeCommandIdentify = 0xEC;
const u8 IdeCommandDiagnostic = 0x90;
