#pragma once

#include "../common/common.h"

/// @brief copy src to dest, byte by byte
/// @param dest: destination address
/// @param src: source address
/// @param size
void MemoryCopy(void* dest, const void* src, Size size);

/// @brief clear memory of ptr, byte by byte
/// @param ptr 
/// @param size 
void MemoryFree(void* ptr, Size size);

/// @brief set memory of ptr to value, byte by byte
/// @param ptr
/// @param value
/// @param size
void MemorySet(void* ptr, u8 value, Size size);

Size StringLength(const char* str);