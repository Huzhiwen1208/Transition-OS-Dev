#include "mod.h"

void MemoryCopy(void* dest, const void* src, Size size) {
    u8* dest8 = (u8*)dest;
    u8* src8 = (u8*)src;
    while (size--) {
        *dest8++ = *src8++;
    }
}

void MemoryFree(void* ptr, Size size) {
    u8* ptr8 = (u8*)ptr;
    while (size--) {
        *ptr8++ = 0;
    }
}

void MemorySet(void* ptr, u8 value, Size size) {
    u8* ptr8 = (u8*)ptr;
    while (size--) {
        *ptr8++ = value;
    }
}