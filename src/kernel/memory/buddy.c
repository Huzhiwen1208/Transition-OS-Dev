#include "memory.h"
#include "type.h"
#include "../lib/lib.h"

BuddyAllocator globalBuddyAllocator;

static i32 getFreeBlockIndex();
static void freeBlock(i32 index);
static void mergeBuddy();
static BuddyBlock* getBuddyBlock(i32 index);

PhysicalAddress Malloc(Size size) {
    // find first block which size is bigger than needed
    i32 pre = 0;
    i32 p = getBuddyBlock(0)->Next;
    while (p != -1) {
        if (getBuddyBlock(p)->IsUsed == FALSE && getBuddyBlock(p)->BlockSize >= size) {
            break;
        }
        pre = p;
        p = getBuddyBlock(p)->Next;
    }

    if (p == -1)
        Panic("No enough memory when malloc: size=%d", size);
    
    while(getBuddyBlock(p)->BlockSize >= 2*size) {
        // split
        Size splitSize = getBuddyBlock(p)->BlockSize >> 1;
        
        i32 block1Index = getFreeBlockIndex();
        i32 block2Index = getFreeBlockIndex();
        BuddyBlock* block1 = getBuddyBlock(block1Index);
        BuddyBlock* block2 = getBuddyBlock(block2Index);
        block1->BaseAddress = getBuddyBlock(p)->BaseAddress;
        block1->BlockSize = splitSize;
        block1->Next = block2Index;
        block2->BaseAddress = getBuddyBlock(p)->BaseAddress + splitSize;
        block2->BlockSize = splitSize;
        block2->Next = getBuddyBlock(p)->Next;
        getBuddyBlock(pre)->Next = block1Index;
        freeBlock(p);
        p = block1Index;
    }

    getBuddyBlock(p)->IsUsed = TRUE;
    return getBuddyBlock(p)->BaseAddress;
}

void Free(PhysicalAddress address) {
    i32 pre = 0;
    i32 p = getBuddyBlock(0)->Next;
    while (p != -1) {
        if (getBuddyBlock(p)->BaseAddress == address) {
            break;
        }
        pre = p;
        p = getBuddyBlock(p)->Next;
    }

    getBuddyBlock(p)->IsUsed = FALSE;
    mergeBuddy();
}

void globalBuddyAllocatorInit() {
    for (Size i = 0; i < BUDDY_PAGES; i++) {
        globalBuddyAllocator.Blocks[i].BaseAddress = 0;
        globalBuddyAllocator.Blocks[i].BlockSize = 0;
        globalBuddyAllocator.Blocks[i].IsUsed = FALSE;
        globalBuddyAllocator.Blocks[i].Next = 0;
    }

    globalBuddyAllocator.Mode = KernelMode;
    PhysicalAddress baseAddress = AllocatePagesContinuously(KernelMode, BUDDY_PAGES);
    Size size = BUDDY_PAGES * PageSize;

    globalBuddyAllocator.Blocks[0].BaseAddress = baseAddress;
    globalBuddyAllocator.Blocks[0].BlockSize = size;
    globalBuddyAllocator.Blocks[0].IsUsed = TRUE;
    globalBuddyAllocator.Blocks[0].Next = 1;

    globalBuddyAllocator.Blocks[1].BaseAddress = baseAddress;
    globalBuddyAllocator.Blocks[1].BlockSize = size;
    globalBuddyAllocator.Blocks[1].IsUsed = FALSE;
    globalBuddyAllocator.Blocks[1].Next = -1;
}

static i32 getFreeBlockIndex() {                                                                                                                                               
    for (i32 i = 0; i < BUDDY_BLOCKS; i++) {
        if (globalBuddyAllocator.Blocks[i].Next == 0) {
            globalBuddyAllocator.Blocks[i].Next = -1;
            globalBuddyAllocator.Blocks[i].IsUsed = FALSE;
            globalBuddyAllocator.Blocks[i].BlockSize = 0;
            globalBuddyAllocator.Blocks[i].BaseAddress = 0;
            return i;
        }
    }

    Panic("No free block when get free block index");
}

static void freeBlock(i32 index) {
    globalBuddyAllocator.Blocks[index].Next = 0;
}

static void mergeBuddy() {
    i32 pre = getBuddyBlock(0)->Next;
    i32 p = getBuddyBlock(pre)->Next;
    while (p != -1) {
        if (getBuddyBlock(p)->IsUsed == FALSE 
            && getBuddyBlock(pre)->IsUsed == FALSE
            && ((getBuddyBlock(pre)->BaseAddress - getBuddyBlock(0)->BaseAddress) % getBuddyBlock(p)->BlockSize == 0)
            && getBuddyBlock(pre)->BlockSize == getBuddyBlock(p)->BlockSize) {
                // merge
                getBuddyBlock(pre)->BlockSize <<= 1;
                getBuddyBlock(pre)->Next = getBuddyBlock(p)->Next;
                freeBlock(p);
                p = getBuddyBlock(0)->Next;
        }
        pre = p;
        p = getBuddyBlock(p)->Next;
    }
}

static BuddyBlock* getBuddyBlock(i32 index) {
    return &globalBuddyAllocator.Blocks[index];
}