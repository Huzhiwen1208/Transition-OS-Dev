#pragma once


typedef unsigned char Boolean;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef char i8;
typedef short i16;
typedef int i32;
typedef long long i64;
typedef unsigned int Size;
typedef unsigned int PID;

#define StructNotAlign __attribute__((packed));

// memory manage
typedef unsigned int PhysicalAddress;
typedef unsigned int VirtualAddress;
typedef unsigned int PhysicalPageNumber;

// variable argument
typedef char *va_list;