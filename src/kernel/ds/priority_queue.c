#include "mod.h"

static Boolean Empty(PriorityQueue* self);
static Boolean Full(PriorityQueue* self);

static void Push(PriorityQueue* self, void* value);
static void* Pop(PriorityQueue* self);

PriorityQueue* NewPriorityQueue(char* type, i32 size) {
    PriorityQueue* queue = Malloc(sizeof(PriorityQueue));
    queue->Front = 0;
    queue->Rear = 0;
    queue->MallocSize = size;
    queue->Type = type;
    queue->Value = (void**)Malloc(sizeof(void*) * size);

    queue->Push = Push;
    queue->Pop = Pop;
    return queue;
}

void DeletePriorityQueue(PriorityQueue** self) {
    Free((*self)->Value);
    Free(*self);
}


static Boolean Empty(PriorityQueue* self) {
    return self->Front == self->Rear;
}

static Boolean Full(PriorityQueue* self) {
    return (self->Rear + 1) % self->MallocSize == self->Front;
}

static void Push(PriorityQueue* self, void* value) {
    if (Full(self)) {
        Panic("PriorityQueue is full");
    }

    if (
        StringEqual(self->Type, "u32") || 
        StringEqual(self->Type, "i32") ||
        StringEqual(self->Type, "i8") ||
        StringEqual(self->Type, "u8") ||
        StringEqual(self->Type, "u16") ||
        StringEqual(self->Type, "i16")
    ) {
        u32 v = (u32)value;
        i32 i = self->Front;
        while (i != self->Rear && (u32)self->Value[i] < v) {
            i = (i + 1) % self->MallocSize;
        }

        i32 j = self->Rear;
        while (j != i) {
            self->Value[j] = self->Value[(j - 1 + self->MallocSize) % self->MallocSize];
            j = (j - 1 + self->MallocSize) % self->MallocSize;
        }

        self->Value[i] = value;
        self->Rear = (self->Rear + 1) % self->MallocSize;
    } else {
        Panic("PriorityQueue type error");
    }
}

static void* Pop(PriorityQueue* self) {
    if (Empty(self)) {
        return NULL;
    }

    void* value = self->Value[self->Front];
    self->Front = (self->Front + 1) % self->MallocSize;
    return value;
}