#pragma once

#include "../common/mod.h"
#include "../ds/type.h"

typedef struct IdeDisk {
    char Name[NAME_LENGTH]; // 磁盘名称
    struct IdeController* Controller; // 磁盘所属IDE控制器
    u32 Selector; // 磁盘的选择器
    Boolean IsMaster; // 是否是主盘
    Size TotalSector; // 该磁盘总扇区数
    u32 C, H, S; // 柱面数、磁头数、扇区数
} IdeDisk;

typedef struct IdeController {
    char Name[NAME_LENGTH]; // IDE控制器名称
    ReentrantLock* rlock; // 可重入互斥锁
    u32 IoBase; // IO基址
    IdeDisk DiskList[DISK_COUNT_PER_CONTROLLER]; // IDE控制器下的磁盘列表
    IdeDisk* CurrentDisk; // 当前磁盘
    u8 ControlByte; // 控制字节
} IdeController;

struct IdeParam {
    u16 Config;
    u16 Cylinders;
    u16 unused_;
    u16 Heads;
    u16 unused_1[5 - 3];
    u16 Sectors;
    u16 unused_2[9 - 6];
    u8 Serial[20];
    u16 unused_3[22 - 19];
    u8 Firmware[8];
    u8 Model[40];
    u8 DrqSectors;
    u8 unused_4[3];
    u16 Capabilities;
    u16 unused_5[59 - 49];
    u32 TotalSectors;
    u16 unused_6;
    u16 MDMAMode;
    u8 unused_7;
    u8 PioMode;
    u16 unused_8[79 - 64];
    u16 MajorVersion;
    u16 MinorVersion;
    u16 CommmandSets[87 - 81];
    u16 unused_9[118 - 87];
    u16 SupportSettings;
    u16 EnableSettings;
    u16 unused_10[221 - 120];
    u16 TransportMajor;
    u16 TransportMinor;
    u16 unused_11[254 - 223];
    u16 Integrity;
} StructNotAlign;
typedef struct IdeParam IdeParam;