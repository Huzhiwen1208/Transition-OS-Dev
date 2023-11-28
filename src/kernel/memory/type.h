#pragma once

#include "../common/common.h"

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