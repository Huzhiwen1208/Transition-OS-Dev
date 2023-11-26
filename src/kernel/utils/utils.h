#pragma once

#include "../common/common.h"

void AssertFailed(char *exp, char *file, char *base, i32 line);

#define Assert(exp) \
    if (!(exp)) AssertFailed(#exp, __FILE__, __BASE_FILE__, __LINE__)

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
Boolean IsDigit(char c);
Boolean IsAlpha(char c);
Boolean IsUpper(char c);
Boolean IsLower(char c);

Size Printf(const char *fmt, ...);
Size Println(const char *fmt, ...);
Size Sprintf(char *buf, const char *fmt, ...);
Size PrintWithColor(ConsoleColor color, const char *fmt, ...);
void Panic(const char *fmt, ...);

void Suspend();