#pragma once

#include "../common/mod.h"

// Page
typedef struct AddressRangeDescriptor {
    u64 BaseAddress;
    u64 Length;
    u32 Type;
} AddressRangeDescriptor;

typedef struct FrameAllocator {
    /* Pages[i]: 0b00000xxx;
        x0: 0 means free, 1 means used
        x1: 0 means kernel, 1 means user
        x2: 0 means not dirty, 1 means dirty
    */
    u8 Pages[MAX_PAGE_COUNT];
    u32 TotalFreePageCount;
    u32 KernelFreePageCount;
} FrameAllocator;

// Buddy
typedef struct BuddyBlock {
    PhysicalAddress BaseAddress;
    Size BlockSize;
    Boolean IsUsed;
    i32 Next;
} BuddyBlock;

typedef struct BuddyAllocator {
    BuddyBlock Blocks[BUDDY_BLOCKS];
    MachineMode Mode;
} BuddyAllocator;

// 内存映射，二级页表

struct PageTableEntry {
    u8 Present : 1; // present in memory?
    u8 Write : 1;   // 0: readonly, 1: rw
    u8 User : 1;    // 用户可访问？
    u8 PageWriteThrough : 1;
    u8 PageCacheDisable : 1;
    u8 Access : 1;    // 访问位
    u8 Dirty : 1;     // 脏位
    u8 Pat : 1;       // page attribute table, 0: 4K, 1: 4M
    u8 Global : 1;    // 全局页？
    u8 Reversed : 3;  // unused
    u32 NextPPN : 20; // page index，下一级页表的PPN
} StructNotAlign;
typedef struct PageTableEntry PageTableEntry;