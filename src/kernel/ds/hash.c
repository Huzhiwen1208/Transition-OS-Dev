#include "mod.h"

static char* typeList[] = {"u32", "i32", "i8", "u8", "u16", "i16", "string", "char*", "char *"};

/// @brief 哈希映射，对所有类型，先进行u32类型转换，再进行取余
/// @param keyType 
/// @param key 
/// @return 映射后的插入位置
static u32 hash(char* keyType, void* key);

/// @brief 当前位置是否可插入
/// @param map 
/// @param index 
/// @return 可插入？
static Boolean available(HashTable* map, u32 index);
static Boolean isKeyMatched(HashTable* map, u32 index, void* key);
static void* Get(HashTable* self, void* key);
static void Put(HashTable* self, void* key, void* value);
static void Delete(HashTable* self, void* key);


HashTable* NewMap(char* keyType, char* valueType) {
    HashTable* result = (HashTable*)Malloc(sizeof(HashTable));
    for (Size i = 0; i < HASH_TABLE_SIZE; i++) {
        result->Table[i].Available = TRUE;
        result->Table[i].key = result->Table[i].value = NULL;
    }
    result->KeyType = keyType;
    if (StringEqual(valueType, "PhysicalAddress") || StringEqual(valueType, "PCB*")) 
        valueType = "u32";
    result->ValueType = valueType;
    result->Initialized = TRUE;
    result->Get = Get;
    result->Put = Put;
    result->Delete = Delete;
    return result;
}

void DeleteMap(HashTable** map) {
    if (!(*map)->Initialized) Panic("Hash map not initialized.");
    Free(*map);
}


static u32 hash(char* keyType, void* key) {
    u32 k = -1;
    for (Size i = 0; i < sizeof(typeList) / sizeof(char*); i++) {
        if (i <= 5 && StringEqual(keyType, typeList[i])) {
            k = (u32)key;
            break;
        }

        if (i > 5 && StringEqual(keyType, typeList[i])) {
            k = StringASCIITotal((char*)key);
            break;
        }
    }
    if (k == -1) Panic("Unknown key type: {%s} in hash function.", keyType);
    return k % HASH_TABLE_SIZE;
}

static Boolean available(HashTable* map, u32 index) {
    if (!map->Initialized) Panic("Hash map not initialized.");
    return map->Table[index].Available;
}

static Boolean isKeyMatched(HashTable* map, u32 index, void* key) {
    if (!map->Initialized) Panic("Hash map not initialized.");
    for (Size i = 0; i < sizeof(typeList) / sizeof(char*); i++) {
        if (i <= 5 && StringEqual(map->KeyType, typeList[i])) {
            return (u32)key == (u32)map->Table[index].key;
        }

        if (i > 5 && StringEqual(map->KeyType, typeList[i])) {
            return StringEqual((char*)key, (char*)map->Table[index].key);
        }
    }
    Panic("Unknown key type: {%s} in hash function.", map->KeyType);
}

static void* Get(HashTable* self, void* key) {
    if (!self->Initialized) Panic("Hash map not initialized.");

    u32 index = hash(self->KeyType, key);
    while (!available(self, index)) {
        if (isKeyMatched(self, index, key)) {
            return self->Table[index].value;
        }

        index = (index + 1) % HASH_TABLE_SIZE;
        if (index == hash(self->KeyType, key)) {
            // Hash表被插满
            break;
        }
    }

    return NULL;
}

static void Put(HashTable* self, void* key, void* value) {
    if (!self->Initialized) Panic("Hash map not initialized.");

    u32 index = hash(self->KeyType, key);
    while (!available(self, index)) {
        // key存在，更新value
        if (isKeyMatched(self, index, key)) {
            self->Table[index].value = value;
            return;
        }

        index = (index + 1) % HASH_TABLE_SIZE;
        if (index == hash(self->KeyType, key)) {
            // Hash表被插满
            Panic("Hash table is full.");
        }
    }

    // key不存在，插入
    self->Table[index].key = key;
    self->Table[index].value = value;
    self->Table[index].Available = FALSE;
}

static void Delete(HashTable* self, void* key) {
    if (!self->Initialized) Panic("Hash map not initialized.");

    u32 index = hash(self->KeyType, key);
    while (!available(self, index)) {
        if (isKeyMatched(self, index, key)) {
            self->Table[index].Available = TRUE;
            self->Table[index].key = self->Table[index].value = NULL;
            return;
        }

        index = (index + 1) % HASH_TABLE_SIZE;
        if (index == hash(self->KeyType, key)) {
            // Hash表被插满，说明删除了不存在的key
            return;
        }
    }
}