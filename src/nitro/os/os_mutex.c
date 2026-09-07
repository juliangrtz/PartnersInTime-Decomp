#include <nitro/os_sync.h>

OsMutex *func_02039d28(OsThread *thread);
void func_02039d50(OsThread *thread, OsMutex *mutex);
void func_02039d74(OsThread *thread, OsMutex *mutex);

void func_02039f58(OsMutex *mutex) {
    mutex->queue = 0;
    mutex->owner = 0;
    mutex->count = 0;
}

void func_02039ecc(OsMutex *mutex) {
    u32 state = OS_DisableInterrupts();
    OsThread *thread = OSi_ThreadInfo.current;
    for (;;) {
        if (!mutex->owner) {
            mutex->owner = thread;
            mutex->count++;
            func_02039d74(thread, mutex);
            break;
        }
        if (mutex->owner == thread) {
            mutex->count++;
            break;
        }
        thread->waiting_mutex = mutex;
        OS_SleepThread(&mutex->queue);
        thread->waiting_mutex = 0;
    }
    OS_RestoreInterrupts(state);
}

void func_02039e5c(OsMutex *mutex) {
    u32 state = OS_DisableInterrupts();
    OsThread *thread = OSi_ThreadInfo.current;
    if (mutex->owner == thread) {
        --mutex->count;
        if (mutex->count == 0) {
            func_02039d50(thread, mutex);
            mutex->owner = 0;
            OS_WakeupThread(&mutex->queue);
        }
    }
    OS_RestoreInterrupts(state);
}

void func_02039e0c(OsThread *thread) {
    while (thread->mutex_head) {
        OsMutex *mutex = func_02039d28(thread);
        mutex->count = 0;
        mutex->owner = 0;
        OS_WakeupThread(&mutex->queue);
    }
}

int func_02039d98(OsMutex *mutex) {
    u32 state = OS_DisableInterrupts();
    OsThread *thread = OSi_ThreadInfo.current;
    int acquired;
    if (!mutex->owner) {
        mutex->owner = thread;
        mutex->count++;
        func_02039d74(thread, mutex);
        acquired = 1;
    } else if (mutex->owner == thread) {
        mutex->count++;
        acquired = 1;
    } else {
        acquired = 0;
    }
    OS_RestoreInterrupts(state);
    return acquired;
}

void func_02039d74(OsThread *thread, OsMutex *mutex) {
    OsMutex *last = thread->mutex_tail;
    if (!last) thread->mutex_head = mutex;
    else last->next = mutex;
    mutex->previous = last;
    mutex->next = 0;
    thread->mutex_tail = mutex;
}

void func_02039d50(OsThread *thread, OsMutex *mutex) {
    OsMutex *next = mutex->next;
    OsMutex *previous = mutex->previous;
    if (!next) thread->mutex_tail = previous;
    else next->previous = previous;
    if (!previous) thread->mutex_head = next;
    else previous->next = next;
}

OsMutex *func_02039d28(OsThread *thread) {
    OsMutex *mutex = thread->mutex_head;
    OsMutex *next = mutex->next;
    if (!next) thread->mutex_tail = 0;
    else next->previous = 0;
    thread->mutex_head = next;
    return mutex;
}
