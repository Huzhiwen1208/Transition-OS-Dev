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