#pragma once

#include "type.h"

HashTable* NewMap(char* keyType, char* valueType);
void DeleteMap(HashTable** map);