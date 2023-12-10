#pragma once

#include "type.h"

// 分页管理
/// @brief allocate one page in specified mode
/// @param mode: User / kernel
/// @return physical address
PhysicalAddress AllocateOnePage(MachineMode mode);

/// @brief allocate pageCount pages continuously in specified mode
/// @param mode 
/// @param pageCount 
/// @return Physical address
PhysicalAddress AllocatePagesContinuously(MachineMode mode, u32 pageCount);

/// @brief free one page by physical address
/// @param address 
void FreeOnePage(PhysicalAddress address);

// 页表管理
void SetRootPageTableAddr(PhysicalAddress addr);
PhysicalAddress GetRootPageTableAddr();
void InitializeMemoryMapping();
PhysicalPageNumber GetPPNFromAddressFloor(PhysicalAddress address);
PhysicalPageNumber GetPPNFromAddressCeil(PhysicalAddress address);
PhysicalAddress GetAddressFromPPN(PhysicalPageNumber ppn);
VirtualAddress GetCR2();
void FlushTLB(VirtualAddress addr);
void MapPage(VirtualAddress addr);
void EnablePaging();
void DisablePaging();

// 堆分配管理
/// @brief allocate memory size
/// @param size: memory size
/// @return physical address
PhysicalAddress Malloc(Size size);

/// @brief free memory by physical address
/// @param address 
void Free(PhysicalAddress address);

// 初始化
/// @brief Check out memory and construct memory manager
/// @param ardCount The count of address range descriptors
void MemoryCheckout(PhysicalAddress* ardCountAddress);

void InitializeMemoryManager();