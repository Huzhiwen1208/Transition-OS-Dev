#pragma once

#include "../common/mod.h"

typedef struct BitMap {
    u32 Blocks; // 位图所占总块数
    u32 StartBlockID; // 位图开始的块号
} BitMap;

typedef struct SuperBlock {
    u32 Magic; // 魔数
    u32 TotalBlocks; // 总块数
    u32 InodeBitMapBlocks; // inode位图所占块数
    u32 InodeAreaBlocks; // inode区域所占块数
    u32 DataBitMapBlocks; // 数据位图所占块数
    u32 DataAreaBlocks; // 数据区域所占块数
} SuperBlock;

typedef enum FileType {
    FT_UNKNOWN,
    FT_FILE,
    FT_DIRECTORY
} FileType;

typedef struct DiskInode {
    FileType Type; // 文件类型
    u32 Size; // 文件大小
    u32 RefCount; // 引用计数
    u32 DirectBlock[INODE_DIRECT_BLOCKS]; // 直接块, 凑齐128字节
    u32 Indirect1; // 一级间接
    u32 Indirect2; // 二级间接
} DiskInode;

typedef struct EasyFS {
    Device* Device; // 文件系统承载的设备
    BitMap* InodeBitMap; // inode位图
    BitMap* DataBitMap; // 数据位图
    u32 InodeAreaStartBlockID; // inode区域开始的块号
    u32 DataAreaStartBlockID; // 数据区域开始的块号

    ReentrantLock* rlock; // 可重入锁
} EasyFS;

typedef struct DirectoryEntry {
    u32 InodeID; // inode号
    char Name[28]; // 文件名
} DirectoryEntry;