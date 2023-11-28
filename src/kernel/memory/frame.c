#include "memory.h"
#include "type.h"
#include "../lib/lib.h"

FrameAllocator frameAllocator;
static void globalFrameAllocatorInit();
static Boolean isFree(PhysicalPageNumber ppn); 
static void setFree(PhysicalPageNumber ppn);
static void setUsed(PhysicalPageNumber ppn);
static Boolean isSpecifiedMode(PhysicalPageNumber ppn, MachineMode mode);
static Boolean isDirty(PhysicalPageNumber ppn);
static void setDirty(PhysicalPageNumber ppn);
static void setNotDirty(PhysicalPageNumber ppn);

PhysicalAddress AllocateOnePage(MachineMode mode) {
    for (Size i = 256; i < MAX_PAGE_COUNT; i++) {
        if (isFree(i) && isSpecifiedMode(i, mode)) {
            setUsed(i);
            frameAllocator.TotalFreePageCount--;
            if (mode == KernelMode) {
                frameAllocator.KernelFreePageCount--;
            }

            MemoryFree(GetAddressFromPPN(i), PageSize);
            return GetAddressFromPPN(i);
        }
    }
    Panic("No free page when allocate one page: mode=%d", mode);
}

PhysicalAddress AllocatePagesContinuously(MachineMode mode, u32 pageCount) {
    if (pageCount == 1) {
        return AllocateOnePage(mode);
    }

    for (Size i = 256; i < MAX_PAGE_COUNT; i++) {
        if (isFree(i) && isSpecifiedMode(i, mode)) {
            Boolean hasPages = TRUE;
            for (Size j = 0; j < pageCount; j++) {
                if (!isFree(i + j) || !isSpecifiedMode(i + j, mode)) {
                    hasPages = FALSE;
                    break;
                }
            }

            if (hasPages) {
                for (Size j = 0; j < pageCount; j++) {
                    setUsed(i + j);
                    frameAllocator.TotalFreePageCount--;
                    if (mode == KernelMode) {
                        frameAllocator.KernelFreePageCount--;
                    }
                }

                MemoryFree(GetAddressFromPPN(i), pageCount * PageSize);
                return GetAddressFromPPN(i);
            }
        }
    }
    Panic("No free page when allocate pages: mode=%d, pageCount=%d", mode, pageCount);
}

void FreeOnePage(PhysicalAddress address) {
    Assert(address % PageSize == 0);

    PhysicalPageNumber ppn = GetPPNFromAddressFloor(address);
    setFree(ppn);
    frameAllocator.TotalFreePageCount++;
    if (isSpecifiedMode(ppn, KernelMode)) {
        frameAllocator.KernelFreePageCount++;
    }
}

void MemoryCheckout(PhysicalAddress* ardCountAddress) {
    // Construct frameAllocator
    globalFrameAllocatorInit();

    Size ardCount = *(Size*)ardCountAddress;

    AddressRangeDescriptor* ardt = (AddressRangeDescriptor*)(ardCountAddress + 1);
    for (Size i = 0; i < ardCount; i++) {
        AddressRangeDescriptor ard = ardt[i];
        if (ard.Type == 1 && ard.BaseAddress >= 0x100000) {
            // Available Memory

            // align to PageSize
            ard.BaseAddress = GetAddressFromPPN(GetPPNFromAddressCeil(ard.BaseAddress));
            for (Size j = 0; j < ard.Length; j += PageSize) {
                if (frameAllocator.TotalFreePageCount == 7853) {
                    for (Size k = 0; k < 2; k++);
                }
                FreeOnePage(ard.BaseAddress + j);
            }
        }
    }
}

PhysicalPageNumber GetPPNFromAddressFloor(PhysicalAddress address) {
    return address >> PageSizeBits;
}

PhysicalPageNumber GetPPNFromAddressCeil(PhysicalAddress address) {
    return (address + PageSize - 1) >> PageSizeBits;
}

PhysicalAddress GetAddressFromPPN(PhysicalPageNumber ppn) {
    return ppn << PageSizeBits;
}

static void globalFrameAllocatorInit() {
    frameAllocator.TotalFreePageCount = 0;
    frameAllocator.KernelFreePageCount = 0;
    for (Size i = 0; i < MAX_PAGE_COUNT; i++) {
        if (i < 1024) { // 4MB
            frameAllocator.Pages[i] = 0b00000011; // 0b00000001 means free and belong to kernel
            continue;
        }

        frameAllocator.Pages[i] = 0b00000001; // 0b00000001 means free and belong to user
    }
}

static Boolean isFree(PhysicalPageNumber ppn) {
    return (frameAllocator.Pages[ppn] & 0b00000001) == 0;
}

static void setFree(PhysicalPageNumber ppn) {
    frameAllocator.Pages[ppn] = frameAllocator.Pages[ppn] & 0b11111110;
}

static void setUsed(PhysicalPageNumber ppn) {
    frameAllocator.Pages[ppn] = frameAllocator.Pages[ppn] | 0b00000001;
}

static Boolean isSpecifiedMode(PhysicalPageNumber ppn, MachineMode mode) {
    switch (mode) {
        case KernelMode:
            return frameAllocator.Pages[ppn] & 0b00000010;
        case UserMode:
            return !(frameAllocator.Pages[ppn] & 0b00000010);
        default:
            Panic("Unknown mode: mode=%d", mode);
    }
}

static Boolean isDirty(PhysicalPageNumber ppn) {
    return (frameAllocator.Pages[ppn] & 0b00000100) == 0;
}

static void setDirty(PhysicalPageNumber ppn) {
    frameAllocator.Pages[ppn] = frameAllocator.Pages[ppn] | 0b00000100;
}

static void setNotDirty(PhysicalPageNumber ppn) {
    frameAllocator.Pages[ppn] = frameAllocator.Pages[ppn] & 0b11111011;
}