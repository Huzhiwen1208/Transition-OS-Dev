#include "mod.h"

void DebugHandle(const char *file, i32 line, const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Vsprintf(buf, fmt, args);
    PrintWithColor(CYAN, "[DEBUG] %s:%d %s\n", file, line, buf);
}

void TraceHandle(const char *file, i32 line, const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Vsprintf(buf, fmt, args);
    PrintWithColor(GRAY, "[TRACE] %s:%d %s\n", file, line, buf);
}

void InfoHandle(const char *file, i32 line, const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Vsprintf(buf, fmt, args);
    PrintWithColor(GREEN, "[INFO] %s:%d %s\n", file, line, buf);
}

void WarnHandle(const char *file, i32 line, const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Vsprintf(buf, fmt, args);
    PrintWithColor(LIGHT_YELLOW, "[WARN] %s:%d %s\n", file, line, buf);
}

void ErrorHandle(const char *file, i32 line, const char *fmt, ...) {
    char buf[1024];
    va_list args;
    va_start(args, fmt);
    Vsprintf(buf, fmt, args);
    PrintWithColor(RED, "[ERROR] %s:%d %s\n", file, line, buf);
}