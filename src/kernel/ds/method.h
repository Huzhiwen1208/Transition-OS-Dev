#pragma once

#include "type.h"

HashTable* NewMap(char* keyType, char* valueType);
void DeleteMap(HashTable** map);

Queue* NewQueue(char* type, i32 size);
void DeleteQueue(Queue** self);

PriorityQueue* NewPriorityQueue(char* type, i32 size);
void DeletePriorityQueue(PriorityQueue** self);

ReentrantLock* NewReentrantLock();