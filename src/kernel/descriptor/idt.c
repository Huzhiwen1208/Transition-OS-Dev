#include "mod.h"

extern void* InterruptHandlerEntryTable[INTERRUPT_COUNT];
static InterruptDescriptor IDT[INTERRUPT_COUNT];
static DescriptorTablePointer IDTR;


void InitializeIDT() {
    for (i32 i = 0; i < EXCEPTION_COUNT; i++) {
        void* handler = InterruptHandlerEntryTable[i];
        IDT[i].OffsetLow = (u32)handler & 0xFFFF;
        IDT[i].Selector = 1 << 3;
        IDT[i].Reserved = 0;
        IDT[i].Type = 0b1110;
        IDT[i].Segment = 0;
        IDT[i].DPL = 0;
        IDT[i].Present = 1;
        IDT[i].OffsetHigh = ((u32)handler >> 16) & 0xFFFF;
    }
    IDTR.Base = (u32)IDT;
    IDTR.Limit = INTERRUPT_COUNT * sizeof(InterruptDescriptor) - 1;

    asm volatile("lidt %0" ::"m"(IDTR));
}