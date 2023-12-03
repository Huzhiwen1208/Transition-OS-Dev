#pragma once

#include "../common/mod.h"

// hash
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

// queue
typedef struct Queue {
    i32 Front;
    i32 Rear;
    void** Value;
    i32 MallocSize;
    char* Type;

    void (*Push)(struct Queue* self, void* value);
    void* (*Pop)(struct Queue* self);
} Queue;

// priority queue
typedef struct PriorityQueue {
    i32 Front;
    i32 Rear;
    void** Value;
    i32 MallocSize;
    char* Type;

    void (*Push)(struct PriorityQueue* self, void* value);
    void* (*Pop)(struct PriorityQueue* self);
} PriorityQueue;