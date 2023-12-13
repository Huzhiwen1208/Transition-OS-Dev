#pragma once

#include "../common/mod.h"
#include "../ds/type.h"

typedef enum DeviceType {
    DEVICE_TYPE_UNKNOWN, // 未知设备
    DEVICE_TYPE_BLOCK,  // 块设备
    DEVICE_TYPE_CHAR    // 字符设备 我不打算将控制台和键盘注册为字符设备，当然按逻辑是需要这么做的
} DeviceType;

typedef enum DeviceSubType {
    DEVICE_SUB_TYPE_DISK // 磁盘
} DeviceSubType;

typedef struct Device {
    char Name[NAME_LENGTH];
    DeviceType Type;
    DeviceSubType SubType;
    u32 DeviceID; // 设备ID
    void* DevicePtr; // 设备指针

    /// @brief 读取设备
    /// @param device 设备
    /// @param offset 偏移量，在磁盘中表示起始扇区号
    /// @param size 读取的扇区数
    /// @param buffer 读取的数据缓冲区，读取结果写入此缓冲区
    /// @return 读取的扇区数
    i32 (*Read)(struct Device* device, u32 offset, u32 size, void* buffer);

    /// @brief 参数同上
    i32 (*Write)(struct Device* device, u32 offset, u32 size, void* buffer);

    /// @brief 设备控制
    /// @param device 设备
    /// @param cmd 控制命令
    /// @param args 参数
    void (*Ioctl)(struct Device* device, i32 cmd, void* args);
} Device;