#pragma once

#include "type.h"

#define EOF -1
#define NULL 0
#define TRUE 1
#define FALSE 0

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

// Memory Manager
extern const u32 ManagableMemorySize;
extern const u32 PageSize;
extern const u32 PageSizeBits;
#define MAX_PAGE_COUNT 8192
extern const u32 KernelPageCount;
#define BUDDY_PAGES 16
#define BUDDY_BLOCKS 1024

// Mode
typedef enum MachineMode {
    KernelMode,
    UserMode
} MachineMode;

// Data structure
#define HASH_TABLE_SIZE 101

// Interrupt
#define INTERRUPT_COUNT 256
#define EXCEPTION_COUNT 32
#define OUTERAL_INTERRUPT_COUNT 16
extern const u16 ClockCounter0; // Intel 8253/8254
extern const u16 ClockCounter1;
extern const u16 ClockCounter2;
extern const u16 ControlWordRegister;
extern const u32 OsclilatorFrequency;
extern const u32 ClockFrequency;
extern const u32 ClockMaxTicks;
extern const u32 JeffyMS;
extern const float MSPerTick;
extern const u32 KeyboardDataPort; //键盘中断数据端口

// Task PCB
#define MAX_PROCESS_COUNT 1024
extern const u32 UserStackTop;

// Descriptor
#define SEGMENT_COUNT 8192
extern const u32 KernelCodeSegmentSelector;
extern const u32 KernelDataSegmentSelector;
extern const u32 UserCodeSegmentSelector;
extern const u32 UserDataSegmentSelector;
extern const u32 TSSSegmentSelector;

// System call number
#define SYSCALL_TEST 1
#define SYSCALL_TIME 2
#define SYSCALL_YIELD 3
#define SYSCALL_WRITE 4
#define SYSCALL_READ 5
#define SYSCALL_FORK 6
#define SYSCALL_GETPID 7
#define SYSCALL_GETPPID 8