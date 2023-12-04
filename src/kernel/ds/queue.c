#include "mod.h"

static Boolean Empty(Queue* self);
static Boolean Full(Queue* self);

static void Push(Queue* self, void* value);
static void* Pop(Queue* self);


Queue* NewQueue(char* type, i32 size) {
    Queue* queue = Malloc(sizeof(Queue));
    queue->Front = 0;
    queue->Rear = 0;
    queue->MallocSize = size;
    queue->Type = type;
    queue->Value = (void**)Malloc(sizeof(void*) * size);

    queue->Push = Push;
    queue->Pop = Pop;
    queue->Empty = Empty;
    return queue;
}

void DeleteQueue(Queue** self) {
    Free((*self)->Value);
    Free(*self);
}


static Boolean Empty(Queue* self) {
    return self->Front == self->Rear;
}

static Boolean Full(Queue* self) {
    return (self->Rear + 1) % self->MallocSize == self->Front;
}

static void Push(Queue* self, void* value) {
    if (Full(self)) {
        Panic("Queue is full");
    }

    self->Value[self->Rear] = value;
    self->Rear = (self->Rear + 1) % self->MallocSize;
}

static void* Pop(Queue* self) {
    if (Empty(self)) {
        return NULL;
    }

    void* value = self->Value[self->Front];
    self->Front = (self->Front + 1) % self->MallocSize;
    return value;
}