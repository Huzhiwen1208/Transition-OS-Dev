#include "mod.h"

void InitializeMemoryManager() {
    globalBuddyAllocatorInit();
    InitializeMemoryMapping();
}