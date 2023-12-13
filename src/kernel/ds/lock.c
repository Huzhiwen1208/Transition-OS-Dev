#include "mod.h"

// static methods declaration

static void ReentrantLock_Lock(ReentrantLock* self);
static void ReentrantLock_Unlock(ReentrantLock* self);

// public methods

ReentrantLock* NewReentrantLock() {
    ReentrantLock* lock = (ReentrantLock*)Malloc(sizeof(ReentrantLock));
    lock->Owner = NULL;
    lock->Initialized = TRUE;
    lock->EntrantCount = 0;
    lock->Locked = FALSE;
    lock->WaitQueue = NewQueue("PCB*", MAX_PROCESS_COUNT);

    lock->Lock = ReentrantLock_Lock;
    lock->Unlock = ReentrantLock_Unlock;
    return lock;
}

// static methods implementation

static void ReentrantLock_Lock(ReentrantLock* self) {
    if (self->Initialized == FALSE) {
        return;
    }

    PCB* current = GetCurrentProcess();
    if (current == NULL) { // 如果当前没有进程，直接返回
        return;
    }

    if (self->Owner == current) {
        self->EntrantCount++;
        return;
    }

    while (self->Locked == TRUE) {
        self->WaitQueue->Push(self->WaitQueue, current);
        current->Status = PROCESS_STATE_BLOCKED;
        Schedule();
    }

    self->Locked = TRUE;
    self->Owner = current;
    self->EntrantCount = 1;
}

static void ReentrantLock_Unlock(ReentrantLock* self) {
    if (self->Initialized == FALSE) {
        return;
    }

    PCB* current = GetCurrentProcess();
    if (current == NULL) { // 如果当前没有进程，直接返回
        return;
    }
    
    Assert((PCB*)self->Owner == current);

    self->EntrantCount--;
    if (self->EntrantCount == 0) {
        self->Locked = FALSE;
        self->Owner = NULL;
        if (self->WaitQueue->Empty(self->WaitQueue) == FALSE) {
            PCB* next = (PCB*)self->WaitQueue->Pop(self->WaitQueue);
            next->Status = PROCESS_STATE_RUNNABLE;
            AddProcess(next);
        }
    }
}