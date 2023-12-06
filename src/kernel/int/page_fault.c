#include "mod.h"

static void PageFaultHandler(u32 vector) {
    Assert(vector == 0xe);

    VirtualAddress addr = GetCR2();
    // Warn("Page fault at 0x%x\n", addr);
    MapPage(addr);
}

void InitializePageFaultHandler() {
    SetInterruptHandler(0xe, PageFaultHandler);
}