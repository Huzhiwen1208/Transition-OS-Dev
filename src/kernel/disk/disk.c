#include "mod.h"

static IdeController ideControllers[IDE_CONTROLLER_COUNT];


static void diskWait(IdeController* ctrl);
static void sectorSelect(IdeDisk* disk, u32 lba, u32 count);
/// @brief IDE磁盘读，返回读取的字节数
/// @param disk 磁盘, device
/// @param lba 逻辑块地址, offset
/// @param count 读取的逻辑块数, size
/// @param buffer 缓冲区, buffer
/// @return i32 读取的字节数
static i32 diskRead(IdeDisk* disk, u32 lba, u32 count, void* buffer);
/// @brief IDE磁盘写，返回写入的字节数
/// @param disk 磁盘, device
/// @param lba 逻辑块地址, offset
/// @param count 读取的逻辑块数, size
/// @param buffer 缓冲区, buffer
/// @return i32 读取的字节数
static i32 diskWrite(IdeDisk* disk, u32 lba, u32 count, void* buffer);


void InitializeIdeDisk() {
    for (u32 i = 0; i < IDE_CONTROLLER_COUNT; i++) {
        Sprintf(ideControllers[i].Name, "ide-%d", i); // 对每个控制器起名：ide-x
        ideControllers[i].rlock = NewReentrantLock(); // 初始化可重入互斥锁
        ideControllers[i].CurrentDisk = NULL; // 当前磁盘为空
        ideControllers[i].IoBase = i == 0 ? IdeIobaseMaster : IdeIobaseSlave; // 设置IO基地址
        ideControllers[i].ControlByte = ReadByte(ideControllers[i].IoBase + IdeControlRegister); // 读取控制字节

        // 读取磁盘信息
        for (u32 j = 0; j < DISK_COUNT_PER_CONTROLLER; j++) {
            IdeDisk* disk = &ideControllers[i].DiskList[j];
            Sprintf(disk->Name, "ide-%d-%d", i, j); // 对每个磁盘起名：ide-x-y
            disk->Controller = &ideControllers[i]; // 设置磁盘的控制器
            disk->IsMaster = j == 0 ? TRUE : FALSE; // 设置是否是主盘
            disk->Selector = disk->IsMaster ? IdeLbaMaster : IdeLbaSlave; // 设置选择器


            WriteByte(ideControllers[i].IoBase + IdeSelectorRegister, disk->Selector);
            ideControllers[i].CurrentDisk = disk;

            /*
            * 识别磁盘：
            * 1. 发送识别命令
            * 2. 读取数据寄存器的数据并写入ide param
            * 3. 检验总扇区数是否为0，如果为0则磁盘不存在
            * 4. 读取参数中的数据，填充磁盘信息
            * 5. 安装磁盘到设备表
            */
            WriteByte(ideControllers[i].IoBase + IdeCommandRegister, IdeCommandIdentify); // 发送识别命令
            
            IdeParam* param = (IdeParam*)Malloc(sizeof(IdeParam));
            for (u32 k = 0; k < 256; k++) {
                u16 data = ReadWord(ideControllers[i].IoBase + IdeDataRegister); // 从数据寄存器读取数据
                ((u16*)param)[k] = data; // 将数据写入buffer
            }

            if (param->TotalSectors == 0) { // 检验总扇区数是否为0
                continue;
            }

            disk->TotalSector = param->TotalSectors;
            disk->C = param->Cylinders;
            disk->H = param->Heads;
            disk->S = param->Sectors;
            Free(param);
            
            // Info("find disk{%s}, total sectors: %d", disk->Name, disk->TotalSector);
            Device* device = (Device*)Malloc(sizeof(Device));
            MemoryCopy(device->Name, disk->Name, NAME_LENGTH);
            device->DevicePtr = disk;
            device->Type = DEVICE_TYPE_BLOCK;
            device->SubType = DEVICE_SUB_TYPE_DISK;
            device->Ioctl = NULL;
            device->Read = (void*)diskRead;
            device->Write = (void*)diskWrite;
            InstallDevice(device); // 安装磁盘到设备表
        }
    }
}

// static methods implementation

static void diskWait(IdeController* ctrl) {
    while (TRUE) {
        u8 status = ReadByte(ctrl->IoBase + IdeAlternateStatusRegister);
        if (status & IdeStatusErr) Panic("ide error in disk wait\n"); // 如果出错，panic
        if (status & IdeStatusBusy) continue; // 如果忙，继续等待
        if (status & IdeStatusDrdy) break; // 如果就绪，跳出循环
    }
}

static void sectorSelect(IdeDisk* disk, u32 lba, u32 count) {
    IdeController* ctrl = disk->Controller;
    WriteByte(ctrl->IoBase + IdeSectorCountRegister, count); // 设置扇区数
    WriteByte(ctrl->IoBase + IdeLbaLow, lba & 0xFF); // 设置LBA低位
    WriteByte(ctrl->IoBase + IdeLbaMid, (lba >> 8) & 0xFF); // 设置LBA中位
    WriteByte(ctrl->IoBase + IdeLbaHigh, (lba >> 16) & 0xFF); // 设置LBA高位
    WriteByte(ctrl->IoBase + IdeSelectorRegister, disk->Selector | ((lba >> 24) & 0xF)); // 设置选择器
}

static i32 diskRead(IdeDisk* disk, u32 lba, u32 count, void* buffer) {
    if (!count) return 0;
    IdeController* ctrl = disk->Controller;
    ctrl->rlock->Lock(ctrl->rlock); // 上锁

    // 磁盘选择
    WriteByte(ctrl->IoBase + IdeSelectorRegister, disk->Selector); // 选择磁盘
    ctrl->CurrentDisk = disk; // 设置当前磁盘

    // 磁盘等待，程序IO等待
    diskWait(ctrl);

    // 扇区选择
    sectorSelect(disk, lba, count);

    // 发出读命令=读
    WriteByte(ctrl->IoBase + IdeCommandRegister, IdeCommandRead);
    for (u32 i = 0; i < count; i++) {
        diskWait(ctrl); // 等待磁盘
        u16* buffer16 = (u16*)buffer;
        for (u32 j = 0; j < 256; j++) {
            u16 data = ReadWord(ctrl->IoBase + IdeDataRegister); // 从数据寄存器读取数据
            buffer16[j] = data;
        }
        buffer16 += 256;
    }

    // 解锁
    ctrl->rlock->Unlock(ctrl->rlock);
    return count * 512;
}

static i32 diskWrite(IdeDisk* disk, u32 lba, u32 count, void* buffer) {
    if (!count) return 0;
    IdeController* ctrl = disk->Controller;
    ctrl->rlock->Lock(ctrl->rlock); // 上锁

    // 磁盘选择
    WriteByte(ctrl->IoBase + IdeSelectorRegister, disk->Selector); // 选择磁盘
    ctrl->CurrentDisk = disk; // 设置当前磁盘

    // 磁盘等待，程序IO等待
    diskWait(ctrl);

    // 扇区选择
    sectorSelect(disk, lba, count);

    // 发出写命令=写
    WriteByte(ctrl->IoBase + IdeCommandRegister, IdeCommandWrite);
    for (u32 i = 0; i < count; i++) {
        diskWait(ctrl); // 等待磁盘
        u16* buffer16 = (u16*)buffer;
        for (u32 j = 0; j < 256; j++) {
            WriteWord(ctrl->IoBase + IdeDataRegister, buffer16[j]); // 将数据写入数据寄存器
        }
        buffer16 += 256;
    }

    // 解锁
    ctrl->rlock->Unlock(ctrl->rlock);
    return count * 512;
}