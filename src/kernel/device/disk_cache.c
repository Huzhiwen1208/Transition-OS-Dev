#include "mod.h"

static HashTable* diskCacheMap; // key: blockID(lba) value: PhysicalAddress
static PhysicalAddress currentCachePtr; // 当前空块缓存地址，每次使用后，+= 512

void InitializeDiskCache() {
    diskCacheMap = NewMap("u32", "PhysicalAddress");
    currentCachePtr = BlockCacheStart;
}

void DiskCacheRead(u32 blockID, void *buffer) {
    PhysicalAddress address = (PhysicalAddress)diskCacheMap->Get(diskCacheMap, blockID);
    // 如果缓存中没有，就从磁盘中读取, 并将读取的数据放入缓存中
    if (address == NULL) {
        DeviceRead(1, blockID, 1, buffer);
        MemoryCopy((void*)currentCachePtr, buffer, 512);
        diskCacheMap->Put(diskCacheMap, blockID, currentCachePtr);  // 更新缓存

        currentCachePtr += 512;
        if (currentCachePtr >= BlockCacheEnd) {
            currentCachePtr = BlockCacheStart; // 循环使用缓存，FIFO的淘汰策略
        }
        return;
    }

    // 如果缓存中有，就从缓存中读取
    MemoryCopy(buffer, (void*)address, 512);
}

// 全写法，写入缓存，紧接着再写入磁盘
void DiskCacheWrite(u32 blockID, void *buffer) {
    PhysicalAddress address = (PhysicalAddress)diskCacheMap->Get(diskCacheMap, blockID);
    if (address) {
        // 缓存存在，写入缓存
        MemoryCopy((void*)address, buffer, 512);
    }
    
    // 写入磁盘
    DeviceWrite(1, blockID, 1, buffer);
}