#pragma once

#include "../common/mod.h"

typedef struct Value {
    void* key;
    void* value;
    Boolean Available;
} Value;

typedef struct HashTable {
    Value Table[HASH_TABLE_SIZE];
    char* KeyType;
    char* ValueType;
    Boolean Initialized;

    void* (*Get)(struct HashTable* self, void* key);
    void (*Put)(struct HashTable* self, void* key, void* value);
    void (*Delete)(struct HashTable* self, void* key);
} HashTable;