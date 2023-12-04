#pragma once

#include "type.h"
#include "../int/type.h"

// log
void DebugHandle(const char *file, i32 line, const char *fmt, ...);
void TraceHandle(const char *file, i32 line, const char *fmt, ...);
void InfoHandle(const char *file, i32 line, const char *fmt, ...);
void WarnHandle(const char *file, i32 line, const char *fmt, ...);
void ErrorHandle(const char *file, i32 line, const char *fmt, ...);

#define Debug(fmt, ...) DebugHandle(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Trace(fmt, ...) TraceHandle(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Info(fmt, ...) InfoHandle(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Warn(fmt, ...) WarnHandle(__FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define Error(fmt, ...) ErrorHandle(__FILE__, __LINE__, fmt, ##__VA_ARGS__)

// memory

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

// string
Size StringLength(const char* str);
Boolean StringEqual(const char* str1, const char* str2);
u32 StringToInt32(const char* str);
u32 StringASCIITotal(const char* str);
Boolean IsDigit(char c);
Boolean IsAlpha(char c);
Boolean IsUpper(char c);
Boolean IsLower(char c);

// print
Size Printf(const char *fmt, ...);
Size Println(const char *fmt, ...);
Size Sprintf(char *buf, const char *fmt, ...);
Size PrintWithColor(ConsoleColor color, const char *fmt, ...);
Size Vsprintf(char *buf, const char *fmt, va_list args);
void Panic(const char *fmt, ...);

// util
void Suspend();

void AssertFailed(char *exp, char *file, char *base, i32 line);

#define Assert(exp) \
    if (!(exp)) AssertFailed(#exp, __FILE__, __BASE_FILE__, __LINE__)


// SystemCall
/// @brief 测试系统调用
void Test();
/// @brief 获得当前系统开启时钟至今的时间
void GetTimeValue(TimeValue* tv);
/// @brief 让出CPU
void Yield();