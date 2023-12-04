/*
    ref: https://www.scs.stanford.edu/10wi-cs140/pintos/specs/8254.pdf
*/

#include "mod.h"

// global variables
/// @brief 系统从开启时钟至今的时间片(10ms)数
u64 jiffies;
static u32 ClockVector = 0x20;


// static methods

static void clockInterruptHandler(u32 vector);
static void initPIT();
static u16 readCounter0();


// public methods

void InitializeClock() {
    initPIT();
    SetInterruptHandler(ClockVector, clockInterruptHandler);
    SetInterrupt(ClockVector);
    jiffies = 0;
    asm volatile("sti");
}

// the max usec of 32bits is 4294967295, which is about 1.19 hours
// should optimize this function to support larger time, reason: unsupported u64 division
void GetTime(TimeValue* tv) {
    u32 usec = jiffies * JeffyMS * 1000 + (ClockMaxTicks - readCounter0()) * (MSPerTick * 1000 * 1000);
    tv->Second = usec / (1000 * 1000);
    tv->MicroSecond = usec % (1000 * 1000);
}

u32 GetTimeMS() {
    return jiffies * JeffyMS + (ClockMaxTicks - readCounter0()) * (MSPerTick * 1000);
}

// static methods implementation

static void clockInterruptHandler(u32 vector) {
    Assert(vector == ClockVector);
    jiffies++;
    OuteralInterruptCompleted(vector);
    // Info("clock interrupt invoked: %d", jiffies);
    Schedule();
}

static void initPIT() {
    WriteByte(ControlWordRegister, 0b00110100); // 00, 11, 010, 0
    WriteByte(ClockCounter0, ClockMaxTicks & 0xFF); // low byte
    WriteByte(ClockCounter0, ClockMaxTicks >> 8); // high byte
}

static u16 readCounter0() {
    WriteByte(ControlWordRegister, 0b00110100);
    u8 low = ReadByte(ClockCounter0);
    u8 high = ReadByte(ClockCounter0);
    return (high << 8) | low;
}