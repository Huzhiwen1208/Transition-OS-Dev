#pragma once
#include "type.h"

void InitializeDevice();
// 安装设备，此时会忽略device.ID，并返回分配的设备ID
u32 InstallDevice(Device *device);
u32 UninstallDevice(Device *device);

// 设备操作
void DeviceIoctl(u32 deviceID, u32 cmd, void *arg);
i32 DeviceRead(u32 deviceID, u64 offset, u32 size, void *buffer);
i32 DeviceWrite(u32 deviceID, u64 offset, u32 size, void *buffer);