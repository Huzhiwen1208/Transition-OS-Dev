#include "mod.h"

static GlobalDescriptor GDT[SEGMENT_COUNT];
static DescriptorTablePointer GDTR;
static TaskStateSegment TSS;

/*
    global_descriptor_table:
        dd 0
        dd 0 ; the first descriptor should be null
    code_segment:
        ; mem base: 0
        ; segment size: 2^20 * granularity(4kb) = 4GB
        dw 0xffff
        dw 0
        db 0
        db 0b1001_1010; type= 1CRA(1010), segment=1, DPL=00, present=1
        db 0xdf       ; granularity=1, big=1, long_mode=0, available=1, limit_high=1111
        db 0
    data_segment:
        ; mem base: 0
        ; segment size: 2^20 * granularity(4kb) = 4GB
        dw 0xffff
        dw 0
        db 0
        db 0b1001_0010; type= 0DWA(0010), segment=1, DPL=00, present=1
        db 0xdf       ; granularity=1, big=1, long_mode=0, available=1, limit_high=1111
        db 0
    global_descriptor_table_end: 
*/
void InitializeGDT() {
    GDT[0].LimitLow = 0;
    GDT[0].BaseLow = 0;
    GDT[0].Type = 0;
    GDT[0].Segment = 0;
    GDT[0].DPL = 0;
    GDT[0].Present = 0;
    GDT[0].LimitHigh = 0;
    GDT[0].Available = 0;
    GDT[0].LongMode = 0;
    GDT[0].DefaultOperationSize = 0;
    GDT[0].Granularity = 0;
    GDT[0].BaseHigh = 0;

    // Kernel Code
    GDT[1].LimitLow = 0xffff;
    GDT[1].BaseLow = 0;
    GDT[1].Type = 0b1010;
    GDT[1].Segment = 1;
    GDT[1].DPL = 0;
    GDT[1].Present = 1;
    GDT[1].LimitHigh = 0b1111;
    GDT[1].Available = 1;
    GDT[1].LongMode = 0;
    GDT[1].DefaultOperationSize = 1;
    GDT[1].Granularity = 1;
    GDT[1].BaseHigh = 0;

    // Kernel Data
    GDT[2].LimitLow = 0xffff;
    GDT[2].BaseLow = 0;
    GDT[2].Type = 0b0010;
    GDT[2].Segment = 1;
    GDT[2].DPL = 0;
    GDT[2].Present = 1;
    GDT[2].LimitHigh = 0b1111;
    GDT[2].Available = 1;
    GDT[2].LongMode = 0;
    GDT[2].DefaultOperationSize = 1;
    GDT[2].Granularity = 1;
    GDT[2].BaseHigh = 0;

    // Tss
    MemoryFree((void*)&TSS, sizeof(TSS));
    TSS.SS0 = KernelDataSegmentSelector;
    TSS.IOBase = sizeof(TSS);

    GDT[3].LimitLow = (sizeof(TSS) - 1) & 0xffff;
    GDT[3].BaseLow = (u32)&TSS & 0xffffff; // 24 bits
    GDT[3].Type = 0b1001; // TSS(Available 32-bit TSS)
    GDT[3].Segment = 0; // System Segment
    GDT[3].DPL = 0;
    GDT[3].Present = 1;
    GDT[3].LimitHigh = ((sizeof(TSS) - 1) >> 16) & 0xf; // 4 bits
    GDT[3].Available = 0;
    GDT[3].LongMode = 0;
    GDT[3].DefaultOperationSize = 0; // 16-bit
    GDT[3].Granularity = 0; // 1 byte
    GDT[3].BaseHigh = ((u32)&TSS >> 24) & 0xff; // 8 bits

    // User Code
    GDT[4].LimitLow = 0xffff;
    GDT[4].BaseLow = 0;
    GDT[4].Type = 0b1010;
    GDT[4].Segment = 1;
    GDT[4].DPL = 3;
    GDT[4].Present = 1;
    GDT[4].LimitHigh = 0b1111;
    GDT[4].Available = 1;
    GDT[4].LongMode = 0;
    GDT[4].DefaultOperationSize = 1;
    GDT[4].Granularity = 1;
    GDT[4].BaseHigh = 0;

    // User Data
    GDT[5].LimitLow = 0xffff;
    GDT[5].BaseLow = 0;
    GDT[5].Type = 0b0010;
    GDT[5].Segment = 1;
    GDT[5].DPL = 3;
    GDT[5].Present = 1;
    GDT[5].LimitHigh = 0b1111;
    GDT[5].Available = 1;
    GDT[5].LongMode = 0;
    GDT[5].DefaultOperationSize = 1;
    GDT[5].Granularity = 1;
    GDT[5].BaseHigh = 0;

    GDTR.Limit = sizeof(GDT) - 1;
    GDTR.Base = (u32)GDT;

    asm volatile("lgdt %0" ::"m"(GDTR));
}

void SetTSSEsp0(u32 esp0) {
    TSS.ESP0 = esp0;
}