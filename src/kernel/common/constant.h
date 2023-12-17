#pragma once

#include "type.h"

// common
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
#define BUDDY_PAGES 32
#define BUDDY_BLOCKS 1024

// Process Mode
typedef enum MachineMode
{
    KernelMode,
    UserMode
} MachineMode;

// Data structure
#define HASH_TABLE_SIZE 129

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
extern const u32 KeyboardDataPort; // 键盘中断数据端口

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
#define SYSCALL_EXIT 9
#define SYSCALL_WAIT_PID 10

// Device
#define NAME_LENGTH 32
#define IDE_CONTROLLER_COUNT 2
#define DISK_COUNT_PER_CONTROLLER 2
#define DEVICE_COUNT 16

// Ide 相关
extern const u16 IdeIobaseMaster; // 主盘IO基地址
extern const u16 IdeIobaseSlave;  // 从盘IO基地址
extern const u32 IdeLbaMaster;    // 主盘 LBA
extern const u32 IdeLbaSlave;     // 从盘 LBA
extern const u32 IdeTimeout;      // 超时时间

// Ide 寄存器
extern const u16 IdeDataRegister;            // 数据寄存器
extern const u16 IdeErrorRegister;           // 错误寄存器
extern const u16 IdeFeatureRegister;         // 功能寄存器
extern const u16 IdeSectorCountRegister;     // 扇区数量寄存器
extern const u16 IdeSectorLocationRegister;  // 扇区位置寄存器
extern const u16 IdeCylinderLowRegister;     // 柱面低字节寄存器
extern const u16 IdeCylinderHighRegister;    // 柱面高字节寄存器
extern const u16 IdeLbaLow;                  // LBA 低字节
extern const u16 IdeLbaMid;                  // LBA 中字节
extern const u16 IdeLbaHigh;                 // LBA 高字节
extern const u16 IdeSelectorRegister;        // 磁盘选择寄存器
extern const u16 IdeStatusRegister;          // 状态寄存器
extern const u16 IdeCommandRegister;         // 命令寄存器
extern const u16 IdeAlternateStatusRegister; // 备用状态寄存器
extern const u16 IdeControlRegister;         // 设备控制寄存器
extern const u16 IdeDriverAddressRegister;   // 驱动器地址寄存器

// Ide 状态寄存器
extern const u16 IdeStatusNull; // NULL
extern const u16 IdeStatusErr;  // Error
extern const u16 IdeStatusIdx;  // Index
extern const u16 IdeStatusCorr; // Corrected data
extern const u16 IdeStatusDrq;  // Data request
extern const u16 IdeStatusDsc;  // Drive seek complete
extern const u16 IdeStatusDwf;  // Drive write fault
extern const u16 IdeStatusDrdy; // Drive ready
extern const u16 IdeStatusBusy; // Controller busy

// Ide 命令
extern const u8 IdeCommandRead;       // 读命令
extern const u8 IdeCommandWrite;      // 写命令
extern const u8 IdeCommandIdentify;   // 识别命令
extern const u8 IdeCommandDiagnostic; // 诊断命令

// 文件系统
extern const u32 BlockCacheStart; // 磁盘块缓存起始地址
extern const u32 BlockCacheEnd;   // 磁盘块缓存结束地址
#define INODE_DIRECT_BLOCKS 27