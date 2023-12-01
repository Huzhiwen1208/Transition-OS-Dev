#include "console/console.h"
#include "lib/lib.h"
#include "ds/ds.h"
#include "memory/memory.h"

void TransitionMain() {
    InitConsole();
    InitMemoryManager();
    Printf("Welcome to Transition OS!");

    HashTable* map = NewMap("string", "string");
    char* v =  (char*)map->Get(map, "hello");

    map->Put(map, "hello", "world");
    map->Put(map, "hello1", "world2");
    map->Put(map, "hello2", "world3");
    v = map->Get(map, "hello2");
    map->Put(map, "hello2", "world4");
    v = map->Get(map, "hello2");
    map->Delete(map, "hello2");

    v = map->Get(map, "hello2");

    DeleteMap(&map);
}