#include "mod.h"

// global device list
static Device* devices[DEVICE_COUNT];


static Device* getDeviceByID(u32 deviceID);


void InitializeDevice() {
    for (u32 i = 0; i < DEVICE_COUNT; i++) {
        devices[i] = NULL;
    }
    InitializeIdeDisk();
}

u32 InstallDevice(Device* device) {
    if (device == NULL) {
        return -1;
    }

    for (u32 i = 0; i < DEVICE_COUNT; i++) {
        if (devices[i] == NULL) {
            device->DeviceID = i;
            devices[i] = device;
            return i;
        }
    }

    Panic("Device list is full");
}

u32 UninstallDevice(Device* device) {
    if (device == NULL) {
        return -1;
    }

    if (device->DeviceID >= DEVICE_COUNT) {
        Panic("DeviceID is too large id: %d", device->DeviceID);
    }

    u32 id = device->DeviceID;
    Free(device);
    devices[id] = NULL;
    return id;
}

void DeviceIoctl(u32 deviceID, u32 cmd, void *arg) {
    Device *device = getDeviceByID(deviceID);
    if (device == NULL) {
        return -1;
    }

    if (device->Ioctl == NULL) {
        return -1;
    }

    device->Ioctl(device->DevicePtr, cmd, arg);
}

i32 DeviceRead(u32 deviceID, u64 offset, u32 size, void *buffer) {
    Device *device = getDeviceByID(deviceID);
    if (device == NULL) {
        return -1;
    }

    if (device->Read == NULL) {
        return -1;
    }

    return device->Read(device->DevicePtr, offset, size, buffer);

}

i32 DeviceWrite(u32 deviceID, u64 offset, u32 size, void *buffer) {
    Device *device = getDeviceByID(deviceID);
    if (device == NULL) {
        return -1;
    }

    if (device->Write == NULL) {
        return -1;
    }

    return device->Write(device->DevicePtr, offset, size, buffer);
}


static Device* getDeviceByID(u32 deviceID) {
    if (deviceID >= DEVICE_COUNT) {
        return NULL;
    }

    return devices[deviceID];
}