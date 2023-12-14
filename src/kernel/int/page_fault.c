#include "mod.h"

static void PageFaultHandler(u32 vector) {
    Assert(vector == 0xe);

    VirtualAddress addr = GetCR2();
    Assert(addr >= 0x400000); // 确保不是内核地址缺页，因为内核地址都被恒等映射了
    MapPage(addr);
}

void InitializePageFaultHandler() {
    SetInterruptHandler(0xe, PageFaultHandler);
}