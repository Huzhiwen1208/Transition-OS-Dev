#pragma once

#include "../common/mod.h"

struct InterruptDescriptor {
    u16 OffsetLow;  // 中断处理函数的偏移量 0-15
    u16 Selector;   // 中断处理函数的段选择子
    u8 Reserved;    // unused
    u8 Type : 4;    // task gate/interrupt gate/trap gate -> 0x0110/0x1110/...
    u8 Segment : 1; // 0: system segment, 1: code segment, should be 0
    u8 DPL : 2;     // DPL, should be 0
    u8 Present : 1; // present in memory?
    u16 OffsetHigh; // 中断处理函数的偏移量 16-31
} StructNotAlign;
typedef struct  InterruptDescriptor InterruptDescriptor;


struct DescriptorTablePointer {
    u16 Limit; // 表界限
    u32 Base;  // 表基址
} StructNotAlign;
typedef struct DescriptorTablePointer DescriptorTablePointer;


struct GlobalDescriptor {
    u16 LimitLow; // 段界限 0-15
    u32 BaseLow: 24; // 段基址 0-23
    u8 Type: 4; // 段类型
    u8 Segment: 1; // 0: system segment, 1: code segment, should be 0
    u8 DPL: 2; // DPL, should be 0
    u8 Present: 1;
    u8 LimitHigh: 4; // 段界限 16-19
    u8 Available: 1; 
    u8 LongMode: 1; // 64位代码标志
    u8 DefaultOperationSize: 1; // default operation size 0: 16bit, 1: 32bit
    u8 Granularity: 1; // 0: 1B, 1: 4KB
    u8 BaseHigh; // 段基址 24-31
} StructNotAlign;
typedef struct GlobalDescriptor GlobalDescriptor;

struct TaskStateSegment {
    u32 BackLink; // segment selector of former task
    u32 ESP0;     // stack pointer of ring0
    u32 SS0;      // stack segment selector of ring0
    u32 ESP1;
    u32 SS1; 
    u32 ESP2;
    u32 SS2; 
    u32 CR3;
    u32 EIP;
    u32 EFLAGS;
    u32 EAX;
    u32 ECX;
    u32 EDX;
    u32 EBX;
    u32 ESP;
    u32 EBP;
    u32 ESI;
    u32 EDI;
    u32 ES;
    u32 CS;
    u32 SS;
    u32 DS;
    u32 FS;
    u32 GS;
    u32 LDTR;          // local descriptor table segment selector
    u8 Trace : 1;     // if 1, will cause an interrupt when a task switch occurs
    u16 Reserved : 15; // unused
    u16 IOBase;
    u32 SSP;
} StructNotAlign;
typedef struct TaskStateSegment TaskStateSegment;