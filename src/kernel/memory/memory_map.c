#include "mod.h"

// global var

/// @brief 内核页表的页目录（根页表）PPN
static u32 KernelRootPPN;

// static methods declaration

static void initPageTableEntry(PageTableEntry* pte, u32 nextPPN, Boolean user);
static void enablePaging();
static void disablePaging();
static PageTableEntry* findPTE(VirtualAddress addr);
static PageTableEntry* findPTECreate(VirtualAddress addr);

// public methods

void SetRootPageTableAddr(PhysicalAddress addr) {
    Assert(addr % PageSize == 0);
    asm volatile ("movl %0, %%cr3":: "r"(addr));
}

PhysicalAddress GetRootPageTableAddr() {
    asm volatile ("movl %cr3, %eax");
}

VirtualAddress GetCR2() {
    asm volatile ("movl %cr2, %eax");
}

void FlushTLB(VirtualAddress addr) {
    addr = GetAddressFromPPN(GetPPNFromAddressFloor(addr)); // 按页对齐
    asm volatile ("invlpg (%0)":: "r"(addr));
}

void MapPage(VirtualAddress addr) {
    disablePaging();
    findPTECreate(addr);
    enablePaging();
    FlushTLB(addr);
}

void InitializeMemoryMapping() {
    KernelRootPPN = GetPPNFromAddressFloor(
        AllocateOnePage(KernelMode)
    );

    // 初始化内核根页表的第一个页表项，指向内核二级页表的物理地址
    PageTableEntry* pte = (PageTableEntry*)GetAddressFromPPN(KernelRootPPN);
    PhysicalAddress kernelSecondPageTable = AllocateOnePage(KernelMode);
    initPageTableEntry(pte, GetPPNFromAddressFloor(kernelSecondPageTable), TRUE);

    // 初始化内核二级页表，映射内核的所有物理地址(0-4MB)
    pte = (PageTableEntry*)kernelSecondPageTable;
    for (u32 i = 0; i < 1024; i++) {
        if (i == 0) continue; // 我们认为0-0x1000的虚拟地址是空指针，不映射
        initPageTableEntry(pte + i, i, TRUE);
    }

    // 告知CPU根页表的物理地址
    SetRootPageTableAddr(GetAddressFromPPN(KernelRootPPN));
    // 通知CPU开启分页功能
    enablePaging();
}


// static methods implementation

static void initPageTableEntry(PageTableEntry* pte, u32 nextPPN, Boolean user) {
    pte->Present = 1;
    pte->Write = 1;
    pte->User = user;
    pte->PageWriteThrough = 0;
    pte->PageCacheDisable = 0;
    pte->Access = 0;
    pte->Dirty = 0;
    pte->Pat = 0;
    pte->Global = 0;
    pte->Reversed = 0;
    pte->NextPPN = nextPPN;
}

static void enablePaging() {
    asm volatile ("movl %cr0, %eax");
    asm volatile ("orl $0x80000000, %eax");
    asm volatile ("movl %eax, %cr0");
}

static void disablePaging() {
    asm volatile ("movl %cr0, %eax");
    asm volatile ("andl $0x7FFFFFFF, %eax");
    asm volatile ("movl %eax, %cr0");
}

static PageTableEntry* findPTE(VirtualAddress addr) {
    PageTableEntry* rootPTE = (PageTableEntry*)GetRootPageTableAddr();
    u32 firstIndex = addr >> 22;
    u32 secondIndex = (addr >> 12) & 0x3ff;

    PageTableEntry* pte = rootPTE + firstIndex;
    if (pte->Present == 0) return NULL;
    return (PageTableEntry*)GetAddressFromPPN(pte->NextPPN) + secondIndex;
}

static PageTableEntry* findPTECreate(VirtualAddress addr) {
    PageTableEntry* rootPTE = (PageTableEntry*)GetRootPageTableAddr();
    u32 firstIndex = addr >> 22;
    u32 secondIndex = (addr >> 12) & 0x3ff;

    PageTableEntry* pte = rootPTE + firstIndex;
    if (pte->Present == 0) {
        PhysicalAddress secondPageTable = AllocateOnePage(UserMode);
        initPageTableEntry(pte, GetPPNFromAddressFloor(secondPageTable), TRUE);
    }
    pte = (PageTableEntry*)GetAddressFromPPN(pte->NextPPN) + secondIndex;
    if (pte->Present == 0) {
        PhysicalAddress page = AllocateOnePage(UserMode);
        initPageTableEntry(pte, GetPPNFromAddressFloor(page), TRUE);
    }

    return pte;
}