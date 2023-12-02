#include "mod.h"

void*  InterruptHandlerList[INTERRUPT_COUNT];
static HashTable* ExceptionMessage;


static void initInterruptInfoMap();
static void defaultInterruptHandler(u32 vector);
static void initDefaultInterruptHandler();


void InitInterrupt() {
    InitializeIDT();
    initInterruptInfoMap();
    initDefaultInterruptHandler();
}

static void defaultInterruptHandler(u32 vector) {
    Error("Exception{%d}: %s", vector, ExceptionMessage->Get(ExceptionMessage, vector));
    Suspend();
}

static void initDefaultInterruptHandler() {
    for (u32 i = 0; i < INTERRUPT_COUNT; i++) {
         InterruptHandlerList[i] = defaultInterruptHandler;
    }
}

static void initInterruptInfoMap() {
    ExceptionMessage = NewMap("u32", "string");
    ExceptionMessage->Put(ExceptionMessage, 0,  "Divide Error");
    ExceptionMessage->Put(ExceptionMessage, 1,  "Debug");
    ExceptionMessage->Put(ExceptionMessage, 2,  "NMI Interrupt");
    ExceptionMessage->Put(ExceptionMessage, 3,  "Breakpoint");
    ExceptionMessage->Put(ExceptionMessage, 4,  "Overflow");
    ExceptionMessage->Put(ExceptionMessage, 5,  "BOUND Range Exceeded");
    ExceptionMessage->Put(ExceptionMessage, 6,  "Invalid Opcode");
    ExceptionMessage->Put(ExceptionMessage, 7,  "Device Not Available");
    ExceptionMessage->Put(ExceptionMessage, 8,  "Double Fault");
    ExceptionMessage->Put(ExceptionMessage, 9,  "Coprocessor Segment Overrun");
    ExceptionMessage->Put(ExceptionMessage, 10, "Invalid TSS");
    ExceptionMessage->Put(ExceptionMessage, 11, "Segment Not Present");
    ExceptionMessage->Put(ExceptionMessage, 12, "Stack-Segment Fault");
    ExceptionMessage->Put(ExceptionMessage, 13, "General Protection");
    ExceptionMessage->Put(ExceptionMessage, 14, "Page Fault");
    ExceptionMessage->Put(ExceptionMessage, 15, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 16, "x87 FPU Floating-Point Error");
    ExceptionMessage->Put(ExceptionMessage, 17, "Alignment Check");
    ExceptionMessage->Put(ExceptionMessage, 18, "Machine Check");
    ExceptionMessage->Put(ExceptionMessage, 19, "SIMD Floating-Point Exception");
    ExceptionMessage->Put(ExceptionMessage, 20, "Virtualization Exception");
    ExceptionMessage->Put(ExceptionMessage, 21, "Control Protection Exception");
    ExceptionMessage->Put(ExceptionMessage, 22, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 23, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 24, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 25, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 26, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 27, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 28, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 29, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 30, "Reserved");
    ExceptionMessage->Put(ExceptionMessage, 31, "Reserved");
}